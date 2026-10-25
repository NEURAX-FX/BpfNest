#include "bpfnest/elf.hpp"
#include <elf.h>
#include <cstring>
#include <limits>

namespace bpfnest {
namespace {
bool add(std::uintptr_t a,std::uint64_t b,std::uintptr_t* out) {
    if (b>std::numeric_limits<std::uintptr_t>::max()-a) return false;
    *out=a+b; return true;
}
struct Reader {
    const Memory& memory;
    std::uintptr_t base;
    Elf64_Phdr phdrs[32]{};
    unsigned count{};
    bool raw(std::uint64_t va,void* out,std::size_t size) const {
        std::uintptr_t address;
        return add(base,va,&address) && memory.read && memory.read(memory.opaque,out,address,size);
    }
    bool contains(std::uint64_t va,std::size_t size,unsigned flags=PF_R) const {
        for(unsigned i=0;i<count;++i) {
            const auto& p=phdrs[i];
            if(p.p_type==PT_LOAD && (p.p_flags&flags)==flags && va>=p.p_vaddr &&
               va-p.p_vaddr<=p.p_memsz && size<=p.p_memsz-(va-p.p_vaddr)) return true;
        }
        return false;
    }
    bool get(std::uint64_t va,void* out,std::size_t size,unsigned flags=PF_R) const {
        return contains(va,size,flags) && raw(va,out,size);
    }
};
}
bool validate_target(const Memory& memory,std::uintptr_t base,std::uintptr_t function,
                     std::size_t function_size,Target* target) noexcept {
    if(!target || !base || !memory.read || function<base || function-base!=0x52d0 || function_size!=0x20c) return false;
    Reader reader{memory,base};
    Elf64_Ehdr header{};
    if(!reader.raw(0,&header,sizeof(header)) || std::memcmp(header.e_ident,ELFMAG,SELFMAG) ||
       header.e_ident[EI_CLASS]!=ELFCLASS64 || header.e_ident[EI_DATA]!=ELFDATA2LSB ||
       header.e_ident[EI_VERSION]!=EV_CURRENT || header.e_version!=EV_CURRENT ||
       header.e_machine!=EM_AARCH64 || header.e_type!=ET_DYN || header.e_ehsize!=sizeof(header) ||
       header.e_phentsize!=sizeof(Elf64_Phdr) || !header.e_phnum || header.e_phnum>32 ||
       header.e_phoff>4096 || !reader.raw(header.e_phoff,reader.phdrs,header.e_phnum*sizeof(Elf64_Phdr))) return false;
    reader.count=header.e_phnum;
    if(!reader.contains(0,sizeof(header)) || !reader.contains(header.e_phoff,header.e_phnum*sizeof(Elf64_Phdr)) ||
       !reader.contains(0x52d0,0x20c,PF_R|PF_X) || !reader.contains(0xac70,16,PF_R|PF_X) ||
       !reader.contains(0xc478,8,PF_R|PF_W)) return false;
    bool identity=false,relro=false;
    std::uint64_t dynamic_va=0,dynamic_size=0;
    constexpr unsigned char build_id[]={0xd1,0x14,0x50,0x9a,0xc2,0x0a,0xb4,0x03,0x7e,0xd5,0x99,0x23,0xcf,0xa0,0x32,0xd5};
    for(unsigned i=0;i<reader.count;++i) {
        const auto& p=reader.phdrs[i];
        if(p.p_type==PT_GNU_RELRO && p.p_vaddr<=0xc478 && p.p_memsz>=8 && 0xc478-p.p_vaddr<=p.p_memsz-8) relro=true;
        if(p.p_type==PT_DYNAMIC) { dynamic_va=p.p_vaddr; dynamic_size=p.p_memsz; }
        if(p.p_type!=PT_NOTE || p.p_filesz>4096) continue;
        for(std::size_t off=0;off+sizeof(Elf64_Nhdr)<=p.p_filesz;) {
            Elf64_Nhdr note{};
            if(!reader.get(p.p_vaddr+off,&note,sizeof(note))) return false;
            if(note.n_namesz>4096 || note.n_descsz>4096) return false;
            const std::size_t names=(note.n_namesz+3u)&~3u, descs=(note.n_descsz+3u)&~3u;
            if(names+descs>p.p_filesz-off-sizeof(note)) return false;
            if(note.n_type==NT_GNU_BUILD_ID && note.n_namesz==4 && note.n_descsz==sizeof(build_id)) {
                char name[4]; unsigned char id[sizeof(build_id)];
                if(!reader.get(p.p_vaddr+off+sizeof(note),name,4) ||
                   !reader.get(p.p_vaddr+off+sizeof(note)+names,id,sizeof(id))) return false;
                if(!std::memcmp(name,"GNU",4) && !std::memcmp(id,build_id,sizeof(id))) identity=true;
            }
            off+=sizeof(note)+names+descs;
        }
    }
    if(!identity || !relro || !dynamic_va || dynamic_size>8192 || dynamic_size%sizeof(Elf64_Dyn)) return false;
    std::uint64_t relas=0,rela_size=0,symbols=0,strings=0,string_size=0;
    bool now=false,rela_format=false,terminated=false;
    for(std::size_t off=0;off<dynamic_size;off+=sizeof(Elf64_Dyn)) {
        Elf64_Dyn d{}; if(!reader.get(dynamic_va+off,&d,sizeof(d))) return false;
        if(d.d_tag==DT_NULL) { terminated=true; break; }
        switch(d.d_tag) {
            case DT_JMPREL: relas=d.d_un.d_ptr; break;
            case DT_PLTRELSZ: rela_size=d.d_un.d_val; break;
            case DT_PLTREL: rela_format=d.d_un.d_val==DT_RELA; break;
            case DT_SYMTAB: symbols=d.d_un.d_ptr; break;
            case DT_STRTAB: strings=d.d_un.d_ptr; break;
            case DT_STRSZ: string_size=d.d_un.d_val; break;
            case DT_BIND_NOW: now=true; break;
            case DT_FLAGS: now=now || (d.d_un.d_val&DF_BIND_NOW); break;
            case DT_FLAGS_1: now=now || (d.d_un.d_val&DF_1_NOW); break;
            default: break;
        }
    }
    if(!terminated || !now || !rela_format || !relas || !symbols || !strings ||
       !rela_size || rela_size>32768 || rela_size%sizeof(Elf64_Rela)) return false;
    bool syscall_relocation=false;
    for(std::size_t off=0;off<rela_size;off+=sizeof(Elf64_Rela)) {
        Elf64_Rela r{}; if(!reader.get(relas+off,&r,sizeof(r))) return false;
        if(r.r_offset!=0xc478) continue;
        if(ELF64_R_TYPE(r.r_info)!=R_AARCH64_JUMP_SLOT || r.r_addend) return false;
        const auto index=ELF64_R_SYM(r.r_info);
        if(index>4096) return false;
        Elf64_Sym s{}; char name[8];
        if(!reader.get(symbols+index*sizeof(s),&s,sizeof(s)) || s.st_name>string_size ||
           sizeof(name)>string_size-s.st_name || !reader.get(strings+s.st_name,name,sizeof(name)) ||
           std::memcmp(name,"syscall",8) || s.st_shndx!=SHN_UNDEF) return false;
        syscall_relocation=true;
    }
    if(!syscall_relocation) return false;
    // Exact bounded sequence, independently recovered from the supplied ELF. No unknown-layout fallback.
    constexpr std::uint32_t expected[]={0x910083e2,0x52802300,0x52800101,0x52801303,
        0xf9004b1f,0x290423e9,0x29057ff5,0xad038300,0xad0183e0,0xad0283e0,0xad0383e0,0x94001633};
    std::uint32_t instructions[12];
    constexpr std::uint32_t expected_plt[]={0xd0000010,0xf9423e11,0x9111e210,0xd61f0220};
    std::uint32_t plt[4];
    if(!reader.get(0x5378,instructions,sizeof(instructions),PF_R|PF_X) ||
       std::memcmp(instructions,expected,sizeof(expected)) ||
       !reader.get(0xac70,plt,sizeof(plt),PF_R|PF_X) || std::memcmp(plt,expected_plt,sizeof(plt))) return false;
    std::uintptr_t lr,got;
    if(!add(base,0x53a8,&lr) || !add(base,0xc478,&got)) return false;
    *target={base,function,lr,got,function_size};
    return true;
}
} // namespace bpfnest

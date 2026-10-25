#include "bpfnest/elf.hpp"
#include <elf.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <vector>
using namespace bpfnest;
#define CHECK(x) do { if(!(x)){ std::fprintf(stderr,"FAIL line %d: %s\n",__LINE__,#x); std::exit(1); } } while(0)
static constexpr std::uintptr_t base=0x7000000000;
struct Image {
    std::vector<unsigned char> bytes;
    explicit Image(const char* path) {
        std::ifstream f(path,std::ios::binary); CHECK(f.good());
        bytes.assign(std::istreambuf_iterator<char>(f),{});
    }
    static bool read(void* p,void* dst,std::uintptr_t address,std::size_t n) {
        auto& image=*static_cast<Image*>(p);
        if(address<base || address-base>image.bytes.size() || n>image.bytes.size()-(address-base)) return false;
        std::memcpy(dst,image.bytes.data()+address-base,n); return true;
    }
    bool valid(std::uintptr_t function=base+0x52d0,std::size_t size=0x20c) {
        Target target{};
        const bool ok=validate_target({this,read},base,function,size,&target);
        if(ok) CHECK(target.base==base && target.function==base+0x52d0 && target.caller_lr==base+0x53a8 && target.got==base+0xc478);
        return ok;
    }
};
int main(int argc,char** argv) {
    CHECK(argc==2);
    Image original(argv[1]); CHECK(original.valid());
    CHECK(!original.valid(base+0x52d4)); CHECK(!original.valid(base+0x52d0,0x208));
    for(auto offset:{0u,4u,5u,18u,0x53a4u,0x5390u,0x5380u,0xac70u,0xac74u,0xac7cu}) {
        Image broken=original; broken.bytes[offset]^=1; CHECK(!broken.valid());
    }
    // BuildID note desc is located using the real note header, not a guessed file offset.
    auto* eh=reinterpret_cast<const Elf64_Ehdr*>(original.bytes.data());
    auto* ph=reinterpret_cast<const Elf64_Phdr*>(original.bytes.data()+eh->e_phoff);
    bool changed=false;
    for(unsigned i=0;i<eh->e_phnum;++i) if(ph[i].p_type==PT_NOTE) {
        for(std::size_t off=ph[i].p_offset;off+12<=ph[i].p_offset+ph[i].p_filesz;) {
            Elf64_Nhdr nh{}; std::memcpy(&nh,original.bytes.data()+off,12);
            std::size_t desc=off+12+((nh.n_namesz+3)&~3u);
            if(nh.n_type==NT_GNU_BUILD_ID && nh.n_namesz==4) {
                Image changed_id=original; changed_id.bytes[desc]^=1; CHECK(!changed_id.valid()); changed=true;
            }
            off=desc+((nh.n_descsz+3)&~3u);
        }
    }
    CHECK(changed);
    Image badph=original; reinterpret_cast<Elf64_Ehdr*>(badph.bytes.data())->e_phnum=65535; CHECK(!badph.valid());
    Image truncated=original; truncated.bytes.resize(64); CHECK(!truncated.valid());
    Image reloc=original; reloc.bytes[0x11e0+22*24+8]^=1; CHECK(!reloc.valid());
    std::puts("elf: real supplied ELF accepted; malformed identity/layout/callsite rejected");
}

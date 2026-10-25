#include "bpfnest/core.h"
#include <cerrno>
#include <cstdarg>
#include <cstdint>
#include <cstring>

using namespace bpfnest;
extern "C" long smoke_call(long, ...);
extern "C" char smoke_return[];
namespace {
int failures=0;
unsigned calls=0, query_calls=0;
int before=0;
bool patched=false;
long wanted=-1;
int wanted_errno=1;
Mode mode=Mode::Observe;
Query attached{};
alignas(8) std::uint8_t attr[152]{};
void check(bool ok) { if (!ok) ++failures; }
long original(long number,...) {
    ++calls;
    before=errno;
    va_list ap; va_start(ap,number);
    const auto cmd=va_arg(ap,std::uint64_t);
    const auto pointer=va_arg(ap,std::uint64_t);
    const auto size=va_arg(ap,std::uint64_t);
    const auto a4=va_arg(ap,std::uint64_t);
    const auto a5=va_arg(ap,std::uint64_t);
    const auto a6=va_arg(ap,std::uint64_t);
    va_end(ap);
    check(number==280 && cmd==8 && size==152 && a4==0x4444 && a5==0x5555 && a6==0x6666);
    check((pointer!=reinterpret_cast<std::uintptr_t>(attr))==patched);
    std::uint32_t flags=0;
    std::memcpy(&flags,reinterpret_cast<const void*>(pointer+12),4);
    check(flags==(patched ? 2u : 0u));
    errno=wanted_errno;
    return wanted;
}
bool read_attr(void*,std::uintptr_t address,void* output,std::size_t size) {
    check(address==reinterpret_cast<std::uintptr_t>(attr));
    std::memcpy(output,attr,size); errno=5; return true;
}
bool target(void*,int fd) { check(fd==41); errno=9; return true; }
bool query(void*,int fd,std::uint32_t type,Query* out) { ++query_calls; check(fd==41 && type==0); *out=attached; errno=13; return true; }
long invoke(void*,const std::uint64_t* a) { return original(static_cast<long>(a[0]),a[1],a[2],a[3],a[4],a[5],a[6]); }
void report(void*,const Event& event) { check(event.result==wanted && event.error==wanted_errno); errno=33; }
void run(Mode next, bool patch, Query q,long result,int error) {
    mode=next; patched=patch; attached=q; wanted=result; wanted_errno=error; before=0;
    errno=4;
    const long actual=smoke_call(280,8,reinterpret_cast<std::uintptr_t>(attr),152,0x4444,0x5555,0x6666);
    check(actual==wanted); check(errno==wanted_errno); check(before==4);
    std::uint32_t flags; std::memcpy(&flags,attr+12,4); check(flags==0);
}
}
extern "C" unsigned smoke_tls_accesses;
extern "C" long bpfnest_dispatch_frame(const Frame* frame) {
    check(reinterpret_cast<std::uintptr_t>(frame)%16==0);
    check(frame->lr==reinterpret_cast<std::uintptr_t>(smoke_return));
    const Operations operations{nullptr,read_attr,target,query,invoke,report,
        [](void*,int fd) { return fd; }, [](void*,int) { errno=9; }};
    return dispatch(*frame,{mode,reinterpret_cast<std::uintptr_t>(smoke_return)},operations);
}
extern "C" int smoke_main() {
    const std::uint32_t fields[]{41,42,0,0}; std::memcpy(attr,fields,sizeof(fields));
    run(Mode::Observe,false,{0,0},-1,1); check(query_calls==0);
    run(Mode::Patch,true,{0,0},-1,1);
    run(Mode::Patch,true,{0,0},123,0);
    run(Mode::Patch,false,{1,2},-1,22);
    run(Mode::Disabled,false,{0,0},0,0);
    check(calls==5 && query_calls==3);
    const Operations passthrough{nullptr,nullptr,nullptr,nullptr,
        [](void*,const std::uint64_t* args) { check(errno==4); check(args[4]==44 && args[5]==55 && args[6]==66); errno=22; return -1L; },nullptr};
    const State state{Mode::Patch,0x53a8};
    const Frame frames[]{
        {{280,8,1,152,44,55,66},0x53a8},
        {{172,8,1,152,44,55,66},0x53a8},
        {{280,7,1,152,44,55,66},0x53a8},
        {{280,8,1,152,44,55,66},0x53ac}};
    for(unsigned i=0;i<4;++i) {
        const auto before_tls=smoke_tls_accesses;
        errno=4;
        check(dispatch(frames[i],i==0 ? State{Mode::Disabled,0x53a8} : state,passthrough)==-1);
        check(errno==22 && smoke_tls_accesses==before_tls);
    }
    return failures==0 ? 0 : 1;
}

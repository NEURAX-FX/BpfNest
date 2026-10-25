#include "bpfnest/core.h"
#include <cerrno>
#include <cstdio>
#include <cstdlib>
#include <cstring>
using namespace bpfnest;
#define CHECK(x) do{if(!(x)){std::fprintf(stderr,"policy-safety:%d: %s\n",__LINE__,#x);std::exit(1);}}while(0)
struct Fixture {
    alignas(8) unsigned char attr[152]{};
    Frame frame{{280,8,reinterpret_cast<std::uintptr_t>(attr),152,44,55,66},0x53a8};
    Query state{0,0}; bool patched{}; unsigned calls{},queries{};
    Fixture(){const std::uint32_t fields[]{17,23,0,0};std::memcpy(attr,fields,sizeof(fields));}
    void run(){
        const Operations ops{this,
            [](void*,std::uintptr_t p,void* out,std::size_t n){std::memcpy(out,reinterpret_cast<void*>(p),n);errno=EFAULT;return true;},
            [](void*,int fd){return fd==17;},
            [](void* p,int fd,std::uint32_t t,Query* out){auto& f=*static_cast<Fixture*>(p);CHECK(fd==17&&t==0);++f.queries;*out=f.state;errno=EPERM;return true;},
            [](void* p,const std::uint64_t* args){auto& f=*static_cast<Fixture*>(p);++f.calls;CHECK(errno==EDOM);f.patched=args[2]!=f.frame.args[2];if(f.patched){std::uint32_t fields[4];std::memcpy(fields,reinterpret_cast<void*>(args[2]),16);CHECK(fields[3]==2);}errno=EDOM;return 91L;},
            [](void*,const Event& e){CHECK(e.result==91&&e.error==EDOM);errno=EINTR;},
            [](void*,int fd){return fd;},[](void*,int){errno=EBADF;}};
        errno=EDOM;CHECK(dispatch(frame,{Mode::Patch,0x53a8},ops)==91);CHECK(errno==EDOM&&calls==1);
    }
};
int main(){
    // EVERY nonempty direct attachment is refused, including otherwise-compatible MULTI.
    {Fixture f;f.state={3,2};f.run();CHECK(!f.patched&&f.queries==1);}
    for(auto state:{Query{1,0},Query{1,1},Query{1,3},Query{1,4},Query{0,8}}){Fixture f;f.state=state;f.run();CHECK(!f.patched);}
    // Unsupported tail fields are not silently changed, even at the last bounded byte.
    for(unsigned byte:{16u,23u,151u}){Fixture f;f.attr[byte]=1;f.run();CHECK(!f.patched&&f.queries==0);}
    // Successful original operations preserve their prior errno despite read/query/close/report.
    {Fixture f;f.run();CHECK(f.patched);}
    std::puts("policy-safety: all nonempty direct attachments refused, unsafe tail and success errno passed");
}

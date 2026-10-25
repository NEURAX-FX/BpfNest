#include "bpfnest/core.h"
#include <cerrno>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <thread>
#include <atomic>
using namespace bpfnest;
#define CHECK(x) do{if(!(x)){std::fprintf(stderr,"reentrancy:%d: %s\n",__LINE__,#x);std::exit(1);}}while(0)
struct Fixture {
    alignas(8) unsigned char attr[152]{};
    Frame frame{{280,8,reinterpret_cast<std::uintptr_t>(attr),152,44,55,66},0x53a8};
    State state{Mode::Patch,0x53a8};
    unsigned reads{},queries{},calls{};
    bool recurse{},first_original{};
    std::atomic<unsigned>* barrier{};
    Fixture(){const std::uint32_t fields[]{17,23,0,0};std::memcpy(attr,fields,16);}
    Operations ops(){return {this,
        [](void* p,std::uintptr_t address,void* output,std::size_t size){
            auto& f=*static_cast<Fixture*>(p);++f.reads;
            if(f.barrier){f.barrier->fetch_add(1);while(f.barrier->load()<2)std::this_thread::yield();}
            if(f.recurse){f.recurse=false;CHECK(dispatch(f.frame,f.state,f.ops())==42);}
            std::memcpy(output,reinterpret_cast<void*>(address),size);errno=EFAULT;return true;},
        [](void*,int fd){return fd==17;},
        [](void* p,int,std::uint32_t,Query* out){++static_cast<Fixture*>(p)->queries;*out={0,0};return true;},
        [](void* p,const std::uint64_t* args){auto& f=*static_cast<Fixture*>(p);if(f.calls++==0)f.first_original=args[2]==f.frame.args[2];errno=EDOM;return 42L;},
        nullptr,[](void*,int fd){return fd;},[](void*,int){errno=EBADF;}};}
    void run(){errno=EDOM;CHECK(dispatch(frame,state,ops())==42);CHECK(errno==EDOM);}
};
int main(){
    {Fixture f;f.recurse=true;f.run();CHECK(f.calls==2);CHECK(f.reads==1&&f.queries==1&&f.first_original);}
    // A global guard would incorrectly suppress a second thread's independent candidate.
    std::atomic<unsigned> barrier{0};Fixture a,b;a.barrier=&barrier;b.barrier=&barrier;
    std::thread one([&]{a.run();});std::thread two([&]{b.run();});one.join();two.join();
    CHECK(a.calls==1&&b.calls==1&&a.queries==1&&b.queries==1);
    CHECK(!a.first_original&&!b.first_original);
    std::puts("reentrancy: recursive same-thread passthrough and independent concurrent candidates passed");
}

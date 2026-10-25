#include "bpfnest/core.h"
#include <cerrno>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
using namespace bpfnest;
#define CHECK(x) do { if(!(x)) { std::fprintf(stderr,"fd:%d: %s\n",__LINE__,#x); std::exit(1); } } while(0)
struct Fixture {
    int target, program, pins[2]{-1,-1}, acquired=0,released=0,queries=0;
    struct stat target_stat{},program_stat{};
    alignas(8) unsigned char attr[152]{};
    bool fail_second=false, expect_patch=true;
    static bool same(int fd,const struct stat& expected) { struct stat actual{}; return fstat(fd,&actual)==0 && actual.st_dev==expected.st_dev && actual.st_ino==expected.st_ino; }
    Fixture() {
        target=open("/dev/null",O_RDONLY|O_CLOEXEC); program=open("/dev/zero",O_RDONLY|O_CLOEXEC); CHECK(target>=0 && program>=0);
        CHECK(fstat(target,&target_stat)==0 && fstat(program,&program_stat)==0);
        std::uint32_t fields[]{static_cast<std::uint32_t>(target),static_cast<std::uint32_t>(program),0,0}; std::memcpy(attr,fields,sizeof(fields));
    }
    ~Fixture() { close(target); close(program); }
    void run() {
        Operations operations{this,
            [](void*,std::uintptr_t p,void* out,std::size_t n) { std::memcpy(out,reinterpret_cast<void*>(p),n); return true; },
            [](void* p,int fd) { auto& f=*static_cast<Fixture*>(p); CHECK(fd!=f.target); return same(fd,f.target_stat); },
            [](void* p,int fd,std::uint32_t type,Query* q) { auto& f=*static_cast<Fixture*>(p); ++f.queries; CHECK(type==0 && same(fd,f.target_stat));
                // Close and reuse BOTH original numeric fds after validation.
                CHECK(dup2(f.program,f.target)==f.target); CHECK(!same(f.target,f.target_stat));
                CHECK(dup2(f.target,f.program)==f.program); *q={0,0}; return true; },
            [](void* p,const std::uint64_t* a) { auto& f=*static_cast<Fixture*>(p); CHECK(errno==EINTR);
                if(f.expect_patch) { std::uint32_t fields[4]; std::memcpy(fields,reinterpret_cast<void*>(a[2]),sizeof(fields));
                    CHECK(fields[0]==static_cast<unsigned>(f.pins[0]) && fields[1]==static_cast<unsigned>(f.pins[1]) && fields[3]==2);
                    CHECK(same(fields[0],f.target_stat)); CHECK(same(fields[1],f.program_stat)); }
                else CHECK(a[2]==reinterpret_cast<std::uintptr_t>(f.attr));
                errno=EINVAL; return -1L; },
            [](void* p,const Event&) { auto& f=*static_cast<Fixture*>(p); CHECK(f.released==f.acquired); errno=EDOM; },
            [](void* p,int fd) { auto& f=*static_cast<Fixture*>(p); if(f.fail_second && f.acquired==1) return -1; int pin=fcntl(fd,F_DUPFD_CLOEXEC,3); CHECK(pin>=0); f.pins[f.acquired++]=pin; return pin; },
            [](void* p,int fd) { auto& f=*static_cast<Fixture*>(p); CHECK(close(fd)==0); ++f.released; errno=EBADF; }};
        Frame frame{{280,8,reinterpret_cast<std::uintptr_t>(attr),152,44,55,66},0x53a8};
        errno=EINTR; CHECK(dispatch(frame,{Mode::Patch,0x53a8},operations)==-1); CHECK(errno==EINVAL);
        CHECK(acquired==released); for(int i=0;i<acquired;++i) { CHECK(fcntl(pins[i],F_GETFD)==-1); CHECK(errno==EBADF); }
        std::uint32_t flags; std::memcpy(&flags,attr+12,4); CHECK(flags==0);
    }
};
int main() { { Fixture f; f.run(); CHECK(f.queries==1 && f.acquired==2); } { Fixture f; f.fail_second=true; f.expect_patch=false; f.run(); CHECK(f.queries==0 && f.acquired==1); } std::puts("fd: real close/reuse stable target+program pins, partial pin failure and cleanup passed"); }

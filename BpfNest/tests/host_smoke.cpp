#include "bpfnest/core.h"
#include <cerrno>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/vfs.h>
#include <sys/syscall.h>
#include <unistd.h>
using namespace bpfnest;
#define CHECK(x) do { if(!(x)){std::fprintf(stderr,"host-smoke:%d: %s errno=%d\n",__LINE__,#x,errno);std::exit(1);} } while(0)
struct Platform {
    int mem{-1},maps{-1},parent{-1};
    struct stat expected{};
    unsigned patched{},reports{},queries{};
};
static bool read_attr(void* p,std::uintptr_t address,void* out,std::size_t n) {
    auto& platform=*static_cast<Platform*>(p);
    return read_readable_memory(platform.mem,platform.maps,address,out,n);
}
static bool same_target(void* p,int fd) {
    auto& platform=*static_cast<Platform*>(p);
    struct stat st{};struct statfs fs{};
    return fstat(fd,&st)==0 && S_ISDIR(st.st_mode) && fstatfs(fd,&fs)==0 &&
        fs.f_type==0x63677270 && st.st_dev==platform.expected.st_dev && st.st_ino==platform.expected.st_ino;
}
static bool query(void* p,int fd,std::uint32_t type,Query* output) {
    auto& platform=*static_cast<Platform*>(p);
    CHECK(same_target(p,fd));CHECK(type<=1);++platform.queries;
    // Privileged BPF query is the sole external-policy double in this host smoke.
    *output={0,0};errno=EPERM;return true;
}
static long invoke(void* p,const std::uint64_t* args) {
    auto& platform=*static_cast<Platform*>(p);
    if(args[0]!=280 || args[1]!=8)
        return syscall(static_cast<long>(args[0]),args[1],args[2],args[3],args[4],args[5],args[6]);
    const int input_errno=errno;
    CHECK(args[3]==152 && args[4]==44 && args[5]==55 && args[6]==66);
    std::uint32_t fields[4];std::memcpy(fields,reinterpret_cast<const void*>(args[2]),sizeof(fields));
    CHECK(fields[2]==1 && fields[3]==2 && input_errno==EDOM);
    CHECK(same_target(p,static_cast<int>(fields[0])));
    CHECK(fcntl(static_cast<int>(fields[1]),F_GETFD)>=0);
    ++platform.patched;errno=EDOM;return 42;
}
static void report(void* p,const Event& event) {
    auto& platform=*static_cast<Platform*>(p);++platform.reports;
    CHECK(event.decision==Decision::Patched && event.result==42 && event.error==EDOM);
    errno=EINTR;
}
int main() {
    Platform platform;
    platform.mem=open("/proc/self/mem",O_RDONLY|O_CLOEXEC);
    platform.maps=open("/proc/self/maps",O_RDONLY|O_CLOEXEC);
    platform.parent=open("/sys/fs/cgroup",O_RDONLY|O_DIRECTORY|O_CLOEXEC);
    CHECK(platform.mem>=0 && platform.maps>=0 && platform.parent>=0);
    CHECK(fstat(platform.parent,&platform.expected)==0);CHECK(same_target(&platform,platform.parent));
    int program=open("/dev/null",O_RDONLY|O_CLOEXEC);CHECK(program>=0);
    const Operations ops{&platform,read_attr,same_target,query,invoke,report,
        [](void*,int fd){return fcntl(fd,F_DUPFD_CLOEXEC,0);},[](void*,int fd){close(fd);errno=EBADF;}};
    alignas(8) unsigned char attr[152]{};
    std::uint32_t fields[]{static_cast<std::uint32_t>(platform.parent),static_cast<std::uint32_t>(program),1,0};
    std::memcpy(attr,fields,sizeof(fields));
    Frame attach{{280,8,reinterpret_cast<std::uintptr_t>(attr),152,44,55,66},0x53a8};
    const State patch{Mode::Patch,0x53a8};errno=EDOM;
    CHECK(dispatch(attach,patch,ops)==42 && errno==EDOM);
    std::uint32_t unchanged[4];std::memcpy(unchanged,attr,sizeof(unchanged));CHECK(unchanged[0]==fields[0] && unchanged[1]==fields[1] && unchanged[3]==0);
    CHECK(platform.patched==1 && platform.reports==1 && platform.queries==1);
    Frame getpid_frame{{SYS_getpid,1,2,3,4,5,6},0};errno=EDOM;
    CHECK(dispatch(getpid_frame,patch,ops)==getpid() && errno==EDOM);
    Frame invalid{{0x7fffffff,1,2,3,4,5,6},0};
    CHECK(dispatch(invalid,patch,ops)==-1 && errno==ENOSYS);
    int wrong=open("/tmp",O_RDONLY|O_DIRECTORY|O_CLOEXEC);CHECK(wrong>=0);CHECK(!same_target(&platform,wrong));close(wrong);
    CHECK(close(program)==0);CHECK(close(platform.parent)==0);CHECK(close(platform.mem)==0);CHECK(close(platform.maps)==0);
    std::puts("host-smoke: real cgroup2 identity/descriptor pins + safe self-read + changed dispatch + original-buffer/errno semantics passed");
    std::puts("host-smoke: real libc getpid and ENOSYS passthrough passed; privileged BPF query/attach not executed");
}

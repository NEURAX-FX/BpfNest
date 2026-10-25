#include "bpfnest/install.h"
#include "bpfnest/elf.hpp"
#include <android/log.h>
#include <cerrno>
#include <climits>
#include <cstring>
#include <dlfcn.h>
#include <fcntl.h>
#include <linux/bpf.h>
#include <sys/stat.h>
#include <sys/vfs.h>
#include <unistd.h>

namespace {
using namespace bpfnest;
using Syscall = long (*)(long, ...);
constexpr long kCgroup2Magic = 0x63677270;
struct Runtime {
    Installation installation;
    int memory_fd = -1;
    int maps_fd = -1;
    int parent_fd = -1;
    dev_t parent_dev = 0;
    ino_t parent_ino = 0;
};
Runtime runtime;
extern "C" long bpfnest_syscall_entry(long, ...);

bool is_netd(void*) {
    char executable[PATH_MAX];
    const auto n = readlink("/proc/self/exe", executable, sizeof(executable));
    constexpr char expected[] = "/system/bin/netd";
    return n == sizeof(expected)-1 && std::memcmp(executable, expected, sizeof(expected)-1) == 0;
}
Mode load_config(void*) {
    Dl_info own{};
    if (!dladdr(reinterpret_cast<void*>(load_config), &own) || !own.dli_fname) return Mode::Disabled;
    const auto size = std::strlen(own.dli_fname);
    if (size >= PATH_MAX) return Mode::Disabled;
    char path[PATH_MAX];
    std::memcpy(path, own.dli_fname, size+1);
    char* slash = std::strrchr(path, '/');
    if (!slash) return Mode::Disabled;
    *slash = '\0';
    const int directory = open(path, O_RDONLY|O_DIRECTORY|O_CLOEXEC|O_NOFOLLOW);
    if (directory < 0) return Mode::Disabled;
    const int fd = openat(directory, "bpfnest.conf", O_RDONLY|O_CLOEXEC|O_NOFOLLOW);
    const int error = errno;
    close(directory);
    if (fd < 0) return error == ENOENT ? Mode::Observe : Mode::Disabled;
    struct stat st{};
    char data[4097];
    ssize_t count = -1;
    if (fstat(fd, &st) == 0 && S_ISREG(st.st_mode) && st.st_size >= 0 && st.st_size <= 4096) {
        do { count = read(fd, data, sizeof(data)); } while (count < 0 && errno == EINTR);
    }
    close(fd);
    return count >= 0 && count <= 4096 ? parse_config({data, static_cast<std::size_t>(count)}) : Mode::Disabled;
}
bool read_self(std::uintptr_t address, void* out, std::size_t size) {
    return read_memory(runtime.memory_fd, address, out, size);
}
bool certify(void*, void* base_ptr, void* symbol, std::size_t symbol_size) {
    runtime.memory_fd = open("/proc/self/mem", O_RDONLY|O_CLOEXEC);
    runtime.maps_fd = open("/proc/self/maps", O_RDONLY|O_CLOEXEC);
    if (runtime.memory_fd < 0 || runtime.maps_fd < 0) return false;
    runtime.parent_fd = open("/sys/fs/cgroup", O_RDONLY|O_DIRECTORY|O_CLOEXEC|O_NOFOLLOW);
    struct stat st{}; struct statfs fs{};
    if (runtime.parent_fd < 0 || fstat(runtime.parent_fd,&st) != 0 || !S_ISDIR(st.st_mode) ||
        fstatfs(runtime.parent_fd,&fs) != 0 || fs.f_type != kCgroup2Magic) return false;
    runtime.parent_dev=st.st_dev; runtime.parent_ino=st.st_ino;
    const Memory memory{nullptr,[](void*,void* output,std::uintptr_t address,std::size_t size) {
        if (size > 8192 || address > UINTPTR_MAX-size) return false;
        auto* bytes=static_cast<std::uint8_t*>(output);
        for (std::size_t offset=0;offset<size;) {
            const auto count=size-offset > kMaxAttr ? kMaxAttr : size-offset;
            if (!read_self(address+offset,bytes+offset,count)) return false;
            offset+=count;
        }
        return true;
    }};
    Target target{};
    return validate_target(memory,reinterpret_cast<std::uintptr_t>(base_ptr),
        reinterpret_cast<std::uintptr_t>(symbol),symbol_size,&target);
}
bool original_slot(void*,void* base,void** slot,void** genuine) {
    if (!read_self(reinterpret_cast<std::uintptr_t>(base)+kGotOffset,slot,sizeof(*slot))) return false;
    *genuine=dlsym(RTLD_DEFAULT,"syscall");
    Dl_info info{};
    if (!*genuine || !dladdr(*genuine,&info) || !info.dli_fname) return false;
    const char* basename=std::strrchr(info.dli_fname,'/');
    basename=basename ? basename+1 : info.dli_fname;
    return std::strcmp(basename,"libc.so")==0;
}
bool copy_attr(void*,std::uintptr_t address,void* out,std::size_t size) {
    return read_readable_memory(runtime.memory_fd,runtime.maps_fd,address,out,size);
}
bool same_target(void*,int fd) {
    struct stat st{}; struct statfs fs{};
    return fstat(fd,&st)==0 && S_ISDIR(st.st_mode) && fstatfs(fd,&fs)==0 && fs.f_type==kCgroup2Magic &&
        st.st_dev==runtime.parent_dev && st.st_ino==runtime.parent_ino;
}
long invoke(void*,const std::uint64_t* a) {
    auto original=reinterpret_cast<Syscall>(runtime.installation.original);
    return original(static_cast<long>(a[0]),a[1],a[2],a[3],a[4],a[5],a[6]);
}
bool query(void*,int fd,std::uint32_t type,Query* result) {
    union bpf_attr attr{};
    attr.query.target_fd=static_cast<std::uint32_t>(fd);
    attr.query.attach_type=type;
    // Direct, count-only query: never QUERY_EFFECTIVE for attach-mode decisions.
    const std::uint64_t args[]{280,BPF_PROG_QUERY,reinterpret_cast<std::uintptr_t>(&attr),sizeof(attr),0,0,0};
    if (invoke(nullptr,args)!=0) return false;
    *result={attr.query.prog_cnt,attr.query.attach_flags};
    return true;
}
const char* decision_name(Decision d) {
    switch(d) {
    case Decision::Observed: return "observe-eligible";
    case Decision::Patched: return "patch-empty-direct";
    case Decision::QueryFailed: return "refuse-query-failed";
    default: return "refuse-nonempty-or-flags";
    }
}
void report(void*,const Event& e) {
    __android_log_print(ANDROID_LOG_INFO,"BpfNest","%s target_fd=%u type=%u flags=%u->%u result=%ld errno=%d",
        decision_name(e.decision),e.target_fd,e.attach_type,e.original_flags,e.forwarded_flags,e.result,e.error);
}
int pin(void*,int fd) { return fcntl(fd,F_DUPFD_CLOEXEC,3); }
void release(void*,int fd) { close(fd); }
const Operations operations{nullptr,copy_attr,same_target,query,invoke,report,pin,release};
void on_loaded(void*,const ZygiskNextAPI* api) {
    const int saved=errno;
    const InstallOps ops{nullptr,is_netd,load_config,certify,original_slot,reinterpret_cast<void*>(bpfnest_syscall_entry)};
    const auto result=install(runtime.installation,api,ops);
    __android_log_print(ANDROID_LOG_INFO,"BpfNest","module loaded api_target=4 mode=%s install_result=%d (0=installed; device attach not proven)",mode_name(runtime.installation.state.mode),static_cast<int>(result));
    errno=saved;
}
} // namespace
extern "C" __attribute__((visibility("hidden"))) long bpfnest_dispatch_frame(const bpfnest::Frame* frame) {
    return bpfnest::dispatch(*frame,bpfnest::active_state(runtime.installation),operations);
}
extern "C" {
__attribute__((visibility("default"))) ZygiskNextModule zn_module{ZYGISK_NEXT_API_VERSION,on_loaded};
}

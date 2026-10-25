#include "bpfnest/core.h"
#include <cerrno>
#include <cstring>
#include <cstdio>
#include <limits>
#include <cinttypes>
#include <unistd.h>

namespace bpfnest {
Mode parse_config(std::string_view text, bool missing) {
    if (missing) return Mode::Observe;
    bool found = false;
    Mode result = Mode::Disabled;
    while (!text.empty()) {
        const auto end = text.find('\n');
        auto line = text.substr(0, end);
        text = end == text.npos ? std::string_view{} : text.substr(end + 1);
        if (line.empty() || line.front() == '#') continue;
        if (found) return Mode::Disabled;
        found = true;
        if (line == "mode=observe") result = Mode::Observe;
        else if (line == "mode=patch") result = Mode::Patch;
        else if (line == "mode=disabled") result = Mode::Disabled;
        else return Mode::Disabled;
    }
    return result;
}
const char* mode_name(Mode mode) {
    switch (mode) { case Mode::Observe: return "observe"; case Mode::Patch: return "patch"; default: return "disabled"; }
}
bool compatible_query(const Query& query) { return query.count == 0 && query.flags == 0; }
bool certify_image(std::uintptr_t base, std::uintptr_t symbol, std::size_t size,
                   const std::uint8_t* id, std::size_t id_size,
                   const std::uint32_t* words, std::size_t count) {
    constexpr std::uint32_t expected[]{0x52802300,0x52800101,0x52801303,0xf9004b1f,
        0x290423e9,0x29057ff5,0xad038300,0xad0183e0,0xad0283e0,0xad0383e0,0x94001633};
    return base != 0 && base <= UINTPTR_MAX - kGotOffset - sizeof(void*) &&
        symbol == base + kSymbolOffset && size == kSymbolSize && id && words &&
        id_size == sizeof(kBuildId) && count == sizeof(expected)/sizeof(expected[0]) &&
        std::memcmp(id, kBuildId, sizeof(kBuildId)) == 0 &&
        std::memcmp(words, expected, sizeof(expected)) == 0;
}
bool read_memory(int fd, std::uintptr_t address, void* output, std::size_t size) {
    if (fd < 0 || address == 0 || !output || size == 0 || size > kMaxAttr ||
        address > static_cast<std::uintptr_t>(std::numeric_limits<off_t>::max()) - size) return false;
    ssize_t count;
    do { count = pread(fd, output, size, static_cast<off_t>(address)); } while (count < 0 && errno == EINTR);
    return count == static_cast<ssize_t>(size);
}
bool read_readable_memory(int memory_fd, int maps_fd, std::uintptr_t address, void* output, std::size_t size) {
    if (maps_fd < 0 || !address || !size || size > kMaxAttr || address > UINTPTR_MAX-size) return false;
    char chunk[1024], line[512];
    std::size_t length=0;
    bool overflow=false;
    for (off_t offset=0; offset < 1024*1024;) {
        ssize_t count;
        do { count=pread(maps_fd,chunk,sizeof(chunk),offset); } while(count<0 && errno==EINTR);
        if (count<=0) return false;
        offset+=count;
        for (ssize_t i=0;i<count;++i) {
            if (chunk[i]!='\n') {
                if (length+1<sizeof(line)) line[length++]=chunk[i]; else overflow=true;
                continue;
            }
            if (!overflow) {
                line[length]='\0';
                std::uintptr_t start=0,end=0; char permissions[5]{};
                if (std::sscanf(line,"%" SCNxPTR "-%" SCNxPTR " %4s",&start,&end,permissions)==3 &&
                    address>=start && address<end) {
                    return permissions[0]=='r' && size<=end-address && read_memory(memory_fd,address,output,size);
                }
            }
            length=0; overflow=false;
        }
    }
    return false;
}
long dispatch(const Frame& frame, const State& state, const Operations& ops) {
    const int incoming_errno = errno;
    if (state.mode==Mode::Disabled || frame.args[0]!=280 || frame.args[1]!=8 || frame.lr!=state.return_pc) {
        errno=incoming_errno;
        return ops.invoke(ops.context,frame.args);
    }
    static thread_local bool in_dispatch=false;
    if (in_dispatch) { errno=incoming_errno; return ops.invoke(ops.context,frame.args); }
    struct Guard { bool& flag; explicit Guard(bool& value):flag(value) { flag=true; } ~Guard() { flag=false; } } guard(in_dispatch);
    alignas(8) std::uint8_t copy[kMaxAttr];
    std::uint64_t forwarded[7];
    const std::uint64_t* args = frame.args;
    int target_pin=-1, program_pin=-1;
    bool candidate = false;
    Event event{};
    if (frame.args[2] != 0 && frame.args[3] == 152 &&
        ops.read(ops.context, frame.args[2], copy, static_cast<std::size_t>(frame.args[3]))) {
        std::uint32_t fields[4];
        std::memcpy(fields, copy, sizeof(fields));
        bool tail_zero=true;
        for(std::size_t i=16;i<152;++i) if(copy[i]!=0) { tail_zero=false; break; }
        bool stable=false;
        if (tail_zero && fields[0] <= INT32_MAX && fields[1] <= INT32_MAX && fields[2] <= 1 && fields[3] == 0) {
            if (state.mode==Mode::Observe) stable=ops.same_target(ops.context,static_cast<int>(fields[0]));
            else if (ops.pin && ops.release) {
                target_pin=ops.pin(ops.context,static_cast<int>(fields[0]));
                if (target_pin>=0) program_pin=ops.pin(ops.context,static_cast<int>(fields[1]));
                stable=target_pin>=0 && program_pin>=0 && ops.same_target(ops.context,target_pin);
            }
        }
        if (stable) {
            candidate = true;
            event = {Decision::Observed, fields[0], fields[2], fields[3], fields[3], 0, 0};
            if (state.mode == Mode::Patch) {
                Query query{};
                if (!ops.query(ops.context, target_pin, fields[2], &query)) event.decision = Decision::QueryFailed;
                else if (!compatible_query(query)) event.decision = Decision::Incompatible;
                else {
                    const std::uint32_t flags = kAllowMulti;
                    std::memcpy(copy + 12, &flags, sizeof(flags));
                    const std::uint32_t pinned[]{static_cast<std::uint32_t>(target_pin),static_cast<std::uint32_t>(program_pin)};
                    std::memcpy(copy,pinned,sizeof(pinned));
                    std::memcpy(forwarded, frame.args, sizeof(forwarded));
                    forwarded[2] = reinterpret_cast<std::uintptr_t>(copy);
                    args = forwarded;
                    event.decision = Decision::Patched;
                    event.forwarded_flags = flags;
                }
            }
        }
    }
    errno = incoming_errno;
    const long result = ops.invoke(ops.context, args);
    const int outgoing_errno = errno;
    if (program_pin>=0) ops.release(ops.context,program_pin);
    if (target_pin>=0) ops.release(ops.context,target_pin);
    if (candidate && ops.report) {
        event.result = result; event.error = outgoing_errno;
        ops.report(ops.context, event);
    }
    errno = outgoing_errno;
    return result;
}
} // namespace bpfnest

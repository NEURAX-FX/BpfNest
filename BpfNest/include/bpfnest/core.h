#pragma once

#include <cstddef>
#include <cstdint>
#include <string_view>

namespace bpfnest {
enum class Mode { Observe, Patch, Disabled };
Mode parse_config(std::string_view text, bool missing = false);
const char* mode_name(Mode mode);

constexpr std::size_t kMaxAttr = 256;
constexpr std::uint32_t kAllowMulti = 2;
constexpr std::uintptr_t kSymbolOffset = 0x52d0;
constexpr std::size_t kSymbolSize = 0x20c;
constexpr std::uintptr_t kReturnOffset = 0x53a8;
constexpr std::uintptr_t kGotOffset = 0xc478;
inline constexpr char kAttachSymbol[] =
    "_ZN7android3netL21attachProgramToCgroupEPKcRKNS_4base14unique_fd_implINS3_13DefaultCloserEEE15bpf_attach_type";
inline constexpr std::uint8_t kBuildId[] = {
    0xd1, 0x14, 0x50, 0x9a, 0xc2, 0x0a, 0xb4, 0x03,
    0x7e, 0xd5, 0x99, 0x23, 0xcf, 0xa0, 0x32, 0xd5};

struct Frame { std::uint64_t args[7]; std::uintptr_t lr; };
static_assert(sizeof(Frame) == 64);
struct Query { std::uint32_t count; std::uint32_t flags; };
bool compatible_query(const Query& query);
// The words are the complete certified 0x537c..0x53a4 instruction sequence.
bool certify_image(std::uintptr_t base, std::uintptr_t symbol, std::size_t size,
                   const std::uint8_t* build_id, std::size_t build_id_size,
                   const std::uint32_t* words, std::size_t word_count);

struct State { Mode mode; std::uintptr_t return_pc; };
enum class Decision { Observed, Patched, QueryFailed, Incompatible };
struct Event {
    Decision decision;
    std::uint32_t target_fd, attach_type, original_flags, forwarded_flags;
    long result;
    int error;
};
struct Operations {
    void* context;
    bool (*read)(void*, std::uintptr_t, void*, std::size_t);
    bool (*same_target)(void*, int);
    bool (*query)(void*, int, std::uint32_t, Query*);
    long (*invoke)(void*, const std::uint64_t*);
    void (*report)(void*, const Event&);
    int (*pin)(void*, int) = nullptr;
    void (*release)(void*, int) = nullptr;
};
long dispatch(const Frame& frame, const State& state, const Operations& operations);
// pread() through a cached /proc/self/mem descriptor; no direct user-pointer access.
bool read_memory(int fd, std::uintptr_t address, void* output, std::size_t size);
bool read_readable_memory(int memory_fd, int maps_fd, std::uintptr_t address, void* output, std::size_t size);
} // namespace bpfnest

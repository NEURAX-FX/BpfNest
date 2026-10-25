#include "bpfnest/core.h"
#include <array>
#include <cerrno>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>

using namespace bpfnest;
namespace {
int checks;
#define CHECK(x) do { ++checks; if (!(x)) { std::fprintf(stderr, "%s:%d: %s\n", __FILE__, __LINE__, #x); std::exit(1); } } while (0)
struct Fixture {
    alignas(8) std::array<std::uint8_t, 152> attr{};
    std::array<std::uint8_t, 152> forwarded{};
    Frame frame{{280, 8, 0, 152, 0x4444, 0x5555, 0x6666}, 0x100053a8};
    State state{Mode::Patch, 0x100053a8};
    bool readable = true, identity = true, query_ok = true;
    bool reenter=false;
    Query attached{};
    unsigned reads = 0, identities = 0, queries = 0, calls = 0, reports = 0;
    long result = -1;
    int output_errno = EPERM, input_errno = 0;
    std::uint64_t seen[7]{};
    Event event{};
    Fixture() { std::uint32_t values[]{41, 42, 0, 0}; std::memcpy(attr.data(), values, sizeof(values)); frame.args[2] = reinterpret_cast<std::uintptr_t>(attr.data()); }
    void field(unsigned index, std::uint32_t value) { std::memcpy(attr.data() + index * 4, &value, 4); }
    Operations ops() {
        return {this,
            [](void* p, std::uintptr_t a, void* out, std::size_t n) { auto& f = *static_cast<Fixture*>(p); ++f.reads; errno = EIO; if (!f.readable || a != f.frame.args[2] || n > f.attr.size()) return false; std::memcpy(out, f.attr.data(), n); return true; },
            [](void* p, int fd) { auto& f = *static_cast<Fixture*>(p); ++f.identities; CHECK(fd == 41); errno = EIO; return f.identity; },
            [](void* p, int fd, std::uint32_t type, Query* out) { auto& f = *static_cast<Fixture*>(p); ++f.queries; CHECK(fd == 41); CHECK(type <= 1); *out = f.attached; errno = EACCES; return f.query_ok; },
            [](void* p, const std::uint64_t* args) { auto& f = *static_cast<Fixture*>(p); ++f.calls; f.input_errno = errno; std::memcpy(f.seen, args, sizeof(f.seen)); if (args[2] != f.frame.args[2]) std::memcpy(f.forwarded.data(), reinterpret_cast<const void*>(args[2]), f.attr.size()); errno = f.output_errno; return f.result; },
            [](void* p, const Event& e) { auto& f = *static_cast<Fixture*>(p); ++f.reports; f.event = e;
                if(f.reenter) { f.reenter=false; const auto reads=f.reads,queries=f.queries; errno=EINTR; CHECK(dispatch(f.frame,f.state,f.ops())==f.result); CHECK(f.reads==reads && f.queries==queries); CHECK(f.seen[2]==f.frame.args[2]); CHECK(errno==f.output_errno); }
                errno = EDOM; },
            [](void*, int fd) { return fd; }, [](void*, int) { errno = EBADF; }};
    }
    void run() { errno = EINTR; CHECK(dispatch(frame, state, ops()) == result); CHECK(calls == 1); CHECK(input_errno == EINTR); CHECK(errno == output_errno); for (unsigned i : {0u, 1u, 3u, 4u, 5u, 6u}) CHECK(seen[i] == frame.args[i]); }
    void unchanged() { run(); CHECK(seen[2] == frame.args[2]); }
};
void config_tests() {
    CHECK(parse_config("", true) == Mode::Observe);
    CHECK(parse_config("mode=observe\n") == Mode::Observe);
    CHECK(parse_config("# comment\n\nmode=patch\n") == Mode::Patch);
    CHECK(parse_config("mode=disabled") == Mode::Disabled);
    for (auto s : {"", "mode=other", "mode=patch\nmode=patch", "mode=patch\nunknown=yes", "mode =patch", "mode=patch # comment", "mode=Patch", "mode=patch\nmode=observe"}) CHECK(parse_config(s) == Mode::Disabled);
    CHECK(parse_config(std::string_view("mode=patch\0", 11)) == Mode::Disabled);
}
void certification_tests() {
    std::uint32_t words[]{0x52802300,0x52800101,0x52801303,0xf9004b1f,0x290423e9,0x29057ff5,0xad038300,0xad0183e0,0xad0283e0,0xad0383e0,0x94001633};
    const auto valid = [&](std::uintptr_t b, std::uintptr_t s, std::size_t z, const std::uint8_t* id, std::size_t n, std::size_t w) { return certify_image(b,s,z,id,n,words,w); };
    CHECK(valid(0x10000000,0x100052d0,0x20c,kBuildId,sizeof(kBuildId),11));
    CHECK(!valid(0x10000000,0x100052d4,0x20c,kBuildId,sizeof(kBuildId),11));
    CHECK(!valid(0x10000000,0x100052d0,0x208,kBuildId,sizeof(kBuildId),11));
    CHECK(!valid(0x10000000,0x100052d0,0x20c,kBuildId,15,11));
    CHECK(!valid(0x10000000,0x100052d0,0x20c,kBuildId,16,10));
    std::uint8_t bad[16]{}; CHECK(!valid(0x10000000,0x100052d0,0x20c,bad,16,11));
    for (unsigned i = 0; i < 11; ++i) { words[i] ^= 1; CHECK(!valid(0x10000000,0x100052d0,0x20c,kBuildId,16,11)); words[i] ^= 1; }
    CHECK(!valid(UINTPTR_MAX - 8,0x100052d0,0x20c,kBuildId,16,11));
}
void dispatch_tests() {
    { Fixture f; f.run(); CHECK(f.queries == 1); CHECK(f.seen[2] != f.frame.args[2]); std::uint32_t flags; std::memcpy(&flags, f.forwarded.data()+12,4); CHECK(flags == kAllowMulti); auto expected=f.attr; std::memcpy(expected.data()+12,&flags,4); CHECK(expected == f.forwarded); std::uint32_t original; std::memcpy(&original,f.attr.data()+12,4); CHECK(original == 0); CHECK(f.event.decision == Decision::Patched); CHECK(f.event.error == EPERM); }
    { Fixture f; f.field(2,1); f.result=123; f.output_errno=0; f.run(); CHECK(f.event.decision == Decision::Patched); CHECK(f.event.result == 123); }
    { Fixture f; f.state.mode=Mode::Observe; f.unchanged(); CHECK(f.queries == 0); CHECK(f.event.decision == Decision::Observed); }
    { Fixture f; f.state.mode=Mode::Disabled; f.unchanged(); CHECK(f.reads == 0); CHECK(f.queries == 0); CHECK(f.reports == 0); }
    for (unsigned arg : {0u,1u,3u}) { Fixture f; f.frame.args[arg]=arg==3 ? 257 : 999; f.unchanged(); CHECK(f.reads == 0); CHECK(f.queries == 0); }
    { Fixture f; f.frame.args[3]=15; f.unchanged(); CHECK(f.reads == 0); }
    for (std::uint64_t size : {16u,151u,153u,256u}) { Fixture f; f.frame.args[3]=size; f.unchanged(); CHECK(f.reads == 0); CHECK(f.queries == 0); }
    { Fixture f; f.frame.lr+=4; f.unchanged(); CHECK(f.reads == 0); }
    { Fixture f; f.frame.args[2]=0; f.unchanged(); CHECK(f.reads == 0); }
    { Fixture f; f.readable=false; f.unchanged(); CHECK(f.queries == 0); }
    { Fixture f; f.identity=false; f.unchanged(); CHECK(f.queries == 0); }
    for (std::uint32_t type : {2u,3u,UINT32_MAX}) { Fixture f; f.field(2,type); f.unchanged(); CHECK(f.queries == 0); }
    for (std::uint32_t flags : {1u,2u,3u,4u,UINT32_MAX}) { Fixture f; f.field(3,flags); f.unchanged(); CHECK(f.queries == 0); }
    { Fixture f; f.query_ok=false; f.unchanged(); CHECK(f.event.decision == Decision::QueryFailed); }
    for (std::uint32_t flags : {0u,1u,2u,3u,4u}) { Fixture f; f.attached={1,flags}; f.unchanged(); CHECK(f.event.decision == Decision::Incompatible); CHECK(f.event.result == -1); CHECK(f.event.error == EPERM); }
    { Fixture f; f.attached={1,2}; f.output_errno=EINVAL; f.unchanged(); CHECK(f.event.error == EINVAL); }
    CHECK(compatible_query({0,0})); CHECK(!compatible_query({0,2})); CHECK(!compatible_query({1,2})); CHECK(!compatible_query({1,0})); CHECK(!compatible_query({1,3})); CHECK(!compatible_query({0,1}));
}
void memory_tests() {
    { Fixture f; f.attr[151]=1; f.unchanged(); CHECK(f.queries==0); }
    { Fixture f; f.reenter=true; errno=EINTR; CHECK(dispatch(f.frame,f.state,f.ops())==-1); CHECK(f.calls==2 && f.reports==1 && f.queries==1 && errno==EPERM); }
    int fd=open("/proc/self/mem",O_RDONLY|O_CLOEXEC); CHECK(fd>=0);
    const std::uint64_t source=0x123456789abcdef0ULL; std::uint64_t result=0;
    CHECK(read_memory(fd,reinterpret_cast<std::uintptr_t>(&source),&result,sizeof(result))); CHECK(result==source);
    CHECK(!read_memory(fd,0,&result,sizeof(result))); CHECK(!read_memory(fd,1,&result,sizeof(result)));
    CHECK(!read_memory(fd,UINTPTR_MAX-2,&result,sizeof(result))); CHECK(!read_memory(-1,1,&result,sizeof(result)));
    int maps=open("/proc/self/maps",O_RDONLY|O_CLOEXEC); CHECK(maps>=0);
    CHECK(read_readable_memory(fd,maps,reinterpret_cast<std::uintptr_t>(&source),&result,sizeof(result)));
    void* hidden=mmap(nullptr,4096,PROT_NONE,MAP_PRIVATE|MAP_ANONYMOUS,-1,0); CHECK(hidden!=MAP_FAILED);
    CHECK(!read_readable_memory(fd,maps,reinterpret_cast<std::uintptr_t>(hidden),&result,sizeof(result)));
    munmap(hidden,4096); close(maps);
    CHECK(!read_memory(fd,reinterpret_cast<std::uintptr_t>(&source),&result,kMaxAttr+1)); close(fd);
}
}
int main() { config_tests(); certification_tests(); dispatch_tests(); memory_tests(); std::printf("core: %d checks passed\n",checks); }

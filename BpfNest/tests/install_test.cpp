#include "bpfnest/install.h"
#include <cerrno>
#include <cstdio>
#include <cstdlib>
#include <cstring>

using namespace bpfnest;
#define CHECK(x) do { if (!(x)) { std::fprintf(stderr,"install:%d: %s\n",__LINE__,#x); std::exit(1); } } while(0)
namespace {
long original(long, ...) { return -7; }
long entry(long, ...) { return -8; }
struct Fixture {
    Installation state;
    ZygiskNextAPI api{};
    bool netd=true, certified=true, slot_ok=true, resolver_ok=true, symbol_ok=true;
    Mode mode=Mode::Observe;
    int hooks=0, frees=0, queries=0, hook_result=ZN_SUCCESS;
    static Fixture* current;
    Fixture() {
        current=this;
        api.newSymbolResolver=[](const char* path, void* base) { CHECK(std::strcmp(path,"libnetd_updatable.so")==0); CHECK(base==nullptr); return current->resolver_ok ? reinterpret_cast<ZnSymbolResolver*>(1) : nullptr; };
        api.freeSymbolResolver=[](ZnSymbolResolver*) { ++current->frees; };
        api.getBaseAddress=[](ZnSymbolResolver*) -> void* { return reinterpret_cast<void*>(0x10000000); };
        api.symbolLookup=[](ZnSymbolResolver*,const char* name,bool prefix,std::size_t* size) -> void* { CHECK(std::strcmp(name,kAttachSymbol)==0); CHECK(!prefix); *size=kSymbolSize; return current->symbol_ok ? reinterpret_cast<void*>(0x100052d0) : nullptr; };
        api.pltHook=[](void* base,const char* symbol,void* handler,void** previous) {
            ++current->hooks; CHECK(base==reinterpret_cast<void*>(0x10000000)); CHECK(std::strcmp(symbol,"syscall")==0); CHECK(handler==reinterpret_cast<void*>(entry));
            // Simulate publication immediately, BEFORE the API returns/writes previous.
            CHECK(current->state.ready.load(std::memory_order_acquire)); CHECK(current->state.original==reinterpret_cast<void*>(original)); CHECK(current->state.state.return_pc==0x100053a8); CHECK(current->state.state.mode==current->mode);
            CHECK(active_state(current->state).mode==Mode::Disabled);
            current->check_passthrough();
            CHECK(previous==nullptr); return current->hook_result;
        };
    }
    InstallOps ops() { return {this,
        [](void* p) { return static_cast<Fixture*>(p)->netd; },
        [](void* p) { return static_cast<Fixture*>(p)->mode; },
        [](void* p,void* base,void* symbol,std::size_t size) { CHECK(base==reinterpret_cast<void*>(0x10000000)); CHECK(symbol==reinterpret_cast<void*>(0x100052d0)); CHECK(size==kSymbolSize); return static_cast<Fixture*>(p)->certified; },
        [](void* p,void*,void** slot,void** libc) { *libc=reinterpret_cast<void*>(original); *slot=static_cast<Fixture*>(p)->slot_ok ? *libc : reinterpret_cast<void*>(entry); return true; }, reinterpret_cast<void*>(entry)}; }
    InstallResult run() { return install(state,&api,ops()); }
    void check_passthrough() {
        Frame frame{{280,8,1,152,44,55,66},0x100053a8};
        Operations ops{&frame,
            [](void*,std::uintptr_t,void*,std::size_t) { CHECK(false); return false; },
            [](void*,int) { CHECK(false); return false; },
            [](void*,int,std::uint32_t,Query*) { CHECK(false); return false; },
            [](void* p,const std::uint64_t* a) { CHECK(std::memcmp(a,static_cast<Frame*>(p)->args,56)==0); errno=EINVAL; return -1L; },
            [](void*,const Event&) { CHECK(false); }};
        errno=EINTR; CHECK(dispatch(frame,active_state(state),ops)==-1); CHECK(errno==EINVAL);
    }
};
Fixture* Fixture::current;
}
int main() {
    { Fixture f; CHECK(f.run()==InstallResult::Installed); CHECK(f.hooks==1); CHECK(f.frees==1); CHECK(f.run()==InstallResult::AlreadyAttempted); CHECK(f.hooks==1); }
    { Fixture f; f.mode=Mode::Patch; CHECK(f.run()==InstallResult::Installed); }
    { Fixture f; f.netd=false; CHECK(f.run()==InstallResult::WrongProcess); CHECK(f.hooks==0); }
    { Fixture f; f.mode=Mode::Disabled; CHECK(f.run()==InstallResult::Disabled); CHECK(f.hooks==0); CHECK(f.frees==0); }
    { Fixture f; f.api.pltHook=nullptr; CHECK(f.run()==InstallResult::InvalidApi); CHECK(f.hooks==0); }
    { Fixture f; CHECK(install(f.state,nullptr,f.ops())==InstallResult::InvalidApi); }
    { Fixture f; f.resolver_ok=false; CHECK(f.run()==InstallResult::ResolverFailed); CHECK(f.hooks==0); }
    { Fixture f; f.symbol_ok=false; CHECK(f.run()==InstallResult::UncertifiedImage); CHECK(f.hooks==0); CHECK(f.frees==1); }
    { Fixture f; f.certified=false; CHECK(f.run()==InstallResult::UncertifiedImage); CHECK(f.hooks==0); }
    { Fixture f; f.slot_ok=false; CHECK(f.run()==InstallResult::UnexpectedHook); CHECK(f.hooks==0); }
    { Fixture f; f.mode=Mode::Patch; f.hook_result=ZN_FAILED; CHECK(f.run()==InstallResult::HookFailed); CHECK(f.hooks==1); CHECK(f.state.ready.load()); f.check_passthrough(); CHECK(active_state(f.state).mode==Mode::Disabled); CHECK(f.run()==InstallResult::AlreadyAttempted); f.check_passthrough(); }
    std::puts("install: process/config/API/resolver/certification/chain/publication/reinit cases passed");
}

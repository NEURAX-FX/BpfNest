#include "bpfnest/install.h"
#include <atomic>
#include <thread>
#include <cstdio>
#include <cstdlib>
#include <cstring>
using namespace bpfnest;
#define CHECK(x) do{if(!(x)){std::fprintf(stderr,"install-concurrency:%d: %s\n",__LINE__,#x);std::exit(1);}}while(0)
static long original(long,...){return 73;}
static long entry(long,...){return 74;}
struct Fixture {
    Installation state;
    ZygiskNextAPI api{};
    std::atomic<unsigned> hooks{0};
    std::atomic<bool> in_hook{false},release_hook{false};
    static Fixture* current;
    Fixture(){current=this;
        api.newSymbolResolver=[](const char*,void*){return reinterpret_cast<ZnSymbolResolver*>(1);};
        api.freeSymbolResolver=[](ZnSymbolResolver*){};
        api.getBaseAddress=[](ZnSymbolResolver*){return reinterpret_cast<void*>(0x10000000);};
        api.symbolLookup=[](ZnSymbolResolver*,const char* name,bool prefix,std::size_t* size){CHECK(!prefix&&!std::strcmp(name,kAttachSymbol));*size=0x20c;return reinterpret_cast<void*>(0x100052d0);};
        api.pltHook=[](void*,const char* symbol,void* handler,void** previous){auto& f=*current;CHECK(!std::strcmp(symbol,"syscall")&&handler==reinterpret_cast<void*>(entry)&&previous==nullptr);++f.hooks;f.in_hook.store(true);while(!f.release_hook.load())std::this_thread::yield();return ZN_SUCCESS;};
    }
    InstallOps ops(){return {this,[](void*){return true;},[](void*){return Mode::Patch;},
        [](void*,void*,void*,std::size_t){return true;},
        [](void*,void*,void** slot,void** genuine){*slot=*genuine=reinterpret_cast<void*>(original);return true;},reinterpret_cast<void*>(entry)};}
};
Fixture* Fixture::current=nullptr;
int main(){
    Fixture f;InstallResult first{};
    std::thread installing([&]{first=install(f.state,&f.api,f.ops());});
    while(!f.in_hook.load())std::this_thread::yield();
    CHECK(f.state.ready.load(std::memory_order_acquire));CHECK(f.state.original==reinterpret_cast<void*>(original));
    CHECK(active_state(f.state).mode==Mode::Disabled);
    CHECK(install(f.state,&f.api,f.ops())==InstallResult::AlreadyAttempted);
    f.release_hook.store(true);installing.join();CHECK(first==InstallResult::Installed&&f.hooks==1);
    CHECK(active_state(f.state).mode==Mode::Patch);
    for(unsigned missing=0;missing<5;++missing){Fixture q;
        if(missing==0)q.api.pltHook=nullptr;
        if(missing==1)q.api.newSymbolResolver=nullptr;
        if(missing==2)q.api.freeSymbolResolver=nullptr;
        if(missing==3)q.api.getBaseAddress=nullptr;
        if(missing==4)q.api.symbolLookup=nullptr;
        CHECK(install(q.state,&q.api,q.ops())==InstallResult::InvalidApi);CHECK(!q.hooks);
    }
    Fixture fresh;fresh.release_hook.store(true);CHECK(install(fresh.state,&fresh.api,fresh.ops())==InstallResult::Installed);
    std::puts("install-concurrency: pre-return publication, duplicate race, required API pointers and fresh process passed");
}

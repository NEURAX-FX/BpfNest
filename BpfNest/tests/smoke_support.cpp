// Freestanding Linux-only smoke runtime. These replace libc, not production code.
#include <cstddef>
namespace { int smoke_errno; }
// This freestanding smoke is single-threaded; production uses compiler-rt emutls.
extern "C" { unsigned smoke_tls_accesses=0; }
extern "C" void* __emutls_get_address(void*) { ++smoke_tls_accesses; static unsigned char slot[64]{}; return slot; }
extern "C" int* __errno() { return &smoke_errno; }
extern "C" void* memcpy(void* destination,const void* source,std::size_t size) {
    auto* d=static_cast<unsigned char*>(destination); const auto* s=static_cast<const unsigned char*>(source);
    for(std::size_t i=0;i<size;++i) d[i]=s[i];
    return destination;
}
extern "C" void* memset(void* destination,int value,std::size_t size) {
    auto* d=static_cast<unsigned char*>(destination);
    for(std::size_t i=0;i<size;++i) d[i]=static_cast<unsigned char>(value);
    return destination;
}
extern "C" int memcmp(const void* left,const void* right,std::size_t size) {
    const auto* a=static_cast<const unsigned char*>(left); const auto* b=static_cast<const unsigned char*>(right);
    for(std::size_t i=0;i<size;++i) if(a[i]!=b[i]) return static_cast<int>(a[i])-b[i];
    return 0;
}

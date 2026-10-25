#pragma once
#include "bpfnest/core.h"
#include "zygisk_next_api.h"
#include <atomic>

namespace bpfnest {
struct Installation {
    std::atomic_flag started = ATOMIC_FLAG_INIT;
    std::atomic<bool> ready{false};
    std::atomic<Mode> active_mode{Mode::Disabled};
    State state{Mode::Disabled, 0};
    void* original = nullptr;
};
struct InstallOps {
    void* context;
    bool (*is_netd)(void*);
    Mode (*load_config)(void*);
    // Obtain memory reader/cgroup identity and certify loaded ELF against this symbol.
    bool (*certify)(void*, void* base, void* symbol, std::size_t size);
    // Return current validated syscall GOT slot and independently resolved libc syscall.
    bool (*original_slot)(void*, void* base, void** slot, void** libc_syscall);
    void* entry;
};
enum class InstallResult { Installed, AlreadyAttempted, WrongProcess, Disabled, InvalidApi,
    ResolverFailed, UncertifiedImage, UnexpectedHook, HookFailed };
InstallResult install(Installation& state, const ZygiskNextAPI* api, const InstallOps& operations);
State active_state(const Installation& installation);
} // namespace bpfnest

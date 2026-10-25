#include "bpfnest/install.h"

namespace bpfnest {
InstallResult install(Installation& target, const ZygiskNextAPI* api, const InstallOps& ops) {
    if (target.started.test_and_set(std::memory_order_acq_rel)) return InstallResult::AlreadyAttempted;
    if (!ops.is_netd(ops.context)) return InstallResult::WrongProcess;
    const Mode mode = ops.load_config(ops.context);
    if (mode == Mode::Disabled) return InstallResult::Disabled;
    if (!api || !api->pltHook || !api->newSymbolResolver || !api->freeSymbolResolver ||
        !api->getBaseAddress || !api->symbolLookup) return InstallResult::InvalidApi;
    auto* resolver = api->newSymbolResolver("libnetd_updatable.so", nullptr);
    if (!resolver) return InstallResult::ResolverFailed;
    void* base = api->getBaseAddress(resolver);
    std::size_t size = 0;
    void* symbol = api->symbolLookup(resolver, kAttachSymbol, false, &size);
    const bool certified = base && symbol && ops.certify(ops.context, base, symbol, size);
    api->freeSymbolResolver(resolver);
    if (!certified) return InstallResult::UncertifiedImage;
    void* current = nullptr;
    void* libc_syscall = nullptr;
    if (!ops.original_slot(ops.context, base, &current, &libc_syscall) || !current || current != libc_syscall)
        return InstallResult::UnexpectedHook;
    target.original = current;
    target.state = {mode, reinterpret_cast<std::uintptr_t>(base) + kReturnOffset};
    target.ready.store(true, std::memory_order_release);
    // NULL original avoids the provider racing a write to the already published pointer.
    // Even failure keeps immutable forwarding state alive in case the provider touched GOT.
    if (api->pltHook(base, "syscall", ops.entry, nullptr) != ZN_SUCCESS) {
        target.active_mode.store(Mode::Disabled,std::memory_order_release);
        return InstallResult::HookFailed;
    }
    target.active_mode.store(mode,std::memory_order_release);
    return InstallResult::Installed;
}
State active_state(const Installation& installation) {
    installation.ready.load(std::memory_order_acquire);
    return {installation.active_mode.load(std::memory_order_acquire),installation.state.return_pc};
}
} // namespace bpfnest

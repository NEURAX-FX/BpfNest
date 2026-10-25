#pragma once
#include "bpfnest/core.h"
namespace bpfnest {
inline constexpr char kTargetLibrary[]="libnetd_updatable.so";
struct Memory {
    void* opaque{};
    bool (*read)(void*,void*,std::uintptr_t,std::size_t){};
};
struct Target {
    std::uintptr_t base{}, function{}, caller_lr{}, got{};
    std::size_t function_size{};
};
bool validate_target(const Memory&, std::uintptr_t base, std::uintptr_t function,
                     std::size_t function_size, Target*) noexcept;
} // namespace bpfnest

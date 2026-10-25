#!/usr/bin/env bash
set -euo pipefail
source "$(dirname -- "${BASH_SOURCE[0]}")/common.sh"
[[ $# == 0 ]] || { printf 'Usage: %s\n' "$0" >&2; exit 2; }
QEMU=$(command -v qemu-aarch64 || command -v qemu-aarch64-static || true)
[[ -n "$QEMU" ]] || { printf 'SKIP: qemu-aarch64 is not installed\n'; exit 77; }
NDK=$(find_ndk)
CXX="$NDK/toolchains/llvm/prebuilt/linux-x86_64/bin/aarch64-linux-android26-clang++"
mkdir -p "$BPFNEST_ROOT/build/smoke"
"$CXX" -std=c++17 -O2 -g -fno-exceptions -fno-rtti -fno-stack-protector -fno-builtin \
    -ffunction-sections -fdata-sections -nostdlib -static -Wl,--gc-sections,-e,_start \
    -I"$BPFNEST_ROOT/include" "$BPFNEST_ROOT/src/core.cpp" "$BPFNEST_ROOT/src/trampoline.S" \
    "$BPFNEST_ROOT/tests/trampoline_smoke.cpp" "$BPFNEST_ROOT/tests/smoke_support.cpp" \
    "$BPFNEST_ROOT/tests/smoke_start.S" -o "$BPFNEST_ROOT/build/smoke/trampoline-arm64"
"$QEMU" "$BPFNEST_ROOT/build/smoke/trampoline-arm64"
printf 'ARM64 QEMU PASS: production trampoline + production dispatcher; all seven argument words, exact LR, copy isolation, observe/no query, empty patch, nonempty refusal, return/input+output errno\n'

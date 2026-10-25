#!/usr/bin/env bash
set -euo pipefail
source "$(dirname -- "${BASH_SOURCE[0]}")/common.sh"
[[ $# == 0 ]] || { printf 'Usage: %s\n' "$0" >&2; exit 2; }
need cmake; need ninja; need python3
NDK=$(find_ndk)
printf 'NDK=%s\nABI=arm64-v8a\nAPI=26\n' "$NDK"
cmake -S "$BPFNEST_ROOT" -B "$BPFNEST_ROOT/build/arm64" -G Ninja \
    -DCMAKE_BUILD_TYPE=Release \
    -DCMAKE_TOOLCHAIN_FILE="$NDK/build/cmake/android.toolchain.cmake" \
    -DANDROID_ABI=arm64-v8a -DANDROID_PLATFORM=android-26 -DANDROID_STL=c++_static
cmake --build "$BPFNEST_ROOT/build/arm64" --target bpfnest --parallel "${BPFNEST_JOBS:-2}"
"$BPFNEST_ROOT/scripts/inspect-elf.sh" --module "$BPFNEST_ROOT/build/arm64/libbpfnest.so"

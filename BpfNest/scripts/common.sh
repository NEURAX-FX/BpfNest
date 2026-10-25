#!/usr/bin/env bash
# Shared local build paths only; never contacts a device.
set -euo pipefail
BPFNEST_ROOT=$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")/.." && pwd)
find_ndk() {
    local candidate version
    for candidate in "${ANDROID_NDK_HOME:-}" "${ANDROID_NDK_ROOT:-}" "${NDK_HOME:-}"; do
        if [[ -n "$candidate" && -f "$candidate/build/cmake/android.toolchain.cmake" && -x "$candidate/toolchains/llvm/prebuilt/linux-x86_64/bin/clang++" ]]; then
            printf '%s\n' "$candidate"; return
        fi
    done
    local -a candidates=()
    local root
    for root in "${ANDROID_HOME:-}/ndk" "${ANDROID_SDK_ROOT:-}/ndk" "$HOME/Android/Sdk/ndk" /opt/android-sdk/ndk; do
        [[ -d "$root" ]] || continue
        for candidate in "$root"/*; do
            [[ -f "$candidate/build/cmake/android.toolchain.cmake" && -x "$candidate/toolchains/llvm/prebuilt/linux-x86_64/bin/clang++" ]] || continue
            version=${candidate##*/}
            [[ "$version" =~ ^[0-9]+(\.[0-9]+)+$ ]] && candidates+=("$candidate")
        done
    done
    if ((${#candidates[@]} == 0)); then
        printf 'ERROR: no valid Linux Android NDK. Set ANDROID_NDK_HOME.\n' >&2; return 1
    fi
    printf '%s\n' "${candidates[@]}" | sort -V | tail -n 1
}
need() { command -v "$1" >/dev/null || { printf 'ERROR: required tool missing: %s\n' "$1" >&2; return 1; }; }

#!/usr/bin/env bash
# Local-only checks: never invokes adb, a manager, or a running netd.
set -euo pipefail
source "$(dirname -- "${BASH_SOURCE[0]}")/common.sh"
[[ $# == 0 ]] || { printf 'Usage: %s\n' "$0" >&2; exit 2; }
need cmake; need ninja; need python3
mkdir -p "$BPFNEST_ROOT/build/logs"
LOG="$BPFNEST_ROOT/build/logs/verify.log"
exec > >(tee "$LOG") 2>&1
printf 'VERIFY UTC %s\n' "$(date -u +%FT%TZ)"
cmake -S "$BPFNEST_ROOT" -B "$BPFNEST_ROOT/build/host" -G Ninja -DCMAKE_BUILD_TYPE=Debug
cmake --build "$BPFNEST_ROOT/build/host" --parallel "${BPFNEST_JOBS:-2}"
ctest --test-dir "$BPFNEST_ROOT/build/host" --output-on-failure
"$BPFNEST_ROOT/scripts/build.sh"
if command -v qemu-aarch64 >/dev/null || command -v qemu-aarch64-static >/dev/null; then
    "$BPFNEST_ROOT/scripts/smoke-arm64.sh"
else
    printf 'SKIP ARM64 execution: QEMU unavailable (not an execution pass)\n'
fi
python3 "$BPFNEST_ROOT/scripts/tooling_tests.py"
printf 'VERIFY PASS: configured host suite, Android build/static checks and available QEMU completed\n'

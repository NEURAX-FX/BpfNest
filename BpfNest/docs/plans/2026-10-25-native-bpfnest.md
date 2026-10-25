# Native BpfNest Implementation Plan

> **For agentic workers:** REQUIRED SUB-SKILL: Use superpowers:executing-plans to implement this plan task-by-task. User approved inline implementation in the existing checkout; no git operations or deployment.

**Goal:** Build a complete, conservative netd-only AArch64 ZygiskNext PLT interceptor, default observe, with proven host and QEMU behavior and a real Android module.

**Architecture:** A fixed-size register frame enters a platform-independent dispatch/filter. Linux adapters provide safe self-read, fd identity/pinning, direct BPF query and logging. Initialization validates the actual process, official API pointers, runtime symbol, known ELF BuildID and exact AArch64 call sequence before a single caller-oriented PLT hook; unsupported inputs always forward.

**Tech Stack:** C++17, AArch64 assembly, CMake/CTest, Android NDK API26, Linux host tests and freestanding QEMU harness.

**Spec:** `reports/BpfNest-preliminary-analysis.md` plus the approved implementation request and configuration contract.

## Global Constraints
- All new work stays in BpfNest; preserve supplied ELFs/analysis/reports. No commits, stash, clean, worktrees, device commands, deployment or system changes.
- Official API4 header is unchanged; `zn_module` uses the actual two-argument callback. No invented runtime API version member.
- Target only `/system/bin/netd`, `libnetd_updatable.so` syscall PLT. No inline/global hooks.
- Config is module-root bpfnest.conf, exact mode=observe/patch/disabled; blanks/comments allowed; missing=>observe; other errors=>disabled. Fixed `/sys/fs/cgroup`.
- Patch requires known BuildID d114509ac20ab4037ed59923cfa032d5, runtime local symbol and exact validated callsite; no offset fallback.
- __NR_bpf=280, attach cmd8, types0/1, attr152 with zero unused tail, input flags exactly0. Query direct attachments; existing flags0, failure or unknown mode refuses patch.
- Preserve caller buffer, all six args, LR, original return and input/output errno. Safe read failure passes original. Never detach, alter maps or weaken security.

## Review Focus
- Synchronous hook callbacks before pltHook returns must have a usable original and immutable state.
- Hook APIs may report failure after publishing; passthrough must remain safe.
- Closed/reused cgroup descriptor must not redirect the patched operation.
- Invalid/mapped-boundary attr and query-side errno must not crash/change original semantics.
- API35 supplied ELFs do not prove the user's Android14 runtime or BPF query permission.

## Integration ruling
Director concurrently integrated the authoritative `core.h`/`install.h` design instead of the initial `core.hpp`/`module.hpp` interfaces below. Preserve the initial plan as provenance; active implementation mapping: `Frame`/`State`/`Operations` in `core.h`, lifecycle in `install.cpp`, Linux adapters and callback in `runtime.cpp`, entry in `trampoline.S`. Removed intermediate files/tests do not count as final coverage. Final executed host suite has eight CTests, plus production and raw-syscall AArch64 QEMU smoke; see native-verification.md. Initial implementation accepted existing MULTI; director's integrated version refuses all nonempty direct-query states, a conservative behavioral restriction recorded in native-progress.md (not silently claimed as support).

### Task 1: Config and dispatch policy
**Review:** per-task — concurrency/security
**Files:** include/bpfnest/core.hpp, src/core.cpp, src/config.cpp, tests/core_tests.cpp, CMakeLists.txt.
**Interfaces:** SyscallFrame (number,six args,LR), genuine variadic SyscallFn, immutable Context plus injected unavoidable platform Ops; parse_config and dispatch.
- [ ] Write consumer-visible failing config/dispatch tests; run RED.
- [ ] Implement strict parsing, stack-copy filter, direct query compatibility, fd pin/identity checks, reentrancy and errno preservation.
- [ ] Run covering tests and full host suite GREEN.

### Task 2: Verified ELF and initialization
**Review:** per-task — security/publication
**Files:** include/bpfnest/elf.hpp, include/bpfnest/module.hpp, src/elf.cpp, src/module.cpp, src/platform.cpp, tests/elf_tests.cpp, tests/module_tests.cpp, third_party/zygisk_next_api.h and source/license notice.
**Interfaces:** validated Target with base,function,exact LR,GOT; initialize callback verifies process/config/API/target/original before publishing, one attempt per process.
- [ ] Write failing real-ELF mutation, hook lifecycle and Linux adapter tests; run RED.
- [ ] Vendor unchanged verified header; implement bounded runtime ELF validation and official resolver lookup; no guessed names/offset fallback.
- [ ] Implement safe platform adapters, netd-only configuration discovery and official exported module.
- [ ] Run covering tests and full suite GREEN; preserve precise evidence and limited support documentation.

### Task 3: Assembly, native smoke and NDK artifact
**Files:** src/entry_aarch64.S, tests/trampoline_smoke.S, tests/host_smoke.cpp, CMakeLists.txt, reports/native-verification.md, reports/native-progress.md.
- [ ] Write failing freestanding six-register/LR + real six-argument mmap syscall harness; observe absent entry RED.
- [ ] Implement register-capture entry; execute QEMU harness GREEN.
- [ ] Execute real host smoke exercising dispatch with safe Linux memory/fd adapters and real syscall forwarding.
- [ ] Run cmake/CTest full suite, Android arm64 API26 build, ELF symbol/architecture/dependency/entry checks.
- [ ] Record exact commands, outputs, red/green, artifact path and device-only unknowns. No deployment.

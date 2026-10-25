# BpfNest implementation plan

Approved scope: the root `reports/BpfNest-preliminary-analysis.md` and the implementation assignment. Work in place, preserve supplied binaries, no commits, deployment, service restarts, detach, map changes or SELinux rules.

## Contracts

- CMake shared target `bpfnest`, Android ARM64 API 26, output `build/arm64/libbpfnest.so`; host CTest in `build/host`.
- Unchanged official API4 header at `third_party/zygisk_next_api.h`; export only required `zn_module`. Loader negotiates `target_api_version=4`; no invented core runtime-version field.
- Module root `bpfnest.conf`: one exact `mode=observe|patch|disabled`, optional comments/blank lines. Missing means observe; malformed/unknown/duplicate/unreadable means disabled. Shipped default observe. Fixed `/sys/fs/cgroup`.
- Exact `/proc/self/exe` process guard. Runtime mini-debug symbol resolution is mandatory. Known BuildID `d114509ac20ab4037ed59923cfa032d5`, symbol start `0x52d0`, size `0x20c`, LR `0x53a8`, original flags-store and BL instructions, GOT `0xc478`, PLT `0xac70` must all match. No unknown-image fallback, inline hook or global libc hook.
- Initialize genuine original syscall pointer before exposing the handler; refuse an unexpected existing GOT hook. Publish immutable state before caller-oriented PLT installation.
- AArch64 assembly captures x0..x6 and original LR. C++ forwards all seven words to original libc syscall and preserves incoming errno before that call and outgoing errno across diagnostics. No hot-path allocations.
- Bounded attr copy via cached `/proc/self/mem` + `pread`, never unchecked pointer dereference or process_vm_readv. Inspection failures forward original arguments.
- Patch only syscall 280/cmd 8/type 0 or 1/exact flags 0/exact certified LR/attr size exactly152/verified cgroup2 directory dev+inode. Count-only direct query (query_flags=0, prog_cnt=0, prog_ids=0) must succeed with count==0 AND flags==0. ALL nonempty or flagged states are refused, including MULTI. Observe never queries and never rewrites attach arguments.

## Ordered work and evidence

1. **Official contract and binary proof**: compare official raw API/sample/conventions, save hashes and source URLs; inspect supplied ELF notes, mini-debug symbols and exact instructions. Record supported installation path, header rights and API/sample drift.
2. **Core tests first**: create host tests for config rejection/defaults, all policy gates, exact BuildID/instruction certification, compatible query decisions, stack-owned copy isolation, invalid pointer reads, and genuine errno/result/all-argument forwarding. Run red against missing implementation, then implement `include/bpfnest/core.h`, `src/core.cpp`, CMake host suite and pass.
3. **Runtime first tests**: introduce adapter-level tests for observe not querying, patch query failures, cgroup identity mismatch, disabled no hook, unknown-image rejection and install ordering. Implement `src/runtime.cpp` and `src/trampoline.S` using the same tested dispatcher, static immutable runtime state and official API. Build Android, inspect required exports and API data. No device success claim.
4. **Actual ARM64 smoke**: build a freestanding static Linux AArch64 test binary from the production assembly plus a deterministic original-function fixture. Run with available qemu-aarch64-static. Cover all register words, original LR, original return, both unchanged and copied attr paths, and errno state. This is ABI evidence, not Android loader/PLT evidence.
5. **Tooling and module**: test absent scripts as red, implement NDK discovery/build, full host verification plus required-if-available QEMU smoke, static ELF inspection and standard manager ZIP packaging. Module includes `module.prop`, exact `zn_modules.txt`, observe config, guarded installer and genuine official Magisk installer support; no service scripts or policy broadening.
6. **End-to-end local evidence**: execute every shipped script, inspect real input ELFs, reject incompatible input for module checks, validate ZIP manifest/config/export/API/architecture and record exact artifact hashes/sizes/commands/status/output in tooling/native reports.
7. **Documentation and review**: Chinese README, changelog and attach/ELF/compatibility/device reports describe actual implemented behavior and limits, A/B/C device validation gates and non-live rollback. Record all remaining device-only evidence honestly. Consume independent director review, correct reachable issues, rerun affected checks once after final changes.

## Acceptance boundaries

Host tests and QEMU validate local policy/ABI behavior only. Android compilation and ELF/ZIP inspection validate distributable structure only. ZygiskNext startup timing, SELinux access, RELRO PLT install, BPF query permission, successful MULTI parent/child attach, joint execution and Android/Fedora nonregression remain device-only. Netd restart while MULTI persists is a serious unresolved patch-enable blocker: duplicate attach can fail EINVAL, additional root programs can cause netd's one-element query to fail ENOSPC. No error suppression/detach/REPLACE is permitted; patch is NOT production/restart-safe without an authorized restart strategy. No systemd-fix claim follows from module loading.

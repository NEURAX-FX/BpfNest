# Third-party notices

本仓库原创代码以 GPL-3.0-only 发布（见 `LICENSE`）。以下材料各自保留原许可；本仓库的整体许可不改变它们。

## AOSP reference snapshots — Apache-2.0

`analysis/` 中保留了从 AOSP 取得的源码/配置快照，用于静态分析对照：

- `analysis/aosp/Android.bp`
- `analysis/aosp/Android.bp.refs_heads_main`
- `analysis/aosp/BpfHandler.android15.cpp`
- `analysis/aosp/BpfHandler.cpp.refs_heads_main`
- `analysis/aosp/BpfHandler.h.refs_heads_main`
- `analysis/aosp/BpfSyscallWrappers.h`
- `analysis/aosp/NetBpfLoad.cpp.refs_heads_main`
- `analysis/aosp/NetdUpdatable.cpp.refs_heads_main`
- `analysis/aosp/TrafficController.android15.cpp`
- `analysis/aosp/TrafficController.android15.h`
- `analysis/aosp/loader-Android.bp`
- `analysis/aosp/main.cpp`
- `analysis/aosp/netd-updatable-Android.android15.bp`
- `analysis/aosp/netd-updatable-Android.bp`
- `analysis/aosp/system-bpf-Android.bp`
- `analysis/aosp/netd.rc`
- `analysis/aosp/bpfloader-sdk35-15-V-24Q3.rc`
- `analysis/aosp/bpfloader-sdk35-15-V-QPR1-24Q4.rc`
- `analysis/aosp/netbpfload.35rc.refs_heads_main`
- `analysis/aosp/netbpfload.rc.refs_heads_main`

These files are Copyright (C) The Android Open Source Project and are licensed
under the Apache License, Version 2.0. `analysis/aosp/README.txt` is a local
provenance note describing the extraction commands and upstream branches; it is
not an AOSP source file and is retained as documentation.

A copy of the license is in `LICENSES/Apache-2.0.txt`. Primary sources:

- https://android.googlesource.com/platform/system/netd/
- https://android.googlesource.com/platform/system/bpf/
- https://android.googlesource.com/platform/packages/modules/Connectivity/

Modifications: comment stripping, extraction from the named branches, file
renaming, and removal of unrelated sections. Retained files with an original
Apache-2.0 header keep that header. Files without a header (generated `Android.bp`,
`.rc` snapshots, provenance notes) are still derived from the AOSP material
above and are not relicensed by this repository.

## Magisk module installer — GPL-3.0

`BpfNest/module/META-INF/com/google/android/update-binary` is the unmodified
Magisk v28.1 `scripts/module_installer.sh`:

- Source: https://github.com/topjohnwu/Magisk/blob/v28.1/scripts/module_installer.sh
- License: https://github.com/topjohnwu/Magisk/blob/v28.1/LICENSE (GPL-3.0)

It is included so the ZIP uses Magisk's real `install_module` entry instead of a
fake updater. This repository's `LICENSE` is also GPL-3.0; the corresponding
source is this repository plus the upstream link above.

`BpfNest/module/customize.sh` is original code implementing standard
manager-only/architecture checks; it contains no copied upstream sample code.

## ZygiskNext API header — not redistributed

`BpfNest/third_party/zygisk_next_api.h` is **not** included in this repository.
Upstream states Copyright © 2024-2026 Zygisk Next developers, all rights
reserved, with no modification, redistribution, or component-extraction grant:

- https://github.com/LSPosed/ZygiskNext#copyright-notice
- Pinned commit: `2620542f29284672c9e869e285fceafe68ee6259`
- SHA256: `957922aeec1bb31091cc37dca29267b7fa41e4e109f679af1526ee4c5e4325c8`

`BpfNest/scripts/fetch-api-header.sh` downloads that pinned header directly
from upstream at build time. Users are responsible for their own use and for
obtaining any redistribution authorization. The header is not part of the
module ZIP and is not relicensed as GPL/MIT/Apache here. See
`BpfNest/third_party/NOTICE.txt`.

The upstream ZygiskNext README, module sample, and repository tree metadata that
were used only as local references are not redistributed.

## Device firmware images — not redistributed

The following files are not included: `netd`, `libbpf_android.so`,
`libnetd_updatable.so`, `analysis/*.debugdata.xz`, and
`analysis/*.minidebug.elf`. They are device/ROM images, and the repository does
not grant or imply redistribution rights for vendor-built binaries. Their
BuildIDs and SHA256 values are recorded in
`reports/BpfNest-preliminary-analysis.md` for identification. Users must obtain
their own copies from a device or ROM they are authorized to use.

## Trademarks

Android is a trademark of Google LLC. Magisk, KernelSU, ZygiskNext, and other
names are trademarks of their respective owners. This project is not endorsed
by or affiliated with any of them.

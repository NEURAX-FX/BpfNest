# BpfNest

在已 root 的 Android 设备上，让容器/子系统（例如 Droidspaces 中的 Fedora + systemd）在 cgroup v2 子层级附加自己的 BPF 程序时，与 Android netd 已经附加在父 cgroup 上的 ingress/egress 程序共存。

BpfNest 是一个只针对 `/system/bin/netd` 的 ZygiskNext API4 原生模块。它 hook `libnetd_updatable.so` 自身的 `syscall@plt`，只处理已认证调用点上的 `BPF_PROG_ATTACH`：

- `observe`（默认）：检查并记录目标 attach，原样转发；不查询、不改 flags。
- `patch`（实验选项）：仅在 ingress/egress、原 flags 为 0、attr 为 152 字节、父 cgroup 身份匹配、direct query 的 count 和 flags 都为 0 时，修改**私有副本**中的 `ALLOW_MULTI`。
- `disabled`：不安装 hook。

不 detach、不修改 map、不放宽 SELinux/seccomp、不伪造成功，也不做 inline hook 或全局 libc hook。

> **状态（设备所有者报告，2026-10-26）**：项目已在 MT6989 / Android 14 目标设备实测可用。仓库内的自动化证据仍只覆盖 host、QEMU、ELF/ZIP 静态检查；`patch` 的持久 MULTI 与 netd 重启风险没有消除，不应在生产系统启用。

## 为什么需要它

Android netd 启动时会向固定父 cgroup `/sys/fs/cgroup` 附加 ingress/egress BPF 程序，附加 flags 为 `0`（单程序模式）。Linux cgroup v2 允许子层级附加程序，但如果父级是单程序模式，子级再附加就会失败。重启 netd 或改写系统策略都不是可接受的方案。

显式设置 `BPF_F_ALLOW_MULTI` 可以保留 Android 原有程序并允许子级继续附加。BpfNest 只在该已认证调用点、只对满足全部条件的请求做这一件事；其余请求全部原样转发。

## 认证范围

| 项目 | 约束 |
|---|---|
| 进程 | 精确 `/system/bin/netd` |
| 注入 | ZygiskNext API4，`zn_modules.txt` 精确声明 `path=/system/bin/netd` |
| 库 | `libnetd_updatable.so`，BuildID `d114509ac20ab4037ed59923cfa032d5` |
| 符号/指令 | `attachProgramToCgroup` VA `0x52d0` / size `0x20c`，LR `0x53a8`，PLT `0xac70`，GOT `0xc478`，指令逐字节认证 |
| 父 cgroup | 固定 `/sys/fs/cgroup`，cgroup2 dev+inode 身份检查 |
| syscall | `__NR_bpf` (280)，`BPF_PROG_ATTACH` (8)，type 0/1，attr size 152，原 flags 0 |
| ABI | arm64-v8a，最低链接 API26；不是全 API26+ ROM 兼容承诺 |

未知 BuildID、未知布局、查询失败、非空 attachment、既有 MULTI、配置错误都不会被“尽力而为”处理：模块不修改参数，`patch` 模式要求显式配置。

## 仓库结构

```text
BpfNest/      模块源码、构建/验证/打包脚本、模块文件、证据报告
analysis/     AOSP 参考快照（受限的 ZygiskNext 材料不随仓库分发）
reports/      BpfNest 预研报告
```

先读 [`BpfNest/README.md`](BpfNest/README.md)。安全边界与失败模式见：

- [`BpfNest/reports/attach-path.md`](BpfNest/reports/attach-path.md)
- [`BpfNest/reports/elf-analysis.md`](BpfNest/reports/elf-analysis.md)
- [`BpfNest/reports/compatibility.md`](BpfNest/reports/compatibility.md)
- [`BpfNest/reports/device-validation.md`](BpfNest/reports/device-validation.md)

## 构建

依赖：Linux x86_64、CMake ≥ 3.22、Ninja、Python 3、Android NDK（实测 `29.0.14206865`，r29）、`curl`、`sha256sum`、`zip`/`unzip`。建议安装 `qemu-user-static`，否则 ARM64 执行冒烟测试会被跳过而不是通过。

```sh
cd BpfNest
./scripts/fetch-api-header.sh   # 从上游固定 commit 获取不随本仓库分发的 API4 头文件
./scripts/build.sh              # NDK arm64 构建 + ELF/API 静态检查
./scripts/verify.sh             # host CTest + Android 构建 + QEMU 冒烟 + tooling 检查
./scripts/package.sh            # 重新执行 verify，生成 dist/BpfNest-v0.1.0-arm64.zip + SHA256
```

NDK 可用 `ANDROID_NDK_HOME` 指定；脚本也会查找 `$HOME/Android/Sdk/ndk`。构建产物在 `BpfNest/build/`，发布包在 `BpfNest/dist/`，两者都不入库。

### 关于真实 ELF 测试

`libnetd_updatable.so` 是设备/ROM 二进制，本仓库不重分发它。把从你自己的目标 ROM 提取的文件放到仓库根目录，`verify.sh` 就会运行针对该文件的真实 ELF 认证测试；也可以用 CMake 缓存变量显式指定：

```sh
cmake -S BpfNest -B BpfNest/build/host -DBPFNEST_ELF_FIXTURE=/path/to/libnetd_updatable.so
```

文件缺失时该 CTest 会被明确跳过（并在配置输出中说明），不会伪装成通过。BuildID 不匹配的镜像本来就不在支持范围内。

## 安装与启用

只支持在 Magisk/KernelSU 管理器中手动安装 ZIP，不支持 recovery。安装包默认 `mode=observe`，不会向当前运行的 netd 注入任何东西；启用/禁用/卸载都在设备所有者安排的下一次受控重启后生效。仓库脚本不连接设备、不安装、不重启。

完整步骤与禁用/卸载说明见 [`BpfNest/README.md`](BpfNest/README.md)。不要在未经授权或生产用途的设备上启用 `patch`。已有的内核 attachment 不会因为禁用、卸载或改回 `observe` 而撤销。

## 不随仓库分发的材料

为避免重新分发无授权或与具体设备绑定的材料，公开源码树**不包含**：

- `netd`、`libbpf_android.so`、`libnetd_updatable.so` 及其提取的 debugdata/minidebug；
- ZygiskNext API4 头文件（上游声明版权保留、禁止再分发；由 `fetch-api-header.sh` 在构建时从上游固定 commit 获取）；
- 上游 ZygiskNext README、模块 sample 与仓库树元数据。

AOSP 参考片段仍是 Apache-2.0；Magisk 安装入口仍是 GPL-3.0。详见 [`THIRD_PARTY_NOTICES.md`](THIRD_PARTY_NOTICES.md)。

## 许可

除非文件内另有说明，本仓库原创代码以 **GPL-3.0-only** 发布，见 [`LICENSE`](LICENSE)。`analysis/` 中保留的 AOSP 片段继续适用 Apache-2.0（[`LICENSES/Apache-2.0.txt`](LICENSES/Apache-2.0.txt)），`BpfNest/module/META-INF/com/google/android/update-binary` 是 Magisk v28.1 的 GPL-3.0 安装器。第三方许可不因本仓库的整体许可而改变。

## English

BpfNest is a netd-only ZygiskNext API4 module for arm64 Android. It observes (default) or experimentally patches the cgroup ingress/egress `BPF_PROG_ATTACH` flags at a byte-certified `libnetd_updatable.so` callsite, so a child cgroup can attach its own BPF programs while Android's original programs keep running. It never detaches programs, changes maps, or weakens policy. `patch` is experimental and not restart-safe. Device binaries and the upstream ZygiskNext header are not redistributed. Original code is GPL-3.0-only; retained AOSP snippets remain Apache-2.0.

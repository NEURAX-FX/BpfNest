# 变更记录

## BpfNest 0.1.0 — 公开源码发布 (2026-10-26)

- 原创代码以 GPL-3.0-only 公开；AOSP 参考片段保持 Apache-2.0，Magisk 安装器保持 GPL-3.0，详见根目录 `THIRD_PARTY_NOTICES.md`。
- 不重分发设备 ELF（`netd`、`libbpf_android.so`、`libnetd_updatable.so`、debugdata/minidebug）以及上游 ZygiskNext 头文件、README、sample 和仓库树元数据。
- 新增 `scripts/fetch-api-header.sh`，从上游固定 commit 获取 API4 头并校验 SHA256；CMake 在头文件缺失时给出明确错误。
- 真实 ELF 认证 CTest 改为可配置 fixture：提供 `libnetd_updatable.so` 时运行，否则明确跳过，不伪装为通过。
- NDK 自动发现不再硬编码个人路径；补充公开构建、验证、许可与限制说明。

## BpfNest 0.1.0 — 本地实验交付

- 新增 API4、netd-only、精确 BuildID/callsite 认证的 ARM64 模块；默认 observe，disabled 不 hook，patch 必须显式配置。
- 增加本地构建、host/ARM64 QEMU 验证、ELF/API4 静态检查、真实管理器安装入口和带哈希清单的 ZIP 打包；不部署、不重启。
- 记录持久 MULTI、重复附加 EINVAL、原模式冲突 EPERM、Android 单槽查询 ENOSPC/abort 与非原子 query/attach 风险。patch 不宣称生产/重启安全。
- 补充中文范围、兼容性与未执行设备 A/B/C 验证清单。主机通过不代表设备兼容或网络功能已验证。
- 保留 ZygiskNext 头文件权利限制，不更改其许可归属。

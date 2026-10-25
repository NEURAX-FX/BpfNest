# 最终本地工具验证

本页替代早期千行报告及旧 a288…/5cb94…哈希；不将历史失败或旧 ZIP 当成当前产物。完整输出保留于 `../build/logs/verify.log`、`../build/logs/package.log`。

## 实际执行

最终执行 `BpfNest/scripts/package.sh`，2026-10-25T22:18:31Z（UTC），**退出0**。为排除旧对象文件影响，先删除 `build/`/`dist/` 后从当前源码完整重建；package 内执行完整 verify，并编译既有 install_concurrency 测试。`fetch-api-header.sh` 已在隔离目录实测下载固定 commit 并通过 SHA256 校验。另在干净 checkout（无设备 ELF）中实测：CMake 对缺失头文件明确报错；fetch 后 host 7/7、Android 构建、QEMU 与打包全部通过，`elf` 测试被明确跳过。

本页的 8/8 是在本地保留了目标 ELF 的工作树中执行。公开源码树不包含 `libnetd_updatable.so`，干净的公开 checkout 只会注册 7 个 CTest；CMake 配置阶段会明确说明 `elf` 测试因缺少设备镜像而跳过，不会伪装为通过。把自有镜像放到仓库根目录或设置 `-DBPFNEST_ELF_FIXTURE=<path>` 后恢复完整的 8/8。

- CTest **8/8**：core、install、elf、fd、host_smoke、reentrancy、policy_safety、install_concurrency。新增目标覆盖hook返回前原指针/Disabled门发布、重复初始化竞争、全部五个必需API指针缺失及新进程状态。
- NDK29.0.14206865，arm64-v8a，API26，目标 bpfnest 编译/链接成功。
- ELF/API 检查通过：AArch64 DYN、导出 zn_module OBJECT/size16/API4、RELRO/NOW、非执行栈证据输出。
- **ARM64 QEMU PASS**：生产 trampoline + dispatcher，七参数字、LR、私有副本、observe不查询、空查询patch/非空拒绝、返回值和输入/输出 errno。
- **TOOLING_TESTS PASS**：scope/default/hash篡改和无效/API3 ELF拒绝。
- **PACKAGE_CHECK PASS**：精确manifest、observe默认、netd-only scope、真实安装入口、payload哈希及ZIP CRC。
- bash/sh 语法检查退出0；没有设备动作。

## 最终产物

| 文件 | 字节 | SHA256 |
|---|---:|---|
| build/arm64/libbpfnest.so（dist同字节副本） | 1823920 | `6ab15a86946487dd0e36d13238fc2ec3fda5fb6151c910c5165770b7c7e9e676` |
| dist/BpfNest-v0.1.0-arm64.zip | 451593 | `bca95dbbbd98545abe1f519be5f23eef2daa87cc131e234e47ca1b0f6f1f9855` |

各有旁置 `.sha256`；ZIP内 SHA256SUMS覆盖除自身外的每个文件。旧 arm64-v8a.zip、旧外部SHA256SUMS及旧so已清理，dist仅保留最终ZIP/so与各自哈希。

ZIP根目录：module.prop、customize.sh、bpfnest.conf、zn_modules.txt、libbpfnest.so、NOTICE.txt、SHA256SUMS、META-INF真实Magisk安装入口/marker。作用域精确 `path=/system/bin/netd libbpfnest.so`，默认mode=observe；无service/post-fs-data/sepolicy脚本。

## 评审与限制

最终阅读了 core/runtime/install/CMake：active_mode在hook确认成功前保持Disabled，原指针与不可变状态先发布；重入原样转发；target/program dup在query/调用期间保留，关闭和日志后恢复原errno。maps权限过滤与复制仍非原子，不能声称消除并发mprotect/映射变化。query+attach同样非原子。

拒绝既有MULTI是明确安全限制，不是通用共存/重启方案：重复对象可EINVAL，原flags0到既有MULTI可EPERM；额外直接程序可使Android单槽query ENOSPC/abort。patch不宣称生产或重启安全。

没有设备安装/注入/BPF附加、Android网络或重启测试。主机/QEMU通过不证明设备权限、时序、RELRO hook或Android统计/策略正确。API头文件权利说明不阻止本地工程构建，也不授予再分发权利。

# BpfNest

仅针对已认证 `libnetd_updatable.so` 的 netd cgroup BPF 附加实验模块。**默认 observe；patch 不是生产安全或 netd 重启安全方案。设备所有者报告（2026-10-26）已在 MT6989 / Android 14 目标设备实测可用；仓库内自动化仍不覆盖设备注入时序与网络行为。**

公开源码树不包含设备 ELF 和 ZygiskNext 头文件；构建前先运行 `scripts/fetch-api-header.sh`。安全边界见[根目录 README](../README.md)与[第三方声明](../THIRD_PARTY_NOTICES.md)。

## 范围

- ARM64，构建最低 API26；这不是对全部 API26+ ROM 的兼容承诺。
- 唯一允许的目标 BuildID：`d114509ac20ab4037ed59923cfa032d5`。
- 仅 `/system/bin/netd`，父 cgroup 固定 `/sys/fs/cgroup`；仅该库已认证调用点的 ingress/egress、原 flags0、精确152字节 attr。
- ZygiskNext API4，以导出 `zn_module.target_api_version=4` 协商；基础 API 表没有运行时版本字段。未知镜像不启用 hook，没有 inline 或全局 syscall fallback。
- 需要支持基础模块 API4 的 ZygiskNext，未验证具体发行版本或设备组合；不是“Zygisk版本4”的泛化承诺。Android14口述与supplied ELF note35差异仍待核对，不能用平台版本代替精确BuildID认证。

## 本机构建与打包

```sh
./scripts/fetch-api-header.sh
./scripts/build.sh
./scripts/verify.sh
./scripts/package.sh
```

从 BpfNest 目录运行；脚本也可从其他目录调用。NDK 自动发现 `$HOME/Android/Sdk/ndk`、`/opt/android-sdk/ndk` 或 `ANDROID_HOME`/`ANDROID_SDK_ROOT` 下的 `ndk`，也可用 `ANDROID_NDK_HOME` 指定。输出为 `build/arm64/libbpfnest.so`、`dist/BpfNest-v0.1.0-arm64.zip` 及 SHA256。package 会重新执行完整 verify，失败不产生新的合格发布声明。日志保存在 `build/logs/`。verify 包含实际 host CTest、Android ELF/API4 检查、可用时必须通过的 QEMU 生产 trampoline+dispatcher 测试。

`fetch-api-header.sh` 从上游固定 commit 获取 API4 头文件并校验 SHA256；该头文件没有再分发授权，所以不在本仓库/ZIP 内。`libnetd_updatable.so` 同样不入库；把它放到仓库根目录后，真实 ELF 认证 CTest 才会运行，否则该测试会明确跳过（不是通过）。

ZIP 根目录含 `module.prop`、`customize.sh`、`bpfnest.conf`、`zn_modules.txt`、`libbpfnest.so`、真实 Magisk 安装入口、NOTICE 和文件哈希清单；没有 service、post-fs-data、sepolicy 脚本。安装入口沿用官方 Magisk v28.1，不是假成功 updater。仅支持管理器安装；本仓库脚本不安装、不连接设备、不重启。

## 模式

模块根目录 `bpfnest.conf` 只有一项 `mode=observe|patch|disabled`，加载时读取一次，无热更新。发行包为 `mode=observe`。配置格式错误、未知值禁用；文件缺失采用 observe。

- `observe`：检查并记录目标 attach，原样调用，不改 flags，不增加 BPF 查询。
- `patch`：显式实验选项；全部校验通过且直接查询 count0/flags0 时才修改私有副本中的 ALLOW_MULTI。不修改原 attr、不 detach、不改 map、不放宽安全策略。
- `disabled`：不安装 hook。

## 手动安装、观察与禁用

以下设备操作仅供设备所有者手动执行，**本次未执行**，脚本不会代办。

1. 在本机构建并校验 `dist/BpfNest-v0.1.0-arm64.zip` 及旁置SHA256；不要使用旧包。
2. 在支持的 Magisk/KernelSU 管理器选择“从本地安装”，手动选择该ZIP；不支持recovery安装。确认ZygiskNext已安装且支持基础API4。保留默认observe。
3. 由所有者自行安排下一次受控重启后观察；安装不会将模块注入当前运行的netd，不要为生效单独重启netd。
4. 只读日志命令（未执行）：`adb logcat -s BpfNest:V`。若当前ZygiskNext支持，可在有权限的设备shell执行 `zygisk-ctl dump-zn -sa` 检查netd作用域；该列表不证明hook或首次attach命中。

`module loaded`日志含 `install_result`：0=Installed、1=AlreadyAttempted、2=WrongProcess、3=Disabled、4=InvalidApi、5=ResolverFailed、6=UncertifiedImage、7=UnexpectedHook、8=HookFailed。0只证明hook API报告成功，仍须看到目标attach事件及其result/errno；没有attach事件不能认定首次附加已覆盖。mode日志也不能代替install_result。

配置路径为 `/data/adb/modules/bpfnest/bpfnest.conf`。用root权限文本编辑器只保留 `mode=observe`（观察）或 `mode=disabled`（不hook），不要加未知项；加载时读取一次，修改后由所有者自行安排受控重启，无热更新。因patch存在明确重启/持久MULTI风险，此处不提供安全启用patch的步骤，也不宣称它可以安全启用。

要禁用注入，可在root管理器关闭BpfNest；等价的手动root设备shell操作为 `touch /data/adb/modules/bpfnest/disable`，再由所有者安排受控重启。要卸载，在管理器移除模块并安排受控重启。**不能实时卸载已加载代码，也不能靠disabled、disable标记或卸载撤销既有BPF attachment；之前patch留下的MULTI也不会被这些操作恢复。** 不自动detach或重启关键服务。

ALLOW_MULTI使符合条件的Android祖先程序继续执行；ALLOW_OVERRIDE会允许子级替代祖先，因此不采用。详见[附加路径与安全限制](reports/attach-path.md)。

## 不可忽略的限制

1. 注入前已有 mode0、错过首次 attach，均无法修复。
2. query 与 attach 非原子；并发附加存在竞争。
3. MULTI 持续存在时，重复同一 pinned program 对象可能 EINVAL；拒绝修改后原 flags0 对既有 MULTI 可能 EPERM。进程内 once 标志不解决 netd 重启。
4. 同类型额外直接程序可能使 Android 自身单槽 query 返回 ENOSPC 并 abort。不能用伪造成功、静默跳过、detach 或 REPLACE 掩盖。
5. 保留 Android 父程序执行不等于已经证明统计、包副作用与网络限制完全不变。

因此不要在生产系统启用 patch，也不要为测试主动重启 netd。设备 A/B/C 验证清单见 [device-validation](reports/device-validation.md)；实际本地结果见 [tooling-verification](reports/tooling-verification.md)。静态定位沿用[根目录预研](../reports/BpfNest-preliminary-analysis.md)，其中重启/inline fallback 等早期建议已由本页限制取代。

## 证据与权利

[附加路径](reports/attach-path.md) · [ELF 边界](reports/elf-analysis.md) · [兼容性](reports/compatibility.md) · [变更记录](CHANGELOG.md)。原创代码以 GPL-3.0-only 发布；AOSP 参考片段为 Apache-2.0，Magisk 安装器为 GPL-3.0，详见[第三方声明](../THIRD_PARTY_NOTICES.md)与[许可证](../LICENSE)。

API4 头文件权利说明见 `third_party/NOTICE.txt`；不得将其标成 GPL/MIT/Apache。本仓库不重分发该头文件，`fetch-api-header.sh` 只从上游固定 commit 下载；生成本地包不代表已经取得再分发授权。

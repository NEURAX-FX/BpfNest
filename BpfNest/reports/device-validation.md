# 设备验证清单（未执行）

> 2026-10-26 注：设备所有者报告已在 MT6989 / Android 14 目标设备实测可用。本页保留实现阶段的“未执行”状态与验证清单；具体设备日志、模式和 A/B/C 结果未整理进本页。

**本次没有安装模块、连接设备、附加/分离程序、修改 map、安全策略或重启手机/netd。下列是人工受控验证方案，不是完成记录。** patch 的持久 MULTI/重启风险未消除，不建议生产启用。

## 前置只读证据

记录 fingerprint、平台 SDK、内核、Tethering APEX 版本、实际映射库路径和 BuildID；核对唯一允许的 BuildID。保存安装前 netd PID/存活、相关日志、AVC、根和容器所有祖先 ingress/egress 直接 attachment 的 ID/flags。直接查询与 effective 查询分别标注，不用 effective 的 flags0 判断附加模式。不要将查询失败当成空集合。

## A：observe 与 Android 基线

由设备所有者在其受控维护流程中决定加载；本仓库不自动部署或重启。观察 module loaded、hook 结果、egress/ingress 请求和原 syscall 返回；确认日志顺序早于 Android 初始化成功。只有 module loaded 而无 attach，不足以断言太晚，还可能是 hook/认证/过滤未命中。

核对 netd 存活、无新增 AVC/crash，Android 原程序 ID/flags 不变；对比 Wi-Fi/蜂窝、DNS、流量统计、UID 前后台/省电/拒绝规则、热点和网络共享。观察模式不应增加 BPF query 或修改 attach flags。

## B：层级与父程序保留（仅专用可恢复测试环境）

进入 patch 前必须明确接受风险并证明目标 direct count0/flags0；既有 mode0/MULTI 或错过首次附加即停止，不做 detach 补救。分别确认原 Android 程序 ID保留、根 ingress/egress flags=MULTI、子级专用 cgroup 可附加、父子程序确实执行、子级不能抵消父级拒绝。记录计数/规则效果，不把 load 成功当作 attach 成功，也不把 attach 成功当作策略不变。

不要安排 netd 重启测试：重复程序 EINVAL、模式冲突 EPERM 和 Android 单槽查询 ENOSPC/abort 是明确风险。query+attach 并发竞争仍须列入结果，不得隐藏。

## C：专用 systemd service

仅在可丢弃容器/测试服务中逐项测试 `IPAccounting=yes`、`IPAddressDeny=any`、明确的 `IPAddressAllow=`；保留成功与拒绝目标流量的双向对照、服务日志、计数和实际网络结果。不修改关键 Android 或宿主服务。

仍 EPERM 时区分祖先 mode0、CAP_BPF/CAP_NET_ADMIN、SELinux/seccomp、cgroup delegation 与 namespace；本模块不解决这些权限问题。停止条件：新增 crash/AVC、Android 限制减弱或统计异常。停止后仅记录并退出实验，不自动放宽策略、分离程序或重启。恢复动作需设备所有者单独批准。

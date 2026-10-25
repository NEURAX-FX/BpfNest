# 附加路径与边界

静态定位复用[预研报告](../../reports/BpfNest-preliminary-analysis.md)，不重复反向分析。

路径：netd main → libnetd_updatable_init → BpfHandler::init → attachProgramToCgroup → **该库自身** syscall PLT。不是 libbpf_android 的加载/map 操作，也不是全局 libc hook。

仅允许 BuildID `d114509ac20ab4037ed59923cfa032d5`，函数 VA0x52d0/size0x20c，BL0x53a4→PLT0xac70，LR0x53a8，attr152，flags0，ingress/egress，固定父 cgroup。观察模式原样转发，不添加 BPF_PROG_QUERY；patch 仅接受直接 count0/flags0。任何不匹配都不修改参数。没有伪造成功、detach、REPLACE、map 更改或 inline fallback。

Linux6.1 MULTI 保留符合条件的祖先程序执行，子程序先执行；不能以此替代 Android 网络策略/统计 A/B 验证。既有 mode0 不可静默迁移；首次附加已错过则无法修复。query+attach 不是一个事务。

**重启限制**：同一 pinned program 对象重复 MULTI 附加会 EINVAL；若拒绝 patch、原 flags0 到已有 MULTI，则可能 EPERM。额外同类型直接程序还可能使 Android 单槽 query ENOSPC/abort。进程内 once、默认 observe、非空拒绝都不将 patch 变成重启安全。不得通过吞掉错误或伪造 no-op 成功回避。

既有源码包含 query 调用只证明代码路径存在，不证明设备运行时权限成功。所有设备结果仍未验证。

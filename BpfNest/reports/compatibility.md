# 兼容性与已知限制

| 项目 | 范围/状态 |
|---|---|
| 目标进程 | 精确 `/system/bin/netd` |
| 目标父级 | 固定 `/sys/fs/cgroup`，cgroup2 身份检查 |
| 库 BuildID | 仅 `d114509ac20ab4037ed59923cfa032d5` |
| ABI/最低链接 API | arm64-v8a / 26；不代表全 API26+ 设备支持 |
| 注入 API | ZygiskNext API4，loader 协商 target_api_version；基础表无运行时版本字段 |
| 包作用域 | 根目录 zn_modules.txt：`path=/system/bin/netd libbpfnest.so` |
| 默认 | observe；错误/未知配置 disabled；disabled 不 hook |
| BPF 查询 | patch 才查询，direct、count-only；任何失败原样调用 |
| 设备注入/网络验证 | 未执行，不宣称支持具体 ROM |
| fd close/reuse、权限无效 attr | patch 持有 target/program dup fd 到调用完成；maps 读权限过滤+故障隔离读取已覆盖主机测试，但 maps 与复制间的并发 mprotect/映射变化仍非原子，不宣称完全消除竞态或设备已通过 |

MULTI 不会静默迁移已有 mode0。已错过第一次 attach 无修复能力。query 与 attach 不原子。netd 重启/重复 init 对持久 MULTI 可能 EINVAL；退回 flags0 可能 EPERM；额外直接程序可能触发 Android 单槽 query ENOSPC/abort。**patch 不是生产安全、重启安全或无停机切换功能。**

既有MULTI即使在初始计划中曾考虑允许，也明确不转换：Linux6.1相同pinned对象重复附加可EINVAL，额外直接程序可触发netd单槽查询ENOSPC/abort；仅direct count0/flags0具备候选资格。这是证据驱动的安全停止，不是完整通用attach兼容性承诺；默认observe可原样检查这些请求。

不提供 detach/map 清空/权限放宽/全局 syscall/inline fallback/自动重启。配置加载一次；禁用/卸载不会改变已经存在的内核 attachment，亦不撤回已加载模块。请勿为了验证触发关键服务重启。

头文件与安装入口各自保留原权利，不将 ZygiskNext 头文件重标 GPL。见 `third_party/NOTICE.txt` 与包内 NOTICE。

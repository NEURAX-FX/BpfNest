# ELF 认证与构建检查

只支持 supplied libnetd_updatable.so BuildID `d114509ac20ab4037ed59923cfa032d5`，不是通用 ROM 特征匹配。

认证必须同时满足：ARM64 little-endian DYN、精确本地符号 VA0x52d0/size0x20c、目标指令完整区域0x5378..0x53a4、PLT0xac70 指令、GOT0xc478 的 syscall JUMP_SLOT、已知 BuildID。禁止仅凭名字/范围/BL 指令接受未知布局。原始定位见[预研](../../reports/BpfNest-preliminary-analysis.md)。

`./scripts/inspect-elf.sh --module build/arm64/libbpfnest.so` 检查架构、DYN、导出的 GLOBAL DEFAULT OBJECT zn_module/size16/API4，并输出哈希、BuildID、依赖、RELRO/NOW/栈和反汇编证据。它只解析本地文件，不执行目标。`verify.sh` 额外运行生产 ARM64 trampoline+dispatcher 的 QEMU 测试（安装 QEMU 时不得跳过）。

静态证据不能证明设备注入时序、RELRO hook 成功、BPF_PROG_QUERY 权限、seccomp/SELinux 允许或 Android 网络行为。ELF Android SDK note 与设备口述版本的差异仍是来源核对项，不能自动推断 ROM 版本。

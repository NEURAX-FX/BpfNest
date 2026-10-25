.text
.p2align 2
.global _start
.type _start,%function
_start:
    mov x19,#0
    mov x21,#0x321
    mov x29,#0x456
    mov x0,#0x110
    mov x1,#0x221
    mov x2,#0x332
    mov x3,#0x443
    mov x4,#0x554
    mov x5,#0x665
    mov x6,#0x776
    adr x20,capture_return
    bl bpfnest_syscall_entry
capture_return:
    cmp x0,#0x777
    b.ne failed
    cmp x21,#0x321
    b.ne failed
    cmp x29,#0x456
    b.ne failed
    mov x19,#1
    // A real six-argument mmap: invalid sixth-argument offset must yield EINVAL.
    mov x0,#222
    mov x1,#0
    mov x2,#4096
    mov x3,#3
    mov x4,#0x22
    mov x5,#-1
    mov x6,#1
    adr x20,bad_mmap_return
    bl bpfnest_syscall_entry
bad_mmap_return:
    cmn x0,#22
    b.ne failed
    mov x0,#222
    mov x1,#0
    mov x2,#4096
    mov x3,#3
    mov x4,#0x22
    mov x5,#-1
    mov x6,#0
    adr x20,mmap_return
    bl bpfnest_syscall_entry
mmap_return:
    cmp x0,#0
    b.lt failed
    mov x22,x0
    mov w9,#0x5a
    strb w9,[x22]
    ldrb w10,[x22]
    cmp w10,#0x5a
    b.ne failed
    mov x0,#215
    mov x1,x22
    mov x2,#4096
    mov x3,#0
    mov x4,#0
    mov x5,#0
    mov x6,#0
    adr x20,unmap_return
    bl bpfnest_syscall_entry
unmap_return:
    cbnz x0,failed
    mov x0,#172
    adr x20,pid_return
    bl bpfnest_syscall_entry
pid_return:
    cmp x0,#0
    b.le failed
    mov x0,#0xffff
    adr x20,invalid_return
    bl bpfnest_syscall_entry
invalid_return:
    cmn x0,#38
    b.ne failed
    mov x8,#64
    mov x0,#1
    adr x1,success
    mov x2,#success_end-success
    svc #0
    mov x0,#0
    b exit
failed:
    mov x8,#64
    mov x0,#2
    adr x1,failure
    mov x2,#failure_end-failure
    svc #0
    mov x0,#1
exit:
    mov x8,#93
    svc #0
.size _start,.-_start

.global bpfnest_dispatch_frame
.hidden bpfnest_dispatch_frame
.type bpfnest_dispatch_frame,%function
bpfnest_dispatch_frame:
    ldr x9,[x0,#56]
    cmp x9,x20
    b.ne failed
    and x9,x0,#15
    cbnz x9,failed
    cbnz x19,real_syscall
    ldp x9,x10,[x0]
    cmp x9,#0x110
    b.ne failed
    cmp x10,#0x221
    b.ne failed
    ldp x9,x10,[x0,#16]
    cmp x9,#0x332
    b.ne failed
    cmp x10,#0x443
    b.ne failed
    ldp x9,x10,[x0,#32]
    cmp x9,#0x554
    b.ne failed
    cmp x10,#0x665
    b.ne failed
    ldr x9,[x0,#48]
    cmp x9,#0x776
    b.ne failed
    mov x0,#0x777
    ret
real_syscall:
    mov x10,x0
    ldr x8,[x10]
    ldp x0,x1,[x10,#8]
    ldp x2,x3,[x10,#24]
    ldp x4,x5,[x10,#40]
    svc #0
    ret
.size bpfnest_dispatch_frame,.-bpfnest_dispatch_frame
.section .rodata
success: .ascii "aarch64-smoke: x0..x6/LR/callee-saved and real mmap sixth argument/getpid/ENOSYS passed\n"
success_end:
failure: .ascii "aarch64-smoke: FAILED register/syscall check\n"
failure_end:
.section .note.GNU-stack,"",%progbits

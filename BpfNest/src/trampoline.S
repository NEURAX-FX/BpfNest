.text
.p2align 2
.global bpfnest_syscall_entry
.hidden bpfnest_syscall_entry
.type bpfnest_syscall_entry, %function
bpfnest_syscall_entry:
    .cfi_startproc
    hint #34
    sub sp, sp, #80
    .cfi_def_cfa_offset 80
    stp x29, x30, [sp, #64]
    .cfi_offset x29, -16
    .cfi_offset x30, -8
    stp x0, x1, [sp]
    stp x2, x3, [sp, #16]
    stp x4, x5, [sp, #32]
    stp x6, x30, [sp, #48]
    mov x29, sp
    mov x0, sp
    bl bpfnest_dispatch_frame
    ldp x29, x30, [sp, #64]
    add sp, sp, #80
    .cfi_def_cfa_offset 0
    ret
    .cfi_endproc
.size bpfnest_syscall_entry, .-bpfnest_syscall_entry
.section .note.GNU-stack,"",%progbits

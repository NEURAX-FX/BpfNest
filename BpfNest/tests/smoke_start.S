.text
.global _start
.type _start, %function
_start:
    bl smoke_main
    mov x8, #93
    svc #0
.size _start, .-_start
.global smoke_call
.type smoke_call, %function
smoke_call:
    stp x29, x30, [sp, #-16]!
    bl bpfnest_syscall_entry
.global smoke_return
smoke_return:
    ldp x29, x30, [sp], #16
    ret
.size smoke_call, .-smoke_call
.section .note.GNU-stack,"",%progbits

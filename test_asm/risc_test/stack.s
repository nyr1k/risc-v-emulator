.section .text
.global _start

_start:
    addi sp, sp, -16

    li t0, 99
    sw t0, 0(sp)

    lw t1, 0(sp)

    addi sp, sp, 16

halt:
    j halt

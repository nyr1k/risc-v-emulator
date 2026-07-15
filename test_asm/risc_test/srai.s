.section .text
.global _start

_start:
    li t0, -16

    srai t1, t0, 2
    srli t2, t0, 2

halt:
    j halt

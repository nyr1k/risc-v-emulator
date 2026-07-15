.section .text
.global _start

_start:
    li t0, 1

    slli t1, t0, 8
    srli t2, t1, 4
    srai t3, t1, 4

halt:
    j halt

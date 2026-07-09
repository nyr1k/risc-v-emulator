.section .text
.global _start

_start:
    li t0, 30
    li t1, 18

    add t2, t0, t1
    sub t3, t0, t1

halt:
    j halt

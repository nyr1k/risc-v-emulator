.section .text
.global _start

_start:
    li t0, 0xAA
    li t1, 0x55

    and t2, t0, t1
    or  t3, t0, t1
    xor t4, t0, t1

halt:
    j halt

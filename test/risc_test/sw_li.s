.section .text
.global _start

_start:
    li t0, 0x80001000
    li t1, 0x12345678

    sw t1, 0(t0)

    lw t2, 0(t0)

halt:
    j halt

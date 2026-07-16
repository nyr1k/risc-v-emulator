.section .text
.global _start

_start:
    li t0, 0x80001000

    li t1, 0x11
    sb t1, 0(t0)

    li t1, 0x22
    sb t1, 1(t0)

    li t1, 0x33
    sb t1, 2(t0)

    li t1, 0x44
    sb t1, 3(t0)

    lw t2, 0(t0)

halt:
    j halt

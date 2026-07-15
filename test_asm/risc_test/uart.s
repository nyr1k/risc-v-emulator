.section .text

.global _start

_start:
    li t0, 0x10000000
    li t1, 74
    sw t1, 0(t0)

    li a7, 10
    ecall
    
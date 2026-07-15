.section .text
.global _start

_start:
    li t0, 5
    li t1, 6

    beq t0, t1, equal

    li t2, 111
    j halt

equal:
    li t2, 222

halt:
    j halt

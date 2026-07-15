.section .text
.global _start

_start:
    jal ra, func

    li t0, 1

halt:
    j halt

func:
    li t1, 2
    ret

.section .text
.global _start

_start:
    li a0, 5
    li a1, 7

    call add_numbers

    mv t0, a0

halt:
    j halt

add_numbers:
    add a0, a0, a1
    ret

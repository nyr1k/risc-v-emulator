.section .text
.global _start 

_start:
  li t0, 123
  li t1, -5

  ebreak

halt:
  j halt

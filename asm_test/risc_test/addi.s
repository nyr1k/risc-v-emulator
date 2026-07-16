.section .text
.global _start 

_start:
  li t0, 123
  li t1, -5
  
  ebreak

  li a7, 93
  li a0, 69
  ecall
halt:
  j halt

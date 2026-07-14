.section .text
.global _start 

_start:
  li t0, 123
  li t1, -5
  
  ebreak

  li a7, 10
  ecall
halt:
  j halt

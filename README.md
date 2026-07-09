# riscv32I emulator 
A riscv32i emulator I decided to write to improve my C and assembly skills, and to learn how emulators and ELF files work.  
It currently supports ELF loading, subset of rv32i instruction set, and execution of simple assembly programs. 

## Features
- ELF loading
- rv32i instruction set
- 2 debug functions (inspect_ram, dump_cpu)
- test ELFs

## Build & Run
`make`
`./rv32i test/bin/NAME.elf`

## Example 
`./rv32i test/bin/addi.elf`

![Alt text](/ex.png)

## TODO
- Refactor ELF loader because I don't like the structure
- Lookup tables instead of large switches
- Add UART
- Accept non-contiguous memory addresses in inspect_ram()
- etc.

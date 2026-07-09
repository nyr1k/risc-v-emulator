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

## Custom tests
If you want to do your custom tests, then you will have to get riscv-gnu-toolchain from https://github.com/riscv-collab/riscv-gnu-toolchain.

`sudo dnf install autoconf automake libmpc-devel mpfr-devel gmp-devel gawk bison flex texinfo patchutils gcc gcc-c++ zlib-devel expat-devel git`

`git clone --recursive https://github.com/riscv-collab/riscv-gnu-toolchain
cd riscv-gnu-toolchain
./configure --prefix=/opt/riscv --with-arch=rv32i --with-abi=ilp32
make -j$(nproc)`         

Then, you should add `/opt/riscv/bin` to your PATH. 
`export PATH="/opt/riscv/bin:$PATH"`

After that, you can write test assembly program in `test/risc_test/` and `make`.

## TODO
- Refactor ELF loader because I don't like the structure
- Lookup tables instead of large switches
- Add UART
- Accept non-contiguous memory addresses in inspect_ram()
- etc.

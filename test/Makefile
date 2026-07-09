.PHONY: all clean

AS = riscv32-unknown-elf-as
LD = riscv32-unknown-elf-ld
AS_FLAGS = -march=rv32i 
LD_FLAGS = -T linker.ld

SRC = $(wildcard risc_test/*.s)
OBJ = $(patsubst risc_test/%.s, build/%.o, $(SRC))
ELF = $(patsubst build/%.o, bin/%.elf, $(OBJ))    

all: $(ELF)

build/%.o: risc_test/%.s
	@mkdir -p build
	$(AS) $(AS_FLAGS) -o $@ $<

bin/%.elf: build/%.o linker.ld
	@mkdir -p bin
	$(LD) $(LD_FLAGS) -o $@ $<

clean: 
	rm -rf bin build

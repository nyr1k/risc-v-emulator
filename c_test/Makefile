.PHONY: all clean
.SECONDARY: $(OBJS)

CC = riscv32-unknown-elf-gcc 
AS = riscv32-unknown-elf-as
LD = riscv32-unknown-elf-ld

CC_FLAGS = -march=rv32i -mabi=ilp32 -c
AS_FLAGS = -march=rv32i 
LD_FLAGS = -T linker.ld

CRT0 = lib/crt0.o
SRC_DIR = tests

SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(patsubst $(SRC_DIR)/%.c, build/%.o, $(SRCS))
ELFS = $(patsubst build/%.o, bin/%.elf, $(OBJS))    

all: $(ELFS)

build/%.o: $(SRC_DIR)/%.c
	@mkdir -p build
	$(CC) $(CC_FLAGS) -o $@ $<

bin/%.elf: build/%.o linker.ld
	@mkdir -p bin
	$(LD) $(LD_FLAGS) -o $@ $< $(CRT0)

clean: 
	rm -rf bin build


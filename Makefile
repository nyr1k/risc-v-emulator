CC = gcc
CFLAGS = -std=gnu99 -Wall -Wextra -Iinclude -O2 
AS = nasm
SRC_DIR = src
BUILD_DIR = build
TARGET_BUILD = rv32i

SRC = $(wildcard $(SRC_DIR)/*.c)
OBJ = $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(SRC))
OBJ += $(BUILD_DIR)/elf_loader.o

.PHONY: all run clean

all: $(TARGET_BUILD)

$(TARGET_BUILD): $(OBJ) 		
	$(CC) $(CFLAGS) -o $@ $(OBJ)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@ 

$(BUILD_DIR)/elf_loader.o: $(SRC_DIR)/asm/elf_loader.s
	@mkdir -p $(BUILD_DIR)
	$(AS) -felf64 $(SRC_DIR)/asm/elf_loader.s -o $@

clean:
	rm -rf $(BUILD_DIR) $(TARGET_BUILD)

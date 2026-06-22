CC = gcc
CFLAGS = -std=gnu99 -Wall -Wextra -Iinclude -O2 
SRC_DIR = src
BUILD_DIR = build
TARGET_BUILD = rv32i

all: $(wildcard $(SRC_DIR)/*.c)
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -o $(TARGET_BUILD) $(SRC_DIR)/*.c 
	@printf "\n[OK] Running...\n\n"
	@./$(TARGET_BUILD) 

clean:
	rm -rf $(BUILD_DIR) $(TARGET_BUILD)

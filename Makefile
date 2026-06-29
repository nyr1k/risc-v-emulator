CC = gcc
CFLAGS = -std=gnu99 -Wall -Wextra -Iinclude -O2 
SRC_DIR = src
BUILD_DIR = build
TARGET_BUILD = rv32i

SRC = $(wildcard $(SRC_DIR)/*.c)
OBJ = $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(SRC))

.PHONY: all run clean

all: $(TARGET_BUILD)

$(TARGET_BUILD): $(OBJ) 		
	$(CC) $(CFLAGS) -o $@ $(OBJ)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@ 
	
run: $(TARGET_BUILD)
	@printf "\n[OK] Running...\n\n"
	@./$(TARGET_BUILD)  

clean:
	rm -rf $(BUILD_DIR) $(TARGET_BUILD)

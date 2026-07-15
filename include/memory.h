#ifndef MEMORY_H
#define MEMORY_H

#include <stdint.h> 

enum : uint32_t {
  MEMORY_SIZE  = 1024 * 1024 * 4, // 4 MiB 
  BASE_ADDRESS = 0x80000000,
  END_ADDRESS  = (BASE_ADDRESS + MEMORY_SIZE),
  UART_ADDRESS    = 0x10000000,
  STACK_TOP    = END_ADDRESS,
  STACK_LIMIT  = (STACK_TOP - 0x00001000) // max stack size = 4 KiB
};

typedef struct {
  uint8_t memory[MEMORY_SIZE]; 
} ram_t;    

void memory_init(ram_t *ram); 

uint32_t read_word(const ram_t *ram, uint32_t address);
void write_word(ram_t *ram, uint32_t address, uint32_t data);

uint16_t read_halfword(const ram_t *ram, uint32_t address);
void write_halfword(ram_t *ram, uint32_t address, uint16_t data);

uint8_t read_byte(const ram_t *ram, uint32_t address);
void write_byte(ram_t *ram, uint32_t address, uint8_t data);

void inspect_ram(const ram_t *ram, uint32_t address, uint32_t words_num);

#endif 

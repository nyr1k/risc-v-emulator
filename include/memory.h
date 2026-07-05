#ifndef MEMORY_H
#define MEMORY_H

#include <stdint.h> 

enum : uint32_t {
  MEMORY_SIZE  = 1024 * 1024 * 4, // 4MB 
  BASE_ADDRESS = 0x80000000,
  MMIO_BASE    = 0x10000000,
  END_ADDRESS  = (BASE_ADDRESS + MEMORY_SIZE)
};

typedef struct {
  uint8_t memory[MEMORY_SIZE]; 
} ram_t;    

typedef enum : uint8_t {
  INVALID_ADDRESS,
  MMIO_ADDRESS,
  RAM_ADDRESS
} memory_segment;

void memory_init(ram_t *ram); 

uint32_t read_word(ram_t ram, uint32_t address);
void write_word(ram_t *ram, uint32_t address, uint32_t data);

uint16_t read_halfword(ram_t ram, uint32_t address);
void write_halfword(ram_t *ram, uint32_t address, uint16_t data);

uint8_t read_byte(ram_t ram, uint32_t address);
void write_byte(ram_t *ram, uint32_t address, uint8_t data);

#endif 

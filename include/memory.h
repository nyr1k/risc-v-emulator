#ifndef MEMORY_H
#define MEMORY_H

#include <stdint.h> 

#define MEMORY_SIZE 1024 * 1024 * 4 /* 4 MB memory */

#define BASE_ADDRESS 0x80000000
#define MMIO_BASE 0x10000000
#define END_ADDRESS (BASE_ADDRESS + MEMORY_SIZE)

typedef struct {
  uint8_t memory[MEMORY_SIZE]; 
} ram_t;    

typedef enum : uint8_t {
  INVALID_ADDRESS,
  MMIO_ADDRESS,
  RAM_ADDRESS
} memory_segment;

void memory_init(); 

uint32_t read_word(uint32_t address);
void write_word(uint32_t address, uint32_t data);

uint16_t read_halfword(uint32_t address);
void write_halfword(uint32_t address, uint16_t data);

uint8_t read_byte(uint32_t address);
void write_byte(uint32_t address, uint8_t data);

#endif 

#include <stdint.h> 
#include <string.h>

#include <memory.h>
#include <err.h>

ram_t ram;

static inline int check_address(const uint32_t address)
{
    if (address >= BASE_ADDRESS && address < BASE_ADDRESS+MEMORY_SIZE) return RAM_ADDRESS;
    else if (address == MMIO_BASE) return MMIO_ADDRESS;
    else report_and_abort(INVALID_MEMORY_ACCESS);
}

void memory_init() 
{
    memset(ram.memory, 0, MEMORY_SIZE);
}

uint32_t read_word(uint32_t address) 
{
    if (check_address(address) != RAM_ADDRESS) report_and_abort(INVALID_READ_ADDRESS);
    
    uint32_t true_address = address - BASE_ADDRESS; 
    uint32_t word = 0;

    word += (uint32_t)ram.memory[true_address];
    word += ((uint32_t)ram.memory[true_address+1] << 8);
    word += ((uint32_t)ram.memory[true_address+2] << 16);
    word += ((uint32_t)ram.memory[true_address+3] << 24);
    
    return word;
}
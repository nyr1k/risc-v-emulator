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
    memset(ram.memory, 0, sizeof(ram.memory));
}

uint32_t read_word(uint32_t address) 
{
    if (check_address(address) != RAM_ADDRESS) report_and_abort(INVALID_READ_ADDRESS);
    
    uint32_t true_address = address - BASE_ADDRESS; 
    uint32_t word = 0;

    /* RISC-V uses little-endian mode */
    word += (uint32_t)ram.memory[true_address];
    word += ((uint32_t)ram.memory[true_address+1] << 8);
    word += ((uint32_t)ram.memory[true_address+2] << 16);
    word += ((uint32_t)ram.memory[true_address+3] << 24);
    
    return word;
}

void write_word(uint32_t address, uint32_t data) 
{
    switch (check_address(address)) {
        case INVALID_ADDRESS:
            report_and_abort(INVALID_WRITE_ADDRESS);
            break;

        case RAM_ADDRESS:
            uint32_t true_address = address - BASE_ADDRESS;
            
            /* RISC-V uses little-endian mode */
            ram.memory[true_address]   = (uint8_t)(data & 0x000000FF);
            ram.memory[true_address+1] = (uint8_t)((data & 0x0000FF00) >> 8);
            ram.memory[true_address+2] = (uint8_t)((data & 0x00FF0000) >> 16);
            ram.memory[true_address+3] = (uint8_t)((data & 0xFF000000) >> 24);

            break;
            
        default:
            break;
    }
}
#include <stdint.h>
#include <string.h>

#include <memory.h>
#include <err.h>

static inline int check_address(const uint32_t address)
{
    if (address >= BASE_ADDRESS && address < BASE_ADDRESS+MEMORY_SIZE) return RAM_ADDRESS;
    else if (address == MMIO_BASE) return MMIO_ADDRESS;
    else report_and_abort(INVALID_MEMORY_ACCESS);
    return 0;
}

void memory_init(ram_t *ram) 
{
    memset(ram->memory, 0, sizeof(ram->memory));
}

uint32_t read_word(ram_t ram, uint32_t address) 
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

void write_word(ram_t *ram, uint32_t address, uint32_t data) 
{
    switch (check_address(address)) {
        case INVALID_ADDRESS:
            report_and_abort(INVALID_WRITE_ADDRESS);
            break;

        case RAM_ADDRESS: {
            uint32_t true_address = address - BASE_ADDRESS;
            
            /* RISC-V uses little-endian mode */
            ram->memory[true_address]   = (uint8_t)(data & 0x000000FF);
            ram->memory[true_address+1] = (uint8_t)((data & 0x0000FF00) >> 8);
            ram->memory[true_address+2] = (uint8_t)((data & 0x00FF0000) >> 16);
            ram->memory[true_address+3] = (uint8_t)((data & 0xFF000000) >> 24);

            break;
        }
        default:
            break;
    }
}

uint16_t read_halfword(ram_t ram, uint32_t address)
{
    if (check_address(address) != RAM_ADDRESS) report_and_abort(INVALID_READ_ADDRESS);
    
    uint32_t true_address = address - BASE_ADDRESS; 
    uint16_t halfword = 0;

    /* RISC-V uses little-endian mode */
    halfword += (uint16_t)ram.memory[true_address];
    halfword += ((uint16_t)ram.memory[true_address+1] << 8);
    
    return halfword;
}

void write_halfword(ram_t *ram, uint32_t address, uint16_t data)
{
    switch (check_address(address)) {
        case INVALID_ADDRESS:
            report_and_abort(INVALID_WRITE_ADDRESS);
            break;

        case RAM_ADDRESS: {
            uint32_t true_address = address - BASE_ADDRESS;
            
            /* RISC-V uses little-endian mode */
            ram->memory[true_address]   = (uint8_t)(data & 0x00FF);
            ram->memory[true_address+1] = (uint8_t)((data & 0xFF00) >> 8);

            break;
        }
        default:
            break;
    }
}

uint8_t read_byte(ram_t ram, uint32_t address)
{
    if (check_address(address) != RAM_ADDRESS) report_and_abort(INVALID_READ_ADDRESS);
    
    uint32_t true_address = address - BASE_ADDRESS; 
    uint8_t byte = 0;

    /* RISC-V uses little-endian mode */
    byte += (uint8_t)ram.memory[true_address];
    
    return byte;
}

void write_byte(ram_t *ram, uint32_t address, uint8_t data)
{
    switch (check_address(address)) {
        case INVALID_ADDRESS:
            report_and_abort(INVALID_WRITE_ADDRESS);
            break;

        case RAM_ADDRESS: {
            uint32_t true_address = address - BASE_ADDRESS;
            
            /* RISC-V uses little-endian mode */
            ram->memory[true_address]   = (uint8_t)(data & 0xFF);

            break;
        }
        default:
            break;
    }
}
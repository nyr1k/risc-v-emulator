#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include <memory.h>
#include <err.h>

#include <devices/uart.h>

static inline void check_address(const uint32_t address, const uint32_t size)
{
    if (address >= BASE_ADDRESS && address < END_ADDRESS && size <= END_ADDRESS - address) 
        return ;
    if (address == UART_ADDRESS) 
        return;
     
    report_and_abort(INVALID_MEMORY_ACCESS);
}

void memory_init(ram_t *ram) 
{
    memset(ram->memory, 0, sizeof(ram->memory));
    printf("[OK] RAM INIT SUCCESS\n");
}

uint32_t read_word(const ram_t *ram, const uint32_t address) 
{
    check_address(address, 4);
    
    uint32_t true_address = address - BASE_ADDRESS; 
    uint32_t word = 0;

    /* RISC-V uses little-endian mode */
    word += (uint32_t)ram->memory[true_address];
    word += ((uint32_t)ram->memory[true_address+1] << 8);
    word += ((uint32_t)ram->memory[true_address+2] << 16);
    word += ((uint32_t)ram->memory[true_address+3] << 24);
    
    return word;
}

void write_word(ram_t *ram, const uint32_t address, const uint32_t data) 
{
    check_address(address, 4);

    if (address == UART_ADDRESS) {
        uart_tx(data & 0xFF);
        return;
    }

    uint32_t true_address = address - BASE_ADDRESS;
    
    /* RISC-V uses little-endian mode */
    ram->memory[true_address]   = (uint8_t)(data & 0x000000FF);
    ram->memory[true_address+1] = (uint8_t)((data & 0x0000FF00) >> 8);
    ram->memory[true_address+2] = (uint8_t)((data & 0x00FF0000) >> 16);
    ram->memory[true_address+3] = (uint8_t)((data & 0xFF000000) >> 24);
}

uint16_t read_halfword(const ram_t *ram, const uint32_t address)
{
    check_address(address, 2);
    
    uint32_t true_address = address - BASE_ADDRESS; 
    uint16_t halfword = 0;

    /* RISC-V uses little-endian mode */
    halfword += (uint16_t)ram->memory[true_address];
    halfword += ((uint16_t)ram->memory[true_address+1] << 8);
    
    return halfword;
}

void write_halfword(ram_t *ram, const uint32_t address, const uint16_t data)
{
    check_address(address, 2);
    
    if (address == UART_ADDRESS) {
        uart_tx(data & 0xFF);
        return;
    }

    uint32_t true_address = address - BASE_ADDRESS;
    
    /* RISC-V uses little-endian mode */
    ram->memory[true_address]   = (uint8_t)(data & 0x00FF);
    ram->memory[true_address+1] = (uint8_t)((data & 0xFF00) >> 8);
}

uint8_t read_byte(const ram_t *ram, const uint32_t address)
{
    check_address(address, 1);
    
    uint32_t true_address = address - BASE_ADDRESS; 
    uint8_t byte = 0;

    /* RISC-V uses little-endian mode */
    byte += (uint8_t)ram->memory[true_address];
    
    return byte;
}

void write_byte(ram_t *ram, const uint32_t address, const uint8_t data)
{
    check_address(address, 1);

    if (address == UART_ADDRESS) {
        uart_tx(data);
        return;
    }

    uint32_t true_address = address - BASE_ADDRESS;
            
    /* RISC-V uses little-endian mode */
    ram->memory[true_address]   = (uint8_t)(data & 0xFF);
}

void inspect_ram(const ram_t *ram, uint32_t address, uint32_t words_num) 
{
    printf("\n------------------------------------\n[DEBUG] INSPECT RAM START\n\n");
    for (unsigned int i = 0; i < words_num; i++) {
        check_address(address, 4);
        printf("0x%08x: %08x\n", address, read_word(ram, address));
        
        address += 4;
    }
    printf("\n[DEBUG] INSPECT RAM END\n------------------------------------\n");
}
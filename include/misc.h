#ifndef MISC_H
#define MISC_H

#include <stdint.h>

typedef uint32_t Instruction;

typedef struct {
    uint32_t opcode;
    uint32_t rd;
    uint32_t funct3;
    uint32_t rs1;
    uint32_t rs2;
    uint32_t funct7;
    int32_t imm; 
} Decoded_instruction;    

/* bits = the number of bits the original value required */
static inline int32_t sign_extend(uint32_t value, int bits) 
{
    uint32_t mask = 1U << (bits-1); // isolate the sign bit
    return (value ^ mask) - mask; 
}

#endif
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
    uint32_t imm; 
} Decoded_instruction;    

typedef enum {
    ADD  = 0x00,
    SUB  = 0x20,
    XOR  = 0x4,
    OR   = 0x6,
    AND  = 0x7,
    SLL  = 0x1,
    SRL  = 0x00,
    SRA  = 0x20,
    SLT  = 0x2,
    SLTU = 0x3,

    ADDI = 0x0,
    XORI = 0x4,
    ORI  = 0x6,
    ANDI = 0x7,
    SLLI = 0x00,
    SRLI = 0x00,
    SRAI = 0x20,
    SLTI = 0x2,
    SLTIU = 0x3,
    LB  = 0x0,
    LH  = 0x1,
    LW  = 0x2, 
    LBU = 0x4,
    LHU = 0x5,
    JALR = 0x67,
    ECALL = 0x0,
    EBREAK = 0x1,
    
    SB = 0x0,
    SH = 0x1,
    SW = 0x2,

    LUI = 0x37,
    AUIPC = 0x17,

    BEQ = 0x0,
    BNE = 0x1,
    BLT = 0x4,
    BGE = 0x5,
    BLTU = 0x6,
    BGEU = 0x7,
} instr_t;  

/* bits = the number of bits the original value required */
static inline int32_t sign_extend(uint32_t value, int bits) 
{
    uint32_t mask = 1U << (bits-1); // isolate the sign bit
    return (value ^ mask) - mask; 
}

static inline uint32_t shift_right_arith(uint32_t value, int bits)
{
    return (uint32_t)((int32_t)value >> (bits));
}

#endif
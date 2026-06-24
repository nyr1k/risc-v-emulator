#include <stdint.h>

#include <decoders.h>
#include <misc.h>

void r_decode(Decoded_instruction *decoded_instr, const Instruction instr) 
{
    decoded_instr->rd     = (instr >> 7) & 0x1F;  // 5 bits
    decoded_instr->funct3 = (instr >> 12) & 0x07; // 3 bits
    decoded_instr->rs1    = (instr >> 15) & 0x1F; // 5 bits
    decoded_instr->rs2    = (instr >> 20) & 0x1F; // 5 bits
    decoded_instr->funct7 = (instr >> 25) & 0x7F; // 7 bits
}

void i_decode(Decoded_instruction *decoded_instr, const Instruction instr)
{
    decoded_instr->rd     = (instr >> 7) & 0x1F;  // 5 bits
    decoded_instr->funct3 = (instr >> 12) & 0x07; // 3 bits
    decoded_instr->rs1    = (instr >> 15) & 0x1F; // 5 bits
    decoded_instr->imm    = sign_extend((instr >> 20), 12); // 12 bits
}

void s_decode(Decoded_instruction *decoded_instr, const Instruction instr)
{
    decoded_instr->funct3 = (instr >> 12) & 0x07; // 3 bits
    decoded_instr->rs1    = (instr >> 15) & 0x1F; // 5 bits
    decoded_instr->rs2    = (instr >> 20) & 0x1F; // 5 bits

    uint32_t imm = ((instr >> 25) & 0x7F) << 5 | 
                   ((instr >> 7) & 0x1F);
    
    decoded_instr->imm = sign_extend(imm, 12); // 12 bits
}

void u_decode(Decoded_instruction *decoded_instr, const Instruction instr)
{
    decoded_instr->rd     = (instr >> 7) & 0x1F;  // 5 bits
    decoded_instr->imm    = (instr & 0xFFFFF000); // 20 bits
}

void b_decode(Decoded_instruction *decoded_instr, const Instruction instr) 
{
    decoded_instr->funct3 = (instr >> 12) & 0x07; // 3 bits
    decoded_instr->rs1    = (instr >> 15) & 0x1F; // 5 bits
    decoded_instr->rs2    = (instr >> 20) & 0x1F; // 5 bits

    uint32_t imm = ((instr >> 31) & 0x1)   << 12 | // bit 12
                   ((instr >> 7)  & 0x1)   << 11 | // bit 11
                   ((instr >> 25) & 0x3F)  << 5 |  // bits 10-5 
                   ((instr >> 8)  & 0xF)   << 1;   // bits 4-1 
                                                   // bit 0 = 0

    decoded_instr->imm = (sign_extend(imm, 13) << 1);
}                   

void j_decode(Decoded_instruction *decoded_instr, const Instruction instr)
{
    decoded_instr->rd     = (instr >> 7) & 0x1F;  // 5 bits

    uint32_t imm = ((instr >> 31)  & 0x1)     << 20 | // bit 20
                   ((instr >> 12)  & 0xFF)    << 12 | // bit 19-12
                   ((instr >> 20)  & 0x1)     << 11 | // bits 11 
                   ((instr >> 21)  & 0x3FF)   << 1;   // bits 10-1 
                                                   // bit 0 = 0

    decoded_instr->imm = (sign_extend(imm, 21) << 1 );
}
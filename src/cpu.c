#include <stdint.h>
#include <string.h>

#include <cpu.h>
#include <memory.h>
#include <err.h>
#include <misc.h>

cpu_t cpu;

void cpu_init()
{
    memset(cpu.regs, 0, sizeof(cpu.regs));
    cpu.pc = BASE_ADDRESS;
}

Instruction fetch() 
{
    return (Instruction)(read_word(cpu.pc));
}

Decoded_instruction decode(const Instruction instr)
{
    Decoded_instruction decoded_instr = {0};
    decoded_instr.opcode =  instr & 0x7F; // 7 bits

    switch (decoded_instr.opcode) {
        /* R-type */
        case 0x33: 
            r_decode(&decoded_instr, instr);
            break;
        
        /* I-type */
        case 0x03: 
        case 0x13: 
        case 0x67:
        case 0x73: 
            i_decode(&decoded_instr, instr);
            break; 

        /* S-type */
        case 0x23: 
            s_decode(&decoded_instr, instr);
            break;

        /* U-type */
        case 0x17: 
        case 0x37: 
            u_decode(&decoded_instr, instr);
            break;

        /* B-type */
        case 0x63:
            b_decode(&decoded_instr, instr);
            break;

        /* J-type */
        case 0x6F:
            j_decode(&decoded_instr, instr);
            break;

        default:
            report_and_abort(INVALID_INSTRUCTION);
    }

    return decoded_instr;
}

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
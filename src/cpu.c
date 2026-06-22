#include <string.h>

#include <cpu.h>
#include <memory.h>
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

Decoded_instruction decode(Instruction instr) 
{
    Decoded_instruction decoded = {0};
    decoded.opcode =  instr & 0x7F;
    decoded.rd     = (instr >> 7) & 0x1F;
    decoded.funct3 = (instr >> 12) & 0x07;
    decoded.rs1    = (instr >> 15) & 0x1F;
    decoded.rs2    = (instr >> 20) & 0x1F;
    decoded.funct7 = (instr >> 25) & 0x7F;
    
    return decoded; 
}
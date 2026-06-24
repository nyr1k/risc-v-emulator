#include <stdio.h>
#include <string.h>

#include <cpu.h>
#include <memory.h>
#include <decoders.h>
#include <execute_switches.h>
#include <err.h>
#include <sys/types.h>

cpu_t cpu;

void cpu_init()
{
    memset(cpu.regs, 0, sizeof(cpu.regs));
    cpu.pc = BASE_ADDRESS;
    cpu.regs[1] = 0xFFFFFFFF;
    cpu.regs[2] = 1;
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



void execute(const Decoded_instruction decoded_instr)
{
    uint32_t next_pc = cpu.pc + 4;
    switch (decoded_instr.opcode) {
        
        /* R-type */
        case 0x33:
            r_type(&cpu, decoded_instr);
            break;

        /* I-type */
        case 0x03: 
        case 0x13: 
        case 0x67:
        case 0x73: 
            i_type(&cpu, decoded_instr);
            break;

        default:
            report_and_abort(INVALID_INSTRUCTION);
    }

    cpu.regs[0] = 0; // x0 should always be zero
    cpu.pc = next_pc;
}

void dump_cpu()
{
    printf("x0: %08X\nx1: %08X\nx2: %08X\nx3: %08X\n", cpu.regs[0], cpu.regs[1], cpu.regs[2], cpu.regs[3]);
    printf("x4: %08X\nx5: %08X\nx6: %08X\nx7: %08X\n", cpu.regs[4], cpu.regs[5], cpu.regs[6], cpu.regs[7]);
    printf("x8: %08X\nx9: %08X\nx10: %08X\nx11: %08X\n", cpu.regs[8], cpu.regs[9], cpu.regs[10], cpu.regs[11]);
    printf("x12: %08X\nx13: %08X\nx14: %08X\nx15: %08X\n", cpu.regs[12], cpu.regs[13], cpu.regs[14], cpu.regs[15]);
    printf("x16: %08X\nx17: %08X\nx18: %08X\nx19: %08X\n", cpu.regs[16], cpu.regs[17], cpu.regs[18], cpu.regs[19]);
    printf("x20: %08X\nx21: %08X\nx22: %08X\nx23: %08X\n", cpu.regs[20], cpu.regs[21], cpu.regs[22], cpu.regs[23]);
    printf("x24: %08X\nx25: %08X\nx26: %08X\nx27: %08X\n", cpu.regs[24], cpu.regs[25], cpu.regs[26], cpu.regs[27]);
    printf("x28: %08X\nx29: %08X\nx30: %08X\nx31: %08X\n", cpu.regs[28], cpu.regs[29], cpu.regs[30], cpu.regs[31]);
    printf("pc: %08X\n", cpu.pc);
}
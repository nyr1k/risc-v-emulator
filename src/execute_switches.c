#include "misc.h"
#include <execute_switches.h>
#include <err.h>

void r_type(cpu_t *cpu, const Decoded_instruction decoded_instr)
{

    switch (decoded_instr.funct3) {

        case 0x0:
            if (decoded_instr.funct7 == 0x00) 
                cpu->regs[decoded_instr.rd] = cpu->regs[decoded_instr.rs1] + cpu->regs[decoded_instr.rs2];
            else if (decoded_instr.funct7 == 0x20)
                cpu->regs[decoded_instr.rd] = cpu->regs[decoded_instr.rs1] - cpu->regs[decoded_instr.rs2];
            else
                report_and_abort(INVALID_INSTRUCTION);
            break;
        case 0x4:
            cpu->regs[decoded_instr.rd] = cpu->regs[decoded_instr.rs1] ^ cpu->regs[decoded_instr.rs2];
            break;
        case 0x6:
            cpu->regs[decoded_instr.rd] = cpu->regs[decoded_instr.rs1] | cpu->regs[decoded_instr.rs2];
            break;
        case 0x7:
            cpu->regs[decoded_instr.rd] = cpu->regs[decoded_instr.rs1] & cpu->regs[decoded_instr.rs2];
            break;
        case 0x1:
            cpu->regs[decoded_instr.rd] = cpu->regs[decoded_instr.rs1] << (cpu->regs[decoded_instr.rs2] & 0x1F); // riscV uses only the lowest 5 bits to shift 
            break;
        case 0x5:
            if (decoded_instr.funct7 == 0x00)
                cpu->regs[decoded_instr.rd] = cpu->regs[decoded_instr.rs1] >> (cpu->regs[decoded_instr.rs2] & 0x1F); // riscV uses only the lowest 5 bits to shift 
            else if (decoded_instr.funct7 == 0x20)
                cpu->regs[decoded_instr.rd] = shift_right_arith(cpu->regs[decoded_instr.rs1], (cpu->regs[decoded_instr.rs2] & 0x1F)); // riscV uses only the lowest 5 bits to shift 
            else
                report_and_abort(INVALID_INSTRUCTION);
            break;
        case 0x2:
            cpu->regs[decoded_instr.rd] = (int32_t)cpu->regs[decoded_instr.rs1] < (int32_t)cpu->regs[decoded_instr.rs2] ? 1 : 0;
            break;
        case 0x3:
            cpu->regs[decoded_instr.rd] = cpu->regs[decoded_instr.rs1] < cpu->regs[decoded_instr.rs2] ? 1 : 0;
            break;  
        default:
            report_and_abort(INVALID_INSTRUCTION);
    }
}
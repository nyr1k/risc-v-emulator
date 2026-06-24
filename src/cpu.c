#include <stdint.h>
#include <string.h>

#include <cpu.h>
#include <memory.h>
#include <decoders.h>
#include <err.h>
#include <misc.h>
#include <sys/types.h>

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



void execute(const Decoded_instruction decoded_instr)
{
    uint32_t next_pc = cpu.pc + 4;
    switch (decoded_instr.opcode) {
        /* R-type */
        case 0x33:
            switch (decoded_instr.funct3) {

                case 0x0:
                    if (decoded_instr.funct7 == 0x00) 
                        cpu.regs[decoded_instr.rd] = cpu.regs[decoded_instr.rs1] + cpu.regs[decoded_instr.rs2];
                    else if (decoded_instr.funct7 == 0x20)
                        cpu.regs[decoded_instr.rd] = cpu.regs[decoded_instr.rs1] - cpu.regs[decoded_instr.rs2];
                    else
                        report_and_abort(INVALID_INSTRUCTION);
                    break;
                case 0x4:
                    cpu.regs[decoded_instr.rd] = cpu.regs[decoded_instr.rs1] ^ cpu.regs[decoded_instr.rs2];
                    break;
                case 0x6:
                    cpu.regs[decoded_instr.rd] = cpu.regs[decoded_instr.rs1] | cpu.regs[decoded_instr.rs2];
                    break;
                case 0x7:
                    cpu.regs[decoded_instr.rd] = cpu.regs[decoded_instr.rs1] & cpu.regs[decoded_instr.rs2];
                    break;
                case 0x1:
                    cpu.regs[decoded_instr.rd] = cpu.regs[decoded_instr.rs1] << (cpu.regs[decoded_instr.rs2] & 0x1F); // riscV uses only the lowest 5 bits to shift 
                    break;
                case 0x5:
                    if (decoded_instr.funct7 == 0x00)
                        cpu.regs[decoded_instr.rd] = cpu.regs[decoded_instr.rs1] >> (cpu.regs[decoded_instr.rs2] & 0x1F); // riscV uses only the lowest 5 bits to shift 
                    else if (decoded_instr.funct7 == 0x20)
                        cpu.regs[decoded_instr.rd] = shift_right_arith(cpu.regs[decoded_instr.rs1], (cpu.regs[decoded_instr.rs2] & 0x1F)); // riscV uses only the lowest 5 bits to shift 
                    else
                        report_and_abort(INVALID_INSTRUCTION);
                    break;
                case 0x2:
                    cpu.regs[decoded_instr.rd] = (int32_t)cpu.regs[decoded_instr.rs1] < (int32_t)cpu.regs[decoded_instr.rs2] ? 1 : 0;
                    break;
                case 0x3:
                    cpu.regs[decoded_instr.rd] = cpu.regs[decoded_instr.rs1] < cpu.regs[decoded_instr.rs2] ? 1 : 0;
                    break;  
                default:
                report_and_abort(INVALID_INSTRUCTION);
            }
            break;
            
        default:
            report_and_abort(INVALID_INSTRUCTION);
    }

    cpu.regs[0] = 0; // x0 should always be zero
    cpu.pc = next_pc;
}
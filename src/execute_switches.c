#include <stdint.h>

#include <memory.h>
#include <misc.h>
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

void i_type(cpu_t *cpu, const Decoded_instruction decoded_instr)
{
    switch (decoded_instr.opcode) {
        
        case 0x13: {
            uint32_t funct7 = (decoded_instr.imm >> 5) & 0x7F;
            switch (decoded_instr.funct3) {

                case 0x0:
                    cpu->regs[decoded_instr.rd] = cpu->regs[decoded_instr.rs1] + decoded_instr.imm;
                    break;
                case 0x4:
                    cpu->regs[decoded_instr.rd] = cpu->regs[decoded_instr.rs1] ^ decoded_instr.imm;
                    break;
                case 0x6:
                    cpu->regs[decoded_instr.rd] = cpu->regs[decoded_instr.rs1] | decoded_instr.imm;
                    break;
                case 0x7:
                    cpu->regs[decoded_instr.rd] = cpu->regs[decoded_instr.rs1] & decoded_instr.imm;
                    break;
                case 0x1:
                    if (funct7 == 0x00)
                        cpu->regs[decoded_instr.rd] = cpu->regs[decoded_instr.rs1] << (decoded_instr.imm & 0x1F);
                    else 
                        report_and_abort(INVALID_INSTRUCTION);
                    break;
                case 0x5:
                    if (funct7 == 0x00)
                        cpu->regs[decoded_instr.rd] = cpu->regs[decoded_instr.rs1] >> (decoded_instr.imm & 0x1F);
                    else if (funct7 == 0x20)
                        cpu->regs[decoded_instr.rd] = shift_right_arith(cpu->regs[decoded_instr.rs1], decoded_instr.imm & 0x1F);
                    else
                        report_and_abort(INVALID_INSTRUCTION);
                    break;
                case 0x2:
                    cpu->regs[decoded_instr.rd] = (int32_t)cpu->regs[decoded_instr.rs1] < (int32_t)decoded_instr.imm ? 1 : 0;
                    break;
                case 0x3:
                    cpu->regs[decoded_instr.rd] = cpu->regs[decoded_instr.rs1] < decoded_instr.imm ? 1 : 0;
                    break;
                default:
                    report_and_abort(INVALID_INSTRUCTION);
            }
            break;
        }

        case 0x03: {
            uint32_t target_address = cpu->regs[decoded_instr.rs1] + decoded_instr.imm;
            switch (decoded_instr.funct3) {
                 
                case 0x0: {
                    uint8_t value = read_byte(target_address);
                    cpu->regs[decoded_instr.rd] = sign_extend(value, 8);
                    break;
                }
                case 0x1: {
                    if (target_address % 2 != 0)
                        report_and_abort(INVALID_LOAD_ALIGNMENT);
                    uint16_t value = read_halfword(target_address);
                    cpu->regs[decoded_instr.rd] = sign_extend(value, 16);
                    break;
                }
                case 0x2: {
                    if (target_address % 4 != 0)
                        report_and_abort(INVALID_LOAD_ALIGNMENT);
                    uint32_t value = read_word(target_address);
                    cpu->regs[decoded_instr.rd] = value;
                    break;
                }
                case 0x4: {
                    uint8_t value = read_byte(target_address);
                    cpu->regs[decoded_instr.rd] = value;
                    break;
                }
                case 0x5: {
                    if (target_address % 2 != 0)
                        report_and_abort(INVALID_LOAD_ALIGNMENT);
                    uint16_t value = read_halfword(target_address);
                    cpu->regs[decoded_instr.rd] = value;
                    break;
                }
                default:
                    report_and_abort((INVALID_INSTRUCTION));
            }
            break;
        }
        default:
            report_and_abort(INVALID_INSTRUCTION);
    } 
}
#include <misc.h>
#include <execute_switches.h>
#include <err.h>
#include <stdint.h>


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

void i_type(cpu_t *cpu, const Decoded_instruction decoded_instr, uint32_t *next_pc, const ram_t *ram)
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
                    uint8_t value = read_byte(ram, target_address);
                    cpu->regs[decoded_instr.rd] = sign_extend(value, 8);
                    break;
                }
                case 0x1: {
                    if (target_address % 2 != 0)
                        report_and_abort(INVALID_LOAD_ALIGNMENT);
                    uint16_t value = read_halfword(ram, target_address);
                    cpu->regs[decoded_instr.rd] = sign_extend(value, 16);
                    break;
                }
                case 0x2: {
                    if (target_address % 4 != 0)
                        report_and_abort(INVALID_LOAD_ALIGNMENT);
                    uint32_t value = read_word(ram, target_address);
                    cpu->regs[decoded_instr.rd] = value;
                    break;
                }
                case 0x4: {
                    uint8_t value = read_byte(ram, target_address);
                    cpu->regs[decoded_instr.rd] = value;
                    break;
                }
                case 0x5: {
                    if (target_address % 2 != 0)
                        report_and_abort(INVALID_LOAD_ALIGNMENT);
                    uint16_t value = read_halfword(ram, target_address);
                    cpu->regs[decoded_instr.rd] = value;
                    break;
                }
                default:
                    report_and_abort(INVALID_INSTRUCTION);
            }
            break;
        }

        case 0x67:
            cpu->regs[decoded_instr.rd] = *next_pc;
            *next_pc = (cpu->regs[decoded_instr.rs1] + decoded_instr.imm) & ~1; // because the immediate is not shifted by 1 bit, we clear the lowest bit manually
            if ((*next_pc & 0x3) != 0) // Target address should 4-byte aligned
                report_and_abort(INVALID_INSTRUCTION_ALIGNMENT);
            break;

        default:
            report_and_abort(INVALID_INSTRUCTION);
    } 
}

void s_type(cpu_t *cpu, const Decoded_instruction decoded_instr, ram_t *ram)
{
    uint32_t target_address = cpu->regs[decoded_instr.rs1] + decoded_instr.imm;
    switch (decoded_instr.funct3) {

        case 0x0:
            write_byte(ram, target_address, cpu->regs[decoded_instr.rs2] & 0xFF);
            break;

        case 0x1:
            if (target_address % 2 != 0)
                report_and_abort(INVALID_STORE_ALIGNMENT);
            write_halfword(ram, target_address, cpu->regs[decoded_instr.rs2] & 0xFFFF);
            break;

        case 0x2:
            if (target_address % 4 != 0)
                report_and_abort(INVALID_STORE_ALIGNMENT);
            write_word(ram, target_address, cpu->regs[decoded_instr.rs2]);
            break;

        default:
            report_and_abort(INVALID_INSTRUCTION);
    }
}

void u_type(cpu_t *cpu, const Decoded_instruction decoded_instr)
{
    switch (decoded_instr.opcode) {
    
        case 0x37:
            cpu->regs[decoded_instr.rd] = decoded_instr.imm;
            break;
        
        case 0x17:
            cpu->regs[decoded_instr.rd] = cpu->pc + decoded_instr.imm;
            break;

        default:
            report_and_abort(INVALID_INSTRUCTION);
    }
}

void b_type(cpu_t *cpu, const Decoded_instruction decoded_instr, uint32_t *next_pc)
{
    switch (decoded_instr.funct3) {
        
        case 0x0:
            if (cpu->regs[decoded_instr.rs1] == cpu->regs[decoded_instr.rs2])
                *next_pc = cpu->pc + decoded_instr.imm;
            break;

        case 0x1:
            if (cpu->regs[decoded_instr.rs1] != cpu->regs[decoded_instr.rs2])
                *next_pc = cpu->pc + decoded_instr.imm;
            break;

        case 0x4:
            if ((int32_t)cpu->regs[decoded_instr.rs1] < (int32_t)cpu->regs[decoded_instr.rs2])
                *next_pc = cpu->pc + decoded_instr.imm;
            break;

        case 0x5:
            if ((int32_t)cpu->regs[decoded_instr.rs1] >= (int32_t)cpu->regs[decoded_instr.rs2])
                *next_pc = cpu->pc + decoded_instr.imm;
            break;

        case 0x6:
            if (cpu->regs[decoded_instr.rs1] < cpu->regs[decoded_instr.rs2])
                *next_pc = cpu->pc + decoded_instr.imm;
            break;

        case 0x7:
            if (cpu->regs[decoded_instr.rs1] >= cpu->regs[decoded_instr.rs2])
                *next_pc = cpu->pc + decoded_instr.imm;
            break;

        default:
            report_and_abort(INVALID_INSTRUCTION);
    }

    if ((*next_pc & 0x3) != 0) // Target address should be 4-byte aligned 
        report_and_abort(INVALID_INSTRUCTION_ALIGNMENT);
}

void j_type(cpu_t *cpu, const Decoded_instruction decoded_instr, uint32_t *next_pc)
{
    cpu->regs[decoded_instr.rd] = *next_pc;
    *next_pc = cpu->pc + decoded_instr.imm;
    if ((*next_pc & 0x3) != 0) // Target address should 4-byte aligned
        report_and_abort(INVALID_INSTRUCTION_ALIGNMENT);
}
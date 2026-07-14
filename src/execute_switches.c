#include "memory.h"
#include <stdint.h>
#include <stdio.h>

#include <cpu.h>
#include <syscall.h>
#include <misc.h>
#include <execute_switches.h>
#include <err.h>

uint8_t r_type(cpu_t *cpu, const Decoded_instruction decoded_instr)
{
    switch (decoded_instr.funct3) {

        case 0x0: 
            if (decoded_instr.funct7 == ADD) 
                cpu->regs[decoded_instr.rd] = cpu->regs[decoded_instr.rs1] + cpu->regs[decoded_instr.rs2];
            else if (decoded_instr.funct7 == SUB)
                cpu->regs[decoded_instr.rd] = cpu->regs[decoded_instr.rs1] - cpu->regs[decoded_instr.rs2];
            else
                return INVALID_INSTRUCTION;
            break;

        case XOR:
            cpu->regs[decoded_instr.rd] = cpu->regs[decoded_instr.rs1] ^ cpu->regs[decoded_instr.rs2];
            break;

        case OR:
            cpu->regs[decoded_instr.rd] = cpu->regs[decoded_instr.rs1] | cpu->regs[decoded_instr.rs2];
            break;

        case AND:
            cpu->regs[decoded_instr.rd] = cpu->regs[decoded_instr.rs1] & cpu->regs[decoded_instr.rs2];
            break;

        case SLL:
            cpu->regs[decoded_instr.rd] = cpu->regs[decoded_instr.rs1] << (cpu->regs[decoded_instr.rs2] & 0x1F); // riscV uses only the lowest 5 bits to shift 
            break;

        case 0x5:
            if (decoded_instr.funct7 == SRL)
                cpu->regs[decoded_instr.rd] = cpu->regs[decoded_instr.rs1] >> (cpu->regs[decoded_instr.rs2] & 0x1F); // riscV uses only the lowest 5 bits to shift 
            else if (decoded_instr.funct7 == SRA)
                cpu->regs[decoded_instr.rd] = shift_right_arith(cpu->regs[decoded_instr.rs1], (cpu->regs[decoded_instr.rs2] & 0x1F)); // riscV uses only the lowest 5 bits to shift 
            else
                return INVALID_INSTRUCTION;
            break;

        case SLT:
            cpu->regs[decoded_instr.rd] = (int32_t)cpu->regs[decoded_instr.rs1] < (int32_t)cpu->regs[decoded_instr.rs2] ? 1 : 0;
            break;

        case SLTU:
            cpu->regs[decoded_instr.rd] = cpu->regs[decoded_instr.rs1] < cpu->regs[decoded_instr.rs2] ? 1 : 0;
            break;  

        default:
            return INVALID_INSTRUCTION;
    }
    return 0;
}

uint8_t i_type(cpu_t *cpu, const Decoded_instruction decoded_instr, uint32_t *next_pc, const ram_t *ram)
{
    switch (decoded_instr.opcode) {
        
        case 0x13: {
            uint32_t funct7 = (decoded_instr.imm >> 5) & 0x7F;
            switch (decoded_instr.funct3) {

                case ADDI:
                    cpu->regs[decoded_instr.rd] = cpu->regs[decoded_instr.rs1] + decoded_instr.imm;
                    break;

                case XORI:
                    cpu->regs[decoded_instr.rd] = cpu->regs[decoded_instr.rs1] ^ decoded_instr.imm;
                    break;

                case ORI:
                    cpu->regs[decoded_instr.rd] = cpu->regs[decoded_instr.rs1] | decoded_instr.imm;
                    break;

                case ANDI:
                    cpu->regs[decoded_instr.rd] = cpu->regs[decoded_instr.rs1] & decoded_instr.imm;
                    break;

                case 0x1:
                    if (funct7 == SLLI)
                        cpu->regs[decoded_instr.rd] = cpu->regs[decoded_instr.rs1] << (decoded_instr.imm & 0x1F);
                    else 
                        return INVALID_INSTRUCTION;
                    break;

                case 0x5:
                    if (funct7 == SRLI)
                        cpu->regs[decoded_instr.rd] = cpu->regs[decoded_instr.rs1] >> (decoded_instr.imm & 0x1F);
                    else if (funct7 == SRAI)
                        cpu->regs[decoded_instr.rd] = shift_right_arith(cpu->regs[decoded_instr.rs1], decoded_instr.imm & 0x1F);
                    else
                        return INVALID_INSTRUCTION;
                    break;

                case SLTI:
                    cpu->regs[decoded_instr.rd] = (int32_t)cpu->regs[decoded_instr.rs1] < (int32_t)decoded_instr.imm ? 1 : 0;
                    break;

                case SLTIU:
                    cpu->regs[decoded_instr.rd] = cpu->regs[decoded_instr.rs1] < decoded_instr.imm ? 1 : 0;
                    break;

                default:
                    return INVALID_INSTRUCTION;
            }
            break;
        }

        case 0x03: {
            uint32_t target_address = cpu->regs[decoded_instr.rs1] + decoded_instr.imm;
            switch (decoded_instr.funct3) {
                 
                case LB: {
                    uint8_t value = read_byte(ram, target_address);
                    cpu->regs[decoded_instr.rd] = sign_extend(value, 8);
                    break;
                }
                case LH: {
                    if (target_address % 2 != 0)
                        return INVALID_LOAD_ALIGNMENT;
                    uint16_t value = read_halfword(ram, target_address);
                    cpu->regs[decoded_instr.rd] = sign_extend(value, 16);
                    break;
                }
                case LW: {
                    if (target_address % 4 != 0)
                        return INVALID_LOAD_ALIGNMENT;
                    uint32_t value = read_word(ram, target_address);
                    cpu->regs[decoded_instr.rd] = value;
                    break;
                }
                case LBU: {
                    uint8_t value = read_byte(ram, target_address);
                    cpu->regs[decoded_instr.rd] = value;
                    break;
                }
                case LHU: {
                    if (target_address % 2 != 0)
                        return INVALID_LOAD_ALIGNMENT;
                    uint16_t value = read_halfword(ram, target_address);
                    cpu->regs[decoded_instr.rd] = value;
                    break;
                }
                default:
                    return INVALID_INSTRUCTION;
            }
            break;
        }

        case JALR: {
            uint32_t target = (cpu->regs[decoded_instr.rs1] + decoded_instr.imm) & ~1U; // because the immediate is not shifted by 1 bit, we clear the lowest bit manually
            if ((target & 0x3) != 0) // Target address should 4-byte aligned
                return INVALID_INSTRUCTION_ALIGNMENT;

            cpu->regs[decoded_instr.rd] = *next_pc;
            *next_pc = target;
            break; 
        }

        case 0x73:
            if (decoded_instr.imm == ECALL)
                return handle_syscall(cpu);
            else if (decoded_instr.imm == EBREAK) {
                printf("\nEBREAK at 0x%08X", cpu->pc);
                dump_cpu(*cpu);

                // TODO: hex string to hex num
                // uint32_t choice; 
                // do {
                //     printf("\nInspect RAM? (0-1): ");
                //     scanf("%u", &choice);
                    
                //     if (choice == 1) {
                //         uint32_t address;
                //         uint32_t words_num;

                //         printf("Address: 0x")
                //         scanf("%u", &num_w);
                //         inspect_ram(ram, address, num_w);
                //     }
                // } while (choice != 0);

                printf("Press enter to continue program...");
                getchar(); 
            }
            else 
                return INVALID_INSTRUCTION;
            break;

        default:
            return INVALID_INSTRUCTION;
    } 
    return 0;
}

uint8_t s_type(cpu_t *cpu, const Decoded_instruction decoded_instr, ram_t *ram)
{
    uint32_t target_address = cpu->regs[decoded_instr.rs1] + decoded_instr.imm;
    switch (decoded_instr.funct3) {

        case SB:
            write_byte(ram, target_address, cpu->regs[decoded_instr.rs2] & 0xFF);
            break;

        case SH:
            if (target_address % 2 != 0)
                return INVALID_STORE_ALIGNMENT;
            write_halfword(ram, target_address, cpu->regs[decoded_instr.rs2] & 0xFFFF);
            break;

        case SW:
            if (target_address % 4 != 0)
                return INVALID_STORE_ALIGNMENT;
            write_word(ram, target_address, cpu->regs[decoded_instr.rs2]);
            break;

        default:
            return INVALID_INSTRUCTION;
    }
    return 0;
}

uint8_t u_type(cpu_t *cpu, const Decoded_instruction decoded_instr)
{
    switch (decoded_instr.opcode) {
    
        case LUI:
            cpu->regs[decoded_instr.rd] = decoded_instr.imm;
            break;
        
        case AUIPC:
            cpu->regs[decoded_instr.rd] = cpu->pc + decoded_instr.imm;
            break;

        default:
            return INVALID_INSTRUCTION;
    }
    return 0;
}

uint8_t b_type(cpu_t *cpu, const Decoded_instruction decoded_instr, uint32_t *next_pc)
{
    switch (decoded_instr.funct3) {
        
        case BEQ:
            if (cpu->regs[decoded_instr.rs1] == cpu->regs[decoded_instr.rs2])
                *next_pc = cpu->pc + decoded_instr.imm;
            break;

        case BNE:
            if (cpu->regs[decoded_instr.rs1] != cpu->regs[decoded_instr.rs2])
                *next_pc = cpu->pc + decoded_instr.imm;
            break;

        case BLT:
            if ((int32_t)cpu->regs[decoded_instr.rs1] < (int32_t)cpu->regs[decoded_instr.rs2])
                *next_pc = cpu->pc + decoded_instr.imm;
            break;

        case BGE:
            if ((int32_t)cpu->regs[decoded_instr.rs1] >= (int32_t)cpu->regs[decoded_instr.rs2])
                *next_pc = cpu->pc + decoded_instr.imm;
            break;

        case BLTU:
            if (cpu->regs[decoded_instr.rs1] < cpu->regs[decoded_instr.rs2])
                *next_pc = cpu->pc + decoded_instr.imm;
            break;

        case BGEU:
            if (cpu->regs[decoded_instr.rs1] >= cpu->regs[decoded_instr.rs2])
                *next_pc = cpu->pc + decoded_instr.imm;
            break;

        default:
            return INVALID_INSTRUCTION;
    }

    if ((*next_pc & 0x3) != 0) // Target address should be 4-byte aligned 
        return INVALID_INSTRUCTION_ALIGNMENT;
    
    return 0;
}


/* jal */
uint8_t j_type(cpu_t *cpu, const Decoded_instruction decoded_instr, uint32_t *next_pc)
{
    cpu->regs[decoded_instr.rd] = *next_pc;
    *next_pc = cpu->pc + decoded_instr.imm;
    if ((*next_pc & 0x3) != 0) // Target address should 4-byte aligned
        return INVALID_INSTRUCTION_ALIGNMENT;

    return 0;
}
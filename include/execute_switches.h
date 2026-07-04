#ifndef EXECUTE_SWITCHES_H
#define EXECUTE_SWITCHES_H

#include <stdint.h>

#include <cpu.h>
#include <memory.h>
#include <misc.h>

void r_type(cpu_t *cpu, const Decoded_instruction decoded_instr);
void i_type(cpu_t *cpu, const Decoded_instruction decoded_instr, uint32_t *next_pc, ram_t ram);
void s_type(cpu_t *cpu, const Decoded_instruction decoded_instr, ram_t *ram);
void u_type(cpu_t *cpu, const Decoded_instruction decoded_instr);
void b_type(cpu_t *cpu, const Decoded_instruction decoded_instr, uint32_t *next_pc);
void j_type(cpu_t *cpu, const Decoded_instruction decoded_instr, uint32_t *next_pc);

#endif
#ifndef EXECUTE_SWITCHES_H
#define EXECUTE_SWITCHES_H

#include <stdint.h>

#include <cpu.h>
#include <memory.h>
#include <misc.h>

uint8_t r_type(cpu_t *cpu, const Decoded_instruction decoded_instr);
uint8_t i_type(cpu_t *cpu, const Decoded_instruction decoded_instr, uint32_t *next_pc, const ram_t *ram);
uint8_t s_type(cpu_t *cpu, const Decoded_instruction decoded_instr, ram_t *ram);
uint8_t u_type(cpu_t *cpu, const Decoded_instruction decoded_instr);
uint8_t b_type(cpu_t *cpu, const Decoded_instruction decoded_instr, uint32_t *next_pc);
uint8_t j_type(cpu_t *cpu, const Decoded_instruction decoded_instr, uint32_t *next_pc);

#endif
#ifndef EXECUTE_SWITCHES_H
#define EXECUTE_SWITCHES_H

#include <cpu.h>
#include <misc.h>

void r_type(cpu_t* cpu, const Decoded_instruction decoded_instr);
void i_type(cpu_t* cpu, const Decoded_instruction decoded_instr);
void s_type(cpu_t* cpu, const Decoded_instruction decoded_instr);

#endif
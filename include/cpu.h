#ifndef CPU_H 
#define CPU_H

#include <stdint.h>
#include <memory.h>
#include <misc.h>

#define SP 2

typedef struct {
  uint32_t regs[32];
  uint32_t pc;    
} cpu_t;

void cpu_init(cpu_t *cpu);

Instruction fetch(const ram_t *ram, const cpu_t *cpu);
Decoded_instruction decode(const Instruction instr);
void execute(cpu_t *cpu, const Decoded_instruction decoded_instr, ram_t *ram);

/* For debugging */
void dump_cpu(cpu_t cpu);

#endif

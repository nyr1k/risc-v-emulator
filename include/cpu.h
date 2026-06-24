#ifndef CPU_H 
#define CPU_H

#include <stdint.h>
#include <misc.h>

typedef struct {
  uint32_t regs[32];
  uint32_t pc;    
} cpu_t;

void cpu_init();

Instruction fetch();
Decoded_instruction decode(const Instruction instr);
void execute(const Decoded_instruction decoded_instr);

#endif

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

void r_decode(Decoded_instruction *decoded_instr, const Instruction instr); 
void i_decode(Decoded_instruction *decoded_instr, const Instruction instr); 
void s_decode(Decoded_instruction *decoded_instr, const Instruction instr); 
void u_decode(Decoded_instruction *decoded_instr, const Instruction instr); 
void b_decode(Decoded_instruction *decoded_instr, const Instruction instr);
void j_decode(Decoded_instruction *decoded_instr, const Instruction instr);

#endif

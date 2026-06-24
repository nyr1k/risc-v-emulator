#ifndef DECODERS_H
#define DECODERS_H

#include <misc.h>

void r_decode(Decoded_instruction *decoded_instr, const Instruction instr); 
void i_decode(Decoded_instruction *decoded_instr, const Instruction instr); 
void s_decode(Decoded_instruction *decoded_instr, const Instruction instr); 
void u_decode(Decoded_instruction *decoded_instr, const Instruction instr); 
void b_decode(Decoded_instruction *decoded_instr, const Instruction instr);
void j_decode(Decoded_instruction *decoded_instr, const Instruction instr);

#endif
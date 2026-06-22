#ifndef CPU_H 
#define CPU_H

#include <stdint.h>

typedef struct {
  uint32_t regs[32];
  uint32_t pc;    
} cpu_t;

cpu_t cpu = {0};  

#endif

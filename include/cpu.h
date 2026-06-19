#ifndef CPU_H 
#define CPU_H

#include <stdint.h>

struct cpu_t {
  uint32_t regs[32];
  uint32_t pc;    
};

struct cpu_t cpu = {0};  

#endif

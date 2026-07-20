#ifndef DEBUGGER_H
#define DEBUGGER_H

#include <stdbool.h>
#include <stdint.h>

#include <cpu.h>
#include <memory.h>
#include <misc.h>

typedef struct {
    uint8_t stp_count;
} dbg_t; 


void dbg_init(dbg_t* dbg);
void debugger(dbg_t *dbg, cpu_t *cpu, ram_t *mem, Decoded_instruction d_instr);

#endif
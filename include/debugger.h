#ifndef DEBUGGER_H
#define DEBUGGER_H

#include <stdbool.h>
#include <stdint.h>

#include <cpu.h>
#include <memory.h>
#include <misc.h>

typedef struct {
    uint32_t brk_pts[256];
    uint32_t pts_cnt;
} brkp_t; 

typedef struct {
    brkp_t brk; 
    uint8_t stp_count;
    uint8_t con_flag;
} dbg_t; 

/*
    what do i do with breakpoints now;
    i will save them in a list  
*/

void dbg_init(dbg_t* dbg);
void debugger(dbg_t *dbg, Decoded_instruction d_instr);

bool is_brk_point(dbg_t *dbg, const uint32_t pc);

#endif
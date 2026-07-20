#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>

#include <cpu.h>
#include <memory.h>
#include <debugger.h>
#include <misc.h>

static inline int stp_num_check(const char stp_num)
{
    if (stp_num <= '0' || stp_num > '9') {
        puts("#rv32i: The number should be between 1-9");
        return 0;
    } 
    return 1;
}

static inline uint32_t stoh(const char* str)
{
    uint32_t hex = 0;
    uint32_t base = 1;
    
    for (int i = 7; i >= 0; i--) {
        char c = str[i];
        
        if (c >= '0' && c <= '9')
            hex += base * (c-'0');
        else if (c >= 'A' && c <= 'F')
            hex += base * (c-'A'+10);
        else if (c >= 'a' && c <= 'f')
            hex += base * (c-'a'+10);
        else 
            return 0;
        base *= 16;
    }  

    return hex;
}

static inline int brk_addr_check(const uint32_t addr) 
{
    if (addr < BASE_ADDRESS || addr >= END_ADDRESS || addr%4 != 0)
        return -1;
    return 0;
}

static inline int add_brk_point(brkp_t *brk, const uint32_t brk_addr)
{
    if (brk->pts_cnt == 255) {
        printf("Breakpoints limit is reached\n");
        return -1;
    }

    for (uint32_t i = 0; i < brk->pts_cnt; i++) {
        if (brk->brk_pts[i] == brk_addr) {
            printf("Breakpoint at %0x08X already exists\n", brk_addr);
            return -1;
        }
    }

    brk->pts_cnt++;
    for (uint32_t i = 0; i < brk->pts_cnt; i++) {
        if (brk->brk_pts[i] == 0) {
            brk->brk_pts[i] = brk_addr;
            break;
        }
    }

    return 0;
}

void dbg_init(dbg_t *dbg)
{
    memset(dbg->brk.brk_pts, 0, sizeof(uint32_t) * 256);
    dbg->brk.pts_cnt = 0;

    dbg->stp_count = 0;
    dbg->con_flag = false;
}

/*
options:
    stp <n> - step <n> number of instruction;
    con - continue execution until a breakpoint or the end of the prorgam;
    brk <address> - set a breakpoint at the specified address;
    cpu - print values in the cpu registers;
    mem <address> <n> - print <n> values from the memory starting from <address>;
*/
void debugger(dbg_t *dbg, Decoded_instruction d_instr)
{
    while (true) {
        char buffer[128];
        char option[4];
        char stp_num[2];
        char brk_addr[9];

        printf("#rv32i: ");
        fgets(buffer, 128, stdin);

        memcpy(option, buffer, 3);
        option[sizeof(option)-1] = '\0';

        if (strcmp(option, "stp") == 0) {
            memcpy(stp_num, buffer+3, sizeof(char));
            stp_num[sizeof(stp_num)-1] = '\0';
            
            if (stp_num_check(stp_num[0])) {
                dbg->stp_count = stp_num[0]-'1'; 

                // remove when finish
                printf("step %d\n", dbg->stp_count+1);
                break;
            } 
        } 
        else if (strcmp(option, "brk") == 0) {
            printf("#rv32i: ");

            memcpy(brk_addr, buffer+4, sizeof(char)*8);
            brk_addr[sizeof(brk_addr)-1] = '\0';
        
            uint32_t brk_addr_i = stoh(brk_addr);
            if (brk_addr_check(brk_addr_i) == -1)
                printf("Invalid instruction address!\n");
            else {
                if (add_brk_point(&(dbg->brk), brk_addr_i) == 0) { 
                    printf("Added breakpoint at 0x%08X\n", brk_addr_i);
                }
            }
        } 
        else if  (strcmp(option, "con") == 0) {
            dbg->con_flag = true;
            break;
        }
    }
}

bool is_brk_point(dbg_t *dbg, const uint32_t pc)
{
    for (uint32_t i = 0; i < dbg->brk.pts_cnt; i++) {
        if (dbg->brk.brk_pts[i] == pc) {
            dbg->stp_count = 0;
            dbg->con_flag = false;
            return true;
        }
    }
    return false;
}
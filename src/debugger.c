#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include <debugger.h>

#define SKIP_OPTION 3

static inline int stp_num_check(const char stp_num)
{
    if (stp_num <= '0' || stp_num > '9') {
        puts("The number should be between 1-9");
        return 0;
    } 
    return 1;
}

void dbg_init(dbg_t *dbg)
{
    dbg->stp_count = 0;
}

/*
options:
    stp <n> - step <n> number of instruction;
    cnt - continue execution until a breakpoint or the end of the prorgam;
    brk <address> - set a breakpoint at the specified address;
    cpu - print values in the cpu registers;
    mem <address> <n> - print <n> values from the memory starting from <address>;
*/
void debugger(dbg_t *dbg, cpu_t *cpu, ram_t *mem, Decoded_instruction d_instr)
{
    while (true) {
        char buffer[128];
        char option[4];
        char stp_num[2];

        printf("#rv32i: ");
        fgets(buffer, 128, stdin);

        memcpy(option, buffer, 3);
        option[sizeof(option)-1] = '\0';

        if (strcmp(option, "stp") == 0) {
            memcpy(stp_num, buffer+SKIP_OPTION, 1);
            stp_num[sizeof(stp_num)-1] = '\0';
            
            if (stp_num_check(stp_num[0])) {
                dbg->stp_count = stp_num[0]-'0';

                // remove when finish
                printf("step %d\n", dbg->stp_count);
                break;
            }
        } 
    }
}
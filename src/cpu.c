#include <stdio.h>
#include <string.h>

#include <cpu.h>
#include <decoders.h>
#include <execute_switches.h>
#include <err.h>
#include <sys/types.h>

void cpu_init(cpu_t *cpu)
{
    memset(cpu->regs, 0, sizeof(cpu->regs));
    cpu->pc = BASE_ADDRESS;

    printf("[OK] CPU INIT SUCCESS\n");
}

Instruction fetch(const ram_t *ram, const cpu_t *cpu) 
{
    return (Instruction)(read_word(ram, cpu->pc));
}

Decoded_instruction decode(const Instruction instr)
{
    Decoded_instruction decoded_instr = {0};
    decoded_instr.opcode =  instr & 0x7F; // 7 bits

    switch (decoded_instr.opcode) {
        /* R-type */
        case 0x33: 
            r_decode(&decoded_instr, instr);
            break;
        
        /* I-type */
        case 0x03: 
        case 0x13: 
        case 0x67:
        case 0x73: 
            i_decode(&decoded_instr, instr);
            break; 

        /* S-type */
        case 0x23: 
            s_decode(&decoded_instr, instr);
            break;

        /* U-type */
        case 0x17: 
        case 0x37: 
            u_decode(&decoded_instr, instr);
            break;

        /* B-type */
        case 0x63:
            b_decode(&decoded_instr, instr);
            break;

        /* J-type */
        case 0x6F:
            j_decode(&decoded_instr, instr);
            break;

        default:
            report_and_abort(INVALID_INSTRUCTION);
    }

    return decoded_instr;
}



void execute(cpu_t *cpu, const Decoded_instruction decoded_instr, ram_t *ram)
{
    uint32_t next_pc = cpu->pc + 4;
    switch (decoded_instr.opcode) {

        /* R-type */
        case 0x33:
            r_type(cpu, decoded_instr);
            break;

        /* I-type */
        case 0x03: 
        case 0x13: 
        case 0x67:
        case 0x73: 
            i_type(cpu, decoded_instr, &next_pc, ram);
            break;

        /* S-type */
        case 0x23: 
            s_type(cpu, decoded_instr, ram);
            break;

        /* U-type */
        case 0x17: 
        case 0x37: 
            u_type(cpu, decoded_instr);
            break;

        /* B-type */
        case 0x63:
            b_type(cpu, decoded_instr, &next_pc);
            break;

        /* J-type */
        case 0x6F:
            j_type(cpu, decoded_instr, &next_pc);
            break;

        default:
            report_and_abort(INVALID_INSTRUCTION);
    }

    cpu->regs[0] = 0; // x0 should always be zero
    cpu->pc = next_pc;
}

void dump_cpu(cpu_t cpu)
{
    printf("\n------------------------------------\n[DEBUG] INSPECT CPU START\n\n");
    
    printf("x0: %08X  ra: %08X  sp:  %08X  gp:  %08X\n", cpu.regs[0], cpu.regs[1], cpu.regs[2], cpu.regs[3]);
    printf("tp: %08X  t0: %08X  t1:  %08X  t2:  %08X\n", cpu.regs[4], cpu.regs[5], cpu.regs[6], cpu.regs[7]);
    printf("s0: %08X  s1: %08X  a0:  %08X  a1:  %08X\n", cpu.regs[8], cpu.regs[9], cpu.regs[10], cpu.regs[11]);
    printf("a2: %08X  a3: %08X  a4:  %08X  a5:  %08X\n", cpu.regs[12], cpu.regs[13], cpu.regs[14], cpu.regs[15]);
    printf("a6: %08X  a7: %08X  s2:  %08X  s3:  %08X\n", cpu.regs[16], cpu.regs[17], cpu.regs[18], cpu.regs[19]);
    printf("s4: %08X  s5: %08X  s6:  %08X  s7:  %08X\n", cpu.regs[20], cpu.regs[21], cpu.regs[22], cpu.regs[23]);
    printf("s8: %08X  s9: %08X  s10: %08X  s11: %08X\n", cpu.regs[24], cpu.regs[25], cpu.regs[26], cpu.regs[27]);
    printf("t3: %08X  t4: %08X  t5:  %08X  t6:  %08X\n", cpu.regs[28], cpu.regs[29], cpu.regs[30], cpu.regs[31]);
    printf("pc:  %08X\n", cpu.pc);

    printf("\n[DEBUG] INSPECT CPU END\n------------------------------------\n");

}
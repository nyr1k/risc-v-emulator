#include <stdio.h>

#include <cpu.h>
#include <memory.h>
#include <misc.h>
#include <emulator.h>
#include <err.h>

extern int load_elf(uint32_t base_address, uint8_t *memory, const char* elf_name, uint32_t memory_size);

void emulator_init(emulator_t *emul, const char *elf_name)
{
    cpu_init(&(emul->cpu));
    memory_init(&(emul->ram));
    int ret_val = load_elf(BASE_ADDRESS, emul->ram.memory, elf_name, MEMORY_SIZE);

    switch (ret_val) {
        case -1:
            report_and_abort(ELF_OPEN_FAIL);
            break;
        case -2: 
            report_and_abort(ELF_HEADER_READ_FAIL);
            break;
        case -3:
            report_and_abort(PROGRAM_HEADER_READ_FAIL);
            break;    
        case -4:
            report_and_abort(ELF_IS_CORRUPTED);
            break; 
        case -5:
            report_and_abort(MEMORY_BOUNDS_HIT);
            break; 
        case -6:
            report_and_abort(LOAD_ELF_LSEEK_FAIL);
            break;
        case -7:
            report_and_abort(LOAD_SEGMENT_FAIL);
            break;
        case -8:
            report_and_abort(ELF_CLOSE_FAIL);
            break;
        case -98:
            report_and_abort(WRONG_ARCHITECTURE_OR_NOT_EXEC);
            break; 
        case -99:
            report_and_abort(WRONG_ENDIAN_OR_SYSTEM);
            break; 
        case -100:
            report_and_abort(NOT_ELF_FILE);
            break; 
        default:
            printf("[OK] ELF LOAD SUCCESS\n\n");
            emul->cpu.pc = (uint32_t) ret_val;

            /*
                for now, I initialize SP here, but later I want to add 
                a symbol in the linker.ld _stack_start, and parse the symbol table
                in load_elf and initialize sp there 

                for C programs, C runtime will overwrite the SP register if it needs to,
                but I think that there will be no need in that. 
                Anyway, just wanted to share the design.  
            */
            emul->cpu.regs[SP] = STACK_TOP;
            break;
    }
}   

uint8_t emulator_step(emulator_t *emul)
{
    /* Check if $sp overflowed*/
    if (emul->cpu.regs[SP] < STACK_LIMIT)
        report_and_abort(STACK_OVERFLOW);

    Instruction instr = fetch(&(emul->ram), &(emul->cpu));
    Decoded_instruction d_instr = decode(instr);
    uint8_t ret_val = execute(&(emul->cpu), d_instr, &(emul->ram));

    if (ret_val != 0 && ret_val < 228)
        report_and_abort(ret_val);

    return ret_val;
}

void emulator_run(emulator_t *emul)
{
    while (1) {
        uint8_t ret_val = emulator_step(emul);
        
        if (ret_val == EXIT_PROGRAM) {
            printf("Program has been finished!\n");
            break;
        }
    }
}
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
            printf("[OK] ELF LOAD SUCCESS\n");
            emul->cpu.pc = (uint32_t) ret_val;
            break;
    }
}   

void emulator_step(emulator_t *emul)
{
    Instruction instr = fetch(&(emul->ram), &(emul->cpu));
    Decoded_instruction d_instr = decode(instr);
    execute(&(emul->cpu), d_instr, &(emul->ram));
}

void emulator_run(emulator_t *emul)
{
    uint32_t prev_pc = 0;
    while (1) {
        emulator_step(emul);
        
        if (emul->cpu.pc == prev_pc) 
            break;

        prev_pc = emul->cpu.pc;
    }
}
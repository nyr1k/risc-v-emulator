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
    printf("%p\n", emul->ram.memory);
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
            printf("Entry point: %08X\n", ret_val);
            emul->cpu.pc = ret_val;
            break;
    }
}   

void emulator_step(emulator_t *emul)
{
    Instruction instr = fetch(&(emul->ram), emul->cpu);
    Decoded_instruction d_instr = decode(instr);
    execute(&(emul->cpu), d_instr, &(emul->ram));
    dump_cpu(emul->cpu);
}
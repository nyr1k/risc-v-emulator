#include "memory.h"
#include <emulator.h>

extern int load_elf(uint32_t base_address, uint8_t *memory, const char* elf_name, uint32_t memory_size);

void emulator_init(emulator_t *emul, const char *elf_name)
{
    cpu_init(&(emul->cpu));
    memory_init(&(emul->ram));
    int err = load_elf(BASE_ADDRESS, emul->ram.memory, elf_name, MEMORY_SIZE);
}   
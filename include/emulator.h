#ifndef EMULATOR_H
#define EMULATOR_H

#include <cpu.h>
#include <memory.h>

typedef struct {
    cpu_t cpu;
    ram_t ram;
} emulator_t;

void emulator_init(emulator_t *emul, const char *elf_name);
void emulator_step(emulator_t *emul);

#endif
#ifndef EMULATOR_H
#define EMULATOR_H

#include <cpu.h>
#include <memory.h>

typedef struct {
    cpu_t cpu;
    ram_t ram;

} emulator_t;

#endif
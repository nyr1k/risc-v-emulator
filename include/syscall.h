#ifndef SYSCALL_H
#define SYSCALL_H

#include <cpu.h>

enum {
    EXIT = 10,
    EXIT2 = 93
};

uint8_t handle_syscall(const cpu_t *cpu);

#endif 
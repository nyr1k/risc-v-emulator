#include <syscall.h>
#include <err.h>

uint8_t handle_syscall(const cpu_t *cpu)
{
    uint32_t syscall = cpu->regs[17]; // a7 register

    switch (syscall) {

        case EXIT:
            return EXIT_PROGRAM;
            break;

        default:
            return SYSCALL_NOT_SUPPORTED;
            break;
    }
}
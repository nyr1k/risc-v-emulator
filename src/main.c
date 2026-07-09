#include <stdio.h>
#include <emulator.h>
#include <cpu.h>
#include <memory.h>
#include <err.h>
#include <misc.h>

int main(int argc, char *argv[]) {

  if (argc == 1) {
    printf("Path to the ELF file is required!\n");
    return 1;
  } else if (argc >= 3) {
    printf("too many arguments...\n");
    return 1;
  }

  emulator_t rv32i;   
  emulator_init(&rv32i, argv[1]);  
  emulator_run(&rv32i);

  dump_cpu(rv32i.cpu);

  return 0;
}

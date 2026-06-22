#include <stdio.h>
#include <cpu.h>
#include <memory.h>
#include <err.h>

int main(int argc, char **argv) {

  printf("%d\n", cpu.regs[5]);  
  memory_init();
  
  write_word(BASE_ADDRESS, -1);
  printf("%zu\n", read_word(BASE_ADDRESS));
  return 0;
}

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
  inspect_ram(&(rv32i.ram), 0x80000000, 6);
  emulator_step(&rv32i);

//   dump_cpu();

//   printf("instr=0x%08X opcode=0x%02X rd=%u funct3=%u rs1=%u rs2=%u funct7=0x%02X\n",
//     instr,
//     decoded_instr.opcode,
//     decoded_instr.rd,
//     decoded_instr.funct3,
//     decoded_instr.rs1,
//     decoded_instr.rs2,
//     decoded_instr.funct7
// );

  return 0;
}

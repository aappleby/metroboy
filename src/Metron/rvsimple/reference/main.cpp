// RISC-V SiMPLE SV -- testbench
// BSD 3-Clause License
// (c) 2019, Marek Materzok, University of Wrocław

#include "Vtoplevel.h"
#include "verilated.h"
#include <stdio.h>

int run_test(const char* test_name) {
  printf("running %6s: ", test_name);

  char buf1[256];
  char buf2[256];
  sprintf(buf1, "+text_file=tests/%s.text.vh", test_name);
  sprintf(buf2, "+data_file=tests/%s.data.vh", test_name);
  const char* argv2[2] = { buf1, buf2 };
  Verilated::commandArgs(2, argv2);

  Vtoplevel top;

  int time;
  for (time = 0; time < 100000; time++) {
    top.reset = time >= 4 && time <= 7;
    top.clock = time & 1;
    top.eval();
    if (top.bus_write_enable && top.bus_address == 0xfffffff0) break;
  }

  if (time == 100000) {
    printf("TIMEOUT\n");
    return -1;
  } else if (top.bus_write_data) {
    printf("PASS %d\n", top.bus_write_data);
    return 0;
  } else {
    printf("FAIL %d\n", top.bus_write_data);
    return -1;
  }
}

int main(int argc, const char **argv, const char **env) {
  const char* instructions[38] = {
    "add", "addi", "and", "andi", "auipc", "beq", "bge", "bgeu", "blt", "bltu",
    "bne", "jal", "jalr", "lb", "lbu", "lh", "lhu", "lui", "lw", "or", "ori",
    "sb", "sh", "simple", "sll", "slli", "slt", "slti", "sltiu", "sltu", "sra",
    "srai", "srl", "srli", "sub", "sw", "xor", "xori"
  };

  for (int i = 0; i < 38; i++) {
    run_test(instructions[i]);
  }
  return 0;
}

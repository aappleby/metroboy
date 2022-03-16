#include "rtl/toplevel.h"
#include <stdio.h>

#pragma warning(disable:4996)

//------------------------------------------------------------------------------

toplevel top;

int run_test(const char* test_name) {
  printf("running %6s: ", test_name);

  char buf1[256];
  char buf2[256];
  sprintf(buf1, "+text_file=tests/%s.text.vh", test_name);
  sprintf(buf2, "+data_file=tests/%s.data.vh", test_name);
  const char* argv2[2] = { buf1, buf2 };

  metron_reset();
  metron_init(2, argv2);

  top.init();

  int time;
  for (time = 0; time < 100000; time++) {
    top.tick(time >= 4 && time <= 7);
    top.tock();

    //printf("0x%08x\n", top.bus_address);

    if (top.bus_write_enable && top.bus_address == 0xfffffff0) break;
  }

  //exit(0);

  if (time == 100000) {
    printf("TIMEOUT\n");
    return -1;
  } else if (top.bus_write_data) {
    printf("PASS %d\n", (int)top.bus_write_data);
    return 0;
  } else {
    printf("FAIL %d\n", (int)top.bus_write_data);
    return -1;
  }
}

//------------------------------------------------------------------------------

int main(int argc, const char **argv) {
  const char* instructions[38] = {
    "add", "addi", "and", "andi", "auipc", "beq", "bge", "bgeu", "blt", "bltu",
    "bne", "jal", "jalr", "lb", "lbu", "lh", "lhu", "lui", "lw", "or", "ori",
    "sb", "sh", "simple", "sll", "slli", "slt", "slti", "sltiu", "sltu", "sra",
    "srai", "srl", "srli", "sub", "sw", "xor", "xori"
  };

  for (int i = 0; i < 38; i++) {
    run_test(instructions[i]);
  }

  printf("Hello World\n");

  return 0;
}

//------------------------------------------------------------------------------

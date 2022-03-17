#include "rtl/toplevel.h"
#include <stdio.h>

#include "Log.h"
#include "Tests.h"

#pragma warning(disable:4996)

//------------------------------------------------------------------------------

int run_test(const char* test_name) {
  fflush(stdout);

  char buf1[256];
  char buf2[256];
  sprintf(buf1, "+text_file=tests/%s.text.vh", test_name);
  sprintf(buf2, "+data_file=tests/%s.data.vh", test_name);
  const char* argv2[2] = { buf1, buf2 };

  metron_reset();
  metron_init(2, argv2);

  int time;
  int result = 0;

  toplevel top;
  top.init();

  LOG_R("running %6s: \n", test_name);
  for (int rep = 0; rep < 100000; rep++)
  {
    top.tick(1);
    top.tock();

    for (time = 0; time < 100000; time++) {
      top.tick(0);
      top.tock();

      //printf("0x%08x\n", top.bus_address);

      if (top.bus_write_enable && top.bus_address == 0xfffffff0) {
        //printf("finish at %d\n", time);
        result = top.bus_write_data;
        break;
      }
    }
  }

  if (time == 100000) {
    LOG_Y("TIMEOUT\n");
    return -1;
  } else if (result) {
    LOG_G("PASS %d @ %d\n", result, time);
    return 0;
  } else {
    LOG_R("FAIL %d @ %d\n", result, time);
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

  return 0;
}

//------------------------------------------------------------------------------

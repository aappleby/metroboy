#include <stdio.h>

#include "metron.h"
#include "uart_top.h"
#include "vcd_dump.h"

#include "message.blob.h"

struct uart_test {
  static const int clock_rate = 24000000;
  static const int bit_rate = 8000000;
  static const int cycles_per_bit = clock_rate / bit_rate;

  uart_top<cycles_per_bit> top;
  int cycle = 1;
  int timestamp = 0;

  void dump_vars(int clk, int rst_n, VcdDump& dump) {
    dump.set_value("clk",   clk,   1);
    dump.set_value("rst_n", rst_n, 1);
    //top.dump_value(dump);
    dump.advance(5);
  }

  int main(int argc, char** arv) {

    VcdDump dump("uart_test_mt.vcd");

    memset(&top, 0, sizeof(top));

    memcpy(top.hello.memory, message, message_len);

    {
      fprintf(dump.file, "$timescale 1ns $end\n");
      fprintf(dump.file, "$scope module top $end\n");

      fprintf(dump.file, "$var wire 1  clk         clk $end\n");
      fprintf(dump.file, "$var wire 1  rst_n       rst_n $end\n");

      //top.dump_vcd_header(dump);
      //top.hello.dump_vcd_header(dump);
      //top.tx.dump_vcd_header(dump);
      //top.rx.dump_vcd_header(dump);

      fprintf(dump.file, "$upscope $end\n");
      fprintf(dump.file, "$enddefinition $end\n");
      fprintf(dump.file, "$dumpvars\n");
    }

    printf("\n");
    printf("Metron simulation:\n");
    printf("================================================================================\n");

    printf("\n");

    dump_vars(0, 1, dump);

    top.tock(0);
    top.tick(0);

    dump_vars(0, 0, dump);
    dump_vars(1, 0, dump);
    dump_vars(0, 1, dump);

    bool old_valid = false;
    for (; cycle < 500000; cycle++) {
      top.tock(1);
      top.tick(1);
      if (!old_valid && top.o_valid) printf("%c", top.o_data);
      old_valid = top.o_valid;

      dump_vars(1, 1, dump);

      dump.set_value("clk", 0, 1);
      if (cycle > 100 && top.tx.o_idle) break;
      dump.advance(5);
      dump_vars(0, 1, dump);
    }

    printf("\n");
    printf("================================================================================\n");
    if (top.o_sum == 0x0000b764) printf("All tests pass.\n");

    return 0;
  }
};

int main(int argc, char** argv) {
  uart_test test;
  return test.main(argc, argv);

}

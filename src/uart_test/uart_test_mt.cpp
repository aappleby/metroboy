#include <stdio.h>

#include "metron.h"
#include "uart_top.h"
#include "vcd_dump.h"

int main(int argc, char** argv) {

  VcdDump dump("test.vcd");

  const int clock_rate = 24000000;
  const int bit_rate   =  8000000;
  const int cycles_per_bit = clock_rate / bit_rate;

  uart_top<cycles_per_bit> top;
  top.initial();
  top.reset();

  {
    fprintf(dump.file, "$timescale 1ns $end\n");
    fprintf(dump.file, "$scope module top $end\n");

    top.dump_vcd_header(dump);
    top.hello.dump_vcd_header(dump);
    top.hello.mem.dump_vcd_header(dump);
    top.tx.dump_vcd_header(dump);
    top.rx.dump_vcd_header(dump);

    fprintf(dump.file, "$upscope $end\n");
    fprintf(dump.file, "$enddefinition $end\n");
    fprintf(dump.file, "$dumpvars\n");
  }

  printf("\n");
  printf("Metron simulation:\n");
  printf("================================================================================\n");

  top.dump_header();
  printf("\n");

  top.tick();
  top.dump();
  printf("\n");

  dump.set_time(0);
  dump.set_value("clk",   0, 1);
  dump.set_value("rst_n", 0, 1);
  top.dump_value(dump);

  bool old_valid = false;
  int cycle = 4;
  for (; cycle < 5000; cycle++) {
    top.tock();
    top.tick();

    dump.set_time(cycle * 10);
    dump.set_value("clk",   1, 1);
    dump.set_value("rst_n", 1, 1);
    top.dump_value(dump);

    if (cycle > 100 && top.tx.o_idle) break;

    dump.set_time(cycle * 10 + 5);
    dump.set_value("clk", 0, 1);

    if (!old_valid && top.o_valid) {
      printf("{{{ %c %02x }}}\n", top.o_data, top.o_data);
    }
    old_valid = top.o_valid;
  }

  printf("\n");
  printf("================================================================================\n");
  printf("cycle = %d\n", cycle);
  printf("checksum = %08x %s\n", top.o_sum, top.o_sum == 0x0000b764 ? "ok" : "fail");
  printf("\n");
  return 0;
}

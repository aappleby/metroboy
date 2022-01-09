#include <stdio.h>

#include "metron.h"
#include "uart_top.h"
#include "vcd_dump.h"

#include "message.blob.h"

int main(int argc, char** argv) {

  VcdDump dump("uart_test_mt.vcd");

  const int clock_rate = 24000000;
  const int bit_rate   =  8000000;
  const int cycles_per_bit = clock_rate / bit_rate;

  uart_top<cycles_per_bit> top;

  memcpy(top.hello.mem.memory, message, message_len);

  top.reset();

  {
    fprintf(dump.file, "$timescale 1ns $end\n");
    fprintf(dump.file, "$scope module top $end\n");

    fprintf(dump.file, "$var wire 1  clk         clk $end\n");
    fprintf(dump.file, "$var wire 1  rst_n       rst_n $end\n");
    fprintf(dump.file, "$var wire 32 timestamp   timestamp $end\n");
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

  top.tick();
  printf("\n");

  dump.set_time(0);
  dump.set_value("clk",   0, 1);
  dump.set_value("rst_n", 0, 1);
  dump.set_value("timestamp", 0, 32);
  top.dump_value(dump);

  bool old_valid = false;
  int cycle = 1;
  for (; cycle < 500000; cycle++) {
    top.tock();
    top.tick();

    dump.set_time((cycle + 3) * 10);
    dump.set_value("clk",   1, 1);
    dump.set_value("rst_n", 1, 1);
    dump.set_value("timestamp", cycle, 32);
    top.dump_value(dump);

    if (cycle > 100 && top.tx.o_idle) break;

    dump.set_time((cycle + 3) * 10 + 5);
    dump.set_value("clk", 0, 1);

    if (!old_valid && top.o_valid) {
      //printf("%c", top.o_data);
      putc(top.o_data, stdout);
    }
    old_valid = top.o_valid;
  }

  printf("\n");
  printf("================================================================================\n");
  printf("cycle = %d\n", cycle);
  printf("checksum = %08x %s\n", top.o_sum, top.o_sum == 0x0000b764 ? "pass" : "fail");
  printf("\n");
  return 0;
}

#include <stdio.h>

#include "metron.h"
#include "uart_top.h"
#include "vcd_dump.h"

int main(int argc, char** argv) {

  VcdDump dump("test.vcd");

  /*
  dump.set_width("foo", 8);
  dump.set_width("bar", 1);
  dump.emit_header();
  
  dump.set_time(0);
  dump.set_value("foo", 100);
  dump.set_value("bar", 1);

  dump.set_time(10);
  dump.set_value("foo", 200);
  dump.set_value("bar", 1);

  dump.set_time(20);
  dump.set_value("foo", 100);
  dump.set_value("bar", 0);

  dump.set_time(30);
  dump.set_value("foo", 100);
  dump.set_value("bar", 1);

  dump.set_time(40);
  */


  const int clock_rate = 24000000;
  const int bit_rate   =  8000000;
  const int clocks_per_bit = clock_rate / bit_rate;

  uart_top<clocks_per_bit> top;
  top.initial();
  top.reset();

  dump.set_width("clk", 1);
  dump.set_width("rst_n", 1);

  top.dump_width(dump);
  //dump.emit_header();

  {
    fprintf(dump.file, "$timescale 1ns $end\n");
    fprintf(dump.file, "$scope module top $end\n");

    top.dump_vcd_header(dump);
    top.hello.dump_vcd_header(dump);
    top.hello.rom.dump_vcd_header(dump);
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
  dump.set_value("clk", 0);
  dump.set_value("rst_n", 0);
  top.dump_value(dump);

  bool old_valid = false;
  int cycle = 16;
  for (; cycle < 200; cycle++) {
    top.tock();
    top.tick();

    dump.set_time(cycle * 10);
    dump.set_value("clk", 1);
    dump.set_value("rst_n", 1);
    top.dump_value(dump);

    dump.set_time(cycle * 10 + 5);
    dump.set_value("clk", 0);

    //top.dump();
    //printf("\n");

    if (!old_valid && top.o_valid) {
      printf("{{{ %c %02x }}}\n", top.o_data, top.o_data);
    }
    old_valid = top.o_valid;
  }

  if (top.o_done) {
    printf("\n");
    printf("================================================================================\n");
    printf("cycle = %d\n", cycle);
    printf("checksum = %08x %s\n", top.o_sum, top.o_sum == 0x0000b764 ? "ok" : "fail");
    printf("\n");
  }
  return 0;
}

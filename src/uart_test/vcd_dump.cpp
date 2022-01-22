#include "vcd_dump.h"

/*
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


void dump_vars(int clk, int rst_n, VcdDump& dump) {
  dump.set_value("clk",   clk,   1);
  dump.set_value("rst_n", rst_n, 1);
  //top.dump_value(dump);
  dump.advance(5);
}
*/
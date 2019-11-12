#include <stdio.h>
#include <verilated.h>
#include "obj_dir/VVRAM.h"

double sc_time_stamp() { return 0; }

/*
int main(int argc, char** argv) {
  printf("----------\n");

  Verilated::commandArgs(argc, argv);

  VVRAM vram;

  vram.rst = 0;

  for (int i = 0; i < 10; i++) {
    printf("cycle %d\n", i);
    vram.clk = 0;
    vram.eval();
    vram.clk = 1;
    vram.eval();
  }

  printf("----------\n");
  printf("Done\n");
}
*/
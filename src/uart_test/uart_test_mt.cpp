#include <stdio.h>

#include "metron.h"
#include "uart_top.h"

int main(int argc, char** argv) {

  const int clock_rate = 24000000;
  const int bit_rate   =  8000000;
  const int clocks_per_bit = clock_rate / bit_rate;

  uart_top<clocks_per_bit> top;
  top.initial();
  top.reset();

  printf("\n");
  printf("Metron simulation:\n");
  printf("================================================================================\n");


  bool old_valid = false;

  for (int cycle = 16; cycle < 20000; cycle++) {
    top.tock();

    top.tick();

    if (!old_valid && top.o_valid) {
      printf("%c", top.o_data);
      if (top.o_done) {
        printf("\n");
        printf("================================================================================\n");
        printf("cycle = %d\n", cycle);
        printf("checksum = %08x %s\n", top.o_sum, top.o_sum == 0x0000b764 ? "ok" : "fail");
        printf("\n");
        break;
      }
    }
    old_valid = top.o_valid;
  }

  return 0;
}

#include <stdio.h>

#include "metron.h"
#include "uart_top.h"
#include "vcd_dump.h"

//#include "message.blob.h"

static const int clock_rate = 24000000;
static const int bit_rate = 8000000;
static const int cycles_per_bit = clock_rate / bit_rate;


int main(int argc, char** arv) {
  uart_top<cycles_per_bit> top;
  top.init();

  printf("\n");
  printf("Metron simulation:\n");
  printf("================================================================================\n");

  printf("\n");

  top.tick(0);
  top.tock(0);

  int timestamp = 0;

  while(1) {
    bool old_valid = top.o_valid;
    top.tick(1);
    top.tock(1);
    timestamp += 10;
    if (!old_valid && top.o_valid) printf("%c", (uint8_t)top.o_data);
    old_valid = top.o_valid;

    if (timestamp > 100 && top.tx.o_idle) break;
  }

  printf("\n");
  printf("================================================================================\n");
  if (top.o_sum == 0x0000b764) printf("All tests pass.\n");

  return 0;
}

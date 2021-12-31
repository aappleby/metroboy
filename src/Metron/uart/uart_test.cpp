#include <stdio.h>

#include "uart_bench.cpp"

int main(int argc, char** argv) {
  const int clock_rate = 24000000;
  const int bit_rate   =  8000000;
  const int clocks_per_bit = clock_rate / bit_rate;

  uart_top<clocks_per_bit> top;
  bool ser_tx;
  uint8_t leds;

  top.initial();
  top.reset();

  /*
  top.dump_title();
  printf("\n");
  top.dump();
  printf("\n");
  */

  uint64_t cycle = 15;

  int reps = 0;

  while(1) {
    uint8_t old_data = 0;
    bool old_valid = 0;
    top.rx.tick(old_data, old_valid);

    top.tick(ser_tx, leds);
    top.tock(ser_tx, leds);
    cycle++;
    //top.dump();
    //printf("\n");

    uint8_t new_data = 0;
    bool new_valid = 0;
    top.rx.tick(new_data, new_valid);

    if (!old_valid && new_valid) {
      if (new_data == '\n') {
        reps++;
        printf("%8d \\n\n", cycle);
        if (reps == 2) break;
      }
      else {
        printf("%8d %c\n", cycle, new_data);
      }
    }
  }

  return 0;
}

#include <stdio.h>

#include "metron.h"
#include "uart_top.cpp"

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
  int reps = 0;
  for (int cycle = 16; cycle < 20000; cycle++) {
    uint8_t old_data = 0;
    bool old_valid = 0;
    top.rx.tick(old_data, old_valid);

    bool ser_tx;
    uint8_t leds;
    top.tick(ser_tx, leds);
    top.tock(ser_tx);

    uint8_t new_data = 0;
    bool new_valid = 0;
    top.rx.tick(new_data, new_valid);

    if (!old_valid && new_valid) {
      printf("%c", new_data);
    }
  }

  printf("\n");
  printf("================================================================================\n");
  printf("\n");

  return 0;
}

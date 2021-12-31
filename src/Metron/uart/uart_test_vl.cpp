#include <stdio.h>

#include "obj/Vuart_top.h"

int main(int argc, char** argv) {
  printf("Verilator simulation:\n");
  Vuart_top uart_top;

  uart_top.clk = 0;
  uart_top.rst_n = 0;
  uart_top.eval();

  uart_top.clk = 1;
  uart_top.rst_n = 0;
  uart_top.eval();

  int reps = 0;
  for (int cycle = 16; cycle < 10000; cycle++) {
    auto old_valid = uart_top.out_valid;

    uart_top.clk = 0;
    uart_top.rst_n = 1;
    uart_top.eval();
    uart_top.clk = 1;
    uart_top.rst_n = 1;
    uart_top.eval();

    auto new_valid = uart_top.out_valid;

    if (!old_valid && new_valid) {
      if (uart_top.out_data == '\n') {
        reps++;
        printf("%8d \\n\n", cycle);
        if (reps == 2) break;
      }
      else {
        printf("%8d %c\n", cycle, uart_top.out_data);
      }
    }
  }

  return 0;
}

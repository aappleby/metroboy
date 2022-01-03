#include <stdio.h>

#include "obj/DUT.h"
#include "obj/DUT_uart_top.h"
#include "obj/DUT_uart_rx__C3.h"

int main(int argc, char** argv) {
  printf("\n");
  printf("Verilator simulation:\n");
  printf("================================================================================\n");
  DUT dut;

  dut.clk = 0;
  dut.rst_n = 0;
  dut.eval();

  dut.clk = 1;
  dut.rst_n = 0;
  dut.eval();

  int reps = 0;
  for (int cycle = 16; cycle < 20000; cycle++) {
    auto old_valid = dut.top_o_valid;

    dut.clk = 0;
    dut.rst_n = 1;
    dut.eval();
    dut.clk = 1;
    dut.rst_n = 1;
    dut.eval();

    auto new_valid = dut.top_o_valid;

    if (!old_valid && new_valid) {

      printf("%c", dut.top_o_data);
      if (dut.top_o_done) {
        printf("\n");
        printf("================================================================================\n");
        printf("cycle %d\n", cycle);
        printf("checksum %x %s\n", dut.top_o_sum, dut.top_o_sum == 0x0000b989 ? "ok" : "fail" );
        printf("\n");
        return dut.top_o_sum == 0x0000b989 ? 0 : -1;
      }
    }
  }

  printf("stuck!\n");
  return -1;
}

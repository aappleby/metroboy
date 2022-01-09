#include <stdio.h>

#include "obj/DUT.h"
#include "obj/DUT_uart_top.h"
#include "obj/DUT_uart_hello.h"
#include "obj/DUT_uart_tx__C3.h"
#include "obj/DUT_uart_rx__C3.h"
#include "obj/DUT_blockram_512x8.h"

#include "message.blob.h"

int main(int argc, char** argv) {
  Verilated::traceEverOn(true);

  DUT dut;
  memcpy(dut.uart_top->hello->mem->memory.m_storage, message, message_len);


  printf("\n");
  printf("Verilator simulation:\n");
  printf("================================================================================\n");

  dut.clk = 0;
  dut.rst_n = 0;
  dut.eval();

  dut.clk = 1;
  dut.rst_n = 0;
  dut.eval();

  printf("%p", dut.uart_top->hello->mem->memory.m_storage);

  int reps = 0;
  for (int cycle = 1; cycle < 40000; cycle++) {
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
    }

    if (cycle > 100 && dut.uart_top->tx->o_idle) {
      bool pass = dut.top_o_sum == 0x0000b764;

      printf("\n");
      printf("================================================================================\n");
      printf("cycle %d\n", cycle);
      printf("checksum %x %s\n", dut.top_o_sum, pass ? "pass" : "fail");
      printf("\n");
      return pass ? 0 : -1;
    }
  }

  printf("stuck!\n");
  return -1;
}

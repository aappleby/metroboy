#include <stdio.h>

#include "obj/DUT.h"
#include "obj/DUT_uart_top.h"
#include "obj/DUT_uart_hello.h"
#include "obj/DUT_uart_tx__C3.h"
#include "obj/DUT_uart_rx__C3.h"
#include "obj/DUT_blockram_512x8.h"

#include "verilated_vcd_c.h"
#include "message.blob.h"

int main(int argc, char** argv) {
  Verilated::commandArgs(argc, argv);
  Verilated::traceEverOn(true);
  VerilatedVcdC* tfp = new VerilatedVcdC;
  tfp->spTrace()->set_time_resolution("1 ns");

  DUT dut;
  memcpy(dut.uart_top->hello->mem->memory.m_storage, message, message_len);
  dut.trace(tfp, 99);
  tfp->open("uart_test_vl.vcd");


  printf("\n");
  printf("Verilator simulation:\n");
  printf("================================================================================\n");

  double timestamp = 0;
  int cycle = 0;

  timestamp = 0;
  dut.clk = 0;
  dut.rst_n = 1;
  dut.eval();
  tfp->dump(timestamp);

  timestamp = 5;
  dut.clk = 0;
  dut.rst_n = 0;
  dut.eval();
  tfp->dump(timestamp);

  timestamp = 10;
  dut.clk = 1;
  dut.rst_n = 0;
  dut.eval();
  tfp->dump(timestamp);

  timestamp = 15;
  dut.clk = 0;
  dut.rst_n = 1;
  dut.eval();
  tfp->dump(timestamp);

  timestamp = 20;
  while (!Verilated::gotFinish()) {
    cycle++;
    auto old_valid = dut.top_o_valid;

    dut.clk = 1;
    dut.rst_n = 1;
    dut.eval();
    tfp->dump(timestamp);
    timestamp += 5;

    dut.clk = 0;
    dut.rst_n = 1;
    dut.eval();
    tfp->dump(timestamp);
    timestamp += 5;

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
      tfp->close();
      return pass ? 0 : -1;
    }
  }

  printf("stuck!\n");
  tfp->close();
  return -1;
}

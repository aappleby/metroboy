#include <stdio.h>

#include "obj/DUT.h"
#include "obj/DUT_uart_top.h"
#include "obj/DUT_uart_hello.h"
#include "obj/DUT_uart_tx__C3.h"
#include "obj/DUT_uart_rx__C3.h"

#include "verilated_vcd_c.h"
#include "message.blob.h"

int main(int argc, char** argv) {
  Verilated::commandArgs(argc, argv);
  Verilated::traceEverOn(true);
  VerilatedVcdC* tfp = new VerilatedVcdC;
  tfp->spTrace()->set_time_resolution("1 ns");

  DUT dut;
  memcpy(dut.uart_top->hello->memory.m_storage, message, message_len);
  dut.trace(tfp, 99);
  tfp->open("uart_test_vl.vcd");

  printf("\n");
  printf("Verilator simulation:\n");
  printf("================================================================================\n");

  int timestamp = 0;

  timestamp = 0;
  dut.clk = 0;
  dut.rst_n = 1;
  dut.eval();
  tfp->dump(timestamp);

  timestamp += 5;
  dut.clk = 0;
  dut.rst_n = 0;
  dut.eval();
  tfp->dump(timestamp);

  timestamp += 5;
  dut.clk = 1;
  dut.rst_n = 0;
  dut.eval();
  tfp->dump(timestamp);

  timestamp += 5;
  dut.clk = 0;
  dut.rst_n = 1;
  dut.eval();
  tfp->dump(timestamp);


  while (timestamp <= 153575) {
    timestamp += 5;

    auto old_valid = dut.o_valid;
    dut.clk = !dut.clk;
    dut.eval();
    tfp->dump(timestamp);
    auto new_valid = dut.o_valid;

    if (!old_valid && new_valid) printf("%c", dut.o_data);
    if (timestamp > 25 && dut.uart_top->tx->o_idle) break;
  }

  printf("\n");
  printf("================================================================================\n");
  if (dut.o_sum == 0x0000b764) printf("All tests pass.\n");
  tfp->close();

  return 0;
}

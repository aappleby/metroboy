#include <stdio.h>
#include <assert.h>

#include "metron_tools.h"
#include "uart_top.h"
#include "vcd_dump.h"

#include "message.blob.h"

#include "verilated_vcd_c.h"
#include "obj/DUT.h"
#include "obj/DUT_uart_top.h"
#include "obj/DUT_uart_hello.h"
#include "obj/DUT_uart_tx__C3.h"
#include "obj/DUT_uart_rx__C3.h"

int main(int argc, char** argv) {
  printf("Metron+Verilator lockstep simulation:\n");
  printf("================================================================================\n");

  int timestamp = 0;

  // Trace setup
  Verilated::commandArgs(argc, argv);
  Verilated::traceEverOn(true);
  VerilatedVcdC* tfp = new VerilatedVcdC;
  tfp->spTrace()->set_time_resolution("1 ns");

  // Metron sim
  const int cycles_per_bit = 3;
  uart_top<cycles_per_bit> top;
  memset(&top, 0, sizeof(top));
  top.init();
  top.tick(0);
  top.tock(0);

  // Verilated Metron sim
  DUT dut;
  dut.trace(tfp, 99);
  tfp->open("uart_test_vl.vcd");

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

  for (int cycle = 0; cycle < 20000; cycle++) {

    bool old_valid = top.o_valid;

    dut.clk = 0;
    dut.rst_n = 1;
    dut.eval();
    tfp->dump(timestamp);
    timestamp += 5;

    dut.clk = 1;
    dut.rst_n = 1;
    dut.eval();
    tfp->dump(timestamp);
    timestamp += 5;

    top.tick(1);
    top.tock(1);

    assert(top.o_serial == dut.o_serial);
    assert(top.o_data == dut.o_data);
    assert(top.o_valid == dut.o_valid);
    assert(top.o_done == dut.o_done);
    assert(top.o_sum == dut.o_sum);


    if (!old_valid && top.o_valid) printf("%c", (uint8_t)dut.o_data);
    
    if (dut.o_done) {
      printf("\n");
      printf("================================================================================\n");
      if (top.o_sum == 0x0000b764) printf("All tests pass.\n");
      tfp->close();
      return 0;
    }
  }

  tfp->close();
  return -1;
}

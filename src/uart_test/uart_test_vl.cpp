#include <stdio.h>
#include <assert.h>

#include "metron.h"
#include "uart_top.h"
#include "vcd_dump.h"

#include "message.blob.h"

#include "verilated_vcd_c.h"
#include "obj/DUT.h"
#include "obj/DUT_uart_top.h"
#include "obj/DUT_uart_hello.h"
#include "obj/DUT_uart_tx__C3.h"
#include "obj/DUT_uart_rx__C3.h"


static const int clock_rate = 24000000;
static const int bit_rate = 8000000;
static const int cycles_per_bit = clock_rate / bit_rate;

void check(uart_top<cycles_per_bit>& top, DUT& dut) {
  assert(top.o_serial == dut.o_serial);
  assert(top.o_data == dut.o_data);
  assert(top.o_valid == dut.o_valid);
  assert(top.o_done == dut.o_done);
  assert(top.o_sum == dut.o_sum);
}

int main(int argc, char** argv) {
  // Trace setup
  Verilated::commandArgs(argc, argv);
  Verilated::traceEverOn(true);
  VerilatedVcdC* tfp = new VerilatedVcdC;
  tfp->spTrace()->set_time_resolution("1 ns");

  // Metron sim
  uart_top<cycles_per_bit> top;
  top.init();

  // Verilated Metron sim
  DUT dut;
  dut.trace(tfp, 99);
  tfp->open("uart_test_vl.vcd");

  printf("\n");
  printf("Metron+Verilator lockstep simulation:\n");
  printf("================================================================================\n");

  int timestamp = 0;
  auto step = [&](int rst_n) {
    dut.clk = 0;
    dut.rst_n = rst_n;
    dut.eval();
    tfp->dump(timestamp);
    timestamp += 5;

    dut.clk = 1;
    dut.rst_n = rst_n;
    dut.eval();
    tfp->dump(timestamp);
    timestamp += 5;

    top.tock(rst_n);
    top.tick(rst_n);
  };

  step(0);


  while (1) {

    bool old_valid = top.o_valid;
    step(1);
    check(top, dut);
    if (!old_valid && top.o_valid) printf("%c", (uint8_t)dut.o_data);
    if (timestamp > 100 && dut.uart_top->tx->o_idle) break;
  }

  printf("\n");
  printf("================================================================================\n");
  if (top.o_sum == 0x0000b764) printf("All tests pass.\n");
  tfp->close();

  return 0;
}

#include <assert.h>
#include <stdint.h>
#include <stdio.h>

#include "../CoreLib/Tests.h"
#include "generated/uart/Vuart_top.h"
#include "generated/uart/Vuart_top_uart_hello.h"
#include "generated/uart/Vuart_top_uart_rx__C3.h"
#include "generated/uart/Vuart_top_uart_top.h"
#include "generated/uart/Vuart_top_uart_tx__C3.h"
#include "uart/uart_top.h"

//------------------------------------------------------------------------------

void dump(Vuart_top& vtop) {
  printf("v: ");
  printf("s %03d d %03d v %03d d %03d s %03d", (int)vtop.o_serial, (int)vtop.o_data, (int)vtop.o_valid, (int)vtop.o_done, (int)vtop.o_sum);
}

void dump(uart_top<3>& mtop) {
  printf("m: ");
  printf("s %03d d %03d v %03d d %03d s %03d", (int)mtop.o_serial, (int)mtop.o_data, (int)mtop.o_valid, (int)mtop.o_done, (int)mtop.o_sum);
}

//------------------------------------------------------------------------------

TestResults test_lockstep(int argc, char** argv) {
  TEST_INIT("Metron+Verilator lockstep simulation:");

  const int cycles_per_bit = 3;
  uart_top<cycles_per_bit> mtop;
  mtop.init();
  mtop.tick(0);
  mtop.tock();

  Vuart_top vtop;

  vtop.clk = 0;
  vtop.i_rstn = 0;
  vtop.eval();
  vtop.i_rstn = 1;
  vtop.clk = 1;
  vtop.eval();
  vtop.clk = 0;
  vtop.eval();

  LOG_B("========================================\n");
  
  int cycle;
  for (cycle = 0; cycle < 20000; cycle++) {

    if (mtop.o_serial != vtop.o_serial ||
        mtop.o_data   != vtop.o_data   ||
        mtop.o_valid  != vtop.o_valid  ||
        mtop.o_done   != vtop.o_done   ||
        mtop.o_sum    != vtop.o_sum) {
      TEST_FAIL("Mismatch @ %d\n", cycle);
    }

    bool old_valid = vtop.o_valid;

    mtop.tick(1);
    mtop.tock();

    vtop.clk = 1;
    vtop.eval();
    vtop.clk = 0;
    vtop.eval();

    if (!old_valid && vtop.o_valid) LOG_B("%c", (uint8_t)vtop.o_data);

    if (vtop.o_done) {
      break;
    }
  }

  LOG_B("\n");
  LOG_B("========================================\n");
  LOG_B("%d\n", cycle);
  EXPECT_EQ(mtop.o_sum, 0x0000b764, "Metron uart checksum fail");
  EXPECT_EQ(vtop.o_sum, 0x0000b764, "Verilator uart checksum fail");

  TEST_DONE();
}

//------------------------------------------------------------------------------

int main(int argc, char** argv) {
  printf("Running Verilated Metron uart test\n");
  TestResults results;
  results += test_lockstep(argc, argv);
  if (results.test_fail) {
    printf("Some tests fail.\n");
  } else {
    printf("All tests pass.\n");
  }
  return 0;
}

//------------------------------------------------------------------------------

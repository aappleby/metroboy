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
  printf(
      "hello i_cts=%d i_idle=%d o_data=x%02x o_req=%d o_done=%d s=%d "
      "data=x%02x cursor=%d",
      vtop.uart_top->hello->i_cts, vtop.uart_top->hello->i_idle,
      vtop.uart_top->hello->o_data, vtop.uart_top->hello->o_req,
      vtop.uart_top->hello->o_done, vtop.uart_top->hello->s,
      vtop.uart_top->hello->data, vtop.uart_top->hello->cursor);
  printf("\n");
}

void dump(uart_top<3>& top) {
  printf("m: ");
  printf(
      "hello i_cts=%d i_idle=%d o_data=x%02x o_req=%d o_done=%d s=%d "
      "data=x%02x cursor=%d",
      (int)top.tx.o_cts,   // top.hello.i_cts,
      (int)top.tx.o_idle,  // top.hello.i_idle,
      (int)top.hello.o_data, (int)top.hello.o_req, (int)top.hello.o_done,
      (int)top.hello.s, (int)top.hello.data, (int)top.hello.cursor);
  printf("\n");
}

//------------------------------------------------------------------------------

TestResults test_metron(int argc, char** argv) {
  TEST_INIT("Metron simulation:");

  const int cycles_per_bit = 3;
  uart_top<cycles_per_bit> top;
  top.init();
  top.tick(0);
  top.tock();

  LOG_B("========================================\n");

  for (int cycle = 0; cycle < 20000; cycle++) {
    bool old_valid = top.o_valid;
    top.tick(1);
    top.tock();
    if (!old_valid && top.o_valid) LOG_B("%c", (uint8_t)top.o_data);

    if (top.o_done) {
      LOG_B("\n");
      LOG_B("========================================\n");
      LOG_B("%d\n", cycle);
      EXPECT_EQ(top.o_sum, 0x0000b764, "Uart checksum fail");
      break;
    }
  }

  TEST_DONE();
}

//------------------------------------------------------------------------------

TestResults test_lockstep(int argc, char** argv) {
  TEST_INIT("Metron+Verilator lockstep simulation:");

  Vuart_top vuart_top;

  vuart_top.clk = 0;
  vuart_top.i_rstn = 0;
  vuart_top.eval();
  vuart_top.i_rstn = 1;
  vuart_top.eval();

  LOG_B("========================================\n");

  for (int cycle = 0; cycle < 20000; cycle++) {
    bool old_valid = vuart_top.o_valid;
    vuart_top.clk = 1;
    vuart_top.eval();
    vuart_top.clk = 0;
    vuart_top.eval();
    if (!old_valid && vuart_top.o_valid) LOG_B("%c", (uint8_t)vuart_top.o_data);

    // muart_top.tick(1);
    // muart_top.tock(1);
    //
    // if (muart_top.o_serial != vuart_top.o_serial ||
    //    muart_top.o_data   != vuart_top.o_data ||
    //    muart_top.o_valid  != vuart_top.o_valid ||
    //    muart_top.o_done   != vuart_top.o_done ||
    //    muart_top.o_sum    != vuart_top.o_sum) {
    //  printf("Mismatch @ %d\n", cycle);
    //  return;
    //}

    if (vuart_top.o_done) {
      LOG_B("\n");
      LOG_B("========================================\n");
      LOG_B("%d\n", cycle);
      // EXPECT_EQ(vuart_top.o_sum, 0x0000b764, "Uart checksum fail");
      break;
    }
  }

  TEST_DONE();
}

//------------------------------------------------------------------------------

int main(int argc, char** argv) {
  printf("Running Verilated Metron uart test\n");
  TestResults results = test_metron(argc, argv);
  test_lockstep(argc, argv);
  if (results.test_fail) {
    printf("Some tests fail.\n");
  } else {
    printf("All tests pass.\n");
  }
  return 0;
}

//------------------------------------------------------------------------------

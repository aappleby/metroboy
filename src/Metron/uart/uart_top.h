#pragma once
#include "../metron_tools.h"
#include "uart_rx.h"
#include "uart_tx.h"
#include "uart_hello.h"

//==============================================================================
/* verilator lint_off WIDTH */

template<int cycles_per_bit = 3>
struct uart_top {

  uart_hello hello;
  uart_tx<cycles_per_bit> tx;
  uart_rx<cycles_per_bit> rx;

  logic<1>  o_serial;
  logic<8>  o_data;
  logic<1>  o_valid;
  logic<1>  o_done;
  logic<32> o_sum;

  //----------------------------------------

  void init() {
    printf("uart_top.init()\n");

    hello.init();
    tx.init();
    rx.init();

    o_serial = 0;
    o_data = 0;
    o_valid = 0;
    o_done = 0;
    o_sum = 0;
  }

  //----------------------------------------

  void tick(bool rst_n) {
    hello.tick(rst_n, tx.o_cts, tx.o_idle);
    tx.tick(rst_n, hello.o_data, hello.o_req);
    rx.tick(rst_n, tx.o_serial);
  }

  //----------------------------------------

  void tock(bool rst_n) {
    hello.tock(rst_n);
    tx.tock(rst_n);
    rx.tock(rst_n);

    o_serial = tx.o_serial;
    o_data = rx.o_data;
    o_valid = rx.o_valid;
    o_done = hello.o_done && tx.o_idle;
    o_sum = rx.o_sum;
  }
};

//==============================================================================

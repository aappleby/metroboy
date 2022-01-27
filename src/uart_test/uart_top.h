#pragma once
#include "metron.h"
#include "uart_rx.h"
#include "uart_tx.h"
#include "uart_hello.h"

//==============================================================================

template<int cycles_per_bit = 3>
struct uart_top {

  //----------------------------------------
  /*verilator public_module*/

  uart_hello hello;

  uart_tx<cycles_per_bit> tx;

  uart_rx<cycles_per_bit> rx;

  logic<1> o_serial;
  logic<8> o_data;
  logic<1> o_valid;
  logic<1> o_done;
  logic<32> o_sum;

  //----------------------------------------

  void init() {
    hello.init();
    tx.init();
    rx.init();
  }

  //----------------------------------------

  void tick(bool rst_n) {
    hello.tick(rst_n, tx.o_cts, tx.o_idle);
    tx.tick(rst_n, hello.o_data, hello.o_req);
    rx.tick(rst_n, tx.o_serial);

    if (!rst_n) {
    }
    else {
    }
  }

  //----------------------------------------

  void tock(bool rst_n) {
    hello.tock(rst_n);
    tx.tock(rst_n);
    rx.tock(rst_n);

    o_serial = tx.o_serial;
    o_data = rx.o_data;
    o_valid = rx.o_valid;
    o_done = hello.o_done;
    o_sum = rx.o_sum;
  }

  //----------------------------------------
};

//==============================================================================

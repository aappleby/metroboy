#pragma once
#include "metron.h"

#include "uart_rx.h"
#include "uart_tx.h"
#include "uart_hello.h"

//==============================================================================

template<int clocks_per_bit = 3>
struct uart_top : public Module {

  //----------------------------------------

  uint8_t  o_data;
  bool     o_valid;
  bool     o_done;
  uint32_t o_sum;

  //----------------------------------------

  uart_hello hello;
  uart_tx<clocks_per_bit> tx;
  uart_rx<clocks_per_bit> rx;

  void initial() {
    hello.initial();
    tx.initial();
    rx.initial();
  }

  void reset() {
    hello.reset();
    tx.reset();
    rx.reset();
  }

  void tick() {
    hello.tick();
    tx.tick();
    rx.tick();

    hello.i_cts = tx.o_cts;
    hello.i_idle = tx.o_idle;

    tx.i_data = hello.o_data;
    tx.i_req = hello.o_req;

    rx.i_serial = tx.o_serial;

    o_data  = rx.o_data;
    o_done  = hello.o_done;
    o_valid = rx.o_valid;
    o_sum   = rx.o_sum;
  }

  void tock() {
    hello.tock();
    tx.tock();
    rx.tock();
  }
};


//==============================================================================

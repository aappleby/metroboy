#pragma once
#include "metron.h"
#include "uart_rx.h"
#include "uart_tx.h"
#include "uart_hello.h"

/*
dontcare

case (select)
0: out[0] = 1;
1: out[1] = 1;
2: out[2] = 1;
3: out[3] = 1;
4: out[4] = 1;
5: out[5] = 1;
6: out[6] = 1;
7: out[7] = 1;
endcase

name init/tick/tock blocks
*/

//==============================================================================

template<int cycles_per_bit = 3>
struct uart_top {

  uart_hello hello;
  uart_tx<cycles_per_bit> tx;
  uart_rx<cycles_per_bit> rx;

  logic<1> o_serial;
  logic<8> o_data;
  logic<1> o_valid;
  logic<1> o_done;
  logic<32> o_sum;
  logic<8> o_onehot;

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
    o_onehot = 0;
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
    o_done = hello.o_done && tx.o_idle;
    o_sum = rx.o_sum;

    switch (o_data & 0b111) {
    case 0 : o_onehot = 0b00000001;
    case 1 : o_onehot = 0b00000010;
    case 2 : o_onehot = 0b00000100;
    case 3 : o_onehot = 0b00001000;
    case 4 : o_onehot = 0b00010000;
    case 5 : o_onehot = 0b00100000;
    case 6 : o_onehot = 0b01000000;
    case 7 : o_onehot = 0b10000000;
    default: o_onehot = 0b00000000;
    }
  }

};

//==============================================================================

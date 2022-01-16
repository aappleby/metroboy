#pragma once
#include "metron.h"

#include "uart_rx.h"
#include "uart_tx.h"
#include "uart_hello.h"

//==============================================================================

template<int cycles_per_bit = 3>
struct uart_top {
  //input logic clk,
  //input logic rst_n,

  logic<1>  o_serial;
  logic<8>  o_data;
  logic<1>  o_valid;
  logic<1>  o_done;
  logic<32> o_sum;


  //----------------------------------------

  /*verilator public_module*/

  //logic       hello_i_cts;
  //logic       hello_i_idle;
  //logic[7:0]  hello_o_data;
  //logic       hello_o_req;
  //logic       hello_o_done;
  uart_hello hello/*(clk, rst_n, hello_i_cts, hello_i_idle, hello_o_data, hello_o_req, hello_o_done)*/;

  //logic[7:0]  tx_i_data;
  //logic       tx_i_req;
  //logic       tx_o_serial;
  //logic       tx_o_cts;
  //logic       tx_o_idle;
  uart_tx<cycles_per_bit> tx/*(clk, rst_n, tx_i_data, tx_i_req, tx_o_serial, tx_o_cts, tx_o_idle)*/;

  //logic       rx_i_serial;
  //logic[7:0]  rx_o_data;
  //logic       rx_o_valid;
  //logic[31:0] rx_o_sum;
  uart_rx<cycles_per_bit> rx/*(clk, rst_n, rx_i_serial, rx_o_data, rx_o_valid, rx_o_sum)*/;

  //----------------------------------------

  void tick(bool rst_n) {
    hello.tick(rst_n);
    tx.tick(rst_n);
    rx.tick(rst_n);

    o_serial = tx.o_serial;
    o_data   = rx.o_data;
    o_valid  = rx.o_valid;
    o_done   = hello.o_done;
    o_sum    = rx.o_sum;

    hello.i_cts  = tx.o_cts;
    hello.i_idle = tx.o_idle;
    tx.i_data    = hello.o_data;
    tx.i_req     = hello.o_req;
    rx.i_serial  = tx.o_serial;
  }

  //----------------------------------------

  void tock(bool rst_n) {
    hello.tock(rst_n);
    tx.tock(rst_n);
    rx.tock(rst_n);

    if (!rst_n) {
    } else {
    }
  }
};


//==============================================================================

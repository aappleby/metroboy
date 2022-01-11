#pragma once
#include "metron.h"

#include "uart_rx.h"
#include "uart_tx.h"
#include "uart_hello.h"

#include "vcd_dump.h"

//==============================================================================

template<int cycles_per_bit = 3>
struct uart_top {
  //input logic clk,
  //input logic rst_n,

  bool     o_serial;
  uint8_t  o_data;
  bool     o_valid;
  bool     o_done;
  uint32_t o_sum;

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

  //----------------------------------------

  void dump_header() {
    printf("[top data valid done sum ] ");
    hello.dump_header();
    tx.dump_header();
    rx.dump_header();
  }

  void dump() {
    printf("[    %04x %-5d %-4d %04x] ", o_data, o_valid, o_done, o_sum);
    hello.dump();
    tx.dump();
    rx.dump();
  }

  void dump_vcd_header(VcdDump& d) {
    fprintf(d.file, "$var wire 8  top_o_data  o_data $end\n");
    fprintf(d.file, "$var wire 1  top_o_valid o_valid $end\n");
    fprintf(d.file, "$var wire 1  top_o_done  o_done $end\n");
    fprintf(d.file, "$var wire 32 top_o_sum   o_sum $end\n");
  }

  void dump_value(VcdDump& d) {
    d.set_value("top_o_data",  o_data, 8);
    d.set_value("top_o_valid", o_valid, 1);
    d.set_value("top_o_done",  o_done, 1);
    d.set_value("top_o_sum",   o_sum, 32);
    hello.dump_value(d);
    tx.dump_value(d);
    rx.dump_value(d);
  }
};


//==============================================================================

#pragma once
#include "metron.h"

#include "uart_rx.h"
#include "uart_tx.h"
#include "uart_hello.h"
#include "vcd_dump.h"

//==============================================================================

template<int cycles_per_bit = 3>
struct uart_top : public Module {

  //----------------------------------------

  uint8_t  o_data;
  bool     o_valid;
  bool     o_done;
  uint32_t o_sum;

  //----------------------------------------

  uart_hello hello;
  uart_tx<cycles_per_bit> tx;
  uart_rx<cycles_per_bit> rx;

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

  void dump_header() {
    printf("[top data valid done sum ] ");
    hello.dump_header();
    tx.dump_header();
    rx.dump_header();
  }

  void dump() {
    printf("[    %-04x %-5d %-4d %04x] ", o_data, o_valid, o_done, o_sum);
    hello.dump();
    tx.dump();
    rx.dump();
  }

  void dump_vcd_header(VcdDump& d) {
    fprintf(d.file, "$var wire 1  clk         clk $end\n");
    fprintf(d.file, "$var wire 1  rst_n       rst_n $end\n");
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

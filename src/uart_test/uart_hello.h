#pragma once
#include "metron.h"
#include "vcd_dump.h"

//==============================================================================

struct uart_hello {
  //input logic clk,
  //input logic rst_n,

  bool i_cts;
  bool i_idle;
  
  uint8_t o_data;
  bool    o_req;
  bool    o_done;

  //----------------------------------------
  /*verilator public_module*/

  //static const int message_len = 7;
  static const int message_len = 512;
  static const int cursor_bits = clog2(message_len);

  typedef enum { WAIT, SEND, DONE } e_state;
  e_state state;
  int     cursor;

  //----------------------------------------

  uint8_t memory[512];
  uint8_t data;





  //----------------------------------------

  void tick(bool rst_n) {
    o_data = data;
    o_req  = state == SEND;
    o_done = state == DONE;
  }

  //----------------------------------------

  void tock(bool rst_n) {
    if (!rst_n) {
      state = WAIT;
      cursor = 0;
    } else {
      data = memory[cursor];
      if (state == WAIT && i_idle) {
        state = SEND;
      } else if (state == SEND && i_cts) {
        if (cursor == (message_len - 1)) state = DONE;
        cursor = cursor + 1;
      } else if (state == DONE) {
        state = WAIT;
        cursor = 0;
      }
    }
  }

  //----------------------------------------


  void dump_header() {
    printf("[hlo state cursor data req done] ");
  }

  void dump() {
    printf("[    %-5d %-6d %04x %-3d %-4d] ", state, cursor, o_data, o_req, o_done);
  }

  void dump_vcd_header(VcdDump& d) {
    fprintf(d.file, "$scope module hello $end\n");
    fprintf(d.file, "$var wire 2 hello_state  state  $end\n");
    fprintf(d.file, "$var wire 9 hello_cursor cursor $end\n");
    fprintf(d.file, "$var wire 8 hello_o_data o_data $end\n");
    fprintf(d.file, "$var wire 1 hello_o_req  o_req  $end\n");
    fprintf(d.file, "$var wire 1 hello_o_done o_done $end\n");
    fprintf(d.file, "$upscope $end\n");
  }

  void dump_value(VcdDump& d) {
    d.set_value("hello_state",  state,  2);
    d.set_value("hello_cursor", cursor, 9);
    d.set_value("hello_o_data", o_data, 8);
    d.set_value("hello_o_req",  o_req,  1);
    d.set_value("hello_o_done", o_done, 1);
  }

};

//==============================================================================

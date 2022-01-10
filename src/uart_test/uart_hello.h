#pragma once
#include "metron.h"
#include "blockram_512x8.h"
#include "vcd_dump.h"

//==============================================================================

struct uart_hello {

  //----------------------------------------

  bool    i_cts;
  bool    i_idle;
  
  uint8_t o_data;
  bool    o_req;
  bool    o_done;

  //----------------------------------------

  static const int message_len = 7;
  //static const int message_len = 512;
  static const int cursor_bits = clog2(message_len);

  typedef enum { WAIT, SEND, DONE } e_state;
  e_state state;
  int     cursor;

  //----------------------------------------

  //logic[8:0] mem_i_addr;
  //logic[7:0] mem_o_data;
  blockram_512x8 mem;

  //----------------------------------------

  void reset() {
    mem.reset();
    state = WAIT;
    cursor = 0;
  }

  //----------------------------------------

  void tick() {
    mem.tick();

    o_data = mem.o_data;
    o_req  = state == SEND;
    o_done = state == DONE;
    mem.i_addr = cursor;
  }

  //----------------------------------------

  void tock() {
    mem.tock();

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

  //----------------------------------------
















  void dump_header() {
    printf("[hlo state cursor data req done] ");
    mem.dump_header();
  }

  void dump() {
    printf("[    %-5d %-6d %-04x %-3d %-4d] ", state, cursor, o_data, o_req, o_done);
    mem.dump();
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
    mem.dump_value(d);
  }
};

//==============================================================================

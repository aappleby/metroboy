#pragma once
#include "metron.h"
#include "blockram_512x8.h"
#include "vcd_dump.h"

//==============================================================================

#include "message.blob.h"

struct uart_hello : public Module {

  //----------------------------------------

  bool    i_cts;
  bool    i_idle;
  
  uint8_t o_data;
  bool    o_req;
  bool    o_done;

  //----------------------------------------

  static const int message_len = 7;

  blockram_512x8 rom;

  enum { WAIT, SEND, DONE };
  
  int  state;
  int  cursor;

  //----------------------------------------

  void initial() {
#include "message.blob.h"
    rom.initial(message, message_len);
  }

  //----------------------------------------

  void reset() {
    rom.reset();
    state = WAIT;
    cursor = 0;
  }

  //----------------------------------------

  void tick() {
    rom.tick();

    o_data = rom.o_data;
    o_req  = state == SEND;
    o_done = state == DONE;

    rom.i_addr = cursor;
  }

  //----------------------------------------

  void tock() {

    if (state == WAIT && i_idle) {
      state = SEND;
    }
    else if (state == SEND && i_cts) {
      if (cursor == message_len) state = DONE;
      cursor = cursor + 1;
    }
    else if (state == DONE) {
      state = WAIT;
    cursor = 0;
    }

    rom.tock();
  }

  //----------------------------------------

  void dump_header() {
    printf("[hlo state cursor data req done] ");
    rom.dump_header();
  }

  void dump() {
    printf("[    %-5d %-6d %-04x %-3d %-4d] ", state, cursor, o_data, o_req, o_done);
    rom.dump();
  }

  void dump_vcd_header(VcdDump& d) {
    fprintf(d.file, "$scope module hello $end\n");
    fprintf(d.file, "$var wire 2 hello_state  state  $end\n");
    fprintf(d.file, "$var wire 9 hello_cursor cursor $end\n");
    fprintf(d.file, "$var wire 1 hello_o_req  o_req  $end\n");
    fprintf(d.file, "$var wire 8 hello_o_data o_data $end\n");
    fprintf(d.file, "$var wire 1 hello_o_done o_done $end\n");
    fprintf(d.file, "$upscope $end\n");
  }

  void dump_width(VcdDump& d) {
    d.set_width("hello_state",  2);
    d.set_width("hello_cursor", 9);
    d.set_width("hello_o_data", 8);
    d.set_width("hello_o_req",  1);
    d.set_width("hello_o_done", 1);
    rom.dump_width(d);
  }

  void dump_value(VcdDump& d) {
    d.set_value("hello_state",  state);
    d.set_value("hello_cursor", cursor);
    d.set_value("hello_o_data", o_data);
    d.set_value("hello_o_req",  o_req);
    d.set_value("hello_o_done", o_done);
    rom.dump_value(d);
  }
};

//==============================================================================

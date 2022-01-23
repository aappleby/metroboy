#pragma once
#include "metron.h"

//==============================================================================

struct uart_hello {
  logic<1> i_cts;
  logic<1> i_idle;
  
  logic<8> o_data;
  logic<1> o_req;
  logic<1> o_done;

  //----------------------------------------
  /*verilator public_module*/

  static const int message_len = 512;
  static const int cursor_bits = clog2(message_len);

  typedef enum { WAIT, SEND, DONE } e_state;
  logic<2> state;
  logic<cursor_bits> cursor;
  logic<8> memory[512];
  logic<8> data;

  //----------------------------------------

  void init() {
    readmemh("obj/message.hex", memory, 0, 511);
  }

  //----------------------------------------

  void final() {
  }

  //----------------------------------------

  void tick(bool rst_n) {
    o_data = data;
    o_req = state == SEND;
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
};

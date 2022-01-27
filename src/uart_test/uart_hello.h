#pragma once
#include "metron.h"

//==============================================================================

struct uart_hello {

  //----------------------------------------
  /*verilator public_module*/

  static const int message_len = 512;
  static const int cursor_bits = clog2(message_len);

  typedef enum { WAIT, SEND, DONE } e_state;
  logic<2> state;
  logic<cursor_bits> cursor;
  logic<8> memory[512];
  logic<8> data;

  logic<8> o_data;
  logic<1> o_req;
  logic<1> o_done;

  //----------------------------------------

  void init() {
    readmemh("obj/message.hex", memory, 0, 511);
  }

  //----------------------------------------

  void tick(bool rst_n, bool i_cts, bool i_idle) {
    if (!rst_n) {
      state = WAIT;
      cursor = 0;
    }
    else {
      data = memory[cursor];
      if (state == WAIT && i_idle) {
        state = SEND;
      }
      else if (state == SEND && i_cts) {
        if (cursor == (message_len - 1)) state = DONE;
        cursor = cursor + 1;
      }
      else if (state == DONE) {
        state = WAIT;
        cursor = 0;
      }
    }
  }

  //----------------------------------------

  void tock(bool rst_n) {
    o_data = data;
    o_req = state == SEND;
    o_done = state == DONE;
  }

  //----------------------------------------
};

//==============================================================================

#pragma once
#include "metron.h"

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
};

//==============================================================================

#pragma once
#include "../metron_tools.h"

//==============================================================================
/* verilator lint_off WIDTH */

struct uart_hello {

  static const int message_len = 512;
  static const int cursor_bits = clog2(message_len);

  enum class state : logic<2>::BASE {
    WAIT, SEND, DONE
  };

  state s;
  logic<cursor_bits> cursor;
  logic<8> memory[512];
  logic<8> data;

  logic<8> o_data;
  logic<1> o_req;
  logic<1> o_done;

  //----------------------------------------

  void init() {
    readmemh("message.hex", memory, 0, 511);
    o_data = 0;
    o_req = 0;
    o_done = 0;
  }

  //----------------------------------------

  void tick(bool rst_n, bool i_cts, bool i_idle) {
    if (!rst_n) {
      s = state::WAIT;
      cursor = 0;
    }
    else {
      data = memory[cursor];
      if (s == state::WAIT && i_idle) {
        s = state::SEND;
      }
      else if (s == state::SEND && i_cts) {
        if (cursor == b9(message_len - 1)) {
          s = state::DONE;
        }
        else {
          cursor = cursor + 1;
        }
      }
      else if (s == state::DONE) {
        //s = state::WAIT;
        cursor = 0;
      }
    }
  }

  //----------------------------------------

  void tock(bool rst_n) {
    o_data = data;
    o_req  = (s == state::SEND);
    o_done = (s == state::DONE);
  }

};

//==============================================================================

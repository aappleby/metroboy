#pragma once
#include "metron.h"


//==============================================================================

struct uart_hello {

  static const int message_len = 512;
  static const int cursor_bits = clog2(message_len);

  enum class state : typename logic<2>::basetype {
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
    readmemh("obj/message.hex", memory, 0, 511);
    o_data = b8(0);
    o_req = b1(0);
    o_done = b1(0);
  }

  //----------------------------------------

  void tick(bool rst_n, bool i_cts, bool i_idle) {
    if (!rst_n) {
      s = state::WAIT;
      cursor = bx<cursor_bits>(0);
    }
    else {
      data = memory[cursor];
      if (s == state::WAIT && i_idle) {
        s = state::SEND;
      }
      else if (s == state::SEND && i_cts) {
        if (cursor == (message_len - 1)) s = state::DONE;
        cursor = bx<cursor_bits>(cursor + 1);
      }
      else if (s == state::DONE) {
        //state = WAIT;
        cursor = bx<cursor_bits>(0);
      }
    }
  }

  //----------------------------------------

  void tock(bool rst_n) {
    o_data = data;
    o_req = s == state::SEND;
    o_done = s == state::DONE;
  }

};

//==============================================================================

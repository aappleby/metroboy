#pragma once
#include "metron_tools.h"

//==============================================================================
/* verilator lint_off WIDTH */

class uart_hello {
 public:
  /*verilator public_module*/

  static const int message_len = 512;
  static const int cursor_bits = clog2(message_len);

  enum class state : logic<2>::BASE{WAIT, SEND, DONE};

  state s;
  logic<cursor_bits> cursor;
  logic<8> memory[512];
  logic<8> data;

  logic<8> o_data;
  logic<1> o_req;
  logic<1> o_done;

  //----------------------------------------

  void init() { readmemh("message.hex", memory, 0, 511); }

  //----------------------------------------

  void tick(logic<1> i_rstn, logic<1> i_cts, logic<1> i_idle) {
    if (!i_rstn) {
      s = state::WAIT;
      cursor = 0;
    } else {
      data = memory[cursor];
      if (s == state::WAIT && i_idle) {
        s = state::SEND;
      } else if (s == state::SEND && i_cts) {
        if (cursor == b9(message_len - 1)) {
          s = state::DONE;
        } else {
          cursor = cursor + 1;
        }
      } else if (s == state::DONE) {
        // s = state::WAIT;
        cursor = 0;
      }
    }
  }

  //----------------------------------------

  void tock() {
    o_data = data;
    o_req = (s == state::SEND);
    o_done = (s == state::DONE);
  }
};

//==============================================================================

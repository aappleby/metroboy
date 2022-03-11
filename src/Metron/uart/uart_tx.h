#pragma once
#include "metron_tools.h"

//==============================================================================

template <int cycles_per_bit = 4>
class uart_tx {
 public:
  /*verilator public_module*/

  // 1 start bit, 8 data bits, 1 stop bit, 7 additional stop bits to guarantee
  // that recevier can resync between messages
  static const int extra_stop_bits = 7;
  static const int cycle_bits = clog2(cycles_per_bit);
  static const int cursor_bits = clog2(10 + extra_stop_bits);

  logic<cycle_bits> cycle;
  logic<cursor_bits> cursor;
  logic<9> buffer;

  logic<1> o_serial;
  logic<1> o_cts;
  logic<1> o_idle;

  //----------------------------------------

  void tick(logic<1> i_rstn, logic<8> i_data, logic<1> i_req) {
    if (!i_rstn) {
      cycle = 0;
      cursor = 0;
      buffer = 0x1FF;
    } else {
      logic<cycle_bits> cycle_max = bx<cycle_bits>(cycles_per_bit - 1);
      logic<cursor_bits> cursor_max = bx<cursor_bits>(10 + extra_stop_bits - 1);

      if (cursor <= extra_stop_bits && cycle == 0 && i_req) {
        // Transmit start
        cycle = cycle_max;
        cursor = cursor_max;
        buffer = i_data << 1;
      } else if (cycle != 0) {
        // Bit delay
        cycle = cycle - 1;
        cursor = cursor;
        buffer = buffer;
      } else if (cursor != 0) {
        // Bit delay done, switch to next bit.
        cycle = cycle_max;
        cursor = cursor - 1;
        buffer = (buffer >> 1) | 0x100;
      }
    }
  }

  //----------------------------------------

  void tock() {
    o_serial = buffer & 1;
    o_cts = ((cursor == extra_stop_bits) && (cycle == 0)) ||
            (cursor < extra_stop_bits);
    o_idle = (cursor == 0) && (cycle == 0);
  }
};

//==============================================================================

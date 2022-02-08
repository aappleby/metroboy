#pragma once
#include "metron.h"

//==============================================================================

template<int cycles_per_bit = 4>
struct uart_tx {

  // 1 start bit, 8 data bits, 1 stop bit, 7 additional stop bits to guarantee
  // that recevier can resync between messages
  static const int extra_stop_bits = 7;

  static const int cycle_bits = clog2(cycles_per_bit);
  static const int cycle_max = cycles_per_bit - 1;

  static const int cursor_bits = clog2(10 + extra_stop_bits);
  static const int cursor_max = 10 + extra_stop_bits - 1;

  logic<cycle_bits> cycle;
  logic<cursor_bits> cursor;
  logic<9> buffer;

  logic<1> o_serial;
  logic<1> o_cts;
  logic<1> o_idle;

  //----------------------------------------

  void init() {
    o_serial = b1(0);
    o_cts = b1(0);
    o_idle = b1(0);
  }

  //----------------------------------------

  void tick(bool rst_n, logic<8> i_data, logic<1> i_req) {
    if (!rst_n) {
      cycle = bx<cycle_bits>(0);
      cursor = bx<cursor_bits>(0);
      buffer = b9(0x1FF);
    } else {
      if (cursor <= extra_stop_bits && cycle == 0 && i_req) {
        // Transmit start
        cycle = bx<cycle_bits>(cycle_max);
        cursor = bx<cursor_bits>(cursor_max);
        buffer = b9(i_data << 1);
      } else if (cycle != 0) {
        // Bit delay
        cycle = bx<cycle_bits>(cycle - 1);
        cursor = bx<cursor_bits>(cursor);
        buffer = b9(buffer);
      } else if (cursor != 0) {
        // Bit delay done, switch to next bit.
        cycle = bx<cycle_bits>(cycle_max);
        cursor = bx<cursor_bits>(cursor - 1);
        buffer = b9((buffer >> 1) | 0x100);
      }
    }
  }

  //----------------------------------------

  void tock(bool rst_n) {
    o_serial = b1(buffer & 1);
    o_cts = b1(((cursor == extra_stop_bits) && (cycle == 0)) || (cursor < extra_stop_bits));
    o_idle = b1((cursor == 0) && (cycle == 0));
  }

};

//==============================================================================

#pragma once
#include "metron.h"

//==============================================================================

template<int cycles_per_bit = 4>
struct uart_rx {

  static const int cycle_bits = clog2(cycles_per_bit);
  static const int cycle_max = cycles_per_bit - 1;
  static const int cursor_max = 9;
  static const int cursor_bits = clog2(cursor_max);

  logic<cycle_bits> cycle;
  logic<cursor_bits> cursor;
  logic<8> buffer;
  logic<32> sum;
  logic<8> temp;

  logic<8> o_data;
  logic<1> o_valid;
  logic<32> o_sum;

  //----------------------------------------

  void init() {
    o_data = b8(0);
    o_valid = b1(0);
    o_sum = b32(0);
  }

  //----------------------------------------

  void tick(bool rst_n, logic<1> i_serial) {
    if (!rst_n) {
      cycle = bx<cycle_bits>(0);
      cursor = bx<cursor_bits>(0);
      buffer = b8(0);
      sum = b32(0);
      temp = b8(0);
    } else {
      if (cycle != 0) {
        cycle = bx<cycle_bits>(cycle - 1);
      } else if (cursor != 0) {
        logic<8> temp = b8((i_serial << 7) | (buffer >> 1));
        if (cursor - 1 == 1) sum = b32(sum + temp);
        cycle = bx<cycle_bits>(cycle_max);
        cursor = bx<cursor_bits>(cursor - 1);
        buffer = b8(temp);
      }
      else if (i_serial == 0) {
        cycle = bx<cycle_bits>(cycle_max);
        cursor = bx<cursor_bits>(cursor_max);
      }

    }
  }

  //----------------------------------------

  void tock(bool rst_n) {
    o_data = buffer;
    o_valid = cursor == 1;
    o_sum = sum;
  }

};

//==============================================================================
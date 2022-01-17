#pragma once
#include "metron.h"

//==============================================================================

template<int cycles_per_bit = 4>
struct uart_rx {
  logic<1> i_serial;
  logic<8> o_data;
  logic<1> o_valid;
  logic<32> o_sum;

  //----------------------------------------
  /*verilator public_module*/

  static const int cycle_bits = clog2(cycles_per_bit);
  static const int cycle_max = cycles_per_bit - 1;
  static const int cursor_max = 9;
  static const int cursor_bits = clog2(cursor_max);

  logic<cycle_bits> cycle;
  logic<cursor_bits> cursor;
  logic<8> buffer;
  logic<32> sum;

  //----------------------------------------

  void tick(bool rst_n) {
    o_data = buffer;
    o_valid = cursor == 1;
    o_sum = sum;
  }

  //----------------------------------------

  void tock(bool rst_n) {
    if (!rst_n) {
      cycle = 0;
      cursor = 0;
      buffer = 0;
      sum = 0;
    } else {
      if (cycle != 0) {
        cycle = cycle - 1;
      } else if (cursor != 0) {
        logic<8> temp;

        temp = (i_serial << 7) | (buffer >> 1);
        if (cursor - 1 == 1) sum = sum + temp;

        cycle = cycle_max;
        cursor = cursor - 1;
        buffer = temp;
      }
      else if (i_serial == 0) {
        cycle = cycle_max;
        cursor = cursor_max;
      }
    }
  }

};

//==============================================================================
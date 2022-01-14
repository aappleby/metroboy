#pragma once
#include "metron.h"

//==============================================================================

template<int cycles_per_bit = 4>
struct uart_rx {




  //----------------------------------------

  bool     i_serial;

  uint8_t  o_data;
  bool     o_valid;
  uint32_t o_sum;

  //----------------------------------------
  /*verilator public_module*/

  static const int cycle_bits  = clog2(cycles_per_bit);
  static const int cycle_max   = cycles_per_bit - 1;
  static const int cursor_max  = 9;
  static const int cursor_bits = clog2(cursor_max);

  int cycle;
  int cursor;
  uint8_t buffer;
  uint32_t sum;

  //----------------------------------------

  void tick(bool rst_n) {
    o_data  = buffer;
    o_valid = cursor == 1;
    o_sum   = sum;
  }

  //----------------------------------------

  void tock(bool rst_n) {
    if (!rst_n) {
      cycle  = 0;
      cursor = 0;
      buffer = 0;
      sum    = 0;
    } else {
      if (cycle != 0) {
        cycle = cycle - 1;
      } else if (cursor != 0) {
        uint8_t temp;

        temp = (i_serial << 7) | (buffer >> 1);
        if (cursor - 1 == 1) sum = sum + temp;

        cycle  = cycle_max;
        cursor = cursor - 1;
        buffer = temp;
      }
      else if (i_serial == 0) {
        cycle  = cycle_max;
        cursor = cursor_max;
      }
    }
  }
};

//==============================================================================

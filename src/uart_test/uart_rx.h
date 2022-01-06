#pragma once
#include "metron.h"

//==============================================================================

template<int clocks_per_bit = 4>
struct uart_rx : public Module {

  //----------------------------------------

  bool     i_serial;

  uint8_t  o_data;
  bool     o_valid;
  uint32_t o_sum;

  //----------------------------------------

  static constexpr int timer_bits = clog2(clocks_per_bit);
  int cycle;
  int bit;
  uint8_t data;
  uint32_t sum;

  //----------------------------------------

  void reset() {
    cycle = 0;
    bit = 0;
    data = 0;
    sum = 0;
  }

  //----------------------------------------

  void tick() {
    o_data  = data;
    o_valid = bit == 1;
    o_sum   = sum;
  }

  //----------------------------------------

  void tock() {
    if (cycle != 0) {
      cycle = cycle - 1;
    }
    else if (bit != 0) {
      cycle = (clocks_per_bit - 1);
      bit = bit - 1;
      data = (i_serial << 7) | (data >> 1);
      if (bit == 1) sum += data;
    }
    else if (i_serial == 0) {
      cycle = (clocks_per_bit - 1);
      bit = 9;
    }
  }
};

//==============================================================================

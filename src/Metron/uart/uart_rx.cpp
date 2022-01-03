#include "metron.h"

//==============================================================================

template<int clocks_per_bit = 4>
struct uart_rx : public Module {

  static constexpr int timer_bits = clog2(clocks_per_bit);
  int rx_cycle;
  int rx_bit;
  uint8_t rx_data;
  uint32_t rx_sum;

  //----------------------------------------

  void reset() {
    rx_cycle = 0;
    rx_bit = 0;
    rx_data = 0;
    rx_sum = 0;
  }

  //----------------------------------------

  void tick(uint8_t& o_data, bool& o_valid, uint32_t& o_sum) {
    o_data  = rx_data;
    o_valid = rx_bit == 1;
    o_sum   = rx_sum;
  }

  //----------------------------------------

  void tock(bool i_sio) {
    if (rx_cycle != 0) {
      rx_cycle = rx_cycle - 1;
    }
    else if (rx_bit != 0) {
      rx_cycle = (clocks_per_bit - 1);
      rx_bit = rx_bit - 1;
      rx_data = (i_sio << 7) | (rx_data >> 1);
      if (rx_bit == 1) rx_sum = rx_sum + rx_data;
    }
    else if (i_sio == 0) {
      rx_cycle = (clocks_per_bit - 1);
      rx_bit = 9;
    }
  }
};

//==============================================================================

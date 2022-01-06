#include "metron.h"

//==============================================================================

template<int clocks_per_bit = 4>
struct uart_tx : public Module {

  //----------------------------------------

  uint8_t i_data;
  bool    i_req;

  bool    o_serial;
  bool    o_cts;
  bool    o_idle;

  //----------------------------------------

  static constexpr int timer_bits = clog2(clocks_per_bit);

  // 1 start bit, 8 data bits, 1 stop bit, 7 additional stop bits to guarantee
  // that recevier can resync between messages
  static constexpr int extra_stop_bits = 7;

  int tx_cycle;
  int tx_bit;
  uint16_t tx_buf;

  //----------------------------------------

  void reset() {
    tx_cycle = 0;
    tx_bit   = 0;
    tx_buf   = 0x1FF;
  }

  //----------------------------------------

  void tick() {
    o_serial  = tx_buf & 1;
    o_cts     = ((tx_bit <= extra_stop_bits) && (tx_cycle <= 1)) || (tx_bit < extra_stop_bits);
    o_idle    = (tx_bit == 0) && (tx_cycle <= 1);
  }

  //----------------------------------------

  void tock() {
    if (tx_bit <= extra_stop_bits && tx_cycle <= 1 && i_req) {
      // Transmit start
      tx_cycle = clocks_per_bit - 1;
      tx_bit   = 10 + extra_stop_bits - 1;
      tx_buf   = (i_data << 1) | 0;
    } else if (tx_cycle != 0) {
      // Bit delay
      tx_cycle = tx_cycle - 1;
      tx_bit   = tx_bit;
      tx_buf   = tx_buf;
    } else if (tx_bit != 0) {
      // Bit delay done, switch to next bit.
      tx_cycle = clocks_per_bit - 1;
      tx_bit   = tx_bit - 1;
      tx_buf   = (tx_buf >> 1) | 0x100;
    }
  }
};

//==============================================================================

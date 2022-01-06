#include "metron.h"

//==============================================================================

template<int clocks_per_bit = 4>
struct uart_tx : public Module {

  static constexpr int timer_bits = clog2(clocks_per_bit);

  // 1 start bit, 8 data bits, 1 stop bit, 7 additional stop bits to guarantee
  // that recevier can resync between messages
  static constexpr int extra_stop_bits = 7;

  int tx_cycle_;
  int tx_bit_;
  uint16_t tx_buf_;

  //----------------------------------------

  void reset() {
    tx_cycle_ = 0;
    tx_bit_   = 0;
    tx_buf_   = 0x1FF;
  }

  //----------------------------------------

  void tick(bool& SER_TX, bool& tx_cts, bool& tx_idle) {
    SER_TX  = tx_buf_ & 1;
    tx_cts  = ((tx_bit_ <= extra_stop_bits) && (tx_cycle_ <= 1)) || (tx_bit_ < extra_stop_bits);
    tx_idle = (tx_bit_ == 0) && (tx_cycle_ <= 1);
  }

  //----------------------------------------

  void tock(uint8_t tx_data, bool tx_req) {
    int _tx_cycle = tx_cycle_;
    int _tx_bit = tx_bit_;
    uint16_t _tx_buf = tx_buf_;

    if (tx_bit_ <= extra_stop_bits && tx_cycle_ <= 1 && tx_req) {
      // Transmit start
      _tx_cycle = clocks_per_bit - 1;
      _tx_bit   = 10 + extra_stop_bits - 1;
      _tx_buf   = (tx_data << 1) | 0;
    } else if (tx_cycle_ != 0) {
      // Bit delay
      _tx_cycle = tx_cycle_ - 1;
      _tx_bit   = tx_bit_;
      _tx_buf   = tx_buf_;
    } else if (tx_bit_ != 0) {
      // Bit delay done, switch to next bit.
      _tx_cycle = clocks_per_bit - 1;
      _tx_bit   = tx_bit_ - 1;
      _tx_buf   = (tx_buf_ >> 1) | 0x100;
    }

    tx_cycle_ = _tx_cycle;
    tx_bit_   = _tx_bit;
    tx_buf_   = _tx_buf;
  }

  //----------------------------------------

  void dump_title() {
    printf("tx: [bit  cyc   buf tx cts idle] ");
  }

  void dump() {
    bool SER_TX = 0;
    bool tx_cts = 0;
    bool tx_idle = 0;
    tick(SER_TX, tx_cts, tx_idle);
    printf("    |%3d  %3d  %04x %2d %3d %4d| ", tx_bit_, tx_cycle_, tx_buf_, SER_TX, tx_cts, tx_idle);
  }
};

//==============================================================================

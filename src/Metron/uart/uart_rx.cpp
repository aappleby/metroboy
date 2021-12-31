#include "metron.h"

//==============================================================================

template<int clocks_per_bit = 4>
struct uart_rx : public Module {

  static constexpr int timer_bits = clog2(clocks_per_bit);
  int rx_cycle_;
  int rx_bit_;
  uint8_t out_data_;

  //----------------------------------------

  void reset() {
    rx_cycle_ = 0;
    rx_bit_ = 0;
    out_data_ = 0;
  }

  //----------------------------------------

  void tick(uint8_t& out_data, bool& out_valid) {
    out_data = out_data_;
    out_valid = rx_bit_ == 1;
  }

  //----------------------------------------

  void tock(bool SER_TX_) {
    if (rx_cycle_ != 0) {
      rx_cycle_ = rx_cycle_ - 1;
    } else if (rx_bit_ != 0) {
      rx_cycle_ = (clocks_per_bit - 1);
      rx_bit_ = rx_bit_ - 1;
      out_data_ = (SER_TX_ << 7) | (out_data_ >> 1);
    } else if (SER_TX_ == 0) {
      rx_cycle_ = (clocks_per_bit - 1);
      rx_bit_ = 9;
    }
  }

  //----------------------------------------

  void dump_title() {
    printf("rx: [bit cyc data valid] ");
  }

  void dump() {
    uint8_t out_data = 0;
    bool out_valid = 0;
    tick(out_data, out_valid);

    printf("    [%3d %3d %4x %5d] ", rx_bit_, rx_cycle_, out_data, out_valid);
  }
};

//==============================================================================

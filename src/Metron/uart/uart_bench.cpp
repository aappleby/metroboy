#include "metron.h"

#include "uart_top.cpp"

//==============================================================================

struct uart_bench : public Module {

  void initial() {
    dut.initial();
  }

  void reset() {
    dut.reset();
    rst_counter = 15;
    ser_tx = 0;
    leds = 0;
  }

  void tick() {
    dut.tick(ser_tx, leds);
  }

  void tock() {
    if (rst_counter) {
      dut.reset();
      rst_counter = rst_counter - 1;
    }
    else {
      dut.tock(ser_tx, leds);
    }
  }

  uart_top<3> dut;

  uint8_t rst_counter;
  bool ser_tx;
  uint8_t leds;
};

//==============================================================================

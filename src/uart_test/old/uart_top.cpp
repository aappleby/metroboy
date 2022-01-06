#include "metron.h"

#include "uart_rx.cpp"
#include "uart_tx.cpp"
#include "uart_hello.cpp"

//==============================================================================


template<int clocks_per_bit = 3>
struct uart_top : public Module {

  uint8_t  tx_data;
  bool     tx_req;
  bool     tx_cts;
  bool     tx_idle;
  uint8_t  out_data;
  bool     out_valid;

  uart_hello hello;
  uart_tx<clocks_per_bit> tx;
  uart_rx<clocks_per_bit> rx;

  void initial() {
    hello.initial();
    tx.initial();
    rx.initial();
  }

  void reset() {
    tx_data = 0;
    tx_req = 0;
    tx_cts = 0;
    tx_idle = 0;
    out_data = 0;
    out_valid = 0;

    hello.reset();
    tx.reset();
    rx.reset();
  }

  void tick(bool& SER_TX, uint8_t& LEDS) {
    hello.tick(tx_data, tx_req);
    rx.tick(out_data, out_valid);
    tx.tick(SER_TX, tx_cts, tx_idle);
    LEDS = out_valid ? out_data : 0;
  }

  void tock(bool SER_TX) {
    hello.tock(tx_cts, tx_idle);
    tx.tock(tx_data, tx_req);
    rx.tock(SER_TX);
  }

  void dump_title() {
    hello.dump_title();
    tx.dump_title();
    rx.dump_title();
  }

  void dump() {
    hello.dump();
    tx.dump();
    rx.dump();

    //printf("[uart_top tx_data=0x%02x tx_req=%d tx_cts=%d tx_idle=%d, out_data=0x%02x out_valid=%d] ", tx_data, tx_req, tx_cts, tx_idle, out_data, out_valid);
    //hello.dump();
    //tx.dump();
    //rx.dump();
    //printf("\n");
  }
};


//==============================================================================

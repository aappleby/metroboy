#include "metron.h"

#include "message2.blob.h"
#include "blockram_512x8.cpp"

//==============================================================================

struct uart_hello : public Module {

  //----------------------------------------

  blockram_512x8 message;

  enum { WAIT_IDLE, WAIT_ACK, WAIT_CTS } state;
  int  cursor;
  bool done;
  bool req;

  //----------------------------------------

  void initial() {
    message.initial(message2, sizeof(message2));
  }

  //----------------------------------------

  void reset() {
    cursor = message2_len;
    done = 0;
    req = 0;
    state = WAIT_IDLE;
  }

  //----------------------------------------

  void tick(uint8_t& o_data, bool& o_req, bool& o_done) {
    message.tick(o_data);
    o_req  = req;
    o_done = done;
  }

  //----------------------------------------

  void tock(bool i_cts, bool i_idle) {
    message.tock(cursor);
    switch(state) {
      case WAIT_IDLE: if (i_idle) on_tx_idle(); break;
      case WAIT_ACK:  if (!i_cts) on_tx_ack(); break;
      case WAIT_CTS:  if (i_cts)  on_tx_cts(); break;
    }
  }

  void on_tx_idle() {
    state = WAIT_ACK;
    cursor = 0;
    done = 0;
    req = 1;
  }

  void on_tx_ack() {
    if (cursor == message2_len - 1) {
      state = WAIT_IDLE;
      cursor = 0;
      done = 1;
      req = 0;
    } else {
      state = WAIT_CTS;
      cursor = cursor + 1;
      done = 0;
      req = 0;
    }
  }

  void on_tx_cts() {
    state = WAIT_ACK;
    cursor = cursor;
    done = 0;
    req = 1;
  }
};

//==============================================================================

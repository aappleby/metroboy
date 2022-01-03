#include "metron.h"

#include "message2.blob.h"
#include "blockram_512x8.cpp"

//==============================================================================

struct uart_hello : public Module {

  //----------------------------------------

  blockram_512x8 message;

  enum { WAIT_IDLE, WAIT_ACK, WAIT_CTS } state;
  int  message_cursor;
  bool message_done;
  bool tx_req;

  //----------------------------------------

  void initial() {
    message.initial(message2, sizeof(message2));
  }

  //----------------------------------------

  void reset() {
    message_cursor = message2_len;
    message_done = 0;
    tx_req = 0;
    state = WAIT_IDLE;
  }

  //----------------------------------------

  void tick(uint8_t& out_tx_data, bool& out_tx_req) {
    message.tick(out_tx_data);
    out_tx_req  = tx_req;
  }

  //----------------------------------------

  void tock(bool tx_cts, bool tx_idle) {
    message.tock(message_cursor, 0, 0, 0);
    switch(state) {
      case WAIT_IDLE: if (tx_idle) on_tx_idle(); break;
      case WAIT_ACK:  if (!tx_cts) on_tx_ack(); break;
      case WAIT_CTS:  if (tx_cts)  on_tx_cts(); break;
    }
  }

  void on_tx_idle() {
    state = WAIT_ACK;
    message_cursor = 0;
    message_done = 0;
    tx_req = 1;
  }

  void on_tx_ack() {
    if (message_cursor == message2_len - 1) {
      state = WAIT_IDLE;
      message_cursor = 0;
      message_done = 1;
      tx_req = 0;
    } else {
      state = WAIT_CTS;
      message_cursor = message_cursor + 1;
      message_done = 0;
      tx_req = 0;
    }
  }

  void on_tx_cts() {
    state = WAIT_ACK;
    message_cursor = message_cursor;
    message_done = 0;
    tx_req = 1;
  }

  //----------------------------------------

  void dump_title() {
    printf("hello: [cursor done data req] ");
  }

  void dump() {
    uint8_t tx_data;
    bool tx_req;
    tick(tx_data, tx_req);
    printf("       [%6d %4d   %02x %3d] ", message_cursor, message_done, tx_data, tx_req);
  }
};

//==============================================================================

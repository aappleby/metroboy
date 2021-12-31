#include "metron.h"

//==============================================================================

struct uart_hello : public Module {

  static const int message_len = 14;
  uint8_t message[message_len];

  bool    tx_req;
  uint8_t message_cursor;
  bool    message_done;

  //----------------------------------------

  void initial() {
    uint8_t _message[] = {0x48, 0x65, 0x6c, 0x6c, 0x6f, 0x20, 0x57, 0x6f, 0x72, 0x6c, 0x64, 0x21, 0x21, 0x0A};
    for (int i = 0; i < message_len; i++) message[i] = _message[i];
  }

  //----------------------------------------

  void reset() {
    tx_req = 0;
    message_cursor = message_len;
    message_done = 0;
  }

  //----------------------------------------

  void tick(uint8_t& tx_data_, bool& tx_req_) {
    tx_data_ = message_cursor < message_len ? message[message_cursor] : 0xFF;
    tx_req_ = tx_req;
  }

  //----------------------------------------

  void tock(bool tx_cts, bool tx_idle) {
    auto tx_req_ = tx_req;
    auto message_cursor_ = message_cursor;
    auto message_done_ = message_done;

    auto _message_cursor = message_cursor;
    auto _message_done = message_done;
    auto _tx_req = tx_req;

    if (tx_idle && !message_done_) {
      _message_cursor = 0;
      _tx_req = 1;
    } else if (tx_cts && !tx_req_ && message_cursor_ < message_len) {
      _message_cursor = message_cursor_ + 1;
      _tx_req = message_cursor_ < message_len - 1;
    }
    if (tx_req_ && !tx_cts) _tx_req = 0;

    message_cursor = _message_cursor;
    message_done = _message_done;
    tx_req = _tx_req;
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

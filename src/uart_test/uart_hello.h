#pragma once
#include "metron.h"
#include "blockram_512x8.h"

//==============================================================================

#include "message.blob.h"

struct uart_hello : public Module {

  //----------------------------------------

  bool    i_cts;
  bool    i_idle;
  
  uint8_t o_data;
  bool    o_req;
  bool    o_done;

  //----------------------------------------

  static const int message_len = 512;

  blockram_512x8 rom;

  enum { WAIT, SEND, DONE };
  
  int  state;
  int  cursor;

  //----------------------------------------

  void initial() {
#include "message.blob.h"
    rom.initial(message, message_len);
  }

  //----------------------------------------

  void reset() {
    state = WAIT;
    cursor = 0;
  }

  //----------------------------------------

  void tick() {
    rom.tick();

    o_data = rom.o_data;
    o_req  = state == SEND;
    o_done = state == DONE;

    rom.i_addr = cursor;
  }

  //----------------------------------------

  void tock() {

    if (state == WAIT && i_idle) {
      state = SEND;
    }
    else if (state == SEND && i_cts) {
      if (cursor == message_len) state = DONE;
      cursor = cursor + 1;
    }
    else if (state == DONE) {
      state = WAIT;
    cursor = 0;
    }

    rom.tock();
  }

  // always_ff @(posedge clk, negedge rst_n) if (!rst_n) reset(); else tock();
};

//==============================================================================

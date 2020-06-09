#include "Serial.h"
#include "Constants.h"
#include <assert.h>

//-----------------------------------------------------------------------------

void Serial::reset() {
  sb = 0x00; // FF01
  sc = 0x7E; // FF02
}

//-----------------------------------------------------------------------------

void Serial::tock_req(const Req& req) {
  if ((req.addr != ADDR_SB) && (req.addr != ADDR_SC)) {
    ack = {0};
  }
  else if (req.write) {
    ack.addr  = req.addr;
    ack.data  = req.data;
    ack.read  = 0;
    if (req.addr == ADDR_SB) sb = (uint8_t)req.data;
    if (req.addr == ADDR_SC) sc = (uint8_t)req.data | 0b01111110;
  }
  else if (req.read) {
    ack.addr  = req.addr;
    ack.data  = 0;
    ack.read  = 1;
    if (req.addr == ADDR_SB) ack.data = sb;
    if (req.addr == ADDR_SC) ack.data = sc;
  }
}

void Serial::tick_ack(Ack& ack_) const {
  ack_.addr  += ack.addr;
  ack_.data  += ack.data;
  ack_.read  += ack.read;
}

//-----------------------------------------------------------------------------

void Serial::dump(std::string& d) {
  sprintf(d, "\002--------------SERIAL-----------\001\n");
  sprintf(d, "SB 0x%02x\n", sb);
  sprintf(d, "SC 0x%02x\n", sc);
}

//-----------------------------------------------------------------------------

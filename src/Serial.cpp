#include "Serial.h"
#include "Constants.h"
#include <assert.h>

//-----------------------------------------------------------------------------

void Serial::reset() {
  *this = {};
  sb = 0x00;
  sc = 0x7E;
}

//-----------------------------------------------------------------------------

void Serial::ibus_req(Req ibus_req) {
  if ((ibus_req.addr != ADDR_SB) && (ibus_req.addr != ADDR_SC)) {
    ack = {0};
  }
  else if (ibus_req.write) {
    ack.addr  = ibus_req.addr;
    ack.data  = ibus_req.data;
    ack.read  = 0;
    ack.write = 1;
    if (ibus_req.addr == ADDR_SB) sb = (uint8_t)ibus_req.data;
    if (ibus_req.addr == ADDR_SC) sc = (uint8_t)ibus_req.data | 0b01111110;
  }
  else if (ibus_req.read) {
    ack.addr  = ibus_req.addr;
    ack.data  = 0;
    ack.read  = 1;
    ack.write = 0;
    if (ibus_req.addr == ADDR_SB) ack.data = sb;
    if (ibus_req.addr == ADDR_SC) ack.data = sc;
  }
}

void Serial::ibus_ack(Ack& ibus_ack) const {
  ibus_ack.addr  += ack.addr;
  ibus_ack.data  += ack.data;
  ibus_ack.read  += ack.read;
  ibus_ack.write += ack.write;
}

//-----------------------------------------------------------------------------

void Serial::dump(std::string& d) {
  sprintf(d, "\002--------------SERIAL-----------\001\n");
  sprintf(d, "SB 0x%02x\n", sb);
  sprintf(d, "SC 0x%02x\n", sc);
}

//-----------------------------------------------------------------------------

#include "Serial.h"

#include "Constants.h"

//-----------------------------------------------------------------------------

void Serial::reset() {
  *this = {};
  sb = 0x00;
  sc = 0x7E;
}

//-----------------------------------------------------------------------------

Ack Serial::on_ibus_req(Req ibus_req) {
  if ((ibus_req.addr != ADDR_SB) && (ibus_req.addr != ADDR_SC)) return {};

  Ack ack;
  ack.addr  = ibus_req.addr;
  ack.data  = ibus_req.data;
  ack.read  = ibus_req.read;
  ack.write = ibus_req.write;

  if (ibus_req.read) {
    if (ibus_req.addr == ADDR_SB) ack.data = sb;
    if (ibus_req.addr == ADDR_SC) ack.data = sc;
  }

  if (ibus_req.write) {
    if (ibus_req.addr == ADDR_SB) sb = (uint8_t)ibus_req.data;
    if (ibus_req.addr == ADDR_SC) sc = (uint8_t)ibus_req.data | 0b01111110;
  }

  return ack;
}

//-----------------------------------------------------------------------------

void Serial::dump(std::string& d) {
  sprintf(d, "\002--------------SERIAL-----------\001\n");
  sprintf(d, "SB 0x%02x\n", sb);
  sprintf(d, "SC 0x%02x\n", sc);
}

//-----------------------------------------------------------------------------

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

bool Serial::on_ibus_req(Req ibus_req, Ack& ibus_ack) {
  if ((ibus_req.addr != ADDR_SB) && (ibus_req.addr != ADDR_SC)) return false;

  assert(!ibus_ack.read && !ibus_ack.write);

  ibus_ack.addr  = ibus_req.addr;
  ibus_ack.data  = ibus_req.data;
  ibus_ack.read  = ibus_req.read;
  ibus_ack.write = ibus_req.write;

  if (ibus_req.read) {
    if (ibus_req.addr == ADDR_SB) ibus_ack.data = sb;
    if (ibus_req.addr == ADDR_SC) ibus_ack.data = sc;
  }

  if (ibus_req.write) {
    if (ibus_req.addr == ADDR_SB) sb = (uint8_t)ibus_req.data;
    if (ibus_req.addr == ADDR_SC) sc = (uint8_t)ibus_req.data | 0b01111110;
  }

  return true;
}

//-----------------------------------------------------------------------------

void Serial::dump(std::string& d) {
  sprintf(d, "\002--------------SERIAL-----------\001\n");
  sprintf(d, "SB 0x%02x\n", sb);
  sprintf(d, "SC 0x%02x\n", sc);
}

//-----------------------------------------------------------------------------

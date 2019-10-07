#include "Serial.h"

#include "Constants.h"

//-----------------------------------------------------------------------------

void Serial::reset() {
  *this = {};
  sb = 0x00;
  sc = 0x7E;
}

//-----------------------------------------------------------------------------

BusOut Serial::tick() const {
  return out;
}

void Serial::tock(CpuBus bus) {
  out = {};

  if (bus.write) {
    if (bus.addr == ADDR_SB) sb = bus.data;
    if (bus.addr == ADDR_SC) sc = bus.data | 0b01111110;
  }

  if (bus.read) {
    if (bus.addr == ADDR_SB) {
      out.data = sb;
      out.oe = true;
    }
    if (bus.addr == ADDR_SC) {
      out.data = sc;
      out.oe = true;
    }
  }
}

//-----------------------------------------------------------------------------

void Serial::dump(std::string& d) {
  dumpit(out.addr, "0x%04x");
  dumpit(out.data, "0x%02x");
  dumpit(out.oe,   "%d");
}

//-----------------------------------------------------------------------------

#include "Serial.h"

#include "Constants.h"

//-----------------------------------------------------------------------------

void Serial::reset() {
  *this = {};
  sb = 0x00;
  sc = 0x7E;
}

//-----------------------------------------------------------------------------

Serial::Out Serial::tick() const {
  return out;
}

void Serial::tock(int tphase, CpuBus bus) {
  if (tphase == 0 && bus.read) {
    out = {};
    if (bus.addr == ADDR_SB) {
      out.addr = bus.addr;
      out.data = sb;
      out.oe = true;
    }
    if (bus.addr == ADDR_SC) {
      out.addr = bus.addr;
      out.data = sc;
      out.oe = true;
    }
  }

  if (tphase == 2 && bus.write) {
    if (bus.addr == ADDR_SB) sb = bus.data;
    if (bus.addr == ADDR_SC) sc = bus.data | 0b01111110;
  }
}

//-----------------------------------------------------------------------------

void Serial::dump(std::string& d) {
  dumpit(out.addr, "0x%04x");
  dumpit(out.data, "0x%02x");
  dumpit(out.oe,   "%d");
}

//-----------------------------------------------------------------------------

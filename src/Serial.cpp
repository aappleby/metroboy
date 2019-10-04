#include "Serial.h"

#include "Constants.h"

//-----------------------------------------------------------------------------

BusOut Serial::reset() {
  sb = 0x00;
  sc = 0x7E;
  return { 0 };
}

//-----------------------------------------------------------------------------

BusOut Serial::tick() const {
  return out;
}

void Serial::tock(CpuBus bus) {
  out = { 0,0 };

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

#include "Serial.h"

#include "Constants.h"

//-----------------------------------------------------------------------------

void Serial::reset() {
  sb = 0x00;
  sc = 0x7E;
}

//-----------------------------------------------------------------------------

BusOut Serial::tock(CpuBus bus) {
  BusOut out = { 0,0 };

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

  return out;
}

//-----------------------------------------------------------------------------

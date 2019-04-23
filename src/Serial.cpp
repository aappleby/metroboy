#include "Platform.h"
#include "Serial.h"

#include "Constants.h"

//-----------------------------------------------------------------------------

void Serial::reset() {
  bus_out = 0x00;
  bus_oe = false;

  sb = 0x00;
  sc = 0x7E;
}

//-----------------------------------------------------------------------------

void Serial::tock(uint16_t addr, uint8_t data, bool read, bool write) {
  bus_out = 0x00;
  bus_oe = false;

  if (write) {
    if (addr == ADDR_SB) sb = data;
    if (addr == ADDR_SC) sc = data | 0b01111110;
  }

  if (read) {
    if (addr == ADDR_SB) {
      bus_out = sb;
      bus_oe = true;
    }
    if (addr == ADDR_SC) {
      bus_out = sc;
      bus_oe = true;
    }
  }
}

//-----------------------------------------------------------------------------

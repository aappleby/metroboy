#include "ZRAM.h"

#include "Constants.h"

//-----------------------------------------------------------------------------

void ZRAM::reset() {
  *this = {};
}

//-----------------------------------------------------------------------------

BusOut ZRAM::tick() const {
  return out;
}

void ZRAM::tock(CpuBus bus) {
  out = {};

  if (bus.addr < ADDR_ZEROPAGE_BEGIN || ADDR_ZEROPAGE_END < bus.addr) {
    return;
  }

  if (bus.write) {
    ram[bus.addr - ADDR_ZEROPAGE_BEGIN] = bus.data;
  }

  if (bus.read) {
    out.data = ram[bus.addr - ADDR_ZEROPAGE_BEGIN];
    out.oe = true;
  }
}

//-----------------------------------------------------------------------------

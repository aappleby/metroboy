#include "ZRAM.h"

#include "Constants.h"

//-----------------------------------------------------------------------------

void ZRAM::reset() {
  memset(ram, 0, sizeof(ram));
}

//-----------------------------------------------------------------------------

BusOut ZRAM::tock(CpuBus bus) {
  BusOut out = { 0,0 };

  if (bus.addr < ADDR_ZEROPAGE_BEGIN || ADDR_ZEROPAGE_END < bus.addr) {
    return out;
  }

  if (bus.write) {
    ram[bus.addr - ADDR_ZEROPAGE_BEGIN] = bus.data;
  }

  if (bus.read) {
    out.data = ram[bus.addr - ADDR_ZEROPAGE_BEGIN];
    out.oe = true;
  }

  return out;
}

//-----------------------------------------------------------------------------

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

void ZRAM::dump(std::string& d) {
  dumpit(out.addr, "0x%04x");
  dumpit(out.data, "0x%02x");
  dumpit(out.oe,   "%d");
}

//-----------------------------------------------------------------------------

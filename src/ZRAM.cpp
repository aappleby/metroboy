#include "ZRAM.h"

#include "Constants.h"

//-----------------------------------------------------------------------------

void ZRAM::reset() {
  *this = {};
}

//-----------------------------------------------------------------------------

ZRAM::Out ZRAM::tick() const {
  return out;
}

void ZRAM::tock(int tphase, CpuBus bus) {
  if (tphase == 0 && bus.read) {
    out = {};
    if (ADDR_ZEROPAGE_BEGIN <= bus.addr && bus.addr <= ADDR_ZEROPAGE_END) {
      out.addr = bus.addr;
      out.data = ram[bus.addr - ADDR_ZEROPAGE_BEGIN];
      out.oe = true;
    }
  }

  if (tphase == 2 && bus.write) {
    if (ADDR_ZEROPAGE_BEGIN <= bus.addr && bus.addr <= ADDR_ZEROPAGE_END) {
      ram[bus.addr - ADDR_ZEROPAGE_BEGIN] = bus.data;
    }
  }
}

//-----------------------------------------------------------------------------

void ZRAM::dump(std::string& d) {
  dumpit(out.addr, "0x%04x");
  dumpit(out.data, "0x%02x");
  dumpit(out.oe,   "%d");
}

//-----------------------------------------------------------------------------

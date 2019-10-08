#include "IRAM.h"

#include "Constants.h"

//-----------------------------------------------------------------------------

void IRAM::reset() {
  *this = {};
}

//-----------------------------------------------------------------------------

IRAM::Out IRAM::tick() const {
  return out;
}

void IRAM::tock(int tphase, CpuBus bus) {
  if (tphase == 0 && bus.read) {
    out = {};
    if (ADDR_IRAM_BEGIN <= bus.addr && bus.addr <= ADDR_IRAM_END) {
      out.addr = bus.addr;
      out.data = ram[bus.addr - ADDR_IRAM_BEGIN];
      out.oe = true;
    }
    else if (ADDR_ECHO_BEGIN <= bus.addr && bus.addr <= ADDR_ECHO_END) {
      out.addr = bus.addr;
      out.data = ram[bus.addr - ADDR_ECHO_BEGIN];
      out.oe = true;
    }   
  }

  if (tphase == 2 && bus.write) {
    if (ADDR_IRAM_BEGIN <= bus.addr && bus.addr <= ADDR_IRAM_END) {
      ram[bus.addr - ADDR_IRAM_BEGIN] = bus.data;
    }
    else if (ADDR_ECHO_BEGIN <= bus.addr && bus.addr <= ADDR_ECHO_END) {
      ram[bus.addr - ADDR_ECHO_BEGIN] = bus.data;
    }
  }
}

//-----------------------------------------------------------------------------

void IRAM::dump(std::string& d) {
  dumpit(out.addr, "0x%04x");
  dumpit(out.data, "0x%02x");
  dumpit(out.oe,   "%d");
}

//-----------------------------------------------------------------------------

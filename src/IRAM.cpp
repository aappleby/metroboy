#include "IRAM.h"

#include "Constants.h"

//-----------------------------------------------------------------------------

void IRAM::reset() {
  *this = {};
}

//-----------------------------------------------------------------------------

BusOut IRAM::tick() const {
  return out;
}

void IRAM::tock(CpuBus bus) {
  if (bus.read) {
    out = {};
    if (ADDR_IRAM_BEGIN <= bus.addr && bus.addr <= ADDR_IRAM_END) {
      out.data = ram[bus.addr - ADDR_IRAM_BEGIN];
      out.oe = true;
    }
    else if (ADDR_ECHO_BEGIN <= bus.addr && bus.addr <= ADDR_ECHO_END) {
      out.data = ram[bus.addr - ADDR_ECHO_BEGIN];
      out.oe = true;
    }   
  }

  if (bus.write) {
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

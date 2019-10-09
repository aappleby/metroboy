#include "IRAM.h"

#include "Constants.h"

//-----------------------------------------------------------------------------

void IRAM::reset() {
  *this = {};
}

//-----------------------------------------------------------------------------

IRAM::Out IRAM::tick() const {
  return {
    addr,
    data,
    oe
  };
}

void IRAM::tock(int tphase, CpuBus bus) {
  if (tphase == 0 && bus.read) {
    addr = 0;
    data = 0;
    oe = 0;
    if (ADDR_IRAM_BEGIN <= bus.addr && bus.addr <= ADDR_IRAM_END) {
      addr = bus.addr;
      data = ram[bus.addr - ADDR_IRAM_BEGIN];
      oe = true;
    }
    else if (ADDR_ECHO_BEGIN <= bus.addr && bus.addr <= ADDR_ECHO_END) {
      addr = bus.addr;
      data = ram[bus.addr - ADDR_ECHO_BEGIN];
      oe = true;
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
  dumpit(addr, "0x%04x");
  dumpit(data, "0x%02x");
  dumpit(oe,   "%d");
}

//-----------------------------------------------------------------------------

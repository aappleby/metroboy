#include "OAM.h"

#include "Constants.h"

//-----------------------------------------------------------------------------

void OAM::reset() {
  *this = {};
}

OAM::Out OAM::tick() const {
  return out;
}

// note this has to dispatch read/write on any phase

void OAM::tock(int tphase, CpuBus bus) {
  if (bus.read) {
    out = {};
    if (ADDR_OAM_BEGIN <= bus.addr && bus.addr <= ADDR_OAM_END) {
      out.addr = bus.addr;
      out.data16 = ram[(bus.addr - ADDR_OAM_BEGIN) >> 1];
      out.data = uint8_t(bus.addr & 1 ? (out.data16 >> 8) : out.data16);
      out.oe = true;
    }
  }

  if (bus.write) {
    if (ADDR_OAM_BEGIN <= bus.addr && bus.addr <= ADDR_OAM_END) {
      uint16_t& d = ram[(bus.addr - ADDR_OAM_BEGIN) >> 1];
      if (bus.addr & 1) {
        d = (d & 0x00FF) | (bus.data << 8);
      }
      else {
        d = (d & 0xFF00) | (bus.data << 0);
      }
    }
  }
}

void OAM::dump(std::string& d) const {
  dumpit(out.addr,   "0x%04x");
  dumpit(out.data,   "0x%02x");
  dumpit(out.data16, "0x%04x");
  dumpit(out.oe,     "%d");

  /*
  for (int i = 0; i < 10; i++) {
    sprintf(out, "%2x ", ram[i]);
  }
  sprintf(out, "\n");
  */
}

//-----------------------------------------------------------------------------

#include "OAM.h"

#include "Constants.h"




//-----------------------------------------------------------------------------

void OAM::reset() {
  *this = {};
}

//-----------------------------------------------------------------------------

OAM::Out OAM::tick() const {
  return out;
}

void OAM::tock(int tphase, bool oam_lock, const CpuBus cpu_bus, CpuBus ppu_bus) {
  out = {};

  if (oam_lock) {
    if (ppu_bus.read) {
      out.ppu_addr = ppu_bus.addr;
      out.ppu_data16 = ram[(ppu_bus.addr - ADDR_OAM_BEGIN) >> 1];
      out.ppu_oe = true;
    }
    return;
  }

  if (cpu_bus.read) {
    if (ADDR_OAM_BEGIN <= cpu_bus.addr && cpu_bus.addr <= ADDR_OAM_END) {
      uint16_t data16 = ram[(cpu_bus.addr - ADDR_OAM_BEGIN) >> 1];

      out.cpu_addr = cpu_bus.addr;
      out.cpu_data = uint8_t(cpu_bus.addr & 1 ? (data16 >> 8) : data16);
      out.cpu_oe = true;
    }
  }

  if (cpu_bus.write) {
    if (ADDR_OAM_BEGIN <= cpu_bus.addr && cpu_bus.addr <= ADDR_OAM_END) {
      uint16_t& d = ram[(cpu_bus.addr - ADDR_OAM_BEGIN) >> 1];
      if (cpu_bus.addr & 1) {
        d = (d & 0x00FF) | (cpu_bus.data << 8);
      }
      else {
        d = (d & 0xFF00) | (cpu_bus.data << 0);
      }
    }
  }
}

void OAM::dump(std::string& d) const {
  dumpit(out.cpu_addr,   "0x%04x");
  dumpit(out.cpu_data,   "0x%02x");
  dumpit(out.cpu_oe,     "%d");
  dumpit(out.ppu_addr,   "0x%04x");
  dumpit(out.ppu_data16, "0x%04x");
  dumpit(out.ppu_oe,     "%d");

  /*
  for (int i = 0; i < 10; i++) {
    sprintf(out, "%2x ", ram[i]);
  }
  sprintf(out, "\n");
  */
}

//-----------------------------------------------------------------------------

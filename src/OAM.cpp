#include "OAM.h"

#include "Constants.h"

//-----------------------------------------------------------------------------

void OAM::reset() {
  *this = {};
}

//-----------------------------------------------------------------------------

OAM::Out OAM::tick() const {
  return {
    cpu_addr,
    cpu_data,
    cpu_oe,
    ppu_addr,
    ppu_data16,
    ppu_oe,
  };
}

void OAM::tock(int tphase, bool oam_lock, const CpuBus cpu_bus, CpuBus ppu_bus) {

  if (oam_lock) {
    if (ppu_bus.read) {
      ppu_addr = ppu_bus.addr;
      ppu_data16 = ram[(ppu_bus.addr - ADDR_OAM_BEGIN) >> 1];
      ppu_oe = true;
    }
    return;
  }

  if (cpu_bus.read) {
    if (ADDR_OAM_BEGIN <= cpu_bus.addr && cpu_bus.addr <= ADDR_OAM_END) {
      uint16_t data16 = ram[(cpu_bus.addr - ADDR_OAM_BEGIN) >> 1];

      cpu_addr = cpu_bus.addr;
      cpu_data = uint8_t(cpu_bus.addr & 1 ? (data16 >> 8) : data16);
      cpu_oe = true;
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
  dumpit(cpu_addr,   "0x%04x");
  dumpit(cpu_data,   "0x%02x");
  dumpit(cpu_oe,     "%d");
  dumpit(ppu_addr,   "0x%04x");
  dumpit(ppu_data16, "0x%04x");
  dumpit(ppu_oe,     "%d");

  /*
  for (int i = 0; i < 10; i++) {
    sprintf(out, "%2x ", ram[i]);
  }
  sprintf(out, "\n");
  */
}

//-----------------------------------------------------------------------------

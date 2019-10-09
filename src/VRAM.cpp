#include "VRAM.h"

#include "Constants.h"

extern uint8_t bootrom_logo[];

//-----------------------------------------------------------------------------

void VRAM::reset() {
  *this = {};
  memcpy(ram, bootrom_logo, 416);
}

//-----------------------------------------------------------------------------

VRAM::Out VRAM::tick() const {
  return {
    cpu_addr,
    cpu_data,
    cpu_oe,
    ppu_addr,
    ppu_data,
    ppu_oe,
  };
}

void VRAM::tock(int tphase, bool vram_lock, CpuBus cpu_bus, CpuBus ppu_bus) {
  if (vram_lock) {
    if (ppu_bus.read) {
      ppu_addr = ppu_bus.addr;
      ppu_data = ram[ppu_bus.addr - ADDR_VRAM_BEGIN];
      ppu_oe = true;
    }
    return;
  }

  if (cpu_bus.read) {
    if (ADDR_VRAM_BEGIN <= cpu_bus.addr && cpu_bus.addr <= ADDR_VRAM_END) {
      cpu_addr = cpu_bus.addr;
      cpu_data = ram[cpu_bus.addr - ADDR_VRAM_BEGIN];
      cpu_oe = true;
    }
  }

  if (cpu_bus.write) {
    if (ADDR_VRAM_BEGIN <= cpu_bus.addr && cpu_bus.addr <= ADDR_VRAM_END) {
      ram[cpu_bus.addr - ADDR_VRAM_BEGIN] = cpu_bus.data;
    }
  }
}

//-----------------------------------------------------------------------------

void VRAM::dump(std::string& d) {
  dumpit(cpu_addr, "0x%04x");
  dumpit(cpu_data, "0x%02x");
  dumpit(cpu_oe,   "%d");
  dumpit(ppu_addr, "0x%04x");
  dumpit(ppu_data, "0x%02x");
  dumpit(ppu_oe,   "%d");
}

//-----------------------------------------------------------------------------

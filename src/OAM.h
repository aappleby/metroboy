#pragma once
#include "Types.h"

//-----------------------------------------------------------------------------

struct OAM {

  struct Out {
    uint16_t cpu_addr;
    uint8_t  cpu_data;
    bool     cpu_oe;

    uint16_t ppu_addr;
    uint16_t ppu_data16;
    bool     ppu_oe;
  };

  void reset();
  Out  tick() const;
  void tock(int tphase, bool oam_lock, const CpuBus cpu_bus, CpuBus ppu_bus);
  void dump(std::string& out) const;

  const uint8_t* get() const { return (uint8_t*)ram; }

private:

  uint16_t cpu_addr;
  uint8_t  cpu_data;
  bool     cpu_oe;

  uint16_t ppu_addr;
  uint16_t ppu_data16;
  bool     ppu_oe;

  uint16_t ram[128];
};

//-----------------------------------------------------------------------------

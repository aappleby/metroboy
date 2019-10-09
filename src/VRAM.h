#pragma once
#include "Types.h"

//-----------------------------------------------------------------------------

struct VRAM {

  struct Out {
    uint16_t cpu_addr;
    uint8_t  cpu_data;
    bool     cpu_oe;

    uint16_t ppu_addr;
    uint8_t  ppu_data;
    bool     ppu_oe;
  };

  void  reset();
  Out   tick() const;
  void  tock(int tphase, bool vram_lock, CpuBus cpu_bus, CpuBus ppu_bus);
  void  dump(std::string& d);

  const uint8_t* get_ram() const { return ram; }

private:

  uint16_t cpu_addr;
  uint8_t  cpu_data;
  bool     cpu_oe;

  uint16_t ppu_addr;
  uint8_t  ppu_data;
  bool     ppu_oe;

  uint8_t ram[8192];
};

//-----------------------------------------------------------------------------

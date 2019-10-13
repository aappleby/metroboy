#pragma once
#include "Types.h"

//-----------------------------------------------------------------------------

struct VRAM {

  struct Out {
    Bus vram_to_bus;
    Bus vram_to_dma;
    Bus vram_to_ppu;
  };

  void  reset();
  Out   tick() const;
  void  tock(int tcycle_, Bus bus_to_vram_, Bus dma_to_vram_, Bus ppu_to_vram_);
  void  dump(std::string& d);

  const uint8_t* get_ram() const { return ram; }

private:

  int tphase;
  Bus bus_to_vram;
  Bus dma_to_vram;
  Bus ppu_to_vram;

  Bus vram_to_bus;
  Bus vram_to_dma;
  Bus vram_to_ppu;

  uint8_t ram[8192];
};

//-----------------------------------------------------------------------------

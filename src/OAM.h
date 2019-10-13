#pragma once
#include "Types.h"

//-----------------------------------------------------------------------------

struct OAM {

  struct Out {
    Bus oam_to_bus;
    Bus oam_to_ppu;
  };

  void reset();
  Out  tick() const;
  void tock(int tcycle_, const Bus bus_to_oam_, Bus dma_to_oam_, Bus ppu_to_oam_);
  void dump(std::string& out) const;

  const uint8_t* get() const { return (uint8_t*)ram; }

private:

  int tphase;
  bool oam_lock;
  
  Bus bus_to_oam;
  Bus dma_to_oam;
  Bus ppu_to_oam;

  Bus oam_to_bus;
  Bus oam_to_ppu;

  uint16_t ram[128];
};

//-----------------------------------------------------------------------------

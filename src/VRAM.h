#pragma once
#include "Types.h"

//-----------------------------------------------------------------------------

struct VRAM {

  struct Out {
    uint16_t addr;
    uint8_t data;
    bool oe;
  };

  void  reset();
  Out   tick() const;
  void  tock(CpuBus bus_);

  const uint8_t* get_ram() const { return ram; }

private:
  Out out;
  uint8_t ram[8192];
};

//-----------------------------------------------------------------------------

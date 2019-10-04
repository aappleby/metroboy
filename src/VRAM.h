#pragma once
#include "Types.h"

//-----------------------------------------------------------------------------

struct VRAM {
  BusOut reset();

  BusOut tick() const;
  void   tock(CpuBus bus_);
  const  uint8_t* get_ram() const { return ram; }

private:
  BusOut out;
  uint8_t ram[8192];
};

//-----------------------------------------------------------------------------

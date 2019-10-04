#pragma once
#include "Types.h"

//-----------------------------------------------------------------------------

struct ZRAM {
  BusOut reset();

  BusOut tick() const;
  void tock(CpuBus bus);
  const uint8_t* get() const { return ram; }

private:
  BusOut out;
  uint8_t ram[127];
};

//-----------------------------------------------------------------------------

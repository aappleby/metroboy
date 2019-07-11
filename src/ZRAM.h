#pragma once
#include "Types.h"

//-----------------------------------------------------------------------------

struct ZRAM {
  BusOut reset();
  BusOut tock(CpuBus bus);
  const uint8_t* get() const { return ram; }

private:
  uint8_t ram[127];
};

//-----------------------------------------------------------------------------

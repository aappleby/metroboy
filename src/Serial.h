#pragma once
#include "Types.h"

//-----------------------------------------------------------------------------

struct Serial {
  BusOut reset();
  BusOut tock(CpuBus bus);

private:
  uint8_t sb; // FF01
  uint8_t sc; // FF02
};

//-----------------------------------------------------------------------------

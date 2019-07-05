#pragma once
#include "Types.h"

//-----------------------------------------------------------------------------

struct OAM {
  void reset();
  BusOut tock(CpuBus bus);
  void dump(std::string& out) const;
  uint8_t ram[256];
};

//-----------------------------------------------------------------------------

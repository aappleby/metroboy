#pragma once
#include "Types.h"

//-----------------------------------------------------------------------------

struct OAM {
  BusOut reset();
  BusOut tock(CpuBus bus);
  void dump(std::string& out) const;
  const uint8_t* get() const { return ram; }

private:
  uint8_t ram[256];
};

//-----------------------------------------------------------------------------

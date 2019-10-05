#pragma once
#include "Types.h"

//-----------------------------------------------------------------------------

struct OAM {
  void reset();

  BusOut tick() const;
  void tock(const CpuBus bus);

  void dump(std::string& out) const;
  const uint8_t* get() const { return ram; }

private:
  BusOut out;
  uint8_t ram[256];
};

//-----------------------------------------------------------------------------

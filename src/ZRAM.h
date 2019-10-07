#pragma once
#include "Types.h"

//-----------------------------------------------------------------------------

struct ZRAM {
  typedef BusOut Out;

  void reset();
  Out  tick() const;
  void tock(CpuBus bus);
  void dump(std::string& d);

  const uint8_t* get() const { return ram; }

private:
  Out out;
  uint8_t ram[127];
};

//-----------------------------------------------------------------------------

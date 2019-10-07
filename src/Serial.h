#pragma once
#include "Types.h"

//-----------------------------------------------------------------------------

struct Serial {
  typedef BusOut Out;

  void reset();
  Out  tick() const;
  void tock(CpuBus bus);
  void dump(std::string& d);

private:
  Out out;
  uint8_t sb; // FF01
  uint8_t sc; // FF02
};

//-----------------------------------------------------------------------------

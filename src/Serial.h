#pragma once
#include "Types.h"

//-----------------------------------------------------------------------------

struct Serial {
  struct Out {
    uint16_t addr;
    uint8_t data;
    bool oe;
  };

  void reset();
  Out  tick() const;
  void tock(int tphase, CpuBus bus);
  void dump(std::string& d);

private:
  Out out;
  uint8_t sb; // FF01
  uint8_t sc; // FF02
};

//-----------------------------------------------------------------------------

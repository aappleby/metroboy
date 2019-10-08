#pragma once
#include "Types.h"

//-----------------------------------------------------------------------------

struct Timer {
  struct Out {
    uint16_t addr;
    uint8_t data;
    bool oe;
    bool interrupt;
  };

  void reset();
  Out  tick() const;
  void tock(int tphase, const CpuBus bus);

  void dump(std::string& out);

private:

  uint16_t counter;  // FF04
  uint16_t tima;     // FF05
  uint8_t  tma;      // FF06
  uint8_t  tac;      // FF07
  
  bool do_tick;
  Out out;
};

//-----------------------------------------------------------------------------

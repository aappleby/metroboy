#pragma once
#include "Types.h"

//-----------------------------------------------------------------------------

struct Timer {
  void reset();
  BusOut tock(int tphase, CpuBus bus);
  void dump(std::string& out);

  bool overflow;

//private:

  void do_tima();

  uint16_t counter;  // FF04
  uint8_t  old_tima; // FF05
  uint8_t  new_tima;
  uint8_t  tma;      // FF06
  uint8_t  tac;      // FF07
  bool     tick;
};

//-----------------------------------------------------------------------------

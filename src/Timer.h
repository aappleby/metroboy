#pragma once
#include "Types.h"

//-----------------------------------------------------------------------------

struct Timer {
  TimerOut reset();
  TimerOut tock(int tphase, CpuBus bus);
  void dump(std::string& out);

private:

  uint16_t counter;  // FF04
  //uint8_t  tima;     // FF05
  uint8_t old_tima;
  uint8_t new_tima;
  uint8_t  tma;      // FF06
  uint8_t  tac;      // FF07
  bool     tick;
  
  TimerOut out;
};

//-----------------------------------------------------------------------------

#pragma once
#include "Types.h"

//-----------------------------------------------------------------------------

struct Timer {
  void reset();
  Bus  tick() const;
  bool get_interrupt() const;
  void tock(int tphase_, Bus bus_to_timer_);

  void dump(std::string& out);

private:
  int tphase;
  Bus bus_to_timer;
  Bus timer_to_bus;

  uint16_t counter;  // FF04
  uint16_t tima;     // FF05
  uint8_t  tma;      // FF06
  uint8_t  tac;      // FF07
  
  bool do_tick;
  bool do_interrupt;
};

//-----------------------------------------------------------------------------

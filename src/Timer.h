#pragma once
#include "Types.h"

//-----------------------------------------------------------------------------

struct Timer {
  void reset();
  Bus  to_bus() const { return timer_to_bus; }
  bool get_interrupt() const;
  void tock(int tcycle_, Bus bus_to_timer_);

  void dump(std::string& out);

private:
  int tcycle;
  Bus bus_to_timer;
  Bus timer_to_bus;

  uint16_t counter;
  uint16_t tima;
  uint8_t  tma;
  uint8_t  tac;
  bool     do_int;
  bool     tick;
};

//-----------------------------------------------------------------------------

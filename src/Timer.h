#pragma once
#include "Types.h"

//-----------------------------------------------------------------------------

struct Timer {
  void reset();
  Bus  tick() const;
  bool get_interrupt() const;
  void tock(int tcycle_, Bus bus_to_timer_);

  void dump(std::string& out);

private:
  Bus bus_to_timer;
  Bus timer_to_bus;

  uint16_t counter;
  uint16_t tima;
  uint8_t  tma;
  uint8_t  tac;
  bool     do_int;

};

//-----------------------------------------------------------------------------

#pragma once
#include "Types.h"

//-----------------------------------------------------------------------------

struct Timer {
  void reset();
  bool get_interrupt() const;

  bool on_ibus_req(Req ibus_req, Ack& ibus_ack);

  void tock(int tcycle);

  void dump(std::string& out);

private:
  uint16_t counter;
  uint16_t tima;
  uint8_t  tma;
  uint8_t  tac;
  bool     do_int;
  bool     do_tick;
};

//-----------------------------------------------------------------------------

#pragma once
#include "Types.h"

//-----------------------------------------------------------------------------

struct Timer {
  void reset();
  bool get_interrupt() const;

  void ibus_req(Req ibus_req);
  void ibus_ack(Ack& ibus_ack) const;

  void tock(int tcycle);

  void dump(std::string& out);

private:
  Ack ack;
  uint16_t counter;
  uint16_t tima;
  uint8_t  tma;
  uint8_t  tac;
  bool     do_int;
  bool     do_tick;
};

//-----------------------------------------------------------------------------

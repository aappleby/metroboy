#pragma once
#include "Types.h"

//-----------------------------------------------------------------------------

struct Timer {
  void reset();
  bool get_interrupt() const;

  void tock_req(const Req& req);
  void tick_ack(Ack& ack) const;

  void tock_t0();

  void dump(std::string& out);

private:
  Ack ack;
  uint16_t counter = 0;
  uint16_t tima = 0;
  uint8_t  tma = 0;
  uint8_t  tac = 0;
  bool     do_int = 0;
  bool     do_tick = 0;
};

//-----------------------------------------------------------------------------

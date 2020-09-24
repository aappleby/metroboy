#pragma once
#include "CoreLib/Types.h"

//-----------------------------------------------------------------------------

struct Timer {
  void reset();

  void tick(int phase_total, const Req& req, Ack& ack);
  void tock(int phase_total, const Req& req);
  void dump(Dumper& dump) const;
  void update_tima();

  uint16_t div = 0;
  uint8_t  tima = 0;
  uint8_t  tma = 0;
  uint8_t  tac = 0;

  bool tima_clk = 0;
  bool tima_7_sync = 0;
  bool timer_int = 0;
};

//-----------------------------------------------------------------------------

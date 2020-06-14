#pragma once
#include "Types.h"

//-----------------------------------------------------------------------------

struct NewTimer {
  void reset();

  void tick(const Req& req, Ack& ack);
  void tock(int phase, const Req& req);
  void dump(std::string& d);

  void update_tima();

  bool get_interrupt() const {
    return timer_int;
  }

  //private:

  uint16_t div = 0;
  uint8_t  tima = 0;
  uint8_t  tma = 0;
  uint8_t  tac = 0;

  bool tima_clk = 0;
  bool tima_7_sync = 0;
  bool timer_int = 0;
};

//-----------------------------------------------------------------------------

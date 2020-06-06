#pragma once
#include "Types.h"

//-----------------------------------------------------------------------------

struct NewTimer {
  void reset();
  bool get_interrupt() const;

  void tick(int phase, const Req& req, Ack& ack);
  void tock(int phase, const Req& req);
  void update_tima();

//private:

  uint16_t div = 0;
  uint8_t  tima = 0;
  uint8_t  tma = 0;
  uint8_t  tac = 0;

  bool tima_c = 0;
  bool tima_max = 0;
  bool tima_clk = 0;
  bool int_timer = 0;
};

//-----------------------------------------------------------------------------

struct Timer {
  void reset();
  bool get_interrupt() const;

  void tick(int phase, const Req& req, Ack& ack);
  void tock(int phase, const Req& req);

  //void reg_test();
  void dump(std::string& out);

//private:

  //NewTimer new_timer;

  uint16_t counter = 0;
  uint8_t  old_tima = 0;
  uint8_t  new_tima = 0;
  uint8_t  tma = 0;
  uint8_t  tac = 0;
  bool     do_int = 0;
  bool     do_tick = 0;
};


//-----------------------------------------------------------------------------

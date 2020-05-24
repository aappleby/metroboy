#pragma once
#include "Types.h"

//-----------------------------------------------------------------------------

struct Bootrom {
  Bootrom() = default;

  void reset(uint16_t new_pc);
  void dump(std::string& d);

  void tock_req(const Req& req);
  void tick_ack(Ack& ack) const;

  Ack  ack;
  int disable_bootrom = 0;
};

//-----------------------------------------------------------------------------

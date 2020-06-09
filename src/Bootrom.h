#pragma once
#include "Types.h"

//-----------------------------------------------------------------------------

struct Bootrom {
  Bootrom() = default;

  void reset(uint16_t new_pc);
  void dump(std::string& d);

  void tock(int phase, const Req& req);
  void tick(int phase, const Req& req, Ack& ack) const;

  int disable_bootrom = 0;
};

//-----------------------------------------------------------------------------

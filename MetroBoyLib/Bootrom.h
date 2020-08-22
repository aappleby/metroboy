#pragma once
#include "CoreLib/Types.h"

//-----------------------------------------------------------------------------

struct Bootrom {
  Bootrom() = default;

  void reset(uint16_t new_pc);

  void tock(const Req& req);
  void tick(const Req& req, Ack& ack) const;

  int disable_bootrom = 0;
};

//-----------------------------------------------------------------------------

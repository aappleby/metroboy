#pragma once
#include "MetroLib/src/CoreLib/Types.h"

struct Req;
struct Ack;

//-----------------------------------------------------------------------------

struct MetroBoyBootrom {
  MetroBoyBootrom() = default;

  void tick(int phase_total, const Req& req, Ack& ack) const;
  void tock(int phase_total, const Req& req);

  int disable_bootrom = 1;
};

//-----------------------------------------------------------------------------

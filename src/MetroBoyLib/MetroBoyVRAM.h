#pragma once
#include "CoreLib/Types.h"

//-----------------------------------------------------------------------------

struct MetroBoyVRAM {
  void  reset();
  void  tick(int phase_total, const Req& req, Ack& ack) const;
  void  tock(int phase_total, const Req& req);

  uint8_t ram[8192] = {0};
};

//-----------------------------------------------------------------------------

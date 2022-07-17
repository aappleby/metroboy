#pragma once
#include "CoreLib/Types.h"

//-----------------------------------------------------------------------------

struct MetroBoyOAM {

  void reset();
  void tock(int phase_total, const Req& req);
  void tick(int phase_total, const Req& req, Ack& ack) const;

  void dump(Dumper& dump) const;

  uint16_t ram[128] = {0};
};

//-----------------------------------------------------------------------------

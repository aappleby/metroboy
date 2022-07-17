#pragma once
#include "CoreLib/Types.h"

//-----------------------------------------------------------------------------

struct MetroBoyZRAM {
  void reset();

  void tock(int phase_total, const Req& req);
  void tick(int phase_total, const Req& req, Ack& ack) const;
  void dump(Dumper& dump) const;
  uint8_t* get() { return ram; }

  uint8_t ram[128] = {0};
};

//-----------------------------------------------------------------------------

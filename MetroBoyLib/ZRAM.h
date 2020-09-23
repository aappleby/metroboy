#pragma once
#include "CoreLib/Types.h"

//-----------------------------------------------------------------------------

struct ZRAM {
  void reset();

  void tock(const Req& req);
  void tick(const Req& req, Ack& ack) const;
  void dump(Dumper& dump) const;
  uint8_t* get() { return ram; }

  uint8_t ram[128] = {0};
};

//-----------------------------------------------------------------------------

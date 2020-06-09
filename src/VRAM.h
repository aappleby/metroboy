#pragma once
#include "Types.h"

//-----------------------------------------------------------------------------

struct VRAM {
  void  reset();
  void  tock(int phase, const Req& req);
  void  tick(int phase, const Req& req, Ack& ack) const;
  uint8_t* get() { return ram; }

private:
  uint8_t ram[8192] = {0};
};

//-----------------------------------------------------------------------------

#pragma once
#include "CoreLib/Types.h"

//-----------------------------------------------------------------------------

struct VRAM {
  void  reset();
  void  tock(const Req& req);
  void  tick(const Req& req, Ack& ack) const;
  uint8_t* get() { return ram; }

private:
  uint8_t ram[8192] = {0};
};

//-----------------------------------------------------------------------------

#pragma once
#include "Types.h"

//-----------------------------------------------------------------------------

struct VRAM {
  void  reset();
  void  vbus_req(Req vbus_req);
  void  vbus_ack(Ack& vbus_ack) const;
  uint8_t* get() { return ram; }

private:
  Ack ack;
  uint8_t ram[8192];
};

//-----------------------------------------------------------------------------

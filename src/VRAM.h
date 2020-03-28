#pragma once
#include "Types.h"

//-----------------------------------------------------------------------------

struct VRAM {
  void  reset();
  bool  on_vbus_req(Req vbus_req, Ack& vbus_ack);
  uint8_t* get() { return ram; }

private:
  uint8_t ram[8192];
};

//-----------------------------------------------------------------------------

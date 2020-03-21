#pragma once
#include "Types.h"

//-----------------------------------------------------------------------------

struct VRAM {
  void  reset();
  Ack   on_vbus_req(Req vbus_req);
  void  dump(std::string& d);
  uint8_t* get() { return ram; }

private:
  uint8_t ram[8192];
};

//-----------------------------------------------------------------------------

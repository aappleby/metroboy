#pragma once
#include "Types.h"

//-----------------------------------------------------------------------------

struct ZRAM {
  void reset();

  bool on_ibus_req(Req ibus_req, Ack& ibus_ack);
  void dump(std::string& d);
  uint8_t* get() { return ram; }

private:
  uint8_t  ram[128];
};

//-----------------------------------------------------------------------------

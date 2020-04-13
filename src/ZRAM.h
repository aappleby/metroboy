#pragma once
#include "Types.h"

//-----------------------------------------------------------------------------

struct ZRAM {
  void reset();

  void ibus_req(Req ibus_req);
  void ibus_ack(Ack& ibus_ack) const;
  void dump(std::string& d);
  uint8_t* get() { return ram; }

private:
  Ack ack;
  uint8_t  ram[128];
};

//-----------------------------------------------------------------------------

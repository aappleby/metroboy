#pragma once
#include "Types.h"

//-----------------------------------------------------------------------------

struct ZRAM {
  void reset();

  void tock_req(const Req& req);
  void tick_ack(Ack& ack) const;
  void dump(std::string& d);
  uint8_t* get() { return ram; }

private:
  Ack ack;
  uint8_t ram[128] = {0};
};

//-----------------------------------------------------------------------------

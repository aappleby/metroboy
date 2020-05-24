#pragma once
#include "Types.h"

//-----------------------------------------------------------------------------

struct Serial {
  void reset();
  
  void tock_req(const Req& ibus_req);
  void tick_ack(Ack& ibus_ack) const;

  void dump(std::string& d);

private:
  Ack ack;
  uint8_t sb = 0;
  uint8_t sc = 0;
};

//-----------------------------------------------------------------------------

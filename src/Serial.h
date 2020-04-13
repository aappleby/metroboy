#pragma once
#include "Types.h"

//-----------------------------------------------------------------------------

struct Serial {
  void reset();
  
  void ibus_req(Req ibus_req);
  void ibus_ack(Ack& ibus_ack) const;

  void dump(std::string& d);

private:
  Ack ack;
  uint8_t sb; // FF01
  uint8_t sc; // FF02
};

//-----------------------------------------------------------------------------

#pragma once
#include "Types.h"

//-----------------------------------------------------------------------------

struct Serial {
  void reset();
  
  bool on_ibus_req(Req ibus_req, Ack& ibus_ack);

  void dump(std::string& d);

private:
  uint8_t sb; // FF01
  uint8_t sc; // FF02
};

//-----------------------------------------------------------------------------

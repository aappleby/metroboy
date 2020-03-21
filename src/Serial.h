#pragma once
#include "Types.h"

//-----------------------------------------------------------------------------

struct Serial {
  void reset();
  
  Ack  on_ibus_req(Req ibus_req);

  void dump(std::string& d);

private:
  uint8_t sb; // FF01
  uint8_t sc; // FF02
};

//-----------------------------------------------------------------------------

#pragma once
#include "Types.h"

//-----------------------------------------------------------------------------

struct Bootrom {
  void reset(uint16_t new_pc);
  Ack  on_ibus_req(Req ibus_req);
  void dump(std::string& d);

  bool disable_bootrom;
};

//-----------------------------------------------------------------------------

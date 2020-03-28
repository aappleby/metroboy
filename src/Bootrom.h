#pragma once
#include "Types.h"

//-----------------------------------------------------------------------------

struct Bootrom {
  void reset(uint16_t new_pc);
  bool on_ibus_req(Req ibus_req, Ack& ibus_ack);
  void dump(std::string& d);

  bool disable_bootrom;
};

//-----------------------------------------------------------------------------

#pragma once
#include "Types.h"

//-----------------------------------------------------------------------------

struct Bootrom {
  void reset(uint16_t new_pc);
  void dump(std::string& d);

  void ibus_req(Req ibus_req);
  void ibus_ack(Ack& ack);

  Ack  ack;
  bool disable_bootrom;
};

//-----------------------------------------------------------------------------

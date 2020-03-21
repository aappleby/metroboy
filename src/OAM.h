#pragma once
#include "Types.h"

//-----------------------------------------------------------------------------

struct OAM {

  void reset();
  Ack  on_obus_req(Req obus_req);
  void dump(std::string& out) const;

  const uint8_t* get() const { return (uint8_t*)ram; }

//private:
  uint16_t ram[128];
};

//-----------------------------------------------------------------------------

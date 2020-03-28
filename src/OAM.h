#pragma once
#include "Types.h"

//-----------------------------------------------------------------------------

struct OAM {

  void reset();
  bool on_obus_req(Req obus_req, Ack& obus_ack);
  void dump(std::string& out) const;

  const uint8_t* get() const { return (uint8_t*)ram; }

//private:
  uint16_t ram[128];
};

//-----------------------------------------------------------------------------

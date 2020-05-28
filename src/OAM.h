#pragma once
#include "Types.h"

//-----------------------------------------------------------------------------

struct OAM {

  void reset();

  void tock_req(const Req& obus_req);
  void tick_ack(Ack& obus_ack) const;

  void dump(std::string& out) const;

  const uint8_t* get() const { return (uint8_t*)ram; }

private:
  Ack ack;
  uint16_t ram[128] = {0};
};

//-----------------------------------------------------------------------------

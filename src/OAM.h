#pragma once
#include "Types.h"

//-----------------------------------------------------------------------------

struct OAM {

  void reset();

  void tock(const Req& req);
  void tick(const Req& req, Ack& ack) const;

  void dump(std::string& out) const;

  const uint8_t* get() const { return (uint8_t*)ram; }

private:
  uint16_t ram[128] = {0};
};

//-----------------------------------------------------------------------------

#pragma once
#include "Types.h"

//-----------------------------------------------------------------------------

struct Serial {
  void reset();
  
  void tock(int phase, const Req& req);
  void tick(int phase, const Req& req, Ack& ack) const;

  void dump(std::string& d);

private:
  uint8_t sb = 0;
  uint8_t sc = 0;
};

//-----------------------------------------------------------------------------

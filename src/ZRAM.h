#pragma once
#include "Types.h"

//-----------------------------------------------------------------------------

struct ZRAM {
  void reset();

  void tock(int phase, const Req& req);
  void tick(int phase, const Req& req, Ack& ack) const;
  void dump(std::string& d);
  uint8_t* get() { return ram; }

private:
  uint8_t ram[128] = {0};
};

//-----------------------------------------------------------------------------

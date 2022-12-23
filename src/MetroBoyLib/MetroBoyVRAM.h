#pragma once

#include <stdint.h>

struct Req;
struct Ack;
struct Dumper;

//-----------------------------------------------------------------------------

struct MetroBoyVRAM {
  void  reset();
  void  tick(int phase_total, const Req& req, Ack& ack) const;
  void  tock(int phase_total, const Req& req);

  uint8_t ram[8192] = {0};
};

//-----------------------------------------------------------------------------

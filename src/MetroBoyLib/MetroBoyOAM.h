#pragma once

#include <stdint.h>
struct Req;
struct Ack;
struct Dumper;

//-----------------------------------------------------------------------------

struct MetroBoyOAM {

  void reset();
  void tock(int phase_total, const Req& req);
  void tick(int phase_total, const Req& req, Ack& ack) const;

  void dump(Dumper& dump) const;

  uint16_t ram[128] = {0};
};

//-----------------------------------------------------------------------------

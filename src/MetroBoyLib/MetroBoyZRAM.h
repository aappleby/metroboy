#pragma once

#include <stdint.h>

struct Req;
struct Ack;
struct Dumper;

//-----------------------------------------------------------------------------

struct MetroBoyZRAM {
  void reset();

  void tock(int phase_total, const Req& req);
  void tick(int phase_total, const Req& req, Ack& ack) const;
  void dump(Dumper& dump) const;
  uint8_t* get() { return ram; }

  uint8_t ram[128] = {0};
};

//-----------------------------------------------------------------------------

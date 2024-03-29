#pragma once

#include <stdint.h>

struct Req;
struct Ack;
struct Dumper;

//-----------------------------------------------------------------------------

struct MetroBoySerial {
  void reset();

  void tick(int phase_total, const Req& req, Ack& ack) const;
  void tock(int phase_total, const Req& req);

  void dump(Dumper& dump) const;

private:
  uint8_t sb = 0;
  uint8_t sc = 0;
};

//-----------------------------------------------------------------------------

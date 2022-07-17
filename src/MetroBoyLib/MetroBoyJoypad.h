#pragma once
#include "CoreLib/Types.h"

struct MetroBoyJoypad {
  MetroBoyJoypad() = default;

  void reset();

  void tick(int phase_total, const Req& req, Ack& ack) const;
  void tock(int phase_total, const Req& req);

  uint8_t get() const { return val; }
  void set(uint8_t new_val);
  void dump(Dumper& dump) const;

  uint8_t val = 0;
  uint8_t p1 = 0; // FF00
};
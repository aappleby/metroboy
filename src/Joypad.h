#pragma once
#include "Types.h"

struct Joypad {
  Joypad() = default;

  void reset();

  void tock(int phase, const Req& req);
  void tick(int phase, const Req& req, Ack& ack) const;
  void dump(std::string& d) const;

  uint8_t get() const { return val; }
  void set(uint8_t new_val);

  uint8_t val = 0;
  uint8_t p1 = 0; // FF00
};
#pragma once
#include "Types.h"

struct Joypad {
  void reset();

  void ibus_req(Req ibus_req);
  void ibus_ack(Ack& ibus_ack) const;
  void dump(std::string& d) const;

  uint8_t get() const { return val; }
  void set(uint8_t new_val);

  Ack ack;
  uint8_t val;
  uint8_t p1 = 0; // FF00
};
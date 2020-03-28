#pragma once
#include "Types.h"

struct Joypad {
  void reset();

  bool on_ibus_req(Req ibus_req, Ack& ibus_ack);
  void dump(std::string& d) const;

  uint8_t get() const { return val; }
  void set(uint8_t new_val);

  uint8_t val;
  uint8_t p1 = 0; // FF00
};
#pragma once
#include "Types.h"

struct Buttons {

  void reset();
  Bus  tick() const;
  void tock(int tcycle_, Bus bus_to_buttons_);
  void dump(std::string& d) const;

  uint8_t get() const { return val; }
  void set(uint8_t new_val);

private:
  int tphase;
  Bus bus_to_buttons;
  Bus buttons_to_bus;

  uint8_t val;
  uint8_t p1 = 0; // FF00
};
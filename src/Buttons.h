#pragma once
#include "Types.h"

struct Buttons {
  ButtonsOut reset();
  ButtonsOut tock(CpuBus bus);

  void set(uint8_t new_val) { val = new_val; }
  void dump(std::string& out) const;

private:

  uint8_t val = 0;
  uint8_t p1 = 0; // FF00
};
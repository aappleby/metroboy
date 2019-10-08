#pragma once
#include "Types.h"

struct Buttons {

  struct Out {
    uint16_t addr;
    uint8_t data;
    bool oe;
    uint8_t val;
  };

  void reset();
  Out  tick() const;
  void tock(int tphase, CpuBus bus);
  void dump(std::string& d) const;

  void set(uint8_t new_val);

private:

  Out out;
  uint8_t p1 = 0; // FF00
};
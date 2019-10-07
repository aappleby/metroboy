#pragma once
#include "Types.h"

struct IRAM {
  typedef BusOut Out;

  void reset();
  Out  tick() const;
  void tock(CpuBus bus);
  void dump(std::string& d);

  const uint8_t* get() { return ram; }
private:
  BusOut out;
  uint8_t ram[8192];
};
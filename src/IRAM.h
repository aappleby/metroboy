#pragma once
#include "Types.h"

struct IRAM {
  void reset();
  
  BusOut tick() const;
  void tock(CpuBus bus);

  const uint8_t* get() { return ram; }
private:
  BusOut out;
  uint8_t ram[8192];
};
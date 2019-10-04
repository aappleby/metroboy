#pragma once
#include "Types.h"

struct IRAM {
  BusOut reset();
  
  BusOut tick() const;
  void tock(CpuBus bus);

  const uint8_t* get() { return ram; }
private:
  uint8_t ram[8192];
  BusOut out;
};
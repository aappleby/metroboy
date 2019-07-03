#pragma once
#include "Types.h"

struct IRAM {
  void reset();
  BusOut tock_t2(CpuBus bus);
  const uint8_t* get() { return ram; }
  uint8_t ram[8192];
};
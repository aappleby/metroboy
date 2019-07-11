#pragma once
#include "Types.h"

struct IRAM {
  BusOut reset();
  BusOut tock_t2(CpuBus bus);
  const uint8_t* get() { return ram; }
private:
  uint8_t ram[8192];
};
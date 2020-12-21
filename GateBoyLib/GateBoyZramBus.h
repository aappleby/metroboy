#pragma once
#include "GateBoyLib/Gates.h"

//------------------------------------------------------------------------------------------------------------------------

struct GateBoyZramBus {
  void read(GateBoyCpuBus& cpu_bus, uint8_t* zero_ram);
  void write(GateBoyCpuBus& cpu_bus, uint8_t* zero_ram);
  Signal clk_old;
};

//------------------------------------------------------------------------------------------------------------------------

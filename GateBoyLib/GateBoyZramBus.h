#pragma once
#include "GateBoyLib/Gates.h"

#include "GateBoyLib/GateBoyBuses.h"

//------------------------------------------------------------------------------------------------------------------------

struct GateBoyZramBus {
  void read(GateBoyBuses& new_bus, GateBoyCpuBus& cpu_bus, uint8_t* zero_ram);
  void write(GateBoyBuses& new_bus, GateBoyCpuBus& cpu_bus, uint8_t* zero_ram);
  Gate clk_old;
};

//------------------------------------------------------------------------------------------------------------------------

#pragma once
#include "GateBoyLib/Gates.h"

struct GateBoyResetDebug;
struct GateBoyCpuBus;

//-----------------------------------------------------------------------------

struct GateBoyBootrom {
  static void tock(GateBoyResetDebug& rst, GateBoyCpuBus& cpu_bus, uint8_t* boot_buf);
};


#pragma once
#include "GateBoyLib/Gates.h"

struct GateBoyResetDebug;
struct GateBoyCpuBus;

//-----------------------------------------------------------------------------

struct GateBoyBootrom {
  static void read_boot_bit(GateBoyCpuBus& cpu_bus);
  static void write_boot_bit_sync(GateBoyResetDebug& rst, GateBoyCpuBus& cpu_bus);
  static void read_bootrom(GateBoyResetDebug& rst, GateBoyCpuBus& cpu_bus, uint8_t* boot_buf);
};


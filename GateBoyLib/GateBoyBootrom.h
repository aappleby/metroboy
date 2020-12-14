#pragma once
#include "GateBoyLib/Gates.h"

struct GateBoyResetDebug;
struct GateBoyCpuBus;

//-----------------------------------------------------------------------------

struct GateBoyBootrom {
  void reset_cart();

  void tock(GateBoyResetDebug& rst, GateBoyCpuBus& cpu_bus, uint8_t* boot_buf);

  wire TUTU_READ_BOOTROMp_new(Signal BUS_CPU_A[16]);

  /*p07.TEPU*/ DFF17 BOOT_BITn_h;
};


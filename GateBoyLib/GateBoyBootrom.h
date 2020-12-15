#pragma once
#include "GateBoyLib/Gates.h"

struct GateBoyResetDebug;
struct GateBoyCpuBus;

//-----------------------------------------------------------------------------

struct GateBoyBootrom {
  void reset_cart();

  void tock(GateBoyResetDebug& rst, GateBoyCpuBus& cpu_bus, uint8_t* boot_buf);

  /*p07.TEPU*/ DFF17 BOOT_BITn_h;
  PinOut PIN_CPU_BOOTp;         // top right port PORTA_04: <- P07.READ_BOOTROM tutu?
};


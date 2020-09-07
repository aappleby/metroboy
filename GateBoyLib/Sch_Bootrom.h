#pragma once
#include "GateBoyLib/Gates.h"

namespace Schematics {

struct SchematicTop;
struct CpuBus;

//-----------------------------------------------------------------------------

struct Bootrom {
  void tock(const SchematicTop& gb, CpuBus& cpu_bus);

  // Starts 0, set to 1 by bootrom which blocks reading 0x0000-0x00FF.
  // In run mode, BOOT_BITn must _not_ be reset.
  /*p07.TEPU*/ RegQPN BOOT_BITn = REG_D0C0;
};

//-----------------------------------------------------------------------------

}; // namespace Schematics
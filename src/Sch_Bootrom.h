#pragma once
#include "Cells.h"

namespace Schematics {

struct SchematicTop;
struct CpuBus;

//-----------------------------------------------------------------------------

struct Bootrom {
  void tick(const SchematicTop& gb);
  void tock(const SchematicTop& gb, CpuBus& cpu_bus);
  uint64_t commit();

  /*p07.TEPU*/ wire BOOT_BITn() const { return _BOOT_BITn.q(); }

private:

  // In run mode, BOOT_BITn must _not_ be reset.
  /*p07.TEPU*/ Reg2 _BOOT_BITn = Reg2::D0C0; // Starts 0, set to 1 by bootrom which blocks reading 0x0000-0x00FF.
};

//-----------------------------------------------------------------------------

}; // namespace Schematics
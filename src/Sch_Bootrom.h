#pragma once
#include "Sch_Common.h"

namespace Schematics {

struct SchematicTop;

//-----------------------------------------------------------------------------

struct Bootrom {
  void tick(SchematicTop& gb);
  SignalHash commit();

private:
  friend struct SchematicTop;

  // In run mode, BOOT_BITn must _not_ be reset.
  /*p07.TEPU*/ Reg17 BOOT_BITn; // Starts 0, set to 1 by bootrom which blocks reading 0x0000-0x00FF.
};

//-----------------------------------------------------------------------------

}; // namespace Schematics
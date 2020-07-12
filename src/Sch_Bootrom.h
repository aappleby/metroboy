#pragma once
#include "Cells.h"

namespace Schematics {

struct SchematicTop;

//-----------------------------------------------------------------------------

struct Bootrom {
  void tick(SchematicTop& gb);
  void tock(SchematicTop& gb);
  SignalHash commit();

  /*p07.TEPU*/ wire BOOT_BITn() const { return _BOOT_BITn.q(); }

private:

  // In run mode, BOOT_BITn must _not_ be reset.
  /*p07.TEPU*/ Reg17 _BOOT_BITn; // Starts 0, set to 1 by bootrom which blocks reading 0x0000-0x00FF.
};

//-----------------------------------------------------------------------------

}; // namespace Schematics
#pragma once
#include "Sch_Common.h"

namespace Schematics {

struct SchematicTop;

//-----------------------------------------------------------------------------

struct BootSignals {
  /*p07.TEPU*/ Signal BOOT_BITn;
  /*p07.TUTU*/ Signal TUTU_BOOTp;
};

//-----------------------------------------------------------------------------

struct Bootrom {
  BootSignals sig(const SchematicTop& gb) const;
  void tick(SchematicTop& gb);
  SignalHash commit();

private:
  /*p07.TEPU*/ Reg BOOT_BITn; // Starts 0, set to 1 by bootrom which blocks reading 0x0000-0x00FF.
};

//-----------------------------------------------------------------------------

}; // namespace Schematics
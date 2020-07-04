#pragma once
#include "Sch_Common.h"

namespace Schematics {

struct SchematicTop;

//-----------------------------------------------------------------------------

struct DebugRegisters {

  void tick(const SchematicTop& top);
  SignalHash commit();

private:
  friend SchematicTop;

  /*p25.SOTO*/ Reg17 SOTO_DBG;
};

//-----------------------------------------------------------------------------

}; // namespace Schematics
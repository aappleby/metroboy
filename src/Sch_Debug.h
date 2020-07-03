#pragma once
#include "Sch_Common.h"

namespace Schematics {

struct SchematicTop;

//-----------------------------------------------------------------------------

struct DebugRegisters {

  void tick(const SchematicTop& top);
  SignalHash commit();
  /*p25.SOTO*/ Reg SOTO_DBG; // 17-rung
};

//-----------------------------------------------------------------------------

}; // namespace Schematics
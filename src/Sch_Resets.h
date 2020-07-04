#pragma once
#include "Sch_Common.h"

namespace Schematics {

struct SchematicTop;

//-----------------------------------------------------------------------------

struct ResetRegisters {

  void tick(SchematicTop& gb);
  SignalHash commit();

private:
  friend SchematicTop;

  /*p01.TUBO*/ NorLatch TUBO_CLKREQn_LATCH;
  /*p01.ASOL*/ NorLatch ASOL_RST_LATCHp; // Schematic wrong, this is a latch.
  /*p01.AFER*/ Reg13    AFER_RSTp;
};


//-----------------------------------------------------------------------------

}; // namespace Schematics
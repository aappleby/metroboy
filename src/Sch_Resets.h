#pragma once
#include "Sch_Common.h"

namespace Schematics {

struct SchematicTop;

//-----------------------------------------------------------------------------

struct ResetRegisters {

  void tick(SchematicTop& gb);
  void tock(SchematicTop& top);
  SignalHash commit();

//private:
  friend SchematicTop;

  // Must be 0 in run mode, otherwise we'd ping CPU_PIN_DBG_RST when UPOF_DIV_15 changed
  /*p01.TUBO*/ NorLatch _TUBO_CPU_READYn;

  // If AVOR_RSTp was 1 in run mode
  // then ALUR_RSTn = 0
  // then MULO_TIMA_RST = 1
  // then PUXY/NERO = 0 -> stuck
  // Therefore AVOR_RSTp = 0 in run mode
  // Therefore ASOL|AFER = 0 in run mode

  /*p01.ASOL*/ NorLatch ASOL_POR_DONEn; // Schematic wrong, this is a latch.
  /*p01.AFER*/ Reg13    AFER_SYS_RSTp; // AFER should keep clocking even if CPU_PIN_CLKREQ = 0
};


//-----------------------------------------------------------------------------

}; // namespace Schematics
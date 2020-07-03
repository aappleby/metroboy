#pragma once
#include "Sch_Common.h"

namespace Schematics {

struct SchematicTop;

//-----------------------------------------------------------------------------

struct ClockRegisters {

  void tick(const SchematicTop& top);
  SignalHash commit();

private:
  friend SchematicTop;

  /* PIN_75 */ PinOut PIN_PHI;    // <- P01.BUDE/BEVA

  // Phase generator. These registers tick on _BOTH_EDGES_ of the master clock.
  /*p01.AFUR*/ Reg9_Duo AFUR_xBCDExxx;
  /*p01.ALEF*/ Reg9_Duo ALEF_xxCDEFxx;
  /*p01.APUK*/ Reg9_Duo APUK_xxxDEFGx;
  /*p01.ADYK*/ Reg9_Duo ADYK_xxxxEFGH;

  /*p29.WUVU*/ Reg WUVU_AxxDExxH;
  /*p21.VENA*/ Reg VENA_xBCDExxx;
  /*p29.WOSU*/ Reg WOSU_xxCDxxGH;
};

//-----------------------------------------------------------------------------

}; // namespace Schematics
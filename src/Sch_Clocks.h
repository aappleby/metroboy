#pragma once
#include "Sch_Common.h"

namespace Schematics {

//-----------------------------------------------------------------------------

struct ClockSignals {
  /*p01.AFUR*/ Signal AFUR_xBCDExxx;
  /*p01.ALEF*/ Signal ALEF_xxCDEFxx;
  /*p01.APUK*/ Signal APUK_xxxDEFGx;
  /*p01.ADYK*/ Signal ADYK_xxxxEFGH;

  /*p29.WUVU*/ Signal WUVU_AxxDExxH; // to lcd spritescanner spritestore
  /*p21.VENA*/ Signal VENA_xBCDExxx; // to lcd window
  /*p29.WOSU*/ Signal WOSU_xxCDxxGH; // to busmux spritescanner
};

//-----------------------------------------------------------------------------

struct ClockRegisters {

  ClockSignals sig() const;

  void tick(const SchematicTop& gb);
  void tick(wire ABOL_CLKREQn, wire XAPO_VID_RSTn, wire UPOJ_MODE_PROD, wire PIN_CLK_IN_xBxDxFxH);
  void tick2(wire ABOL_CLKREQn, wire XAPO_VID_RSTn, wire UPOJ_MODE_PROD, wire PIN_CLK_IN_xBxDxFxH);

  SignalHash commit();

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
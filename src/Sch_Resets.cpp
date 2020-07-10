#include "Sch_Resets.h"
#include "Sch_Top.h"

using namespace Schematics;

//-----------------------------------------------------------------------------

void ResetRegisters::tick(SchematicTop& top) {
  /*p01.UCOB*/ wire UCOB = not(top.SYS_PIN_CLK_A);
  /*p01.UPYF*/ wire UPYF = or(top.SYS_PIN_RSTp, UCOB);

  // Are we _sure_ this is a nor latch?
  /*p01.TUBO*/ _TUBO_CPU_READYn.nor_latch(UPYF, top.CPU_PIN_READYp);

  /*p07.UVAR*/ wire UVAR_T2p = not(top.SYS_PIN_T2n);
  /*p07.UMUT*/ wire UMUT_MODE_DBG1p = and (top.SYS_PIN_T1n, UVAR_T2p);

  /*p07.UBET*/ wire UBET_T1p = not(top.SYS_PIN_T1n);
  /*p07.UNOR*/ wire UNOR_MODE_DBG2p = and (top.SYS_PIN_T2n, UBET_T1p);

  /*p01.UNUT*/ wire _UNUT_POR_TRIGn = and (_TUBO_CPU_READYn, top.UPOF_DIV_15());
  /*p01.TABA*/ wire _TABA_POR_TRIGn = or(UNOR_MODE_DBG2p, UMUT_MODE_DBG1p, _UNUT_POR_TRIGn);
  /*p01.ALYP*/ wire _ALYP_RSTn = not(_TABA_POR_TRIGn);
  /*p01.AFAR*/ wire _AFAR_RST  = nor(_ALYP_RSTn, top.SYS_PIN_RSTp);

  /*p01.ASOL*/ ASOL_POR_DONEn.nor_latch(top.SYS_PIN_RSTp, _AFAR_RST); // Schematic wrong, this is a latch.

  /*p07.UPOJ*/ wire UPOJ = nand(UBET_T1p, UVAR_T2p, top.SYS_PIN_RSTp);

  /*p01.AFER*/ AFER_SYS_RSTp.set(top.BOGA_xBCDEFGH(), top.BOMA_Axxxxxxx(), UPOJ, ASOL_POR_DONEn);

  top.CPU_PIN_STARTp.set(_TABA_POR_TRIGn);
  top.CPU_PIN_SYS_RSTp.set(AFER_SYS_RSTp);
}

//-----------------------------------------------------------------------------

SignalHash ResetRegisters::commit() {
  SignalHash hash;

  /*p01.TUBO*/ hash << _TUBO_CPU_READYn.commit();
  /*p01.ASOL*/ hash << ASOL_POR_DONEn.commit(); // Schematic wrong, this is a latch.
  /*p01.AFER*/ hash << AFER_SYS_RSTp.commit();
  return hash;
}

//-----------------------------------------------------------------------------

#include "Sch_Debug.h"

#include "Sch_Top.h"

using namespace Schematics;

//-----------------------------------------------------------------------------

void DebugRegisters::tick(const SchematicTop& /*top*/) {
}

void DebugRegisters::tock(const SchematicTop& top) {
  /*p25.SYCY*/ wire _SYCY_DBG_CLOCKn = not(top.UNOR_MODE_DBG2p());
  /*p25.SOTO*/ SOTO_DBG.set(_SYCY_DBG_CLOCKn, top.CUNU_SYS_RSTn(), SOTO_DBG.qn());

  //cpu_pins.UMUT_MODE_DBG1.set(dbg_sig.UMUT_MODE_DBG1);
  //cpu_pins.UNOR_MODE_DBG2.set(dbg_sig.UNOR_MODE_DBG2n);

  CPU_PIN_UNOR_DBG.set(top.UNOR_MODE_DBG2p());
  CPU_PIN_UMUT_DBG.set(top.UMUT_MODE_DBG1p());

  //cpu_pins.UPOJ_MODE_PRODn = UPOJ_MODE_PRODn;
  //cpu_pins.TOVA_MODE_DBG2n = TOVA_MODE_DBG2n;
  //cpu_pins.RYCA_MODE_DBG2n = RYCA_MODE_DBG2n;
}

//-----------------------------------------------------------------------------

SignalHash DebugRegisters::commit() {
  SignalHash hash;

  /*p25.SOTO*/ hash << SOTO_DBG.commit();
  ///*p07.BURO*/ hash << BURO_FF60_0.commit();
  ///*p07.AMUT*/ hash << AMUT_FF60_1.commit();

  hash << CPU_PIN_UNOR_DBG.commit();      // PORTA_02: <- P07.UNOR_MODE_DBG2
  hash << CPU_PIN_UMUT_DBG.commit();      // PORTA_05: <- P07.UMUT_MODE_DBG1

  return hash;
}

//-----------------------------------------------------------------------------

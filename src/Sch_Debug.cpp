#include "Sch_Debug.h"

#include "Sch_Top.h"

using namespace Schematics;

//text_painter.dprintf("UNOR_MODE_DBG2           %d\n", UNOR_MODE_DBG2        .a.val);
//text_painter.dprintf("UMUT_MODE_DBG1           %d\n", UMUT_MODE_DBG1        .a.val);
//text_painter.dprintf("UPOJ_MODE_PRODn  %d\n", UPOJ_MODE_PRODn );
//text_painter.dprintf("UMUT_MODE_DBG1  %d\n", UMUT_MODE_DBG1 );
//text_painter.dprintf("UNOR_MODE_DBG2  %d\n", UNOR_MODE_DBG2 );
//text_painter.dprintf("TOVA_MODE_DBG2n %d\n", TOVA_MODE_DBG2n);
//text_painter.dprintf("RYCA_MODE_DBG2n %d\n", RYCA_MODE_DBG2n);

//-----------------------------------------------------------------------------

void DebugRegisters::tick(const SchematicTop& top) {
  /*p25.SYCY*/ wire SYCY_DBG_CLOCKp = not(top.UNOR_MODE_DBG2p());
  /*p01.ALUR*/ wire ALUR_RSTn = not(top.AVOR_RSTp());   // this goes all over the place
  /*p01.DULA*/ wire DULA_RSTp = not(ALUR_RSTn);
  /*p01.CUNU*/ wire CUNU_RSTn = not(DULA_RSTp);
  /*p25.SOTO*/ SOTO_DBG.set(SYCY_DBG_CLOCKp, CUNU_RSTn, !SOTO_DBG);

  //cpu_pins.UMUT_MODE_DBG1.set(dbg_sig.UMUT_MODE_DBG1);
  //cpu_pins.UNOR_MODE_DBG2.set(dbg_sig.UNOR_MODE_DBG2n);
  //cpu_pins.UPOJ_MODE_PRODn = UPOJ_MODE_PRODn;
  //cpu_pins.TOVA_MODE_DBG2n = TOVA_MODE_DBG2n;
  //cpu_pins.RYCA_MODE_DBG2n = RYCA_MODE_DBG2n;
}

//-----------------------------------------------------------------------------

SignalHash DebugRegisters::commit() {
  SignalHash hash;

  /*p25.SOTO*/ hash << SOTO_DBG.commit_reg();
  ///*p07.BURO*/ hash << BURO_FF60_0.commit_reg();
  ///*p07.AMUT*/ hash << AMUT_FF60_1.commit_reg();
  return hash;
}

//-----------------------------------------------------------------------------

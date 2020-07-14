#include "Sch_Resets.h"
#include "Sch_Top.h"

#define FAST_BOOT

using namespace Schematics;

//-----------------------------------------------------------------------------

void ResetRegisters::tick(const SchematicTop& top) {
  _XONA_LCDC_EN = top.ppu_reg.XONA_LCDC_EN.q();
  _UPOF_DIV_15 = top.tim_reg.UPOF_DIV_15();
}

//-----------------------------------------------------------------------------

void ResetRegisters::tock(const SchematicTop& top) {
  /*p01.UPYF*/ wire _UPYF = or(_SYS_PIN_RSTp, top.clk_reg.UCOB_CLKBADp());

  /*p01.TUBO*/ _TUBO_CPU_READYn.nor_latch(_UPYF, CPU_PIN_READYp());

  /*p01.ALYP*/ wire _ALYP_RSTn = not(TABA_POR_TRIGn());
  /*p01.AFAR*/ wire _AFAR_RST  = nor(_ALYP_RSTn, _SYS_PIN_RSTp);

  /*p01.ASOL*/ _ASOL_POR_DONEn.nor_latch(_SYS_PIN_RSTp, _AFAR_RST); // Schematic wrong, this is a latch.
  /*p01.AFER*/ _AFER_SYS_RSTp.set(top.clk_reg.BOGA_xBCDEFGH(), top.clk_reg.BOMA_Axxxxxxx(), UPOJ_MODE_PRODn(), _ASOL_POR_DONEn.q());

  _CPU_PIN_STARTp.set(TABA_POR_TRIGn());
  _CPU_PIN_SYS_RSTp.set(AFER_SYS_RSTp());
  _CPU_PIN_EXT_RST.set(_SYS_PIN_RSTp);

  /*p25.SYCY*/ wire _SYCY_DBG_CLOCKn = not(UNOR_MODE_DBG2p());
  /*p25.SOTO*/ SOTO_DBG.set(_SYCY_DBG_CLOCKn, CUNU_SYS_RSTn(), SOTO_DBG.qn());

  //cpu_pins.UMUT_MODE_DBG1.set(dbg_sig.UMUT_MODE_DBG1);
  //cpu_pins.UNOR_MODE_DBG2.set(dbg_sig.UNOR_MODE_DBG2n);

  CPU_PIN_UNOR_DBG.set(UNOR_MODE_DBG2p());
  CPU_PIN_UMUT_DBG.set(UMUT_MODE_DBG1p());

  //cpu_pins.UPOJ_MODE_PRODn = UPOJ_MODE_PRODn;
  //cpu_pins.TOVA_MODE_DBG2n = TOVA_MODE_DBG2n;
  //cpu_pins.RYCA_MODE_DBG2n = RYCA_MODE_DBG2n;
}

//-----------------------------------------------------------------------------

SignalHash ResetRegisters::commit() {
  SignalHash hash;

  hash << _XONA_LCDC_EN.commit();
  hash << _UPOF_DIV_15.commit();

  /*p01.TUBO*/ hash << _TUBO_CPU_READYn.commit();
  /*p01.ASOL*/ hash << _ASOL_POR_DONEn.commit(); // Schematic wrong, this is a latch.
  /*p01.AFER*/ hash << _AFER_SYS_RSTp.commit();

  hash << _CPU_PIN_STARTp.commit();
  hash << _CPU_PIN_READYp.commit();
  hash << _CPU_PIN_SYS_RSTp.commit();
  hash << _CPU_PIN_EXT_RST.commit();

  /*p25.SOTO*/ hash << SOTO_DBG.commit();
  ///*p07.BURO*/ hash << BURO_FF60_0.commit();
  ///*p07.AMUT*/ hash << AMUT_FF60_1.commit();

  hash << CPU_PIN_UNOR_DBG.commit();      // PORTA_02: <- P07.UNOR_MODE_DBG2
  hash << CPU_PIN_UMUT_DBG.commit();      // PORTA_05: <- P07.UMUT_MODE_DBG1

  hash << _SYS_PIN_RSTp.commit();
  hash << _SYS_PIN_T2n.commit();
  hash << _SYS_PIN_T1n.commit();

  return hash;
}

//-----------------------------------------------------------------------------

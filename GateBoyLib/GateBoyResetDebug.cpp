#include "GateBoyLib/GateBoyResetDebug.h"

#include "GateBoyLib/GateBoyClocks.h"

//------------------------------------------------------------------------------------------------------------------------

void GateBoyResetDebug::tock(const GateBoyClock& clk, wire2 sys_clkreq, wire2 sys_clkgood, wire2 UPOF_DIV15p) {

  /* p01.AFER*/ AFER_SYS_RSTp.dff13(clk.BOGA_Axxxxxxx(), UPOJ_MODE_PRODn(), ASOL_POR_DONEn.qp_old());

  /* p01.UPYF*/ wire2 _UPYF = or2(PIN71_RST.int_qp_new(), clk.UCOB_CLKBADp());
  /* p01.TUBO*/ TUBO_WAITINGp.nor_latch(_UPYF, sys_clkreq);
  /* p01.UNUT*/ wire2 _UNUT_POR_TRIGn = and2(TUBO_WAITINGp.qp_new(), UPOF_DIV15p);
  /* p01.TABA*/ wire2 _TABA_POR_TRIGn = or3(UNOR_MODE_DBG2p(), UMUT_MODE_DBG1p(), _UNUT_POR_TRIGn);
  /*#p01.ALYP*/ wire2 _ALYP_RSTn = not1(_TABA_POR_TRIGn);
  /*#p01.AFAR*/ wire2 _AFAR_RSTp  = nor2(PIN71_RST.int_qp_new(), _ALYP_RSTn);
  /* p01.ASOL*/ ASOL_POR_DONEn.nor_latch(PIN71_RST.int_qp_new(), _AFAR_RSTp); // Schematic wrong, this is a latch.

  SIG_CPU_EXT_CLKGOOD.set(sys_clkgood);
  SIG_CPU_EXT_RESETp.set(PIN71_RST.int_qp_new());
  SIG_CPU_STARTp.set(_TABA_POR_TRIGn);
  SIG_CPU_INT_RESETp.set(AFER_SYS_RSTp.qp_new());

  /*#p25.SYCY*/ wire2 _SYCY_MODE_DBG2n = not1(UNOR_MODE_DBG2p());
  /*#p25.SOTO*/ SOTO_DBG_VRAMp.dff17(_SYCY_MODE_DBG2n, CUNU_SYS_RSTn(), SOTO_DBG_VRAMp.qn_old());
}

//------------------------------------------------------------------------------------------------------------------------

void GateBoyResetDebug::set_signals(wire2 XONA_LCDC_LCDENp)
{
  _XONA_LCDC_LCDENp.set(XONA_LCDC_LCDENp);
}

//------------------------------------------------------------------------------------------------------------------------

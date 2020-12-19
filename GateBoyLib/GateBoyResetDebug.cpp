#include "GateBoyLib/GateBoyResetDebug.h"

#include "GateBoyLib/GateBoyClocks.h"

//------------------------------------------------------------------------------------------------------------------------

void GateBoyResetDebug::tock(const GateBoyClock& clk, wire sys_clkreq, wire sys_clkgood, wire UPOF_DIV15p) {

  /* p01.AFER*/ AFER_SYS_RSTp.dff13(clk.BOGA_Axxxxxxx(), UPOJ_MODE_PRODn(), ASOL_POR_DONEn.qp_old());

  /* p01.UPYF*/ wire _UPYF = or2(PIN_SYS_RST.qp_new(), clk.UCOB_CLKBADp());
  /* p01.TUBO*/ TUBO_WAITINGp.nor_latch(_UPYF, sys_clkreq);
  /* p01.UNUT*/ wire _UNUT_POR_TRIGn = and2(TUBO_WAITINGp.qp_new(), UPOF_DIV15p);
  /* p01.TABA*/ wire _TABA_POR_TRIGn = or3(UNOR_MODE_DBG2p(), UMUT_MODE_DBG1p(), _UNUT_POR_TRIGn);
  /*#p01.ALYP*/ wire _ALYP_RSTn = not1(_TABA_POR_TRIGn);
  /*#p01.AFAR*/ wire _AFAR_RSTp  = nor2(PIN_SYS_RST.qp_new(), _ALYP_RSTn);
  /* p01.ASOL*/ ASOL_POR_DONEn.nor_latch(PIN_SYS_RST.qp_new(), _AFAR_RSTp); // Schematic wrong, this is a latch.

  PIN_CPU_EXT_CLKGOOD.pin_out(sys_clkgood);
  PIN_CPU_EXT_RST.pin_out(PIN_SYS_RST.qp_new());
  PIN_CPU_STARTp.pin_out(_TABA_POR_TRIGn);
  PIN_CPU_SYS_RSTp.pin_out(AFER_SYS_RSTp.qp_new());

  /*#p25.SYCY*/ wire _SYCY_MODE_DBG2n = not1(UNOR_MODE_DBG2p());
  /*#p25.SOTO*/ SOTO_DBG_VRAMp.dff17(_SYCY_MODE_DBG2n, CUNU_SYS_RSTn(), SOTO_DBG_VRAMp.qn_old());
}

//------------------------------------------------------------------------------------------------------------------------

void GateBoyResetDebug::set_signals(wire XONA_LCDC_LCDENp)
{
  _XONA_LCDC_LCDENp.set_new(XONA_LCDC_LCDENp);
}

//------------------------------------------------------------------------------------------------------------------------

#include "GateBoyLib/GateBoyResetDebug.h"

#include "GateBoyLib/GateBoyClocks.h"

void GateBoyResetDebug::tock(const GateBoyPhaseClock& pclk, wire sys_clkreq, wire sys_clkgood, wire UPOF_DIV15p) {

  /* p01.AFER*/ AFER_SYS_RSTp_evn.dff13(pclk.BOGA_Axxxxxxx(), UPOJ_MODE_PRODn_ext(), ASOL_POR_DONEn.qp_old());

  /* p01.UPYF*/ wire _UPYF_ext = or2(PIN_SYS_RST.qp_new(), pclk.UCOB_CLKBADp());
  /* p01.TUBO*/ TUBO_WAITINGp.nor_latch(_UPYF_ext, sys_clkreq);
  /* p01.UNUT*/ wire _UNUT_POR_TRIGn_new = and2(TUBO_WAITINGp.qp_new(), UPOF_DIV15p);
  /* p01.TABA*/ wire _TABA_POR_TRIGn_new = or3(UNOR_MODE_DBG2p(), UMUT_MODE_DBG1p_ext(), _UNUT_POR_TRIGn_new);
  /*#p01.ALYP*/ wire _ALYP_RSTn_new = not1(_TABA_POR_TRIGn_new);
  /*#p01.AFAR*/ wire _AFAR_RSTp_new  = nor2(PIN_SYS_RST.qp_new(), _ALYP_RSTn_new);
  /* p01.ASOL*/ ASOL_POR_DONEn.nor_latch(PIN_SYS_RST.qp_new(), _AFAR_RSTp_new); // Schematic wrong, this is a latch.

  PIN_CPU_EXT_CLKGOOD.setp(sys_clkgood);
  PIN_CPU_EXT_RST.setp(PIN_SYS_RST.qp_new());
  PIN_CPU_STARTp.setp(_TABA_POR_TRIGn_new);
  PIN_CPU_SYS_RSTp.setp(AFER_SYS_RSTp_evn.qp_new());

  /*#p25.SYCY*/ wire _SYCY_MODE_DBG2n_ext = not1(UNOR_MODE_DBG2p());
  /*#p25.SOTO*/ SOTO_DBG_VRAMp.dff17(_SYCY_MODE_DBG2n_ext, CUNU_SYS_RSTn(), SOTO_DBG_VRAMp.qn_any());
}

void GateBoyResetDebug::tock2(wire XONA_LCDC_LCDENp)
{
  _XONA_LCDC_LCDENp = XONA_LCDC_LCDENp;
}

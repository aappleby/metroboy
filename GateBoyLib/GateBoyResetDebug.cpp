#include "GateBoyLib/GateBoyResetDebug.h"

#include "GateBoyLib/GateBoyClocks.h"

//------------------------------------------------------------------------------------------------------------------------

void GateBoyResetDebug::tock(const GateBoyClock& clk, DFF17 UPOF_DIV15p) {

  /* p01.AFER*/ AFER_SYS_RSTp.dff13(clk.BOGA_Axxxxxxx(), UPOJ_MODE_PRODn(), ASOL_POR_DONEn.qp_old());

  /* p01.UPYF*/ wire _UPYF = or2(PIN_71_RST.qp_new(), clk.UCOB_CLKBADp());
  /* p01.TUBO*/ TUBO_WAITINGp.nor_latch(_UPYF, clk.SIG_CPU_CLKREQ);
  /* p01.UNUT*/ wire _UNUT_POR_TRIGn = and2(TUBO_WAITINGp.qp_new(), UPOF_DIV15p.qp_new());
  /* p01.TABA*/ wire _TABA_POR_TRIGn = or3(UNOR_MODE_DBG2p(), UMUT_MODE_DBG1p(), _UNUT_POR_TRIGn);
  /*#p01.ALYP*/ wire _ALYP_RSTn = not1(_TABA_POR_TRIGn);
  /*#p01.AFAR*/ wire _AFAR_RSTp  = nor2(PIN_71_RST.qp_new(), _ALYP_RSTn);
  /* p01.ASOL*/ ASOL_POR_DONEn.nor_latch(PIN_71_RST.qp_new(), _AFAR_RSTp); // Schematic wrong, this is a latch.

  /*SIG_CPU_EXT_CLKGOOD*/ SIG_CPU_EXT_CLKGOOD.sig_out(clk.PIN_74_CLK.clock_good());
  /*SIG_CPU_EXT_RESETp */ SIG_CPU_EXT_RESETp.sig_out(PIN_71_RST.qp_new());
  /*SIG_CPU_STARTp     */ SIG_CPU_STARTp.sig_out(_TABA_POR_TRIGn);
  /*SIG_CPU_INT_RESETp */ SIG_CPU_INT_RESETp.sig_out(AFER_SYS_RSTp.qp_new());

  /*#p25.SYCY*/ wire _SYCY_MODE_DBG2n = not1(UNOR_MODE_DBG2p());
  /*#p25.SOTO*/ SOTO_DBG_VRAMp.dff17(_SYCY_MODE_DBG2n, CUNU_SYS_RSTn(), SOTO_DBG_VRAMp.qn_old());

  // APET
  // APER
  // AMUT
  // BURO
}

//------------------------------------------------------------------------------------------------------------------------

void GateBoyResetDebug::set_signals(DFF9 XONA_LCDC_LCDENn)
{
  //_XONA_LCDC_LCDENp.set(XONA_LCDC_LCDENp);
  _XONA_LCDC_LCDENn = XONA_LCDC_LCDENn;

}

//------------------------------------------------------------------------------------------------------------------------

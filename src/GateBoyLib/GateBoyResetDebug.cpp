#include "GateBoyLib/GateBoyResetDebug.h"

#include "GateBoyLib/GateBoy.h"

//------------------------------------------------------------------------------------------------------------------------

void GateBoy::tock_reset_gates(DFF17 UPOF_DIV15p) {

  /*_p01.AFER*/ gbs.rst.AFER_SYS_RSTp.dff13(BOGA_Axxxxxxx(), UPOJ_MODE_PRODn(), gbs.rst.ASOL_POR_DONEn.qp_old());

  /*_p01.UPYF*/ wire UPYF = or2(gbs.rst.PIN_71_RST.qp_int_new(), UCOB_CLKBADp());
  /*_p01.TUBO*/ gbs.rst.TUBO_WAITINGp.nor_latch(UPYF, gbs.clk.SIG_CPU_CLKREQ.out_new());
  /*_p01.UNUT*/ wire UNUT_POR_TRIGn = and2(gbs.rst.TUBO_WAITINGp.qp_new(), UPOF_DIV15p.qp_new());
  /*_p01.TABA*/ wire TABA_POR_TRIGn = or3(UNOR_MODE_DBG2p(), UMUT_MODE_DBG1p(), UNUT_POR_TRIGn);
  /*#p01.ALYP*/ wire ALYP_RSTn = not1(TABA_POR_TRIGn);
  /*#p01.AFAR*/ wire AFAR_RSTp  = nor2(gbs.rst.PIN_71_RST.qp_int_new(), ALYP_RSTn);
  /*_p01.ASOL*/ gbs.rst.ASOL_POR_DONEn.nor_latch(gbs.rst.PIN_71_RST.qp_int_new(), AFAR_RSTp); // Schematic wrong, this is a latch.

  /*_SIG_CPU_EXT_CLKGOOD*/ gbs.rst.SIG_CPU_EXT_CLKGOOD.sig_out(gbs.clk.PIN_74_CLK.clkgood());
  /*_SIG_CPU_EXT_RESETp */ gbs.rst.SIG_CPU_EXT_RESETp.sig_out(gbs.rst.PIN_71_RST.qp_int_new());
  /*_SIG_CPU_STARTp     */ gbs.rst.SIG_CPU_STARTp.sig_out(TABA_POR_TRIGn);
  /*_SIG_CPU_INT_RESETp */ gbs.rst.SIG_CPU_INT_RESETp.sig_out(gbs.rst.AFER_SYS_RSTp.qp_new());

  /*#p25.SYCY*/ wire SYCY_MODE_DBG2n = not1(UNOR_MODE_DBG2p());
  /*#p25.SOTO*/ gbs.rst.SOTO_DBG_VRAMp.dff17(SYCY_MODE_DBG2n, CUNU_SYS_RSTn(), gbs.rst.SOTO_DBG_VRAMp.qn_old());

  // APET
  // APER
  // AMUT
  // BURO
}

//------------------------------------------------------------------------------------------------------------------------

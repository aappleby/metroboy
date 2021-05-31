#include "GateBoyLib/GateBoyResetDebug.h"

#include "GateBoyLib/GateBoy.h"

//------------------------------------------------------------------------------------------------------------------------

void GateBoy::tock_reset(DFF17 UPOF_DIV15p) {

  /* p01.AFER*/ rst.AFER_SYS_RSTp.dff13(BOGA_Axxxxxxx(), UPOJ_MODE_PRODn(), rst.ASOL_POR_DONEn.qp_old());

  /* p01.UPYF*/ wire _UPYF = or2(rst.PIN_71_RST.qp_int_new(), UCOB_CLKBADp());
  /* p01.TUBO*/ rst.TUBO_WAITINGp.nor_latch(_UPYF, clk.SIG_CPU_CLKREQ.qp_new());
  /* p01.UNUT*/ wire _UNUT_POR_TRIGn = and2(rst.TUBO_WAITINGp.qp_new(), UPOF_DIV15p.qp_new());
  /* p01.TABA*/ wire _TABA_POR_TRIGn = or3(UNOR_MODE_DBG2p(), UMUT_MODE_DBG1p(), _UNUT_POR_TRIGn);
  /*#p01.ALYP*/ wire _ALYP_RSTn = not1(_TABA_POR_TRIGn);
  /*#p01.AFAR*/ wire _AFAR_RSTp  = nor2(rst.PIN_71_RST.qp_int_new(), _ALYP_RSTn);
  /* p01.ASOL*/ rst.ASOL_POR_DONEn.nor_latch(rst.PIN_71_RST.qp_int_new(), _AFAR_RSTp); // Schematic wrong, this is a latch.

  /*SIG_CPU_EXT_CLKGOOD*/ rst.SIG_CPU_EXT_CLKGOOD.sig_out(clk.PIN_74_CLK.clock_good());
  /*SIG_CPU_EXT_RESETp */ rst.SIG_CPU_EXT_RESETp.sig_out(rst.PIN_71_RST.qp_int_new());
  /*SIG_CPU_STARTp     */ rst.SIG_CPU_STARTp.sig_out(_TABA_POR_TRIGn);
  /*SIG_CPU_INT_RESETp */ rst.SIG_CPU_INT_RESETp.sig_out(rst.AFER_SYS_RSTp.qp_new());

  /*#p25.SYCY*/ wire _SYCY_MODE_DBG2n = not1(UNOR_MODE_DBG2p());
  /*#p25.SOTO*/ rst.SOTO_DBG_VRAMp.dff17(_SYCY_MODE_DBG2n, CUNU_SYS_RSTn(), rst.SOTO_DBG_VRAMp.qn_old());

  // APET
  // APER
  // AMUT
  // BURO
}

//------------------------------------------------------------------------------------------------------------------------

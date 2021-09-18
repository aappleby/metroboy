#include "GateBoyLib/GateBoyReset.h"

#include "GateBoyLib/GateBoy.h"

//-----------------------------------------------------------------------------

void GateBoy::tock_reset_gates(DFF17 UPOF_DIV15p) {

  /*_p01.AFER*/ gb_state.sys_rst.AFER_SYS_RSTp.dff13(gb_state.sys_clk.BOGA_Axxxxxxx(), gb_state.pins.sys.UPOJ_MODE_PRODn(), gb_state.sys_rst.ASOL_POR_DONEn.qp_old());

  /*_p01.UPYF*/ wire UPYF = or2(gb_state.pins.sys.PIN_71_RST.qp_int_new(), gb_state.sys_clk.UCOB_CLKBADp());
  /*_p01.TUBO*/ gb_state.sys_rst.TUBO_WAITINGp.nor_latch(UPYF, gb_state.sys_clk.SIG_CPU_CLKREQ.out_new());
  /*_p01.UNUT*/ wire UNUT_POR_TRIGn = and2(gb_state.sys_rst.TUBO_WAITINGp.qp_new(), UPOF_DIV15p.qp_new());
  /*_p01.TABA*/ wire TABA_POR_TRIGn = or3(gb_state.pins.sys.UNOR_MODE_DBG2p(), gb_state.pins.sys.UMUT_MODE_DBG1p(), UNUT_POR_TRIGn);
  /*#p01.ALYP*/ wire ALYP_RSTn = not1(TABA_POR_TRIGn);
  /*#p01.AFAR*/ wire AFAR_RSTp  = nor2(gb_state.pins.sys.PIN_71_RST.qp_int_new(), ALYP_RSTn);
  /*_p01.ASOL*/ gb_state.sys_rst.ASOL_POR_DONEn.nor_latch(gb_state.pins.sys.PIN_71_RST.qp_int_new(), AFAR_RSTp); // Schematic wrong, this is a latch.

  /*_SIG_CPU_EXT_CLKGOOD*/ gb_state.sys_rst.SIG_CPU_EXT_CLKGOOD.sig_out(gb_state.sys_clk.PIN_74_CLK.clkgood());
  /*_SIG_CPU_EXT_RESETp */ gb_state.sys_rst.SIG_CPU_EXT_RESETp.sig_out(gb_state.pins.sys.PIN_71_RST.qp_int_new());
  /*_SIG_CPU_STARTp     */ gb_state.sys_rst.SIG_CPU_STARTp.sig_out(TABA_POR_TRIGn);
  /*_SIG_CPU_INT_RESETp */ gb_state.sys_rst.SIG_CPU_INT_RESETp.sig_out(gb_state.sys_rst.AFER_SYS_RSTp.qp_new());

  /*#p25.SYCY*/ wire SYCY_MODE_DBG2n = not1(gb_state.pins.sys.UNOR_MODE_DBG2p());
  /*#p25.SOTO*/ gb_state.sys_rst.SOTO_DBG_VRAMp.dff17(SYCY_MODE_DBG2n, gb_state.sys_rst.CUNU_SYS_RSTn(), gb_state.sys_rst.SOTO_DBG_VRAMp.qn_old());

  // APET
  // APER
  // AMUT
  // BURO
}

//-----------------------------------------------------------------------------

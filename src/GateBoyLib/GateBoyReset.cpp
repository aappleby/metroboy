#include "GateBoyLib/GateBoyReset.h"

#include "GateBoyLib/GateBoy.h"
#include "GateBoyLib/Gates.h"

//-----------------------------------------------------------------------------

void GateBoy::tock_reset_gates(const GateBoyState& reg_old, DFF17 UPOF_DIV15p) {
  auto& reg_new = gb_state;

  /*_p01.ABOL*/ wire ABOL_CLKREQn  = not1(reg_new.sys_clk.SIG_CPU_CLKREQ.out_new());
  /*#p01.AROV*/ wire AROV_xxCDEFxx = not1(reg_new.sys_clk.APUK_xxCDEFxx.qn_newB());
  /*#p01.AFEP*/ wire AFEP_AxxxxFGH = not1(reg_new.sys_clk.ALEF_xBCDExxx.qp_newB());
  /*#p01.ATYP*/ wire ATYP_ABCDxxxx = not1(reg_new.sys_clk.AFUR_ABCDxxxx.qn_newB());
  
  /*#p01.BELU*/ wire BELU_xxxxEFGH = nor2(ATYP_ABCDxxxx, ABOL_CLKREQn);
  /*#p01.BYRY*/ wire BYRY_ABCDxxxx = not1(BELU_xxxxEFGH);
  /*#p01.BUDE*/ wire BUDE_xxxxEFGH = not1(BYRY_ABCDxxxx);
  /*#p01.BAPY*/ wire BAPY_xxxxxxGH = nor3(ABOL_CLKREQn, AROV_xxCDEFxx, ATYP_ABCDxxxx);
  /*#p01.BERU*/ wire BERU_ABCDEFxx = not1(BAPY_xxxxxxGH);
  /*#p01.BUFA*/ wire BUFA_xxxxxxGH = not1(BERU_ABCDEFxx);
  /*#p01.BOLO*/ wire BOLO_ABCDEFxx = not1(BUFA_xxxxxxGH);
  /*#p01.BEKO*/ wire BEKO_ABCDxxxx = not1(BUDE_xxxxEFGH); // BEKO+BAVY parallel
  /*#p01.BEJA*/ wire BEJA_xxxxEFGH = nand4(BOLO_ABCDEFxx, BOLO_ABCDEFxx, BEKO_ABCDxxxx, BEKO_ABCDxxxx);
  /*#p01.BANE*/ wire BANE_ABCDxxxx = not1(BEJA_xxxxEFGH);
  /*#p01.BELO*/ wire BELO_xxxxEFGH = not1(BANE_ABCDxxxx);
  /*#p01.BAZE*/ wire BAZE_ABCDxxxx = not1(BELO_xxxxEFGH);
  /*#p01.BUTO*/ wire BUTO_xBCDEFGH = nand3(AFEP_AxxxxFGH, ATYP_ABCDxxxx, BAZE_ABCDxxxx);
  /*#p01.BELE*/ wire BELE_Axxxxxxx_new = not1(BUTO_xBCDEFGH);
  /*#p01.BYJU*/ wire BYJU_Axxxxxxx_new = or2(BELE_Axxxxxxx_new, pins.sys.ATEZ_CLKBADp_new());
  /*#p01.BALY*/ wire BALY_xBCDEFGH_new = not1(BYJU_Axxxxxxx_new);
  /*_p01.BOGA*/ wire BOGA_Axxxxxxx_new = not1(BALY_xBCDEFGH_new);

  /*_p01.AFER*/ reg_new.sys_rst.AFER_SYS_RSTp.dff13(BOGA_Axxxxxxx_new, pins.sys.UPOJ_MODE_PRODn_new(), reg_old.sys_rst.ASOL_POR_DONEn.qp_old());

  /*_p01.UPYF*/ wire UPYF_new = or2(pins.sys.PIN_71_RST.qp_int_new(), pins.sys.UCOB_CLKBADp_new());
  /*_p01.TUBO*/ reg_new.sys_rst.TUBO_WAITINGp.nor_latch(UPYF_new, reg_new.sys_clk.SIG_CPU_CLKREQ.out_new());
  /*_p01.UNUT*/ wire UNUT_POR_TRIGn_new = and2(reg_new.sys_rst.TUBO_WAITINGp.qp_new(), UPOF_DIV15p.qp_new());
  /*_p01.TABA*/ wire TABA_POR_TRIGn_new = or3(pins.sys.UNOR_MODE_DBG2p_new(), pins.sys.UMUT_MODE_DBG1p_new(), UNUT_POR_TRIGn_new);
  /*#p01.ALYP*/ wire ALYP_RSTn_new = not1(TABA_POR_TRIGn_new);
  /*#p01.AFAR*/ wire AFAR_RSTp_new  = nor2(pins.sys.PIN_71_RST.qp_int_new(), ALYP_RSTn_new);
  /*_p01.ASOL*/ reg_new.sys_rst.ASOL_POR_DONEn.nor_latch(pins.sys.PIN_71_RST.qp_int_new(), AFAR_RSTp_new); // Schematic wrong, this is a latch.

  /*_SIG_CPU_EXT_CLKGOOD*/ reg_new.sys_rst.SIG_CPU_EXT_CLKGOOD.sig_out(pins.sys.PIN_74_CLK.clkgood_new());
  /*_SIG_CPU_EXT_RESETp */ reg_new.sys_rst.SIG_CPU_EXT_RESETp.sig_out(pins.sys.PIN_71_RST.qp_int_new());
  /*_SIG_CPU_STARTp     */ reg_new.sys_rst.SIG_CPU_STARTp.sig_out(TABA_POR_TRIGn_new);
  /*_SIG_CPU_INT_RESETp */ reg_new.sys_rst.SIG_CPU_INT_RESETp.sig_out(reg_new.sys_rst.AFER_SYS_RSTp.qp_new());

  /*#p25.SYCY*/ wire SYCY_MODE_DBG2n_new = not1(pins.sys.UNOR_MODE_DBG2p_new());

  /*#p01.AVOR*/ wire AVOR_SYS_RSTp =  or2(reg_new.sys_rst.AFER_SYS_RSTp.qp_new(), reg_new.sys_rst.ASOL_POR_DONEn.qp_new());
  /*#p01.ALUR*/ wire ALUR_SYS_RSTn = not1(AVOR_SYS_RSTp);
  /*#p01.DULA*/ wire DULA_SYS_RSTp = not1(ALUR_SYS_RSTn);  /*#p01.CUNU*/ wire CUNU_SYS_RSTn = not1(DULA_SYS_RSTp);

  /*#p25.SOTO*/ reg_new.sys_rst.SOTO_DBG_VRAMp.dff17(SYCY_MODE_DBG2n_new, CUNU_SYS_RSTn, reg_old.sys_rst.SOTO_DBG_VRAMp.qn_old());
}

//-----------------------------------------------------------------------------

void GateBoyReset::reset() {
  TUBO_WAITINGp.state  = 0b00011000;
  ASOL_POR_DONEn.state = 0b00011000;
  AFER_SYS_RSTp.state  = 0b00011000;
  SOTO_DBG_VRAMp.state = 0b00011010;

  SIG_CPU_EXT_CLKGOOD.state = 0b00011001;
  SIG_CPU_EXT_RESETp.state  = 0b00011000;
  SIG_CPU_STARTp.state      = 0b00011000;
  SIG_CPU_INT_RESETp.state  = 0b00011000;
}

///*_p25.TUTO*/ wire TUTO_VRAM_DBGp()  const { return and2(UNOR_MODE_DBG2p_new(), SOTO_DBG_VRAMp.qn_new()); }

//-----------------------------------------------------------------------------

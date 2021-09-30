#include "GateBoyLib/GateBoyReset.h"

#include "GateBoyLib/GateBoy.h"
#include "GateBoyLib/Gates.h"

//-----------------------------------------------------------------------------

void GateBoy::tock_reset_gates(const GateBoyState& reg_old, DFF17 UPOF_DIV15p) {

  /*_p01.AFER*/ gb_state.sys_rst.AFER_SYS_RSTp.dff13(BOGA_Axxxxxxx(), pins.sys.UPOJ_MODE_PRODn(), gb_state.sys_rst.ASOL_POR_DONEn.qp_old());

  /*_p01.UPYF*/ wire UPYF = or2(pins.sys.PIN_71_RST.qp_int_new(), pins.sys.UCOB_CLKBADp());
  /*_p01.TUBO*/ gb_state.sys_rst.TUBO_WAITINGp.nor_latch(UPYF, gb_state.sys_clk.SIG_CPU_CLKREQ.out_new());
  /*_p01.UNUT*/ wire UNUT_POR_TRIGn = and2(gb_state.sys_rst.TUBO_WAITINGp.qp_new(), UPOF_DIV15p.qp_new());
  /*_p01.TABA*/ wire TABA_POR_TRIGn = or3(pins.sys.UNOR_MODE_DBG2p(), pins.sys.UMUT_MODE_DBG1p(), UNUT_POR_TRIGn);
  /*#p01.ALYP*/ wire ALYP_RSTn = not1(TABA_POR_TRIGn);
  /*#p01.AFAR*/ wire AFAR_RSTp  = nor2(pins.sys.PIN_71_RST.qp_int_new(), ALYP_RSTn);
  /*_p01.ASOL*/ gb_state.sys_rst.ASOL_POR_DONEn.nor_latch(pins.sys.PIN_71_RST.qp_int_new(), AFAR_RSTp); // Schematic wrong, this is a latch.

  /*_SIG_CPU_EXT_CLKGOOD*/ gb_state.sys_rst.SIG_CPU_EXT_CLKGOOD.sig_out(pins.sys.PIN_74_CLK.clkgood());
  /*_SIG_CPU_EXT_RESETp */ gb_state.sys_rst.SIG_CPU_EXT_RESETp.sig_out(pins.sys.PIN_71_RST.qp_int_new());
  /*_SIG_CPU_STARTp     */ gb_state.sys_rst.SIG_CPU_STARTp.sig_out(TABA_POR_TRIGn);
  /*_SIG_CPU_INT_RESETp */ gb_state.sys_rst.SIG_CPU_INT_RESETp.sig_out(gb_state.sys_rst.AFER_SYS_RSTp.qp_new());

  /*#p25.SYCY*/ wire SYCY_MODE_DBG2n = not1(pins.sys.UNOR_MODE_DBG2p());
  /*#p25.SOTO*/ gb_state.sys_rst.SOTO_DBG_VRAMp.dff17(SYCY_MODE_DBG2n, gb_state.sys_rst.CUNU_SYS_RSTn(), gb_state.sys_rst.SOTO_DBG_VRAMp.qn_old());

  // APET
  // APER
  // AMUT
  // BURO
}

//-----------------------------------------------------------------------------

void GateBoyReset::reset_to_poweron() {
  memset(this, BIT_OLD | BIT_DRIVEN, sizeof(*this));
}

void GateBoyReset::reset_to_bootrom() {
  TUBO_WAITINGp.state  = BIT_OLD | BIT_DRIVEN | BIT_DATA;
  ASOL_POR_DONEn.state = BIT_OLD | BIT_DRIVEN;
  AFER_SYS_RSTp.state  = BIT_OLD | BIT_DRIVEN | BIT_CLOCK;
  SOTO_DBG_VRAMp.state = BIT_OLD | BIT_DRIVEN | BIT_CLOCK;

  SIG_CPU_EXT_CLKGOOD.state = BIT_OLD | BIT_DRIVEN | BIT_DATA;
  SIG_CPU_EXT_RESETp.state  = BIT_OLD | BIT_DRIVEN;
  SIG_CPU_STARTp.state      = BIT_OLD | BIT_DRIVEN | BIT_DATA;
  SIG_CPU_INT_RESETp.state  = BIT_OLD | BIT_DRIVEN;
}

void GateBoyReset::reset_to_cart() {
  TUBO_WAITINGp.state  = 0b00011000;
  ASOL_POR_DONEn.state = 0b00011000;
  AFER_SYS_RSTp.state  = 0b00011010;
  SOTO_DBG_VRAMp.state = 0b00011010;

  SIG_CPU_EXT_CLKGOOD.state = 0b00011001;
  SIG_CPU_EXT_RESETp.state  = 0b00011000;
  SIG_CPU_STARTp.state      = 0b00011000;
  SIG_CPU_INT_RESETp.state  = 0b00011000;
}

///*_p25.TUTO*/ wire TUTO_VRAM_DBGp()  const { return and2(UNOR_MODE_DBG2p(), SOTO_DBG_VRAMp.qn_new()); }

/*#p01.AVOR*/ wire GateBoyReset::AVOR_SYS_RSTp() const { return or2(AFER_SYS_RSTp.qp_new(), ASOL_POR_DONEn.qp_new()); }
/*#p01.ALUR*/ wire GateBoyReset::ALUR_SYS_RSTn() const { return not1(AVOR_SYS_RSTp()); }
/*#p01.DULA*/ wire GateBoyReset::DULA_SYS_RSTp() const { return not1(ALUR_SYS_RSTn()); }
/*#p01.CUNU*/ wire GateBoyReset::CUNU_SYS_RSTn() const { return not1(DULA_SYS_RSTp()); }
/*#p01.XORE*/ wire GateBoyReset::XORE_SYS_RSTp() const { return not1(CUNU_SYS_RSTn()); }
/*_p01.XEBE*/ wire GateBoyReset::XEBE_SYS_RSTn() const { return not1(XORE_SYS_RSTp()); }
/*#p01.WALU*/ wire GateBoyReset::WALU_SYS_RSTn() const { return not1(XORE_SYS_RSTp()); }
/*_p01.WESY*/ wire GateBoyReset::WESY_SYS_RSTn() const { return not1(XORE_SYS_RSTp()); }
/*_p01.XARE*/ wire GateBoyReset::XARE_SYS_RSTn() const { return not1(XORE_SYS_RSTp()); }
/*_p03.MULO*/ wire GateBoyReset::MULO_SYS_RSTn() const { return not1(ALUR_SYS_RSTn()); }

//-----------------------------------------------------------------------------

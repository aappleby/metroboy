#include "GateBoyLib/GateBoyReset.h"

#include "GateBoyLib/GateBoy.h"
#include "GateBoyLib/Gates.h"

//-----------------------------------------------------------------------------

void GateBoy::tock_reset_gates(const GateBoyState& reg_old, DFF17 UPOF_DIV15p) {
  auto& reg_new = gb_state;

  /*_p01.AFER*/ reg_new.sys_rst.AFER_SYS_RSTp.dff13(BOGA_Axxxxxxx_new(), pins.sys.UPOJ_MODE_PRODn_new(), reg_old.sys_rst.ASOL_POR_DONEn.qp_old());

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
  /*#p25.SOTO*/ reg_new.sys_rst.SOTO_DBG_VRAMp.dff17(SYCY_MODE_DBG2n_new, reg_new.sys_rst.CUNU_SYS_RSTn_new(), reg_old.sys_rst.SOTO_DBG_VRAMp.qn_old());

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
  TUBO_WAITINGp.state  = 0b00011001;
  ASOL_POR_DONEn.state = 0b00011000;
  AFER_SYS_RSTp.state  = 0b00011000;
  SOTO_DBG_VRAMp.state = 0b00011010;

  SIG_CPU_EXT_CLKGOOD.state = 0b00011001;
  SIG_CPU_EXT_RESETp.state  = 0b00011000;
  SIG_CPU_STARTp.state      = 0b00011001;
  SIG_CPU_INT_RESETp.state  = 0b00011000;
}

void GateBoyReset::reset_to_cart() {
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

wire GateBoyReset::AVOR_SYS_RSTp_old() const { /*#p01.AVOR*/ return or2(AFER_SYS_RSTp.qp_old(), ASOL_POR_DONEn.qp_old()); }
wire GateBoyReset::ALUR_SYS_RSTn_old() const { /*#p01.ALUR*/ return not1(AVOR_SYS_RSTp_old()); }
wire GateBoyReset::DULA_SYS_RSTp_old() const { /*#p01.DULA*/ return not1(ALUR_SYS_RSTn_old()); }
wire GateBoyReset::CUNU_SYS_RSTn_old() const { /*#p01.CUNU*/ return not1(DULA_SYS_RSTp_old()); }
wire GateBoyReset::XORE_SYS_RSTp_old() const { /*#p01.XORE*/ return not1(CUNU_SYS_RSTn_old()); }
wire GateBoyReset::XEBE_SYS_RSTn_old() const { /*_p01.XEBE*/ return not1(XORE_SYS_RSTp_old()); }
wire GateBoyReset::WALU_SYS_RSTn_old() const { /*#p01.WALU*/ return not1(XORE_SYS_RSTp_old()); }
wire GateBoyReset::WESY_SYS_RSTn_old() const { /*_p01.WESY*/ return not1(XORE_SYS_RSTp_old()); }
wire GateBoyReset::XARE_SYS_RSTn_old() const { /*_p01.XARE*/ return not1(XORE_SYS_RSTp_old()); }
wire GateBoyReset::MULO_SYS_RSTn_old() const { /*_p03.MULO*/ return not1(ALUR_SYS_RSTn_old()); }

wire GateBoyReset::AVOR_SYS_RSTp_new() const { /*#p01.AVOR*/ return or2(AFER_SYS_RSTp.qp_new(), ASOL_POR_DONEn.qp_new()); }
wire GateBoyReset::ALUR_SYS_RSTn_new() const { /*#p01.ALUR*/ return not1(AVOR_SYS_RSTp_new()); }
wire GateBoyReset::DULA_SYS_RSTp_new() const { /*#p01.DULA*/ return not1(ALUR_SYS_RSTn_new()); }
wire GateBoyReset::CUNU_SYS_RSTn_new() const { /*#p01.CUNU*/ return not1(DULA_SYS_RSTp_new()); }
wire GateBoyReset::XORE_SYS_RSTp_new() const { /*#p01.XORE*/ return not1(CUNU_SYS_RSTn_new()); }
wire GateBoyReset::XEBE_SYS_RSTn_new() const { /*_p01.XEBE*/ return not1(XORE_SYS_RSTp_new()); }
wire GateBoyReset::WALU_SYS_RSTn_new() const { /*#p01.WALU*/ return not1(XORE_SYS_RSTp_new()); }
wire GateBoyReset::WESY_SYS_RSTn_new() const { /*_p01.WESY*/ return not1(XORE_SYS_RSTp_new()); }
wire GateBoyReset::XARE_SYS_RSTn_new() const { /*_p01.XARE*/ return not1(XORE_SYS_RSTp_new()); }
wire GateBoyReset::MULO_SYS_RSTn_new() const { /*_p03.MULO*/ return not1(ALUR_SYS_RSTn_new()); }

//-----------------------------------------------------------------------------

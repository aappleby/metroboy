#pragma once
#include "GateBoyLib/Gates.h"

//------------------------------------------------------------------------------------------------------------------------

struct GateBoyResetDebug {
  void reset_to_cart() {
    PIN_71_RST.reset(0);
    PIN_77_T1.reset(0);
    PIN_76_T2.reset(0);

    TUBO_WAITINGp.reset(0);
    ASOL_POR_DONEn.reset(0);
    AFER_SYS_RSTp.reset(1, 0);
    SOTO_DBG_VRAMp.reset(1, 0);

    _XONA_LCDC_LCDENn.reset(0, 0);
    SIG_CPU_EXT_CLKGOOD.reset(1);
    SIG_CPU_EXT_RESETp.reset(0);
    SIG_CPU_STARTp.reset(0);
    SIG_CPU_INT_RESETp.reset(0);
  }

  /*#p01.AVOR*/ wire AVOR_SYS_RSTp() const { return or2(AFER_SYS_RSTp.qp_new(), ASOL_POR_DONEn.qp_new()); }
  /*#p01.ALUR*/ wire ALUR_SYS_RSTn() const { return not1(AVOR_SYS_RSTp()); }
  /*#p01.DULA*/ wire DULA_SYS_RSTp() const { return not1(ALUR_SYS_RSTn()); }
  /*#p01.CUNU*/ wire CUNU_SYS_RSTn() const { return not1(DULA_SYS_RSTp()); }
  /*#p01.XORE*/ wire XORE_SYS_RSTp() const { return not1(CUNU_SYS_RSTn()); }
  /* p01.XEBE*/ wire XEBE_SYS_RSTn() const { return not1(XORE_SYS_RSTp()); }
  /*#p01.WALU*/ wire WALU_SYS_RSTn() const { return not1(XORE_SYS_RSTp()); }
  /* p01.WESY*/ wire WESY_SYS_RSTn() const { return not1(XORE_SYS_RSTp()); }
  /* p01.XARE*/ wire XARE_SYS_RSTn() const { return not1(XORE_SYS_RSTp()); }
  /* p03.MULO*/ wire MULO_SYS_RSTn() const { return not1(ALUR_SYS_RSTn()); }

  /* p01.XODO*/ wire XODO_VID_RSTp() const { return nand2(XEBE_SYS_RSTn(), _XONA_LCDC_LCDENn.qn_new()); }
  /* p01.XAPO*/ wire XAPO_VID_RSTn() const { return not1(XODO_VID_RSTp()); }
  /* p01.LYHA*/ wire LYHA_VID_RSTp() const { return not1(XAPO_VID_RSTn()); }
  /* p01.LYFE*/ wire LYFE_VID_RSTn() const { return not1(LYHA_VID_RSTp()); }
  /* p01.TOFU*/ wire TOFU_VID_RSTp() const { return not1(XAPO_VID_RSTn()); }
  /* p01.ROSY*/ wire ROSY_VID_RSTp() const { return not1(XAPO_VID_RSTn()); }
  /*#p01.ATAR*/ wire ATAR_VID_RSTp() const { return not1(XAPO_VID_RSTn()); }
  /*#p01.ABEZ*/ wire ABEZ_VID_RSTn() const { return not1(ATAR_VID_RSTp()); }
  /* p01.PYRY*/ wire PYRY_VID_RSTp() const { return not1(XAPO_VID_RSTn()); }
  /* p01.AMYG*/ wire AMYG_VID_RSTp() const { return not1(XAPO_VID_RSTn()); }

  /* p07.UBET*/ wire UBETp()           const { return not1(PIN_77_T1.qp_new()); }
  /* p07.UVAR*/ wire UVARp()           const { return not1(PIN_76_T2.qp_new()); }
  /* p07.UMUT*/ wire UMUT_MODE_DBG1p() const { return and2(PIN_77_T1.qp_new(), UVARp()); }
  /* p07.UNOR*/ wire UNOR_MODE_DBG2p() const { return and2(PIN_76_T2.qp_new(), UBETp()); }
  /* p07.UPOJ*/ wire UPOJ_MODE_PRODn() const { return nand3(UBETp(), UVARp(), PIN_71_RST.qp_new()); }
  /* p08.RYCA*/ wire RYCA_MODE_DBG2n() const { return not1(UNOR_MODE_DBG2p()); }
  /* p08.TOVA*/ wire TOVA_MODE_DBG2n() const { return not1(UNOR_MODE_DBG2p()); }
  /* p08.MULE*/ wire MULE_MODE_DBG1n() const { return not1(UMUT_MODE_DBG1p()); }
  /* p25.TUTO*/ wire TUTO_VRAM_DBGp()  const { return and2(UNOR_MODE_DBG2p(), SOTO_DBG_VRAMp.qn_new()); }

  void tock(const GateBoyClock& clk, DFF17 UPOF_DIV15p);
  void set_signals(DFF9 XONA_LCDC_LCDENp);

  void dump(Dumper& d) {
    d.dump_bitp("TUBO_WAITINGp  : ", TUBO_WAITINGp.state);
    d.dump_bitn("ASOL_POR_DONEn : ", ASOL_POR_DONEn.state);
    d.dump_bitp("AFER_SYS_RSTp  : ", AFER_SYS_RSTp.state);
  }

  //----------------------------------------

  // This is here because it controls the reset signals for all the graphics stuff.
  DFF9 _XONA_LCDC_LCDENn;  // xxxxxxxH

  /*PIN_71*/ PinIn  PIN_71_RST;
  /*PIN_77*/ PinIn  PIN_77_T1;
  /*PIN_76*/ PinIn  PIN_76_T2;

  /*p01.TUBO*/ NorLatch TUBO_WAITINGp;  // Must be 0 in run mode, otherwise we'd ping SIG_CPU_INT_RESETp when UPOF_DIV_15 changed
  /*p01.ASOL*/ NorLatch ASOL_POR_DONEn; // Schematic wrong, this is a latch.
  /*p01.AFER*/ DFF13 AFER_SYS_RSTp;     // AFER should keep clocking even if SIG_CPU_CLKREQ = 0
  /*p25.SOTO*/ DFF17 SOTO_DBG_VRAMp;

  /*SIG_CPU_EXT_CLKGOOD*/ SigOut SIG_CPU_EXT_CLKGOOD;   // top center port PORTC_03: <- chip.CLKIN_A top wire on PAD_XI,
  /*SIG_CPU_EXT_RESETp*/  SigOut SIG_CPU_EXT_RESETp;    // top center port PORTC_02: <- PIN_71_RST directly connected to the pad
  /*SIG_CPU_STARTp*/      SigOut SIG_CPU_STARTp;        // top center port PORTC_04: <- P01.CPU_RESET
  /*SIG_CPU_INT_RESETp*/  SigOut SIG_CPU_INT_RESETp;    // top center port PORTC_01: <- P01.AFER , reset related state
};

//------------------------------------------------------------------------------------------------------------------------


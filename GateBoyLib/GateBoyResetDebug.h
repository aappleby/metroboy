#pragma once
#include "GateBoyLib/Gates.h"

//------------------------------------------------------------------------------------------------------------------------

struct GateBoyResetDebug {
  void reset_to_cart() {
    PIN71_RST.reset(0);
    PIN77_T1.reset(0);
    PIN76_T2.reset(0);

    TUBO_WAITINGp.reset(0);
    ASOL_POR_DONEn.reset(0);
    AFER_SYS_RSTp.reset(1, 0);
    SOTO_DBG_VRAMp.reset(1, 0);

    _XONA_LCDC_LCDENp.reset(1);
    SIG_CPU_EXT_CLKGOOD.reset(1);
    SIG_CPU_EXT_RESETp.reset(0);
    SIG_CPU_STARTp.reset(0);
    SIG_CPU_INT_RESETp.reset(0);
  }

  /*#p01.AVOR*/ wire2 AVOR_SYS_RSTp() const { return or2(AFER_SYS_RSTp.qp_new(), ASOL_POR_DONEn.qp_new()); }
  /*#p01.ALUR*/ wire2 ALUR_SYS_RSTn() const { return not1(AVOR_SYS_RSTp()); }
  /*#p01.DULA*/ wire2 DULA_SYS_RSTp() const { return not1(ALUR_SYS_RSTn()); }
  /*#p01.CUNU*/ wire2 CUNU_SYS_RSTn() const { return not1(DULA_SYS_RSTp()); }
  /*#p01.XORE*/ wire2 XORE_SYS_RSTp() const { return not1(CUNU_SYS_RSTn()); }
  /* p01.XEBE*/ wire2 XEBE_SYS_RSTn() const { return not1(XORE_SYS_RSTp()); }
  /*#p01.WALU*/ wire2 WALU_SYS_RSTn() const { return not1(XORE_SYS_RSTp()); }
  /* p01.WESY*/ wire2 WESY_SYS_RSTn() const { return not1(XORE_SYS_RSTp()); }
  /* p01.XARE*/ wire2 XARE_SYS_RSTn() const { return not1(XORE_SYS_RSTp()); }
  /* p03.MULO*/ wire2 MULO_SYS_RSTn() const { return not1(ALUR_SYS_RSTn()); }

  /* p01.XODO*/ wire2 XODO_VID_RSTp() const { return nand2(XEBE_SYS_RSTn(), _XONA_LCDC_LCDENp.qp_new()); }
  /* p01.XAPO*/ wire2 XAPO_VID_RSTn() const { return not1(XODO_VID_RSTp()); }
  /* p01.LYHA*/ wire2 LYHA_VID_RSTp() const { return not1(XAPO_VID_RSTn()); }
  /* p01.LYFE*/ wire2 LYFE_VID_RSTn() const { return not1(LYHA_VID_RSTp()); }
  /* p01.TOFU*/ wire2 TOFU_VID_RSTp() const { return not1(XAPO_VID_RSTn()); }
  /* p01.ROSY*/ wire2 ROSY_VID_RSTp() const { return not1(XAPO_VID_RSTn()); }
  /*#p01.ATAR*/ wire2 ATAR_VID_RSTp() const { return not1(XAPO_VID_RSTn()); }
  /*#p01.ABEZ*/ wire2 ABEZ_VID_RSTn() const { return not1(ATAR_VID_RSTp()); }
  /* p01.PYRY*/ wire2 PYRY_VID_RSTp() const { return not1(XAPO_VID_RSTn()); }
  /* p01.AMYG*/ wire2 AMYG_VID_RSTp() const { return not1(XAPO_VID_RSTn()); }

  /* p07.UBET*/ wire2 UBETp()           const { return not1(PIN77_T1.int_qp_new()); }
  /* p07.UVAR*/ wire2 UVARp()           const { return not1(PIN76_T2.int_qp_new()); }
  /* p07.UMUT*/ wire2 UMUT_MODE_DBG1p() const { return and2(PIN77_T1.int_qp_new(), UVARp()); }
  /* p07.UNOR*/ wire2 UNOR_MODE_DBG2p() const { return and2(PIN76_T2.int_qp_new(), UBETp()); }
  /* p07.UPOJ*/ wire2 UPOJ_MODE_PRODn() const { return nand3(UBETp(), UVARp(), PIN71_RST.int_qp_new()); }
  /* p08.RYCA*/ wire2 RYCA_MODE_DBG2n() const { return not1(UNOR_MODE_DBG2p()); }
  /* p08.TOVA*/ wire2 TOVA_MODE_DBG2n() const { return not1(UNOR_MODE_DBG2p()); }
  /* p08.MULE*/ wire2 MULE_MODE_DBG1n() const { return not1(UMUT_MODE_DBG1p()); }
  /* p25.TUTO*/ wire2 TUTO_VRAM_DBGp()  const { return and2(UNOR_MODE_DBG2p(), SOTO_DBG_VRAMp.qn_new()); }

  void tock(const GateBoyClock& clk, wire2 sys_clkreq, wire2 sys_clkgood, wire2 UPOF_DIV15p);
  void set_signals(wire2 XONA_LCDC_LCDENp);

  void dump(Dumper& d) {
    d.dump_bitp("TUBO_WAITINGp  : ", TUBO_WAITINGp.state);
    d.dump_bitn("ASOL_POR_DONEn : ", ASOL_POR_DONEn.state);
    d.dump_bitp("AFER_SYS_RSTp  : ", AFER_SYS_RSTp.state);
  }

  //----------------------------------------

  PinIn  PIN71_RST;
  PinIn  PIN77_T1;
  PinIn  PIN76_T2;

  /*p01.TUBO*/ NorLatch TUBO_WAITINGp;  // Must be 0 in run mode, otherwise we'd ping SIG_CPU_INT_RESETp when UPOF_DIV_15 changed
  /*p01.ASOL*/ NorLatch ASOL_POR_DONEn; // Schematic wrong, this is a latch.
  /*p01.AFER*/ DFF13 AFER_SYS_RSTp;     // AFER should keep clocking even if SIG_CPU_CLKREQ = 0
  /*p25.SOTO*/ DFF17 SOTO_DBG_VRAMp;

  // This is here because it controls the reset signals for all the graphics stuff.
  /*p23.XONA*/ Signal _XONA_LCDC_LCDENp;  // xxxxxxxH

  Signal SIG_CPU_EXT_CLKGOOD;   // top center port PORTC_03: <- chip.CLKIN_A top wire2 on PAD_XI,
  Signal SIG_CPU_EXT_RESETp;    // top center port PORTC_02: <- PIN71_RST directly connected to the pad
  Signal SIG_CPU_STARTp;        // top center port PORTC_04: <- P01.CPU_RESET
  Signal SIG_CPU_INT_RESETp;    // top center port PORTC_01: <- P01.AFER , reset related state
};

//------------------------------------------------------------------------------------------------------------------------


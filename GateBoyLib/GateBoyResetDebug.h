#pragma once
#include "GateBoyLib/Gates.h"

//-----------------------------------------------------------------------------

struct GateBoyResetDebug {
  void reset_to_cart() {
    PIN_CPU_STARTp.reset(REG_D0C0);
    TUBO_WAITINGp.reset(REG_D0C0);
  }

  /*#p01.AVOR*/ wire AVOR_SYS_RSTp() const { return or2(AFER_SYS_RSTp.qp(), ASOL_POR_DONEn.qp()); }
  /*#p01.ALUR*/ wire ALUR_SYS_RSTn() const { return not1(AVOR_SYS_RSTp()); }
  /*#p01.DULA*/ wire DULA_SYS_RSTp() const { return not1(ALUR_SYS_RSTn()); }
  /*#p01.CUNU*/ wire CUNU_SYS_RSTn() const { return not1(DULA_SYS_RSTp()); }
  /*#p01.XORE*/ wire XORE_SYS_RSTp() const { return not1(CUNU_SYS_RSTn()); }
  /* p01.XEBE*/ wire XEBE_SYS_RSTn() const { return not1(XORE_SYS_RSTp()); }
  /*#p01.WALU*/ wire WALU_SYS_RSTn() const { return not1(XORE_SYS_RSTp()); }
  /* p01.WESY*/ wire WESY_SYS_RSTn() const { return not1(XORE_SYS_RSTp()); }
  /* p01.XARE*/ wire XARE_SYS_RSTn() const { return not1(XORE_SYS_RSTp()); }
  /* p03.MULO*/ wire MULO_SYS_RSTn() const { return not1(ALUR_SYS_RSTn()); }

  /* p01.XODO*/ wire XODO_VID_RSTp() const { return nand2(XEBE_SYS_RSTn(), _XONA_LCDC_LCDENp.qp()); }
  /* p01.XAPO*/ wire XAPO_VID_RSTn() const { return not1(XODO_VID_RSTp()); }
  /* p01.LYHA*/ wire LYHA_VID_RSTp() const { return not1(XAPO_VID_RSTn()); }
  /* p01.LYFE*/ wire LYFE_VID_RSTn() const { return not1(LYHA_VID_RSTp()); }
  /* p01.TOFU*/ wire TOFU_VID_RSTp() const { return not1(XAPO_VID_RSTn()); }
  /* p01.ROSY*/ wire ROSY_VID_RSTp() const { return not1(XAPO_VID_RSTn()); }
  /*#p01.ATAR*/ wire ATAR_VID_RSTp() const { return not1(XAPO_VID_RSTn()); }
  /*#p01.ABEZ*/ wire ABEZ_VID_RSTn() const { return not1(ATAR_VID_RSTp()); }
  /* p01.PYRY*/ wire PYRY_VID_RSTp() const { return not1(XAPO_VID_RSTn()); }
  /* p01.AMYG*/ wire AMYG_VID_RSTp() const { return not1(XAPO_VID_RSTn()); }

  /* p07.UBET*/ wire UBETp()           const { return not1(PIN_SYS_T1.qp()); }
  /* p07.UVAR*/ wire UVARp()           const { return not1(PIN_SYS_T2.qp()); }
  /* p07.UMUT*/ wire UMUT_MODE_DBG1p() const { return and2(PIN_SYS_T1.qp(), UVARp()); }
  /* p07.UNOR*/ wire UNOR_MODE_DBG2p() const { return and2(PIN_SYS_T2.qp(), UBETp()); }
  /* p07.UPOJ*/ wire UPOJ_MODE_PRODn() const { return nand3(UBETp(), UVARp(), PIN_SYS_RST.qp()); }
  /* p08.RYCA*/ wire RYCA_MODE_DBG2n() const { return not1(UNOR_MODE_DBG2p()); }
  /* p08.TOVA*/ wire TOVA_MODE_DBG2n() const { return not1(UNOR_MODE_DBG2p()); }
  /* p08.MULE*/ wire MULE_MODE_DBG1n() const { return not1(UMUT_MODE_DBG1p()); }
  /* p25.TUTO*/ wire TUTO_VRAM_DBGp()  const { return and2(UNOR_MODE_DBG2p(), SOTO_DBG_VRAMp.qn_new()); }

  void tock(const GateBoyClock& clk, wire sys_clkreq, wire sys_clkgood, wire UPOF_DIV15p);
  void tock(wire XONA_LCDC_LCDENp);

  void dump(Dumper& d) {
    d.dump_bitp("TUBO_WAITINGp ", TUBO_WAITINGp.state);
    d.dump_bitn("ASOL_POR_DONEn", ASOL_POR_DONEn.state);
    d.dump_bitp("AFER_SYS_RSTp ", AFER_SYS_RSTp.state);
  }

  //----------------------------------------

  /*p01.TUBO*/ NorLatch TUBO_WAITINGp;  // Must be 0 in run mode, otherwise we'd ping PIN_CPU_DBG_RST when UPOF_DIV_15 changed
  /*p01.ASOL*/ NorLatch ASOL_POR_DONEn; // Schematic wrong, this is a latch.
  /*p01.AFER*/ DFF13 AFER_SYS_RSTp; // AFER should keep clocking even if PIN_CPU_CLKREQ = 0
  /*p25.SOTO*/ DFF17 SOTO_DBG_VRAMp;

  // This is here because it controls the reset signals for all the graphics stuff.
  /*p23.XONA*/ Signal _XONA_LCDC_LCDENp;  // xxxxxxxH

  PinIn PIN_SYS_RST;
  PinIn PIN_SYS_T1;
  PinIn PIN_SYS_T2;

  PinOut PIN_CPU_EXT_CLKGOOD;   // top center port PORTC_03: <- chip.CLKIN_A top wire on PAD_XI,
  PinOut PIN_CPU_EXT_RST;       // top center port PORTC_02: <- PIN_RESET directly connected to the pad
  PinOut PIN_CPU_STARTp;        // top center port PORTC_04: <- P01.CPU_RESET
  PinOut PIN_CPU_SYS_RSTp;      // top center port PORTC_01: <- P01.AFER , reset related state
};

//-----------------------------------------------------------------------------

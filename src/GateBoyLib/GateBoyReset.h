#pragma once
#include "GateBoyLib/Gates.h"

//-----------------------------------------------------------------------------

struct GateBoyReset {
  void reset_to_poweron() {
    memset(this, BIT_OLD | BIT_DRIVEN, sizeof(*this));
  }

  void reset_to_bootrom() {
    TUBO_WAITINGp.state  = BIT_OLD | BIT_DRIVEN | BIT_DATA;
    ASOL_POR_DONEn.state = BIT_OLD | BIT_DRIVEN;
    AFER_SYS_RSTp.state  = BIT_OLD | BIT_DRIVEN | BIT_CLOCK;
    SOTO_DBG_VRAMp.state = BIT_OLD | BIT_DRIVEN | BIT_CLOCK;

    SIG_CPU_EXT_CLKGOOD.state = BIT_OLD | BIT_DRIVEN | BIT_DATA;
    SIG_CPU_EXT_RESETp.state  = BIT_OLD | BIT_DRIVEN;
    SIG_CPU_STARTp.state      = BIT_OLD | BIT_DRIVEN | BIT_DATA;
    SIG_CPU_INT_RESETp.state  = BIT_OLD | BIT_DRIVEN;
  }

  void reset_to_cart() {
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

  /*#p01.AVOR*/ wire AVOR_SYS_RSTp() const { return or2(AFER_SYS_RSTp.qp_new(), ASOL_POR_DONEn.qp_new()); }
  /*#p01.ALUR*/ wire ALUR_SYS_RSTn() const { return not1(AVOR_SYS_RSTp()); }
  /*#p01.DULA*/ wire DULA_SYS_RSTp() const { return not1(ALUR_SYS_RSTn()); }
  /*#p01.CUNU*/ wire CUNU_SYS_RSTn() const { return not1(DULA_SYS_RSTp()); }
  /*#p01.XORE*/ wire XORE_SYS_RSTp() const { return not1(CUNU_SYS_RSTn()); }
  /*_p01.XEBE*/ wire XEBE_SYS_RSTn() const { return not1(XORE_SYS_RSTp()); }
  /*#p01.WALU*/ wire WALU_SYS_RSTn() const { return not1(XORE_SYS_RSTp()); }
  /*_p01.WESY*/ wire WESY_SYS_RSTn() const { return not1(XORE_SYS_RSTp()); }
  /*_p01.XARE*/ wire XARE_SYS_RSTn() const { return not1(XORE_SYS_RSTp()); }
  /*_p03.MULO*/ wire MULO_SYS_RSTn() const { return not1(ALUR_SYS_RSTn()); }

  //----------------------------------------

  /*_p01.TUBO*/ NorLatch TUBO_WAITINGp;  // Must be 0 in run mode, otherwise we'd ping SIG_CPU_INT_RESETp when UPOF_DIV_15 changed
  /*_p01.ASOL*/ NorLatch ASOL_POR_DONEn; // Schematic wrong, this is a latch.
  /*_p01.AFER*/ DFF13 AFER_SYS_RSTp;     // AFER should keep clocking even if SIG_CPU_CLKREQ = 0
  /*#p25.SOTO*/ DFF17 SOTO_DBG_VRAMp;

  /*_SIG_CPU_EXT_CLKGOOD*/ SigOut SIG_CPU_EXT_CLKGOOD;   // top center port PORTC_03: <- chip.CLKIN_A top wire on PAD_XI,
  /*_SIG_CPU_EXT_RESETp */ SigOut SIG_CPU_EXT_RESETp;    // top center port PORTC_02: <- PIN_71_RST directly connected to the pad
  /*_SIG_CPU_STARTp     */ SigOut SIG_CPU_STARTp;        // top center port PORTC_04: <- P01.CPU_RESET
  /*_SIG_CPU_INT_RESETp */ SigOut SIG_CPU_INT_RESETp;    // top center port PORTC_01: <- P01.AFER , reset related state
};

//-----------------------------------------------------------------------------


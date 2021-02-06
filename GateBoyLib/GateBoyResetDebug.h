#pragma once
#include "GateBoyLib/Gates.h"

//------------------------------------------------------------------------------------------------------------------------

struct GateBoyResetDebug {
  void reset_to_cart() {
    PIN_71_RST.state = 0b00011000;
    PIN_77_T1.state = 0b00011000;
    PIN_76_T2.state = 0b00011000;
    TUBO_WAITINGp.state = 0b00011000;
    ASOL_POR_DONEn.state = 0b00011000;
    AFER_SYS_RSTp.state = 0b00011010;
    SOTO_DBG_VRAMp.state = 0b00011010;
    SIG_CPU_EXT_CLKGOOD.state = 0b00011001;
    SIG_CPU_EXT_RESETp.state = 0b00011000;
    SIG_CPU_STARTp.state = 0b00011000;
    SIG_CPU_INT_RESETp.state = 0b00011000;
  }

  //----------------------------------------

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


#pragma once

#include "GateBoyLib/Gates.h"
#include "GateBoyLib/Regs.h"

//-----------------------------------------------------------------------------

struct GateBoyReset {
  void reset();

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


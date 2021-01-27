#pragma once
#include "GateBoyLib/Gates.h"

#include "GateBoyLib/GateBoyRegisters.h"

//------------------------------------------------------------------------------------------------------------------------

struct GateBoyOamBus {
  void reset_to_cart() {
    MAKA_LATCH_EXTp.state  = 0b00011010;
    WUJE_CPU_OAM_WRn.state = 0b00011001;
    SIG_OAM_CLKn.state  = 0b00011001;
    SIG_OAM_WRn_A.state = 0b00011001;
    SIG_OAM_WRn_B.state = 0b00011001;
    SIG_OAM_OEn.state   = 0b00011001;
    old_oam_clk.state   = 0b00011000;

  }

  /*p04.MAKA*/ DFF17 MAKA_LATCH_EXTp;       // AxxxExxx
  /*p28.WUJE*/ NorLatch WUJE_CPU_OAM_WRn;   // AxxxExxx

  /*SIG_OAM_CLKn */ SigOut SIG_OAM_CLKn;   // ABCDEFGH
  /*SIG_OAM_WRn_A*/ SigOut SIG_OAM_WRn_A;  // AxxxExxH
  /*SIG_OAM_WRn_B*/ SigOut SIG_OAM_WRn_B;  // AxxxExxH
  /*SIG_OAM_OEn  */ SigOut SIG_OAM_OEn;    // ABCDEFGH

  Gate old_oam_clk;
};

//------------------------------------------------------------------------------------------------------------------------

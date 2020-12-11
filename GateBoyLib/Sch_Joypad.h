#pragma once
#include "GateBoyLib/Gates.h"

//-----------------------------------------------------------------------------

struct JoypadRegisters {

  void reset_cart() {
    AWOB_WAKE_CPU.reset(0);

    BATU_JP_GLITCH0.reset(REG_D0C1);
    ACEF_JP_GLITCH1.reset(REG_D0C1);
    AGEM_JP_GLITCH2.reset(REG_D0C1);
    APUG_JP_GLITCH3.reset(REG_D0C1);

    JUTE_JOYP_RA.reset(REG_D0C1);
    KECY_JOYP_LB.reset(REG_D0C1);
    JALE_JOYP_UC.reset(REG_D0C1);
    KYME_JOYP_DS.reset(REG_D0C1);
    KELY_JOYP_UDLRp.reset(REG_D0C1);
    COFY_JOYP_ABCSp.reset(REG_D0C1);
    KUKO_DBG_FF00_D6n.reset(REG_D0C1);
    KERU_DBG_FF00_D7n.reset(REG_D0C1);

    KEVU_JOYP_L0n.reset(0);
    KAPA_JOYP_L1n.reset(0);
    KEJA_JOYP_L2n.reset(0);
    KOLO_JOYP_L3n.reset(0);
  }

//private:
  friend struct GateBoy;

  // This is driven by what we think is a latch and it goes straight to the CPU - maybe there's a pull-down?
  /*p02.AWOB*/ TpLatch AWOB_WAKE_CPU;

  /*p02.BATU*/ DFF17 BATU_JP_GLITCH0;
  /*p02.ACEF*/ DFF17 ACEF_JP_GLITCH1;
  /*p02.AGEM*/ DFF17 AGEM_JP_GLITCH2;
  /*p02.APUG*/ DFF17 APUG_JP_GLITCH3;

  /*p05.JUTE*/ DFF17 JUTE_JOYP_RA;
  /*p05.KECY*/ DFF17 KECY_JOYP_LB;
  /*p05.JALE*/ DFF17 JALE_JOYP_UC;
  /*p05.KYME*/ DFF17 KYME_JOYP_DS;
  /*p05.KELY*/ DFF17 KELY_JOYP_UDLRp;
  /*p05.COFY*/ DFF17 COFY_JOYP_ABCSp;
  /*p05.KUKO*/ DFF17 KUKO_DBG_FF00_D6n;
  /*p05.KERU*/ DFF17 KERU_DBG_FF00_D7n;

  /*p05.KEVU*/ TpLatch KEVU_JOYP_L0n; // 10-rung, looks like pass gate or something
  /*p05.KAPA*/ TpLatch KAPA_JOYP_L1n; // 10-rung, looks like pass gate or something
  /*p05.KEJA*/ TpLatch KEJA_JOYP_L2n; // 10-rung, looks like pass gate or something
  /*p05.KOLO*/ TpLatch KOLO_JOYP_L3n; // 10-rung, looks like pass gate or something
};

//-----------------------------------------------------------------------------

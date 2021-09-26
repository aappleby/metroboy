#pragma once
#include "GateBoyLib/Regs.h"

//-----------------------------------------------------------------------------

struct JoyInt {
  void reset_to_poweron();
  void reset_to_bootrom();
  void reset_to_cart();

  /*_p02.BATU*/ DFF17 BATU_JP_GLITCH0;
  /*_p02.ACEF*/ DFF17 ACEF_JP_GLITCH1;
  /*_p02.AGEM*/ DFF17 AGEM_JP_GLITCH2;
  /*#p02.APUG*/ DFF17 APUG_JP_GLITCH3;
};

//-----------------------------------------------------------------------------

struct RegJoy {
  void reset_to_poweron();
  void reset_to_bootrom();
  void reset_to_cart();

  // Ignoring debug stuff for now
  ///*_p05.JUTE*/ DFF17 JUTE_DBG_D0;
  ///*_p05.KECY*/ DFF17 KECY_DBG_D1;
  ///*_p05.JALE*/ DFF17 JALE_DBG_D2;
  ///*_p05.KYME*/ DFF17 KYME_DBG_D3;
  /*#p05.KELY*/ DFF17 KELY_JOYP_UDLRp; // these two _must_ be 1 on reset or we read button states before we've written these regs
  /*#p05.COFY*/ DFF17 COFY_JOYP_ABCSp; // or wait am i getting polarity wrong?
  ///*_p05.KUKO*/ DFF17 KUKO_DBG_D6;
  ///*_p05.KERU*/ DFF17 KERU_DBG_D7;
};

//-----------------------------------------------------------------------------

struct JoyLatch {
  void reset_to_poweron();
  void reset_to_bootrom();
  void reset_to_cart();

  /*#p05.KEVU*/ TpLatch KEVU_JOYP_L0n;
  /*#p05.KAPA*/ TpLatch KAPA_JOYP_L1n;
  /*#p05.KEJA*/ TpLatch KEJA_JOYP_L2n;
  /*#p05.KOLO*/ TpLatch KOLO_JOYP_L3n;
};

//-----------------------------------------------------------------------------

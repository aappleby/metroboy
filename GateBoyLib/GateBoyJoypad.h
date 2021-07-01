#pragma once
#include "GateBoyLib/Gates.h"

//------------------------------------------------------------------------------------------------------------------------

struct GateBoyJoypad {
  // This is driven by what we think is a latch and it goes straight to the CPU - maybe there's a pull-down?
  /*p02.AWOB*/ TpLatch AWOB_WAKE_CPU;

  /*p02.BATU*/ DFF17 BATU_JP_GLITCH0;
  /*p02.ACEF*/ DFF17 ACEF_JP_GLITCH1;
  /*p02.AGEM*/ DFF17 AGEM_JP_GLITCH2;
  /*p02.APUG*/ DFF17 APUG_JP_GLITCH3;

  // Ignoring debug stuff for now
  ///*p05.JUTE*/ DFF17 JUTE_DBG_D0;
  ///*p05.KECY*/ DFF17 KECY_DBG_D1;
  ///*p05.JALE*/ DFF17 JALE_DBG_D2;
  ///*p05.KYME*/ DFF17 KYME_DBG_D3;

  // these two _must_ be 1 on reset or we read button states before we've written these regs

  /*p05.KELY*/ DFF17 KELY_JOYP_UDLRp;
  /*p05.COFY*/ DFF17 COFY_JOYP_ABCSp;

  ///*p05.KUKO*/ DFF17 KUKO_DBG_D6;
  ///*p05.KERU*/ DFF17 KERU_DBG_D7;

  /*p05.KEVU*/ TpLatch KEVU_JOYP_L0n;
  /*p05.KAPA*/ TpLatch KAPA_JOYP_L1n;
  /*p05.KEJA*/ TpLatch KEJA_JOYP_L2n;
  /*p05.KOLO*/ TpLatch KOLO_JOYP_L3n;

  /*SIG_CPU_WAKE*/ SigOut SIG_CPU_WAKE;  // top right wire by itself <- P02.AWOB

  // Pressing a button pulls the corresponding pin _down_.
  /*PIN_67*/ PinIn  PIN_67_JOY_P10;
  /*PIN_66*/ PinIn  PIN_66_JOY_P11;
  /*PIN_65*/ PinIn  PIN_65_JOY_P12;
  /*PIN_64*/ PinIn  PIN_64_JOY_P13;
  /*PIN_63*/ PinOut PIN_63_JOY_P14;
  /*PIN_62*/ PinOut PIN_62_JOY_P15;
};

//------------------------------------------------------------------------------------------------------------------------

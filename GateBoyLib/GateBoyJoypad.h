#pragma once
#include "GateBoyLib/Gates.h"

//-----------------------------------------------------------------------------

struct GateBoyJoypad {

  void tock(
    Signal BUS_CPU_A[16],
    Signal BUS_CPU_D[8],
    wire AVOR_SYS_RSTp,
    wire BOGA_Axxxxxxx_clkevn,
    wire TEDO_CPU_RDp,
    wire TAPU_CPU_WRp,
    BusOut BUS_CPU_D_out[8]);

  /* p02.ASOK*/ wire ASOK_INT_JOYp() const { return and2(APUG_JP_GLITCH3.qp_new(), BATU_JP_GLITCH0.qp_new()); }

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

  PinOut PIN_CPU_WAKE;          // top right wire by itself <- P02.AWOB

  PinIn  PIN_JOY_P10; // PIN_67   Pressing a button pulls the corresponding pin _down_.
  PinIn  PIN_JOY_P11; // PIN_66
  PinIn  PIN_JOY_P12; // PIN_65
  PinIn  PIN_JOY_P13; // PIN_64
  PinOut PIN_JOY_P14; // PIN_63
  PinOut PIN_JOY_P15; // PIN_62
};

//-----------------------------------------------------------------------------

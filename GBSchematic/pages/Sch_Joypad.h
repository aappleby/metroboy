#pragma once
#include "Sch_Common.h"

namespace Schematics {

struct JoypadPins;

//-----------------------------------------------------------------------------

struct JoypadSignals {
  /*p02.ASOK*/ bool INT_JP;
  /*p02.AWOB*/ bool WAKE_CPU;
};

//-----------------------------------------------------------------------------

struct JoypadRegisters {
  void pwron();
  void reset();
  void phase_begin();
  void phase_end();
  void pass_begin();
  bool pass_end();

  JoypadSignals tick(const ClkSignals& clk,
                     const RstSignals& rst_sig,
                     const BusSignals& bus_ctl,
                     const DebugSignals& dbg_sig,
                     const DecoderSignals& dec_sig,
                     JoypadPins& joy_pins,
                     BusTristates& bus_tri);

  /*p02.BATU*/ Reg2 JP_GLITCH0;
  /*p02.ACEF*/ Reg2 JP_GLITCH1;
  /*p02.AGEM*/ Reg2 JP_GLITCH2;
  /*p02.APUG*/ Reg2 JP_GLITCH3;

  /*p05.JUTE*/ Reg2 JOYP_RA;
  /*p05.KECY*/ Reg2 JOYP_LB;
  /*p05.JALE*/ Reg2 JOYP_UC;
  /*p05.KYME*/ Reg2 JOYP_DS;
  /*p05.KELY*/ Reg2 JOYP_UDLR;
  /*p05.COFY*/ Reg2 JOYP_ABCS;
  /*p05.KUKO*/ Reg2 DBG_FF00_D6;
  /*p05.KERU*/ Reg2 DBG_FF00_D7;

  /*p05.KEVU*/ Latch JOYP_L0;
  /*p05.KAPA*/ Latch JOYP_L1;
  /*p05.KEJA*/ Latch JOYP_L2;
  /*p05.KOLO*/ Latch JOYP_L3;
  /*p02.AWOB*/ Latch WAKE_CPU;
};

//-----------------------------------------------------------------------------

};
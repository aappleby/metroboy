#pragma once
#include "Sch_Common.h"

namespace Schematics {

struct ResetSignals;
struct ClockSignals;
struct CpuBus;
struct CpuBusSignals;

//-----------------------------------------------------------------------------

struct JoypadRegisters {

  void tick(SchematicTop& gb);
  SignalHash commit();

private:
  friend struct SchematicTop;

  /*p02.BATU*/ Reg17 JP_GLITCH0;
  /*p02.ACEF*/ Reg17 JP_GLITCH1;
  /*p02.AGEM*/ Reg17 JP_GLITCH2;
  /*p02.APUG*/ Reg17 JP_GLITCH3;
  /*p05.JUTE*/ Reg17 JOYP_RA;
  /*p05.KECY*/ Reg17 JOYP_LB;
  /*p05.JALE*/ Reg17 JOYP_UC;
  /*p05.KYME*/ Reg17 JOYP_DS;
  /*p05.KELY*/ Reg17 JOYP_UDLR;
  /*p05.COFY*/ Reg17 JOYP_ABCS;
  /*p05.KUKO*/ Reg17 DBG_FF00_D6;
  /*p05.KERU*/ Reg17 DBG_FF00_D7;

  /*p05.KEVU*/ Reg JOYP_L0;       // 10-rung, looks like pass gate or something
  /*p05.KAPA*/ Reg JOYP_L1;       // 10-rung, looks like pass gate or something
  /*p05.KEJA*/ Reg JOYP_L2;       // 10-rung, looks like pass gate or something
  /*p05.KOLO*/ Reg JOYP_L3;       // 10-rung, looks like pass gate or something
  /*p02.AWOB*/ Reg AWOB_WAKE_CPU; // 10-rung, looks like pass gate or something
};

//-----------------------------------------------------------------------------

}; // namespace Schematics
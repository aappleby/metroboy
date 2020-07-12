#pragma once
#include "Sch_Common.h"

namespace Schematics {

struct SchematicTop;
struct ResetSignals;
struct ClockSignals;
struct CpuBus;
struct CpuBusSignals;

//-----------------------------------------------------------------------------

struct JoypadRegisters {

  void tick(SchematicTop& top);
  void tock(SchematicTop& top);
  SignalHash commit(SchematicTop& top);

private:
  friend struct SchematicTop;

  /*p02.AWOB*/ TpLatch AWOB_WAKE_CPU; // 10-rung, looks like pass gate or something

  /*p02.BATU*/ Reg17 BATU_JP_GLITCH0;
  /*p02.ACEF*/ Reg17 ACEF_JP_GLITCH1;
  /*p02.AGEM*/ Reg17 AGEM_JP_GLITCH2;
  /*p02.APUG*/ Reg17 APUG_JP_GLITCH3;
  
  /*p05.JUTE*/ Reg17 JUTE_JOYP_RA;
  /*p05.KECY*/ Reg17 KECY_JOYP_LB;
  /*p05.JALE*/ Reg17 JALE_JOYP_UC;
  /*p05.KYME*/ Reg17 KYME_JOYP_DS;

  /*p05.KELY*/ Reg17 KELY_JOYP_UDLR;
  /*p05.COFY*/ Reg17 COFY_JOYP_ABCS;
  /*p05.KUKO*/ Reg17 KUKO_DBG_FF00_D6;
  /*p05.KERU*/ Reg17 KERU_DBG_FF00_D7;

  /*p05.KEVU*/ TpLatch KEVU_JOYP_L0; // 10-rung, looks like pass gate or something
  /*p05.KAPA*/ TpLatch KAPA_JOYP_L1; // 10-rung, looks like pass gate or something
  /*p05.KEJA*/ TpLatch KEJA_JOYP_L2; // 10-rung, looks like pass gate or something
  /*p05.KOLO*/ TpLatch KOLO_JOYP_L3; // 10-rung, looks like pass gate or something

  CpuPinOut CPU_PIN_WAKE;          // top right wire by itself <- P02.AWOB
};

//-----------------------------------------------------------------------------

}; // namespace Schematics
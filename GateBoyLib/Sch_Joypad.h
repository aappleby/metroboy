#pragma once
#include "GateBoyLib/Gates.h"

namespace Schematics {

struct SchematicTop;
struct CpuBus;

//-----------------------------------------------------------------------------

struct Joypad {

  void reset() {}
  void reset_bootrom() {}
  void tick(const SchematicTop& top);
  void tock(const SchematicTop& top, CpuBus& cpu_bus);
  void dump(Dumper& d) const;
  void set_buttons(uint8_t buttons);

  /*p02.ASOK*/ Sig ASOK_INT_JOYp;

//private:
  friend struct SchematicTop;

  // This is driven by what we think is a latch and it goes straight to the CPU - maybe there's a pull-down?
  /*p02.AWOB*/ TpLatch AWOB_WAKE_CPU = TRI_D0NP;
  Pin PIN_CPU_WAKE = TRI_HZNP; // top right wire by itself <- P02.AWOB

  /*p02.BATU*/ DFF17 BATU_JP_GLITCH0;
  /*p02.ACEF*/ DFF17 ACEF_JP_GLITCH1;
  /*p02.AGEM*/ DFF17 AGEM_JP_GLITCH2;
  /*p02.APUG*/ DFF17 APUG_JP_GLITCH3;

  /*p05.JUTE*/ DFF17 JUTE_JOYP_RA;
  /*p05.KECY*/ DFF17 KECY_JOYP_LB;
  /*p05.JALE*/ DFF17 JALE_JOYP_UC;
  /*p05.KYME*/ DFF17 KYME_JOYP_DS;
  /*p05.KELY*/ DFF17 KELY_JOYP_UDLR;
  /*p05.COFY*/ DFF17 COFY_JOYP_ABCS;
  /*p05.KUKO*/ DFF17 KUKO_DBG_FF00_D6;
  /*p05.KERU*/ DFF17 KERU_DBG_FF00_D7;

  /*p05.KEVU*/ TpLatch KEVU_JOYP_L0 = TRI_D0NP; // 10-rung, looks like pass gate or something
  /*p05.KAPA*/ TpLatch KAPA_JOYP_L1 = TRI_D0NP; // 10-rung, looks like pass gate or something
  /*p05.KEJA*/ TpLatch KEJA_JOYP_L2 = TRI_D0NP; // 10-rung, looks like pass gate or something
  /*p05.KOLO*/ TpLatch KOLO_JOYP_L3 = TRI_D0NP; // 10-rung, looks like pass gate or something

  Pin PIN_JOY_P10 = TRI_HZPU; // PIN_67
  Pin PIN_JOY_P11 = TRI_HZPU; // PIN_66
  Pin PIN_JOY_P12 = TRI_HZPU; // PIN_65
  Pin PIN_JOY_P13 = TRI_HZPU; // PIN_64
  Pin PIN_JOY_P14 = TRI_HZPU; // PIN_63
  Pin PIN_JOY_P15 = TRI_HZPU; // PIN_62
};

//-----------------------------------------------------------------------------

}; // namespace Schematics
#pragma once
#include "GateBoyLib/Cells.h"

namespace Schematics {

struct SchematicTop;
struct CpuBus;

//-----------------------------------------------------------------------------

struct Joypad {

  void tick(const SchematicTop& top);
  void tock(const SchematicTop& top, CpuBus& cpu_bus);
  void dump(Dumper& d) const;
  void preset_buttons(uint8_t buttons);

  /*p02.ASOK*/ Sig ASOK_INT_JOYp;

private:
  friend struct SchematicTop;

  // This is driven by what we think is a latch and it goes straight to the CPU - maybe there's a pull-down?
  /*p02.AWOB*/ Tri AWOB_WAKE_CPU = TRI_D0NP;
  Tri PIN_CPU_WAKE = TRI_HZNP; // top right wire by itself <- P02.AWOB

  /*p02.BATU*/ RegQP BATU_JP_GLITCH0  = REG_D0C0;
  /*p02.ACEF*/ RegQP ACEF_JP_GLITCH1  = REG_D0C0;
  /*p02.AGEM*/ RegQP AGEM_JP_GLITCH2  = REG_D0C0;
  /*p02.APUG*/ RegQP APUG_JP_GLITCH3  = REG_D0C0;

  /*p05.JUTE*/ RegQP JUTE_JOYP_RA     = REG_D0C0;
  /*p05.KECY*/ RegQP KECY_JOYP_LB     = REG_D0C0;
  /*p05.JALE*/ RegQP JALE_JOYP_UC     = REG_D0C0;
  /*p05.KYME*/ RegQP KYME_JOYP_DS     = REG_D0C0;
  /*p05.KELY*/ RegQN KELY_JOYP_UDLR   = REG_D0C0;
  /*p05.COFY*/ RegQN COFY_JOYP_ABCS   = REG_D0C0;
  /*p05.KUKO*/ RegQP KUKO_DBG_FF00_D6 = REG_D0C0;
  /*p05.KERU*/ RegQP KERU_DBG_FF00_D7 = REG_D0C0;

  /*p05.KEVU*/ Tri KEVU_JOYP_L0     = TRI_D0NP; // 10-rung, looks like pass gate or something
  /*p05.KAPA*/ Tri KAPA_JOYP_L1     = TRI_D0NP; // 10-rung, looks like pass gate or something
  /*p05.KEJA*/ Tri KEJA_JOYP_L2     = TRI_D0NP; // 10-rung, looks like pass gate or something
  /*p05.KOLO*/ Tri KOLO_JOYP_L3     = TRI_D0NP; // 10-rung, looks like pass gate or something

  Pin PIN_JOY_P10 = TRI_HZPU; // PIN_67
  Pin PIN_JOY_P11 = TRI_HZPU; // PIN_66
  Pin PIN_JOY_P12 = TRI_HZPU; // PIN_65
  Pin PIN_JOY_P13 = TRI_HZPU; // PIN_64
  Pin PIN_JOY_P14 = TRI_HZPU; // PIN_63
  Pin PIN_JOY_P15 = TRI_HZPU; // PIN_62
};

//-----------------------------------------------------------------------------

}; // namespace Schematics
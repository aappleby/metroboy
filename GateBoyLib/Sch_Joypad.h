#pragma once
#include "GateBoyLib/Cells.h"

namespace Schematics {

struct SchematicTop;
struct CpuBus;

//-----------------------------------------------------------------------------

struct Joypad {

  void tick(const SchematicTop& top);
  void tock(wire CLKGOOD, const SchematicTop& top, CpuBus& cpu_bus);
  void dump(Dumper& d) const;

  void preset_buttons(uint8_t buttons);
  /*p02.ASOK*/ wire ASOK_INT_JOYPADp() const { return and2(APUG_JP_GLITCH3.qp(), BATU_JP_GLITCH0.qp()); }

private:
  friend struct SchematicTop;

  // This is driven by what we think is a latch and it goes straight to the CPU - maybe there's a pull-down?
  /*p02.AWOB*/ Tri AWOB_WAKE_CPU    = TRI_D0NP;

  Tri CPU_PIN_WAKE  = TRI_HZNP;   // top right wire by itself <- P02.AWOB

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

  Tri JOY_PIN_P10_A = TRI_HZNP;   // PIN_67 <- P05.KOLE
  Tri JOY_PIN_P10_B = TRI_HZNP;   // PIN_67 <- tied low between BONE and BUFY
  Tri JOY_PIN_P10_C = TRI_D0NP;   // PIN_67 -> P02.KERY, P05.KEVU
  Tri JOY_PIN_P10_D = TRI_HZNP;   // PIN_67 <- P05.KYBU

  Tri JOY_PIN_P11_A = TRI_HZNP;   // PIN_66 <- P05.KYTO
  Tri JOY_PIN_P11_B = TRI_HZNP;   // PIN_66 <- tied low between BONE and BUFY
  Tri JOY_PIN_P11_C = TRI_D0NP;   // PIN_66 -> P02.KERY, P05.KAPA
  Tri JOY_PIN_P11_D = TRI_HZNP;   // PIN_66 <- P05.KABU

  Tri JOY_PIN_P12_A = TRI_HZNP;   // PIN_65 <- P05.KYHU
  Tri JOY_PIN_P12_B = TRI_HZNP;   // PIN_65 <- tied low between BONE and BUFY
  Tri JOY_PIN_P12_C = TRI_D0NP;   // PIN_65 -> P02.KERY, P05.KEJA
  Tri JOY_PIN_P12_D = TRI_HZNP;   // PIN_65 <- P05.KASY

  Tri JOY_PIN_P13_A = TRI_HZNP;   // PIN_64 <- P05.KORY
  Tri JOY_PIN_P13_B = TRI_HZNP;   // PIN_64 <- tied low between BONE and BUFY
  Tri JOY_PIN_P13_C = TRI_D0NP;   // PIN_64 -> P02.KERY, P05.KOLO
  Tri JOY_PIN_P13_D = TRI_HZNP;   // PIN_64 <- P05.KALE

  Tri JOY_PIN_P14_A = TRI_HZNP;   // PIN_63 <- p05.KARU
  Tri JOY_PIN_P14_D = TRI_HZNP;   // PIN_63 <- p05.KELY

  Tri JOY_PIN_P15_A = TRI_HZNP;   // PIN_62 <- p05.CELA
  Tri JOY_PIN_P15_D = TRI_HZNP;   // PIN_62 <- p05.COFY
};

//-----------------------------------------------------------------------------

}; // namespace Schematics
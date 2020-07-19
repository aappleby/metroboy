#pragma once
#include "Cells.h"

namespace Schematics {

struct SchematicTop;
struct CpuBus;

//-----------------------------------------------------------------------------

struct Joypad {

  Joypad();
  void tick(const SchematicTop& top);
  void tock(const SchematicTop& top, CpuBus& cpu_bus);
  uint64_t commit();

  void set_buttons(uint8_t buttons);
  /*p02.ASOK*/ wire ASOK_INT_JOYPADp() const { return and(APUG_JP_GLITCH3.q(), BATU_JP_GLITCH0.q()); }

private:
  friend struct SchematicTop;

  // This is driven by what we think is a latch and it goes straight to the CPU - maybe there's a pull-down?
  /*p02.AWOB*/ Pin2 AWOB_WAKE_CPU    = Pin2::HIZ_PD;

  /*p02.BATU*/ Reg2 BATU_JP_GLITCH0  = Reg2::D0C0;
  /*p02.ACEF*/ Reg2 ACEF_JP_GLITCH1  = Reg2::D0C0;
  /*p02.AGEM*/ Reg2 AGEM_JP_GLITCH2  = Reg2::D0C0;
  /*p02.APUG*/ Reg2 APUG_JP_GLITCH3  = Reg2::D0C0;
  
  /*p05.JUTE*/ Reg2 JUTE_JOYP_RA     = Reg2::D0C0;
  /*p05.KECY*/ Reg2 KECY_JOYP_LB     = Reg2::D0C0;
  /*p05.JALE*/ Reg2 JALE_JOYP_UC     = Reg2::D0C0;
  /*p05.KYME*/ Reg2 KYME_JOYP_DS     = Reg2::D0C0;

  /*p05.KELY*/ Reg2 KELY_JOYP_UDLR   = Reg2::D0C0;
  /*p05.COFY*/ Reg2 COFY_JOYP_ABCS   = Reg2::D0C0;
  /*p05.KUKO*/ Reg2 KUKO_DBG_FF00_D6 = Reg2::D0C0;
  /*p05.KERU*/ Reg2 KERU_DBG_FF00_D7 = Reg2::D0C0;

  /*p05.KEVU*/ Pin2 KEVU_JOYP_L0     = Pin2::LATCH_0; // 10-rung, looks like pass gate or something
  /*p05.KAPA*/ Pin2 KAPA_JOYP_L1     = Pin2::LATCH_0; // 10-rung, looks like pass gate or something
  /*p05.KEJA*/ Pin2 KEJA_JOYP_L2     = Pin2::LATCH_0; // 10-rung, looks like pass gate or something
  /*p05.KOLO*/ Pin2 KOLO_JOYP_L3     = Pin2::LATCH_0; // 10-rung, looks like pass gate or something

  Pin2 JOY_PIN_P10_A = Pin2::HIZ_NP;   // PIN_67 <- P05.KOLE
  Pin2 JOY_PIN_P10_B = Pin2::HIZ_NP;   // PIN_67 <- tied low between BONE and BUFY
  Pin2 JOY_PIN_P10_C = Pin2::HOLD_0;   // PIN_67 -> P02.KERY, P05.KEVU
  Pin2 JOY_PIN_P10_D = Pin2::HIZ_NP;   // PIN_67 <- P05.KYBU

  Pin2 JOY_PIN_P11_A = Pin2::HIZ_NP;   // PIN_66 <- P05.KYTO
  Pin2 JOY_PIN_P11_B = Pin2::HIZ_NP;   // PIN_66 <- tied low between BONE and BUFY
  Pin2 JOY_PIN_P11_C = Pin2::HOLD_0;   // PIN_66 -> P02.KERY, P05.KAPA
  Pin2 JOY_PIN_P11_D = Pin2::HIZ_NP;   // PIN_66 <- P05.KABU

  Pin2 JOY_PIN_P12_A = Pin2::HIZ_NP;   // PIN_65 <- P05.KYHU
  Pin2 JOY_PIN_P12_B = Pin2::HIZ_NP;   // PIN_65 <- tied low between BONE and BUFY
  Pin2 JOY_PIN_P12_C = Pin2::HOLD_0;   // PIN_65 -> P02.KERY, P05.KEJA
  Pin2 JOY_PIN_P12_D = Pin2::HIZ_NP;   // PIN_65 <- P05.KASY

  Pin2 JOY_PIN_P13_A = Pin2::HIZ_NP;   // PIN_64 <- P05.KORY
  Pin2 JOY_PIN_P13_B = Pin2::HIZ_NP;   // PIN_64 <- tied low between BONE and BUFY
  Pin2 JOY_PIN_P13_C = Pin2::HOLD_0;   // PIN_64 -> P02.KERY, P05.KOLO
  Pin2 JOY_PIN_P13_D = Pin2::HIZ_NP;   // PIN_64 <- P05.KALE

  Pin2 JOY_PIN_P14_A = Pin2::HIZ_NP;   // PIN_63 <- p05.KARU
  Pin2 JOY_PIN_P14_D = Pin2::HIZ_NP;   // PIN_63 <- p05.KELY

  Pin2 JOY_PIN_P15_A = Pin2::HIZ_NP;   // PIN_62 <- p05.CELA
  Pin2 JOY_PIN_P15_D = Pin2::HIZ_NP;   // PIN_62 <- p05.COFY

  Pin2 CPU_PIN_WAKE  = Pin2::HIZ_NP;   // top right wire by itself <- P02.AWOB
};

//-----------------------------------------------------------------------------

}; // namespace Schematics
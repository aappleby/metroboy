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
  SignalHash commit();

  void set_buttons(uint8_t buttons);
  /*p02.ASOK*/ wire ASOK_INT_JOYPADp() const { return and(APUG_JP_GLITCH3.q(), BATU_JP_GLITCH0.q()); }

private:
  friend struct SchematicTop;

  /*p02.AWOB*/ Reg AWOB_WAKE_CPU; // 10-rung, looks like pass gate or something

  /*p02.BATU*/ Reg BATU_JP_GLITCH0;
  /*p02.ACEF*/ Reg ACEF_JP_GLITCH1;
  /*p02.AGEM*/ Reg AGEM_JP_GLITCH2;
  /*p02.APUG*/ Reg APUG_JP_GLITCH3;
  
  /*p05.JUTE*/ Reg JUTE_JOYP_RA;
  /*p05.KECY*/ Reg KECY_JOYP_LB;
  /*p05.JALE*/ Reg JALE_JOYP_UC;
  /*p05.KYME*/ Reg KYME_JOYP_DS;

  /*p05.KELY*/ Reg KELY_JOYP_UDLR;
  /*p05.COFY*/ Reg COFY_JOYP_ABCS;
  /*p05.KUKO*/ Reg KUKO_DBG_FF00_D6;
  /*p05.KERU*/ Reg KERU_DBG_FF00_D7;

  /*p05.KEVU*/ Reg KEVU_JOYP_L0; // 10-rung, looks like pass gate or something
  /*p05.KAPA*/ Reg KAPA_JOYP_L1; // 10-rung, looks like pass gate or something
  /*p05.KEJA*/ Reg KEJA_JOYP_L2; // 10-rung, looks like pass gate or something
  /*p05.KOLO*/ Reg KOLO_JOYP_L3; // 10-rung, looks like pass gate or something

  Reg JOY_PIN_P10_A;   // PIN_67 <- P05.KOLE
  Reg JOY_PIN_P10_B;   // PIN_67 <- tied low between BONE and BUFY
  ExtPinIn  JOY_PIN_P10_C;   // PIN_67 -> P02.KERY, P05.KEVU
  Reg JOY_PIN_P10_D;   // PIN_67 <- P05.KYBU

  Reg JOY_PIN_P11_A;   // PIN_66 <- P05.KYTO
  Reg JOY_PIN_P11_B;   // PIN_66 <- tied low between BONE and BUFY
  ExtPinIn  JOY_PIN_P11_C;   // PIN_66 -> P02.KERY, P05.KAPA
  Reg JOY_PIN_P11_D;   // PIN_66 <- P05.KABU

  Reg JOY_PIN_P12_A;   // PIN_65 <- P05.KYHU
  Reg JOY_PIN_P12_B;   // PIN_65 <- tied low between BONE and BUFY
  ExtPinIn  JOY_PIN_P12_C;   // PIN_65 -> P02.KERY, P05.KEJA
  Reg JOY_PIN_P12_D;   // PIN_65 <- P05.KASY

  Reg JOY_PIN_P13_A;   // PIN_64 <- P05.KORY
  Reg JOY_PIN_P13_B;   // PIN_64 <- tied low between BONE and BUFY
  ExtPinIn  JOY_PIN_P13_C;   // PIN_64 -> P02.KERY, P05.KOLO
  Reg JOY_PIN_P13_D;   // PIN_64 <- P05.KALE

  Reg JOY_PIN_P14_A;   // PIN_63 <- p05.KARU
  Reg JOY_PIN_P14_D;   // PIN_63 <- p05.KELY

  Reg JOY_PIN_P15_A;   // PIN_62 <- p05.CELA
  Reg JOY_PIN_P15_D;   // PIN_62 <- p05.COFY

  Reg CPU_PIN_WAKE;          // top right wire by itself <- P02.AWOB
};

//-----------------------------------------------------------------------------

}; // namespace Schematics
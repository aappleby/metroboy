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

  ExtPinOut JOY_PIN_P10_A;   // PIN_67 <- P05.KOLE
  ExtPinOut JOY_PIN_P10_B;   // PIN_67 <- tied low between BONE and BUFY
  ExtPinIn  JOY_PIN_P10_C;   // PIN_67 -> P02.KERY, P05.KEVU
  ExtPinOut JOY_PIN_P10_D;   // PIN_67 <- P05.KYBU

  ExtPinOut JOY_PIN_P11_A;   // PIN_66 <- P05.KYTO
  ExtPinOut JOY_PIN_P11_B;   // PIN_66 <- tied low between BONE and BUFY
  ExtPinIn  JOY_PIN_P11_C;   // PIN_66 -> P02.KERY, P05.KAPA
  ExtPinOut JOY_PIN_P11_D;   // PIN_66 <- P05.KABU

  ExtPinOut JOY_PIN_P12_A;   // PIN_65 <- P05.KYHU
  ExtPinOut JOY_PIN_P12_B;   // PIN_65 <- tied low between BONE and BUFY
  ExtPinIn  JOY_PIN_P12_C;   // PIN_65 -> P02.KERY, P05.KEJA
  ExtPinOut JOY_PIN_P12_D;   // PIN_65 <- P05.KASY

  ExtPinOut JOY_PIN_P13_A;   // PIN_64 <- P05.KORY
  ExtPinOut JOY_PIN_P13_B;   // PIN_64 <- tied low between BONE and BUFY
  ExtPinIn  JOY_PIN_P13_C;   // PIN_64 -> P02.KERY, P05.KOLO
  ExtPinOut JOY_PIN_P13_D;   // PIN_64 <- P05.KALE

  ExtPinOut JOY_PIN_P14_A;   // PIN_63 <- p05.KARU
  ExtPinOut JOY_PIN_P14_D;   // PIN_63 <- p05.KELY

  ExtPinOut JOY_PIN_P15_A;   // PIN_62 <- p05.CELA
  ExtPinOut JOY_PIN_P15_D;   // PIN_62 <- p05.COFY

  CpuPinOut CPU_PIN_WAKE;          // top right wire by itself <- P02.AWOB
};

//-----------------------------------------------------------------------------

}; // namespace Schematics
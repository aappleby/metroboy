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

  // The B connections on the joypad pins are werid.
  // They seem to be used as an input, or at least I can't find the driver
  // PESU
  // RARU ROWE RYKE RYNE RASE REJY REKA ROMY
  // RUNY VYPO TOMY? SEZU? RAWU? PUTE? MYDE RUGO? NYLU WYMO?
  // WEFE WUWE GEFY WYGA? FABY ECAB? DYSO ERUC GEZE GUVA 
  // ARAR ATAJ ASUZ AJEC AKAJ ANOC BENU BEDA
  // BEKU

  /* PIN_67 */ PinOut P10_A;   // <- P05.KOLE
  /* PIN_67 */ PinOut P10_B;   // -> BENU BEDA ATAJ ASUZ AJEC AKAJ ANOC ARAR
  /* PIN_67 */ PinIn  P10_C;   // -> P02.KERY, P05.KEVU
  /* PIN_67 */ PinOut P10_D;   // <- P05.KYBU

  /* PIN_66 */ PinOut P11_A;   // <- P05.KYTO
  /* PIN_66 */ PinOut P11_B;   
  /* PIN_66 */ PinIn  P11_C;   // -> P02.KERY, P05.KAPA
  /* PIN_66 */ PinOut P11_D;   // <- P05.KABU

  /* PIN_65 */ PinOut P12_A;   // <- P05.KYHU
  /* PIN_65 */ PinOut P12_B;   
  /* PIN_65 */ PinIn  P12_C;   // -> P02.KERY, P05.KEJA
  /* PIN_65 */ PinOut P12_D;   // <- P05.KASY

  /* PIN_64 */ PinOut P13_A;   // <- P05.KORY
  /* PIN_64 */ PinOut P13_B;   
  /* PIN_64 */ PinIn  P13_C;   // -> P02.KERY, P05.KOLO
  /* PIN_64 */ PinOut P13_D;   // <- P05.KALE

  /* PIN_63 */ PinOut P14_A;   // <- p05.KARU
  /* PIN_63 */ PinOut P14_D;   // <- p05.KELY

  /* PIN_62 */ PinOut P15_A;   // <- p05.CELA
  /* PIN_62 */ PinOut P15_D;   // <- p05.COFY
};

//-----------------------------------------------------------------------------

}; // namespace Schematics
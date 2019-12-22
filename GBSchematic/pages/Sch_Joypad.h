#pragma once
#include "Sch_Common.h"

namespace Schematics {

//-----------------------------------------------------------------------------

struct Joypad {
  /*p02.ASOK*/ bool INT_JP;
  /*p02.AWOB*/ bool WAKE_CPU;

  /*p02.BATU*/ Reg JP_GLITCH0;
  /*p02.ACEF*/ Reg JP_GLITCH1;
  /*p02.AGEM*/ Reg JP_GLITCH2;
  /*p02.APUG*/ Reg JP_GLITCH3;


  /*p05.KEVU*/ bool JOYP_L0;
  /*p05.KAPA*/ bool JOYP_L1;
  /*p05.KEJA*/ bool JOYP_L2;
  /*p05.KOLO*/ bool JOYP_L3;

  /*p05.JUTE*/ Reg JOYP_RA;
  /*p05.KECY*/ Reg JOYP_LB;
  /*p05.JALE*/ Reg JOYP_UC;
  /*p05.KYME*/ Reg JOYP_DS;
  /*p05.KELY*/ Reg JOYP_UDLR;
  /*p05.COFY*/ Reg JOYP_ABCS;
  /*p05.KUKO*/ Reg DBG_FF00_D6;
  /*p05.KERU*/ Reg DBG_FF00_D7;

  //----------
  // Joypad pins

  /* PIN_67 */ bool PIN_P10_A;   // <- KOLE
  ///* PIN_67 */ bool P10_B;   // This pin is weird and seems to be used as an input, or at least I can't find the driver
  /* PIN_67 */ bool PIN_P10_C;   // -> P02.KERY, P05.KEVU
  /* PIN_67 */ bool PIN_P10_D;   // <- KYBU

  /* PIN_66 */ bool PIN_P11_A;   // <- P05.KYTO
  /* PIN_66 */ bool PIN_P11_B;   
  /* PIN_66 */ bool PIN_P11_C;   // -> P02.KERY, P05.KAPA
  /* PIN_66 */ bool PIN_P11_D;   // <- P05.KABU

  /* PIN_65 */ bool PIN_P12_A;   // <- P05.KYHU
  /* PIN_65 */ bool PIN_P12_B;   
  /* PIN_65 */ bool PIN_P12_C;   // -> P02.KERY, P05.KEJA
  /* PIN_65 */ bool PIN_P12_D;   // <- P05.KASY
                            
  /* PIN_64 */ bool PIN_P13_A;   // <- P05.KORY
  /* PIN_64 */ bool PIN_P13_B;   
  /* PIN_64 */ bool PIN_P13_C;   // -> P02.KERY, P05.KOLO
  /* PIN_64 */ bool PIN_P13_D;   // <- P05.KALE
                             
  /* PIN_63 */ bool PIN_P14_A;   // <- p05.KARU
  /* PIN_63 */ bool PIN_P14_D;   // <- p05.KELY

  /* PIN_62 */ bool PIN_P15_A;   // <- p05.CELA
  /* PIN_62 */ bool PIN_P15_D;   // <- p05.COFY
};

//-----------------------------------------------------------------------------

};
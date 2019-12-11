#pragma once
#include "Schematics.h"

namespace Schematics {

//-----------------------------------------------------------------------------

struct Joypad {
  /*p01.BOGA*/ bool BOGA_xBCDEFGH;

  /*p02.ASOK*/ bool INT_JP;
  /*p02.AWOB*/ bool TO_CPU2;

  /*p02.BATU*/ bool JP_GLITCH0;
  /*p02.ACEF*/ bool JP_GLITCH1;
  /*p02.AGEM*/ bool JP_GLITCH2;
  /*p02.APUG*/ bool JP_GLITCH3;


  /*p05.KEVU*/ bool JOYP_L0;
  /*p05.KAPA*/ bool JOYP_L1;
  /*p05.KEJA*/ bool JOYP_L2;
  /*p05.KOLO*/ bool JOYP_L3;

  /*p10.ATOZ*/ bool FF00_WRn;
  /*p05.JUTE*/ bool JOYP_RA;
  /*p05.KECY*/ bool JOYP_LB;
  /*p05.JALE*/ bool JOYP_UC;
  /*p05.KYME*/ bool JOYP_DS;
  /*p05.KELY*/ bool JOYP_UDLR;
  /*p05.COFY*/ bool JOYP_ABCS;
  /*p05.KUKO*/ bool DBG_FF00_D6;
  /*p05.KERU*/ bool DBG_FF00_D7;

  //----------
  // Joypad pins

  /* PIN_67 */ bool P10_A;   // <- KOLE
  /* PIN_67 */ bool P10_B;   // This pin is weird and seems to be used as an input, or at least I can't find the driver
  /* PIN_67 */ bool P10_C;   // -> P02.KERY, P05.KEVU
  /* PIN_67 */ bool P10_D;   // <- KYBU

  /* PIN_66 */ bool P11_A;   // <- P05.KYTO
  /* PIN_66 */ bool P11_B;   
  /* PIN_66 */ bool P11_C;   // -> P02.KERY, P05.KAPA
  /* PIN_66 */ bool P11_D;   // <- P05.KABU

  /* PIN_65 */ bool P12_A;   // <- P05.KYHU
  /* PIN_65 */ bool P12_B;   
  /* PIN_65 */ bool P12_C;   // -> P02.KERY, P05.KEJA
  /* PIN_65 */ bool P12_D;   // <- P05.KASY
                            
  /* PIN_64 */ bool P13_A;   // <- P05.KORY
  /* PIN_64 */ bool P13_B;   
  /* PIN_64 */ bool P13_C;   // -> P02.KERY, P05.KOLO
  /* PIN_64 */ bool P13_D;   // <- P05.KALE
                             
  /* PIN_63 */ bool P14_A;   // <- p05.KARU
  /* PIN_63 */ bool P14_D;   // <- p05.KELY

  /* PIN_62 */ bool P15_A;   // <- p05.CELA
  /* PIN_62 */ bool P15_D;   // <- p05.COFY
};

struct JoypadTickIn {

  bool SYS_RESETn;

  bool PHASE_ABCDxxxx1;  
  bool PHASE_xBCDExxx1;
  bool PHASE_xxCDEFxx1;
  bool CPUCLK_REQn;
  bool CLK_BAD2;

  bool CPU_RD;
  bool CPU_WR;
  bool A00,A01,A02,A03,A04,A05,A06,A07,A08,A09,A10,A11,A12,A13,A14,A15;
  bool D0,D1,D2,D3,D4,D5,D6,D7;

  bool FF60_0;
  bool P10_C;
  bool P11_C;
  bool P12_C;
  bool P13_C;
};

struct JoypadTickOut {
  bool D0,D1,D2,D3,D4,D5,D6,D7;

  /*p05.KOLE*/ bool P10_A;
  /*p05.KYBU*/ bool P10_D;
  /*p05.KYTO*/ bool P11_A;
  /*p05.KABU*/ bool P11_D;
  /*p05.KYHU*/ bool P12_A;
  /*p05.KASY*/ bool P12_D;
  /*p05.KORY*/ bool P13_A;
  /*p05.KALE*/ bool P13_D;
  /*p05.KARU*/ bool P14_A;
  /*p05.KARU*/ bool P14_D;
  /*p05.CELA*/ bool P15_A;
  /*p05.KARU*/ bool P15_D;
};

//-----------------------------------------------------------------------------

};
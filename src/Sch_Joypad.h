#pragma once
#include "Sch_Common.h"

namespace Schematics {

struct TestGB;

//-----------------------------------------------------------------------------

struct JoypadSignals {
  /*p02.ASOK*/ wire ASOK_INT_JPp;
};

struct JoypadPins {

  void dump_pins(TextPainter& text_painter) {
    text_painter.dprintf("----- JOY_PINS -----\n");
    text_painter.dprintf("P10 %d:%d:%d:%d\n", P10_A.a.val, P10_B.a.val, P10_C.a.val, P10_D.a.val);
    text_painter.dprintf("P11 %d:%d:%d:%d\n", P11_A.a.val, P11_B.a.val, P11_C.a.val, P11_D.a.val);
    text_painter.dprintf("P12 %d:%d:%d:%d\n", P12_A.a.val, P12_B.a.val, P12_C.a.val, P12_D.a.val);
    text_painter.dprintf("P13 %d:%d:%d:%d\n", P13_A.a.val, P13_B.a.val, P13_C.a.val, P13_D.a.val);
    text_painter.dprintf("P14 %d:x:x:%d\n", P14_A.a.val, P14_D.a.val);
    text_painter.dprintf("P15 %d:x:x:%d\n", P15_A.a.val, P15_D.a.val);
    text_painter.newline();
  }

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

struct JoypadRegisters {

  JoypadSignals sig(const TestGB& gb) const;
  void tick(TestGB& gb);
  bool commit();

  void dump_regs(TextPainter& text_painter) {
    text_painter.dprintf(" ----- JOY REG -----\n");
    JP_GLITCH0.dump(text_painter, "JP_GLITCH0  ");
    JP_GLITCH1.dump(text_painter, "JP_GLITCH1  ");
    JP_GLITCH2.dump(text_painter, "JP_GLITCH2  ");
    JP_GLITCH3.dump(text_painter, "JP_GLITCH3  ");
    JOYP_RA.dump(text_painter, "JOYP_RA     ");
    JOYP_LB.dump(text_painter, "JOYP_LB     ");
    JOYP_UC.dump(text_painter, "JOYP_UC     ");
    JOYP_DS.dump(text_painter, "JOYP_DS     ");
    JOYP_UDLR.dump(text_painter, "JOYP_UDLR   ");
    JOYP_ABCS.dump(text_painter, "JOYP_ABCS   ");
    DBG_FF00_D6.dump(text_painter, "DBG_FF00_D6 ");
    DBG_FF00_D7.dump(text_painter, "DBG_FF00_D7 ");
    JOYP_L0.dump(text_painter, "JOYP_L0     ");
    JOYP_L1.dump(text_painter, "JOYP_L1     ");
    JOYP_L2.dump(text_painter, "JOYP_L2     ");
    JOYP_L3.dump(text_painter, "JOYP_L3     ");
    WAKE_CPU.dump(text_painter, "WAKE_CPU    ");
    text_painter.newline();
  }

  /*p02.BATU*/ Reg JP_GLITCH0;
  /*p02.ACEF*/ Reg JP_GLITCH1;
  /*p02.AGEM*/ Reg JP_GLITCH2;
  /*p02.APUG*/ Reg JP_GLITCH3;
  /*p05.JUTE*/ Reg JOYP_RA;
  /*p05.KECY*/ Reg JOYP_LB;
  /*p05.JALE*/ Reg JOYP_UC;
  /*p05.KYME*/ Reg JOYP_DS;
  /*p05.KELY*/ Reg JOYP_UDLR;
  /*p05.COFY*/ Reg JOYP_ABCS;
  /*p05.KUKO*/ Reg DBG_FF00_D6;
  /*p05.KERU*/ Reg DBG_FF00_D7;
  /*p05.KEVU*/ Reg JOYP_L0;
  /*p05.KAPA*/ Reg JOYP_L1;
  /*p05.KEJA*/ Reg JOYP_L2;
  /*p05.KOLO*/ Reg JOYP_L3;
  /*p02.AWOB*/ Reg WAKE_CPU;
};


//-----------------------------------------------------------------------------

}; // namespace Schematics
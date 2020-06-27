#pragma once
#include "Sch_Common.h"

namespace Schematics {

struct WavePins {
  void dump_pins(TextPainter& text_painter) {
    text_painter.dprintf("----- WAVE_PINS -----\n");
    text_painter.dprintf("CLK_AxxxxxGH %d\n", BORY_ABxxxxxH.a.val);
  }

  PinOut BORY_ABxxxxxH;
};

//-----------------------------------------------------------------------------
// Clock/reset/debug

struct SysPins {

  void dump_pins(TextPainter& text_painter) {
    text_painter.dprintf("----- SYS_PINS -----\n");
    text_painter.dprintf("RST      %d\n", RST.a.val);
    text_painter.dprintf("PIN_CLK_GOOD %d\n", PIN_CLK_GOOD.a.val);
    text_painter.dprintf("CLK_IN_xBxDxFxH   %d\n", CLK_IN_xBxDxFxH.a.val);
    text_painter.dprintf("T1       %d\n", T1.a.val);
    text_painter.dprintf("T2       %d\n", T2.a.val);
    text_painter.newline();
  }

  /* PIN_71 */ PinIn RST;
  /* PIN_74 */ PinIn PIN_CLK_GOOD;
  /* PIN_74 */ PinIn CLK_IN_xBxDxFxH;
  /* PIN_76 */ PinIn T2;
  /* PIN_77 */ PinIn T1;
};

//-----------------------------------------------------------------------------

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


};
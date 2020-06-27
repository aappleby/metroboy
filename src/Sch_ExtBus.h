#pragma once
#include "Sch_Common.h"

namespace Schematics {

struct TestGB;

//-----------------------------------------------------------------------------

struct ExtBus {

  void tick(TestGB& gb);
  bool commit();

  inline void dump_pins(TextPainter& text_painter, const PinOut& a, const PinOut& d) {
    dump_pin(text_painter, a.a, d.a);
  }

  inline void dump_pins(TextPainter& text_painter, const PinOut& a, const PinOut& b, const PinIn& c, const PinOut& d) {
    dump_pin(text_painter, a.a, b.a, c.a, d.a);
  }

  void dump_regs(TextPainter& text_painter) {
    text_painter.dprintf(" ----- EXT_BUS -----\n");

    /*
    text_painter.dprintf("ADDR_LATCH ");
    dump2(text_painter, EXT_ADDR_LATCH_14.a);
    dump2(text_painter, EXT_ADDR_LATCH_13.a);
    dump2(text_painter, EXT_ADDR_LATCH_12.a);
    text_painter.add_char(':');
    dump2(text_painter, EXT_ADDR_LATCH_11.a);
    dump2(text_painter, EXT_ADDR_LATCH_10.a);
    dump2(text_painter, EXT_ADDR_LATCH_09.a);
    dump2(text_painter, EXT_ADDR_LATCH_08.a);
    text_painter.add_char(':');
    dump2(text_painter, EXT_ADDR_LATCH_07.a);
    dump2(text_painter, EXT_ADDR_LATCH_06.a);
    dump2(text_painter, EXT_ADDR_LATCH_05.a);
    dump2(text_painter, EXT_ADDR_LATCH_04.a);
    text_painter.add_char(':');
    dump2(text_painter, EXT_ADDR_LATCH_03.a);
    dump2(text_painter, EXT_ADDR_LATCH_02.a);
    dump2(text_painter, EXT_ADDR_LATCH_01.a);
    dump2(text_painter, EXT_ADDR_LATCH_00.a);
    text_painter.newline();
    */

    /*
    text_painter.dprintf("DATA_LATCH ");
    dump2(text_painter, CART_DATA_LATCH_07.a);
    dump2(text_painter, CART_DATA_LATCH_06.a);
    dump2(text_painter, CART_DATA_LATCH_05.a);
    dump2(text_painter, CART_DATA_LATCH_04.a);
    text_painter.add_char(':');
    dump2(text_painter, CART_DATA_LATCH_03.a);
    dump2(text_painter, CART_DATA_LATCH_02.a);
    dump2(text_painter, CART_DATA_LATCH_01.a);
    dump2(text_painter, CART_DATA_LATCH_00.a);
    */
    text_painter.newline();
    text_painter.newline();
  }

  void dump_pins(TextPainter& text_painter) {
    text_painter.dprintf("----- EXT_PINS -----\n");

    text_painter.dprintf("PHI %d\n", PHI.a.val);
    text_painter.dprintf("WR  %d\n", WR_A.a.val);
    text_painter.dprintf("RD  %d\n", RD_A.a.val);
    text_painter.dprintf("CS  %d\n", CS_A.a.val);

    text_painter.add_text("Axx ");
    dump_pins(text_painter, A15_A, A15_D);
    dump_pins(text_painter, A14_A, A14_D);
    dump_pins(text_painter, A13_A, A13_D);
    dump_pins(text_painter, A12_A, A12_D);
    text_painter.add_char(':');
    dump_pins(text_painter, A11_A, A11_D);
    dump_pins(text_painter, A10_A, A10_D);
    dump_pins(text_painter, A09_A, A09_D);
    dump_pins(text_painter, A08_A, A08_D);
    text_painter.add_char(':');
    dump_pins(text_painter, A07_A, A07_D);
    dump_pins(text_painter, A06_A, A06_D);
    dump_pins(text_painter, A05_A, A05_D);
    dump_pins(text_painter, A04_A, A04_D);
    text_painter.add_char(':');
    dump_pins(text_painter, A03_A, A03_D);
    dump_pins(text_painter, A02_A, A02_D);
    dump_pins(text_painter, A01_A, A01_D);
    dump_pins(text_painter, A00_A, A00_D);
    text_painter.newline();

    text_painter.add_text("Dxx ");
    dump_pins(text_painter, D7_A, D7_B, D7_C, D7_D);
    dump_pins(text_painter, D6_A, D6_B, D6_C, D6_D);
    dump_pins(text_painter, D5_A, D5_B, D5_C, D5_D);
    dump_pins(text_painter, D4_A, D4_B, D4_C, D4_D);
    dump_pins(text_painter, D3_A, D3_B, D3_C, D3_D);
    dump_pins(text_painter, D2_A, D2_B, D2_C, D2_D);
    dump_pins(text_painter, D1_A, D1_B, D1_C, D1_D);
    dump_pins(text_painter, D0_A, D0_B, D0_C, D0_D);
    text_painter.newline();
  }

  /*p08.ALOR*/ TpLatch EXT_ADDR_LATCH_00;
  /*p08.APUR*/ TpLatch EXT_ADDR_LATCH_01;
  /*p08.ALYR*/ TpLatch EXT_ADDR_LATCH_02;
  /*p08.ARET*/ TpLatch EXT_ADDR_LATCH_03;
  /*p08.AVYS*/ TpLatch EXT_ADDR_LATCH_04;
  /*p08.ATEV*/ TpLatch EXT_ADDR_LATCH_05;
  /*p08.AROS*/ TpLatch EXT_ADDR_LATCH_06;
  /*p08.ARYM*/ TpLatch EXT_ADDR_LATCH_07;
  /*p08.LUNO*/ TpLatch EXT_ADDR_LATCH_08;
  /*p08.LYSA*/ TpLatch EXT_ADDR_LATCH_09;
  /*p08.PATE*/ TpLatch EXT_ADDR_LATCH_10;
  /*p08.LUMY*/ TpLatch EXT_ADDR_LATCH_11;
  /*p08.LOBU*/ TpLatch EXT_ADDR_LATCH_12;
  /*p08.LONU*/ TpLatch EXT_ADDR_LATCH_13;
  /*p08.NYRE*/ TpLatch EXT_ADDR_LATCH_14;

  /*p08.SOMA*/ TpLatch EXT_DATA_LATCH_00;
  /*p08.RONY*/ TpLatch EXT_DATA_LATCH_01;
  /*p08.RAXY*/ TpLatch EXT_DATA_LATCH_02;
  /*p08.SELO*/ TpLatch EXT_DATA_LATCH_03;
  /*p08.SODY*/ TpLatch EXT_DATA_LATCH_04;
  /*p08.SAGO*/ TpLatch EXT_DATA_LATCH_05;
  /*p08.RUPA*/ TpLatch EXT_DATA_LATCH_06;
  /*p08.SAZY*/ TpLatch EXT_DATA_LATCH_07;

  //----------
  // Cart pins

  /* PIN_75 */ PinOut PHI;     // <- P01.BUDE/BEVA

  /* PIN_78 */ PinOut WR_A;   // <- P08.UVER
  /* PIN_78 */ PinIn  WR_C;   // -> P07.UBAL
  /* PIN_78 */ PinOut WR_D;   // <- P08.USUF

  /* PIN_79 */ PinOut RD_A;   // <- P08.UGAC
  /* PIN_79 */ PinIn  RD_C;   // -> P07.UJYV
  /* PIN_79 */ PinOut RD_D;   // <- P08.URUN

  /* PIN_80 */ PinOut CS_A;   // <- P08.TYHO

  //----------
  // Address pins

  /* PIN_01 */ PinOut A00_A;   // <- P08.KUPO
  /* PIN_01 */ PinIn  A00_C;   // -> P08.KOVA
  /* PIN_01 */ PinOut A00_D;   // <- P08.KOTY

  /* PIN_02 */ PinOut A01_A;   // <- P08.CABA
  /* PIN_02 */ PinIn  A01_C;   // -> P08.CAMU
  /* PIN_02 */ PinOut A01_D;   // <- P08.COTU

  /* PIN_03 */ PinOut A02_A;   // <- P08.BOKU
  /* PIN_03 */ PinIn  A02_C;   // -> P08.BUXU
  /* PIN_03 */ PinOut A02_D;   // <- P08.BAJO

  /* PIN_04 */ PinOut A03_A;   // <- P08.BOTY
  /* PIN_04 */ PinIn  A03_C;   // -> P08.BASE
  /* PIN_04 */ PinOut A03_D;   // <- P08.BOLA

  /* PIN_05 */ PinOut A04_A;   // <- P08.BYLA
  /* PIN_05 */ PinIn  A04_C;   // -> P08.AFEC
  /* PIN_05 */ PinOut A04_D;   // <- P08.BEVO

  /* PIN_06 */ PinOut A05_A;   // <- P08.BADU
  /* PIN_06 */ PinIn  A05_C;   // -> P08.ABUP
  /* PIN_06 */ PinOut A05_D;   // <- P08.AJAV

  /* PIN_07 */ PinOut A06_A;   // <- P08.CEPU
  /* PIN_07 */ PinIn  A06_C;   // -> P08.CYGU
  /* PIN_07 */ PinOut A06_D;   // <- P08.CYKA

  /* PIN_08 */ PinOut A07_A;   // <- P08.DEFY
  /* PIN_08 */ PinIn  A07_C;   // -> P08.COGO
  /* PIN_08 */ PinOut A07_D;   // <- P08.COLO

  /* PIN_09 */ PinOut A08_A;   // <- P08.MYNY
  /* PIN_09 */ PinIn  A08_C;   // -> P08.MUJY
  /* PIN_09 */ PinOut A08_D;   // <- P08.MEGO

  /* PIN_10 */ PinOut A09_A;   // <- P08.MUNE
  /* PIN_10 */ PinIn  A09_C;   // -> P08.NENA
  /* PIN_10 */ PinOut A09_D;   // <- P08.MENY

  /* PIN_11 */ PinOut A10_A;   // <- P08.ROXU
  /* PIN_11 */ PinIn  A10_C;   // -> P08.SURA
  /* PIN_11 */ PinOut A10_D;   // <- P08.RORE

  /* PIN_12 */ PinOut A11_A;   // <- P08.LEPY
  /* PIN_12 */ PinIn  A11_C;   // -> P08.MADY
  /* PIN_12 */ PinOut A11_D;   // <- P08.LYNY

  /* PIN_13 */ PinOut A12_A;   // <- P08.LUCE
  /* PIN_13 */ PinIn  A12_C;   // -> P08.LAHE
  /* PIN_13 */ PinOut A12_D;   // <- P08.LOSO

  /* PIN_14 */ PinOut A13_A;   // <- P08.LABE
  /* PIN_14 */ PinIn  A13_C;   // -> P08.LURA
  /* PIN_14 */ PinOut A13_D;   // <- P08.LEVA

  /* PIN_15 */ PinOut A14_A;   // <- P08.PUHE
  /* PIN_15 */ PinIn  A14_C;   // -> P08.PEVO
  /* PIN_15 */ PinOut A14_D;   // <- P08.PAHY

  /* PIN_16 */ PinOut A15_A;   // <- P08.SUZE
  /* PIN_16 */ PinIn  A15_C;   // -> P08.RAZA
  /* PIN_16 */ PinOut A15_D;   // <- P08.RULO

  //----------
  // Data pins

  /* PIN_17 */ PinOut D0_A;    // <- P08.RUXA
  /* PIN_17 */ PinOut D0_B;    // <- P08.LULA
  /* PIN_17 */ PinIn  D0_C;    // -> P08.TOVO,SOMA
  /* PIN_17 */ PinOut D0_D;    // <- P08.RUNE

  /* PIN_18 */ PinOut D1_A;    // <- P08.RUJA
  /* PIN_18 */ PinOut D1_B;    // <- P08.LULA
  /* PIN_18 */ PinIn  D1_C;    // -> P08.RUZY,RONY
  /* PIN_18 */ PinOut D1_D;    // <- P08.RYPU

  /* PIN_19 */ PinOut D2_A;    // <- P08.RABY
  /* PIN_19 */ PinOut D2_B;    // <- P08.LULA
  /* PIN_19 */ PinIn  D2_C;    // -> P08.ROME,RAXY
  /* PIN_19 */ PinOut D2_D;    // <- P08.SULY

  /* PIN_20 */ PinOut D3_A;    // <- P08.RERA
  /* PIN_20 */ PinOut D3_B;    // <- P08.LULA
  /* PIN_20 */ PinIn  D3_C;    // -> P08.SAZA,SELO
  /* PIN_20 */ PinOut D3_D;    // <- P08.SEZE

  /* PIN_21 */ PinOut D4_A;    // <- P08.RORY
  /* PIN_21 */ PinOut D4_B;    // <- P08.LULA
  /* PIN_21 */ PinIn  D4_C;    // -> P08.TEHE,SODY
  /* PIN_21 */ PinOut D4_D;    // <- P08.RESY

  /* PIN_22 */ PinOut D5_A;    // <- P08.RYVO
  /* PIN_22 */ PinOut D5_B;    // <- P08.LULA
  /* PIN_22 */ PinIn  D5_C;    // -> P08.RATU,SAGO
  /* PIN_22 */ PinOut D5_D;    // <- P08.TAMU

  /* PIN_23 */ PinOut D6_A;    // <- P08.RAFY
  /* PIN_23 */ PinOut D6_B;    // <- P08.LULA
  /* PIN_23 */ PinIn  D6_C;    // -> P08.SOCA,RUPA
  /* PIN_23 */ PinOut D6_D;    // <- P08.ROGY

  /* PIN_24 */ PinOut D7_A;    // <- P08.RAVU
  /* PIN_24 */ PinOut D7_B;    // <- P08.LULA
  /* PIN_24 */ PinIn  D7_C;    // -> P08.RYBA,SAZY
  /* PIN_24 */ PinOut D7_D;    // <- P08.RYDA
};

//-----------------------------------------------------------------------------

}; // namespace Schematics
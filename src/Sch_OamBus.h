#pragma once
#include "Sch_Common.h"

namespace Schematics {

struct TestGB;

//-----------------------------------------------------------------------------
// These signals connect directly to OAM

struct OamBus {

  void dump_pins(TextPainter& text_painter) {
    text_painter.dprintf("----- OAM_PINS -----\n");
    text_painter.dprintf("PIN_CLK %d\n", PIN_CLK.a.val);
    text_painter.dprintf("PIN_OE    %d\n", PIN_OE.a.val);
    text_painter.dprintf("PIN_WR_A  %d\n", PIN_WR_A.a.val);
    text_painter.dprintf("PIN_WR_B  %d\n", PIN_WR_B.a.val);

    text_painter.add_text("Axx   ");
    dump2(text_painter, PIN_A7.a);
    dump2(text_painter, PIN_A6.a);
    dump2(text_painter, PIN_A5.a);
    dump2(text_painter, PIN_A4.a);
    dump2(text_painter, PIN_A3.a);
    dump2(text_painter, PIN_A2.a);
    dump2(text_painter, PIN_A1.a);
    dump2(text_painter, PIN_A0.a);
    text_painter.newline();

    text_painter.add_text("A_Dx  ");
    dump2(text_painter, PIN_DA7.a);
    dump2(text_painter, PIN_DA6.a);
    dump2(text_painter, PIN_DA5.a);
    dump2(text_painter, PIN_DA4.a);
    dump2(text_painter, PIN_DA3.a);
    dump2(text_painter, PIN_DA2.a);
    dump2(text_painter, PIN_DA1.a);
    dump2(text_painter, PIN_DA0.a);
    text_painter.newline();

    text_painter.add_text("B_Dx  ");
    dump2(text_painter, PIN_DB7.a);
    dump2(text_painter, PIN_DB6.a);
    dump2(text_painter, PIN_DB5.a);
    dump2(text_painter, PIN_DB4.a);
    dump2(text_painter, PIN_DB3.a);
    dump2(text_painter, PIN_DB2.a);
    dump2(text_painter, PIN_DB1.a);
    dump2(text_painter, PIN_DB0.a);
    text_painter.newline();

    text_painter.newline();
  }

  PinOut PIN_CLK;
  PinOut PIN_OE;
  PinOut PIN_WR_A; // definitely write
  PinOut PIN_WR_B; // definitely write

  PinOut PIN_A0;
  PinOut PIN_A1;
  PinOut PIN_A2;
  PinOut PIN_A3;
  PinOut PIN_A4;
  PinOut PIN_A5;
  PinOut PIN_A6;
  PinOut PIN_A7;

  Tribuf PIN_DA0;
  Tribuf PIN_DA1;
  Tribuf PIN_DA2;
  Tribuf PIN_DA3;
  Tribuf PIN_DA4;
  Tribuf PIN_DA5;
  Tribuf PIN_DA6;
  Tribuf PIN_DA7;

  Tribuf PIN_DB0;
  Tribuf PIN_DB1;
  Tribuf PIN_DB2;
  Tribuf PIN_DB3;
  Tribuf PIN_DB4;
  Tribuf PIN_DB5;
  Tribuf PIN_DB6;
  Tribuf PIN_DB7;
};

//-----------------------------------------------------------------------------

}; // namespace Schematics
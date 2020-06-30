#pragma once
#include "Sch_Common.h"

namespace Schematics {

struct TestGB;

//-----------------------------------------------------------------------------

struct VramBus {

  void tick(TestGB& gb);
  bool commit();

  void dump_pins(TextPainter& text_painter) {
    text_painter.dprintf("----- VRAM_BUS -----\n");
    text_painter.dprintf("MAxx 0x%04x\n", pack(
      TRI_A00.q(), TRI_A01.q(), TRI_A02.q(), TRI_A03.q(),
      TRI_A04.q(), TRI_A05.q(), TRI_A06.q(), TRI_A07.q(),
      TRI_A08.q(), TRI_A09.q(), TRI_A10.q(), TRI_A11.q(), TRI_A12.q()));
    text_painter.dprintf("MDxx 0x%02x\n", pack(
      TRI_D0.q(), TRI_D1.q(), TRI_D2.q(), TRI_D3.q(),
      TRI_D4.q(), TRI_D5.q(), TRI_D6.q(), TRI_D7.q()));
    text_painter.newline();
  }

  //----------
  // VRAM bus

  Tribuf TRI_A00;
  Tribuf TRI_A01;
  Tribuf TRI_A02;
  Tribuf TRI_A03;
  Tribuf TRI_A04;
  Tribuf TRI_A05;
  Tribuf TRI_A06;
  Tribuf TRI_A07;
  Tribuf TRI_A08;
  Tribuf TRI_A09;
  Tribuf TRI_A10;
  Tribuf TRI_A11;
  Tribuf TRI_A12;

  Tribuf TRI_D0;
  Tribuf TRI_D1;
  Tribuf TRI_D2;
  Tribuf TRI_D3;
  Tribuf TRI_D4;
  Tribuf TRI_D5;
  Tribuf TRI_D6;
  Tribuf TRI_D7;
};

//-----------------------------------------------------------------------------

}; // namespace Schematics
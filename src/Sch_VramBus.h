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
    text_painter.dprintf("MAxx 0x%04x\n", pack(TS_MA00.q(), TS_MA01.q(), TS_MA02.q(), TS_MA03.q(), TS_MA04.q(), TS_MA05.q(), TS_MA06.q(), TS_MA07.q(), TS_MA08.q(), TS_MA09.q(), TS_MA10.q(), TS_MA11.q(), TS_MA12.q()));
    text_painter.dprintf("MDxx 0x%02x\n", pack(TS_MD0.q(), TS_MD1.q(), TS_MD2.q(), TS_MD3.q(), TS_MD4.q(), TS_MD5.q(), TS_MD6.q(), TS_MD7.q()));
    text_painter.newline();
  }

  //----------
  // VRAM bus

  Tribuf TS_MA00;
  Tribuf TS_MA01;
  Tribuf TS_MA02;
  Tribuf TS_MA03;
  Tribuf TS_MA04;
  Tribuf TS_MA05;
  Tribuf TS_MA06;
  Tribuf TS_MA07;
  Tribuf TS_MA08;
  Tribuf TS_MA09;
  Tribuf TS_MA10;
  Tribuf TS_MA11;
  Tribuf TS_MA12;

  Tribuf TS_MD0;
  Tribuf TS_MD1;
  Tribuf TS_MD2;
  Tribuf TS_MD3;
  Tribuf TS_MD4;
  Tribuf TS_MD5;
  Tribuf TS_MD6;
  Tribuf TS_MD7;
};

//-----------------------------------------------------------------------------

}; // namespace Schematics
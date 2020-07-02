#include "Sch_VramBus.h"

#include "Sch_Top.h"

using namespace Schematics;

void VramBus::tick(SchematicTop& /*gb*/) {
}

SignalHash VramBus::commit() {
  SignalHash hash;
  hash << TRI_A00.commit_tribuf();
  hash << TRI_A01.commit_tribuf();
  hash << TRI_A02.commit_tribuf();
  hash << TRI_A03.commit_tribuf();
  hash << TRI_A04.commit_tribuf();
  hash << TRI_A05.commit_tribuf();
  hash << TRI_A06.commit_tribuf();
  hash << TRI_A07.commit_tribuf();
  hash << TRI_A08.commit_tribuf();
  hash << TRI_A09.commit_tribuf();
  hash << TRI_A10.commit_tribuf();
  hash << TRI_A11.commit_tribuf();
  hash << TRI_A12.commit_tribuf();

  hash << TRI_D0.commit_tribuf();
  hash << TRI_D1.commit_tribuf();
  hash << TRI_D2.commit_tribuf();
  hash << TRI_D3.commit_tribuf();
  hash << TRI_D4.commit_tribuf();
  hash << TRI_D5.commit_tribuf();
  hash << TRI_D6.commit_tribuf();
  hash << TRI_D7.commit_tribuf();
  return hash;
}

#if 0

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

#endif
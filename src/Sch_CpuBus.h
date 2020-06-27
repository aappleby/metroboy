#pragma once
#include "Sch_Common.h"

namespace Schematics {

struct TestGB;

//-----------------------------------------------------------------------------

struct CpuBus {

  void tick(TestGB& gb);
  bool commit();

  void dump_pins(TextPainter& text_painter) {
    text_painter.dprintf("----- CPU DBG/RST -----\n");
    text_painter.dprintf("PIN_RESET       %d\n", PIN_RESET.a.val);
    text_painter.dprintf("PIN_CPU_RESET       %d\n", PIN_CPU_RESET.a.val);
    text_painter.dprintf("PIN_CLK_GOOD        %d\n", PIN_CLK_GOOD.a.val);

    text_painter.dprintf("----- CPU CLOCKS -----\n");

    text_painter.dprintf("PIN_BOWA_AxCDEFGH %d\n", PIN_BOWA_AxCDEFGH.a.val);
    text_painter.dprintf("PIN_BEDO_xBxxxxxx %d\n", PIN_BEDO_xBxxxxxx.a.val);
    text_painter.dprintf("PIN_BEKO_xBCDExxx %d\n", PIN_BEKO_xBCDExxx.a.val);
    text_painter.dprintf("PIN_BUDE_AxxxxFGH %d\n", PIN_BUDE_AxxxxFGH.a.val);
    text_painter.dprintf("PIN_BOLO_xBCDEFGx %d\n", PIN_BOLO_xBCDEFGx.a.val);
    text_painter.dprintf("PIN_BUKE_ABxxxxxH %d\n", PIN_BUKE_ABxxxxxH.a.val);
    text_painter.dprintf("PIN_BOMA_xBxxxxxx %d\n", PIN_BOMA_xBxxxxxx.a.val);
    text_painter.dprintf("PIN_BOGA_AxCDEFGH %d\n", PIN_BOGA_AxCDEFGH.a.val);

    text_painter.dprintf("----- FROM CPU -----\n");
    text_painter.dprintf("PIN_CLKREQ        %d\n", PIN_CLKREQ.a.val);
    text_painter.dprintf("PIN_FROM_CPU5p     %d\n", PIN_FROM_CPU5p.a.val);
    text_painter.dprintf("PIN_FROM_CPU6     %d\n", PIN_FROM_CPU6.a.val);

    text_painter.dprintf("----- TO CPU -----\n");
    text_painter.dprintf("PIN_AFER          %d\n", PIN_AFER.a.val);
    text_painter.dprintf("PIN_TO_CPU2       %d\n", PIN_TO_CPU2.a.val);
    text_painter.dprintf("PIN_SYRO          %d\n", PIN_SYRO.a.val);
    text_painter.dprintf("PIN_READ_BOOTROM  %d\n", PIN_READ_BOOTROM.a.val);

    text_painter.dprintf("----- CPU INT -----\n");
    text_painter.dprintf("PIN_INT_VBLANK    %d\n", PIN_INT_VBLANK.a.val);
    text_painter.dprintf("PIN_INT_STAT      %d\n", PIN_INT_STAT.a.val);
    text_painter.dprintf("PIN_INT_TIMER     %d\n", PIN_INT_TIMER.a.val);
    text_painter.dprintf("PIN_INT_SERIAL    %d\n", PIN_INT_SERIAL.a.val);
    text_painter.dprintf("PIN_INT_JOYPAD    %d\n", PIN_INT_JOYPAD.a.val);
    text_painter.dprintf("PIN_ACK_VBLANK    %d\n", PIN_ACK_VBLANK.a.val);
    text_painter.dprintf("PIN_ACK_STAT      %d\n", PIN_ACK_STAT.a.val);
    text_painter.dprintf("PIN_ACK_TIMER     %d\n", PIN_ACK_TIMER.a.val);
    text_painter.dprintf("PIN_ACK_SERIAL    %d\n", PIN_ACK_SERIAL.a.val);
    text_painter.dprintf("PIN_ACK_JOYPAD    %d\n", PIN_ACK_JOYPAD.a.val);

    text_painter.dprintf("----- CPU BUS -----\n");
    text_painter.dprintf("PIN_CPU_RAW_RD    %d\n", PIN_CPU_RAW_RD.a.val);
    text_painter.dprintf("PIN_CPU_RAW_WR    %d\n", PIN_CPU_RAW_WR.a.val);
    text_painter.dprintf("PIN_ADDR_VALID    %d\n", PIN_ADDR_VALID.a.val);

    text_painter.add_text("Axx ");
    dump2(text_painter, A15.a);
    dump2(text_painter, A14.a);
    dump2(text_painter, A13.a);
    dump2(text_painter, A12.a);
    text_painter.add_char(':');
    dump2(text_painter, A11.a);
    dump2(text_painter, A10.a);
    dump2(text_painter, A09.a);
    dump2(text_painter, A08.a);
    text_painter.add_char(':');
    dump2(text_painter, A07.a);
    dump2(text_painter, A06.a);
    dump2(text_painter, A05.a);
    dump2(text_painter, A04.a);
    text_painter.add_char(':');
    dump2(text_painter, A03.a);
    dump2(text_painter, A02.a);
    dump2(text_painter, A01.a);
    dump2(text_painter, A00.a);
    text_painter.newline();

    text_painter.add_text("Dxx ");
    dump2(text_painter, D7.a);
    dump2(text_painter, D6.a);
    dump2(text_painter, D5.a);
    dump2(text_painter, D4.a);
    text_painter.add_char(':');
    dump2(text_painter, D3.a);
    dump2(text_painter, D2.a);
    dump2(text_painter, D1.a);
    dump2(text_painter, D0.a);
    text_painter.newline();

    text_painter.newline();
  }

  int get_addr() const {
    return pack(A00, A01, A02, A03, A04, A05, A06, A07,
      A08, A09, A10, A11, A12, A13, A14, A15);
  }

  int get_data() const {
    return pack(D0, D1, D2, D3, D4, D5, D6, D7);
  }

  void preset_addr(bool oe, uint16_t addr) {
    A00.preset(oe, addr & 0x0001);
    A01.preset(oe, addr & 0x0002);
    A02.preset(oe, addr & 0x0004);
    A03.preset(oe, addr & 0x0008);
    A04.preset(oe, addr & 0x0010);
    A05.preset(oe, addr & 0x0020);
    A06.preset(oe, addr & 0x0040);
    A07.preset(oe, addr & 0x0080);
    A08.preset(oe, addr & 0x0100);
    A09.preset(oe, addr & 0x0200);
    A10.preset(oe, addr & 0x0400);
    A11.preset(oe, addr & 0x0800);
    A12.preset(oe, addr & 0x1000);
    A13.preset(oe, addr & 0x2000);
    A14.preset(oe, addr & 0x4000);
    A15.preset(oe, addr & 0x8000);
  }

  /*
  void preset_data(bool oe, uint8_t data) {
    D0.preset(oe, data & 0x01);
    D1.preset(oe, data & 0x02);
    D2.preset(oe, data & 0x04);
    D3.preset(oe, data & 0x08);
    D4.preset(oe, data & 0x10);
    D5.preset(oe, data & 0x20);
    D6.preset(oe, data & 0x40);
    D7.preset(oe, data & 0x80);
  }
  */

  void set_data(bool oe, uint8_t data) {
    D0.set_tribuf(oe, data & 0x01);
    D1.set_tribuf(oe, data & 0x02);
    D2.set_tribuf(oe, data & 0x04);
    D3.set_tribuf(oe, data & 0x08);
    D4.set_tribuf(oe, data & 0x10);
    D5.set_tribuf(oe, data & 0x20);
    D6.set_tribuf(oe, data & 0x40);
    D7.set_tribuf(oe, data & 0x80);
  }

  //----------

  PinIn  PIN_FROM_CPU6;     // top left port PORTD_00: -> LEXY, doesn't do anything. FROM_CPU6? 

  PinOut PIN_BOWA_AxCDEFGH; // top left port PORTD_01: <- BOWA_AxCDEFGH // Blue clock - decoders, alu, some reset stuff
  PinOut PIN_BEDO_xBxxxxxx; // top left port PORTD_02: <- BEDO_xBxxxxxx

  PinOut PIN_BEKO_xBCDExxx; // top left port PORTD_03: <- BEKO_ABCDxxxx + BAVY connection not indicated on P01 - test pad 1
  PinOut PIN_BUDE_AxxxxFGH; // top left port PORTD_04: <- BUDE_AxxxxFGH + BEVA

  PinOut PIN_BOLO_xBCDEFGx; // top left port PORTD_05: <- BOLO_ABCDEFxx + BYDA? - test pad 2
  PinIn  PIN_FROM_CPU5p;     // top left port PORTD_06: -> ANUJ (FROM_CPU5)
  PinOut PIN_BUKE_ABxxxxxH; // top left port PORTD_07: <- BUKE_ABxxxxxH

  PinOut PIN_BOMA_xBxxxxxx; // top left port PORTD_08: <- BOMA_xBxxxxxx (RESET_CLK)
  PinOut PIN_BOGA_AxCDEFGH; // top left port PORTD_09: <- BOGA_AxCDEFGH - test pad 3

  PinIn  PIN_CLKREQ;        // top center port PORTC_00: -> ABOL (an inverter) -> BATE. Something about "cpu ready". clock request?
  PinOut PIN_AFER;          // top center port PORTC_01: <- P01.AFER , reset related reg
  PinOut PIN_RESET;         // top center port PORTC_02: <- PIN_RESET directly connected to the pad
  PinOut PIN_CLK_GOOD;      // top center port PORTC_03: <- chip.CLKIN_A top wire on PAD_XI,
  PinOut PIN_CPU_RESET;     // top center port PORTC_04: <- P01.CPU_RESET

  PinOut PIN_TO_CPU2;       // top right wire by itself <- P02.AWOB

  PinIn  PIN_CPU_RAW_RD;    // top right port PORTA_00: -> P07.UJYV, P08.LAGU, P08.LAVO
  PinIn  PIN_CPU_RAW_WR;    // top right port PORTA_01: -> P01.AREV, P08.LAGU.           This is almost definitely "raw write"
  PinOut PIN_SYRO;          // top right port PORTA_03: <- P25.SYRO
  PinOut PIN_READ_BOOTROM;  // top right port PORTA_04: <- P07.READ_BOOTROM tutu?
  PinIn  PIN_ADDR_VALID;    // top right port PORTA_06: -> TEXO, APAP       This is almost definitely "address valid", but not sure of polarity.

  PinOut PIN_INT_VBLANK;    // bottom right port PORTB_03: <-        P02.LOPE, vblank int
  PinOut PIN_INT_STAT;      // bottom right port PORTB_07: <-        P02.LALU, stat int
  PinOut PIN_INT_TIMER;     // bottom right port PORTB_11: <-        P02.NYBO, timer int
  PinOut PIN_INT_SERIAL;    // bottom right port PORTB_15: <-        P02.UBUL, serial int
  PinOut PIN_INT_JOYPAD;    // bottom right port PORTB_19: <-        P02.ULAK, joypad int

  PinIn  PIN_ACK_VBLANK;    // bottom right port PORTB_01: ->        P02.LETY, vblank int ack
  PinIn  PIN_ACK_STAT;      // bottom right port PORTB_05: ->        P02.LEJA, stat int ack
  PinIn  PIN_ACK_TIMER;     // bottom right port PORTB_09: ->        P02.LESA, timer int ack
  PinIn  PIN_ACK_SERIAL;    // bottom right port PORTB_13: ->        P02.LUFE, serial int ack
  PinIn  PIN_ACK_JOYPAD;    // bottom right port PORTB_17: ->        P02.LAMO, joypad int ack

  PinIn  A00; // bottom right port PORTB_00: -> A00
  PinIn  A01; // bottom right port PORTB_04: -> A01
  PinIn  A02; // bottom right port PORTB_08: -> A02
  PinIn  A03; // bottom right port PORTB_12: -> A03
  PinIn  A04; // bottom right port PORTB_16: -> A04
  PinIn  A05; // bottom right port PORTB_20: -> A05
  PinIn  A06; // bottom right port PORTB_24: -> A06
  PinIn  A07; // bottom right port PORTB_28: -> A07
  PinIn  A08; // bottom right port PORTB_02: -> A08
  PinIn  A09; // bottom right port PORTB_06: -> A09
  PinIn  A10; // bottom right port PORTB_10: -> A10
  PinIn  A11; // bottom right port PORTB_14: -> A11
  PinIn  A12; // bottom right port PORTB_18: -> A12
  PinIn  A13; // bottom right port PORTB_22: -> A13
  PinIn  A14; // bottom right port PORTB_26: -> A14
  PinIn  A15; // bottom right port PORTB_30: -> A15

  //----------
  // bottom left port, tristate data bus

  Tribuf D0;
  Tribuf D1;
  Tribuf D2;
  Tribuf D3;
  Tribuf D4;
  Tribuf D5;
  Tribuf D6;
  Tribuf D7;
};


//-----------------------------------------------------------------------------

}; // namespace Schematics
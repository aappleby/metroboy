#pragma once
#include "Sch_Common.h"

namespace Schematics {

struct TestGB;

//-----------------------------------------------------------------------------

struct CpuBusSignals {
  /*p07.TEDO*/ wire TEDO_CPU_RD;
  /*p07.ASOT*/ wire ASOT_CPU_RD;

  /*p08.MOCA*/ wire MOCA_DBG_EXT_RD;
  /*p08.MOTY*/ wire MOTY_CPU_EXT_RD;

  /*p07.TAPU*/ wire TAPU_CPU_WR_xxxxxFGH;
  /*p07.CUPA*/ wire CUPA_CPU_WR_xxxxxFGH;
  /*p01.APOV*/ wire APOV_CPU_WR_xxxxxFGH;

  /*p04.DECY*/ wire DECY_FROM_CPU5n;
  /*p28.LEKO*/ wire LEKO_CPU_RDp;

  /*p08.MATE*/ wire MATE_LATCH_CPU_ADDRp;
  /*p08.LAVO*/ wire LAVO_LATCH_CPU_DATAp;

  /*p08.RORU*/ wire RORU_IBUS_TO_EBUSn;

  /*p03.TOVY*/ bool TOVY_A00n;
  /*p08.TOLA*/ bool TOLA_A01n;
  /*p06.SEFY*/ bool SEFY_A02n;
  /*p07.TONA*/ bool TONA_A08n;

  /*p22.XOLA*/ bool XOLA_A00n;
  /*p22.XENO*/ bool XENO_A01n;
  /*p22.XUSY*/ bool XUSY_A02n;
  /*p22.XERA*/ bool XERA_A03n;
  /*p10.BYKO*/ bool BYKO_A05n;
  /*p10.AKUG*/ bool AKUG_A06n;

  /*p22.WADO*/ bool WADO_A00p;
  /*p22.WESA*/ bool WESA_A01p;
  /*p22.WALO*/ bool WALO_A02p;
  /*p22.WEPO*/ bool WEPO_A03p;

  /*p07.TUNA*/ bool TUNA_0000_FDFFp;
  /*p06.SARE*/ bool SARE_XX00_XX07p;
  /*p07.SYKE*/ bool SYKE_FF00_FFFFp;
  /*p25.SYRO*/ bool SYRO_FE00_FFFFp;
  /*p03.RYFO*/ bool RYFO_FF04_FF07p;
  /*p07.SARO*/ bool SARO_FE00_FEFFp;
  /*p22.WERO*/ bool WERO_FF40_FF4Fp;
  /*p25.SOSE*/ bool SOSE_8000_9FFFp;
  /*p08.TEXO*/ bool TEXO_8000_9FFFn;
  /*p08.LEVO*/ bool LEVO_8000_9FFFp;
};

//-----------------------------------------------------------------------------

struct CpuBus {

  CpuBusSignals sig(const TestGB& gb) const;
  bool commit();

  int get_data() const {
    return pack(TRI_D0,
                TRI_D1,
                TRI_D2,
                TRI_D3,
                TRI_D4,
                TRI_D5,
                TRI_D6,
                TRI_D7);
  }

  void set_data(bool oe, uint8_t data) {
    TRI_D0.set_tribuf(oe, data & 0x01);
    TRI_D1.set_tribuf(oe, data & 0x02);
    TRI_D2.set_tribuf(oe, data & 0x04);
    TRI_D3.set_tribuf(oe, data & 0x08);
    TRI_D4.set_tribuf(oe, data & 0x10);
    TRI_D5.set_tribuf(oe, data & 0x20);
    TRI_D6.set_tribuf(oe, data & 0x40);
    TRI_D7.set_tribuf(oe, data & 0x80);
  }

  int get_addr() const {
    return pack(PIN_A00, PIN_A01, PIN_A02, PIN_A03, PIN_A04, PIN_A05, PIN_A06, PIN_A07,
      PIN_A08, PIN_A09, PIN_A10, PIN_A11, PIN_A12, PIN_A13, PIN_A14, PIN_A15);
  }

  void preset_rd(bool rd)            { PIN_CPU_RAW_RD.preset(true, rd); }
  bool preset_wr(bool wr)            { PIN_CPU_RAW_WR.preset(true, wr); }
  bool preset_addr_valid(bool valid) { PIN_ADDR_VALID.preset(true, valid); }
  void preset_addr(bool oe, uint16_t addr) {
    PIN_A00.preset(oe, addr & 0x0001);
    PIN_A01.preset(oe, addr & 0x0002);
    PIN_A02.preset(oe, addr & 0x0004);
    PIN_A03.preset(oe, addr & 0x0008);
    PIN_A04.preset(oe, addr & 0x0010);
    PIN_A05.preset(oe, addr & 0x0020);
    PIN_A06.preset(oe, addr & 0x0040);
    PIN_A07.preset(oe, addr & 0x0080);
    PIN_A08.preset(oe, addr & 0x0100);
    PIN_A09.preset(oe, addr & 0x0200);
    PIN_A10.preset(oe, addr & 0x0400);
    PIN_A11.preset(oe, addr & 0x0800);
    PIN_A12.preset(oe, addr & 0x1000);
    PIN_A13.preset(oe, addr & 0x2000);
    PIN_A14.preset(oe, addr & 0x4000);
    PIN_A15.preset(oe, addr & 0x8000);
  }

  //----------
  // bottom left port, tristate data bus

  Tribuf TRI_D0;
  Tribuf TRI_D1;
  Tribuf TRI_D2;
  Tribuf TRI_D3;
  Tribuf TRI_D4;
  Tribuf TRI_D5;
  Tribuf TRI_D6;
  Tribuf TRI_D7;

  PinIn  PIN_CPU_RAW_RD; // top right port PORTA_00: -> P07.UJYV, P08.LAGU, P08.LAVO
  PinIn  PIN_CPU_RAW_WR; // top right port PORTA_01: -> P01.AREV, P08.LAGU.           This is almost definitely "raw write"
  PinIn  PIN_ADDR_VALID; // top right port PORTA_06: -> TEXO, APAP       This is almost definitely "address valid", but not sure of polarity.
  PinIn  PIN_FROM_CPU5p; // top left port PORTD_06: -> ANUJ (FROM_CPU5). Maybe this means "latch the bus"?
  PinIn  PIN_FROM_CPU6;  // top left port PORTD_00: -> LEXY, doesn't do anything. FROM_CPU6? 
  PinIn  PIN_CLKREQ;     // top center port PORTC_00: -> ABOL (an inverter) -> BATE. Something about "cpu ready". clock request?

  PinIn  PIN_A00; // bottom right port PORTB_00: -> A00
  PinIn  PIN_A01; // bottom right port PORTB_04: -> A01
  PinIn  PIN_A02; // bottom right port PORTB_08: -> A02
  PinIn  PIN_A03; // bottom right port PORTB_12: -> A03
  PinIn  PIN_A04; // bottom right port PORTB_16: -> A04
  PinIn  PIN_A05; // bottom right port PORTB_20: -> A05
  PinIn  PIN_A06; // bottom right port PORTB_24: -> A06
  PinIn  PIN_A07; // bottom right port PORTB_28: -> A07
  PinIn  PIN_A08; // bottom right port PORTB_02: -> A08
  PinIn  PIN_A09; // bottom right port PORTB_06: -> A09
  PinIn  PIN_A10; // bottom right port PORTB_10: -> A10
  PinIn  PIN_A11; // bottom right port PORTB_14: -> A11
  PinIn  PIN_A12; // bottom right port PORTB_18: -> A12
  PinIn  PIN_A13; // bottom right port PORTB_22: -> A13
  PinIn  PIN_A14; // bottom right port PORTB_26: -> A14
  PinIn  PIN_A15; // bottom right port PORTB_30: -> A15
};

//-----------------------------------------------------------------------------

struct CpuPinsOut {

  void tick(TestGB& gb);
  bool commit();

  void dump_pins(TextPainter& text_painter) {
    text_painter.dprintf("----- CPU DBG/PIN_RST -----\n");
    text_painter.dprintf("PIN_EXT_RESET     %d\n", PIN_EXT_RESET.a.val);
    text_painter.dprintf("PIN_TABA_RSTp     %d\n", PIN_TABA_RSTp.a.val);
    text_painter.dprintf("PIN_EXT_CLKGOOD   %d\n", PIN_EXT_CLKGOOD.a.val);

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
    //text_painter.dprintf("PIN_CLKREQ        %d\n", PIN_CLKREQ.a.val);
    //text_painter.dprintf("PIN_FROM_CPU5p     %d\n", PIN_FROM_CPU5p.a.val);
    //text_painter.dprintf("PIN_FROM_CPU6     %d\n", PIN_FROM_CPU6.a.val);

    text_painter.dprintf("----- TO CPU -----\n");
    text_painter.dprintf("PIN_AFER_RSTp     %d\n", PIN_AFER_RSTp.a.val);
    text_painter.dprintf("PIN_AWOB          %d\n", PIN_AWOB.a.val);
    text_painter.dprintf("PIN_SYRO          %d\n", PIN_SYRO.a.val);
    text_painter.dprintf("PIN_TUTU_BOOTp    %d\n", PIN_TUTU_BOOTp.a.val);

    /*
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
    */

    /*
    text_painter.dprintf("----- CPU BUS -----\n");
    text_painter.dprintf("PIN_CPU_RAW_RD    %d\n", PIN_CPU_RAW_RD.a.val);
    text_painter.dprintf("PIN_CPU_RAW_WR    %d\n", PIN_CPU_RAW_WR.a.val);
    text_painter.dprintf("PIN_ADDR_VALID    %d\n", PIN_ADDR_VALID.a.val);
    */

    /*
    text_painter.add_text("Axx ");
    dump2(text_painter, PIN_A15.a);
    dump2(text_painter, PIN_A14.a);
    dump2(text_painter, PIN_A13.a);
    dump2(text_painter, PIN_A12.a);
    text_painter.add_char(':');
    dump2(text_painter, PIN_A11.a);
    dump2(text_painter, PIN_A10.a);
    dump2(text_painter, PIN_A09.a);
    dump2(text_painter, PIN_A08.a);
    text_painter.add_char(':');
    dump2(text_painter, PIN_A07.a);
    dump2(text_painter, PIN_A06.a);
    dump2(text_painter, PIN_A05.a);
    dump2(text_painter, PIN_A04.a);
    text_painter.add_char(':');
    dump2(text_painter, PIN_A03.a);
    dump2(text_painter, PIN_A02.a);
    dump2(text_painter, PIN_A01.a);
    dump2(text_painter, PIN_A00.a);
    text_painter.newline();
    */

    /*
    text_painter.add_text("Dxx ");
    dump2(text_painter, BUS_CPU_D7.a);
    dump2(text_painter, BUS_CPU_D6.a);
    dump2(text_painter, BUS_CPU_D5.a);
    dump2(text_painter, BUS_CPU_D4.a);
    text_painter.add_char(':');
    dump2(text_painter, BUS_CPU_D3.a);
    dump2(text_painter, BUS_CPU_D2.a);
    dump2(text_painter, BUS_CPU_D1.a);
    dump2(text_painter, BUS_CPU_D0.a);
    text_painter.newline();
    */

    text_painter.newline();
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

  //----------

private:

  PinOut PIN_BOWA_AxCDEFGH; // top left port PORTD_01: <- BOWA_AxCDEFGH // Blue clock - decoders, alu, some reset stuff
  PinOut PIN_BEDO_xBxxxxxx; // top left port PORTD_02: <- BEDO_xBxxxxxx

  PinOut PIN_BEKO_xBCDExxx; // top left port PORTD_03: <- BEKO_ABCDxxxx + BAVY connection not indicated on P01 - test pad 1
  PinOut PIN_BUDE_AxxxxFGH; // top left port PORTD_04: <- BUDE_AxxxxFGH + BEVA

  PinOut PIN_BOLO_xBCDEFGx; // top left port PORTD_05: <- BOLO_ABCDEFxx + BYDA? - test pad 2
  PinOut PIN_BUKE_ABxxxxxH; // top left port PORTD_07: <- BUKE_ABxxxxxH

  PinOut PIN_BOMA_xBxxxxxx; // top left port PORTD_08: <- BOMA_xBxxxxxx (RESET_CLK)
  PinOut PIN_BOGA_AxCDEFGH; // top left port PORTD_09: <- BOGA_AxCDEFGH - test pad 3

  PinOut PIN_AFER_RSTp;     // top center port PORTC_01: <- P01.AFER , reset related reg
  PinOut PIN_EXT_RESET;     // top center port PORTC_02: <- PIN_RESET directly connected to the pad
  PinOut PIN_EXT_CLKGOOD;   // top center port PORTC_03: <- chip.CLKIN_A top wire on PAD_XI,
  PinOut PIN_TABA_RSTp;     // top center port PORTC_04: <- P01.CPU_RESET

  PinOut PIN_AWOB;          // top right wire by itself <- P02.AWOB

  PinOut PIN_SYRO;          // top right port PORTA_03: <- P25.SYRO
  PinOut PIN_TUTU_BOOTp;    // top right port PORTA_04: <- P07.READ_BOOTROM tutu?

};

//-----------------------------------------------------------------------------

}; // namespace Schematics
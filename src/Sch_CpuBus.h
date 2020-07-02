#pragma once
#include "Sch_Common.h"

namespace Schematics {

struct TestGB;

//-----------------------------------------------------------------------------

struct CpuBusSignals {
  /*p07.TEDO*/ Signal TEDO_CPU_RD;
  /*p07.ASOT*/ Signal ASOT_CPU_RD;

  /*p08.MOCA*/ Signal MOCA_DBG_EXT_RD;
  /*p08.MOTY*/ Signal MOTY_CPU_EXT_RD;

  /*p07.TAPU*/ Signal TAPU_CPU_WR_xxxxxFGH;
  /*p07.CUPA*/ Signal CUPA_CPU_WR_xxxxxFGH;
  /*p01.APOV*/ Signal APOV_CPU_WR_xxxxxFGH;

  /*p04.DECY*/ Signal DECY_FROM_CPU5n;
  /*p28.LEKO*/ Signal LEKO_CPU_RDp;

  /*p08.MATE*/ Signal MATE_LATCH_CPU_ADDRp;
  /*p08.LAVO*/ Signal LAVO_LATCH_CPU_DATAp;

  /*p08.RORU*/ Signal RORU_IBUS_TO_EBUSn;

  /*p03.TOVY*/ Signal TOVY_A00n;
  /*p08.TOLA*/ Signal TOLA_A01n;
  /*p06.SEFY*/ Signal SEFY_A02n;
  /*p07.TONA*/ Signal TONA_A08n;

  /*p22.XOLA*/ Signal XOLA_A00n;
  /*p22.XENO*/ Signal XENO_A01n;
  /*p22.XUSY*/ Signal XUSY_A02n;
  /*p22.XERA*/ Signal XERA_A03n;
  /*p10.BYKO*/ Signal BYKO_A05n;
  /*p10.AKUG*/ Signal AKUG_A06n;

  /*p22.WADO*/ Signal WADO_A00p;
  /*p22.WESA*/ Signal WESA_A01p;
  /*p22.WALO*/ Signal WALO_A02p;
  /*p22.WEPO*/ Signal WEPO_A03p;

  /*p07.TUNA*/ Signal TUNA_0000_FDFFp;
  /*p06.SARE*/ Signal SARE_XX00_XX07p;
  /*p07.SYKE*/ Signal SYKE_FF00_FFFFp;
  /*p25.SYRO*/ Signal SYRO_FE00_FFFFp;
  /*p03.RYFO*/ Signal RYFO_FF04_FF07p;
  /*p07.SARO*/ Signal SARO_FE00_FEFFp;
  /*p22.WERO*/ Signal WERO_FF40_FF4Fp;
  /*p25.SOSE*/ Signal SOSE_8000_9FFFp;
  /*p08.TEXO*/ Signal TEXO_8000_9FFFn;
  /*p08.LEVO*/ Signal LEVO_8000_9FFFp;
};

//-----------------------------------------------------------------------------

struct CpuBus {

  CpuBusSignals sig(const TestGB& gb) const;
  void tick(TestGB& gb);
  bool commit();

  int get_data() const {
    return pack(TRI_D0, TRI_D1, TRI_D2, TRI_D3, TRI_D4, TRI_D5, TRI_D6, TRI_D7);
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

  /*p08.ALOR*/ TpLatch CPU_ADDR_LATCH_00;
  /*p08.APUR*/ TpLatch CPU_ADDR_LATCH_01;
  /*p08.ALYR*/ TpLatch CPU_ADDR_LATCH_02;
  /*p08.ARET*/ TpLatch CPU_ADDR_LATCH_03;
  /*p08.AVYS*/ TpLatch CPU_ADDR_LATCH_04;
  /*p08.ATEV*/ TpLatch CPU_ADDR_LATCH_05;
  /*p08.AROS*/ TpLatch CPU_ADDR_LATCH_06;
  /*p08.ARYM*/ TpLatch CPU_ADDR_LATCH_07;
  /*p08.LUNO*/ TpLatch CPU_ADDR_LATCH_08;
  /*p08.LYSA*/ TpLatch CPU_ADDR_LATCH_09;
  /*p08.PATE*/ TpLatch CPU_ADDR_LATCH_10;
  /*p08.LUMY*/ TpLatch CPU_ADDR_LATCH_11;
  /*p08.LOBU*/ TpLatch CPU_ADDR_LATCH_12;
  /*p08.LONU*/ TpLatch CPU_ADDR_LATCH_13;
  /*p08.NYRE*/ TpLatch CPU_ADDR_LATCH_14;

  /*p04.MAKA*/ Reg MAKA_FROM_CPU5_SYNC;

private:

  //----------
  // Data pins

  /* PIN_17 */ PinOut PIN_D0_A;    // <- P08.RUXA
  /* PIN_17 */ PinOut PIN_D0_B;    // <- P08.LULA
  /* PIN_17 */ PinOut PIN_D0_D;    // <- P08.RUNE
  /* PIN_18 */ PinOut PIN_D1_A;    // <- P08.RUJA
  /* PIN_18 */ PinOut PIN_D1_B;    // <- P08.LULA
  /* PIN_18 */ PinOut PIN_D1_D;    // <- P08.RYPU
  /* PIN_19 */ PinOut PIN_D2_A;    // <- P08.RABY
  /* PIN_19 */ PinOut PIN_D2_B;    // <- P08.LULA
  /* PIN_19 */ PinOut PIN_D2_D;    // <- P08.SULY
  /* PIN_20 */ PinOut PIN_D3_A;    // <- P08.RERA
  /* PIN_20 */ PinOut PIN_D3_B;    // <- P08.LULA
  /* PIN_20 */ PinOut PIN_D3_D;    // <- P08.SEZE
  /* PIN_21 */ PinOut PIN_D4_A;    // <- P08.RORY
  /* PIN_21 */ PinOut PIN_D4_B;    // <- P08.LULA
  /* PIN_21 */ PinOut PIN_D4_D;    // <- P08.RESY
  /* PIN_22 */ PinOut PIN_D5_A;    // <- P08.RYVO
  /* PIN_22 */ PinOut PIN_D5_B;    // <- P08.LULA
  /* PIN_22 */ PinOut PIN_D5_D;    // <- P08.TAMU
  /* PIN_23 */ PinOut PIN_D6_A;    // <- P08.RAFY
  /* PIN_23 */ PinOut PIN_D6_B;    // <- P08.LULA
  /* PIN_23 */ PinOut PIN_D6_D;    // <- P08.ROGY
  /* PIN_24 */ PinOut PIN_D7_A;    // <- P08.RAVU
  /* PIN_24 */ PinOut PIN_D7_B;    // <- P08.LULA
  /* PIN_24 */ PinOut PIN_D7_D;    // <- P08.RYDA
};

//-----------------------------------------------------------------------------

struct CpuPinsOut {

  void tick(TestGB& gb);
  bool commit();

  //----------

private:

  //PinOut UNOR_MODE_DBG2;         // top right port PORTA_02: <- P07.UNOR_MODE_DBG2
  //PinOut UMUT_MODE_DBG1;         // top right port PORTA_05: <- P07.UMUT_MODE_DBG1

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
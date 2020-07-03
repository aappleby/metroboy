#pragma once
#include "Sch_Common.h"

namespace Schematics {

//-----------------------------------------------------------------------------

struct CpuBusSignals {
  /*p07.UJYV*/ Signal UJYV_CPU_RD; // main read
  /*p07.UBAL*/ Signal UBAL_CPU_WRp_ABCDExxx; // write with debug mux, goes everywhere
};

//-----------------------------------------------------------------------------

struct CpuBus {

  CpuBusSignals sig(const SchematicTop& gb) const;
  CpuBusSignals sig(const ClockSignals& clk_sig, const DebugSignals& dbg_sig) const;

  void tick(SchematicTop& gb);
  
  SignalHash commit();

  int get_data() const {
    return pack(CPU_TRI_D0, CPU_TRI_D1, CPU_TRI_D2, CPU_TRI_D3, CPU_TRI_D4, CPU_TRI_D5, CPU_TRI_D6, CPU_TRI_D7);
  }

  void set_data(bool oe, uint8_t data) {
    CPU_TRI_D0.set_tribuf(oe, data & 0x01);
    CPU_TRI_D1.set_tribuf(oe, data & 0x02);
    CPU_TRI_D2.set_tribuf(oe, data & 0x04);
    CPU_TRI_D3.set_tribuf(oe, data & 0x08);
    CPU_TRI_D4.set_tribuf(oe, data & 0x10);
    CPU_TRI_D5.set_tribuf(oe, data & 0x20);
    CPU_TRI_D6.set_tribuf(oe, data & 0x40);
    CPU_TRI_D7.set_tribuf(oe, data & 0x80);
  }

  int get_addr() const {
    return pack(CPU_PIN_A00, CPU_PIN_A01, CPU_PIN_A02, CPU_PIN_A03, CPU_PIN_A04, CPU_PIN_A05, CPU_PIN_A06, CPU_PIN_A07,
      CPU_PIN_A08, CPU_PIN_A09, CPU_PIN_A10, CPU_PIN_A11, CPU_PIN_A12, CPU_PIN_A13, CPU_PIN_A14, CPU_PIN_A15);
  }

  void preset_rd(bool rd)            { CPU_PIN_RD.preset(true, rd); }
  bool preset_wr(bool wr)            { CPU_PIN_WR.preset(true, wr); }
  bool preset_addr_valid(bool valid) { CPU_PIN_ADDR_VALID.preset(true, valid); }
  void preset_addr(bool oe, uint16_t addr) {
    CPU_PIN_A00.preset(oe, addr & 0x0001);
    CPU_PIN_A01.preset(oe, addr & 0x0002);
    CPU_PIN_A02.preset(oe, addr & 0x0004);
    CPU_PIN_A03.preset(oe, addr & 0x0008);
    CPU_PIN_A04.preset(oe, addr & 0x0010);
    CPU_PIN_A05.preset(oe, addr & 0x0020);
    CPU_PIN_A06.preset(oe, addr & 0x0040);
    CPU_PIN_A07.preset(oe, addr & 0x0080);
    CPU_PIN_A08.preset(oe, addr & 0x0100);
    CPU_PIN_A09.preset(oe, addr & 0x0200);
    CPU_PIN_A10.preset(oe, addr & 0x0400);
    CPU_PIN_A11.preset(oe, addr & 0x0800);
    CPU_PIN_A12.preset(oe, addr & 0x1000);
    CPU_PIN_A13.preset(oe, addr & 0x2000);
    CPU_PIN_A14.preset(oe, addr & 0x4000);
    CPU_PIN_A15.preset(oe, addr & 0x8000);
  }

  //----------
  // bottom left port, tristate data bus

  PinIn  CPU_PIN_RD;         // top right port PORTA_00
  PinIn  CPU_PIN_WR;         // top right port PORTA_01
  PinIn  CPU_PIN_ADDR_VALID; // top right port PORTA_06: -> TEXO, APAP       This is almost definitely "address valid", but not sure of polarity.
  PinIn  CPU_PIN5;           // top left port PORTD_06: -> ANUJ (FROM_CPU5). Maybe this means "latch the bus"?
  PinIn  CPU_PIN6;           // top left port PORTD_00: -> LEXY, doesn't do anything. FROM_CPU6? 
  PinIn  CPU_PIN_CLKREQ;     // top center port PORTC_00: -> ABOL (an inverter) -> BATE. Something about "cpu ready". clock request?

  PinIn  CPU_PIN_A00; // bottom right port PORTB_00: -> A00
  PinIn  CPU_PIN_A01; // bottom right port PORTB_04: -> A01
  PinIn  CPU_PIN_A02; // bottom right port PORTB_08: -> A02
  PinIn  CPU_PIN_A03; // bottom right port PORTB_12: -> A03
  PinIn  CPU_PIN_A04; // bottom right port PORTB_16: -> A04
  PinIn  CPU_PIN_A05; // bottom right port PORTB_20: -> A05
  PinIn  CPU_PIN_A06; // bottom right port PORTB_24: -> A06
  PinIn  CPU_PIN_A07; // bottom right port PORTB_28: -> A07
  PinIn  CPU_PIN_A08; // bottom right port PORTB_02: -> A08
  PinIn  CPU_PIN_A09; // bottom right port PORTB_06: -> A09
  PinIn  CPU_PIN_A10; // bottom right port PORTB_10: -> A10
  PinIn  CPU_PIN_A11; // bottom right port PORTB_14: -> A11
  PinIn  CPU_PIN_A12; // bottom right port PORTB_18: -> A12
  PinIn  CPU_PIN_A13; // bottom right port PORTB_22: -> A13
  PinIn  CPU_PIN_A14; // bottom right port PORTB_26: -> A14
  PinIn  CPU_PIN_A15; // bottom right port PORTB_30: -> A15

  Tribuf CPU_TRI_D0;
  Tribuf CPU_TRI_D1;
  Tribuf CPU_TRI_D2;
  Tribuf CPU_TRI_D3;
  Tribuf CPU_TRI_D4;
  Tribuf CPU_TRI_D5;
  Tribuf CPU_TRI_D6;
  Tribuf CPU_TRI_D7;

private:

  /* PIN_01 */ PinIn EXT_PIN_A00_C;   // -> P08.KOVA
  /* PIN_02 */ PinIn EXT_PIN_A01_C;   // -> P08.CAMU
  /* PIN_03 */ PinIn EXT_PIN_A02_C;   // -> P08.BUXU
  /* PIN_04 */ PinIn EXT_PIN_A03_C;   // -> P08.BASE
  /* PIN_05 */ PinIn EXT_PIN_A04_C;   // -> P08.AFEC
  /* PIN_06 */ PinIn EXT_PIN_A05_C;   // -> P08.ABUP
  /* PIN_07 */ PinIn EXT_PIN_A06_C;   // -> P08.CYGU
  /* PIN_08 */ PinIn EXT_PIN_A07_C;   // -> P08.COGO
  /* PIN_09 */ PinIn EXT_PIN_A08_C;   // -> P08.MUJY
  /* PIN_10 */ PinIn EXT_PIN_A09_C;   // -> P08.NENA
  /* PIN_11 */ PinIn EXT_PIN_A10_C;   // -> P08.SURA
  /* PIN_12 */ PinIn EXT_PIN_A11_C;   // -> P08.MADY
  /* PIN_13 */ PinIn EXT_PIN_A12_C;   // -> P08.LAHE
  /* PIN_14 */ PinIn EXT_PIN_A13_C;   // -> P08.LURA
  /* PIN_15 */ PinIn EXT_PIN_A14_C;   // -> P08.PEVO
  /* PIN_16 */ PinIn EXT_PIN_A15_C;   // -> P08.RAZA


  /* PIN_79 */ PinIn EXT_PIN_RD_C;   // -> P07.UJYV
  /* PIN_78 */ PinIn EXT_PIN_WRn_C;   // -> P07.UBAL

  //----------
  // Ext-to-cpu "latch" - looks more like a pass gate really

  /*p08.SOMA*/ TpLatch SOMA_EXT_DATA_LATCH_00;
  /*p08.RONY*/ TpLatch RONY_EXT_DATA_LATCH_01;
  /*p08.RAXY*/ TpLatch RAXY_EXT_DATA_LATCH_02;
  /*p08.SELO*/ TpLatch SELO_EXT_DATA_LATCH_03;
  /*p08.SODY*/ TpLatch SODY_EXT_DATA_LATCH_04;
  /*p08.SAGO*/ TpLatch SAGO_EXT_DATA_LATCH_05;
  /*p08.RUPA*/ TpLatch RUPA_EXT_DATA_LATCH_06;
  /*p08.SAZY*/ TpLatch SAZY_EXT_DATA_LATCH_07;

  //----------
  // Data pins

  /* PIN_17 */ PinOut EXT_PIN_D0_A;    // <- P08.RUXA
  /* PIN_17 */ PinOut EXT_PIN_D0_B;    // <- P08.LULA
  /* PIN_17 */ PinOut EXT_PIN_D0_D;    // <- P08.RUNE
  /* PIN_18 */ PinOut EXT_PIN_D1_A;    // <- P08.RUJA
  /* PIN_18 */ PinOut EXT_PIN_D1_B;    // <- P08.LULA
  /* PIN_18 */ PinOut EXT_PIN_D1_D;    // <- P08.RYPU
  /* PIN_19 */ PinOut EXT_PIN_D2_A;    // <- P08.RABY
  /* PIN_19 */ PinOut EXT_PIN_D2_B;    // <- P08.LULA
  /* PIN_19 */ PinOut EXT_PIN_D2_D;    // <- P08.SULY
  /* PIN_20 */ PinOut EXT_PIN_D3_A;    // <- P08.RERA
  /* PIN_20 */ PinOut EXT_PIN_D3_B;    // <- P08.LULA
  /* PIN_20 */ PinOut EXT_PIN_D3_D;    // <- P08.SEZE
  /* PIN_21 */ PinOut EXT_PIN_D4_A;    // <- P08.RORY
  /* PIN_21 */ PinOut EXT_PIN_D4_B;    // <- P08.LULA
  /* PIN_21 */ PinOut EXT_PIN_D4_D;    // <- P08.RESY
  /* PIN_22 */ PinOut EXT_PIN_D5_A;    // <- P08.RYVO
  /* PIN_22 */ PinOut EXT_PIN_D5_B;    // <- P08.LULA
  /* PIN_22 */ PinOut EXT_PIN_D5_D;    // <- P08.TAMU
  /* PIN_23 */ PinOut EXT_PIN_D6_A;    // <- P08.RAFY
  /* PIN_23 */ PinOut EXT_PIN_D6_B;    // <- P08.LULA
  /* PIN_23 */ PinOut EXT_PIN_D6_D;    // <- P08.ROGY
  /* PIN_24 */ PinOut EXT_PIN_D7_A;    // <- P08.RAVU
  /* PIN_24 */ PinOut EXT_PIN_D7_B;    // <- P08.LULA
  /* PIN_24 */ PinOut EXT_PIN_D7_D;    // <- P08.RYDA
};

//-----------------------------------------------------------------------------

struct CpuPinsOut {

  void tick(SchematicTop& gb);
  void commit(SignalHash& hash);

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
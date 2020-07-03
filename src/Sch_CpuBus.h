#pragma once
#include "Sch_Common.h"

namespace Schematics {

//-----------------------------------------------------------------------------

struct CpuBus {

  void tick(SchematicTop& gb);
  SignalHash commit();

  //----------
  // bottom left port, tristate data bus

private:

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

  PinOut PIN_EXT_RESET;     // top center port PORTC_02: <- PIN_RESET directly connected to the pad
  PinOut PIN_EXT_CLKGOOD;   // top center port PORTC_03: <- chip.CLKIN_A top wire on PAD_XI,

  PinOut PIN_AWOB;          // top right wire by itself <- P02.AWOB

  PinOut PIN_SYRO;          // top right port PORTA_03: <- P25.SYRO
};

//-----------------------------------------------------------------------------

}; // namespace Schematics
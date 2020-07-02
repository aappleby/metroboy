#pragma once
#include "Sch_Common.h"

namespace Schematics {

struct TestGB;

//-----------------------------------------------------------------------------

struct ExtPinsIn {

  bool commit();

  /* PIN_71 */ PinIn PIN_RST;
  /* PIN_74 */ PinIn PIN_CLK_GOOD;
  /* PIN_74 */ PinIn PIN_CLK_IN_xBxDxFxH;
  /* PIN_76 */ PinIn PIN_T2;
  /* PIN_77 */ PinIn PIN_T1;

  /* PIN_79 */ PinIn PIN_RD_C;   // -> P07.UJYV
  /* PIN_78 */ PinIn PIN_WR_C;   // -> P07.UBAL

  /* PIN_01 */ PinIn PIN_A00_C;   // -> P08.KOVA
  /* PIN_02 */ PinIn PIN_A01_C;   // -> P08.CAMU
  /* PIN_03 */ PinIn PIN_A02_C;   // -> P08.BUXU
  /* PIN_04 */ PinIn PIN_A03_C;   // -> P08.BASE
  /* PIN_05 */ PinIn PIN_A04_C;   // -> P08.AFEC
  /* PIN_06 */ PinIn PIN_A05_C;   // -> P08.ABUP
  /* PIN_07 */ PinIn PIN_A06_C;   // -> P08.CYGU
  /* PIN_08 */ PinIn PIN_A07_C;   // -> P08.COGO
  /* PIN_09 */ PinIn PIN_A08_C;   // -> P08.MUJY
  /* PIN_10 */ PinIn PIN_A09_C;   // -> P08.NENA
  /* PIN_11 */ PinIn PIN_A10_C;   // -> P08.SURA
  /* PIN_12 */ PinIn PIN_A11_C;   // -> P08.MADY
  /* PIN_13 */ PinIn PIN_A12_C;   // -> P08.LAHE
  /* PIN_14 */ PinIn PIN_A13_C;   // -> P08.LURA
  /* PIN_15 */ PinIn PIN_A14_C;   // -> P08.PEVO
  /* PIN_16 */ PinIn PIN_A15_C;   // -> P08.RAZA

  /* PIN_17 */ PinIn PIN_D0_C;    // -> P08.TOVO,SOMA
  /* PIN_18 */ PinIn PIN_D1_C;    // -> P08.RUZY,RONY
  /* PIN_19 */ PinIn PIN_D2_C;    // -> P08.ROME,RAXY
  /* PIN_20 */ PinIn PIN_D3_C;    // -> P08.SAZA,SELO
  /* PIN_21 */ PinIn PIN_D4_C;    // -> P08.TEHE,SODY
  /* PIN_22 */ PinIn PIN_D5_C;    // -> P08.RATU,SAGO
  /* PIN_23 */ PinIn PIN_D6_C;    // -> P08.SOCA,RUPA
  /* PIN_24 */ PinIn PIN_D7_C;    // -> P08.RYBA,SAZY
};

//-----------------------------------------------------------------------------


struct ExtPinsOut {

  void tick(TestGB& gb);
  bool commit();

private:

  //----------
  // Ext-to-cpu "latch" - looks more like a pass gate really

  /*p08.SOMA*/ TpLatch SOMA_CPU_DATA_LATCH_00;
  /*p08.RONY*/ TpLatch RONY_CPU_DATA_LATCH_01;
  /*p08.RAXY*/ TpLatch RAXY_CPU_DATA_LATCH_02;
  /*p08.SELO*/ TpLatch SELO_CPU_DATA_LATCH_03;
  /*p08.SODY*/ TpLatch SODY_CPU_DATA_LATCH_04;
  /*p08.SAGO*/ TpLatch SAGO_CPU_DATA_LATCH_05;
  /*p08.RUPA*/ TpLatch RUPA_CPU_DATA_LATCH_06;
  /*p08.SAZY*/ TpLatch SAZY_CPU_DATA_LATCH_07;
};

//-----------------------------------------------------------------------------

}; // namespace Schematics
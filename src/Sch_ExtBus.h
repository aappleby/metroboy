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

  //----------
  // Cart pins

  /* PIN_75 */ PinOut PIN_PHI;    // <- P01.BUDE/BEVA
  /* PIN_79 */ PinOut PIN_RD_A;   // <- P08.UGAC
  /* PIN_79 */ PinOut PIN_RD_D;   // <- P08.URUN
  /* PIN_78 */ PinOut PIN_WR_A;   // <- P08.UVER
  /* PIN_78 */ PinOut PIN_WR_D;   // <- P08.USUF
  /* PIN_80 */ PinOut PIN_CS_A;   // <- P08.TYHO

  //----------
  // Address pins

  /* PIN_01 */ PinOut PIN_A00_A;   // <- P08.KUPO
  /* PIN_01 */ PinOut PIN_A00_D;   // <- P08.KOTY
  /* PIN_02 */ PinOut PIN_A01_A;   // <- P08.CABA
  /* PIN_02 */ PinOut PIN_A01_D;   // <- P08.COTU
  /* PIN_03 */ PinOut PIN_A02_A;   // <- P08.BOKU
  /* PIN_03 */ PinOut PIN_A02_D;   // <- P08.BAJO
  /* PIN_04 */ PinOut PIN_A03_A;   // <- P08.BOTY
  /* PIN_04 */ PinOut PIN_A03_D;   // <- P08.BOLA
  /* PIN_05 */ PinOut PIN_A04_A;   // <- P08.BYLA
  /* PIN_05 */ PinOut PIN_A04_D;   // <- P08.BEVO
  /* PIN_06 */ PinOut PIN_A05_A;   // <- P08.BADU
  /* PIN_06 */ PinOut PIN_A05_D;   // <- P08.AJAV
  /* PIN_07 */ PinOut PIN_A06_A;   // <- P08.CEPU
  /* PIN_07 */ PinOut PIN_A06_D;   // <- P08.CYKA
  /* PIN_08 */ PinOut PIN_A07_A;   // <- P08.DEFY
  /* PIN_08 */ PinOut PIN_A07_D;   // <- P08.COLO
  /* PIN_09 */ PinOut PIN_A08_A;   // <- P08.MYNY
  /* PIN_09 */ PinOut PIN_A08_D;   // <- P08.MEGO
  /* PIN_10 */ PinOut PIN_A09_A;   // <- P08.MUNE
  /* PIN_10 */ PinOut PIN_A09_D;   // <- P08.MENY
  /* PIN_11 */ PinOut PIN_A10_A;   // <- P08.ROXU
  /* PIN_11 */ PinOut PIN_A10_D;   // <- P08.RORE
  /* PIN_12 */ PinOut PIN_A11_A;   // <- P08.LEPY
  /* PIN_12 */ PinOut PIN_A11_D;   // <- P08.LYNY
  /* PIN_13 */ PinOut PIN_A12_A;   // <- P08.LUCE
  /* PIN_13 */ PinOut PIN_A12_D;   // <- P08.LOSO
  /* PIN_14 */ PinOut PIN_A13_A;   // <- P08.LABE
  /* PIN_14 */ PinOut PIN_A13_D;   // <- P08.LEVA
  /* PIN_15 */ PinOut PIN_A14_A;   // <- P08.PUHE
  /* PIN_15 */ PinOut PIN_A14_D;   // <- P08.PAHY
  /* PIN_16 */ PinOut PIN_A15_A;   // <- P08.SUZE
  /* PIN_16 */ PinOut PIN_A15_D;   // <- P08.RULO

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

}; // namespace Schematics
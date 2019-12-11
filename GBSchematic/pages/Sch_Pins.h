#pragma once
#include "Schematics.h"

namespace Schematics {

//-----------------------------------------------------------------------------

struct Pins {

  /*p08.RORU*/ bool CBUS_TO_CEXTn;
  /*p08.LULA*/ bool CBUS_TO_CEXT;  // True if the internal data bus is driving the external data bus

  /*p08.LOXO*/ bool ADDR_LATCHb;
  /*p08.MATE*/ bool ADDR_LATCH;

  /*p08.ALOR*/ bool ADDR_LATCH_00;
  /*p08.APUR*/ bool ADDR_LATCH_01;
  /*p08.ALYR*/ bool ADDR_LATCH_02;
  /*p08.ARET*/ bool ADDR_LATCH_03;
  /*p08.AVYS*/ bool ADDR_LATCH_04;
  /*p08.ATEV*/ bool ADDR_LATCH_05;
  /*p08.AROS*/ bool ADDR_LATCH_06;
  /*p08.ARYM*/ bool ADDR_LATCH_07;
  /*p08.LUNO*/ bool ADDR_LATCH_08;
  /*p08.LYSA*/ bool ADDR_LATCH_09;
  /*p08.PATE*/ bool ADDR_LATCH_10;
  /*p08.LUMY*/ bool ADDR_LATCH_11;
  /*p08.LOBU*/ bool ADDR_LATCH_12;
  /*p08.LONU*/ bool ADDR_LATCH_13;
  /*p08.NYRE*/ bool ADDR_LATCH_14;
  /*p08.SEPY*/ bool ADDR_LATCH_15;

  /*p08.AMET*/ bool ADDR_MUX_00;
  /*p08.ATOL*/ bool ADDR_MUX_01;
  /*p08.APOK*/ bool ADDR_MUX_02;
  /*p08.AMER*/ bool ADDR_MUX_03;
  /*p08.ATEM*/ bool ADDR_MUX_04;
  /*p08.ATOV*/ bool ADDR_MUX_05;
  /*p08.ATYR*/ bool ADDR_MUX_06;
  /*p08.ASUR*/ bool ADDR_MUX_07;
  /*p08.MANO*/ bool ADDR_MUX_08;
  /*p08.MASU*/ bool ADDR_MUX_09;
  /*p08.PAMY*/ bool ADDR_MUX_10;
  /*p08.MALE*/ bool ADDR_MUX_11;
  /*p08.MOJY*/ bool ADDR_MUX_12;
  /*p08.MUCE*/ bool ADDR_MUX_13;
  /*p08.PEGE*/ bool ADDR_MUX_14;
  /*p08.TAZY*/ bool ADDR_MUX_15;

  /*p08.SOMA*/ bool LATCH_D0;
  /*p08.RONY*/ bool LATCH_D1;
  /*p08.RAXY*/ bool LATCH_D2;
  /*p08.SELO*/ bool LATCH_D3;
  /*p08.SODY*/ bool LATCH_D4;
  /*p08.SAGO*/ bool LATCH_D5;
  /*p08.RUPA*/ bool LATCH_D6;
  /*p08.SAZY*/ bool LATCH_D7;

  /* PIN_43 */ bool MCS_C;   // -> P25.TEFY
  /* PIN_45 */ bool MOE_C;   // -> P25.TAVY
  /* PIN_49 */ bool MWR_C;   // -> P25.SUDO
                             

  /* PIN_71 */ bool RST;     // -> TUBO,ASOL,UFOL,UPOJ
  /* PIN_74 */ bool CLKIN_A; // clock good signal
  /* PIN_74 */ bool CLKIN_B; // clock signal
  /* PIN_76 */ bool T2;      // -> P07.UVAR
  /* PIN_77 */ bool T1;      // -> P07.UBET
                             
  //----------
  // Cart pins

  /* PIN_75 */ bool PHI;

  /* PIN_78 */ bool WR_A;    // <- P08.UVER
  /* PIN_78 */ bool WR_C;    // -> P07.UBAL
  /* PIN_78 */ bool WR_D;    // <- P08.USUF

  /* PIN_79 */ bool RD_A;    // <- P08.UGAC
  /* PIN_79 */ bool RD_C;    // -> P07.UJYV
  /* PIN_79 */ bool RD_D;    // <- P08.URUN

  /* PIN_80 */ bool CS_A;    // <- P08.TYHO

  //----------
  // Address pins

  /* PIN_01 */ bool A00_C;   // -> P08.KOVA
  /* PIN_02 */ bool A01_C;   // -> P08.CAMU
  /* PIN_03 */ bool A02_C;   // -> P08.BUXU
  /* PIN_04 */ bool A03_C;   // -> P08.BASE
  /* PIN_05 */ bool A04_C;   // -> P08.AFEC
  /* PIN_06 */ bool A05_C;   // -> P08.ABUP
  /* PIN_07 */ bool A06_C;   // -> P08.CYGU
  /* PIN_08 */ bool A07_C;   // -> P08.COGO
  /* PIN_09 */ bool A08_C;   // -> P08.MUJY
  /* PIN_10 */ bool A09_C;   // -> P08.NENA
  /* PIN_11 */ bool A10_C;   // -> P08.SURA
  /* PIN_12 */ bool A11_C;   // -> P08.MADY
  /* PIN_13 */ bool A12_C;   // -> P08.LAHE
  /* PIN_14 */ bool A13_C;   // -> P08.LURA
  /* PIN_15 */ bool A14_C;   // -> P08.PEVO
  /* PIN_16 */ bool A15_C;   // -> P08.RAZA
                             
  /* PIN_01 */ bool A00_A;   // <- P08.KUPO
  /* PIN_01 */ bool A00_D;   // <- P08.KOTY
                             
  /* PIN_02 */ bool A01_A;   // <- P08.CABA
  /* PIN_02 */ bool A01_D;   // <- P08.COTU
                             
  /* PIN_03 */ bool A02_A;   // <- P08.BOKU
  /* PIN_03 */ bool A02_D;   // <- P08.BAJO
                             
  /* PIN_04 */ bool A03_A;   // <- P08.BOTY
  /* PIN_04 */ bool A03_D;   // <- P08.BOLA
                             
  /* PIN_05 */ bool A04_A;   // <- P08.BYLA
  /* PIN_05 */ bool A04_D;   // <- P08.BEVO
                             
  /* PIN_06 */ bool A05_A;   // <- P08.BADU
  /* PIN_06 */ bool A05_D;   // <- P08.AJAV
                             
  /* PIN_07 */ bool A06_A;   // <- P08.CEPU
  /* PIN_07 */ bool A06_D;   // <- P08.CYKA
                             
  /* PIN_08 */ bool A07_A;   // <- P08.DEFY
  /* PIN_08 */ bool A07_D;   // <- P08.COLO
                             
  /* PIN_09 */ bool A08_A;   // <- P08.MYNY
  /* PIN_09 */ bool A08_D;   // <- P08.MEGO
                             
  /* PIN_10 */ bool A09_A;   // <- P08.MUNE
  /* PIN_10 */ bool A09_D;   // <- P08.MENY
                             
  /* PIN_11 */ bool A10_A;   // <- P08.ROXU
  /* PIN_11 */ bool A10_D;   // <- P08.RORE
                             
  /* PIN_12 */ bool A11_A;   // <- P08.LEPY
  /* PIN_12 */ bool A11_D;   // <- P08.LYNY
                             
  /* PIN_13 */ bool A12_A;   // <- P08.LUCE
  /* PIN_13 */ bool A12_D;   // <- P08.LOSO
                             
  /* PIN_14 */ bool A13_A;   // <- P08.LABE
  /* PIN_14 */ bool A13_D;   // <- P08.LEVA
                             
  /* PIN_15 */ bool A14_A;   // <- P08.PUHE
  /* PIN_15 */ bool A14_D;   // <- P08.PAHY
                             
  /* PIN_16 */ bool A15_A;   // <- P08.SUZE
  /* PIN_16 */ bool A15_D;   // <- P08.RULO

  //----------
  // Data pins

  /* PIN_17 */ bool D0_C;    // -> P08.TOVO,SOMA
  /* PIN_18 */ bool D1_C;    // -> P08.RUZY,RONY
  /* PIN_19 */ bool D2_C;    // -> P08.ROME,RAXY
  /* PIN_20 */ bool D3_C;    // -> P08.SAZA,SELO
  /* PIN_21 */ bool D4_C;    // -> P08.TEHE,SODY
  /* PIN_22 */ bool D5_C;    // -> P08.RATU,SAGO
  /* PIN_23 */ bool D6_C;    // -> P08.SOCA,RUPA
  /* PIN_24 */ bool D7_C;    // -> P08.RYBA,SAZY


  /* PIN_17 */ bool D0_A;    // <- P08.RUXA
  /* PIN_17 */ bool D0_B;    // <- P08.LULA
  /* PIN_17 */ bool D0_D;    // <- P08.RUNE
                             
  /* PIN_18 */ bool D1_A;    // <- P08.RUJA
  /* PIN_18 */ bool D1_B;    // <- P08.LULA
  /* PIN_18 */ bool D1_D;    // <- P08.RYPU
                             
  /* PIN_19 */ bool D2_A;    // <- P08.RABY
  /* PIN_19 */ bool D2_B;    // <- P08.LULA
  /* PIN_19 */ bool D2_D;    // <- P08.SULY
                             
  /* PIN_20 */ bool D3_A;    // <- P08.RERA
  /* PIN_20 */ bool D3_B;    // <- P08.LULA
  /* PIN_20 */ bool D3_D;    // <- P08.SEZE
                             
  /* PIN_21 */ bool D4_A;    // <- P08.RORY
  /* PIN_21 */ bool D4_B;    // <- P08.LULA
  /* PIN_21 */ bool D4_D;    // <- P08.RESY
                             
  /* PIN_22 */ bool D5_A;    // <- P08.RYVO
  /* PIN_22 */ bool D5_B;    // <- P08.LULA
  /* PIN_22 */ bool D5_D;    // <- P08.TAMU
                             
  /* PIN_23 */ bool D6_A;    // <- P08.RAFY
  /* PIN_23 */ bool D6_B;    // <- P08.LULA
  /* PIN_23 */ bool D6_D;    // <- P08.ROGY
                             
  /* PIN_24 */ bool D7_A;    // <- P08.RAVU
  /* PIN_24 */ bool D7_B;    // <- P08.LULA
  /* PIN_24 */ bool D7_D;    // <- P08.RYDA

  //----------
  // VRAM control pins

  /* PIN_43 */ bool MCS_A;   // <- P25.SOKY
  /* PIN_43 */ bool MCS_D;   // <- P25.SETY
                             
  /* PIN_45 */ bool MOE_A;   // <- P25.REFO
  /* PIN_45 */ bool MOE_D;   // <- P25.SAHA
                             
  /* PIN_49 */ bool MWR_A;   // <- P25.SYSY
  /* PIN_49 */ bool MWR_D;   // <- P25.RAGU

  //----------
  // VRAM address pins

  /* PIN_34 */ bool MA00;    // <- P04.ECAL
  /* PIN_35 */ bool MA01;    // <- P04.EGEZ
  /* PIN_36 */ bool MA02;    // <- P04.FUHE
  /* PIN_37 */ bool MA03;    // <- P04.FYZY
  /* PIN_38 */ bool MA04;    // <- P04.DAMU
  /* PIN_39 */ bool MA05;    // <- P04.DAVA
  /* PIN_40 */ bool MA06;    // <- P04.ETEG
  /* PIN_41 */ bool MA07;    // <- P04.EREW
  /* PIN_48 */ bool MA08;    // <- P04.EVAX
  /* PIN_47 */ bool MA09;    // <- P04.DUVE
  /* PIN_44 */ bool MA10;    // <- P04.ERAF
  /* PIN_46 */ bool MA11;    // <- P04.FUSY
  /* PIN_42 */ bool MA12;    // <- P04.EXYF

  //----------
  // VRAM data pins

  /* PIN_25 */ bool MD0_C;
  /* PIN_26 */ bool MD1_C;
  /* PIN_27 */ bool MD2_C;
  /* PIN_28 */ bool MD3_C;
  /* PIN_29 */ bool MD4_C;
  /* PIN_30 */ bool MD5_C;
  /* PIN_31 */ bool MD6_C;
  /* PIN_33 */ bool MD7_C;



  /* PIN_25 */ bool MD7_A;   // <- P25.RYZE
  /* PIN_25 */ bool MD7_B;   // <- P25.ROFA
  /* PIN_25 */ bool MD7_D;   
                             
  /* PIN_26 */ bool MD6_A;   // <- P25.REKU
  /* PIN_26 */ bool MD6_B;   // <- P25.ROFA
  /* PIN_26 */ bool MD6_D;   
                             
  /* PIN_27 */ bool MD5_A;   // <- P25.REVU
  /* PIN_27 */ bool MD5_B;   // <- P25.ROFA
  /* PIN_27 */ bool MD5_D;   
                             
  /* PIN_28 */ bool MD4_A;   // <- P25.RYRO
  /* PIN_28 */ bool MD4_B;   // <- P25.ROFA
  /* PIN_28 */ bool MD4_D;   
                             
  /* PIN_29 */ bool MD3_A;   // <- P25.RADA
  /* PIN_29 */ bool MD3_B;   // <- P25.ROFA
  /* PIN_29 */ bool MD3_D;   
                             
  /* PIN_30 */ bool MD2_A;   // <- P25.RAZO
  /* PIN_30 */ bool MD2_B;   // <- P25.ROFA
  /* PIN_30 */ bool MD2_D;   
                             
  /* PIN_31 */ bool MD1_A;   // <- P25.RYKY
  /* PIN_31 */ bool MD1_B;   // <- P25.ROFA
  /* PIN_31 */ bool MD1_D;   
                             
  /* PIN_33 */ bool MD0_A;   // <- P25.REGE
  /* PIN_33 */ bool MD0_B;   // <- P25.ROFA
  /* PIN_33 */ bool MD0_D;


};

//-----------------------------------------------------------------------------

};
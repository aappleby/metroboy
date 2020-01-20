#pragma once
#include "Sch_Common.h"

namespace Schematics {

struct WavePins {
  PinOut CLK_AxxxxxGH;
};

//-----------------------------------------------------------------------------
// These signals connect directly to OAM

struct OamPins {
  /*p25.COTA*/ PinOut CLK_A;
  /*p28.ZODO*/ PinOut OE;
  /*p28.ZONE*/ PinOut WR_A; // definitely write
  /*p28.ZOFE*/ PinOut WR_B; // definitely write

  /*p28.GEKA*/ PinOut A0;
  /*p28.ZYFO*/ PinOut A1;
  /*p28.YFOT*/ PinOut A2;
  /*p28.YFOC*/ PinOut A3;
  /*p28.YVOM*/ PinOut A4;
  /*p28.YMEV*/ PinOut A5;
  /*p28.XEMU*/ PinOut A6;
  /*p28.YZET*/ PinOut A7;

  Tribuf A_D0;
  Tribuf A_D1;
  Tribuf A_D2;
  Tribuf A_D3;
  Tribuf A_D4;
  Tribuf A_D5;
  Tribuf A_D6;
  Tribuf A_D7;

  Tribuf B_D0;
  Tribuf B_D1;
  Tribuf B_D2;
  Tribuf B_D3;
  Tribuf B_D4;
  Tribuf B_D5;
  Tribuf B_D6;
  Tribuf B_D7;
};

//-----------------------------------------------------------------------------

struct CpuPins {

  //----------
  // top right wire by itself

  PinOut TO_CPU2;        // <- P02.AWOB

  //----------
  // top right port

  // ADDR_VALID and one of the other signals might be switched

  PinIn  CPU_RAW_RD;   // PORTA_00: -> P07.UJYV, P08.LAGU, P08.LAVO
  PinIn  CPU_RAW_WR;   // PORTA_01: -> P01.AREV, P08.LAGU. This is almost definitely "raw write"
  PinOut T1nT2;        // PORTA_02: <- P07.T1nT2
  PinOut SYRO;         // PORTA_03: <- P25.SYRO
  PinOut READ_BOOTROM; // PORTA_04: <- P07.READ_BOOTROM tutu?
  PinOut T1T2n;        // PORTA_05: <- P07.T1T2n
  PinIn  ADDR_VALID;   // PORTA_06: -> P01.AGUT, P08.TEX0. This is almost definitely "address valid"

  //----------
  // bottom right port

  Tribuf A00;          // PORTB_00: -> A00
  PinIn  FROM_CPU9;    // PORTB_01: -> P02.LETY, vblank int ack
  Tribuf A08;          // PORTB_02: -> A08
  PinOut TO_CPU3;      // PORTB_03: <- P02.LOPE, vblank int
  Tribuf A01;          // PORTB_04: -> A01
  PinIn  FROM_CPU8;    // PORTB_05: -> P02.LEJA, stat int ack
  Tribuf A09;          // PORTB_06: -> A09
  PinOut TO_CPU6;      // PORTB_07: <- P02.LALU, stat int
  Tribuf A02;          // PORTB_08: -> A02
  PinIn  FROM_CPU10;   // PORTB_09: -> P02.LESA, timer int ack
  Tribuf A10;          // PORTB_10: -> A10
  PinOut TO_CPU7;      // PORTB_11: <- P02.NYBO, timer int
  Tribuf A03;          // PORTB_12: -> A03
  PinIn  FROM_CPU7;    // PORTB_13: -> P02.LUFE, serial int ack
  Tribuf A11;          // PORTB_14: -> A11
  PinOut TO_CPU4;      // PORTB_15: <- P02.UBUL, serial int
  Tribuf A04;          // PORTB_16: -> A04
  PinIn  FROM_CPU11;   // PORTB_17: -> P02.LAMO, joypad int ack
  Tribuf A12;          // PORTB_18: -> A12
  PinOut TO_CPU5;      // PORTB_19: <- P02.ULAK, joypad int
  Tribuf A05;          // PORTB_20: -> A05
  Tribuf A13;          // PORTB_22: -> A13
  Tribuf A06;          // PORTB_24: -> A06
  Tribuf A14;          // PORTB_26: -> A14
  Tribuf A07;          // PORTB_28: -> A07
  Tribuf A15;          // PORTB_30: -> A15

  int get_addr() const {
    return pack(A00, A01, A02, A03, A04, A05, A06, A07,
                A08, A09, A10, A11, A12, A13, A14, A15);
  }

  void set_addr_sync(uint16_t addr) {
    A00.set_sync(1, addr & 0x0001);
    A01.set_sync(1, addr & 0x0002);
    A02.set_sync(1, addr & 0x0004);
    A03.set_sync(1, addr & 0x0008);
    A04.set_sync(1, addr & 0x0010);
    A05.set_sync(1, addr & 0x0020);
    A06.set_sync(1, addr & 0x0040);
    A07.set_sync(1, addr & 0x0080);
    A08.set_sync(1, addr & 0x0100);
    A09.set_sync(1, addr & 0x0200);
    A10.set_sync(1, addr & 0x0400);
    A11.set_sync(1, addr & 0x0800);
    A12.set_sync(1, addr & 0x1000);
    A13.set_sync(1, addr & 0x2000);
    A14.set_sync(1, addr & 0x4000);
    A15.set_sync(1, addr & 0x8000);
  }

  //----------
  // top center port

  PinIn  CLKREQ;       // PORTC_00: -> ABOL (an inverter) -> BATE. Something about "cpu ready". clock request?
  PinOut AFER;         // PORTC_01: <- P01.AFER , reset related reg
  PinOut PIN_RESET;    // PORTC_02: <- PIN_RESET directly connected to the pad
  PinOut CLKIN_A;      // PORTC_03: <- chip.CLKIN_A top wire on PAD_XI,
  PinOut CPU_RESET;    // PORTC_04: <- P01.CPU_RESET

  //----------
  // top left port

  PinIn  FROM_CPU6;    // PORTD_00: -> P07.LEXY, doesn't do anything
  PinOut CLK_xBCDEFGH; // PORTD_01: <- P01.BOWA
  PinOut CLK_Axxxxxxx; // PORTD_02: <- P01.BEDO
  PinOut BEKO;         // PORTD_03: <- P01.BEKO connection not indicated on P01
  PinOut BUDE;         // PORTD_04: <- P01.BUDE

  // controls driving the external data pins onto the internal data bus and
  // other stuff. is this actually a clock, or like OE?
  // if this is low, writes to the APU don't happen and writes to TIMA don't
  // actually load TIMA (I think).
  PinIn FROM_CPU5;     // PORTD_05: -> FROM_CPU5
  
  PinOut BUKE;         // PORTD_06: <- P01.BUKE _____f__
  PinOut BOMA;    // PORTD_07: <- P01.RESET_CLK _____fgh
  PinOut BOGA;         // PORTD_08: <- P01.BOGA abcde___

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
// Clock/reset/debug

struct SysPins {
  /* PIN_71 */ PinIn RST;
  /* PIN_74 */ PinIn CLKIN_A; // CLK_GOOD
  /* PIN_74 */ PinIn CLKIN_B; // CLK
  /* PIN_76 */ PinIn T2;
  /* PIN_77 */ PinIn T1;
};

//-----------------------------------------------------------------------------

struct VramPins {

  //----------
  // VRAM control pins

  /* PIN_43 */ PinOut MCS_A;   // <- P25.SOKY
  /* PIN_43 */ PinIn  MCS_C;   // -> P25.TEFY
  /* PIN_43 */ PinOut MCS_D;   // <- P25.SETY

  /* PIN_45 */ PinOut MOE_A;   // <- P25.REFO
  /* PIN_45 */ PinIn  MOE_C;   // -> P25.TAVY
  /* PIN_45 */ PinOut MOE_D;   // <- P25.SAHA
                             
  /* PIN_49 */ PinOut MWR_A;   // <- P25.SYSY
  /* PIN_49 */ PinIn  MWR_C;   // -> P25.SUDO
  /* PIN_49 */ PinOut MWR_D;   // <- P25.RAGU

  //----------
  // VRAM address pins

  /* PIN_34 */ PinOut MA00_D;  // <- P04.ECAL
  /* PIN_35 */ PinOut MA01_D;  // <- P04.EGEZ
  /* PIN_36 */ PinOut MA02_D;  // <- P04.FUHE
  /* PIN_37 */ PinOut MA03_D;  // <- P04.FYZY
  /* PIN_38 */ PinOut MA04_D;  // <- P04.DAMU
  /* PIN_39 */ PinOut MA05_D;  // <- P04.DAVA
  /* PIN_40 */ PinOut MA06_D;  // <- P04.ETEG
  /* PIN_41 */ PinOut MA07_D;  // <- P04.EREW
  /* PIN_42 */ PinOut MA12_D;  // <- P04.EXYF
  /* PIN_44 */ PinOut MA10_D;  // <- P04.ERAF
  /* PIN_46 */ PinOut MA11_D;  // <- P04.FUSY
  /* PIN_47 */ PinOut MA09_D;  // <- P04.DUVE
  /* PIN_48 */ PinOut MA08_D;  // <- P04.EVAX

  //----------
  // VRAM data pins

  /* PIN_33 */ PinOut MD0_A;   // <- P25.REGE
  /* PIN_31 */ PinOut MD1_A;   // <- P25.RYKY
  /* PIN_30 */ PinOut MD2_A;   // <- P25.RAZO
  /* PIN_29 */ PinOut MD3_A;   // <- P25.RADA
  /* PIN_28 */ PinOut MD4_A;   // <- P25.RYRO
  /* PIN_27 */ PinOut MD5_A;   // <- P25.REVU
  /* PIN_26 */ PinOut MD6_A;   // <- P25.REKU
  /* PIN_25 */ PinOut MD7_A;   // <- P25.RYZE

  /* PIN_33 */ PinOut MD0_B;   // <- P25.ROFA
  /* PIN_31 */ PinOut MD1_B;   // <- P25.ROFA
  /* PIN_30 */ PinOut MD2_B;   // <- P25.ROFA
  /* PIN_29 */ PinOut MD3_B;   // <- P25.ROFA
  /* PIN_28 */ PinOut MD4_B;   // <- P25.ROFA
  /* PIN_27 */ PinOut MD5_B;   // <- P25.ROFA
  /* PIN_26 */ PinOut MD6_B;   // <- P25.ROFA
  /* PIN_25 */ PinOut MD7_B;   // <- P25.ROFA

  /* PIN_33 */ PinIn  MD0_C;   // -> P25.RODY
  /* PIN_31 */ PinIn  MD1_C;   // -> P25.REBA
  /* PIN_30 */ PinIn  MD2_C;   // -> P25.RYDO
  /* PIN_29 */ PinIn  MD3_C;   // -> P25.REMO
  /* PIN_28 */ PinIn  MD4_C;   // -> P25.ROCE
  /* PIN_27 */ PinIn  MD5_C;   // -> P25.ROPU
  /* PIN_26 */ PinIn  MD6_C;   // -> P25.RETA
  /* PIN_25 */ PinIn  MD7_C;   // -> P25.RAKU

  /* PIN_33 */ PinOut MD0_D;   // <- P25.RURA
  /* PIN_31 */ PinOut MD1_D;   // <- P25.RULY
  /* PIN_30 */ PinOut MD2_D;   // <- P25.RARE
  /* PIN_29 */ PinOut MD3_D;   // <- P25.RODU
  /* PIN_28 */ PinOut MD4_D;   // <- P25.RUBE
  /* PIN_27 */ PinOut MD5_D;   // <- P25.RUMU
  /* PIN_26 */ PinOut MD6_D;   // <- P25.RYTY
  /* PIN_25 */ PinOut MD7_D;   // <- P25.RADY
};

//-----------------------------------------------------------------------------

struct SerialPins {
  //----------
  // Serial pins

  /* PIN_68 */ PinOut SCK_A;   // <- P06.KEXU
  /* PIN_68 */ PinOut SCK_B;   // <- P06.CULY
  /* PIN_68 */ PinIn  SCK_C;   // -> P06.CAVE
  /* PIN_68 */ PinOut SCK_D;   // <- P06.KUJO

  /* PIN_69 */ PinOut SIN_A;   // nc?
  /* PIN_69 */ PinOut SIN_B;   // nc?
  /* PIN_69 */ PinIn  SIN_C;   // -> P06.CAGE
  /* PIN_69 */ PinOut SIN_D;   // nc?

  /* PIN_70 */ PinOut SOUT;    // <- P05.KENA
};

//-----------------------------------------------------------------------------

struct LcdPins {
  /* PIN_50 */ PinOut LD1;
  /* PIN_51 */ PinOut LD0;
  /* PIN_52 */ PinOut CPG;
  /* PIN_53 */ PinOut CP;
  /* PIN_54 */ PinOut ST;
  /* PIN_55 */ PinOut CPL;
  /* PIN_56 */ PinOut FR;
  /* PIN_57 */ PinOut S;
};

//-----------------------------------------------------------------------------

struct JoypadPins {

  // The B connections on the joypad pins are werid.
  // They seem to be used as an input, or at least I can't find the driver
  // PESU
  // RARU ROWE RYKE RYNE RASE REJY REKA ROMY
  // RUNY VYPO TOMY? SEZU? RAWU? PUTE? MYDE RUGO? NYLU WYMO?
  // WEFE WUWE GEFY WYGA? FABY ECAB? DYSO ERUC GEZE GUVA 
  // ARAR ATAJ ASUZ AJEC AKAJ ANOC BENU BEDA
  // BEKU

  /* PIN_67 */ PinOut P10_A;   // <- P05.KOLE
  /* PIN_67 */ PinOut P10_B;   
  /* PIN_67 */ PinIn  P10_C;   // -> P02.KERY, P05.KEVU
  /* PIN_67 */ PinOut P10_D;   // <- P05.KYBU

  /* PIN_66 */ PinOut P11_A;   // <- P05.KYTO
  /* PIN_66 */ PinOut P11_B;   
  /* PIN_66 */ PinIn  P11_C;   // -> P02.KERY, P05.KAPA
  /* PIN_66 */ PinOut P11_D;   // <- P05.KABU

  /* PIN_65 */ PinOut P12_A;   // <- P05.KYHU
  /* PIN_65 */ PinOut P12_B;   
  /* PIN_65 */ PinIn  P12_C;   // -> P02.KERY, P05.KEJA
  /* PIN_65 */ PinOut P12_D;   // <- P05.KASY
                        
  /* PIN_64 */ PinOut P13_A;   // <- P05.KORY
  /* PIN_64 */ PinOut P13_B;   
  /* PIN_64 */ PinIn  P13_C;   // -> P02.KERY, P05.KOLO
  /* PIN_64 */ PinOut P13_D;   // <- P05.KALE
                         
  /* PIN_63 */ PinOut P14_A;   // <- p05.KARU
  /* PIN_63 */ PinOut P14_D;   // <- p05.KELY

  /* PIN_62 */ PinOut P15_A;   // <- p05.CELA
  /* PIN_62 */ PinOut P15_D;   // <- p05.COFY
};

//-----------------------------------------------------------------------------

struct ExtPins {

  //----------
  // Cart pins

  /* PIN_75 */ PinOut PHI;     // <- P01.BUDE/BEVA

  /* PIN_78 */ PinOut WRn_A;   // <- P08.UVER
  /* PIN_78 */ PinIn  WRn_C;   // -> P07.UBAL
  /* PIN_78 */ PinOut WRn_D;   // <- P08.USUF

  /* PIN_79 */ PinOut RDn_A;   // <- P08.UGAC
  /* PIN_79 */ PinIn  RDn_C;   // -> P07.UJYV
  /* PIN_79 */ PinOut RDn_D;   // <- P08.URUN

  /* PIN_80 */ PinOut CSn_A;   // <- P08.TYHO

  //----------
  // Address pins

  /* PIN_01 */ PinOut A00_A;   // <- P08.KUPO
  /* PIN_01 */ PinIn  A00_C;   // -> P08.KOVA
  /* PIN_01 */ PinOut A00_D;   // <- P08.KOTY
                             
  /* PIN_02 */ PinOut A01_A;   // <- P08.CABA
  /* PIN_02 */ PinIn  A01_C;   // -> P08.CAMU
  /* PIN_02 */ PinOut A01_D;   // <- P08.COTU
                             
  /* PIN_03 */ PinOut A02_A;   // <- P08.BOKU
  /* PIN_03 */ PinIn  A02_C;   // -> P08.BUXU
  /* PIN_03 */ PinOut A02_D;   // <- P08.BAJO
                             
  /* PIN_04 */ PinOut A03_A;   // <- P08.BOTY
  /* PIN_04 */ PinIn  A03_C;   // -> P08.BASE
  /* PIN_04 */ PinOut A03_D;   // <- P08.BOLA
                             
  /* PIN_05 */ PinOut A04_A;   // <- P08.BYLA
  /* PIN_05 */ PinIn  A04_C;   // -> P08.AFEC
  /* PIN_05 */ PinOut A04_D;   // <- P08.BEVO
                             
  /* PIN_06 */ PinOut A05_A;   // <- P08.BADU
  /* PIN_06 */ PinIn  A05_C;   // -> P08.ABUP
  /* PIN_06 */ PinOut A05_D;   // <- P08.AJAV
                             
  /* PIN_07 */ PinOut A06_A;   // <- P08.CEPU
  /* PIN_07 */ PinIn  A06_C;   // -> P08.CYGU
  /* PIN_07 */ PinOut A06_D;   // <- P08.CYKA
                             
  /* PIN_08 */ PinOut A07_A;   // <- P08.DEFY
  /* PIN_08 */ PinIn  A07_C;   // -> P08.COGO
  /* PIN_08 */ PinOut A07_D;   // <- P08.COLO
                             
  /* PIN_09 */ PinOut A08_A;   // <- P08.MYNY
  /* PIN_09 */ PinIn  A08_C;   // -> P08.MUJY
  /* PIN_09 */ PinOut A08_D;   // <- P08.MEGO
                             
  /* PIN_10 */ PinOut A09_A;   // <- P08.MUNE
  /* PIN_10 */ PinIn  A09_C;   // -> P08.NENA
  /* PIN_10 */ PinOut A09_D;   // <- P08.MENY
                             
  /* PIN_11 */ PinOut A10_A;   // <- P08.ROXU
  /* PIN_11 */ PinIn  A10_C;   // -> P08.SURA
  /* PIN_11 */ PinOut A10_D;   // <- P08.RORE
                             
  /* PIN_12 */ PinOut A11_A;   // <- P08.LEPY
  /* PIN_12 */ PinIn  A11_C;   // -> P08.MADY
  /* PIN_12 */ PinOut A11_D;   // <- P08.LYNY
                             
  /* PIN_13 */ PinOut A12_A;   // <- P08.LUCE
  /* PIN_13 */ PinIn  A12_C;   // -> P08.LAHE
  /* PIN_13 */ PinOut A12_D;   // <- P08.LOSO
                             
  /* PIN_14 */ PinOut A13_A;   // <- P08.LABE
  /* PIN_14 */ PinIn  A13_C;   // -> P08.LURA
  /* PIN_14 */ PinOut A13_D;   // <- P08.LEVA
                             
  /* PIN_15 */ PinOut A14_A;   // <- P08.PUHE
  /* PIN_15 */ PinIn  A14_C;   // -> P08.PEVO
  /* PIN_15 */ PinOut A14_D;   // <- P08.PAHY
                             
  /* PIN_16 */ PinOut A15_A;   // <- P08.SUZE
  /* PIN_16 */ PinIn  A15_C;   // -> P08.RAZA
  /* PIN_16 */ PinOut A15_D;   // <- P08.RULO

  //----------
  // Data pins

  /* PIN_17 */ PinOut D0_A;    // <- P08.RUXA
  /* PIN_17 */ PinOut D0_B;    // <- P08.LULA
  /* PIN_17 */ PinIn  D0_C;    // -> P08.TOVO,SOMA
  /* PIN_17 */ PinOut D0_D;    // <- P08.RUNE
                             
  /* PIN_18 */ PinOut D1_A;    // <- P08.RUJA
  /* PIN_18 */ PinOut D1_B;    // <- P08.LULA
  /* PIN_18 */ PinIn  D1_C;    // -> P08.RUZY,RONY
  /* PIN_18 */ PinOut D1_D;    // <- P08.RYPU
                             
  /* PIN_19 */ PinOut D2_A;    // <- P08.RABY
  /* PIN_19 */ PinOut D2_B;    // <- P08.LULA
  /* PIN_19 */ PinIn  D2_C;    // -> P08.ROME,RAXY
  /* PIN_19 */ PinOut D2_D;    // <- P08.SULY
                             
  /* PIN_20 */ PinOut D3_A;    // <- P08.RERA
  /* PIN_20 */ PinOut D3_B;    // <- P08.LULA
  /* PIN_20 */ PinIn  D3_C;    // -> P08.SAZA,SELO
  /* PIN_20 */ PinOut D3_D;    // <- P08.SEZE
                             
  /* PIN_21 */ PinOut D4_A;    // <- P08.RORY
  /* PIN_21 */ PinOut D4_B;    // <- P08.LULA
  /* PIN_21 */ PinIn  D4_C;    // -> P08.TEHE,SODY
  /* PIN_21 */ PinOut D4_D;    // <- P08.RESY
                             
  /* PIN_22 */ PinOut D5_A;    // <- P08.RYVO
  /* PIN_22 */ PinOut D5_B;    // <- P08.LULA
  /* PIN_22 */ PinIn  D5_C;    // -> P08.RATU,SAGO
  /* PIN_22 */ PinOut D5_D;    // <- P08.TAMU
                             
  /* PIN_23 */ PinOut D6_A;    // <- P08.RAFY
  /* PIN_23 */ PinOut D6_B;    // <- P08.LULA
  /* PIN_23 */ PinIn  D6_C;    // -> P08.SOCA,RUPA
  /* PIN_23 */ PinOut D6_D;    // <- P08.ROGY
                             
  /* PIN_24 */ PinOut D7_A;    // <- P08.RAVU
  /* PIN_24 */ PinOut D7_B;    // <- P08.LULA
  /* PIN_24 */ PinIn  D7_C;    // -> P08.RYBA,SAZY
  /* PIN_24 */ PinOut D7_D;    // <- P08.RYDA
};

//-----------------------------------------------------------------------------

};
#pragma once
#include "Sch_Common.h"

namespace Schematics {

struct WavePins {
  bool CLK_AxxxxxGH;
};

//-----------------------------------------------------------------------------

struct ExtPins {
  bool CLK;
  bool CLK_GOOD;
  bool RST;
  bool T1;
  bool T2;
};

//-----------------------------------------------------------------------------
// These signals connect directly to OAM

struct OamPins {
  /*p25.COTA*/ bool CLK_A;
  /*p28.ZODO*/ bool OE;
  /*p28.ZONE*/ bool WR_A; // definitely write
  /*p28.ZOFE*/ bool WR_B; // definitely write

  /*p28.GEKA*/ bool A0;
  /*p28.ZYFO*/ bool A1;
  /*p28.YFOT*/ bool A2;
  /*p28.YFOC*/ bool A3;
  /*p28.YVOM*/ bool A4;
  /*p28.YMEV*/ bool A5;
  /*p28.XEMU*/ bool A6;
  /*p28.YZET*/ bool A7;

  bool A_D0;
  bool A_D1;
  bool A_D2;
  bool A_D3;
  bool A_D4;
  bool A_D5;
  bool A_D6;
  bool A_D7;

  bool B_D0;
  bool B_D1;
  bool B_D2;
  bool B_D3;
  bool B_D4;
  bool B_D5;
  bool B_D6;
  bool B_D7;
};

//-----------------------------------------------------------------------------

struct CpuPins {

  //----------
  // top right wire by itself

  bool TO_CPU2;        // <- P02.AWOB

  //----------
  // top right port

  // ADDR_VALID and one of the other signals might be switched

  bool CPU_RAW_RD;     // PORTA_00: -> P07.UJYV, P08.LAGU, P08.LAVO
  bool CPU_RAW_WR;     // PORTA_01: -> P01.AREV, P08.LAGU. This is almost definitely "raw write"
  bool T1nT2;          // PORTA_02: <- P07.T1nT2
  bool SYRO;           // PORTA_03: <- P25.SYRO
  bool READ_BOOTROM;   // PORTA_04: <- P07.READ_BOOTROM
  bool T1T2n;          // PORTA_05: <- P07.T1T2n
  bool ADDR_VALID;     // PORTA_06: -> P01.AGUT, P08.TEX0. This is almost definitely "address valid"

  //----------
  // bottom right port

  bool CPU_A00;        // PORTB_00: -> A00
  bool FROM_CPU9;      // PORTB_01: -> P02.LETY, vblank int ack
  bool CPU_A08;        // PORTB_02: -> A08
  bool TO_CPU3;        // PORTB_03: <- P02.LOPE, vblank int
  bool CPU_A01;        // PORTB_04: -> A01
  bool FROM_CPU8;      // PORTB_05: -> P02.LEJA, stat int ack
  bool CPU_A09;        // PORTB_06: -> A09
  bool TO_CPU6;        // PORTB_07: <- P02.LALU, stat int
  bool CPU_A02;        // PORTB_08: -> A02
  bool FROM_CPU10;     // PORTB_09: -> P02.LESA, timer int ack
  bool CPU_A10;        // PORTB_10: -> A10
  bool TO_CPU7;        // PORTB_11: <- P02.NYBO, timer int
  bool CPU_A03;        // PORTB_12: -> A03
  bool FROM_CPU7;      // PORTB_13: -> P02.LUFE, serial int ack
  bool CPU_A11;        // PORTB_14: -> A11
  bool TO_CPU4;        // PORTB_15: <- P02.UBUL, serial int
  bool CPU_A04;        // PORTB_16: -> A04
  bool FROM_CPU11;     // PORTB_17: -> P02.LAMO, joypad int ack
  bool CPU_A12;        // PORTB_18: -> A12
  bool TO_CPU5;        // PORTB_19: <- P02.ULAK, joypad int
  bool CPU_A05;        // PORTB_20: -> A05
  bool CPU_A13;        // PORTB_22: -> A13
  bool CPU_A06;        // PORTB_24: -> A06
  bool CPU_A14;        // PORTB_26: -> A14
  bool CPU_A07;        // PORTB_28: -> A07
  bool CPU_A15;        // PORTB_30: -> A15

  //----------
  // top center port

  bool CLKREQ;         // PORTC_00: -> ABOL (an inverter) -> BATE. Something about "cpu ready". clock request?
  bool AFER;           // PORTC_01: <- P01.AFER , reset related reg
  bool PIN_RESET;      // PORTC_02: <- PIN_RESET directly connected to the pad
  bool CLKIN_A;        // PORTC_03: <- chip.CLKIN_A top wire on PAD_XI,
  bool CPU_RESET;      // PORTC_04: <- P01.CPU_RESET

  //----------
  // top left port

  bool FROM_CPU6;      // PORTD_00: -> P07.LEXY, doesn't do anything
  bool CLK_xBCDEFGH;   // PORTD_01: <- P01.BOWA
  bool BEDO;           // PORTD_02: <- P01.BEDO _____fgh
  bool BEKO;           // PORTD_03: <- P01.BEKO ____efgh connection not indicated on P01
  bool PHI_OUT;        // PORTD_04: <- P01.BUDE abcd____

  // controls driving the external data pins onto the internal data bus and
  // other stuff. is this actually a clock, or like OE?
  // if this is low, writes to the APU don't happen and writes to TIMA don't
  // actually load TIMA (I think).
  bool FROM_CPU5;      // PORTD_05: -> FROM_CPU5
  
  bool BUKE;           // PORTD_06: <- P01.BUKE _____f__
  bool RESET_CLK;      // PORTD_07: <- P01.RESET_CLK _____fgh
  bool BOGA;           // PORTD_08: <- P01.BOGA abcde___

  //----------
  // bottom left port, tristate data bus

  bool CPU_D0;
  bool CPU_D1;
  bool CPU_D2;
  bool CPU_D3;
  bool CPU_D4;
  bool CPU_D5;
  bool CPU_D6;
  bool CPU_D7;
};

//-----------------------------------------------------------------------------

struct VramPins {


  /*p25.SOTO*/ Reg2 SOTO_DBG;

  //----------
  // VRAM control pins

  /* PIN_43 */ bool MCS_A;   // <- P25.SOKY
  /* PIN_43 */ bool MCS_C;   // -> P25.TEFY
  /* PIN_43 */ bool MCS_D;   // <- P25.SETY

  /* PIN_45 */ bool MOE_A;   // <- P25.REFO
  /* PIN_45 */ bool MOE_C;   // -> P25.TAVY
  /* PIN_45 */ bool MOE_D;   // <- P25.SAHA
                             
  /* PIN_49 */ bool MWR_A;   // <- P25.SYSY
  /* PIN_49 */ bool MWR_C;   // -> P25.SUDO
  /* PIN_49 */ bool MWR_D;   // <- P25.RAGU

  //----------
  // VRAM address pins

  /* PIN_34 */ bool MA00_D;  // <- P04.ECAL
  /* PIN_35 */ bool MA01_D;  // <- P04.EGEZ
  /* PIN_36 */ bool MA02_D;  // <- P04.FUHE
  /* PIN_37 */ bool MA03_D;  // <- P04.FYZY
  /* PIN_38 */ bool MA04_D;  // <- P04.DAMU
  /* PIN_39 */ bool MA05_D;  // <- P04.DAVA
  /* PIN_40 */ bool MA06_D;  // <- P04.ETEG
  /* PIN_41 */ bool MA07_D;  // <- P04.EREW
  /* PIN_42 */ bool MA12_D;  // <- P04.EXYF
  /* PIN_44 */ bool MA10_D;  // <- P04.ERAF
  /* PIN_46 */ bool MA11_D;  // <- P04.FUSY
  /* PIN_47 */ bool MA09_D;  // <- P04.DUVE
  /* PIN_48 */ bool MA08_D;  // <- P04.EVAX

  //----------
  // VRAM data pins

  /* PIN_33 */ bool MD0_A;   // <- P25.REGE
  /* PIN_31 */ bool MD1_A;   // <- P25.RYKY
  /* PIN_30 */ bool MD2_A;   // <- P25.RAZO
  /* PIN_29 */ bool MD3_A;   // <- P25.RADA
  /* PIN_28 */ bool MD4_A;   // <- P25.RYRO
  /* PIN_27 */ bool MD5_A;   // <- P25.REVU
  /* PIN_26 */ bool MD6_A;   // <- P25.REKU
  /* PIN_25 */ bool MD7_A;   // <- P25.RYZE

  /* PIN_33 */ bool MD0_B;   // <- P25.ROFA
  /* PIN_31 */ bool MD1_B;   // <- P25.ROFA
  /* PIN_30 */ bool MD2_B;   // <- P25.ROFA
  /* PIN_29 */ bool MD3_B;   // <- P25.ROFA
  /* PIN_28 */ bool MD4_B;   // <- P25.ROFA
  /* PIN_27 */ bool MD5_B;   // <- P25.ROFA
  /* PIN_26 */ bool MD6_B;   // <- P25.ROFA
  /* PIN_25 */ bool MD7_B;   // <- P25.ROFA

  /* PIN_33 */ bool MD0_C;   // -> P25.RODY
  /* PIN_31 */ bool MD1_C;   // -> P25.REBA
  /* PIN_30 */ bool MD2_C;   // -> P25.RYDO
  /* PIN_29 */ bool MD3_C;   // -> P25.REMO
  /* PIN_28 */ bool MD4_C;   // -> P25.ROCE
  /* PIN_27 */ bool MD5_C;   // -> P25.ROPU
  /* PIN_26 */ bool MD6_C;   // -> P25.RETA
  /* PIN_25 */ bool MD7_C;   // -> P25.RAKU

  /* PIN_33 */ bool MD0_D;   // <- P25.RURA
  /* PIN_31 */ bool MD1_D;   // <- P25.RULY
  /* PIN_30 */ bool MD2_D;   // <- P25.RARE
  /* PIN_29 */ bool MD3_D;   // <- P25.RODU
  /* PIN_28 */ bool MD4_D;   // <- P25.RUBE
  /* PIN_27 */ bool MD5_D;   // <- P25.RUMU
  /* PIN_26 */ bool MD6_D;   // <- P25.RYTY
  /* PIN_25 */ bool MD7_D;   // <- P25.RADY
};

//-----------------------------------------------------------------------------

struct SerialPins {
  //----------
  // Serial pins

  /* PIN_68 */ bool SCK_A;   // <- P06.KEXU
  /* PIN_68 */ bool SCK_B;   // <- P06.CULY
  /* PIN_68 */ bool SCK_C;   // -> P06.CAVE
  /* PIN_68 */ bool SCK_D;   // <- P06.KUJO

  /* PIN_69 */ bool SIN_A;   // nc?
  /* PIN_69 */ bool SIN_B;   // nc?
  /* PIN_69 */ bool SIN_C;   // -> P06.CAGE
  /* PIN_69 */ bool SIN_D;   // nc?

  /* PIN_70 */ bool SOUT;    // <- P05.KENA
};

//-----------------------------------------------------------------------------

struct LcdPins {
  /* PIN_50 */ bool LD1;
  /* PIN_51 */ bool LD0;
  /* PIN_52 */ bool CPG;
  /* PIN_53 */ bool CP;
  /* PIN_54 */ bool ST;
  /* PIN_55 */ bool CPL;
  /* PIN_56 */ bool FR;
  /* PIN_57 */ bool S;
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

  /* PIN_67 */ bool P10_A;   // <- P05.KOLE
  /* PIN_67 */ bool P10_B;   
  /* PIN_67 */ bool P10_C;   // -> P02.KERY, P05.KEVU
  /* PIN_67 */ bool P10_D;   // <- P05.KYBU

  /* PIN_66 */ bool P11_A;   // <- P05.KYTO
  /* PIN_66 */ bool P11_B;   
  /* PIN_66 */ bool P11_C;   // -> P02.KERY, P05.KAPA
  /* PIN_66 */ bool P11_D;   // <- P05.KABU

  /* PIN_65 */ bool P12_A;   // <- P05.KYHU
  /* PIN_65 */ bool P12_B;   
  /* PIN_65 */ bool P12_C;   // -> P02.KERY, P05.KEJA
  /* PIN_65 */ bool P12_D;   // <- P05.KASY
                        
  /* PIN_64 */ bool P13_A;   // <- P05.KORY
  /* PIN_64 */ bool P13_B;   
  /* PIN_64 */ bool P13_C;   // -> P02.KERY, P05.KOLO
  /* PIN_64 */ bool P13_D;   // <- P05.KALE
                         
  /* PIN_63 */ bool P14_A;   // <- p05.KARU
  /* PIN_63 */ bool P14_D;   // <- p05.KELY

  /* PIN_62 */ bool P15_A;   // <- p05.CELA
  /* PIN_62 */ bool P15_D;   // <- p05.COFY
};

//-----------------------------------------------------------------------------

struct CartPins {

  //----------
  // Cart pins

  /* PIN_75 */ bool PHI;     // <- P01.BUDE/BEVA

  /* PIN_78 */ bool WRn_A;   // <- P08.UVER
  /* PIN_78 */ bool WRn_C;   // -> P07.UBAL
  /* PIN_78 */ bool WRn_D;   // <- P08.USUF

  /* PIN_79 */ bool RDn_A;   // <- P08.UGAC
  /* PIN_79 */ bool RDn_C;   // -> P07.UJYV
  /* PIN_79 */ bool RDn_D;   // <- P08.URUN

  /* PIN_80 */ bool CSn_A;   // <- P08.TYHO

  //----------
  // Address pins

  /* PIN_01 */ bool A00_A;   // <- P08.KUPO
  /* PIN_01 */ bool A00_C;   // -> P08.KOVA
  /* PIN_01 */ bool A00_D;   // <- P08.KOTY
                             
  /* PIN_02 */ bool A01_A;   // <- P08.CABA
  /* PIN_02 */ bool A01_C;   // -> P08.CAMU
  /* PIN_02 */ bool A01_D;   // <- P08.COTU
                             
  /* PIN_03 */ bool A02_A;   // <- P08.BOKU
  /* PIN_03 */ bool A02_C;   // -> P08.BUXU
  /* PIN_03 */ bool A02_D;   // <- P08.BAJO
                             
  /* PIN_04 */ bool A03_A;   // <- P08.BOTY
  /* PIN_04 */ bool A03_C;   // -> P08.BASE
  /* PIN_04 */ bool A03_D;   // <- P08.BOLA
                             
  /* PIN_05 */ bool A04_A;   // <- P08.BYLA
  /* PIN_05 */ bool A04_C;   // -> P08.AFEC
  /* PIN_05 */ bool A04_D;   // <- P08.BEVO
                             
  /* PIN_06 */ bool A05_A;   // <- P08.BADU
  /* PIN_06 */ bool A05_C;   // -> P08.ABUP
  /* PIN_06 */ bool A05_D;   // <- P08.AJAV
                             
  /* PIN_07 */ bool A06_A;   // <- P08.CEPU
  /* PIN_07 */ bool A06_C;   // -> P08.CYGU
  /* PIN_07 */ bool A06_D;   // <- P08.CYKA
                             
  /* PIN_08 */ bool A07_A;   // <- P08.DEFY
  /* PIN_08 */ bool A07_C;   // -> P08.COGO
  /* PIN_08 */ bool A07_D;   // <- P08.COLO
                             
  /* PIN_09 */ bool A08_A;   // <- P08.MYNY
  /* PIN_09 */ bool A08_C;   // -> P08.MUJY
  /* PIN_09 */ bool A08_D;   // <- P08.MEGO
                             
  /* PIN_10 */ bool A09_A;   // <- P08.MUNE
  /* PIN_10 */ bool A09_C;   // -> P08.NENA
  /* PIN_10 */ bool A09_D;   // <- P08.MENY
                             
  /* PIN_11 */ bool A10_A;   // <- P08.ROXU
  /* PIN_11 */ bool A10_C;   // -> P08.SURA
  /* PIN_11 */ bool A10_D;   // <- P08.RORE
                             
  /* PIN_12 */ bool A11_A;   // <- P08.LEPY
  /* PIN_12 */ bool A11_C;   // -> P08.MADY
  /* PIN_12 */ bool A11_D;   // <- P08.LYNY
                             
  /* PIN_13 */ bool A12_A;   // <- P08.LUCE
  /* PIN_13 */ bool A12_C;   // -> P08.LAHE
  /* PIN_13 */ bool A12_D;   // <- P08.LOSO
                             
  /* PIN_14 */ bool A13_A;   // <- P08.LABE
  /* PIN_14 */ bool A13_C;   // -> P08.LURA
  /* PIN_14 */ bool A13_D;   // <- P08.LEVA
                             
  /* PIN_15 */ bool A14_A;   // <- P08.PUHE
  /* PIN_15 */ bool A14_C;   // -> P08.PEVO
  /* PIN_15 */ bool A14_D;   // <- P08.PAHY
                             
  /* PIN_16 */ bool A15_A;   // <- P08.SUZE
  /* PIN_16 */ bool A15_C;   // -> P08.RAZA
  /* PIN_16 */ bool A15_D;   // <- P08.RULO

  //----------
  // Data pins

  /* PIN_17 */ bool D0_A;    // <- P08.RUXA
  /* PIN_17 */ bool D0_B;    // <- P08.LULA
  /* PIN_17 */ bool D0_C;    // -> P08.TOVO,SOMA
  /* PIN_17 */ bool D0_D;    // <- P08.RUNE
                             
  /* PIN_18 */ bool D1_A;    // <- P08.RUJA
  /* PIN_18 */ bool D1_B;    // <- P08.LULA
  /* PIN_18 */ bool D1_C;    // -> P08.RUZY,RONY
  /* PIN_18 */ bool D1_D;    // <- P08.RYPU
                             
  /* PIN_19 */ bool D2_A;    // <- P08.RABY
  /* PIN_19 */ bool D2_B;    // <- P08.LULA
  /* PIN_19 */ bool D2_C;    // -> P08.ROME,RAXY
  /* PIN_19 */ bool D2_D;    // <- P08.SULY
                             
  /* PIN_20 */ bool D3_A;    // <- P08.RERA
  /* PIN_20 */ bool D3_B;    // <- P08.LULA
  /* PIN_20 */ bool D3_C;    // -> P08.SAZA,SELO
  /* PIN_20 */ bool D3_D;    // <- P08.SEZE
                             
  /* PIN_21 */ bool D4_A;    // <- P08.RORY
  /* PIN_21 */ bool D4_B;    // <- P08.LULA
  /* PIN_21 */ bool D4_C;    // -> P08.TEHE,SODY
  /* PIN_21 */ bool D4_D;    // <- P08.RESY
                             
  /* PIN_22 */ bool D5_A;    // <- P08.RYVO
  /* PIN_22 */ bool D5_B;    // <- P08.LULA
  /* PIN_22 */ bool D5_C;    // -> P08.RATU,SAGO
  /* PIN_22 */ bool D5_D;    // <- P08.TAMU
                             
  /* PIN_23 */ bool D6_A;    // <- P08.RAFY
  /* PIN_23 */ bool D6_B;    // <- P08.LULA
  /* PIN_23 */ bool D6_C;    // -> P08.SOCA,RUPA
  /* PIN_23 */ bool D6_D;    // <- P08.ROGY
                             
  /* PIN_24 */ bool D7_A;    // <- P08.RAVU
  /* PIN_24 */ bool D7_B;    // <- P08.LULA
  /* PIN_24 */ bool D7_C;    // -> P08.RYBA,SAZY
  /* PIN_24 */ bool D7_D;    // <- P08.RYDA

  /*p08.ALOR*/ Reg2 ADDR_LATCH_00;
  /*p08.APUR*/ Reg2 ADDR_LATCH_01;
  /*p08.ALYR*/ Reg2 ADDR_LATCH_02;
  /*p08.ARET*/ Reg2 ADDR_LATCH_03;
  /*p08.AVYS*/ Reg2 ADDR_LATCH_04;
  /*p08.ATEV*/ Reg2 ADDR_LATCH_05;
  /*p08.AROS*/ Reg2 ADDR_LATCH_06;
  /*p08.ARYM*/ Reg2 ADDR_LATCH_07;
  /*p08.LUNO*/ Reg2 ADDR_LATCH_08;
  /*p08.LYSA*/ Reg2 ADDR_LATCH_09;
  /*p08.PATE*/ Reg2 ADDR_LATCH_10;
  /*p08.LUMY*/ Reg2 ADDR_LATCH_11;
  /*p08.LOBU*/ Reg2 ADDR_LATCH_12;
  /*p08.LONU*/ Reg2 ADDR_LATCH_13;
  /*p08.NYRE*/ Reg2 ADDR_LATCH_14;

  /*p08.SOMA*/ Reg2 DATA_LATCH_00;
  /*p08.RONY*/ Reg2 DATA_LATCH_01;
  /*p08.RAXY*/ Reg2 DATA_LATCH_02;
  /*p08.SELO*/ Reg2 DATA_LATCH_03;
  /*p08.SODY*/ Reg2 DATA_LATCH_04;
  /*p08.SAGO*/ Reg2 DATA_LATCH_05;
  /*p08.RUPA*/ Reg2 DATA_LATCH_06;
  /*p08.SAZY*/ Reg2 DATA_LATCH_07;
};

//-----------------------------------------------------------------------------

};
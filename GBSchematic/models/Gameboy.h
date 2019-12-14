#pragma once

typedef const bool wire;

#pragma warning(disable:4201)

//-----------------------------------------------------------------------------

struct ChipIn {

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
                             
  /* PIN_17 */ bool D0_C;    // -> P08.TOVO,SOMA
  /* PIN_18 */ bool D1_C;    // -> P08.RUZY,RONY
  /* PIN_19 */ bool D2_C;    // -> P08.ROME,RAXY
  /* PIN_20 */ bool D3_C;    // -> P08.SAZA,SELO
  /* PIN_21 */ bool D4_C;    // -> P08.TEHE,SODY
  /* PIN_22 */ bool D5_C;    // -> P08.RATU,SAGO
  /* PIN_23 */ bool D6_C;    // -> P08.SOCA,RUPA
  /* PIN_24 */ bool D7_C;    // -> P08.RYBA,SAZY

  /* PIN_25 */ bool MD0_C;
  /* PIN_26 */ bool MD1_C;
  /* PIN_27 */ bool MD2_C;
  /* PIN_28 */ bool MD3_C;
  /* PIN_29 */ bool MD4_C;
  /* PIN_30 */ bool MD5_C;
  /* PIN_31 */ bool MD6_C;
  /* PIN_33 */ bool MD7_C;

  /* PIN_34 */ bool MA00;    // <- P04.ECAL
  /* PIN_35 */ bool MA01;    // <- P04.EGEZ
  /* PIN_36 */ bool MA02;    // <- P04.FUHE
  /* PIN_37 */ bool MA03;    // <- P04.FYZY
  /* PIN_38 */ bool MA04;    // <- P04.DAMU
  /* PIN_39 */ bool MA05;    // <- P04.DAVA
  /* PIN_40 */ bool MA06;    // <- P04.ETEG
  /* PIN_41 */ bool MA07;    // <- P04.EREW
  /* PIN_42 */ bool MA12;    // <- P04.EXYF

  /* PIN_43 */ bool MCS_C;   // -> P25.TEFY
  /* PIN_45 */ bool MOE_C;   // -> P25.TAVY
  /* PIN_49 */ bool MWR_C;   // -> P25.SUDO
                             
  /* PIN_64 */ bool P13_C;   // -> P02.KERY, P05.KOLO
  /* PIN_65 */ bool P12_C;   // -> P02.KERY, P05.KEJA
  /* PIN_66 */ bool P11_C;   // -> P02.KERY, P05.KAPA

  /* PIN_67 */ bool P10_B;   // This pin is weird and seems to be used as an input, or at least I can't find the driver
                             
  /* PIN_67 */ bool P10_C;   // -> P02.KERY, P05.KEVU
                             
  /* PIN_68 */ bool SCK_C;   // -> P06.CAVE, in
  /* PIN_69 */ bool SIN_C;   // -> P06.CAGE (check this)
                             
  /* PIN_71 */ bool RST;     // -> TUBO,ASOL,UFOL,UPOJ

  ///* PIN_74 */ bool CLKIN_A; // clock good signal
  /* PIN_74 */ bool CLKIN_B; // clock signal

  /* PIN_76 */ bool T2;      // -> P07.UVAR
  /* PIN_77 */ bool T1;      // -> P07.UBET
                             
  /* PIN_78 */ bool WR_C;    // -> P07.UBAL
  /* PIN_79 */ bool RD_C;    // -> P07.UJYV
};

//-----------------------------------------------------------------------------

struct ChipOut {

  //----------
  // Address pins

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
  // VRAM data pins

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
                             
  /* PIN_43 */ bool MCS_A;   // <- P25.SOKY
  /* PIN_43 */ bool MCS_D;   // <- P25.SETY
                             
  /* PIN_45 */ bool MOE_A;   // <- P25.REFO
  /* PIN_45 */ bool MOE_D;   // <- P25.SAHA
                             
  /* PIN_49 */ bool MWR_A;   // <- P25.SYSY
  /* PIN_49 */ bool MWR_D;   // <- P25.RAGU

  //----------
  // LCD pins

  /* PIN_50 */ bool LD1;
  /* PIN_51 */ bool LD0;
  /* PIN_52 */ bool CPG;
  /* PIN_53 */ bool CP;
  /* PIN_54 */ bool ST;
  /* PIN_55 */ bool CPL;
  /* PIN_56 */ bool FR;
  /* PIN_57 */ bool S;

  //----------
  // Joypad pins

  /* PIN_62 */ bool P15_A;   // <- p05.CELA
  /* PIN_62 */ bool P15_D;   // <- p05.COFY
  
  /* PIN_63 */ bool P14_A;   // <- p05.KARU
  /* PIN_63 */ bool P14_D;   // <- p05.KELY
  
  /* PIN_64 */ bool P13_A;   // <- P05.KORY
  /* PIN_64 */ bool P13_B;   
  /* PIN_64 */ bool P13_D;   // <- P05.KALE
                             
  /* PIN_65 */ bool P12_A;   // <- P05.KYHU
  /* PIN_65 */ bool P12_B;   
  /* PIN_65 */ bool P12_D;   // <- P05.KASY
                             
  /* PIN_66 */ bool P11_A;   // <- P05.KYTO
  /* PIN_66 */ bool P11_B;   
  /* PIN_66 */ bool P11_D;   // <- P05.KABU
                             
  /* PIN_67 */ bool P10_A;   // <- KOLE
  /* PIN_67 */ bool P10_D;   // <- KYBU

  //----------
  // Serial pins

  /* PIN_68 */ bool SCK_A;   // <- P06.KEXU
  /* PIN_68 */ bool SCK_B;   // <- P06.CULY
  /* PIN_68 */ bool SCK_D;   // <- P06.KUJO

  /* PIN_69 */ bool SIN_A;   // where's the connection to these?
  /* PIN_69 */ bool SIN_B;
  /* PIN_69 */ bool SIN_D;

  /* PIN_70 */ bool SOUT;    // <- P05.KENA

  //----------
  // Cart pins

  /* PIN_75 */ bool PHI;
  /* PIN_78 */ bool WR_A;    // <- P08.UVER
  /* PIN_78 */ bool WR_D;    // <- P08.USUF
  /* PIN_79 */ bool RD_A;    // <- P08.UGAC
  /* PIN_79 */ bool RD_D;    // <- P08.URUN
  /* PIN_80 */ bool CS_A;    // <- P08.TYHO
};

//-----------------------------------------------------------------------------

struct CpuIn {

  //----------
  // top right port

  // ADDR_VALID and one of the other signals might be switched

  bool CPU_RAW_RD;     // PORTA_00: -> P07.UJYV, P08.LAGU, P08.LAVO
  bool CPU_RAW_WR;     // PORTA_01: -> P01.AREV, P08.LAGU. This is almost definitely "raw write"
  bool ADDR_VALID;     // PORTA_06: -> P01.AGUT (actually APAP on die?), P08.TEXO. This is almost definitely "address valid"

  //----------
  // bottom right port

  bool CPU_A00;        // PORTB_00: -> A00
  bool FROM_CPU9;      // PORTB_01: -> P02.LETY, vblank int ack
  bool CPU_A08;        // PORTB_02: -> A08
  bool CPU_A01;        // PORTB_04: -> A01
  bool FROM_CPU8;      // PORTB_05: -> P02.LEJA, stat int ack
  bool CPU_A09;        // PORTB_06: -> A09
  bool CPU_A02;        // PORTB_08: -> A02
  bool FROM_CPU10;     // PORTB_09: -> P02.LESA, timer int ack
  bool CPU_A10;        // PORTB_10: -> A10
  bool CPU_A03;        // PORTB_12: -> A03
  bool FROM_CPU7;      // PORTB_13: -> P02.LUFE, serial int ack
  bool CPU_A11;        // PORTB_14: -> A11
  bool CPU_A04;        // PORTB_16: -> A04
  bool FROM_CPU11;     // PORTB_17: -> P02.LAMO, joypad int ack
  bool CPU_A12;        // PORTB_18: -> A12
  bool CPU_A05;        // PORTB_20: -> A05
  bool CPU_A13;        // PORTB_22: -> A13
  bool CPU_A06;        // PORTB_24: -> A06
  bool CPU_A14;        // PORTB_26: -> A14
  bool CPU_A07;        // PORTB_28: -> A07
  bool CPU_A15;        // PORTB_30: -> A15

  //----------
  // top center port

  //bool CPUCLK_REQ;     // PORTC_00: -> ABOL (an inverter) -> BATE. Something about "cpu ready". clock request?

  //----------
  // top left port

  bool FROM_CPU6;      // PORTD_00: -> P07.LEXY, doesn't do anything

  // PORTD_05: -> FROM_CPU5 - controls driving the external data pins onto the
  // internal data bus and other stuff. is this actually a clock, or like OE?
  bool FROM_CPU5;

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

struct CpuOut {

  //----------
  // top right wire by itself

  bool TO_CPU2;        // <- P02.ASOK

  //----------
  // top right port

  bool T1nT2;          // PORTA_02: <- P07.T1nT2
  bool SYRO;           // PORTA_03: <- P25.SYRO
  bool READ_BOOTROM;   // PORTA_04: <- P07.READ_BOOTROM
  bool T1T2n;          // PORTA_05: <- P07.T1T2n

  //----------
  // bottom right port

  bool TO_CPU3;        // PORTB_03: <- P02.LOPE, vblank int
  bool TO_CPU6;        // PORTB_07: <- P02.LALU, stat int
  bool TO_CPU7;        // PORTB_11: <- P02.NYBO, timer int
  bool TO_CPU4;        // PORTB_15: <- P02.UBUL, serial int
  bool TO_CPU5;        // PORTB_19: <- P02.ULAK, joypad int

  //----------
  // top center port

  bool AFER;           // PORTC_01: <- P01.AFER , reset related reg
  bool PIN_RESET;      // PORTC_02: <- PIN_RESET directly connected to the pad
  bool CLKIN_A;        // PORTC_03: <- chip.CLKIN_A top wire on PAD_XI,
  bool CPU_RESET;      // PORTC_04: <- P01.CPU_RESET

  //----------
  // top left port

  bool CLK_xBCDEFGH;   // PORTD_01: <- P01.BOWA
  bool BEDO;           // PORTD_02: <- P01.BEDO _____fgh

  bool BEKO;           // PORTD_03: <- P01.BEKO ____efgh connection not indicated on P01
  bool PHI_OUT;        // PORTD_04: <- P01.BUDE abcd____

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

union Bus {
  struct {
    bool A00 : 1;
    bool A01 : 1;
    bool A02 : 1;
    bool A03 : 1;
    bool A04 : 1;
    bool A05 : 1;
    bool A06 : 1;
    bool A07 : 1;
    bool A08 : 1;
    bool A09 : 1;
    bool A10 : 1;
    bool A11 : 1;
    bool A12 : 1;
    bool A13 : 1;
    bool A14 : 1;
    bool A15 : 1;
    bool D0 : 1;
    bool D1 : 1;
    bool D2 : 1;
    bool D3 : 1;
    bool D4 : 1;
    bool D5 : 1;
    bool D6 : 1;
    bool D7 : 1;
    char pad1;
  };
  struct {
    unsigned short A;
    unsigned char D;
    unsigned char pad2;
  };
};

struct Gameboy {
  Bus bus;
  Bus vbus;

  bool BOOT_CS;
  bool HRAM_CS;
};

//-----------------------------------------------------------------------------

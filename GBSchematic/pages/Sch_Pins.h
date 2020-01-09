#pragma once
#include "Sch_Common.h"

namespace Schematics {

struct VramBus;
struct DmaSignals;
struct DmaRegisters;

//-----------------------------------------------------------------------------

struct VramPinsIn {

};

struct VramPins {

  void tick(const SysSignals& sys_sig,
            const ClkSignals& clk_sig,
            const RstSignals& rst_sig,
            const BusSignals& bus_sig,
            const DecoderSignals& dec_sig,
            const VidRegisters& vid_reg,
            const VramBus& vram_bus,
            BusTristates& bus_tri);

  bool commit();

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

struct AudioPins {
};


//-----------------------------------------------------------------------------

struct CartPins {

  void tick(const SysSignals& sys_sig,
            const BusSignals& bus_sig,
            const ClkSignals& clk_sig,
            const DecoderSignals& dec_sig,
            const DmaSignals& dma_sig,
            const DmaRegisters& dma_reg,
            BusTristates& bus_tri);

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
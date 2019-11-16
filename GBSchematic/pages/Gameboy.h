#pragma once

#include "../Schematics.h"
#include "ChipSignals.h"
#include "CpuSignals.h"

#include "P01_ClocksReset.h"
#include "P02_Interrupts.h"
#include "P03_Timer.h"
#include "P04_DMA.h"
#include "P05_JoypadIO.h"
#include "P06_SerialLink.h"
#include "P07_SysDecode.h"
#include "P08_ExtCpuBuses.h"
#include "P09_ApuControl.h"
#include "P10_ApuDecode.h"
#include "P11_Ch1Regs.h"
#include "P12_Ch1Sweep.h"
#include "P13_Channel1.h"
#include "P14_Ch2Regs.h"
#include "P15_Channel2.h"
#include "P16_Ch3Regs.h"
#include "P17_WaveRam.h"
#include "P18_Channel3.h"
#include "P19_Ch4Regs.h"
#include "P20_Channel4.h"
#include "P21_VideoControl.h"
#include "P22_PpuDecode.h"
#include "P23_VideoRegs.h"
#include "P24_LcdControl.h"
#include "P25_VramInterface.h"
#include "P26_Background.h"
#include "P27_WindowMapLookup.h"
#include "P28_OAM.h"
#include "P29_SpriteControl.h"
#include "P30_SpriteStore.h"
#include "P31_SpriteXMatchers.h"
#include "P32_BgPixelShifter.h"
#include "P33_SpritePixelShifter.h"
#include "P34_SpritePaletteShifter.h"
#include "P35_PixelMux.h"
#include "P36_Palettes.h"

//-----------------------------------------------------------------------------

struct P01_ClocksReset {

  //----------
  // CPU reset tree

  /*p01.UPYF*/ bool UPYF;
  /*p01.TUBO*/ bool TUBO;
  /*p01.UNUT*/ bool UNUT;
  /*p01.TABA*/ bool CPU_RESET;
  /*p01.ALYP*/ bool CPU_RESETn;

  //----------
  // SYS reset tree

  /*BOMA*/ bool RESET_CLK; // _____fgh -> PORTD_07

  /*p01.AFAR*/ bool AFAR; // this is the sys reset register
  /*p01.AFAR*/ bool ASOL;
  /*p01.AFER*/ bool RESET_REG;
  /*p01.AVOR*/ bool AVOR;
  /*p01.ALUR*/ bool SYS_RESETn1;
  /*p01.DULA*/ bool SYS_RESET1;
  /*p09.HAPO*/ bool SYS_RESET2;
  /*p01.CUNU*/ bool SYS_RESETn2;
  /*p01.GUFO*/ bool SYS_RESETn3;
  /*p01.XORE*/ bool SYS_RESET3;
  /*p01.XEBE*/ bool SYS_RESETn4;
  /*p01.WALU*/ bool SYS_RESETn5;
  /*p01.WESY*/ bool SYS_RESETn6;
  /*XARE*/ bool SYS_RESETn7;

  //----------
  // VID reset tree

  /*p01.XODO*/ bool VID_RESET1;
  /*p01.XAPO*/ bool VID_RESETn1;
  /*p01.LYHA*/ bool VID_RESET2;
  /*TOFU*/ bool VID_RESET3;
  /*p01.PYRY*/ bool VID_RESET4;
  /*ROSY*/ bool VID_RESET5;
  /*p01.ATAR*/ bool VID_RESET6;
  /*AMYG*/ bool VID_RESET7;
  /*p01.LYFE*/ bool VID_RESETn2;
  /*ABEZ*/ bool VID_RESETn3;

  //----------
  // APU reset tree

  /*P09.JYRO*/ bool APU_RST_00;

  /*p01.BOPO*/ bool APU_RESETn1;
  /*ATUS*/ bool APU_RESETn2;
  /*p01.BELA*/ bool APU_RESETn3;

  //----------
  // Clock control

  /*p01.ABOL*/ bool CPUCLK_REQn;
  /*p01.BUTY*/ bool CPUCLK_REQ;
  /*p01.UCOB*/ bool CLK_BAD1;
  /*p01.ATEZ*/ bool CLK_BAD2;

  //----------
  // 4M clocks
  /*p01.APUV*/ bool CLK_AxCxExGx1;
  /*p01.ARYF*/ bool CLK_AxCxExGx2;
  /*p01.ATAL*/ bool CLK_AxCxExGx3;
  /*p01.ALET*/ bool CLK_AxCxExGx4;
  /*p01.ARYS*/ bool CLK_AxCxExGx5;
  /*p01.ANOS*/ bool CLK_AxCxExGx6;
  /*p01.CYBO*/ bool CLK_AxCxExGx7;
  /*p01.ATAG*/ bool CLK_AxCxExGx8;
  /*p01.ZAXY*/ bool CLK_AxCxExGx9;
  /*p01.TAVA*/ bool CLK_AxCxExGx10;

  /*p01.AMUK*/ bool CLK_xBxDxFxH1;
  /*p01.ZEME*/ bool CLK_xBxDxFxH2;
  /*p01.AVET*/ bool CLK_xBxDxFxH3;
  /*p01.AZOF*/ bool CLK_xBxDxFxH4;
  /*p01.LAPE*/ bool CLK_xBxDxFxH5;

  // 2M clocks
  /*p01.CERY*/ bool CLK_ABxxEFxx1; // ab__ef__

                                   // 1M clocks

                                   // 1mhz phase generator, regs in order of triggering.
  /*p01.AFUR*/ bool CLK_ABCDxxxx1;
  /*p01.ALEF*/ bool CLK_xBCDExxx1;
  /*p01.APUK*/ bool CLK_xxCDEFxx1;
  /*p01.ADYK*/ bool CLK_xxxDEFGx1;
  /*p01.ATYP*/ bool CLK_xxxxEFGH1;
  /*p01.AFEP*/ bool CLK_AxxxxFGH1;
  /*p01.AROV*/ bool CLK_ABxxxxGH1;
  /*p01.ADAR*/ bool CLK_ABCxxxxH1;
  /*p01.AJAX*/ bool CLK_ABCDxxxx3;
  /*p01.BUGO*/ bool CLK_xBCDExxx2;

  /*p01.AFAS*/ bool CLK_xxxDxxxx1;
  /*p01.BUTO*/ bool CLK_ABCDExxx3;
  /*p01.BELE*/ bool CLK_xxxxxFGH1;

  //----------
  // clocks for the cpu

  /*p01.BAPY*/ bool CPUCLK_xxDExxxx1;
  /*p01.BUFA*/ bool CPUCLK_xxDExxxx2;
  /*p01.BYLY*/ bool CPUCLK_xxDExxxx3;
  /*p01.BERU*/ bool CPUCLK_ABxxEFGH1;
  /*p01.BOLO*/ bool CPUCLK_ABxxEFGH2;
  /*p01.BYDA*/ bool CPUCLK_ABxxEFGH3;

  /*NULE*/ bool CPUCLK_ABCDxxxx1;
  /*p01.BUDE*/ bool CPUCLK_ABCDxxxx2; // -> PORTD_04
  /*p01.BEVA*/ bool CPUCLK_ABCDxxxx3;
  /*p01.BEJA*/ bool CPUCLK_ABCDxxxx4;
  /*p01.BELO*/ bool CPUCLK_ABCDxxxx5;
  /*p01.BYRY*/ bool CPUCLK_xxxxEFGH1;
  /*p01.BEKO*/ bool CPUCLK_xxxxEFGH2; // -> PORTD_03
  /*p01.BAVY*/ bool CPUCLK_xxxxEFGH3;
  /*p01.BANE*/ bool CPUCLK_xxxxEFGH4;
  /*p01.BAZE*/ bool CPUCLK_xxxxEFGH5;
  /*UVYT*/ bool CPUCLK_xxxxEFGH8;
  /*p01.DOVA*/ bool CPUCLK_xxxxEFGH9;

  /*BOWA*/ bool CPUCLK_ABCDExxx2; // -> PORTD_01
  /*BEDO*/ bool CPUCLK_xxxxxFGH2; // -> PORTD_02
  /*BUKE*/ bool CPUCLK_xxxxxFxx1; // -> PORTD_06
  /*BATE*/ bool CPUCLK_xxxxxFxx2;
  /*BASU*/ bool CPUCLK_xxxxxFxxn;

  /*p01.BYJU*/ bool BYJU; // abcde___

                          //----------
                          // clocks for the cart


  bool BUVU; // _____fgh

  bool AVOK;

  //----------
  // clocks for the apu

  /*P09.AJER*/ bool AJER_2M;
  /*P09.DYFA*/ bool DYFA_1M;

  /*p01.BAVU*/ bool BAVU_1M;
  /*p01.HAMA*/ bool APUCLK_512Kn;
  /*p01.JESO*/ bool JESO_512K;
  /*p01.HORU*/ bool HORU_512;
  /*p01.BUFY*/ bool BUFY_256;
  /*p01.BYFE*/ bool CLK_128;

  /*p01.COKE*/ bool COKE; // ? clock
  /*p01.BURE*/ bool BURE;
  /*p01.FYNE*/ bool FYNE;
  /*p01.CULO*/ bool CULO;
  /*p01.APEF*/ bool APEF;
  /*p01.GALE*/ bool GALE;
  /*p01.BEZE*/ bool BEZE;
  /*p01.BULE*/ bool BULE;
  /*p01.GEXY*/ bool GEXY;
  /*p01.COFU*/ bool COFU;
  /*p01.BARU*/ bool BARU;
  /*p01.BARA*/ bool BARA;
  /*p01.CARU*/ bool CARU;
  /*p01.BYLU*/ bool BYLU;
  /*p01.ATYK*/ bool ATYK;

  //----------
  // DIV

  /*BOGA*/ bool DIV_CLK;  // ABCDExxx -> PORTD_08
  /*UFOL*/ bool DIV_RSTn;
  /*TAGY*/ bool DIV_RD;
  /*TAPE*/ bool DIV_WR;

  // out to high half mux
  /*p01.UVYN*/ bool CLK_16K;

  /*p01.UKUP*/ bool DIV_00;
  /*p01.UFOR*/ bool DIV_01;
  /*p01.UNER*/ bool DIV_02;
  /*p01.TERO*/ bool DIV_03;
  /*p01.UNYK*/ bool DIV_04;
  /*p01.TAMA*/ bool DIV_05;
  /*p01.UGOT*/ bool DIV_06;
  /*p01.TULU*/ bool DIV_07;
  /*p01.TUGO*/ bool DIV_08;
  /*p01.TOFE*/ bool DIV_09;
  /*p01.TERU*/ bool DIV_10;
  /*p01.SOLA*/ bool DIV_11;
  /*p01.SUBU*/ bool DIV_12;
  /*p01.TEKA*/ bool DIV_13;
  /*p01.UKET*/ bool DIV_14;
  /*p01.UPOF*/ bool DIV_15;

  /*p01.UMEK*/ bool DIV_06n;
  /*p01.UREK*/ bool DIV_07n;
  /*p01.UTOK*/ bool DIV_08n;
  /*p01.SAPY*/ bool DIV_09n;
  /*p01.UMER*/ bool DIV_10n;
  /*p01.RAVE*/ bool DIV_11n;
  /*p01.RYSO*/ bool DIV_12n;
  /*p01.UDOR*/ bool DIV_13n;

  /*p01.TAWU*/ bool DIV_D0;
  /*p01.TAKU*/ bool DIV_D1;
  /*p01.TEMU*/ bool DIV_D2;
  /*p01.TUSE*/ bool DIV_D3;
  /*p01.UPUG*/ bool DIV_D4;
  /*p01.SEPU*/ bool DIV_D5;
  /*p01.SAWA*/ bool DIV_D6;
  /*p01.TATU*/ bool DIV_D7;

  //----------
  // Gated rd/wr signals

  /*AREV*/ bool CPU_WR_SYNCn;
  /*APOV*/ bool CPU_WR_SYNC;      // ___d____

  /*p01.AGUT*/ bool AGUT;         // abcd__gh
  /*p01.AWOD*/ bool CPU_RD_SYNCn;
  /*p01.ABUZ*/ bool CPU_RD_SYNC;  // ____ef__

  /*p01.ULUR*/ bool ULUR;
  bool BALY;
  bool BYXO;


};

//-----------------------------------------------------------------------------

struct P02_Interrupts {

  /*p02.ASOK*/ bool INT_JP;
  /*p02.AWOB*/ bool TO_CPU2;

  // FF0F IF reg
  /*p02.LOPE*/ union { bool FF0F_0; bool TO_CPU3; };
  /*p02.UBUL*/ union { bool FF0F_1; bool TO_CPU4; };
  /*p02.ULAK*/ union { bool FF0F_2; bool TO_CPU5; };
  /*p02.LALU*/ union { bool FF0F_3; bool TO_CPU6; };
  /*p02.NYBO*/ union { bool FF0F_4; bool TO_CPU7; };

  /*p02.KERY*/ bool KERY;
  /*p02.ROTU*/ bool ROTU;
  /*p02.LETY*/ bool LETY;
  /*p02.MUXE*/ bool MUXE;
  /*p02.LUFE*/ bool LUFE;
  /*p02.SULO*/ bool SULO;
  /*p02.LAMO*/ bool LAMO;
  /*p02.SEME*/ bool SEME;
  /*p02.LEJA*/ bool LEJA;
  /*p02.NABE*/ bool NABE;
  /*p02.LESA*/ bool LESA;
  /*p02.RAKE*/ bool RAKE;
  /*p02.MYZU*/ bool MYZU;
  /*p02.LYTA*/ bool LYTA;
  /*p02.TOME*/ bool TOME;
  /*p02.TUNY*/ bool TUNY;
  /*p02.TOGA*/ bool TOGA;
  /*p02.TYME*/ bool TYME;
  /*p02.MODY*/ bool MODY;
  /*p02.MOVU*/ bool MOVU;
  /*p02.PYHU*/ bool PYHU;
  /*p02.PYGA*/ bool PYGA;
  /*p02.PESU*/ bool PESU;
  /*p02.POLA*/ bool POLA;
  /*p02.NELA*/ bool NELA;
  /*p02.PADO*/ bool PADO;
  /*p02.PEGY*/ bool PEGY;
  /*p02.NABO*/ bool NABO;
  /*p02.ROVA*/ bool ROVA;

  // glitch filter for joypad interrupt
  /*p02.ACEF*/ bool ACEF;
  /*p02.AGEM*/ bool AGEM;
  /*p02.APUG*/ bool APUG;
  /*p02.BATU*/ bool BATU;

  // FF0F IF reg
  /*p02.MATY*/ bool MATY;
  /*p02.NEJY*/ bool NEJY;
  /*p02.NUTY*/ bool NUTY;
  /*p02.MOPO*/ bool MOPO;
  /*p02.PAVY*/ bool PAVY;
};

//-----------------------------------------------------------------------------

struct Gameboy {
  int64_t timestamp;

  ChipSignals  chip;
  CpuSignals   cpu;

  P01_ClocksReset          p01;
  P02_Interrupts           p02;
  P03_Timer                p03;
  P04_DMA                  p04;
  P05_JoypadIO             p05;
  P06_SerialLink           p06;
  P07_SysDecode            p07;
  P08_ExtCpuBuses          p08;
  P09_ApuControl           p09;
  P10_ApuDecode            p10;
  P11_Ch1Regs              p11;
  P12_Ch1Sweep             p12;
  P13_Channel1             p13;
  P14_Ch2Regs              p14;
  P15_Channel2             p15;
  P16_Ch3Regs              p16;
  P17_WaveRam              p17;
  P18_Channel3             p18;
  P19_Ch4Regs              p19;
  P20_Channel4             p20;
  P21_VideoControl         p21;
  P22_PpuDecode            p22;
  P23_VideoRegs            p23;
  P24_LcdControl           p24;
  P25_VramInterface        p25;
  P26_Background           p26;
  P27_WindowMapLookup      p27;
  P28_OAM                  p28;
  P29_SpriteControl        p29;
  P30_SpriteStore          p30;
  P31_SpriteXMatchers      p31;
  P32_BgPixelShifter       p32;
  P33_SpritePixelShifter   p33;
  P34_SpritePaletteShifter p34;
  P35_PixelMux             p35;
  P36_Palettes             p36;

  //----------
  // main bus

  union {
    uint16_t A;
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
    };
  };

  union { 
    uint8_t D;
    struct {
      bool D0 : 1;
      bool D1 : 1;
      bool D2 : 1;
      bool D3 : 1;
      bool D4 : 1;
      bool D5 : 1;
      bool D6 : 1;
      bool D7 : 1;
    };
  };

  //----------
  // vram bus

  bool MA00,MA01,MA02,MA03,MA04,MA05,MA06,MA07,MA08,MA09,MA10,MA11,MA12;
  bool MD0,MD1,MD2,MD3,MD4,MD5,MD6,MD7;

  //----------
  // oam bus

  bool OAM_A_D0,OAM_A_D1,OAM_A_D2,OAM_A_D3,OAM_A_D4,OAM_A_D5,OAM_A_D6,OAM_A_D7;
  bool OAM_B_D0,OAM_B_D1,OAM_B_D2,OAM_B_D3,OAM_B_D4,OAM_B_D5,OAM_B_D6,OAM_B_D7;

  //----------
  // wave ram bus

  bool WAVE_D0;
  bool WAVE_D1;
  bool WAVE_D2;
  bool WAVE_D3;
  bool WAVE_D4;
  bool WAVE_D5;
  bool WAVE_D6;
  bool WAVE_D7;
};

//-----------------------------------------------------------------------------

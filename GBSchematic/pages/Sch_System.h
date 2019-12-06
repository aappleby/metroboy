#pragma once

namespace Schematics {

struct CpuIn;
struct ChipIn;
struct Gameboy;

//-----------------------------------------------------------------------------

struct System {

  //----------
  // SYS reset tree

  /*p01.BOMA*/ bool RESET_CLK; // _____fgh -> PORTD_07
  /*p01.AFER*/ bool RESET_REG;

  
  /*p01.ALUR*/ bool SYS_RESETn1;
  /*p09.HAPO*/ bool SYS_RESET2;
  /*p01.CUNU*/ bool SYS_RESETn2;
  /*p01.GUFO*/ bool SYS_RESETn3;
  /*p01.XEBE*/ bool SYS_RESETn4;
  /*p01.WALU*/ bool SYS_RESETn5;
  /*p01.WESY*/ bool SYS_RESETn6;
  /*p01.XARE*/ bool SYS_RESETn7;

  //----------
  // VID reset tree

  /*p01.XODO*/ bool VID_RESET1;
  /*p01.XAPO*/ bool VID_RESETn1;
  /*p01.LYHA*/ bool VID_RESET2;
  /*p01.TOFU*/ bool VID_RESET3;
  /*p01.PYRY*/ bool VID_RESET4;
  /*p01.ROSY*/ bool VID_RESET5;
  /*p01.ATAR*/ bool VID_RESET6;
  /*p01.AMYG*/ bool VID_RESET7;
  /*p01.LYFE*/ bool VID_RESETn2;
  /*p01.ABEZ*/ bool VID_RESETn3;

  //----------
  // Clock control

  /*p01.ABOL*/ bool CPUCLK_REQn;
  /*p01.BUTY*/ bool CPUCLK_REQ;
  /*p01.UCOB*/ bool CLK_BAD1;
  /*p01.ATEZ*/ bool CLK_BAD2;

  //----------
  // 4M clocks
  /*p01.ARYS*/ bool CLK_AxCxExGx5; // not(CLKIN_B);

  /*p01.ANOS*/ bool CLK_AxCxExGx6; // deglitch 1
  /*p01.AVET*/ bool CLK_xBxDxFxH3; // deglitch 2
  /*p01.ATAL*/ bool CLK_AxCxExGx3; // drives phase generator

  /*p01.ALET*/ bool CLK_AxCxExGx4; // video
  /*p01.LAPE*/ bool CLK_xBxDxFxH5; // video
  /*p01.TAVA*/ bool CLK_AxCxExGx10; // sprites
  /*p01.ZEME*/ bool CLK_xBxDxFxH2; // sprites, video

  // 1M clocks

  // 1mhz phase generator, regs in order of triggering.
  /*p01.AFUR*/ bool PHASE_ABCDxxxx1; // sys
  /*p01.ALEF*/ bool PHASE_xBCDExxx1; // sys
  /*p01.APUK*/ bool PHASE_xxCDEFxx1; // sys
  /*p01.ADYK*/ bool PHASE_xxxDEFGx1; // sys

  /*p01.AFAS*/ bool PHASE_xxxxEFGx3; // sys, vid

  /*p01.BUKE*/ bool PHASE_AxxxxxGH6; // ch3, PORTD_06?

  /*p01.UVYT*/ bool PHASE_ABCDxxxx7b; // sys
  /*p01.DOVA*/ bool PHASE_ABCDxxxx7c; // apu

  /*p04.MOPA*/ bool PHASE_xxxxEFGH8b; // sys, sprites

  /*p01.BOGA*/ bool BOGA_xBCDEFGH; // sys, PORTD_08?
  /*p01.BEDO*/ bool BEDO_Axxxxxxx; // sys, PORTD_02

  // redundant
  ///*p01.BYLY*/ bool CPUCLK_xxDExxxx3;
  ///*p01.BYDA*/ bool CPUCLK_ABxxEFGH3;
  ///*p01.BEVA*/ bool CPUCLK_ABCDxxxx3;
  ///*p01.BAVY*/ bool CPUCLK_xxxxEFGH3;

  //----------
  // DIV

  /*p01.UFOL*/ bool DIV_RSTn;

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

  //----------
  // Gated rd/wr signals

  /*p01.APOV*/ bool CPU_WR_xxxxEFGx;
  /*p01.ABUZ*/ bool ADDR_VALID_ABxxxxxx;

  /*p01.ULUR*/ bool DIV_06_CLK;


  /*p02.ASOK*/ bool INT_JP;
  /*p02.AWOB*/ bool TO_CPU2;

  //----------
  // FF0F IF reg

  /*
  Bit 0: V-Blank  Interrupt Request (INT 40h)  (1=Request)
  Bit 1: LCD STAT Interrupt Request (INT 48h)  (1=Request)
  Bit 2: Timer    Interrupt Request (INT 50h)  (1=Request)
  Bit 3: Serial   Interrupt Request (INT 58h)  (1=Request)
  Bit 4: Joypad   Interrupt Request (INT 60h)  (1=Request)
  */

  /*p02.LOPE*/ bool FF0F_0;
  /*p02.UBUL*/ bool FF0F_1;
  /*p02.ULAK*/ bool FF0F_2;
  /*p02.LALU*/ bool FF0F_3;
  /*p02.NYBO*/ bool FF0F_4;

  /*p02.MATY*/ bool FF0F_L0;
  /*p02.NEJY*/ bool FF0F_L1;
  /*p02.NUTY*/ bool FF0F_L2;
  /*p02.MOPO*/ bool FF0F_L3;
  /*p02.PAVY*/ bool FF0F_L4;

  //----------


  /*p02.NELA*/ bool NELA;
  /*p02.PADO*/ bool PADO;
  /*p02.PEGY*/ bool PEGY;
  /*p02.NABO*/ bool NABO;
  /*p02.ROVA*/ bool ROVA;

  // glitch filter for joypad interrupt
  /*p02.KERY*/ bool ANY_BUTTON;
  /*p02.BATU*/ bool JP_GLITCH0;
  /*p02.ACEF*/ bool JP_GLITCH1;
  /*p02.AGEM*/ bool JP_GLITCH2;
  /*p02.APUG*/ bool JP_GLITCH3;

  //----------

  /*p03.RYFO*/ bool FF04_FF07;

  //----------
  // FF07 TAC

  /*p03.SORA*/ bool FF07_RD;
  /*p03.SARA*/ bool FF07_WR;

  /*p03.SOPU*/ bool TAC_0;
  /*p03.SAMY*/ bool TAC_1;
  /*p03.SABO*/ bool TAC_2;

  /*p03.SUPE*/ bool FF07_D0;
  /*p03.ROTE*/ bool FF07_D1;
  /*p03.RYLA*/ bool FF07_D2;

  //----------
  // FF06 TMA

  /*p03.TUBY*/ bool FF06_RD;
  /*p03.TYJU*/ bool FF06_WR;

  /*p03.SABU*/ bool TMA_0;
  /*p03.NYKE*/ bool TMA_1;
  /*p03.MURU*/ bool TMA_2;
  /*p03.TYVA*/ bool TMA_3;
  /*p03.TYRU*/ bool TMA_4;
  /*p03.SUFY*/ bool TMA_5;
  /*p03.PETO*/ bool TMA_6;
  /*p03.SETA*/ bool TMA_7;

  //----------
  // TIMA clock mux

  /*p03.UVYR*/ bool CLK_64Kn;
  /*p03.UKAP*/ bool UKAP;
  /*p03.TECY*/ bool TECY;
  /*p03.UBOT*/ bool UBOT;
  /*p03.TEKO*/ bool TEKO;
  /*p03.SOGU*/ bool TIMA_CLK;

  // TIMA reload signal
  /*p03.MUZU*/ bool MUZU;
  /*p03.MEKE*/ bool MEKE;
  /*p03.MEXU*/ bool TIMA_LOAD;

  // TIMA reload mux

  /*p03.ROKE*/ bool TIMA_MUX_0;
  /*p03.PETU*/ bool TIMA_MUX_1;
  /*p03.NYKU*/ bool TIMA_MUX_2;
  /*p03.SOCE*/ bool TIMA_MUX_3;
  /*p03.SALA*/ bool TIMA_MUX_4;
  /*p03.SYRU*/ bool TIMA_MUX_5;
  /*p03.REFU*/ bool TIMA_MUX_6;
  /*p03.RATO*/ bool TIMA_MUX_7;

  /*p03.MULO*/ bool TIMA_RST;

  /*p03.PUXY*/ bool TIMA_LD_0;
  /*p03.NERO*/ bool TIMA_LD_1;
  /*p03.NADA*/ bool TIMA_LD_2;
  /*p03.REPA*/ bool TIMA_LD_3;
  /*p03.ROLU*/ bool TIMA_LD_4;
  /*p03.RUGY*/ bool TIMA_LD_5;
  /*p03.PYMA*/ bool TIMA_LD_6;
  /*p03.PAGU*/ bool TIMA_LD_7;

  //----------
  // FF05 TIMA

  /*p03.TEDA*/ bool FF05_RD;
  /*p03.TOPE*/ bool FF05_WRn;

  /*p03.REGA*/ bool TIMA_0;
  /*p03.POVY*/ bool TIMA_1;
  /*p03.PERU*/ bool TIMA_2;
  /*p03.RATE*/ bool TIMA_3;
  /*p03.RUBY*/ bool TIMA_4;
  /*p03.RAGE*/ bool TIMA_5;
  /*p03.PEDA*/ bool TIMA_6;
  /*p03.NUGA*/ bool TIMA_7;

  //----------
  // INT_TIMER delay

  /*p03.MUGY*/ bool TIMA_LOADn;
  /*p03.NYDU*/ bool TIMA_MAX;
  /*p03.MERY*/ bool INT_TIMER_IN;
  /*p03.MOBA*/ bool INT_TIMER;

  //----------
  // P04

  /*p04.DECY*/ bool FROM_CPU5n;
  /*p04.CATY*/ bool FROM_CPU5;
  /*p04.MAKA*/ bool FROM_CPU5_SYNC;

  //----------
  // DMA

  /*p04.LORU*/ bool FF46_WR;

  /*p04.LOKY*/ bool DMA_RUNNING;
  /*p04.META*/ bool DMA_CLK;

  /*p04.LOKO*/ bool DMA_RST;
  /*p04.LAPA*/ bool DMA_RSTn;

  /*p04.NAKY*/ bool DMA_A00;
  /*p04.PYRO*/ bool DMA_A01; 
  /*p04.NEFY*/ bool DMA_A02; 
  /*p04.MUTY*/ bool DMA_A03; 
  /*p04.NYKO*/ bool DMA_A04; 
  /*p04.PYLO*/ bool DMA_A05; 
  /*p04.NUTO*/ bool DMA_A06; 
  /*p04.MUGU*/ bool DMA_A07; 
  /*p04.NAFA*/ bool DMA_A08; 
  /*p04.PYNE*/ bool DMA_A09; 
  /*p04.PARA*/ bool DMA_A10; 
  /*p04.NYDO*/ bool DMA_A11; 
  /*p04.NYGY*/ bool DMA_A12; 
  /*p04.PULA*/ bool DMA_A13; 
  /*p04.POKU*/ bool DMA_A14; 
  /*p04.MARU*/ bool DMA_A15; 

  /*p04.MATU*/ bool DMA_RUNNING_SYNCn; // -> p25,p28

  /*p04.NOLO*/ bool DMA_DONE;
  /*p04.NAVO*/ bool DMA_DONEn;

  /*p04.LUVY*/ bool LUVY;
  /*p04.MYTE*/ bool DMA_DONE_SYNC;
  /*p04.LENE*/ bool LENE;

  /*p04.LARA*/ bool LARA;
  /*p04.MORY*/ bool DO_DMAn;

  /*p04.NAXY*/ bool NAXY;
  /*p04.POWU*/ bool POWU;
  /*p04.LUPA*/ bool LUPA;


  /*p04.LOGO*/ bool DMA_VRAMn;
  /*p04.MUDA*/ bool DMA_VRAM;
  /*p04.LEBU*/ bool DMA_A15n;
  /*p04.LYXE*/ bool LYXE;

  /*p04.WYJA*/ bool OAM_CS;
  /*p04.LUMA*/ bool DO_DMA; // if true, addr bus = dma addr
  /*p04.DUGA*/ bool OAM_ADDR_DMA;

  /*p04.LUFA*/ bool VRAM_TO_OAMn;
  /*p04.AHOC*/ bool VRAM_TO_OAMa;
  /*p04.MUHO*/ bool VRAM_TO_OAMb;


  //----------
  // P05

  // joypad int is in p02 so there's no outputs here

  //----------
  // FF00 JOYP

  /*p10.AMUS*/ bool ADDR_xxxxxxxx0xx00000;
  /*p10.ANAP*/ bool ADDR_111111110xx00000; // -> p07.APER
  /*p10.ATOZ*/ bool FF00_CLK;

  /*p05.KELY*/ bool KELY;
  /*p05.COFY*/ bool COFY;

  /*p05.JUTE*/ bool JOYP_RA;
  /*p05.KECY*/ bool JOYP_LB;
  /*p05.JALE*/ bool JOYP_UC;
  /*p05.KYME*/ bool JOYP_DS;
  /*p05.KUKO*/ bool DBG_FF00_D6;
  /*p05.KERU*/ bool DBG_FF00_D7;

  /*p05.KORE*/ bool P05_NC0;
  /*p05.KYWE*/ bool P05_NC1;

  /*p05.KEVU*/ bool JOYP_L0;
  /*p05.KAPA*/ bool JOYP_L1;
  /*p05.KEJA*/ bool JOYP_L2;
  /*p05.KOLO*/ bool JOYP_L3;

  //----------
  // P06

  // counter that triggers INT_SERIAL
  /*p06.CAFA*/ bool SER_CNT0;
  /*p06.CYLO*/ bool SER_CNT1;
  /*p06.CYDE*/ bool SER_CNT2;
  /*p06.CALY*/ bool SER_CNT3;
  /*p06.COBA*/ bool SER_CNT3n;

  /*p06.COTY*/ bool SER_CLK;


  /*p06.SANO*/ bool ADDR_FF00_FF03;

  //----------
  // FF01 SB

  /*p06.DAWA*/ bool SER_CLK1;
  /*p06.EDYL*/ bool SER_CLKn;
  /*p06.EPYT*/ bool SER_CLK2;
  /*p06.DAWE*/ bool SER_CLK3;

  /*p06.CAGE*/ bool SIN_Cn;

  /*p06.CUBA*/ bool SER_DATA0;
  /*p06.DEGU*/ bool SER_DATA1;
  /*p06.DYRA*/ bool SER_DATA2;
  /*p06.DOJO*/ bool SER_DATA3;
  /*p06.DOVU*/ bool SER_DATA4;
  /*p06.EJAB*/ bool SER_DATA5;
  /*p06.EROD*/ bool SER_DATA6;
  /*p06.EDER*/ bool SER_DATA7;

  /*p06.ELYS*/ bool SER_OUT;

  //----------
  // FF02 SC


  /*p06.ETAF*/ bool XFER_START;
  /*p06.CULY*/ bool XFER_DIR;


  /*p06.UWAM*/ bool FF02_CLK;

  //----------
  // P07

  /*p07.UBET*/ bool T1n;
  /*p07.UVAR*/ bool T2n;
  /*p07.UPOJ*/ bool MODE_PROD;
  /*p07.UMUT*/ bool MODE_DBG1;
  /*p07.YAZA*/ bool MODE_DBG1n;
  /*p07.UNOR*/ bool MODE_DBG2;
  /*p07.APET*/ bool MODE_DEBUG;

  /*p07.LEXY*/ bool PIN_NC;

  /*p07.TEDO*/ bool CPU_RD;
  /*p07.TAPU*/ bool CPU_WR;
  /*p07.ASOT*/ bool CPU_RD2;
  /*p07.CUPA*/ bool CPU_WR2;

  //----------
  // Bootrom control

  /*p07.TUTU*/ bool ADDR_BOOT;
  /*p07.TUGE*/ bool BOOT_BIT_CLK;
  /*p07.TEPU*/ bool BOOT_BIT;

  // FF60 - secret debug register
  /*p07.APER*/ bool FF60_WRn;
  /*p07.BURO*/ bool FF60_0;
  /*p07.AMUT*/ bool FF60_1;

  /*p07.SARO*/ bool ADDR_OAM;


  /*p07.SYKE*/ bool ADDR_FFXX; // used by a bunch of stuff

  /*p07.ROMY*/ bool DBG_0;
  /*p07.RYNE*/ bool DBG_1;
  /*p07.REJY*/ bool DBG_2;
  /*p07.RASE*/ bool DBG_3;
  /*p07.REKA*/ bool DBG_4;
  /*p07.ROWE*/ bool DBG_5;
  /*p07.RYKE*/ bool DBG_6;
  /*p07.RARU*/ bool DBG_7;


  /*p08.TOVA*/ bool MODE_DBG2n1;
  /*p08.RYCA*/ bool MODE_DBG2n2;

  /*p08.TEXO*/ bool ADDR_VALID_AND_NOT_VRAM; // sys, temp

  //----------
  // Address pin driver

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

  //----------
  // Chip data pin driver.

  /*p08.RORU*/ bool DBUS_OUTn;
  /*p08.LULA*/ bool DBUS_OUT;  // True if the internal data bus is driving the external data bus

  /*p08.LAVO*/ bool LATCH_DX_C;

  /*p08.SOMA*/ bool LATCH_D0_C;
  /*p08.RONY*/ bool LATCH_D1_C;
  /*p08.RAXY*/ bool LATCH_D2_C;
  /*p08.SELO*/ bool LATCH_D3_C;
  /*p08.SODY*/ bool LATCH_D4_C;
  /*p08.SAGO*/ bool LATCH_D5_C;
  /*p08.RUPA*/ bool LATCH_D6_C;
  /*p08.SAZY*/ bool LATCH_D7_C;

  //----------

  /*p08.KOVA*/ bool A00_Cn; // Debug, drives external address onto internal address
  /*p08.CAMU*/ bool A01_Cn;
  /*p08.BUXU*/ bool A02_Cn;
  /*p08.BASE*/ bool A03_Cn;
  /*p08.AFEC*/ bool A04_Cn;
  /*p08.ABUP*/ bool A05_Cn;
  /*p08.CYGU*/ bool A06_Cn;
  /*p08.COGO*/ bool A07_Cn;
  /*p08.MUJY*/ bool A08_Cn;
  /*p08.NENA*/ bool A09_Cn;
  /*p08.SURA*/ bool A10_Cn;
  /*p08.MADY*/ bool A11_Cn;
  /*p08.LAHE*/ bool A12_Cn;
  /*p08.LURA*/ bool A13_Cn;
  /*p08.PEVO*/ bool A14_Cn;
  /*p08.RAZA*/ bool A15_Cn;

  /*p08.KEJO*/ bool A00_C;
  /*p08.BYXE*/ bool A01_C;
  /*p08.AKAN*/ bool A02_C;
  /*p08.ANAR*/ bool A03_C;
  /*p08.AZUV*/ bool A04_C;
  /*p08.AJOV*/ bool A05_C;
  /*p08.BYNE*/ bool A06_C;
  /*p08.BYNA*/ bool A07_C;
  /*p08.LOFA*/ bool A08_C;
  /*p08.MAPU*/ bool A09_C;
  /*p08.RALA*/ bool A10_C;
  /*p08.LORA*/ bool A11_C;
  /*p08.LYNA*/ bool A12_C;
  /*p08.LEFY*/ bool A13_C;
  /*p08.NEFE*/ bool A14_C;
  /*p08.SYZU*/ bool A15_C;

  /*p08.MULE*/ bool MODE_DBG1o;
};

//-----------------------------------------------------------------------------

};
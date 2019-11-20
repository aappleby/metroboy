#pragma once

//-----------------------------------------------------------------------------

struct System {

  //----------
  // CPU reset tree

  /*p01.UPYF*/ bool UPYF;
  /*p01.TUBO*/ bool TUBO;
  /*p01.UNUT*/ bool UNUT;
  /*p01.TABA*/ bool CPU_RESET;
  /*p01.ALYP*/ bool CPU_RESETn;

  //----------
  // SYS reset tree

  /*p01.BOMA*/ bool RESET_CLK; // _____fgh -> PORTD_07

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
  // APU reset tree

  /*p09.JYRO*/ bool APU_RST; // This is the root of the APU reset tree

  /*p01.BOPO*/ bool APU_RESETn1;
  /*p01.ATUS*/ bool APU_RESETn2;
  /*p01.BELA*/ bool APU_RESETn3;
  /*p09.KAME*/ bool APU_RESETn5;

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

  /*p01.NULE*/ bool CPUCLK_ABCDxxxx1;
  /*p01.BUDE*/ bool CPUCLK_ABCDxxxx2; // -> PORTD_04
  /*p01.BEVA*/ bool CPUCLK_ABCDxxxx3;
  /*p01.BEJA*/ bool CPUCLK_ABCDxxxx4;
  /*p01.BELO*/ bool CPUCLK_ABCDxxxx5;
  /*p01.BYRY*/ bool CPUCLK_xxxxEFGH1;
  /*p01.BEKO*/ bool CPUCLK_xxxxEFGH2; // -> PORTD_03
  /*p01.BAVY*/ bool CPUCLK_xxxxEFGH3;
  /*p01.BANE*/ bool CPUCLK_xxxxEFGH4;
  /*p01.BAZE*/ bool CPUCLK_xxxxEFGH5;
  /*p01.UVYT*/ bool CPUCLK_xxxxEFGH8;
  /*p01.DOVA*/ bool CPUCLK_xxxxEFGH9;

  /*p01.BOWA*/ bool CPUCLK_ABCDExxx2; // -> PORTD_01
  /*p01.BEDO*/ bool CPUCLK_xxxxxFGH2; // -> PORTD_02
  /*p01.BUKE*/ bool CPUCLK_xxxxxFxx1; // -> PORTD_06
  /*p01.BATE*/ bool CPUCLK_xxxxxFxx2;
  /*p01.BASU*/ bool CPUCLK_xxxxxFxxn;

  /*p01.BYJU*/ bool BYJU; // abcde___

  //----------
  // clocks for the cart

  /*p01.BUVU*/ bool BUVU; // _____fgh
  /*p01.AVOK*/ bool AVOK;

  //----------
  // clocks for the apu

  /*p09.AJER*/ bool AJER_2M;
  /*p09.DYFA*/ bool DYFA_1M;
  /*p01.BAVU*/ bool BAVU_1M;

  /*p01.JESO*/ bool CLK_512Ka;
  /*p01.HAMA*/ bool CLK_512Kn;

  // these are used by all the channels
  /*p01.HORU*/ bool CLK_512a;
  /*p01.BUFY*/ bool CLK_256a;
  /*p01.BYFE*/ bool CLK_128a;

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

  /*p01.BOGA*/ bool DIV_CLK;  // ABCDExxx -> PORTD_08
  /*p01.UFOL*/ bool DIV_RSTn;
  /*p01.TAGY*/ bool DIV_RD;
  /*p01.TAPE*/ bool DIV_WR;

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

  /*p01.AREV*/ bool CPU_WR_SYNCn;
  /*p01.APOV*/ bool CPU_WR_SYNC;      // ___d____

  /*p01.AGUT*/ bool AGUT;         // abcd__gh
  /*p01.AWOD*/ bool CPU_RD_SYNCn;
  /*p01.ABUZ*/ bool CPU_RD_SYNC;  // ____ef__

  /*p01.ULUR*/ bool DIV_06_CLK;
  /*p01.BALY*/ bool BALY;
  /*p01.BYXO*/ bool BYXO;


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

  /*p07.SEMY*/ bool ADDR_XX0X;
  /*p07.SAPA*/ bool ADDR_XXXF;
  /*p07.ROLO*/ bool FF0F_RDn;
  /*p07.REFA*/ bool FF0F_WRn;

  /*p02.ROTU*/ bool FF0F_WRa;
  /*p02.POLA*/ bool FF0F_RDa;

  /*p02.LETY*/ bool LETY;
  /*p02.LEJA*/ bool LEJA;
  /*p02.LESA*/ bool LESA;
  /*p02.LUFE*/ bool LUFE;
  /*p02.LAMO*/ bool LAMO;

  /*p02.MUXE*/ bool MUXE;
  /*p02.NABE*/ bool NABE;
  /*p02.RAKE*/ bool RAKE;
  /*p02.SULO*/ bool SULO;
  /*p02.SEME*/ bool SEME;

  /*p02.MYZU*/ bool FF0F_SET0;
  /*p02.MODY*/ bool FF0F_SET1;
  /*p02.PYHU*/ bool FF0F_SET2;
  /*p02.TOME*/ bool FF0F_SET3;
  /*p02.TOGA*/ bool FF0F_SET4;

  /*p02.LYTA*/ bool FF0F_RST0;
  /*p02.MOVU*/ bool FF0F_RST1;
  /*p02.PYGA*/ bool FF0F_RST2;
  /*p02.TUNY*/ bool FF0F_RST3;
  /*p02.TYME*/ bool FF0F_RST4;

  /*p02.PESU*/ bool FF0F_IN;

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

  /*p03.RYFO*/ union { bool FF04_FF07; };
  /*p03.TOVY*/ union { bool A0n; };

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

  /*p04.MOLU*/ bool FF46_RDn1;
  /*p04.NYGO*/ bool FF46_RD;
  /*p04.PUSY*/ bool FF46_RDn2;
  /*p04.LAVY*/ bool FF46_WRn;
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

  /*p04.NAVO*/ bool DMA_DONE;
  /*p04.NOLO*/ bool DMA_DONEn;

  /*p04.LUVY*/ bool LUVY;
  /*p04.MYTE*/ bool MYTE;
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

  /*p04.WYJA*/ bool WYJA;
  /*p04.MOPA*/ bool CPUCLK_ABCDxxxx9;
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
  /*p10.BYKO*/ bool ADDR_xxxxxxxxxx0xxxxx;
  /*p10.AKUG*/ bool ADDR_xxxxxxxxx0xxxxxx;
  /*p10.ATOZ*/ bool FF00_WRn;
  /*p10.ACAT*/ bool FF00_RD;

  /*p05.JUTE*/ bool JOYP_RA;
  /*p05.KECY*/ bool JOYP_LB;
  /*p05.JALE*/ bool JOYP_UC;
  /*p05.KYME*/ bool JOYP_DS;
  /*p05.KELY*/ bool P14_D;
  /*p05.COFY*/ bool P15_D;
  /*p05.KUKO*/ bool FF00_D6;
  /*p05.KERU*/ bool FF00_D7;

  /*p05.KURA*/ bool FF60_0n;
  /*p05.JEVA*/ bool FF60_0o;

  /*p05.KENA*/ bool SOUT;

  /*p05.KORE*/ bool P05_NC0;
  /*p05.KYWE*/ bool P05_NC1;

  /*p05.BYZO*/ bool FF00_RDn;

  /*p05.KEVU*/ bool JOYP_L0;
  /*p05.KAPA*/ bool JOYP_L1;
  /*p05.KEJA*/ bool JOYP_L2;
  /*p05.KOLO*/ bool JOYP_L3;

  /*p05.KOLE*/ bool P10_A;
  /*p05.KYBU*/ bool P10_D;
  /*p05.KYTO*/ bool P11_A;
  /*p05.KABU*/ bool P11_D;
  /*p05.KYHU*/ bool P12_A;
  /*p05.KASY*/ bool P12_D;
  /*p05.KORY*/ bool P13_A;
  /*p05.KALE*/ bool P13_D;
  /*p05.KARU*/ bool P14_A;
  /*p05.CELA*/ bool P15_A;

  //----------

  /*p05.AXYN*/ bool AXYN;
  /*p05.ADYR*/ bool ADYR;
  /*p05.APYS*/ bool APYS;
  /*p05.AFOP*/ bool AFOP;

  //----------
  // P06

  /*p06.SARE*/ bool ADDR_XX00_XX07;

  // counter that triggers INT_SERIAL
  /*p06.CAFA*/ bool SER_CNT0;
  /*p06.CYLO*/ bool SER_CNT1;
  /*p06.CYDE*/ bool SER_CNT2;
  /*p06.CALY*/ bool SER_CNT3;
  /*p06.COBA*/ bool SER_CNT3n;

  /*p06.COTY*/ bool SER_CLK;
  /*p06.CAVE*/ bool SER_CLK_MUXn;
  /*p06.DAWA*/ bool SER_TICK;
  /*p06.EDYL*/ bool SER_TICKn;

  /*p06.SEFY*/ bool A02n;
  /*p06.SANO*/ bool ADDR_FF00_FF03;

  //----------
  // FF01 SB

  /*p06.URYS*/ bool FF01_WRn;
  /*p06.DAKU*/ bool FF01_WR;
  /*p06.UFEG*/ bool FF01_RD;

  /*p06.EPYT*/ bool SER_TICK2;
  /*p06.DEHO*/ bool SER_TICKn2;
  /*p06.DAWE*/ bool SER_TICK3;

  /*p06.CUFU*/ bool SER_DATA0_SETn;
  /*p06.DOCU*/ bool SER_DATA1_SETn;
  /*p06.DELA*/ bool SER_DATA2_SETn;
  /*p06.DYGE*/ bool SER_DATA3_SETn;
  /*p06.DOLA*/ bool SER_DATA4_SETn;
  /*p06.ELOK*/ bool SER_DATA5_SETn;
  /*p06.EDEL*/ bool SER_DATA6_SETn;
  /*p06.EFEF*/ bool SER_DATA7_SETn;

  /*p06.COHY*/ bool SER_DATA0_RSTn;
  /*p06.DUMO*/ bool SER_DATA1_RSTn;
  /*p06.DYBO*/ bool SER_DATA2_RSTn;
  /*p06.DAJU*/ bool SER_DATA3_RSTn;
  /*p06.DYLY*/ bool SER_DATA4_RSTn;
  /*p06.EHUJ*/ bool SER_DATA5_RSTn;
  /*p06.EFAK*/ bool SER_DATA6_RSTn;
  /*p06.EGUV*/ bool SER_DATA7_RSTn;

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

  /*p06.CABY*/ bool XFER_RESET;
  /*p06.ETAF*/ bool XFER_START;
  /*p06.CULY*/ bool XFER_DIR;
  /*p06.JAGO*/ bool XFER_DIRn;

  /*p06.UWAM*/ bool FF02_WRn;
  /*p06.UCOM*/ bool FF02_RD;

  /*p06.KEXU*/ bool SCK_A;
  /*p06.KUJO*/ bool SCK_D;

  /*p06.CARO*/ bool SER_RST;

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
  /*p07.AJAS*/ bool CPU_RDn;
  /*p07.UBAL*/ bool CPU_WR_MUX;
  /*p07.UJYV*/ bool CPU_RD_MUX;
  /*p07.DYKY*/ bool CPU_WRn;

  //----------
  // Bootrom control

  /*p07.TYRO*/ bool ADDR_0x0x0000;
  /*p07.TUFA*/ bool ADDR_x1x1xxxx;

  /*p07.TUTU*/ bool ADDR_BOOT;
  /*p07.TEXE*/ bool BOOT_BIT_RD;
  /*p07.TUGE*/ bool BOOT_BIT_WRn;
  /*p07.TEPU*/ bool BOOT_BIT;
  /*p07.SATO*/ bool BOOT_BIT_IN;
  /*p07.TERA*/ bool BOOT_BITn;

  /*p07.YULA*/ bool BOOT_RD;
  /*p07.ZADO*/ bool BOOT_CSn;
  /*p07.ZERY*/ bool BOOT_CS;

  /*p07.ZOLE*/ bool ADDR_00;
  /*p07.ZAJE*/ bool ADDR_01;
  /*p07.ZUBU*/ bool ADDR_10;
  /*p07.ZAPY*/ bool ADDR_11;

  /*p07.ZYRA*/ bool BOOTROM_A7n;
  /*p07.ZAGE*/ bool BOOTROM_A6n;
  /*p07.ZYKY*/ bool BOOTROM_A5nA4n;
  /*p07.ZYGA*/ bool BOOTROM_A5nA4;
  /*p07.ZOVY*/ bool BOOTROM_A5A4n;
  /*p07.ZUKO*/ bool BOOTROM_A5A4;
  /*p07.ZABU*/ bool BOOTROM_A3n;
  /*p07.ZOKE*/ bool BOOTROM_A2n;
  /*p07.ZETE*/ bool BOOTROM_A1nA0n;
  /*p07.ZEFU*/ bool BOOTROM_A1nA0;
  /*p07.ZYRO*/ bool BOOTROM_A1A0n;
  /*p07.ZAPA*/ bool BOOTROM_A1A0;

  /*p07.WALE*/ bool ADDR_x1111111n;
  /*p07.WOLY*/ bool HRAM_CSn;
  /*p07.WUTA*/ bool HRAM_CS;

  // FF60 - secret debug register
  /*p07.APER*/ bool FF60_WRn;
  /*p07.BURO*/ bool FF60_0;
  /*p07.AMUT*/ bool FF60_1;

  /*p07.BAKO*/ bool ADDR_FFXXn1;
  /*p07.SOHA*/ bool ADDR_FFXXn2;
  /*p07.SARO*/ bool ADDR_OAM;
  /*p07.ZYBA*/ bool ADDR_00n;
  /*p07.ZUVY*/ bool ADDR_01n;
  /*p07.ZUFY*/ bool ADDR_04n;
  /*p07.ZERA*/ bool ADDR_05n;
  /*p07.TONA*/ bool ADDR_08n;
  /*p07.TULO*/ bool ADDR_00XX;
  /*p07.ZUFA*/ bool ADDR_00XX2;
  /*p07.ZORO*/ bool ADDR_0XXX;
  /*p07.ZADU*/ bool ADDR_X0XX;

  /*p07.RYCU*/ bool ADDR_FE00_FFFF;
  /*p07.ROPE*/ bool ADDR_FEXXn;
  /*p07.SYKE*/ bool ADDR_FFXX;
  /*p07.TUNA*/ bool ADDR_0000_FE00;

  /*p07.LECO*/ bool LECO;
  /*p07.ROMY*/ bool DBG_0;
  /*p07.RYNE*/ bool DBG_1;
  /*p07.REJY*/ bool DBG_2;
  /*p07.RASE*/ bool DBG_3;
  /*p07.REKA*/ bool DBG_4;
  /*p07.ROWE*/ bool DBG_5;
  /*p07.RYKE*/ bool DBG_6;
  /*p07.RARU*/ bool DBG_7;


  /*p08.TOLA*/ bool A1n;
  /*p08.TOVA*/ union { bool MODE_DBG2n1; bool NET01; };
  /*p08.RYCA*/ bool MODE_DBG2n2;

  /*p08.TEXO*/ bool ADDR_VALID_AND_NOT_VRAM;

  //----------
  // Address pin driver

  /*p08.LOXO*/ bool ADDR_LATCHb;
  /*p08.LASY*/ bool ADDR_LATCHn;
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

  /*p08.SOBY*/ bool SOBY_15;
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

  /*p08.KUPO*/ bool PIN_A00_A;
  /*p08.CABA*/ bool PIN_A01_A;
  /*p08.BOKU*/ bool PIN_A02_A;
  /*p08.BOTY*/ bool PIN_A03_A;
  /*p08.BYLA*/ bool PIN_A04_A;
  /*p08.BADU*/ bool PIN_A05_A;
  /*p08.CEPU*/ bool PIN_A06_A;
  /*p08.DEFY*/ bool PIN_A07_A;
  /*p08.MYNY*/ bool PIN_A08_A;
  /*p08.MUNE*/ bool PIN_A09_A;
  /*p08.ROXU*/ bool PIN_A10_A;
  /*p08.LEPY*/ bool PIN_A11_A;
  /*p08.LUCE*/ bool PIN_A12_A;
  /*p08.LABE*/ bool PIN_A13_A;
  /*p08.PUHE*/ bool PIN_A14_A;
  /*p08.SUZE*/ bool PIN_A15_A;

  /*p08.KOTY*/ bool PIN_A00_D;
  /*p08.COTU*/ bool PIN_A01_D;
  /*p08.BAJO*/ bool PIN_A02_D;
  /*p08.BOLA*/ bool PIN_A03_D;
  /*p08.BEVO*/ bool PIN_A04_D;
  /*p08.AJAV*/ bool PIN_A05_D;
  /*p08.CYKA*/ bool PIN_A06_D;
  /*p08.COLO*/ bool PIN_A07_D;
  /*p08.MEGO*/ bool PIN_A08_D;
  /*p08.MENY*/ bool PIN_A09_D;
  /*p08.RORE*/ bool PIN_A10_D;
  /*p08.LYNY*/ bool PIN_A11_D;
  /*p08.LOSO*/ bool PIN_A12_D;
  /*p08.LEVA*/ bool PIN_A13_D;
  /*p08.PAHY*/ bool PIN_A14_D;
  /*p08.RULO*/ bool PIN_A15_D;

  //----------
  // Chip data pin driver.

  /*p08.RORU*/ bool DBUS_OUTn;
  /*p08.LULA*/ bool DBUS_OUT;  // True if the internal data bus is driving the external data bus

  /*p25.RUXA*/ bool D0_A;
  /*p25.RUJA*/ bool D1_A;
  /*p25.RABY*/ bool D2_A;
  /*p25.RERA*/ bool D3_A;
  /*p25.RORY*/ bool D4_A;
  /*p25.RYVO*/ bool D5_A;
  /*p25.RAVU*/ bool D6_A;
  /*p25.RAFY*/ bool D7_A;

  /*p08.RUNE*/ bool D0_D;
  /*p08.RYPU*/ bool D1_D;
  /*p08.SULY*/ bool D2_D;
  /*p08.SEZE*/ bool D3_D;
  /*p08.RESY*/ bool D4_D;
  /*p08.TAMU*/ bool D5_D;
  /*p08.ROGY*/ bool D6_D;
  /*p08.RYDA*/ bool D7_D;

  /*p08.LAVO*/ bool LATCH_DX_C;

  /*p08.SOMA*/ bool LATCH_D0_C;
  /*p08.RONY*/ bool LATCH_D1_C;
  /*p08.RAXY*/ bool LATCH_D2_C;
  /*p08.SELO*/ bool LATCH_D3_C;
  /*p08.SODY*/ bool LATCH_D4_C;
  /*p08.SAGO*/ bool LATCH_D5_C;
  /*p08.RUPA*/ bool LATCH_D6_C;
  /*p08.SAZY*/ bool LATCH_D7_C;

  /*p08.LYRA*/ bool DBG_D_RD;

  /*p08.TOVO*/ bool DBG_D0_Cn : 1;
  /*p08.RUZY*/ bool DBG_D1_Cn : 1;
  /*p08.ROME*/ bool DBG_D2_Cn : 1;
  /*p08.SAZA*/ bool DBG_D3_Cn : 1;
  /*p08.TEHE*/ bool DBG_D4_Cn : 1;
  /*p08.RATU*/ bool DBG_D5_Cn : 1;
  /*p08.SOCA*/ bool DBG_D6_Cn : 1;
  /*p08.RYBA*/ bool DBG_D7_Cn : 1;

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

  /*p08.SORE*/ bool ADDR_0000_7FFF;
  /*p08.TEVY*/ bool ADDR_NOT_VRAM;

  /*p08.LEVO*/ bool ADDR_VALID_AND_NOT_VRAMn;
  /*p08.LAGU*/ bool LAGU;
  /*p08.LYWE*/ bool LYWE;
  /*p08.MOCA*/ bool MOCA;
  /*p08.MEXO*/ bool MEXO;
  /*p08.NEVY*/ bool NEVY;
  /*p08.MOTY*/ bool MOTY;
  /*p08.PUVA*/ bool PUVA;
  /*p08.TYMU*/ bool TYMU;
  /*p08.TYNU*/ bool TYNU;
  /*p08.TOZA*/ bool TOZA;

  /*p08.SOGY*/ bool A14n;
  /*p08.TUMA*/ bool CART_RAM;

  /*p08.TYHO*/ bool CS_A;
  /*p08.UVER*/ bool WR_A;
  /*p08.USUF*/ bool WR_D;
  /*p08.UGAC*/ bool RD_A;
  /*p08.URUN*/ bool RD_D;

  /*p08.MULE*/ bool MODE_DBG1o;

  /*p08.REDU*/ bool CPU_RDo;
};

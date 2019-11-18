#pragma once

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
  /*p01.TOFU*/ bool VID_RESET3;
  /*p01.PYRY*/ bool VID_RESET4;
  /*p01.ROSY*/ bool VID_RESET5;
  /*p01.ATAR*/ bool VID_RESET6;
  /*p01.AMYG*/ bool VID_RESET7;
  /*p01.LYFE*/ bool VID_RESETn2;
  /*p01.ABEZ*/ bool VID_RESETn3;

  //----------
  // APU reset tree

  /*P09.JYRO*/ bool APU_RST; // This is the root of the APU reset tree

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


#pragma once

//-----------------------------------------------------------------------------
// This file should contain the schematics as directly translated to C,
// no modifications or simplifications.

struct P03_Timer {

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

  /*p03.SETE*/ bool FF06_D0;
  /*p03.PYRE*/ bool FF06_D1;
  /*p03.NOLA*/ bool FF06_D2;
  /*p03.SALU*/ bool FF06_D3;
  /*p03.SUPO*/ bool FF06_D4;
  /*p03.SOTU*/ bool FF06_D5;
  /*p03.REVA*/ bool FF06_D6;
  /*p03.SAPU*/ bool FF06_D7;

  //----------
  // TIMA clock mux

  /*p03.UVYR*/ bool UVYR;
  /*p03.UKAP*/ bool UKAP;
  /*p03.TECY*/ bool TECY;
  /*p03.UBOT*/ bool UBOT;
  /*p03.TEKO*/ bool TEKO;
  /*p03.SOGU*/ bool SOGU;

  // TIMA reload signal
  /*p03.MUZU*/ bool MUZU;
  /*p03.MEKE*/ bool MEKE;
  /*p03.MEXU*/ bool MEXU;

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

  /*p03.SOKU*/ bool FF05_D0;
  /*p03.RACY*/ bool FF05_D1;
  /*p03.RAVY*/ bool FF05_D2;
  /*p03.SOSY*/ bool FF05_D3;
  /*p03.SOMU*/ bool FF05_D4;
  /*p03.SURO*/ bool FF05_D5;
  /*p03.ROWU*/ bool FF05_D6;
  /*p03.PUSO*/ bool FF05_D7;

  //----------
  // INT_TIMER delay

  /*p03.MUGY*/ bool MUGY;
  /*p03.NYDU*/ bool NYDU;
  /*p03.MERY*/ bool MERY;
  /*p03.MOBA*/ bool INT_TIMER;
};

struct P04_DMA {

  /*p04.DECY*/ bool FROM_CPU5n;
  /*p04.CATY*/ bool FROM_CPU5;
  /*p04.MAKA*/ bool FROM_CPU5_SYNC;

  /*p04.WYJA*/ bool WYJA;
  /*p04.MOPA*/ bool CPUCLK_ABCDxxxx9;
  /*p04.LUMA*/ bool DO_DMA; // if true, addr bus = dma addr
  /*p04.DUGA*/ bool OAM_ADDR_DMA;

  /*p04.LUFA*/ bool VRAM_TO_OAM;
  /*p04.AHOC*/ bool VRAM_TO_OAMn;

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

  /*p04.MATU*/ bool MATU; // -> p25,p28

  /*p04.ECAL*/ bool MA00;
  /*p04.EGEZ*/ bool MA01;
  /*p04.FUHE*/ bool MA02;
  /*p04.FYZY*/ bool MA03;
  /*p04.DAMU*/ bool MA04;
  /*p04.DAVA*/ bool MA05;
  /*p04.ETEG*/ bool MA06;
  /*p04.EREW*/ bool MA07;
  /*p04.EVAX*/ bool MA08;
  /*p04.DUVE*/ bool MA09;
  /*p04.ERAF*/ bool MA10;
  /*p04.FUSY*/ bool MA11;
  /*p04.EXYF*/ bool MA12;

  /*p04.POLY*/ bool FF46_D0;
  /*p04.ROFO*/ bool FF46_D1;
  /*p04.REMA*/ bool FF46_D2;
  /*p04.PANE*/ bool FF46_D3;
  /*p04.PARE*/ bool FF46_D4;
  /*p04.RALY*/ bool FF46_D5;
  /*p04.RESU*/ bool FF46_D6;
  /*p04.NUVY*/ bool FF46_D7;

  /*p04.MOLU*/ bool FF46_RDn1;
  /*p04.NYGO*/ bool FF46_RD;
  /*p04.PUSY*/ bool FF46_RDn2;
  /*p04.LAVY*/ bool FF46_WRn;
  /*p04.LORU*/ bool FF46_WR;

  /*p04.META*/ bool DMA_CLK;
  /*p04.LOKY*/ bool DMA_RUNNING;
  /*p04.NAVO*/ bool DMA_DONE;
  /*p04.NOLO*/ bool DMA_DONEn;

  /*p04.LUVY*/ bool LUVY;
  /*p04.MYTE*/ bool MYTE;
  /*p04.LENE*/ bool LENE;

  /*p04.NAXY*/ bool NAXY;
  /*p04.POWU*/ bool POWU;
  /*p04.LUPA*/ bool LUPA;
  /*p04.LOKO*/ bool LOKO;
  /*p04.LAPA*/ bool LAPA;
  /*p04.LARA*/ bool LARA;
  /*p04.MORY*/ bool MORY;
  /*p04.LOGO*/ bool LOGO;
  /*p04.MUHO*/ bool MUHO;
  /*p04.MUDA*/ bool MUDA;
  /*p04.LEBU*/ bool DMA_A15n;
  /*p04.LYXE*/ bool LYXE;
};

struct P05_JoypadIO {

  // joypad int is in p02, so there's no outputs here

  // FF00 JOYP
  bool JUTE_00,KECY_01,JALE_02,KYME_03,KELY_04,COFY_05,KUKO_06,KERU_07;

  // FF00 JOYP read low bits
  bool KEVU_00,KAPA_01,KEJA_02,KOLO_03;
  bool KEMA_00,KURO_01,KUVE_02,JEKU_03,KOCE_04,CUDY_05;

  // some debug thing?
  bool ANOC_00,ATAJ_01,AJEC_02,ASUZ_03,BENU_04,AKAJ_05,ARAR_06,BEDA_07;

  bool BYZO,AXYN,ADYR,APYS,AFOP;
  bool KOLE,KYBU;
  bool KYTO,KABU;
  bool KORY,KALE;
  bool KURA,CELA,KARU;
  bool KYHU,KASY;

  bool KORE,JEVA,KYWE;
  bool KENA;
};

struct P06_SerialLink {

  union { /*bool SARE;*/ bool ADDR_XX00_XX07; };

  // counter that triggers INT_SERIAL
  union { /*bool CAFA;*/ bool SER_CNT0; }; 
  union { /*bool CYLO;*/ bool SER_CNT1; }; 
  union { /*bool CYDE;*/ bool SER_CNT2; }; 
  union { /*bool CALY;*/ bool SER_CNT3; bool INT_SERIAL; };
  union { /*bool COBA;*/ bool SER_CNT3n; };

  union { /*bool COTY;*/ bool SER_CLK; };
  union { /*bool CAVE;*/ bool SER_CLK_MUXn; };
  union { /*bool DAWA;*/ bool SER_TICK; };
  union { /*bool EDYL;*/ bool SER_TICKn; };
  union { /*bool EPYT;*/ bool SER_TICK2; };
  union { /*bool DEHO;*/ bool SER_TICKn2; };
  union { /*bool DAWE;*/ bool SER_TICK3; };

  union { /*bool ELYS;*/ bool SER_OUT; };

  // FF01 SB
  union { /*bool URYS;*/ bool FF01_WRn; };
  union { /*bool DAKU;*/ bool FF01_WR; };
  union { /*bool UFEG;*/ bool FF01_RD; };

  union { /*bool SEFY;*/ bool A02n; };

  union { /*bool SANO;*/ bool ADDR_FF00_FF03; };


  union { /*bool CAGE;*/ bool SIN_Cn; };

  union { /*bool CUFU;*/ bool SER_DATA0_SETn; };
  union { /*bool DOCU;*/ bool SER_DATA1_SETn; };
  union { /*bool DELA;*/ bool SER_DATA2_SETn; };
  union { /*bool DYGE;*/ bool SER_DATA3_SETn; };
  union { /*bool DOLA;*/ bool SER_DATA4_SETn; };
  union { /*bool ELOK;*/ bool SER_DATA5_SETn; };
  union { /*bool EDEL;*/ bool SER_DATA6_SETn; };
  union { /*bool EFEF;*/ bool SER_DATA7_SETn; };

  union { /*bool COHY;*/ bool SER_DATA0_RSTn; };
  union { /*bool DUMO;*/ bool SER_DATA1_RSTn; };
  union { /*bool DYBO;*/ bool SER_DATA2_RSTn; };
  union { /*bool DAJU;*/ bool SER_DATA3_RSTn; };
  union { /*bool DYLY;*/ bool SER_DATA4_RSTn; };
  union { /*bool EHUJ;*/ bool SER_DATA5_RSTn; };
  union { /*bool EFAK;*/ bool SER_DATA6_RSTn; };
  union { /*bool EGUV;*/ bool SER_DATA7_RSTn; };

  union { /*bool CUBA;*/ bool SER_DATA0; };
  union { /*bool DEGU;*/ bool SER_DATA1; };
  union { /*bool DYRA;*/ bool SER_DATA2; };
  union { /*bool DOJO;*/ bool SER_DATA3; };
  union { /*bool DOVU;*/ bool SER_DATA4; };
  union { /*bool EJAB;*/ bool SER_DATA5; };
  union { /*bool EROD;*/ bool SER_DATA6; };
  union { /*bool EDER;*/ bool SER_DATA7; };

  union { /*bool CUGY;*/ bool FF01_D0; };
  union { /*bool DUDE;*/ bool FF01_D1; };
  union { /*bool DETU;*/ bool FF01_D2; };
  union { /*bool DASO;*/ bool FF01_D3; };
  union { /*bool DAME;*/ bool FF01_D4; };
  union { /*bool EVOK;*/ bool FF01_D5; };
  union { /*bool EFAB;*/ bool FF01_D6; };
  union { /*bool ETAK;*/ bool FF01_D7; };

  // FF02 SC
  union { /*bool CULY;*/ bool SCK_B; };
  union { /*bool ETAF;*/ bool XFER_START; };
  union { /*bool CORE;*/ bool FF02_D0; };
  union { /*bool ELUV;*/ bool FF02_D7; };

  union { /*bool UWAM;*/ bool FF02_WRn; };
  union { /*bool UCOM;*/ bool FF02_RD; };

  union { /*bool KEXU;*/ bool SCK_A; };
  union { /*bool JAGO;*/ bool SCK_Bn; };
  union { /*bool KUJO;*/ bool SCK_D; };

  union { /*bool CARO;*/ bool SER_RST; };
  union { /*bool CABY;*/ bool XFER_RESET; };
};

struct P07_SysDecode {

  //----------
  // outputs

  /*p07.UBET*/ bool T1n;
  /*p07.UVAR*/ bool T2n;
  /*p07.UPOJ*/ bool MODE_PROD;
  /*p07.UMUT*/ bool MODE_DBG1;
  /*p07.YAZA*/ bool MODE_DBG1n;
  /*p07.UNOR*/ bool MODE_DBG2;
  /*p07.APET*/ bool MODE_DEBUG;

  /*p07.LEXY*/ bool PIN_NC;

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

  /*p07.TEDO*/ bool CPU_RD;
  /*p07.TAPU*/ bool CPU_WR;
  /*p07.ASOT*/ bool CPU_RD2;
  /*p07.CUPA*/ bool CPU_WR2;
  /*p07.AJAS*/ bool CPU_RDn;
  /*p07.UBAL*/ bool CPU_WR_MUX;
  /*p07.UJYV*/ bool CPU_RD_MUX;
  /*p07.DYKY*/ bool CPU_WRn;

  /*p07.TUTU*/ bool ADDR_BOOT;
  /*p07.TEXE*/ bool BOOT_BIT_RD;
  /*p07.TUGE*/ bool BOOT_BIT_WRn;
  /*p07.TEPU*/ bool BOOT_BIT;
  /*p07.SATO*/ bool BOOT_BIT2;
  /*p07.SYPU*/ bool BOOT_BIT_D0;
  /*p07.TERA*/ bool BOOT_EN;
  /*p07.YULA*/ bool BOOT_RD;
  /*p07.ZERY*/ bool BOOT_CS;
  /*p07.ZADO*/ bool BOOT_CSn;

  /*p07.WALE*/ bool ADDR_xxxxxxxxx1111111n;
  /*p07.WOLY*/ bool HRAM_CSn;
  /*p07.WUTA*/ bool HRAM_CS;

  /*p07.ROLO*/ bool FF0F_RD;
  /*p07.REFA*/ bool FF0F_WR;

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
  /*p07.SEMY*/ bool ADDR_XX0X;
  /*p07.SAPA*/ bool ADDR_XXXF;
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


  /*p07.TYRO*/ bool ADDR_xxxxxxxx0x0x0000;
  /*p07.TUFA*/ bool ADDR_xxxxxxxxx1x1xxxx;

  /*p07.ZOLE*/ bool ADDR_xxxxxxxxxxxxxx00;
  /*p07.ZAJE*/ bool ADDR_xxxxxxxxxxxxxx01;
  /*p07.ZUBU*/ bool ADDR_xxxxxxxxxxxxxx10;
  /*p07.ZAPY*/ bool ADDR_xxxxxxxxxxxxxx11;

};


struct P08_ExtCpuBuses {


  union { /*bool TOLA;*/ bool A1n; };
  union { /*bool TOVA;*/ bool MODE_DBG2n1; bool NET01; };
  union { /*bool RYCA;*/ bool MODE_DBG2n2; };

  // True if the internal data bus is driving the external data bus
  union { /*bool LULA;*/ bool DBUS_OUT; }; 
  union { /*bool RORU;*/ bool DBUS_OUTn; };

  bool TEXO;

  bool SOBY_15;

  // Address latch
  union { /*bool ALOR;*/ bool ADDR_LATCH_00; };
  union { /*bool APUR;*/ bool ADDR_LATCH_01; };
  union { /*bool ALYR;*/ bool ADDR_LATCH_02; };
  union { /*bool ARET;*/ bool ADDR_LATCH_03; };
  union { /*bool AVYS;*/ bool ADDR_LATCH_04; };
  union { /*bool ATEV;*/ bool ADDR_LATCH_05; };
  union { /*bool AROS;*/ bool ADDR_LATCH_06; };
  union { /*bool ARYM;*/ bool ADDR_LATCH_07; };
  union { /*bool LUNO;*/ bool ADDR_LATCH_08; };
  union { /*bool LYSA;*/ bool ADDR_LATCH_09; };
  union { /*bool PATE;*/ bool ADDR_LATCH_10; };
  union { /*bool LUMY;*/ bool ADDR_LATCH_11; };
  union { /*bool LOBU;*/ bool ADDR_LATCH_12; };
  union { /*bool LONU;*/ bool ADDR_LATCH_13; };
  union { /*bool NYRE;*/ bool ADDR_LATCH_14; };
  union { /*bool SEPY;*/ bool ADDR_LATCH_15; };

  // Addres mux
  union { /*bool AMET;*/ bool ADDR_MUX_00; };
  union { /*bool ATOL;*/ bool ADDR_MUX_01; };
  union { /*bool APOK;*/ bool ADDR_MUX_02; };
  union { /*bool AMER;*/ bool ADDR_MUX_03; };
  union { /*bool ATEM;*/ bool ADDR_MUX_04; };
  union { /*bool ATOV;*/ bool ADDR_MUX_05; };
  union { /*bool ATYR;*/ bool ADDR_MUX_06; };
  union { /*bool ASUR;*/ bool ADDR_MUX_07; };
  union { /*bool MANO;*/ bool ADDR_MUX_08; };
  union { /*bool MASU;*/ bool ADDR_MUX_09; };
  union { /*bool PAMY;*/ bool ADDR_MUX_10; };
  union { /*bool MALE;*/ bool ADDR_MUX_11; };
  union { /*bool MOJY;*/ bool ADDR_MUX_12; };
  union { /*bool MUCE;*/ bool ADDR_MUX_13; };
  union { /*bool PEGE;*/ bool ADDR_MUX_14; };
  union { /*bool TAZY;*/ bool ADDR_MUX_15; };


  // AXX_A
  union { /*bool KUPO;*/ bool PIN_A00_A; };
  union { /*bool CABA;*/ bool PIN_A01_A; };
  union { /*bool BOKU;*/ bool PIN_A02_A; };
  union { /*bool BOTY;*/ bool PIN_A03_A; };
  union { /*bool BYLA;*/ bool PIN_A04_A; };
  union { /*bool BADU;*/ bool PIN_A05_A; };
  union { /*bool CEPU;*/ bool PIN_A06_A; };
  union { /*bool DEFY;*/ bool PIN_A07_A; };
  union { /*bool MYNY;*/ bool PIN_A08_A; };
  union { /*bool MUNE;*/ bool PIN_A09_A; };
  union { /*bool ROXU;*/ bool PIN_A10_A; };
  union { /*bool LEPY;*/ bool PIN_A11_A; };
  union { /*bool LUCE;*/ bool PIN_A12_A; };
  union { /*bool LABE;*/ bool PIN_A13_A; };
  union { /*bool PUHE;*/ bool PIN_A14_A; };
  union { /*bool SUZE;*/ bool PIN_A15_A; };

  // AXX_D
  union { /*bool KOTY;*/ bool PIN_A00_D; };
  union { /*bool COTU;*/ bool PIN_A01_D; };
  union { /*bool BAJO;*/ bool PIN_A02_D; };
  union { /*bool BOLA;*/ bool PIN_A03_D; };
  union { /*bool BEVO;*/ bool PIN_A04_D; };
  union { /*bool AJAV;*/ bool PIN_A05_D; };
  union { /*bool CYKA;*/ bool PIN_A06_D; };
  union { /*bool COLO;*/ bool PIN_A07_D; };
  union { /*bool MEGO;*/ bool PIN_A08_D; };
  union { /*bool MENY;*/ bool PIN_A09_D; };
  union { /*bool RORE;*/ bool PIN_A10_D; };
  union { /*bool LYNY;*/ bool PIN_A11_D; };
  union { /*bool LOSO;*/ bool PIN_A12_D; };
  union { /*bool LEVA;*/ bool PIN_A13_D; };
  union { /*bool PAHY;*/ bool PIN_A14_D; };
  union { /*bool RULO;*/ bool PIN_A15_D; };

  // Data output driver
  union { /*bool RUXA_00;*/ bool D0_A; };
  union { /*bool RUJA_01;*/ bool D1_A; };
  union { /*bool RABY_02;*/ bool D2_A; };
  union { /*bool RERA_03;*/ bool D3_A; };
  union { /*bool RORY_04;*/ bool D4_A; };
  union { /*bool RYVO_05;*/ bool D5_A; };
  union { /*bool RAFY_06;*/ bool D6_A; };
  union { /*bool RAVU_07;*/ bool D7_A; };

  union { /*bool RUNE_00;*/ bool D0_D; };
  union { /*bool RYPU_01;*/ bool D1_D; };
  union { /*bool SULY_02;*/ bool D2_D; };
  union { /*bool SEZE_03;*/ bool D3_D; };
  union { /*bool RESY_04;*/ bool D4_D; };
  union { /*bool TAMU_05;*/ bool D5_D; };
  union { /*bool ROGY_06;*/ bool D6_D; };
  union { /*bool RYDA_07;*/ bool D7_D; };

  // DX_C
  union {
    // TOVO,RUZY,ROME,SAZA,TEHE,RATU,SOCA,RYBA
    struct
    {
      bool D0_Cn : 1;
      bool D1_Cn : 1;
      bool D2_Cn : 1;
      bool D3_Cn : 1;
      bool D4_Cn : 1;
      bool D5_Cn : 1;
      bool D6_Cn : 1;
      bool D7_Cn : 1;
    };
  };

  union {
    // TUTY,SYWA,SUGU,TAWO,TUTE,SAJO,TEMY,ROPA
    struct
    {
      bool D0_C : 1;
      bool D1_C : 1;
      bool D2_C : 1;
      bool D3_C : 1;
      bool D4_C : 1;
      bool D5_C : 1;
      bool D6_C : 1;
      bool D7_C : 1;
    };
  };


  union { /*bool SOMA;*/ bool LATCH_D0; };
  union { /*bool RONY;*/ bool LATCH_D1; };
  union { /*bool RAXY;*/ bool LATCH_D2; };
  union { /*bool SELO;*/ bool LATCH_D3; };
  union { /*bool SODY;*/ bool LATCH_D4; };
  union { /*bool SAGO;*/ bool LATCH_D5; };
  union { /*bool RUPA;*/ bool LATCH_D6; };
  union { /*bool SAZY;*/ bool LATCH_D7; };

  union { /*bool RYMA;*/ bool BUS_D0; };
  union { /*bool RUVO;*/ bool BUS_D1; };
  union { /*bool RYKO;*/ bool BUS_D2; };
  union { /*bool TAVO;*/ bool BUS_D3; };
  union { /*bool TEPE;*/ bool BUS_D4; };
  union { /*bool SAFO;*/ bool BUS_D5; };
  union { /*bool SEVU;*/ bool BUS_D6; };
  union { /*bool TAJU;*/ bool BUS_D7; };

  // Debug, drives external address onto internal address
  union { /*bool KOVA;*/ bool A00_Cn; };
  union { /*bool CAMU;*/ bool A01_Cn; };
  union { /*bool BUXU;*/ bool A02_Cn; };
  union { /*bool BASE;*/ bool A03_Cn; };
  union { /*bool AFEC;*/ bool A04_Cn; };
  union { /*bool ABUP;*/ bool A05_Cn; };
  union { /*bool CYGU;*/ bool A06_Cn; };
  union { /*bool COGO;*/ bool A07_Cn; };
  union { /*bool MUJY;*/ bool A08_Cn; };
  union { /*bool NENA;*/ bool A09_Cn; };
  union { /*bool SURA;*/ bool A10_Cn; };
  union { /*bool MADY;*/ bool A11_Cn; };
  union { /*bool LAHE;*/ bool A12_Cn; };
  union { /*bool LURA;*/ bool A13_Cn; };
  union { /*bool PEVO;*/ bool A14_Cn; };
  union { /*bool RAZA;*/ bool A15_Cn; };

  union { /*bool KEJO;*/ bool A00_C; };
  union { /*bool BYXE;*/ bool A01_C; };
  union { /*bool AKAN;*/ bool A02_C; };
  union { /*bool ANAR;*/ bool A03_C; };
  union { /*bool AZUV;*/ bool A04_C; };
  union { /*bool AJOV;*/ bool A05_C; };
  union { /*bool BYNE;*/ bool A06_C; };
  union { /*bool BYNA;*/ bool A07_C; };
  union { /*bool LOFA;*/ bool A08_C; };
  union { /*bool MAPU;*/ bool A09_C; };
  union { /*bool RALA;*/ bool A10_C; };
  union { /*bool LORA;*/ bool A11_C; };
  union { /*bool LYNA;*/ bool A12_C; };
  union { /*bool LEFY;*/ bool A13_C; };
  union { /*bool NEFE;*/ bool A14_C; };
  union { /*bool SYZU;*/ bool A15_C; };

  union { /*bool SORE;*/ bool ADDR_0000_7FFF; };
  union { /*bool TEVY;*/ bool ADDR_NOT_VRAM; };

  bool LEVO,LAGU,LYWE,MOCA,MEXO,NEVY,MOTY,PUVA,TYMU;
  bool LOXO,LASY,MATE;
  bool SOGY,TYNU,TOZA;

  union { bool TUMA; bool CART_RAM; };

  bool LAVO;
  bool LYRA;

  union { bool TYHO; bool CS_A; };
  union { bool UVER; bool WR_A; };
  union { bool USUF; bool WR_D; };
  union { bool UGAC; bool RD_A; };
  union { bool URUN; bool RD_D; };

  union { bool MULE; bool MODE_DBG1n; };

  union { /*bool REDU;*/ bool CPU_RDn; };
};

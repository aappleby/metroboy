#pragma once
#include "../Schematics.h"
#include "../SignalTree.h"

struct Gameboy;

//-----------------------------------------------------------------------------
// This file should contain the schematics as directly translated to C,
// no modifications or simplifications.

struct P01_ClocksReset {

  static void tick(const Gameboy& a, const Gameboy& b, Gameboy& c);
  static const std::vector<SignalData> signals();

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
  /*p01.BYFE*/ bool BYFE_128;

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
  /*APOV*/ bool CPU_WR_SYNC;  // ___d____

  /*p01.AGUT*/ bool AGUT; // abcd__gh
  /*p01.AWOD*/ bool CPU_RD_SYNCn;
  /*ABUZ*/ bool CPU_RD_SYNC;  // ____ef__

  //----------
  // cells

  /*p01.ULUR*/ bool ULUR;
  bool BALY;
  bool BYXO;

  
};

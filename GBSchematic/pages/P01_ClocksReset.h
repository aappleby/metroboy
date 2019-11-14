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

  bool UPYF;
  bool TUBO;
  bool UNUT;
  union { /*bool TABA;*/ bool CPU_RESET; };
  union { /*bool ALYP;*/ bool CPU_RESETn; };

  //----------
  // SYS reset tree

  bool AFAR; // x
  bool ASOL; // x
  /*AFER*/ bool RESET_REG;
  bool AVOR;

  /*ALUR*/ bool SYS_RESETn1;
  /*DULA*/ bool SYS_RESET1;
  /*HAPO*/ bool SYS_RESET2;
  /*CUNU*/ bool SYS_RESETn2;
  /*GUFO*/ bool SYS_RESETn3;
  /*XORE*/ bool SYS_RESET3;
  /*XEBE*/ bool SYS_RESETn4;
  /*WALU*/ bool SYS_RESETn5;
  /*WESY*/ bool SYS_RESETn6;
  /*XARE*/ bool SYS_RESETn7;

  //----------
  // VID reset tree

  /*XODO*/ bool VID_RESET1;
  /*XAPO*/ bool VID_RESETn1;
  /*LYHA*/ bool VID_RESET2;
  /*TOFU*/ bool VID_RESET3;
  /*PYRY*/ bool VID_RESET4;
  /*ROSY*/ bool VID_RESET5;
  /*ATAR*/ bool VID_RESET6;
  /*AMYG*/ bool VID_RESET7;
  /*LYFE*/ bool VID_RESETn2;
  /*ABEZ*/ bool VID_RESETn3;

  //----------
  // APU reset tree

  /*P09.JYRO*/ bool APU_RST_00;

  /*BOPO*/ bool APU_RESETn1;
  /*ATUS*/ bool APU_RESETn2;
  /*BELA*/ bool APU_RESETn3;

  //----------
  // Clock control

  /*ABOL*/ bool CPUCLK_REQn;
  /*BUTY*/ bool CPUCLK_REQ;
  /*UCOB*/ bool CLK_BAD1;
  /*ATEZ*/ bool CLK_BAD2;

  //----------
  // 4M clocks
  /*APUV*/ bool CLK_AxCxExGx1;
  /*ARYF*/ bool CLK_AxCxExGx2;
  /*ATAL*/ bool CLK_AxCxExGx3;
  /*ALET*/ bool CLK_AxCxExGx4;
  /*ARYS*/ bool CLK_AxCxExGx5;
  /*ANOS*/ bool CLK_AxCxExGx6;
  /*CYBO*/ bool CLK_AxCxExGx7;
  /*ATAG*/ bool CLK_AxCxExGx8;
  /*ZAXY*/ bool CLK_AxCxExGx9;
  /*TAVA*/ bool CLK_AxCxExGx10;
  /*AMUK*/ bool CLK_xBxDxFxH1;
  /*ZEME*/ bool CLK_xBxDxFxH2;
  /*AVET*/ bool CLK_xBxDxFxH3;
  /*AZOF*/ bool CLK_xBxDxFxH4;
  /*LAPE*/ bool CLK_xBxDxFxH5;

  // 2M clocks
  /*CERY*/ bool CLK_ABxxEFxx1; // ab__ef__

  // 1M clocks

  // 1mhz phase generator, regs in order of triggering.
  /*AFUR*/ bool CLK_ABCDxxxx1;
  /*ALEF*/ bool CLK_xBCDExxx1;
  /*APUK*/ bool CLK_xxCDEFxx1;
  /*ADYK*/ bool CLK_xxxDEFGx1;
  /*ATYP*/ bool CLK_xxxxEFGH1;
  /*AFEP*/ bool CLK_AxxxxFGH1;
  /*AROV*/ bool CLK_ABxxxxGH1;
  /*ADAR*/ bool CLK_ABCxxxxH1;
  /*AJAX*/ bool CLK_ABCDxxxx3;
  /*BUGO*/ bool CLK_xBCDExxx2;

  /*AFAS*/ bool CLK_xxxDxxxx1;
  /*BUTO*/ bool CLK_ABCDExxx3;
  /*BELE*/ bool CLK_xxxxxFGH1;

  //----------
  // clocks for the cpu

  /*BAPY*/ bool CPUCLK_xxDExxxx1;
  /*BUFA*/ bool CPUCLK_xxDExxxx2;
  /*BYLY*/ bool CPUCLK_xxDExxxx3;
  /*BERU*/ bool CPUCLK_ABxxEFGH1;
  /*BOLO*/ bool CPUCLK_ABxxEFGH2;
  /*BYDA*/ bool CPUCLK_ABxxEFGH3;
  /*NULE*/ bool CPUCLK_ABCDxxxx1;
  /*BEJA*/ bool CPUCLK_ABCDxxxx4;
  /*BELO*/ bool CPUCLK_ABCDxxxx5;
  /*BYRY*/ bool CPUCLK_xxxxEFGH1;
  /*BAVY*/ bool CPUCLK_xxxxEFGH3;
  /*BANE*/ bool CPUCLK_xxxxEFGH4;
  /*BAZE*/ bool CPUCLK_xxxxEFGH5;
  /*BOWA*/ bool CPUCLK_ABCDExxx2; // -> PORTD_01
  /*BEDO*/ bool CPUCLK_xxxxxFGH2; // -> PORTD_02
  /*BEKO*/ bool CPUCLK_xxxxEFGH2; // -> PORTD_03
  /*BUDE*/ bool CPUCLK_ABCDxxxx2; // -> PORTD_04
  /*BUKE*/ bool CPUCLK_xxxxxFxx1; // -> PORTD_06
  /*BATE*/ bool CPUCLK_xxxxxFxx2;
  /*BASU*/ bool CPUCLK_xxxxxFxxn;

  //----------
  // clocks for the cart

  /*BEVA*/ union { bool CPUCLK_ABCDxxxx3; bool PHI_OUT; };
  /*UVYT*/ union { bool CLK_xxxxEFGH3;    bool PHI_OUTn; };
  /*DOVA*/ union { bool CLK_xxxxEFGH2;    bool PHIn; };

  /*BOMA*/ bool RESET_CLK; // _____fgh -> PORTD_07

  bool BYJU; // abcde___
  bool BUVU; // _____fgh

  bool AVOK;

  //----------
  // clocks for the apu

  /*P09.AJER*/ bool AJER_2M;
  /*P09.DYFA*/ bool DYFA_1M;

  /*BAVU*/ bool BAVU_1M;
  /*HAMA*/ bool APUCLK_512Kn;
  /*JESO*/ bool JESO_512K;
  /*UVYN*/ bool CLK_16K;
  /*HORU*/ bool HORU_512;
  /*BUFY*/ bool BUFY_256;
  /*BYFE*/ bool BYFE_128;

  union { bool COKE; }; // ? clock
  bool BURE,FYNE,CULO,APEF,GALE,BEZE,BULE,GEXY,COFU,BARU;
  bool BARA,CARU,BYLU;
  bool ATYK;

  //----------
  // DIV

  /*BOGA*/ bool DIV_CLK;  // ABCDExxx -> PORTD_08
  /*UFOL*/ bool DIV_RSTn;
  /*TAGY*/ bool DIV_RD;
  /*TAPE*/ bool DIV_WR;

  /*UKUP*/ bool DIV_00;
  /*UFOR*/ bool DIV_01;
  /*UNER*/ bool DIV_02;
  /*TERO*/ bool DIV_03;
  /*UNYK*/ bool DIV_04;
  /*TAMA*/ bool DIV_05;
  /*UGOT*/ bool DIV_06;
  /*TULU*/ bool DIV_07;
  /*TUGO*/ bool DIV_08;
  /*TOFE*/ bool DIV_09;
  /*TERU*/ bool DIV_10;
  /*SOLA*/ bool DIV_11;
  /*SUBU*/ bool DIV_12;
  /*TEKA*/ bool DIV_13;
  /*UKET*/ bool DIV_14;
  /*UPOF*/ bool DIV_15;

  /*UMEK*/ bool DIV_06n;
  /*UREK*/ bool DIV_07n;
  /*UTOK*/ bool DIV_08n;
  /*SAPY*/ bool DIV_09n;
  /*UMER*/ bool DIV_10n;
  /*RAVE*/ bool DIV_11n;
  /*RYSO*/ bool DIV_12n;
  /*UDOR*/ bool DIV_13n;

  /*TAWU*/ bool DIV_D0;
  /*TAKU*/ bool DIV_D1;
  /*TEMU*/ bool DIV_D2;
  /*TUSE*/ bool DIV_D3;
  /*UPUG*/ bool DIV_D4;
  /*SEPU*/ bool DIV_D5;
  /*SAWA*/ bool DIV_D6;
  /*TATU*/ bool DIV_D7;

  //----------
  // Gated rd/wr signals

  /*AREV*/ bool CPU_WR_SYNCn;
  /*APOV*/ bool CPU_WR_SYNC;  // ___d____

  bool AGUT; // abcd__gh
  /*AWOD*/ bool CPU_RD_SYNCn;
  /*ABUZ*/ bool CPU_RD_SYNC;  // ____ef__

  //----------
  // cells

  bool ULUR;
  bool BALY;
  bool BYXO;

  
};

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

  // Clock control
  union { /*bool ABOL;*/ bool CPUCLK_REQn; };
  union { /*bool BUTY;*/ bool CPUCLK_REQ;  };
  union { /*bool UCOB;*/ bool CLK_BAD1; bool CLKIN_An1; };
  union { /*bool ATEZ;*/ bool CLK_BAD2; bool CLKIN_An2; };

  // 4M clocks
  union { /*bool APUV;*/ bool CLK_AxCxExGx1; }; // a_c_e_g_
  union { /*bool ARYF;*/ bool CLK_AxCxExGx2; }; // a_c_e_g_
  union { /*bool ATAL;*/ bool CLK_AxCxExGx3; }; // a_c_e_g_
  union { /*bool ALET;*/ bool CLK_AxCxExGx4; }; // a_c_e_g_
  union { /*bool ARYS;*/ bool CLK_AxCxExGx5; }; // a_c_e_g_
  union { /*bool ANOS;*/ bool CLK_AxCxExGx6; }; // a_c_e_g_
  union { /*bool CYBO;*/ bool CLK_AxCxExGx7; }; // a_c_e_g_
  union { /*bool ATAG;*/ bool CLK_AxCxExGx8; }; // a_c_e_g_
  union { /*bool ZAXY;*/ bool CLK_AxCxExGx9; }; // a_c_e_g_
  union { /*bool TAVA;*/ bool CLK_AxCxExGx10;}; // a_c_e_g_

  union { /*bool AMUK;*/ bool CLK_xBxDxFxH1; }; // _b_d_f_h
  union { /*bool ZEME;*/ bool CLK_xBxDxFxH2; }; // _b_d_f_h
  union { /*bool AVET;*/ bool CLK_xBxDxFxH3; }; // _b_d_f_h
  union { /*bool AZOF;*/ bool CLK_xBxDxFxH4; }; // _b_d_f_h
  union { /*bool LAPE;*/ bool CLK_xBxDxFxH5; }; // _b_d_f_h

  // 2M clocks
  union { /*bool CERY;*/ bool CLK_ABxxEFxx1; }; // ab__ef__

  // 1M clocks

  // 1mhz phase generator, regs in order of triggering.
  union { /*bool AFUR;*/ bool CLK_ABCDxxxx1; }; // abcd____
  union { /*bool ALEF;*/ bool CLK_xBCDExxx1; }; // _bcde___
  union { /*bool APUK;*/ bool CLK_xxCDEFxx1; }; // __cdef__
  union { /*bool ADYK;*/ bool CLK_xxxDEFGx1; }; // ___defg_
  union { /*bool ATYP;*/ bool CLK_xxxxEFGH1; }; // ____efgh
  union { /*bool AFEP;*/ bool CLK_AxxxxFGH1; }; // a____fgh
  union { /*bool AROV;*/ bool CLK_ABxxxxGH1; }; // ab____gh
  union { /*bool ADAR;*/ bool CLK_ABCxxxxH1; }; // abc____h
  union { bool AJAX; bool CLK_ABCDxxxx3; };
  union { bool BUGO; bool CLK_xBCDExxx2; };

  //----------
  // clocks for the cpu

  union { /*bool BAPY;*/ bool CPUCLK_xxDExxxx1; };
  union { /*bool BUFA;*/ bool CPUCLK_xxDExxxx2; };
  union { /*bool BYLY;*/ bool CPUCLK_xxDExxxx3; };

  union { /*bool BERU;*/ bool CPUCLK_ABxxEFGH1; };
  union { /*bool BOLO;*/ bool CPUCLK_ABxxEFGH2; };
  union { /*bool BYDA;*/ bool CPUCLK_ABxxEFGH3; };

  union { /*bool NULE;*/ bool CPUCLK_ABCDxxxx1; };
  union { /*bool BEVA;*/ bool CPUCLK_ABCDxxxx3; bool PHI_OUT; };
  union { /*bool BEJA;*/ bool CPUCLK_ABCDxxxx4; };
  union { /*bool BELO;*/ bool CPUCLK_ABCDxxxx5; };

  union { /*bool BYRY;*/ bool CPUCLK_xxxxEFGH1; };
  union { /*bool BAVY;*/ bool CPUCLK_xxxxEFGH3; };
  union { /*bool BANE;*/ bool CPUCLK_xxxxEFGH4; };
  union { /*bool BAZE;*/ bool CPUCLK_xxxxEFGH5; };

  union { /*bool UVYT;*/ bool CLK_xxxxEFGH3; bool PHI_OUTn; };      // ____efgh
  union { /*bool DOVA;*/ bool CLK_xxxxEFGH2; bool PHIn; };          // ____efgh

  union { /*bool BOWA;*/ bool CPUCLK_ABCDExxx2; }; // abcde___ -> PORTD_01
  union { /*bool BEDO;*/ bool CPUCLK_xxxxxFGH2; }; // _____fgh -> PORTD_02
  union { /*bool BEKO;*/ bool CPUCLK_xxxxEFGH2; };  // ____efgh -> PORTD_03
  union { /*bool BUDE;*/ bool CPUCLK_ABCDxxxx2; };  // -> PORTD_04
  union { /*bool BUKE;*/ bool CPUCLK_xxxxxFxx; };  // _____f__ -> PORTD_06
  union { /*bool BOGA;*/ bool CLK_ABCDExxx1; }; // abcde___ -> PORTD_08

  union { /*bool BOMA;*/ bool RESET_CLK; }; // _____fgh -> PORTD_07

  union { bool BUTO; bool CLK_ABCDExxx3; };
  union { bool BELE; bool CLK_xxxxxFGH1; };

  bool BYJU; // abcde___
  bool BUVU; // _____fgh
  bool AGUT; // abcd__gh

  bool AVOK;

  //----------
  // clocks for the apu

  union { bool BAVU; bool BAVU_1M; };
  union { bool HAMA; bool HAMA_512Kn; };
  union { bool JESO; bool JESO_512K; };
  union { /*bool UVYN;*/ bool CLK_16K; };
  union { /*bool HORU;*/ bool HORU_512; };
  union { /*bool BUFY;*/ bool BUFY_256; };
  union { /*bool BYFE;*/ bool BYFE_128; };
  union { bool COKE; }; // ? clock
  bool BURE,FYNE,CULO,APEF,GALE,BEZE,BULE,GEXY,COFU,BARU;
  bool BARA,CARU,BYLU;
  bool ATYK;

  //----------
  // DIV

  union { /*bool UFOL;*/ bool DIV_RSTn; };
  union { /*bool TAGY;*/ bool DIV_RD; };
  union { /*bool TAPE;*/ bool DIV_WR; };

  union { /*bool UKUP;*/ bool DIV_00; };
  union { /*bool UFOR;*/ bool DIV_01; bool CLK_256K; };
  union { /*bool UNER;*/ bool DIV_02; };
  union { /*bool TERO;*/ bool DIV_03; bool CLK_64K; };
  union { /*bool UNYK;*/ bool DIV_04; };
  union { /*bool TAMA;*/ bool DIV_05; bool TAMA_16Kn; };
  union { /*bool UGOT;*/ bool DIV_06; };
  union { /*bool TULU;*/ bool DIV_07; };
  union { /*bool TUGO;*/ bool DIV_08; };
  union { /*bool TOFE;*/ bool DIV_09; };
  union { /*bool TERU;*/ bool DIV_10; };
  union { /*bool SOLA;*/ bool DIV_11; };
  union { /*bool SUBU;*/ bool DIV_12; };
  union { /*bool TEKA;*/ bool DIV_13; };
  union { /*bool UKET;*/ bool DIV_14; };
  union { /*bool UPOF;*/ bool DIV_15; };

  union { /*bool UMEK;*/ bool DIV_06n; };
  union { /*bool UREK;*/ bool DIV_07n; };
  union { /*bool UTOK;*/ bool DIV_08n; };
  union { /*bool SAPY;*/ bool DIV_09n; };
  union { /*bool UMER;*/ bool DIV_10n; };
  union { /*bool RAVE;*/ bool DIV_11n; };
  union { /*bool RYSO;*/ bool DIV_12n; };
  union { /*bool UDOR;*/ bool DIV_13n; };

  union { /*bool TAWU;*/ bool DIV_D0; };
  union { /*bool TAKU;*/ bool DIV_D1; };
  union { /*bool TEMU;*/ bool DIV_D2; };
  union { /*bool TUSE;*/ bool DIV_D3; };
  union { /*bool UPUG;*/ bool DIV_D4; };
  union { /*bool SEPU;*/ bool DIV_D5; };
  union { /*bool SAWA;*/ bool DIV_D6; };
  union { /*bool TATU;*/ bool DIV_D7; };

  //----------
  // Gated rd/wr signals

  union { bool AREV; bool CPU_WR_SYNCn; };
  union { bool AWOD; bool CPU_RD_SYNCn; };
  union { /*bool ABUZ;*/ bool CPU_RD_SYNC; }; // ____ef__ -> CPU_RD_SYNC
  union { /*bool APOV;*/ bool CPU_WR_SYNC; }; // ___d____ -> CPU_WR_SYNC

  //----------
  // Resets

  union { /*bool ALUR;*/ bool SYS_RESETn1; };

  union { /*bool DULA;*/ bool SYS_RESET1; };
  union { /*bool HAPO;*/ bool SYS_RESET2; };

  union { /*bool CUNU;*/ bool SYS_RESETn2; };
  union { /*bool GUFO;*/ bool SYS_RESETn3; };

  union { /*bool XORE;*/ bool SYS_RESET3; };

  union { /*bool XEBE;*/ bool SYS_RESETn4; };
  union { /*bool WALU;*/ bool SYS_RESETn5; };
  union { /*bool WESY;*/ bool SYS_RESETn6; };
  union { /*bool XARE;*/ bool SYS_RESETn7; };

  union { /*bool XODO;*/ bool VID_RESET1; };
  union { /*bool XAPO;*/ bool VID_RESETn1; };

  bool LYHA;
  union { bool TOFU; };
  union { bool PYRY; };
  bool ROSY;
  union { bool ATAR; bool RESET_VIDEO2n; };
  bool AMYG;
  bool LYFE;

  //----------

  union { bool AFAS; bool CLK_xxxDxxxx1; }; // ___d____

  union { bool TABA; bool CPU_RESET; };
  union { bool ALYP; bool CPU_RESETn; };

  bool AFAR; // x
  bool ASOL; // x

  union { bool AFER; bool RESET_REG; };

  union { bool BOPO; bool APU_RESETn1; };
  union { bool ATUS; bool APU_RESETn2; };
  union { bool BELA; bool APU_RESETn3; };

  //----------
  // cells

  bool ULUR;
  union { /*bool BATE;*/ bool CPUCLK_xxxxxFxx2; };
  union { /*bool BASU;*/ bool CPUCLK_xxxxxFxxn; };

  bool BALY;
  bool AVOR;
  bool BYXO;
  bool UPYF,TUBO,UNUT;

  
};

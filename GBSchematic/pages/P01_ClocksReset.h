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

  // 4M clocks
  union { bool APUV; bool CLK_AxCxExGx1; }; // a_c_e_g_
  union { bool ARYF; bool CLK_AxCxExGx2; }; // a_c_e_g_
  union { bool ATAL; bool CLK_AxCxExGx3; }; // a_c_e_g_
  union { bool ALET; bool CLK_AxCxExGx4; }; // a_c_e_g_
  union { bool ARYS; bool CLK_AxCxExGx5; }; // a_c_e_g_
  union { bool ANOS; bool CLK_AxCxExGx6; }; // a_c_e_g_
  union { bool CYBO; bool CLK_AxCxExGx7; }; // a_c_e_g_
  union { bool ATAG; bool CLK_AxCxExGx8; }; // a_c_e_g_
  union { bool ZAXY; bool CLK_AxCxExGx9; }; // a_c_e_g_

  union { bool AMUK; bool CLK_xBxDxFxH1; }; // _b_d_f_h
  union { bool ZEME; bool CLK_xBxDxFxH2; }; // _b_d_f_h
  union { bool AVET; bool CLK_xBxDxFxH3; }; // _b_d_f_h
  union { bool AZOF; bool CLK_xBxDxFxH4; }; // _b_d_f_h

  // 2M clocks
  union { bool CERY; bool CLK_ABxxEFxx1; }; // ab__ef__

  // 1M clocks

  // 1mhz phase generator, regs in order of triggering.
  union { bool AFUR; bool CLK_ABCDxxxx1; }; // abcd____
  union { bool ALEF; bool CLK_xBCDExxx1; }; // _bcde___
  union { bool APUK; bool CLK_xxCDEFxx1; }; // __cdef__
  union { bool ADYK; bool CLK_xxxDEFGx1; }; // ___defg_
  union { bool ATYP; bool CLK_xxxxEFGH1; }; // ____efgh
  union { bool AFEP; bool CLK_AxxxxFGH1; }; // a____fgh
  union { bool AROV; bool CLK_ABxxxxGH1; }; // ab____gh
  union { bool ADAR; bool CLK_ABCxxxxH1; }; // abc____h

  union { bool BEVA; bool PHI_OUT; };       // abcd____
  union { bool UVYT; bool PHI_OUTn; };      // ____efgh
  union { bool DOVA; bool PHIn; };          // ____efgh
  union { bool BEDO; bool CLK_xxxxxFGH2; }; // _____fgh -> PORTD_02
  union { bool BOWA; bool CLK_ABCDExxx2; }; // abcde___ -> PORTD_01
  union { bool BOGA; bool CLK_ABCDExxx1; }; // abcde___ -> PORTD_08
  union { bool BAVU; bool BAVU_1M; };
  union { bool BEKO; bool CLK_xxxxEFGH; };  // ____efgh -> PORTD_03
  union { bool BUDE; bool CLK_ABCDxxxx2; };  // abcd____ -> PORTD_04
  union { bool BUKE; bool CLK_xxxxxFxx; };  // _____f__ -> PORTD_06
  union { bool BOMA; bool CLK_xxxxxFGH1; }; // _____fgh -> PORTD_07

  bool AREV; // abc_efgh
  bool BERU; // ab__efgh
  bool BYRY; // ____efgh
  bool BEJA; // abcd____
  bool BUTO; // abcdef__
  bool BYJU; // abcde___
  bool BUVU; // _____fgh
  bool AGUT; // abcd__gh
  bool AWOD; // abcd__gh

  bool AVOK;

  // Other clocks

  union { bool HAMA;    bool HAMA_512Kn; };
  union { bool JESO;    bool JESO_512K; };
  union { bool UFOR_01; bool CLK_256K; };
  union { bool TERO_03; bool CLK_64K; };
  union { bool TAMA_05; bool TAMA_16Kn; };
  union { bool UVYN;    bool CLK_16K; };

  // Slow clocks for APU sequencer
  union { bool HORU;    bool HORU_512; };
  union { bool BUFY;    bool BUFY_256; };
  union { bool BYFE;    bool BYFE_128; };

  // Gated rd/wr signals
  union { bool ABUZ; bool CPU_RD_SYNC; }; // ____ef__ -> CPU_RD_SYNC
  union { bool APOV; bool CPU_WR_SYNC; }; // ___d____ -> CPU_WR_SYNC

  union { bool UCOB; bool CLKIN_An; };

  // Resets
  union { bool ALUR; bool RESET2; };
  union { bool CUNU; bool RESET6; };
  union { bool XORE; bool RESET7; };
  union { bool XEBE; bool RESET7n; };
  union { bool WALU; bool RESET8; };
  union { bool WESY; bool RESET9; };
  union { bool XAPO; bool RESET_VIDEO; };

  // Div register refs, these should be named like the clocks
  union { bool UMEK; bool FF04_D0n; }; // -> p24.umob
  union { bool UREK; bool FF04_D1n; }; // -> p03.teko, p24.usec

  union { bool AFAS; }; // ___d____

  union { bool LAPE; }; // _b_d_f_h
  union { bool TAVA; }; // a_c_e_g_
  union { bool COKE; }; // ? clock

  union { bool ABOL; };

private:

  //----------
  // registers

  // div
  bool UKUP_00,UNER_02,UNYK_04;
  bool UGOT_06,TULU_07,TUGO_08,TOFE_09,TERU_10,SOLA_11,SUBU_12,TEKA_13,UKET_14,UPOF_15;

  // dividers for apu
  bool BARA,CARU,BYLU;
  bool ATYK;

  // reset reg
  bool AFER; 

  //----------
  // cells

  bool ALYP; // x
  bool AFAR; // x
  bool ASOL; // x

  bool ULUR;
  bool UTOK,SAPY,UMER,RAVE,RYSO,UDOR;
  bool TAGY;
  bool TAWU,TAKU,TEMU,TUSE,UPUG,SEPU,SAWA,TATU;
  bool AJAX;
  bool BUGO,BATE,BASU;
  bool BAPY,BUFA,BYLY,BOLO,BYDA;
  bool NULE,BAVY;
  bool BANE,BELO,BAZE,BELE,ATEZ,BALY,AVOR;
  bool BUTY,BYXO;
  bool TAPE,UFOL;
  bool DULA;
  bool XODO;
  bool BELA;
  bool UPYF,TUBO,UNUT,TABA;
  bool BURE,FYNE,CULO,APEF,GALE,BEZE,BULE,GEXY,COFU,BARU;
  
  union { bool BOPO; bool APU_RESETn1; };
  union { bool ATUS; bool APU_RESETn2; };

  bool RESET_DIVn;
};

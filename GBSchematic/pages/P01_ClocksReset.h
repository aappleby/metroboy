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

  union { bool UFOR_01; bool CLK_256K; };
  union { bool TERO_03; bool CLK_64K; };
  union { bool TAMA_05; bool TAMA_16Kn; };
  union { bool ALUR;    bool RESET2; };
  union { bool BEVA;    bool PHI_OUT; };
  union { bool UVYT;    bool PHI_OUTn; };    // ____efgh
  union { bool DOVA;    bool PHIn; };        // ____efgh
  union { bool BEDO; };                      // _____fgh -> PORTD_02
  union { bool BOWA;    bool TO_CPU; };      // abcde___ -> PORTD_01
  union { bool BOGA;    bool BOGA_1M; };     // abcde___ -> PORTD_08
  union { bool APOV;    bool CPU_WR_SYNC; }; // ___d____ -> CPU_WR_SYNC
  union { bool ABUZ; };                      // ____ef__ -> CPU_RD_SYNC?
  union { bool UVYN;    bool CLK_16K; };
  union { bool UMEK;    bool FF04_D0n; };
  union { bool UREK;    bool FF04_D1n; };
  union { bool BAVU;    bool BAVU_1M; };
  union { bool BUFY;    bool BUFY_256; };
  union { bool BYFE;    bool BYFE_128; };
  union { bool HAMA;    bool HAMA_512Kn; };
  union { bool HORU;    bool HORU_512; };
  union { bool JESO;    bool JESO_512K; };

  union { bool UCOB; bool CLKIN_An; };
  union { bool CUNU; bool RESET6; };
  union { bool XORE; bool RESET7; };
  union { bool XEBE; bool RESET7n; };
  union { bool WALU; bool RESET8; };
  union { bool WESY; bool RESET9; };
  union { bool XAPO; bool RESET_VIDEO; };

  union { bool AMUK; bool AMUK_4M; }; // _b_d_f_h
  union { bool APUV; bool APUV_4M; }; // a_c_e_g_
  union { bool ARYF; bool ARYF_4M; }; // a_c_e_g_
  union { bool ATAL; bool ATAL_4M; }; // a_c_e_g_
  union { bool ZEME; bool CLK1; };    // _b_d_f_h
  union { bool ALET; bool CLK2; };    // a_c_e_g_
  union { bool CERY; bool CERY_2M; }; // ab__ef__

  bool BEKO; // ____efgh -> PORTD_03
  bool BUDE; // abcd____ -> PORTD_04

  bool BUKE; // _____f__ -> PORTD_06
  bool BOMA; // _____fgh -> PORTD_07

private:

  //----------
  // registers

  // 1mhz phase generator, regs in order of triggering.
  bool AFUR; // abcd____
  bool ALEF; // _bcde___
  bool APUK; // __cdef__
  bool ADYK; // ___defg_
  bool ATYP; // ____efgh
  bool AFEP; // a____fgh
  bool AROV; // ab____gh
  bool ADAR; // abc____h

  // div
  bool UKUP_00,UNER_02,UNYK_04;
  bool UGOT_06,TULU_07,TUGO_08,TOFE_09,TERU_10,SOLA_11,SUBU_12,TEKA_13,UKET_14,UPOF_15;

  // dividers for apu
  bool BARA,CARU,BYLU;
  bool ATYK,AVOK;

  // reset reg
  bool AFER; 

  //----------
  // cells

  bool ARYS; // a_c_e_g_
  bool AVET; // _b_d_f_h
  bool ANOS; // a_c_e_g_
  bool AZOF; // _b_d_f_h
  bool CYBO; // a_c_e_g_
  bool ATAG; // a_c_e_g_
  bool ZAXY; // a_c_e_g_
  bool LAPE; // _b_d_f_h
  bool TAVA; // a_c_e_g_

  bool AFAS; // ___d____
  bool AREV; // abc_efgh

  bool BERU; // ab__efgh
  bool BYRY; // ____efgh
  bool BEJA; // abcd____
  bool BUTO; // abcdef__
  
  bool BYJU; // abcde___
  bool BUVU; // _____fgh

  bool AGUT; // abcd__gh
  bool AWOD; // abcd__gh

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
  bool ATUS,COKE,BURE,FYNE,CULO,APEF,GALE,BEZE,BULE,GEXY,COFU,BARU;
  bool BOPO;
  bool RESET_DIVn;
};

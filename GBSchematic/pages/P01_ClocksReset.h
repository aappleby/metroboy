#pragma once
#include "../Schematics.h"
#include "../SignalTree.h"

struct Gameboy;

#pragma warning(disable:4201)

//-----------------------------------------------------------------------------
// This file should contain the schematics as directly translated to C,
// no modifications or simplifications.

struct P01_ClocksReset {

  static void tick(const Gameboy& a, const Gameboy& b, Gameboy& c);
  static const std::vector<SignalData> signals();

  //----------
  // inputs

  //----------
  // outputs

  bool BOWA; // abcde___ -> PORTD_01
  bool BEDO; // _____fgh -> PORTD_02

  bool BEKO; // ____efgh -> PORTD_03
  bool BUDE; // abcd____ -> PORTD_04

  bool BUKE; // _____f__ -> PORTD_06
  bool BOMA; // _____fgh -> PORTD_07
  bool BOGA; // abcde___ -> PORTD_08

  bool APOV; // ___d____ -> CPU_WR_SYNC
  bool ABUZ; // ____ef__ -> CPU_RD_SYNC?

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

  /*
  union {
    uint8_t phasegen;
    struct {
      bool AFUR : 1; // abcd____
      bool ALEF : 1; // _bcde___
      bool APUK : 1; // __cdef__
      bool ADYK : 1; // ___defg_
      bool ATYP : 1; // ____efgh
      bool AFEP : 1; // a____fgh
      bool AROV : 1; // ab____gh
      bool ADAR : 1; // abc____h
    };
  };
  */

  // div
  bool UKUP_00,UFOR_01,UNER_02,TERO_03,UNYK_04,TAMA_05;
  bool UGOT_06,TULU_07,TUGO_08,TOFE_09,TERU_10,SOLA_11,SUBU_12,TEKA_13,UKET_14,UPOF_15;

  // dividers for apu
  bool BARA,CARU,BYLU;
  bool ATYK,AVOK,JESO;

  // reset reg
  bool AFER; 

  //----------
  // cells

  bool ARYS; // a_c_e_g_
  bool AVET; // _b_d_f_h
  bool ANOS; // a_c_e_g_
  bool ATAL; // a_c_e_g_
  bool AZOF; // _b_d_f_h
  bool CYBO; // a_c_e_g_
  bool ATAG; // a_c_e_g_
  bool ZAXY; // a_c_e_g_
  bool AMUK; // _b_d_f_h
  bool ZEME; // _b_d_f_h
  bool APUV; // a_c_e_g_
  bool ARYF; // a_c_e_g_
  bool ALET; // a_c_e_g_
  bool LAPE; // _b_d_f_h
  bool TAVA; // a_c_e_g_

  bool CERY; // ab__ef__

  bool AFAS; // ___d____
  bool AREV; // abc_efgh

  bool BERU; // ab__efgh
  bool BYRY; // ____efgh
  bool BEJA; // abcd____
  bool BUTO; // abcdef__
  
  bool BYJU; // abcde___
  bool BUVU; // _____fgh
  bool UVYT; // ____efgh

  bool AGUT; // abcd__gh
  bool AWOD; // abcd__gh

  bool DOVA; // ____efgh

  bool UCOB; // x
  bool ALYP; // x
  bool AFAR; // x
  bool ASOL; // x

  bool UVYN, ULUR;
  bool UMEK,UREK,UTOK,SAPY,UMER,RAVE,RYSO,UDOR;
  bool TAGY;
  bool TAWU,TAKU,TEMU,TUSE,UPUG,SEPU,SAWA,TATU;
  bool AJAX;
  bool BUGO,BATE,BASU;
  bool BAPY,BUFA,BYLY,BOLO,BYDA;
  bool NULE,BEVA,BAVY;
  bool BANE,BELO,BAZE,BELE,ATEZ,BALY,AVOR,ALUR;
  bool BUTY,BYXO;
  bool TAPE,UFOL;
  bool DULA,CUNU,XORE,WESY,WALU;
  bool XEBE,XODO,XAPO;
  bool BELA;
  bool UPYF,TUBO,UNUT,TABA;
  bool ATUS,COKE,BURE,FYNE,CULO,APEF,GALE,BEZE,BULE,GEXY,COFU,BARU,HORU,BUFY,BYFE;
  bool BOPO,BAVU,HAMA;
  bool RESET_DIVn;
};

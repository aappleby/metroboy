#pragma once
#include "../Schematics.h"
#include "../SignalTree.h"

struct Gameboy;

//-----------------------------------------------------------------------------
// This file should contain the schematics as directly translated to C,
// no modifications or simplifications.

struct P01_ClocksReset {

  //----------
  // Internal named signals

  bool RESET_DIVn;

  //----------
  // Logic cells

  bool BAPY,BERU,BUFA,BYLY,BOLO,BYDA;
  bool NULE,BYRY,BUDE,BEVA,BEKO,BAVY,DOVA;
  bool BEJA,BANE,BELO,BAZE,BUTO,BELE,ATEZ,BYJU,BALY,BOGA,BOMA,AFER,ALYP,AFAR,ASOL,AVOR,ALUR;
  bool BUTY,BUVU,BYXO,BEDO,BOWA;

  bool TAPE,UFOL;

  bool DULA,CUNU,XORE,WESY,WALU;
  bool XEBE,XODO,XAPO;
  bool UCOB;

  bool ARYS,ANOS,AVET,ATAL,AZOF,ATAG,ZAXY,AMUK,ZEME,APUV,ARYF,ALET;

  bool BELA,CERY;

  bool UVYT;

  bool UPYF,TUBO,UNUT,TABA;

  bool ATUS,COKE,BARA,CARU,BYLU,BURE,FYNE,CULO,APEF,GALE,BEZE,BULE,GEXY,COFU,BARU,HORU,BUFY,BYFE;

  bool BOPO,ATYK,AVOK,BAVU,JESO,HAMA;

  //----------
  // Registers

  // 1mhz phase generator, regs in order of triggering.

  bool AFUR; // phases ABCD
  bool ALEF; // phases BCDE
  bool APUK; // phases CDEF
  bool ADYK; // phases DEFG
  bool ATYP,AJAX,AGUT,AWOD,ABUZ;
  bool AFEP,BUGO,BATE,BASU,BUKE;
  bool AROV;
  bool ADAR,AFAS,AREV,APOV;

  // div
  bool UKUP_00,UFOR_01,UNER_02,TERO_03,UNYK_04,TAMA_05;
  bool UVYN, ULUR;
  bool UGOT_06,TULU_07,TUGO_08,TOFE_09,TERU_10,SOLA_11,SUBU_12,TEKA_13,UKET_14,UPOF_15;
  bool UMEK,UREK,UTOK,SAPY,UMER,RAVE,RYSO,UDOR;
  bool TAGY;
  bool TAWU,TAKU,TEMU,TUSE,UPUG,SEPU,SAWA,TATU;

  //----------

  static const std::vector<SignalData> signals();

  static void tick(const Gameboy& a, const Gameboy& b, Gameboy& c);
};

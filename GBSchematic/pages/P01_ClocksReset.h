#pragma once
#include "../Schematics.h"
#include "../SignalTree.h"
#include "CpuSignals.h"
#include "ChipSignals.h"
#include "TristateBus.h"

//-----------------------------------------------------------------------------
// This file should contain the schematics as directly translated to C,
// no modifications or simplifications.

struct P01_ClocksReset {
  //----------
  // Inputs

  struct Input {
    bool ABOL;        // this is actually CPU_RESET or similar
    bool AJER_2M;
    bool APU_RESET5n;
    bool APU_RESET;
    bool FERO_Q;      // something debug-related
    bool FF04_FF07;
    bool FF40_D7;     // lcd on
    bool FF60_D1;     // debugging
    bool TOLA_A1n;
    bool TOVY_A0n;
    bool CYBO_4M;     // from P17
  };

  Input in;

  //----------
  // Outputs

  struct Output {
    // to cpu
    bool TO_CPU;

    // to off-chip
    bool PHI_OUT;
    bool PHI_OUTn;
    bool PHIn;

    bool AMUK_4M;    // phases ACEG
    bool APUV_4M;    // phases BDFH
    bool ARYF_4M;    // phases BDFH
    bool ATAL_4M;    // phases BDFH
    bool BAVU_1M;    // phases CDEF
    bool BOGA_1M;    // high on phases BCDEF
    bool BUFY_256;
    bool BYFE_128;
    bool CERY_2M;
    bool CLK1;
    bool CLK2;
    bool CLK_16K;
    bool CLK_256K;
    bool CLK_64K;
    bool CLKIN_An;
    bool HAMA_512Kn;
    bool HORU_512;
    bool JESO_512K;
    bool TAMA_16K;

    bool CPU_RD_SYNC; // actually CPU_WR_SYNC, __not__ read
    bool FF04_D0n;    // to P24
    bool FF04_D1n;    // to P03, P24

    bool RESET2;      // active low!
    bool RESET6;
    bool RESET7;
    bool RESET7n;
    bool RESET8;
    bool RESET9;
    bool RESET_VIDEO; // to P21, P24, P27, P28, P29

  };

  Output out;

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

  static void tick(const CpuSignals& cpu,
                   const ChipSignals& chip,
                   const P01_ClocksReset& a, const TristateBus& busA,
                   const P01_ClocksReset& b, const TristateBus& busB,
                   P01_ClocksReset& c,       TristateBus& busC);
};

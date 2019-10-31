#pragma once
#include "../Schematics.h"
#include "../SignalTree.h"

//-----------------------------------------------------------------------------
// This file should contain the schematics as directly translated to C,
// no modifications or simplifications.

struct P01_ClocksReset {
  //----------
  // Inputs

  bool ABOL;        // this is actually CPU_RESET or similar
  bool AJER_2M;
  bool APU_RESET5n;
  bool APU_RESET;
  bool CLKIN_A;     // this is actually CLOCK_GOOD or similar
  bool CLKIN_B;     // the master 4mhz clock
  bool CPU_RD;
  bool CPU_WR;
  bool FERO_Q;      // something debug-related
  bool FF04_FF07;
  bool FF40_D7;     // lcd on
  bool FF60_D1;     // debugging
  bool FROM_CPU3;   // probably CPU_RAW_WR
  bool FROM_CPU4;   // controls ABUZ, sets WR_A/WR_C/RD_A/RD_C to 0 if low
  bool RESET;       // active high!
  bool T1nT2;       // true for debugging
  bool T1nT2n;      // true for normal mode
  bool T1T2n;       // true for debugging
  bool TOLA_A1n;
  bool TOVY_A0n;
  bool CYBO_4M;     // from P17

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
  // not 100% sure of the phase of this thing relative to CLKIN_B...

  //bool AFUR; // phases ABCD
  //bool ALEF; // phases BCDE
  //bool APUK; // phases CDEF
  //bool ADYK; // phases DEFG
  bool AFUR,ATYP,AJAX,AGUT,AWOD,ABUZ;
  bool ALEF,AFEP,BUGO,BATE,BASU,BUKE;
  bool APUK,AROV;
  bool ADYK,ADAR,AFAS,AREV,APOV;


  // div
  bool UKUP,UFOR,UNER,TERO,UNYK,TAMA;
  bool UVYN, ULUR;
  bool UGOT,TULU,TUGO,TOFE,TERU,SOLA,SUBU,TEKA,UKET,UPOF;
  bool UMEK,UREK,UTOK,SAPY,UMER,RAVE,RYSO,UDOR;
  bool TAGY;
  bool TAWU,TAKU,TEMU,TUSE,UPUG,SEPU,SAWA,TATU;

  //----------
  // Outputs

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
  bool CPU_RD_SYNC; // actually CPU_WR_SYNC, __not__ read
  bool D0,D1,D2,D3,D4,D5,D6,D7;
  bool FF04_D0n;
  bool FF04_D1n;
  bool HAMA_512Kn;
  bool HORU_512;
  bool JESO_512K;
  bool PHI_OUT;
  bool PHI_OUTn;
  bool PHIn;
  bool RESET2;      // active low!
  bool RESET6;
  bool RESET7;
  bool RESET7n;
  bool RESET8;
  bool RESET9;
  bool RESET_DIVn;
  bool RESET_VIDEO;
  bool TAMA_16K;
  bool TO_CPU;

  //----------

  static const std::vector<SignalData> signals();

  static void tick(const P01_ClocksReset& a, const P01_ClocksReset& b, P01_ClocksReset& c);
};

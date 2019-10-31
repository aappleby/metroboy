#pragma once
#include "../Schematics.h"
#include "../SignalTree.h"

//#define KEEP_CELLS

//-----------------------------------------------------------------------------
// This file should contain the schematics as directly translated to C,
// no modifications or simplifications.

struct P01_ClocksReset {

  //----------
  // Inputs

  // physical pins
  bool in_CPU_RD;
  bool in_CPU_WR;
  bool in_RESET;       // active high!
  bool in_CLKIN_A;     // this is actually CLOCK_GOOD or similar
  bool in_CLKIN_B;     // the master 4mhz clock

  bool in_ABOL;        // this is actually CPU_RESET or similar
  
  // hacking this in here for testing
  //bool in_AJER_2MHZ;   // on apu sheet
  
  bool in_FROM_CPU3;   // probably CPU_RAW_WR
  bool in_FROM_CPU4;   // controls ABUZ, sets WR_A/WR_C/RD_A/RD_C to 0 if low
  bool in_APU_RESET;
  bool in_APU_RESET5n;
  bool in_FERO_Q;      // something debug-related
  bool in_FF04_FF07;
  bool in_TOLA_A1n;
  bool in_TOVY_A0n;
  bool in_T1T2n;       // true for debugging
  bool in_T1nT2;       // true for debugging
  bool in_T1nT2n;      // true for normal mode
  bool in_FF40_D7;     // lcd on
  bool in_FF60_D1;     // debugging

  //----------
  // Outputs

  bool out_CLKIN_An;
  bool out_CLK1;
  bool out_CLK2;
  bool out_ATAL_4M;    // phases BDFH
  bool out_AMUK_4M;    // phases ACEG
  bool out_ARYF_4M;    // phases BDFH
  bool out_APUV_4M;    // phases BDFH
  bool out_CYBO_4M;    // from P17
  bool out_AJER_2M;
  bool out_CERY_2M;
  bool out_BAVU_1M;    // phases CDEF
  bool out_DYFA_1M;
  bool out_BOGA_1M;    // high on phases BCDEF
  bool out_JESO_512K;
  bool out_HAMA_512Kn;
  bool out_CLK_256K;
  bool out_CLK_64K;
  bool out_CLK_16K;
  bool out_TAMA_16K;
  bool out_HORU_512;
  bool out_BUFY_256;
  bool out_BYFE_128;

  bool out_FF04_D0n;
  bool out_FF04_D1n;

  // cartridge clock
  bool out_PHIn;
  bool out_PHI_OUT;
  bool out_PHI_OUTn;

  bool out_RESET2;      // active low!
  bool out_RESET6;
  bool out_RESET7;
  bool out_RESET7n;
  bool out_RESET8;
  bool out_RESET9;
  bool out_RESET_VIDEO;
  bool out_RESET_DIVn;

  bool out_TO_CPU;
  bool out_CPU_RD_SYNC; // actually CPU_WR_SYNC, __not__ read
  bool out_BUKE;
  bool out_ABUZ;
  bool out_AFAS;
  bool out_UMER;


  bool out_D0,out_D1,out_D2,out_D3,out_D4,out_D5,out_D6,out_D7;

  //----------
  // Logic cells

#ifdef KEEP_CELLS

  bool ABUZ,ADAR,AFAR,AFAS,AFEP,AGUT,AJAX,ALET;
  bool ALUR,ALYP,AMUK,APEF,APOV,APUV,AREV,AROV;
  bool ARYF,ASOL,ATAG,ATAL,ATEZ,ATUS,ATYP,AVOR;
  bool AWOD,AZOF,BALY,BANE,BAPY,BARU,BASU,BATE;
  bool BAVY,BAZE,BEDO,BEJA,BEKO,BELE,BELO,BERU;
  bool BEVA,BEZE,BOLO,BOPO,BOWA,BUDE,BUFA,BUFY;
  bool BUGO,BUKE,BULE,BUTO,BUTY,BUVU,BYDA,BYFE;
  bool BYJU,BYLY,BYRY,BYXO,COFU,CULO,CUNU,DOVA;
  bool DULA,FYNE,GALE,GEXY,HAMA,HORU,NULE,RAVE;
  bool RYSO,SAPY,SAWA,SEPU,TABA,TAGY,TAKU,TAPE;
  bool TATU,TAWU,TEMU,TUBO,TUSE,UCOB,UDOR,UFOL;
  bool UMEK,UMER,UNUT,UPUG,UPYF,UREK,UTOK,UVYN;
  bool WALU,WESY,XAPO,XEBE,XODO,XORE,ZAXY,ZEME;
  bool ARYS,BELA,CYBO;

#endif

  //----------
  // Clock cells

  bool BAVU,COKE,BURE,BOGA,ULUR,BOMA;

  // from P09
  bool BATA,DYFA;

  // Cells needed for deglitcher
  bool ANOS,AVET;

  //----------
  // Registers

  // 1mhz phase generator, regs in order of triggering.
  // not 100% sure of the phase of this thing relative to CLKIN_B...

  bool AFUR; // phases ABCD
  bool ALEF; // phases BCDE
  bool APUK; // phases CDEF
  bool ADYK; // phases DEFG

  // register for the RESET2 signal, roughly
  bool AFER;

  // div
  bool UKUP,UFOR,UNER,TERO,UNYK,TAMA;
  bool UGOT,TULU,TUGO,TOFE,TERU,SOLA,SUBU,TEKA,UKET,UPOF;

  // clock divider for apu sequencer
  bool BARA,CARU,BYLU;

  // clock divider for something else...
  bool ATYK,AVOK,JESO;

  // clock regs from P09
  bool AJER,CALO;

  // moar clock dividers
  bool CERY;

  //----------

  static const std::vector<SignalData> signals();

  void tick(const P01_ClocksReset& prev);
};

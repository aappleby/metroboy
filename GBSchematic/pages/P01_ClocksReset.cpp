#include "P01_ClocksReset.h"

#ifdef KEEP_CELLS
#define cell
#else
#define cell bool
#endif

//-----------------------------------------------------------------------------
// This file should contain the schematics as directly translated to C,
// no modifications or simplifications.

const std::vector<SignalData> P01_ClocksReset::signals() {
  return
  {
    SignalData("CLKIN_B",     offsetof(P01_ClocksReset, in_CLKIN_B),    0, 1),
    SignalData("PHASE_A",     offsetof(P01_ClocksReset, AFUR),          0, 1),
    SignalData("PHASE_B",     offsetof(P01_ClocksReset, ALEF),          0, 1),
    SignalData("PHASE_C",     offsetof(P01_ClocksReset, APUK),          0, 1),
    SignalData("PHASE_D",     offsetof(P01_ClocksReset, ADYK),          0, 1),
    /*
    SignalData("AMUK_4M",     offsetof(P01_ClocksReset, out_AMUK_4M),   0, 1),
    SignalData("ATAL_4M",     offsetof(P01_ClocksReset, out_ATAL_4M),   0, 1),
    SignalData("ARYF_4M",     offsetof(P01_ClocksReset, out_ARYF_4M),   0, 1),
    SignalData("APUV_4M",     offsetof(P01_ClocksReset, out_APUV_4M),   0, 1),
    SignalData("CYBO_4M",     offsetof(P01_ClocksReset, out_CYBO_4M),   0, 1),
    SignalData("CERY_2M",     offsetof(P01_ClocksReset, out_CERY_2M),   0, 1),
    SignalData("BAVU_1M",     offsetof(P01_ClocksReset, out_BAVU_1M),   0, 1),
    */
    SignalData("BOGA_1M",     offsetof(P01_ClocksReset, out_BOGA_1M),   0, 1),
    //SignalData("CPU_RD_SYNC", offsetof(P01_ClocksReset, out_CPU_RD_SYNC), 0, 1),
    SignalData("----------"),

    SignalData("RESET2",      offsetof(P01_ClocksReset, AFER),          0, 1),

    SignalData("-----DIV-----"),
    SignalData("UKUP_00",     offsetof(P01_ClocksReset, UKUP),          0, 1),
    SignalData("UFOR_01",     offsetof(P01_ClocksReset, UFOR),          0, 1),
    SignalData("UNER_02",     offsetof(P01_ClocksReset, UNER),          0, 1),
    SignalData("TERO_03",     offsetof(P01_ClocksReset, TERO),          0, 1),
    SignalData("UNYK_04",     offsetof(P01_ClocksReset, UNYK),          0, 1),
    SignalData("TAMA_05",     offsetof(P01_ClocksReset, TAMA),          0, 1),
    SignalData("UGOT_06",     offsetof(P01_ClocksReset, UGOT),          0, 1),
    SignalData("TULU_07",     offsetof(P01_ClocksReset, TULU),          0, 1),
    SignalData("TUGO_08",     offsetof(P01_ClocksReset, TUGO),          0, 1),
    SignalData("TOFE_09",     offsetof(P01_ClocksReset, TOFE),          0, 1),
    SignalData("TERU_10",     offsetof(P01_ClocksReset, TERU),          0, 1),
    SignalData("SOLA_11",     offsetof(P01_ClocksReset, SOLA),          0, 1),
    SignalData("SUBU_12",     offsetof(P01_ClocksReset, SUBU),          0, 1),
    SignalData("TEKA_13",     offsetof(P01_ClocksReset, TEKA),          0, 1),
    SignalData("UKET_14",     offsetof(P01_ClocksReset, UKET),          0, 1),
    SignalData("UPOF_15",     offsetof(P01_ClocksReset, UPOF),          0, 1),

    /*
    SignalData("----------"),
    SignalData("BARA",        offsetof(P01_ClocksReset, BARA),          0, 1),
    SignalData("CARU",        offsetof(P01_ClocksReset, CARU),          0, 1),
    SignalData("BYLU",        offsetof(P01_ClocksReset, BYLU),          0, 1),

    SignalData("----------"),
    SignalData("ATYK",        offsetof(P01_ClocksReset, ATYK),          0, 1),
    SignalData("AVOK",        offsetof(P01_ClocksReset, AVOK),          0, 1),
    SignalData("JESO",        offsetof(P01_ClocksReset, JESO),          0, 1),
    */
  };
}

//-----------------------------------------------------------------------------

void P01_ClocksReset::tick(const P01_ClocksReset& prev) {

  //----------
  // Reset tree

  cell DULA = not(out_RESET2);
  cell CUNU = not(DULA);
  out_RESET6 = CUNU;
  cell XORE = not(CUNU);
  out_RESET7 = XORE;
  cell WESY = not(XORE);
  out_RESET9 = WESY;
  cell WALU = not(XORE);
  out_RESET8 = WALU;
  cell XEBE = not(out_RESET7);
  out_RESET7n = XEBE;
  cell XODO = and(in_FF40_D7, XEBE);
  cell XAPO = not(XODO);
  out_RESET_VIDEO = XAPO;

  //----------
  // Clock deglitcher + clock tree

  cell ARYS = not(in_CLKIN_B);

  // clock deglitcher
  ANOS = nand(in_CLKIN_B, prev.AVET);
  AVET = nand(ANOS, ARYS);
  ANOS = nand(in_CLKIN_B, AVET);
  AVET = nand(ANOS, ARYS);

  cell ATAL = not(AVET);
  cell AZOF = not(ATAL);

  cell ATAG = not(AZOF);
  cell ZAXY = not(AZOF);

  cell AMUK = not(ATAG);
  cell ZEME = not(ZAXY);

  cell APUV = not(AMUK);
  cell ARYF = not(AMUK);
  cell ALET = not(ZEME);

  out_CLK1 = ZEME;
  out_CLK2 = ALET;
  out_ATAL_4M = ATAL;
  out_AMUK_4M = AMUK;
  out_ARYF_4M = ARYF;
  out_APUV_4M = APUV;

  // from P17
  cell CYBO = not(out_AMUK_4M);
  out_CYBO_4M = CYBO;

  cell BELA = not(in_APU_RESET);
  tock_neg(prev.CERY, CERY, prev.out_CYBO_4M, out_CYBO_4M, BELA, !prev.CERY);
  out_CERY_2M = CERY;

  // from P09
  bool APU_RESET3n = not(in_APU_RESET);
  tock_neg(prev.AJER, AJER, prev.out_APUV_4M, out_APUV_4M, APU_RESET3n, !prev.AJER);
  out_AJER_2M = AJER;

  BATA = not(AJER);
  bool APU_RESETn = not(in_APU_RESET);
  tock_neg(prev.CALO, CALO, prev.BATA, BATA, APU_RESETn, !prev.CALO);

  DYFA = not(CALO);
  out_DYFA_1M = DYFA;

  //----------
  // Phase generator. These registers tick on _BOTH_EDGES_ of the master clock.

  tock_duo(prev.AFUR, AFUR, prev.out_ATAL_4M, out_ATAL_4M, in_T1nT2n, !prev.ADYK);
  tock_duo(prev.ALEF, ALEF, prev.out_ATAL_4M, out_ATAL_4M, in_T1nT2n, prev.AFUR);
  tock_duo(prev.APUK, APUK, prev.out_ATAL_4M, out_ATAL_4M, in_T1nT2n, prev.ALEF);
  tock_duo(prev.ADYK, ADYK, prev.out_ATAL_4M, out_ATAL_4M, in_T1nT2n, prev.APUK);

  cell ADAR = not(prev.ADYK);
  cell ATYP = not(prev.AFUR);
  cell AFEP = not(prev.ALEF);
  cell AROV = not(prev.APUK);

  cell AFAS = nor(ADAR, ATYP);
  out_AFAS = AFAS;
  cell AREV = nand(in_FROM_CPU3, AFAS);
  cell APOV = not(AREV);
  out_CPU_RD_SYNC = APOV;

  cell AJAX = not(ATYP);
  cell AGUT = unk3(AJAX, AROV, in_FROM_CPU4);
  cell AWOD = or(in_T1nT2, AGUT);
  cell ABUZ = not(AWOD);
  out_ABUZ = ABUZ;

  cell BUGO = not(AFEP);
  cell BATE = nor(BUGO, AROV, in_ABOL);
  cell BASU = not(BATE);
  cell BUKE = not(BASU);
  out_BUKE = BUKE;

  //----------
  // Weird logic feeding into BOGA_1M

  cell BAPY = nor(in_ABOL, AROV, ATYP);
  cell BERU = not(BAPY);
  cell BUFA = not(BERU);
  cell BYLY = not(BERU);
  cell BOLO = not(BUFA);
  cell BYDA = not(BYLY);

  cell NULE = nor(ATYP, in_ABOL);
  cell BYRY = not(NULE);
  cell BUDE = not(BYRY);
  cell BEVA = not(BYRY);
  cell BEKO = not(BUDE);
  cell BAVY = not(BEVA);

  cell BEJA = nand(BOLO, BYDA, BEKO, BAVY);
  cell BANE = not(BEJA);
  cell BELO = not(BANE);
  cell BAZE = not(BELO);
  cell BUTO = nand(AFEP, ATYP, BAZE);
  cell BELE = not(BUTO);
  cell ATEZ = not(in_CLKIN_A);
  cell BYJU = nor(BELE, ATEZ);
  cell BALY = not(BYJU);
  BOGA = not(BALY);
  out_BOGA_1M = BOGA;

  //----------
  // TO_CPU

  cell BUTY = not(in_ABOL);
  cell BUVU = and(BUTY, BALY);
  cell BYXO = not(BUVU);
  cell BEDO = not(BYXO);
  cell BOWA = not(BEDO);
  out_TO_CPU = BOWA;

  //----------
  // Cartridge clock

  out_PHI_OUT = BEVA;
  cell DOVA = not(BEVA);
  out_PHIn = DOVA;
  cell UVYT = not(out_PHI_OUT);
  out_PHI_OUTn = UVYT;

  //----------
  // RESET2 register

  cell UCOB = not(in_CLKIN_A);
  out_CLKIN_An = UCOB;
  cell UPYF = or(in_RESET, out_CLKIN_An);
  cell TUBO = unk2(in_ABOL, UPYF);
  cell UNUT = and(TUBO, prev.UPOF);
  cell TABA = or(in_T1nT2, in_T1T2n, UNUT);
  cell ALYP = not(TABA);
  cell AFAR = nor(ALYP, in_RESET);
  cell ASOL = unk2(AFAR, in_RESET);

  BOMA = not(BOGA);
  tock_neg(prev.AFER, AFER, prev.BOMA, BOMA, in_T1nT2n, ASOL);
  cell AVOR = or(prev.AFER, ASOL);
  cell ALUR = not(AVOR);
  out_RESET2 = ALUR;

  //----------
  // FF04 DIV

  cell TAPE = and(in_FF04_FF07, in_CPU_WR, in_TOLA_A1n, in_TOVY_A0n);
  cell UFOL = nor(out_CLKIN_An, in_RESET, TAPE);
  out_RESET_DIVn = UFOL;

  tock_neg(prev.UKUP, UKUP, prev.out_BOGA_1M, out_BOGA_1M, out_RESET_DIVn, !prev.UKUP);
  tock_pos(prev.UFOR, UFOR, prev.UKUP,         UKUP,         out_RESET_DIVn, !prev.UFOR);
  tock_pos(prev.UNER, UNER, prev.UFOR,         UFOR,         out_RESET_DIVn, !prev.UNER);
  tock_pos(prev.TERO, TERO, prev.UNER,         UNER,         out_RESET_DIVn, !prev.TERO);
  tock_pos(prev.UNYK, UNYK, prev.TERO,         TERO,         out_RESET_DIVn, !prev.UNYK);
  tock_pos(prev.TAMA, TAMA, prev.UNYK,         UNYK,         out_RESET_DIVn, !prev.TAMA);

  cell UVYN = not(TAMA);

  out_TAMA_16K  = !TAMA;
  out_CLK_256K  = UFOR;
  out_CLK_64K   = TERO;
  out_CLK_16K   = UVYN;

  ULUR = mux2(out_BOGA_1M, out_TAMA_16K, in_FF60_D1);

  tock_neg(prev.UGOT, UGOT, prev.ULUR, ULUR, out_RESET_DIVn, !prev.UGOT);
  tock_pos(prev.TULU, TULU, prev.UGOT, UGOT, out_RESET_DIVn, !prev.TULU);
  tock_pos(prev.TUGO, TUGO, prev.TULU, TULU, out_RESET_DIVn, !prev.TUGO);
  tock_pos(prev.TOFE, TOFE, prev.TUGO, TUGO, out_RESET_DIVn, !prev.TOFE);
  tock_pos(prev.TERU, TERU, prev.TOFE, TOFE, out_RESET_DIVn, !prev.TERU);
  tock_pos(prev.SOLA, SOLA, prev.TERU, TERU, out_RESET_DIVn, !prev.SOLA);
  tock_pos(prev.SUBU, SUBU, prev.SOLA, SOLA, out_RESET_DIVn, !prev.SUBU);
  tock_pos(prev.TEKA, TEKA, prev.SUBU, SUBU, out_RESET_DIVn, !prev.TEKA);
  tock_pos(prev.UKET, UKET, prev.TEKA, TEKA, out_RESET_DIVn, !prev.UKET);
  tock_pos(prev.UPOF, UPOF, prev.UKET, UKET, out_RESET_DIVn, !prev.UPOF);

  cell UMEK = not(prev.UGOT);
  cell UREK = not(prev.TULU);
  cell UTOK = not(prev.TUGO);
  cell SAPY = not(prev.TOFE);
  cell UMER = not(prev.TERU);
  cell RAVE = not(prev.SOLA);
  cell RYSO = not(prev.SUBU);
  cell UDOR = not(prev.TEKA);

  out_FF04_D0n = UMEK;
  out_FF04_D1n = UREK;
  out_UMER = UMER;

  cell TAGY = and(in_FF04_FF07, in_CPU_RD, in_TOLA_A1n, in_TOVY_A0n);

  cell TAWU = not(UMEK);
  cell TAKU = not(UREK);
  cell TEMU = not(UTOK);
  cell TUSE = not(SAPY);
  cell UPUG = not(UMER);
  cell SEPU = not(RAVE);
  cell SAWA = not(RYSO);
  cell TATU = not(UDOR);

  if (TAGY) {
    out_D0 = TAWU;
    out_D1 = TAKU;
    out_D2 = TEMU;
    out_D3 = TUSE;
    out_D4 = UPUG;
    out_D5 = SEPU;
    out_D6 = SAWA;
    out_D7 = TATU;
  }
  else {
    out_D0 = 0;
    out_D1 = 0;
    out_D2 = 0;
    out_D3 = 0;
    out_D4 = 0;
    out_D5 = 0;
    out_D6 = 0;
    out_D7 = 0;
  }

  //----------
  // Clock dividers for APU

  cell ATUS = not(in_APU_RESET);
  COKE = not(out_AJER_2M);
  tock_neg(prev.BARA, BARA, prev.COKE, COKE, ATUS, UMER);

  BURE = not(!BARA);
  tock_neg(prev.CARU, CARU, prev.BURE, BURE, ATUS, !prev.CARU);

  tock_neg(prev.BYLU, BYLU, !prev.CARU, !CARU, ATUS, !prev.BYLU);

  cell FYNE = not(BURE);
  cell CULO = not(!prev.CARU);
  cell APEF = not(!prev.BYLU);

  cell GALE = mux2(out_HAMA_512Kn, FYNE, in_FERO_Q);
  cell BEZE = mux2(out_HAMA_512Kn, CULO, in_FERO_Q);
  cell BULE = mux2(out_HAMA_512Kn, APEF, in_FERO_Q);

  cell GEXY = not(GALE);
  cell COFU = not(BEZE);
  cell BARU = not(BULE);

  cell HORU = not(GEXY);
  cell BUFY = not(COFU);
  cell BYFE = not(BARU);

  out_HORU_512 = HORU;
  out_BUFY_256 = BUFY;
  out_BYFE_128 = BYFE;

  cell BOPO = not(in_APU_RESET);
  tock_neg(prev.ATYK, ATYK, prev.out_ARYF_4M, out_ARYF_4M, BOPO, !prev.ATYK);
  tock_neg(prev.AVOK, AVOK, prev.ATYK, ATYK, BOPO, !prev.AVOK);

  BAVU = not(prev.AVOK);
  out_BAVU_1M = BAVU;

  tock_neg(prev.JESO, JESO, prev.BAVU, BAVU, in_APU_RESET5n, !prev.JESO);
  out_JESO_512K = JESO;

  cell HAMA = not(!JESO);
  out_HAMA_512Kn = HAMA;
}

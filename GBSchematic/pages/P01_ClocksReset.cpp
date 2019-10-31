#include "P01_ClocksReset.h"

//-----------------------------------------------------------------------------
// This file should contain the schematics as directly translated to C,
// no modifications or simplifications.

const std::vector<SignalData> P01_ClocksReset::signals() {
  return
  {
    SignalData("-----Clocks-----"),
    SignalData("PHASE_A",     offsetof(P01_ClocksReset, AFUR),          0, 1),
    SignalData("PHASE_B",     offsetof(P01_ClocksReset, ALEF),          0, 1),
    SignalData("PHASE_C",     offsetof(P01_ClocksReset, APUK),          0, 1),
    SignalData("PHASE_D",     offsetof(P01_ClocksReset, ADYK),          0, 1),
    /*
    SignalData("AMUK_4M",     offsetof(P01_ClocksReset, c.AMUK_4M),   0, 1),
    SignalData("ATAL_4M",     offsetof(P01_ClocksReset, c.ATAL_4M),   0, 1),
    SignalData("ARYF_4M",     offsetof(P01_ClocksReset, c.ARYF_4M),   0, 1),
    SignalData("APUV_4M",     offsetof(P01_ClocksReset, c.APUV_4M),   0, 1),
    SignalData("CYBO_4M",     offsetof(P01_ClocksReset, c.CYBO_4M),   0, 1),
    SignalData("CERY_2M",     offsetof(P01_ClocksReset, c.CERY_2M),   0, 1),
    SignalData("BAVU_1M",     offsetof(P01_ClocksReset, c.BAVU_1M),   0, 1),
    SignalData("BOGA_1M",     offsetof(P01_ClocksReset, c.BOGA_1M),   0, 1),
    SignalData("CPU_RD_SYNC", offsetof(P01_ClocksReset, c.CPU_RD_SYNC), 0, 1),
    */

    SignalData("-----Resets-----"),
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

void P01_ClocksReset::tick(const P01_ClocksReset& a, const P01_ClocksReset& b, P01_ClocksReset& c) {
  //----------
  // Reset tree

  c.DULA = not(b.RESET2);
  c.XEBE = not(b.RESET7);          c.RESET7n = c.XEBE;
  c.UCOB = not(b.CLKIN_A);         c.CLKIN_An = c.UCOB;
  c.CUNU = not(b.DULA);            c.RESET6 = c.CUNU;
  c.XODO = and(b.FF40_D7, b.XEBE);
  c.XORE = not(b.CUNU);            c.RESET7 = c.XORE;
  c.WESY = not(b.XORE);            c.RESET9 = c.WESY;
  c.WALU = not(b.XORE);            c.RESET8 = c.WALU;
  c.XAPO = not(b.XODO);            c.RESET_VIDEO = c.XAPO;

  //----------
  // Clock deglitcher + clock tree

  c.ARYS = not(b.CLKIN_B);

  // clock deglitcher
  c.ANOS = nand(b.CLKIN_B, b.AVET);
  c.AVET = nand(b.ANOS, b.ARYS);

  c.ATAL = not(b.AVET); c.ATAL_4M = c.ATAL;
  c.AZOF = not(b.ATAL);

  c.ATAG = not(b.AZOF);
  c.ZAXY = not(b.AZOF);

  c.AMUK = not(b.ATAG); c.AMUK_4M = c.AMUK;
  c.ZEME = not(b.ZAXY); c.CLK1 = c.ZEME;

  c.APUV = not(b.AMUK); c.APUV_4M = c.APUV;
  c.ARYF = not(b.AMUK); c.ARYF_4M = c.ARYF;
  c.ALET = not(b.ZEME); c.CLK2 = c.ALET;

  //----------
  // random clock

  c.BELA = not(b.APU_RESET);

  if (!b.BELA) {
    c.CERY = 0;
  }
  else if (a.CYBO_4M && !b.CYBO_4M) {
    c.CERY = !b.CERY;
  }
  else {
    c.CERY = b.CERY;
  }

  c.CERY_2M = b.CERY;

  //----------
  // Phase generator. These registers tick on _BOTH_EDGES_ of the master clock.

  if (!b.T1nT2n) {
    c.AFUR = 0;
    c.ALEF = 0;
    c.APUK = 0;
    c.ADYK = 0;
  }
  else if (a.ATAL_4M != b.ATAL_4M) {
    c.AFUR = !b.ADYK;
    c.ALEF = b.AFUR;
    c.APUK = b.ALEF;
    c.ADYK = b.APUK;
  }
  else {
    c.AFUR = b.AFUR;
    c.ALEF = b.ALEF;
    c.APUK = b.APUK;
    c.ADYK = b.ADYK;
  }

  c.ADAR = not(b.ADYK);
  c.ATYP = not(b.AFUR);
  c.AFEP = not(b.ALEF);
  c.AROV = not(b.APUK);

  c.AFAS = nor(b.ADAR, b.ATYP);
  c.AREV = nand(b.FROM_CPU3, b.AFAS);
  c.APOV = not(b.AREV);

  c.AJAX = not(b.ATYP);
  c.AGUT = unk3(b.AJAX, b.AROV, b.FROM_CPU4);
  c.AWOD = or(b.T1nT2, b.AGUT);
  c.ABUZ = not(b.AWOD);

  c.BUGO = not(b.AFEP);
  c.BATE = nor(b.BUGO, b.AROV, b.ABOL);
  c.BASU = not(b.BATE);
  c.BUKE = not(b.BASU);

  //----------
  // Weird logic feeding into BOGA_1M

  c.BAPY = nor(b.ABOL, b.AROV, b.ATYP);
  c.BERU = not(b.BAPY);
  c.BUFA = not(b.BERU);
  c.BYLY = not(b.BERU);
  c.BOLO = not(b.BUFA);
  c.BYDA = not(b.BYLY);

  c.NULE = nor(b.ATYP, b.ABOL);
  c.BYRY = not(b.NULE);
  c.BUDE = not(b.BYRY);
  c.BEVA = not(b.BYRY);
  c.BEKO = not(b.BUDE);
  c.BAVY = not(b.BEVA);

  c.BEJA = nand(b.BOLO, b.BYDA, b.BEKO, b.BAVY);
  c.BANE = not(b.BEJA);
  c.BELO = not(b.BANE);
  c.BAZE = not(b.BELO);
  c.BUTO = nand(b.AFEP, b.ATYP, b.BAZE);
  c.BELE = not(b.BUTO);
  c.ATEZ = not(b.CLKIN_A);
  c.BYJU = nor(b.BELE, b.ATEZ);
  c.BALY = not(b.BYJU);
  c.BOGA = not(b.BALY);

  //----------
  // TO_CPU

  c.BUTY = not(b.ABOL);
  c.BUVU = and(b.BUTY, b.BALY);
  c.BYXO = not(b.BUVU);
  c.BEDO = not(b.BYXO);
  c.BOWA = not(b.BEDO);

  //----------
  // Cartridge clock

  c.DOVA = not(b.BEVA);
  c.UVYT = not(b.PHI_OUT);

  //----------
  // RESET2 register

  c.UPYF = or(b.RESET, b.CLKIN_An);
  c.TUBO = unk2(b.ABOL, b.UPYF);
  c.UNUT = and(b.TUBO, b.UPOF);
  c.TABA = or(b.T1nT2, b.T1T2n, b.UNUT);
  c.ALYP = not(b.TABA);
  c.AFAR = nor(b.ALYP, b.RESET);
  c.ASOL = unk2(b.AFAR, b.RESET);

  c.BOMA = not(b.BOGA);
  tock_neg(b.AFER, c.AFER, a.BOMA, b.BOMA, b.T1nT2n, b.ASOL);
  c.AVOR = or(b.AFER, b.ASOL);
  c.ALUR = not(b.AVOR);

  //----------
  // FF04 DIV

  c.TAPE = and(b.FF04_FF07, b.CPU_WR, b.TOLA_A1n, b.TOVY_A0n);
  c.UFOL = nor(b.CLKIN_An, b.RESET, b.TAPE);

  tock_neg(b.UKUP, c.UKUP, a.BOGA_1M, b.BOGA_1M, b.RESET_DIVn, !b.UKUP);
  tock_pos(b.UFOR, c.UFOR, a.UKUP,    b.UKUP,    b.RESET_DIVn, !b.UFOR);
  tock_pos(b.UNER, c.UNER, a.UFOR,    b.UFOR,    b.RESET_DIVn, !b.UNER);
  tock_pos(b.TERO, c.TERO, a.UNER,    b.UNER,    b.RESET_DIVn, !b.TERO);
  tock_pos(b.UNYK, c.UNYK, a.TERO,    b.TERO,    b.RESET_DIVn, !b.UNYK);
  tock_pos(b.TAMA, c.TAMA, a.UNYK,    b.UNYK,    b.RESET_DIVn, !b.TAMA);

  c.UVYN = not(b.TAMA);

  c.ULUR = mux2(b.BOGA_1M, b.TAMA_16K, b.FF60_D1);

  tock_neg(b.UGOT, c.UGOT, a.ULUR, b.ULUR, b.RESET_DIVn, !b.UGOT);
  tock_pos(b.TULU, c.TULU, a.UGOT, b.UGOT, b.RESET_DIVn, !b.TULU);
  tock_pos(b.TUGO, c.TUGO, a.TULU, b.TULU, b.RESET_DIVn, !b.TUGO);
  tock_pos(b.TOFE, c.TOFE, a.TUGO, b.TUGO, b.RESET_DIVn, !b.TOFE);
  tock_pos(b.TERU, c.TERU, a.TOFE, b.TOFE, b.RESET_DIVn, !b.TERU);
  tock_pos(b.SOLA, c.SOLA, a.TERU, b.TERU, b.RESET_DIVn, !b.SOLA);
  tock_pos(b.SUBU, c.SUBU, a.SOLA, b.SOLA, b.RESET_DIVn, !b.SUBU);
  tock_pos(b.TEKA, c.TEKA, a.SUBU, b.SUBU, b.RESET_DIVn, !b.TEKA);
  tock_pos(b.UKET, c.UKET, a.TEKA, b.TEKA, b.RESET_DIVn, !b.UKET);
  tock_pos(b.UPOF, c.UPOF, a.UKET, b.UKET, b.RESET_DIVn, !b.UPOF);

  c.UMEK = not(b.UGOT);
  c.UREK = not(b.TULU);
  c.UTOK = not(b.TUGO);
  c.SAPY = not(b.TOFE);
  c.UMER = not(b.TERU);
  c.RAVE = not(b.SOLA);
  c.RYSO = not(b.SUBU);
  c.UDOR = not(b.TEKA);

  c.TAGY = and(b.FF04_FF07, b.CPU_RD, b.TOLA_A1n, b.TOVY_A0n);

  c.TAWU = not(b.UMEK);
  c.TAKU = not(b.UREK);
  c.TEMU = not(b.UTOK);
  c.TUSE = not(b.SAPY);
  c.UPUG = not(b.UMER);
  c.SEPU = not(b.RAVE);
  c.SAWA = not(b.RYSO);
  c.TATU = not(b.UDOR);

  //----------
  // Clock dividers for APU

  c.ATUS = not(b.APU_RESET);
  c.COKE = not(b.AJER_2M);
  c.BURE = not(!b.BARA);
  c.BARA = tock_neg( a.COKE,  b.COKE, b.ATUS, b.BARA,  b.UMER);
  c.CARU = tock_neg( a.BURE,  b.BURE, b.ATUS, b.CARU, !b.CARU);
  c.BYLU = tock_neg(!a.CARU, !b.CARU, b.ATUS, b.BYLU, !b.BYLU);

  c.FYNE = not(b.BURE);
  c.CULO = not(!b.CARU);
  c.APEF = not(!b.BYLU);

  c.GALE = mux2(b.HAMA_512Kn, b.FYNE, b.FERO_Q);
  c.BEZE = mux2(b.HAMA_512Kn, b.CULO, b.FERO_Q);
  c.BULE = mux2(b.HAMA_512Kn, b.APEF, b.FERO_Q);

  c.GEXY = not(b.GALE);
  c.COFU = not(b.BEZE);
  c.BARU = not(b.BULE);

  c.HORU = not(b.GEXY);
  c.BUFY = not(b.COFU);
  c.BYFE = not(b.BARU);

  c.BOPO = not(b.APU_RESET);
  c.ATYK = tock_neg(a.ARYF_4M, b.ARYF_4M, b.BOPO, b.ATYK, !b.ATYK);
  c.AVOK = tock_neg(a.ATYK,    b.ATYK,    b.BOPO, b.AVOK, !b.AVOK);

  c.BAVU = not(b.AVOK);

  c.JESO = tock_neg(a.BAVU, b.BAVU, b.APU_RESET5n, b.JESO, !b.JESO);

  c.HAMA = not(!b.JESO);

  //----------

  c.BOGA_1M = b.BOGA;
  c.CLKIN_An = b.UCOB;
  c.RESET2 = b.ALUR;
  c.AFAS = b.AFAS;
  c.CPU_RD_SYNC = b.APOV;
  c.ABUZ = b.ABUZ;
  c.BUKE = b.BUKE;

  c.TAMA_16K  = !b.TAMA;
  c.CLK_256K  = b.UFOR;
  c.CLK_64K   = b.TERO;
  c.CLK_16K   = b.UVYN;

  c.D0 = 0;
  c.D1 = 0;
  c.D2 = 0;
  c.D3 = 0;
  c.D4 = 0;
  c.D5 = 0;
  c.D6 = 0;
  c.D7 = 0;

  if (b.TAGY) {
    c.D0 = b.TAWU;
    c.D1 = b.TAKU;
    c.D2 = b.TEMU;
    c.D3 = b.TUSE;
    c.D4 = b.UPUG;
    c.D5 = b.SEPU;
    c.D6 = b.SAWA;
    c.D7 = b.TATU;
  }

  c.CERY_2M = b.CERY;
  c.FF04_D0n = b.UMEK;
  c.FF04_D1n = b.UREK;
  c.UMER = b.UMER;
  c.HORU_512 = b.HORU;
  c.BUFY_256 = b.BUFY;
  c.BYFE_128 = b.BYFE;
  c.BAVU_1M = b.BAVU;
  c.JESO_512K = b.JESO;
  c.HAMA_512Kn = b.HAMA;
  c.RESET6 = b.CUNU;
  c.RESET7 = b.XORE;
  c.RESET7n = b.XEBE;
  c.RESET9 = b.WESY;
  c.RESET8 = b.WALU;
  c.RESET_VIDEO = b.XAPO;
  c.CLK1 = b.ZEME;
  c.CLK2 = b.ALET;
  c.ATAL_4M = b.ATAL;
  c.AMUK_4M = b.AMUK;
  c.ARYF_4M = b.ARYF;
  c.APUV_4M = b.APUV;
  c.RESET_DIVn = b.UFOL;
  c.TO_CPU = b.BOWA;
  c.PHI_OUT = b.BEVA;
  c.PHIn = b.DOVA;
  c.PHI_OUTn = b.UVYT;
}

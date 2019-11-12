#include "P01_ClocksReset.h"
#include "Gameboy.h"

//-----------------------------------------------------------------------------
// This file should contain the schematics as directly translated to C,
// no modifications or simplifications.

const std::vector<SignalData> P01_ClocksReset::signals() {
  return
  {
    SignalData("-----Clocks-----"),
    SignalData("PHASE_ABCD", offsetof(P01_ClocksReset, AFUR)),
    SignalData("PHASE_BCDE", offsetof(P01_ClocksReset, ALEF)),
    SignalData("PHASE_CDEF", offsetof(P01_ClocksReset, APUK)),
    SignalData("PHASE_DEFG", offsetof(P01_ClocksReset, ADYK)),

    /*
    SignalData("-----DIV-----"),
    SignalData("UKUP_00",    offsetof(P01_ClocksReset, UKUP_00)),
    SignalData("UFOR_01",    offsetof(P01_ClocksReset, UFOR_01)),
    SignalData("UNER_02",    offsetof(P01_ClocksReset, UNER_02)),
    SignalData("TERO_03",    offsetof(P01_ClocksReset, TERO_03)),
    SignalData("UNYK_04",    offsetof(P01_ClocksReset, UNYK_04)),
    SignalData("TAMA_05",    offsetof(P01_ClocksReset, TAMA_05)),
    SignalData("UGOT_06",    offsetof(P01_ClocksReset, UGOT_06)),
    SignalData("TULU_07",    offsetof(P01_ClocksReset, TULU_07)),
    SignalData("TUGO_08",    offsetof(P01_ClocksReset, TUGO_08)),
    SignalData("TOFE_09",    offsetof(P01_ClocksReset, TOFE_09)),
    SignalData("TERU_10",    offsetof(P01_ClocksReset, TERU_10)),
    SignalData("SOLA_11",    offsetof(P01_ClocksReset, SOLA_11)),
    SignalData("SUBU_12",    offsetof(P01_ClocksReset, SUBU_12)),
    SignalData("TEKA_13",    offsetof(P01_ClocksReset, TEKA_13)),
    SignalData("UKET_14",    offsetof(P01_ClocksReset, UKET_14)),
    SignalData("UPOF_15",    offsetof(P01_ClocksReset, UPOF_15)),
    */

    /*
    SignalData("----------"),
    SignalData("BARA",       offsetof(P01_ClocksReset, BARA)),
    SignalData("CARU",       offsetof(P01_ClocksReset, CARU)),
    SignalData("BYLU",       offsetof(P01_ClocksReset, BYLU)),

    SignalData("----------"),
    SignalData("ATYK",       offsetof(P01_ClocksReset, ATYK)),
    SignalData("AVOK",       offsetof(P01_ClocksReset, AVOK)),
    SignalData("JESO",       offsetof(P01_ClocksReset, JESO)),
    */
  };
}

//-----------------------------------------------------------------------------

void P01_ClocksReset::tick(const Gameboy& a, const Gameboy& b, Gameboy& c) {

  //----------
  // Reset tree

  c.p01.DULA = not(b.p01.RESET2);
  c.p01.XEBE = not(b.p01.RESET7);
  c.p01.UCOB = not(b.chip.CLKIN_A);
  c.p01.CUNU = not(b.p01.DULA);
  c.p01.XODO = and(b.p23.FF40_D7, b.p01.XEBE);
  c.p01.XORE = not(b.p01.CUNU);
  c.p01.WESY = not(b.p01.XORE);
  c.p01.WALU = not(b.p01.XORE);
  c.p01.XAPO = not(b.p01.XODO);

  //----------
  // Clock deglitcher + clock tree

  c.p01.ARYS = not(b.chip.CLKIN_B);

  // clock deglitcher
  c.p01.ANOS = nand(b.chip.CLKIN_B, b.p01.AVET);
  c.p01.AVET = nand(b.p01.ANOS, b.p01.ARYS);

  c.p01.ATAL = not(b.p01.AVET);
  c.p01.AZOF = not(b.p01.ATAL);

  c.p01.ATAG = not(b.p01.AZOF);
  c.p01.ZAXY = not(b.p01.AZOF);

  c.p01.AMUK = not(b.p01.ATAG);
  c.p01.ZEME = not(b.p01.ZAXY);

  c.p01.APUV = not(b.p01.AMUK);
  c.p01.ARYF = not(b.p01.AMUK);
  c.p01.ALET = not(b.p01.ZEME);
  c.p01.LAPE = not(b.p01.ALET);
  c.p01.TAVA = not(b.p01.LAPE);

  //----------
  // random clock

  c.p01.BELA = not(b.p09.APU_RESET);
  c.p01.CYBO = not(b.p01.AMUK);
  c.p01.CERY = tock_pos(a.p01.CYBO, b.p01.CYBO, b.p01.BELA, b.p01.CERY, !b.p01.CERY);

  //----------
  // Phase generator. These registers tick on _BOTH_EDGES_ of the master clock.

  c.p01.AFUR = tock_duo(a.p01.ATAL_4M, b.p01.ATAL_4M, b.p07.T1nT2n, b.p01.AFUR, !b.p01.ADYK);
  c.p01.ALEF = tock_duo(a.p01.ATAL_4M, b.p01.ATAL_4M, b.p07.T1nT2n, b.p01.ALEF, b.p01.AFUR);
  c.p01.APUK = tock_duo(a.p01.ATAL_4M, b.p01.ATAL_4M, b.p07.T1nT2n, b.p01.APUK, b.p01.ALEF);
  c.p01.ADYK = tock_duo(a.p01.ATAL_4M, b.p01.ATAL_4M, b.p07.T1nT2n, b.p01.ADYK, b.p01.APUK);

  c.p01.ADAR = not(b.p01.ADYK);
  c.p01.ATYP = not(b.p01.AFUR);
  c.p01.AFEP = not(b.p01.ALEF);
  c.p01.AROV = not(b.p01.APUK);

  c.p01.AFAS = nor(b.p01.ADAR, b.p01.ATYP);
  c.p01.AREV = nand(b.cpu.FROM_CPU3, b.p01.AFAS);
  c.p01.APOV = not(b.p01.AREV);

  c.p01.AJAX = not(b.p01.ATYP);
  
  // AGUT = abcd__gh if FROM_CPU4 is high
  // FIXME make a cell func for and(or())
  c.p01.AGUT =  and(or(b.p01.AJAX, b.p01.AROV), b.cpu.FROM_CPU4);

  c.p01.AWOD = or(b.p07.T1nT2, b.p01.AGUT);
  c.p01.ABUZ = not(b.p01.AWOD);

  c.p01.ABOL = not(b.cpu.TO_ABOL);

  c.p01.BUGO = not(b.p01.AFEP);
  c.p01.BATE = nor(b.p01.BUGO, b.p01.AROV, b.p01.ABOL);
  c.p01.BASU = not(b.p01.BATE);
  c.p01.BUKE = not(b.p01.BASU);

  //----------
  // Weird logic feeding into BOGA_1M

  c.p01.BAPY = nor(b.p01.ABOL, b.p01.AROV, b.p01.ATYP);
  c.p01.BERU = not(b.p01.BAPY);
  c.p01.BUFA = not(b.p01.BERU);
  c.p01.BYLY = not(b.p01.BERU);
  c.p01.BOLO = not(b.p01.BUFA);
  c.p01.BYDA = not(b.p01.BYLY);

  c.p01.NULE = nor(b.p01.ATYP, b.p01.ABOL);
  c.p01.BYRY = not(b.p01.NULE);
  c.p01.BUDE = not(b.p01.BYRY);
  c.p01.BEVA = not(b.p01.BYRY);
  c.p01.BEKO = not(b.p01.BUDE);
  c.p01.BAVY = not(b.p01.BEVA);

  c.p01.BEJA = nand(b.p01.BOLO, b.p01.BYDA, b.p01.BEKO, b.p01.BAVY);
  c.p01.BANE = not(b.p01.BEJA);
  c.p01.BELO = not(b.p01.BANE);
  c.p01.BAZE = not(b.p01.BELO);
  c.p01.BUTO = nand(b.p01.AFEP, b.p01.ATYP, b.p01.BAZE);
  c.p01.BELE = not(b.p01.BUTO);
  c.p01.ATEZ = not(b.chip.CLKIN_A);
  c.p01.BYJU = nor(b.p01.BELE, b.p01.ATEZ);
  c.p01.BALY = not(b.p01.BYJU);
  c.p01.BOGA = not(b.p01.BALY);

  //----------
  // TO_CPU

  c.p01.BUTY = not(b.p01.ABOL);
  c.p01.BUVU = and(b.p01.BUTY, b.p01.BALY);
  c.p01.BYXO = not(b.p01.BUVU);
  c.p01.BEDO = not(b.p01.BYXO);
  c.p01.BOWA = not(b.p01.BEDO);

  //----------
  // Cartridge clock

  c.p01.DOVA = not(b.p01.BEVA);
  c.p01.UVYT = not(b.p01.PHI_OUT);

  //----------
  // RESET2 register

  c.p01.UPYF = or(b.chip.RST, b.p01.CLKIN_An);
  c.p01.TUBO = unk2(b.p01.ABOL, b.p01.UPYF);
  c.p01.UNUT = and(b.p01.TUBO, b.p01.UPOF_15);
  c.p01.TABA = or(b.p07.T1nT2, b.p07.T1T2n, b.p01.UNUT);
  c.p01.ALYP = not(b.p01.TABA);
  c.p01.AFAR = nor(b.p01.ALYP, b.chip.RST);
  c.p01.ASOL = unk2(b.p01.AFAR, b.chip.RST);

  c.p01.BOMA = not(b.p01.BOGA);
  c.p01.AFER = tock_pos(a.p01.BOMA, b.p01.BOMA, b.p07.T1nT2n, b.p01.AFER, b.p01.ASOL);
  c.p01.AVOR = or(b.p01.AFER, b.p01.ASOL);
  c.p01.ALUR = not(b.p01.AVOR);

  c.cpu.TABA = b.p01.TABA;

  //----------
  // FF04 DIV

  c.p01.TAPE = and(b.p03.FF04_FF07, b.p07.CPU_WR, b.p08.TOLA_A1n, b.p03.TOVY_A0n);
  c.p01.UFOL = nor(b.p01.CLKIN_An, b.chip.RST, b.p01.TAPE);
  c.p01.RESET_DIVn = b.p01.UFOL;

  c.p01.UKUP_00 = tock_pos(a.p01.BOGA_1M,  b.p01.BOGA_1M,  b.p01.RESET_DIVn, b.p01.UKUP_00, !b.p01.UKUP_00);
  c.p01.UFOR_01 = tock_pos(!a.p01.UKUP_00, !b.p01.UKUP_00, b.p01.RESET_DIVn, b.p01.UFOR_01, !b.p01.UFOR_01);
  c.p01.UNER_02 = tock_pos(!a.p01.UFOR_01, !b.p01.UFOR_01, b.p01.RESET_DIVn, b.p01.UNER_02, !b.p01.UNER_02);
  c.p01.TERO_03 = tock_pos(!a.p01.UNER_02, !b.p01.UNER_02, b.p01.RESET_DIVn, b.p01.TERO_03, !b.p01.TERO_03);
  c.p01.UNYK_04 = tock_pos(!a.p01.TERO_03, !b.p01.TERO_03, b.p01.RESET_DIVn, b.p01.UNYK_04, !b.p01.UNYK_04);
  c.p01.TAMA_05 = tock_pos(!a.p01.UNYK_04, !b.p01.UNYK_04, b.p01.RESET_DIVn, b.p01.TAMA_05, !b.p01.TAMA_05);


  c.p01.UVYN = not(b.p01.TAMA_05);
  c.p01.ULUR = mux2(b.p01.BOGA_1M, !b.p01.TAMA_16Kn, b.p07.FF60_D1);

  c.p01.UGOT_06 = tock_pos( a.p01.ULUR,     b.p01.ULUR,    b.p01.RESET_DIVn, b.p01.UGOT_06, !b.p01.UGOT_06);
  c.p01.TULU_07 = tock_pos(!a.p01.UGOT_06, !b.p01.UGOT_06, b.p01.RESET_DIVn, b.p01.TULU_07, !b.p01.TULU_07);
  c.p01.TUGO_08 = tock_pos(!a.p01.TULU_07, !b.p01.TULU_07, b.p01.RESET_DIVn, b.p01.TUGO_08, !b.p01.TUGO_08);
  c.p01.TOFE_09 = tock_pos(!a.p01.TUGO_08, !b.p01.TUGO_08, b.p01.RESET_DIVn, b.p01.TOFE_09, !b.p01.TOFE_09);
  c.p01.TERU_10 = tock_pos(!a.p01.TOFE_09, !b.p01.TOFE_09, b.p01.RESET_DIVn, b.p01.TERU_10, !b.p01.TERU_10);
  c.p01.SOLA_11 = tock_pos(!a.p01.TERU_10, !b.p01.TERU_10, b.p01.RESET_DIVn, b.p01.SOLA_11, !b.p01.SOLA_11);
  c.p01.SUBU_12 = tock_pos(!a.p01.SOLA_11, !b.p01.SOLA_11, b.p01.RESET_DIVn, b.p01.SUBU_12, !b.p01.SUBU_12);
  c.p01.TEKA_13 = tock_pos(!a.p01.SUBU_12, !b.p01.SUBU_12, b.p01.RESET_DIVn, b.p01.TEKA_13, !b.p01.TEKA_13);
  c.p01.UKET_14 = tock_pos(!a.p01.TEKA_13, !b.p01.TEKA_13, b.p01.RESET_DIVn, b.p01.UKET_14, !b.p01.UKET_14);
  c.p01.UPOF_15 = tock_pos(!a.p01.UKET_14, !b.p01.UKET_14, b.p01.RESET_DIVn, b.p01.UPOF_15, !b.p01.UPOF_15);

  c.p01.UMEK = not(b.p01.UGOT_06);
  c.p01.UREK = not(b.p01.TULU_07);
  c.p01.UTOK = not(b.p01.TUGO_08);
  c.p01.SAPY = not(b.p01.TOFE_09);
  c.p01.UMER = not(b.p01.TERU_10);
  c.p01.RAVE = not(b.p01.SOLA_11);
  c.p01.RYSO = not(b.p01.SUBU_12);
  c.p01.UDOR = not(b.p01.TEKA_13);

  c.p01.TAGY = and(b.p03.FF04_FF07, b.p07.CPU_RD, b.p08.TOLA_A1n, b.p03.TOVY_A0n);
  c.p01.TAWU = not(b.p01.UMEK);
  c.p01.TAKU = not(b.p01.UREK);
  c.p01.TEMU = not(b.p01.UTOK);
  c.p01.TUSE = not(b.p01.SAPY);
  c.p01.UPUG = not(b.p01.UMER);
  c.p01.SEPU = not(b.p01.RAVE);
  c.p01.SAWA = not(b.p01.RYSO);
  c.p01.TATU = not(b.p01.UDOR);

  if (b.p01.TAGY) {
    c.D0 = b.p01.TAWU;
    c.D1 = b.p01.TAKU;
    c.D2 = b.p01.TEMU;
    c.D3 = b.p01.TUSE;
    c.D4 = b.p01.UPUG;
    c.D5 = b.p01.SEPU;
    c.D6 = b.p01.SAWA;
    c.D7 = b.p01.TATU;
  }

  //----------
  // Clock dividers for APU

  c.p01.ATUS = not(b.p09.APU_RESET);
  c.p01.COKE = not(b.p09.AJER_2M);
  c.p01.BURE = not(!b.p01.BARA);
  c.p01.BARA = tock_pos( a.p01.COKE,  b.p01.COKE, b.p01.ATUS, b.p01.BARA,  b.p01.UMER);
  c.p01.CARU = tock_pos( a.p01.BURE,  b.p01.BURE, b.p01.ATUS, b.p01.CARU, !b.p01.CARU);
  c.p01.BYLU = tock_pos(!a.p01.CARU, !b.p01.CARU, b.p01.ATUS, b.p01.BYLU, !b.p01.BYLU);

  c.p01.FYNE = not(b.p01.BURE);
  c.p01.CULO = not(!b.p01.CARU);
  c.p01.APEF = not(!b.p01.BYLU);

  c.p01.GALE = mux2(b.p01.HAMA_512Kn, b.p01.FYNE, b.p09.FERO);
  c.p01.BEZE = mux2(b.p01.HAMA_512Kn, b.p01.CULO, b.p09.FERO);
  c.p01.BULE = mux2(b.p01.HAMA_512Kn, b.p01.APEF, b.p09.FERO);

  c.p01.GEXY = not(b.p01.GALE);
  c.p01.COFU = not(b.p01.BEZE);
  c.p01.BARU = not(b.p01.BULE);

  c.p01.HORU = not(b.p01.GEXY);
  c.p01.BUFY = not(b.p01.COFU);
  c.p01.BYFE = not(b.p01.BARU);

  c.p01.BOPO = not(b.p09.APU_RESET);
  c.p01.BAVU = not(b.p01.AVOK);

  c.p01.ATYK = tock_pos(a.p01.ARYF_4M, b.p01.ARYF_4M, b.p01.BOPO,        b.p01.ATYK, !b.p01.ATYK);
  c.p01.AVOK = tock_pos(a.p01.ATYK,    b.p01.ATYK,    b.p01.BOPO,        b.p01.AVOK, !b.p01.AVOK);
  c.p01.JESO = tock_pos(a.p01.BAVU,    b.p01.BAVU,    b.p09.APU_RESET5n, b.p01.JESO, !b.p01.JESO);

  c.p01.HAMA = not(!b.p01.JESO);
}

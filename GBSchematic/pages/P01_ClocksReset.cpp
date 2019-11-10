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

void P01_ClocksReset::tick(const Gameboy& ga, const Gameboy& gb, Gameboy& gc) {
  const P01_ClocksReset& pa = ga.p01;
  const P01_ClocksReset& pb = gb.p01;
  P01_ClocksReset& pc = gc.p01;

  //----------
  // Reset tree

  pc.DULA = not(pb.RESET2);
  pc.XEBE = not(pb.RESET7);
  pc.UCOB = not(gb.chip.CLKIN_A);
  pc.CUNU = not(pb.DULA);
  pc.XODO = and(gb.p23.FF40_D7, pb.XEBE);
  pc.XORE = not(pb.CUNU);
  pc.WESY = not(pb.XORE);
  pc.WALU = not(pb.XORE);
  pc.XAPO = not(pb.XODO);

  //----------
  // Clock deglitcher + clock tree

  pc.ARYS = not(gb.chip.CLKIN_B);

  // clock deglitcher
  pc.ANOS = nand(gb.chip.CLKIN_B, pb.AVET);
  pc.AVET = nand(pb.ANOS, pb.ARYS);

  pc.ATAL = not(pb.AVET);
  pc.AZOF = not(pb.ATAL);

  pc.ATAG = not(pb.AZOF);
  pc.ZAXY = not(pb.AZOF);

  pc.AMUK = not(pb.ATAG);
  pc.ZEME = not(pb.ZAXY);

  pc.APUV = not(pb.AMUK);
  pc.ARYF = not(pb.AMUK);
  pc.ALET = not(pb.ZEME);
  pc.LAPE = not(pb.ALET);
  pc.TAVA = not(pb.LAPE);

  //----------
  // random clock

  pc.BELA = not(gb.p09.APU_RESET);
  pc.CYBO = not(pb.AMUK);
  pc.CERY = tock_pos(pa.CYBO, pb.CYBO, pb.BELA, pb.CERY, !pb.CERY);

  //----------
  // Phase generator. These registers tick on _BOTH_EDGES_ of the master clock.

  pc.AFUR = tock_duo(pa.ATAL_4M, pb.ATAL_4M, gb.p07.T1nT2n, pb.AFUR, !pb.ADYK);
  pc.ALEF = tock_duo(pa.ATAL_4M, pb.ATAL_4M, gb.p07.T1nT2n, pb.ALEF, pb.AFUR);
  pc.APUK = tock_duo(pa.ATAL_4M, pb.ATAL_4M, gb.p07.T1nT2n, pb.APUK, pb.ALEF);
  pc.ADYK = tock_duo(pa.ATAL_4M, pb.ATAL_4M, gb.p07.T1nT2n, pb.ADYK, pb.APUK);

  pc.ADAR = not(pb.ADYK);
  pc.ATYP = not(pb.AFUR);
  pc.AFEP = not(pb.ALEF);
  pc.AROV = not(pb.APUK);

  pc.AFAS = nor(pb.ADAR, pb.ATYP);
  pc.AREV = nand(gb.cpu.FROM_CPU3, pb.AFAS);
  pc.APOV = not(pb.AREV);

  pc.AJAX = not(pb.ATYP);
  
  // AGUT = abcd__gh if FROM_CPU4 is high
  pc.AGUT =  and(or(pb.AJAX, pb.AROV), gb.cpu.FROM_CPU4);

  pc.AWOD = or(gb.p07.T1nT2, pb.AGUT);
  pc.ABUZ = not(pb.AWOD);

  pc.BUGO = not(pb.AFEP);
  pc.BATE = nor(pb.BUGO, pb.AROV, gb.ABOL);
  pc.BASU = not(pb.BATE);
  pc.BUKE = not(pb.BASU);

  //----------
  // Weird logic feeding into BOGA_1M

  pc.BAPY = nor(gb.ABOL, pb.AROV, pb.ATYP);
  pc.BERU = not(pb.BAPY);
  pc.BUFA = not(pb.BERU);
  pc.BYLY = not(pb.BERU);
  pc.BOLO = not(pb.BUFA);
  pc.BYDA = not(pb.BYLY);

  pc.NULE = nor(pb.ATYP, gb.ABOL);
  pc.BYRY = not(pb.NULE);
  pc.BUDE = not(pb.BYRY);
  pc.BEVA = not(pb.BYRY);
  pc.BEKO = not(pb.BUDE);
  pc.BAVY = not(pb.BEVA);

  pc.BEJA = nand(pb.BOLO, pb.BYDA, pb.BEKO, pb.BAVY);
  pc.BANE = not(pb.BEJA);
  pc.BELO = not(pb.BANE);
  pc.BAZE = not(pb.BELO);
  pc.BUTO = nand(pb.AFEP, pb.ATYP, pb.BAZE);
  pc.BELE = not(pb.BUTO);
  pc.ATEZ = not(gb.chip.CLKIN_A);
  pc.BYJU = nor(pb.BELE, pb.ATEZ);
  pc.BALY = not(pb.BYJU);
  pc.BOGA = not(pb.BALY);

  //----------
  // TO_CPU

  pc.BUTY = not(gb.ABOL);
  pc.BUVU = and(pb.BUTY, pb.BALY);
  pc.BYXO = not(pb.BUVU);
  pc.BEDO = not(pb.BYXO);
  pc.BOWA = not(pb.BEDO);

  //----------
  // Cartridge clock

  pc.DOVA = not(pb.BEVA);
  pc.UVYT = not(pb.PHI_OUT);

  //----------
  // RESET2 register

  pc.UPYF = or(gb.chip.RST, pb.CLKIN_An);
  pc.TUBO = unk2(gb.ABOL, pb.UPYF);
  pc.UNUT = and(pb.TUBO, pb.UPOF_15);
  pc.TABA = or(gb.p07.T1nT2, gb.p07.T1T2n, pb.UNUT);
  pc.ALYP = not(pb.TABA);
  pc.AFAR = nor(pb.ALYP, gb.chip.RST);
  pc.ASOL = unk2(pb.AFAR, gb.chip.RST);

  pc.BOMA = not(pb.BOGA);
  pc.AFER = tock_pos(pa.BOMA, pb.BOMA, gb.p07.T1nT2n, pb.AFER, pb.ASOL);
  pc.AVOR = or(pb.AFER, pb.ASOL);
  pc.ALUR = not(pb.AVOR);

  gc.cpu.TABA = pb.TABA;

  //----------
  // FF04 DIV

  pc.TAPE = and(gb.p03.FF04_FF07, gb.p07.CPU_WR, gb.p08.TOLA_A1n, gb.p03.TOVY_A0n);
  pc.UFOL = nor(pb.CLKIN_An, gb.chip.RST, pb.TAPE);
  pc.RESET_DIVn = pb.UFOL;

  pc.UKUP_00 = tock_pos(pa.BOGA_1M,  pb.BOGA_1M,  pb.RESET_DIVn, pb.UKUP_00, !pb.UKUP_00);
  pc.UFOR_01 = tock_pos(!pa.UKUP_00, !pb.UKUP_00, pb.RESET_DIVn, pb.UFOR_01, !pb.UFOR_01);
  pc.UNER_02 = tock_pos(!pa.UFOR_01, !pb.UFOR_01, pb.RESET_DIVn, pb.UNER_02, !pb.UNER_02);
  pc.TERO_03 = tock_pos(!pa.UNER_02, !pb.UNER_02, pb.RESET_DIVn, pb.TERO_03, !pb.TERO_03);
  pc.UNYK_04 = tock_pos(!pa.TERO_03, !pb.TERO_03, pb.RESET_DIVn, pb.UNYK_04, !pb.UNYK_04);
  pc.TAMA_05 = tock_pos(!pa.UNYK_04, !pb.UNYK_04, pb.RESET_DIVn, pb.TAMA_05, !pb.TAMA_05);


  pc.UVYN = not(pb.TAMA_05);
  pc.ULUR = mux2(pb.BOGA_1M, !pb.TAMA_16Kn, gb.p07.FF60_D1);

  pc.UGOT_06 = tock_pos( pa.ULUR,     pb.ULUR,    pb.RESET_DIVn, pb.UGOT_06, !pb.UGOT_06);
  pc.TULU_07 = tock_pos(!pa.UGOT_06, !pb.UGOT_06, pb.RESET_DIVn, pb.TULU_07, !pb.TULU_07);
  pc.TUGO_08 = tock_pos(!pa.TULU_07, !pb.TULU_07, pb.RESET_DIVn, pb.TUGO_08, !pb.TUGO_08);
  pc.TOFE_09 = tock_pos(!pa.TUGO_08, !pb.TUGO_08, pb.RESET_DIVn, pb.TOFE_09, !pb.TOFE_09);
  pc.TERU_10 = tock_pos(!pa.TOFE_09, !pb.TOFE_09, pb.RESET_DIVn, pb.TERU_10, !pb.TERU_10);
  pc.SOLA_11 = tock_pos(!pa.TERU_10, !pb.TERU_10, pb.RESET_DIVn, pb.SOLA_11, !pb.SOLA_11);
  pc.SUBU_12 = tock_pos(!pa.SOLA_11, !pb.SOLA_11, pb.RESET_DIVn, pb.SUBU_12, !pb.SUBU_12);
  pc.TEKA_13 = tock_pos(!pa.SUBU_12, !pb.SUBU_12, pb.RESET_DIVn, pb.TEKA_13, !pb.TEKA_13);
  pc.UKET_14 = tock_pos(!pa.TEKA_13, !pb.TEKA_13, pb.RESET_DIVn, pb.UKET_14, !pb.UKET_14);
  pc.UPOF_15 = tock_pos(!pa.UKET_14, !pb.UKET_14, pb.RESET_DIVn, pb.UPOF_15, !pb.UPOF_15);

  pc.UMEK = not(pb.UGOT_06);
  pc.UREK = not(pb.TULU_07);
  pc.UTOK = not(pb.TUGO_08);
  pc.SAPY = not(pb.TOFE_09);
  pc.UMER = not(pb.TERU_10);
  pc.RAVE = not(pb.SOLA_11);
  pc.RYSO = not(pb.SUBU_12);
  pc.UDOR = not(pb.TEKA_13);

  pc.TAGY = and(gb.p03.FF04_FF07, gb.p07.CPU_RD, gb.p08.TOLA_A1n, gb.p03.TOVY_A0n);
  pc.TAWU = not(pb.UMEK);
  pc.TAKU = not(pb.UREK);
  pc.TEMU = not(pb.UTOK);
  pc.TUSE = not(pb.SAPY);
  pc.UPUG = not(pb.UMER);
  pc.SEPU = not(pb.RAVE);
  pc.SAWA = not(pb.RYSO);
  pc.TATU = not(pb.UDOR);

  if (pb.TAGY) {
    gc.D0 = pb.TAWU;
    gc.D1 = pb.TAKU;
    gc.D2 = pb.TEMU;
    gc.D3 = pb.TUSE;
    gc.D4 = pb.UPUG;
    gc.D5 = pb.SEPU;
    gc.D6 = pb.SAWA;
    gc.D7 = pb.TATU;
  }

  //----------
  // Clock dividers for APU

  pc.ATUS = not(gb.p09.APU_RESET);
  pc.COKE = not(gb.p09.AJER_2M);
  pc.BURE = not(!pb.BARA);
  pc.BARA = tock_pos( pa.COKE,  pb.COKE, pb.ATUS, pb.BARA,  pb.UMER);
  pc.CARU = tock_pos( pa.BURE,  pb.BURE, pb.ATUS, pb.CARU, !pb.CARU);
  pc.BYLU = tock_pos(!pa.CARU, !pb.CARU, pb.ATUS, pb.BYLU, !pb.BYLU);

  pc.FYNE = not(pb.BURE);
  pc.CULO = not(!pb.CARU);
  pc.APEF = not(!pb.BYLU);

  pc.GALE = mux2(pb.HAMA_512Kn, pb.FYNE, gb.p09.FERO);
  pc.BEZE = mux2(pb.HAMA_512Kn, pb.CULO, gb.p09.FERO);
  pc.BULE = mux2(pb.HAMA_512Kn, pb.APEF, gb.p09.FERO);

  pc.GEXY = not(pb.GALE);
  pc.COFU = not(pb.BEZE);
  pc.BARU = not(pb.BULE);

  pc.HORU = not(pb.GEXY);
  pc.BUFY = not(pb.COFU);
  pc.BYFE = not(pb.BARU);

  pc.BOPO = not(gb.p09.APU_RESET);
  pc.BAVU = not(pb.AVOK);

  pc.ATYK = tock_pos(pa.ARYF_4M, pb.ARYF_4M, pb.BOPO,            pb.ATYK, !pb.ATYK);
  pc.AVOK = tock_pos(pa.ATYK,    pb.ATYK,    pb.BOPO,            pb.AVOK, !pb.AVOK);
  pc.JESO = tock_pos(pa.BAVU,    pb.BAVU,    gb.p09.APU_RESET5n, pb.JESO, !pb.JESO);

  pc.HAMA = not(!pb.JESO);
}

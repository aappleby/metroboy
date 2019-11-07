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

  pc.DULA = not(gb.RESET2);
  pc.XEBE = not(gb.RESET7);
  pc.UCOB = not(gb.chip.CLKIN_A);
  pc.CUNU = not(pb.DULA);
  pc.XODO = and(gb.FF40_D7, pb.XEBE);
  pc.XORE = not(pb.CUNU);
  pc.WESY = not(pb.XORE);
  pc.WALU = not(pb.XORE);
  pc.XAPO = not(pb.XODO);

  gc.CLKIN_An = pb.UCOB;
  gc.RESET6   = pb.CUNU;
  gc.RESET7   = pb.XORE;
  gc.RESET7n  = pb.XEBE;
  gc.RESET8   = pb.WALU;
  gc.RESET9   = pb.WESY;

  gc.RESET_VIDEO = pb.XAPO;

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

  gc.AMUK_4M = pb.AMUK;
  gc.APUV_4M = pb.APUV;
  gc.ARYF_4M = pb.ARYF;
  gc.ATAL_4M = pb.ATAL;
  gc.CLK1    = pb.ZEME;
  gc.CLK2    = pb.ALET;

  //----------
  // random clock

  pc.BELA = not(gb.APU_RESET);
  pc.CERY = tock_pos(ga.CYBO_4M, gb.CYBO_4M, pb.BELA, pb.CERY, !pb.CERY);

  gc.CERY_2M     = pb.CERY;

  //----------
  // Phase generator. These registers tick on _BOTH_EDGES_ of the master clock.

  pc.AFUR = tock_duo(ga.ATAL_4M, gb.ATAL_4M, gb.T1nT2n, pb.AFUR, !pb.ADYK);
  pc.ALEF = tock_duo(ga.ATAL_4M, gb.ATAL_4M, gb.T1nT2n, pb.ALEF, pb.AFUR);
  pc.APUK = tock_duo(ga.ATAL_4M, gb.ATAL_4M, gb.T1nT2n, pb.APUK, pb.ALEF);
  pc.ADYK = tock_duo(ga.ATAL_4M, gb.ATAL_4M, gb.T1nT2n, pb.ADYK, pb.APUK);

  pc.ADAR = not(pb.ADYK);
  pc.ATYP = not(pb.AFUR);
  pc.AFEP = not(pb.ALEF);
  pc.AROV = not(pb.APUK);

  pc.AFAS = nor(pb.ADAR, pb.ATYP);
  pc.AREV = nand(gb.cpu.FROM_CPU3, pb.AFAS);
  pc.APOV = not(pb.AREV);

  pc.AJAX = not(pb.ATYP);
  pc.AGUT = unk3(pb.AJAX, pb.AROV, gb.cpu.FROM_CPU4);
  pc.AWOD = or(gb.T1nT2, pb.AGUT);
  pc.ABUZ = not(pb.AWOD);

  pc.BUGO = not(pb.AFEP);
  pc.BATE = nor(pb.BUGO, pb.AROV, gb.ABOL);
  pc.BASU = not(pb.BATE);
  pc.BUKE = not(pb.BASU);

  gc.CPU_RD_SYNC = pb.APOV;

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

  gc.BOGA_1M = pb.BOGA;

  //----------
  // TO_CPU

  pc.BUTY = not(gb.ABOL);
  pc.BUVU = and(pb.BUTY, pb.BALY);
  pc.BYXO = not(pb.BUVU);
  pc.BEDO = not(pb.BYXO);
  pc.BOWA = not(pb.BEDO);

  gc.BEDO   = pb.BEDO;
  gc.TO_CPU = pb.BOWA;

  //----------
  // Cartridge clock

  pc.DOVA = not(pb.BEVA);
  pc.UVYT = not(gb.PHI_OUT);

  gc.PHI_OUT     = pb.BEVA;
  gc.PHI_OUTn    = pb.UVYT;
  gc.PHIn        = pb.DOVA;

  //----------
  // RESET2 register

  pc.UPYF = or(gb.chip.RESET, gb.CLKIN_An);
  pc.TUBO = unk2(gb.ABOL, pb.UPYF);
  pc.UNUT = and(pb.TUBO, pb.UPOF_15);
  pc.TABA = or(gb.T1nT2, gb.T1T2n, pb.UNUT);
  pc.ALYP = not(pb.TABA);
  pc.AFAR = nor(pb.ALYP, gb.chip.RESET);
  pc.ASOL = unk2(pb.AFAR, gb.chip.RESET);

  pc.BOMA = not(pb.BOGA);
  pc.AFER = tock_pos(pa.BOMA, pb.BOMA, gb.T1nT2n, pb.AFER, pb.ASOL);
  pc.AVOR = or(pb.AFER, pb.ASOL);
  pc.ALUR = not(pb.AVOR);

  gc.RESET2 = pb.ALUR;
  gc.cpu.TABA = pb.TABA;

  //----------
  // FF04 DIV

  pc.TAPE = and(gb.FF04_FF07, gb.CPU_WR, gb.TOLA_A1n, gb.TOVY_A0n);
  pc.UFOL = nor(gb.CLKIN_An, gb.chip.RESET, pb.TAPE);
  pc.RESET_DIVn = pb.UFOL;

  pc.UKUP_00 = tock_pos(ga.BOGA_1M,  gb.BOGA_1M,  pb.RESET_DIVn, pb.UKUP_00, !pb.UKUP_00);
  pc.UFOR_01 = tock_pos(!pa.UKUP_00, !pb.UKUP_00, pb.RESET_DIVn, pb.UFOR_01, !pb.UFOR_01);
  pc.UNER_02 = tock_pos(!pa.UFOR_01, !pb.UFOR_01, pb.RESET_DIVn, pb.UNER_02, !pb.UNER_02);
  pc.TERO_03 = tock_pos(!pa.UNER_02, !pb.UNER_02, pb.RESET_DIVn, pb.TERO_03, !pb.TERO_03);
  pc.UNYK_04 = tock_pos(!pa.TERO_03, !pb.TERO_03, pb.RESET_DIVn, pb.UNYK_04, !pb.UNYK_04);
  pc.TAMA_05 = tock_pos(!pa.UNYK_04, !pb.UNYK_04, pb.RESET_DIVn, pb.TAMA_05, !pb.TAMA_05);

  gc.CLK_256K    = pb.UFOR_01;
  gc.CLK_64K     = pb.TERO_03;
  gc.TAMA_16K    = !pb.TAMA_05;

  pc.UVYN = not(pb.TAMA_05);
  pc.ULUR = mux2(gb.BOGA_1M, gb.TAMA_16K, gb.FF60_D1);

  gc.CLK_16K     = pb.UVYN;

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

  pc.TAGY = and(gb.FF04_FF07, gb.CPU_RD, gb.TOLA_A1n, gb.TOVY_A0n);
  pc.TAWU = not(pb.UMEK);
  pc.TAKU = not(pb.UREK);
  pc.TEMU = not(pb.UTOK);
  pc.TUSE = not(pb.SAPY);
  pc.UPUG = not(pb.UMER);
  pc.SEPU = not(pb.RAVE);
  pc.SAWA = not(pb.RYSO);
  pc.TATU = not(pb.UDOR);

  if (pb.TAGY) {
    gc.cpu.D0 = pb.TAWU;
    gc.cpu.D1 = pb.TAKU;
    gc.cpu.D2 = pb.TEMU;
    gc.cpu.D3 = pb.TUSE;
    gc.cpu.D4 = pb.UPUG;
    gc.cpu.D5 = pb.SEPU;
    gc.cpu.D6 = pb.SAWA;
    gc.cpu.D7 = pb.TATU;
  }

  gc.FF04_D0n = pb.UMEK;
  gc.FF04_D1n = pb.UREK;

  //----------
  // Clock dividers for APU

  pc.ATUS = not(gb.APU_RESET);
  pc.COKE = not(gb.AJER_2M);
  pc.BURE = not(!pb.BARA);
  pc.BARA = tock_pos( pa.COKE,  pb.COKE, pb.ATUS, pb.BARA,  pb.UMER);
  pc.CARU = tock_pos( pa.BURE,  pb.BURE, pb.ATUS, pb.CARU, !pb.CARU);
  pc.BYLU = tock_pos(!pa.CARU, !pb.CARU, pb.ATUS, pb.BYLU, !pb.BYLU);

  pc.FYNE = not(pb.BURE);
  pc.CULO = not(!pb.CARU);
  pc.APEF = not(!pb.BYLU);

  pc.GALE = mux2(gb.HAMA_512Kn, pb.FYNE, gb.FERO_Q);
  pc.BEZE = mux2(gb.HAMA_512Kn, pb.CULO, gb.FERO_Q);
  pc.BULE = mux2(gb.HAMA_512Kn, pb.APEF, gb.FERO_Q);

  pc.GEXY = not(pb.GALE);
  pc.COFU = not(pb.BEZE);
  pc.BARU = not(pb.BULE);

  pc.HORU = not(pb.GEXY);
  pc.BUFY = not(pb.COFU);
  pc.BYFE = not(pb.BARU);

  pc.BOPO = not(gb.APU_RESET);
  pc.BAVU = not(pb.AVOK);

  pc.ATYK = tock_pos(ga.ARYF_4M, gb.ARYF_4M, pb.BOPO,        pb.ATYK, !pb.ATYK);
  pc.AVOK = tock_pos(pa.ATYK,    pb.ATYK,    pb.BOPO,        pb.AVOK, !pb.AVOK);
  pc.JESO = tock_pos(pa.BAVU,    pb.BAVU,    gb.APU_RESET5n, pb.JESO, !pb.JESO);

  pc.HAMA = not(!pb.JESO);

  gc.BAVU_1M     = pb.BAVU;
  gc.BUFY_256    = pb.BUFY;
  gc.BYFE_128    = pb.BYFE;
  gc.HAMA_512Kn  = pb.HAMA;
  gc.HORU_512    = pb.HORU;
  gc.JESO_512K   = pb.JESO;
}

#include "P01_ClocksReset.h"

//-----------------------------------------------------------------------------
// This file should contain the schematics as directly translated to C,
// no modifications or simplifications.

const std::vector<SignalData> P01_ClocksReset::signals() {
  return
  {
    SignalData("-----Resets-----"),
    SignalData("RESET2", offsetof(P01_ClocksReset, out.RESET2)),

    SignalData("-----Clocks-----"),
    SignalData("PHASE_A",     offsetof(P01_ClocksReset, AFUR),          0, 1),
    SignalData("PHASE_B",     offsetof(P01_ClocksReset, ALEF),          0, 1),
    SignalData("PHASE_C",     offsetof(P01_ClocksReset, APUK),          0, 1),
    SignalData("PHASE_D",     offsetof(P01_ClocksReset, ADYK),          0, 1),

    SignalData("BOGA_1M",     offsetof(P01_ClocksReset, out.BOGA_1M),   0, 1),
    SignalData("AMUK_4M",     offsetof(P01_ClocksReset, out.AMUK_4M),   0, 1),
    SignalData("ATAL_4M",     offsetof(P01_ClocksReset, out.ATAL_4M),   0, 1),
    SignalData("ARYF_4M",     offsetof(P01_ClocksReset, out.ARYF_4M),   0, 1),
    SignalData("APUV_4M",     offsetof(P01_ClocksReset, out.APUV_4M),   0, 1),

    // driven by off-sheet CYBO_4M
    //SignalData("CERY_2M",     offsetof(P01_ClocksReset, out.CERY_2M),   0, 1),

    SignalData("BAVU_1M",     offsetof(P01_ClocksReset, out.BAVU_1M),   0, 1),
    SignalData("CPU_RD_SYNC", offsetof(P01_ClocksReset, out.CPU_RD_SYNC), 0, 1),

    SignalData("-----DIV-----"),
    SignalData("UKUP_00",     offsetof(P01_ClocksReset, UKUP_00),          0, 1),
    SignalData("UFOR_01",     offsetof(P01_ClocksReset, UFOR_01),          0, 1),
    SignalData("UNER_02",     offsetof(P01_ClocksReset, UNER_02),          0, 1),
    SignalData("TERO_03",     offsetof(P01_ClocksReset, TERO_03),          0, 1),
    SignalData("UNYK_04",     offsetof(P01_ClocksReset, UNYK_04),          0, 1),
    SignalData("TAMA_05",     offsetof(P01_ClocksReset, TAMA_05),          0, 1),
    SignalData("UGOT_06",     offsetof(P01_ClocksReset, UGOT_06),          0, 1),
    SignalData("TULU_07",     offsetof(P01_ClocksReset, TULU_07),          0, 1),
    SignalData("TUGO_08",     offsetof(P01_ClocksReset, TUGO_08),          0, 1),
    SignalData("TOFE_09",     offsetof(P01_ClocksReset, TOFE_09),          0, 1),
    SignalData("TERU_10",     offsetof(P01_ClocksReset, TERU_10),          0, 1),
    SignalData("SOLA_11",     offsetof(P01_ClocksReset, SOLA_11),          0, 1),
    SignalData("SUBU_12",     offsetof(P01_ClocksReset, SUBU_12),          0, 1),
    SignalData("TEKA_13",     offsetof(P01_ClocksReset, TEKA_13),          0, 1),
    SignalData("UKET_14",     offsetof(P01_ClocksReset, UKET_14),          0, 1),
    SignalData("UPOF_15",     offsetof(P01_ClocksReset, UPOF_15),          0, 1),

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

void P01_ClocksReset::tick(const CpuSignals& cpu,
                           const ChipSignals& chip,
                           const P01_ClocksReset& a, const TristateBus& busA,
                           const P01_ClocksReset& b, const TristateBus& busB,
                           P01_ClocksReset& c,       TristateBus& busC) {
  (void)busA;
  (void)busB;

  //----------
  // Reset tree

  c.DULA = not(b.out.RESET2);
  c.XEBE = not(b.out.RESET7);
  c.out.RESET7n = b.XEBE;
  c.UCOB = not(chip.CLKIN_A);
  c.out.CLKIN_An = b.UCOB;
  c.CUNU = not(b.DULA);
  c.out.RESET6 = b.CUNU;
  c.XODO = and(b.in.FF40_D7, b.XEBE);
  c.XORE = not(b.CUNU);
  c.out.RESET7 = b.XORE;
  c.WESY = not(b.XORE);
  c.out.RESET9 = b.WESY;
  c.WALU = not(b.XORE);
  c.out.RESET8 = b.WALU;
  c.XAPO = not(b.XODO);
  c.out.RESET_VIDEO = b.XAPO;

  //----------
  // Clock deglitcher + clock tree

  c.ARYS = not(chip.CLKIN_B);

  // clock deglitcher
  c.ANOS = nand(chip.CLKIN_B, b.AVET);
  c.AVET = nand(b.ANOS, b.ARYS);

  c.ATAL = not(b.AVET);
  c.out.ATAL_4M = b.ATAL;
  c.AZOF = not(b.ATAL);

  c.ATAG = not(b.AZOF);
  c.ZAXY = not(b.AZOF);

  c.AMUK = not(b.ATAG);
  c.out.AMUK_4M = b.AMUK;
  c.ZEME = not(b.ZAXY);
  c.out.CLK1 = b.ZEME;

  c.APUV = not(b.AMUK);
  c.out.APUV_4M = b.APUV;
  c.ARYF = not(b.AMUK);
  c.out.ARYF_4M = b.ARYF;
  c.ALET = not(b.ZEME);
  c.out.CLK2 = b.ALET;

  //----------
  // random clock

  c.BELA = not(b.in.APU_RESET);

  if (!b.BELA) {
    c.CERY = 0;
  }
  else if (a.in.CYBO_4M && !b.in.CYBO_4M) {
    c.CERY = !b.CERY;
  }
  else {
    c.CERY = b.CERY;
  }

  c.out.CERY_2M = b.CERY;

  //----------
  // Phase generator. These registers tick on _BOTH_EDGES_ of the master clock.

  if (!chip.T1nT2n) {
    c.AFUR = 0;
    c.ALEF = 0;
    c.APUK = 0;
    c.ADYK = 0;
  }
  else if (a.out.ATAL_4M != b.out.ATAL_4M) {
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
  c.AREV = nand(cpu.FROM_CPU3, b.AFAS);
  c.APOV = not(b.AREV);

  c.AJAX = not(b.ATYP);
  c.AGUT = unk3(b.AJAX, b.AROV, cpu.FROM_CPU4);
  c.AWOD = or(chip.T1nT2, b.AGUT);
  c.ABUZ = not(b.AWOD);

  c.BUGO = not(b.AFEP);
  c.BATE = nor(b.BUGO, b.AROV, b.in.ABOL);
  c.BASU = not(b.BATE);
  c.BUKE = not(b.BASU);

  //----------
  // Weird logic feeding into BOGA_1M

  c.BAPY = nor(b.in.ABOL, b.AROV, b.ATYP);
  c.BERU = not(b.BAPY);
  c.BUFA = not(b.BERU);
  c.BYLY = not(b.BERU);
  c.BOLO = not(b.BUFA);
  c.BYDA = not(b.BYLY);

  c.NULE = nor(b.ATYP, b.in.ABOL);
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
  c.ATEZ = not(chip.CLKIN_A);
  c.BYJU = nor(b.BELE, b.ATEZ);
  c.BALY = not(b.BYJU);
  c.BOGA = not(b.BALY);

  //----------
  // TO_CPU

  c.BUTY = not(b.in.ABOL);
  c.BUVU = and(b.BUTY, b.BALY);
  c.BYXO = not(b.BUVU);
  c.BEDO = not(b.BYXO);
  c.BOWA = not(b.BEDO);

  //----------
  // Cartridge clock

  c.DOVA = not(b.BEVA);
  c.UVYT = not(b.out.PHI_OUT);

  //----------
  // RESET2 register

  c.UPYF = or(chip.RESET, b.out.CLKIN_An);
  c.TUBO = unk2(b.in.ABOL, b.UPYF);
  c.UNUT = and(b.TUBO, b.UPOF_15);
  c.TABA = or(chip.T1nT2, chip.T1T2n, b.UNUT);
  c.ALYP = not(b.TABA);
  c.AFAR = nor(b.ALYP, chip.RESET);
  c.ASOL = unk2(b.AFAR, chip.RESET);

  c.BOMA = not(b.BOGA);
  c.AFER = tock_pos(a.BOMA, b.BOMA, chip.T1nT2n, b.AFER, b.ASOL);
  c.AVOR = or(b.AFER, b.ASOL);
  c.ALUR = not(b.AVOR);

  //----------
  // FF04 DIV

  c.TAPE = and(b.in.FF04_FF07, cpu.CPU_WR, b.in.TOLA_A1n, b.in.TOVY_A0n);
  c.UFOL = nor(b.out.CLKIN_An, chip.RESET, b.TAPE);
  c.RESET_DIVn = b.UFOL;

  c.UKUP_00 = tock_pos(a.out.BOGA_1M, b.out.BOGA_1M, b.RESET_DIVn, b.UKUP_00, !b.UKUP_00);
  c.UFOR_01 = tock_pos(!a.UKUP_00, !b.UKUP_00, b.RESET_DIVn, b.UFOR_01, !b.UFOR_01);
  c.UNER_02 = tock_pos(!a.UFOR_01, !b.UFOR_01, b.RESET_DIVn, b.UNER_02, !b.UNER_02);
  c.TERO_03 = tock_pos(!a.UNER_02, !b.UNER_02, b.RESET_DIVn, b.TERO_03, !b.TERO_03);
  c.UNYK_04 = tock_pos(!a.TERO_03, !b.TERO_03, b.RESET_DIVn, b.UNYK_04, !b.UNYK_04);
  c.TAMA_05 = tock_pos(!a.UNYK_04, !b.UNYK_04, b.RESET_DIVn, b.TAMA_05, !b.TAMA_05);

  c.UVYN = not(b.TAMA_05);

  c.ULUR = mux2(b.out.BOGA_1M, b.out.TAMA_16K, b.in.FF60_D1);

  c.UGOT_06 = tock_pos( a.ULUR,     b.ULUR,    b.RESET_DIVn, b.UGOT_06, !b.UGOT_06);
  c.TULU_07 = tock_pos(!a.UGOT_06, !b.UGOT_06, b.RESET_DIVn, b.TULU_07, !b.TULU_07);
  c.TUGO_08 = tock_pos(!a.TULU_07, !b.TULU_07, b.RESET_DIVn, b.TUGO_08, !b.TUGO_08);
  c.TOFE_09 = tock_pos(!a.TUGO_08, !b.TUGO_08, b.RESET_DIVn, b.TOFE_09, !b.TOFE_09);
  c.TERU_10 = tock_pos(!a.TOFE_09, !b.TOFE_09, b.RESET_DIVn, b.TERU_10, !b.TERU_10);
  c.SOLA_11 = tock_pos(!a.TERU_10, !b.TERU_10, b.RESET_DIVn, b.SOLA_11, !b.SOLA_11);
  c.SUBU_12 = tock_pos(!a.SOLA_11, !b.SOLA_11, b.RESET_DIVn, b.SUBU_12, !b.SUBU_12);
  c.TEKA_13 = tock_pos(!a.SUBU_12, !b.SUBU_12, b.RESET_DIVn, b.TEKA_13, !b.TEKA_13);
  c.UKET_14 = tock_pos(!a.TEKA_13, !b.TEKA_13, b.RESET_DIVn, b.UKET_14, !b.UKET_14);
  c.UPOF_15 = tock_pos(!a.UKET_14, !b.UKET_14, b.RESET_DIVn, b.UPOF_15, !b.UPOF_15);

  c.UMEK = not(b.UGOT_06);
  c.UREK = not(b.TULU_07);
  c.UTOK = not(b.TUGO_08);
  c.SAPY = not(b.TOFE_09);
  c.UMER = not(b.TERU_10);
  c.RAVE = not(b.SOLA_11);
  c.RYSO = not(b.SUBU_12);
  c.UDOR = not(b.TEKA_13);

  c.TAGY = and(b.in.FF04_FF07, cpu.CPU_RD, b.in.TOLA_A1n, b.in.TOVY_A0n);

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

  c.ATUS = not(b.in.APU_RESET);
  c.COKE = not(b.in.AJER_2M);
  c.BURE = not(!b.BARA);
  c.BARA = tock_pos( a.COKE,  b.COKE, b.ATUS, b.BARA,  b.UMER);
  c.CARU = tock_pos( a.BURE,  b.BURE, b.ATUS, b.CARU, !b.CARU);
  c.BYLU = tock_pos(!a.CARU, !b.CARU, b.ATUS, b.BYLU, !b.BYLU);

  c.FYNE = not(b.BURE);
  c.CULO = not(!b.CARU);
  c.APEF = not(!b.BYLU);

  c.GALE = mux2(b.out.HAMA_512Kn, b.FYNE, b.in.FERO_Q);
  c.BEZE = mux2(b.out.HAMA_512Kn, b.CULO, b.in.FERO_Q);
  c.BULE = mux2(b.out.HAMA_512Kn, b.APEF, b.in.FERO_Q);

  c.GEXY = not(b.GALE);
  c.COFU = not(b.BEZE);
  c.BARU = not(b.BULE);

  c.HORU = not(b.GEXY);
  c.BUFY = not(b.COFU);
  c.BYFE = not(b.BARU);

  c.BOPO = not(b.in.APU_RESET);
  c.ATYK = tock_pos(a.out.ARYF_4M, b.out.ARYF_4M, b.BOPO, b.ATYK, !b.ATYK);
  c.AVOK = tock_pos(a.ATYK,    b.ATYK,    b.BOPO, b.AVOK, !b.AVOK);
  c.BAVU = not(b.AVOK);
  c.JESO = tock_pos(a.BAVU, b.BAVU, b.in.APU_RESET5n, b.JESO, !b.JESO);

  c.HAMA = not(!b.JESO);

  //----------

  c.out.BOGA_1M = b.BOGA;
  c.out.CLKIN_An = b.UCOB;
  c.out.RESET2 = b.ALUR;
  c.AFAS = b.AFAS;
  c.out.CPU_RD_SYNC = b.APOV;
  c.ABUZ = b.ABUZ;
  c.BUKE = b.BUKE;

  c.out.TAMA_16K  = !b.TAMA_05;
  c.out.CLK_256K  = b.UFOR_01;
  c.out.CLK_64K   = b.TERO_03;
  c.out.CLK_16K   = b.UVYN;

  if (b.TAGY) {
    busC.D0 = b.TAWU;
    busC.D1 = b.TAKU;
    busC.D2 = b.TEMU;
    busC.D3 = b.TUSE;
    busC.D4 = b.UPUG;
    busC.D5 = b.SEPU;
    busC.D6 = b.SAWA;
    busC.D7 = b.TATU;
  }

  c.out.CERY_2M = b.CERY;
  c.out.FF04_D0n = b.UMEK;
  c.out.FF04_D1n = b.UREK;
  c.out.HORU_512 = b.HORU;
  c.out.BUFY_256 = b.BUFY;
  c.out.BYFE_128 = b.BYFE;
  c.out.BAVU_1M = b.BAVU;
  c.out.JESO_512K = b.JESO;
  c.out.HAMA_512Kn = b.HAMA;
  c.out.RESET6 = b.CUNU;
  c.out.RESET7 = b.XORE;
  c.out.RESET7n = b.XEBE;
  c.out.RESET9 = b.WESY;
  c.out.RESET8 = b.WALU;
  c.out.RESET_VIDEO = b.XAPO;
  c.out.CLK1 = b.ZEME;
  c.out.CLK2 = b.ALET;
  c.out.ATAL_4M = b.ATAL;
  c.out.AMUK_4M = b.AMUK;
  c.out.ARYF_4M = b.ARYF;
  c.out.APUV_4M = b.APUV;
  c.out.TO_CPU = b.BOWA;
  c.out.PHI_OUT = b.BEVA;
  c.out.PHIn = b.DOVA;
  c.out.PHI_OUTn = b.UVYT;
}

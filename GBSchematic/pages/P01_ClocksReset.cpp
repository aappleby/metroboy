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

    SignalData("AREV", offsetof(P01_ClocksReset, AREV)),

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

/*
bool TO_CPU;         // PORTD_01: <- P01.BOWA, this is a 1 mhz clock
bool BEDO;           // PORTD_02: <- P01.BEDO, this is a 1 mhz clock
bool BEKO;           // PORTD_03: <- P01.BEKO, connection not indicated on P01. Clock but not sure what phase
bool PHI_OUT;        // PORTD_04: <- P01.BUDE, PHI_A, PHI_OUT. More clocks
bool FROM_CPU5;      // PORTD_05: -> FROM_CPU5 - controls driving the external data pins onto the internal data bus and other stuff. is this actually a clock, or like OE?
bool BUKE;           // PORTD_06: <- P01.BUKE, 4 mhz clock
bool BOMA;           // PORTD_07: <- P01.BOMA, connection not indicated on P01, 1 mhz clock
bool BOGA1MHZ;       // PORTD_08: <- P01.BOGA1MHZ, clock
*/

//-----------------------------------------------------------------------------

void P01_ClocksReset::tick(const Gameboy& ga, const Gameboy& gb, Gameboy& gc2) {
  const P01_ClocksReset& pa = ga.p01;
  const P01_ClocksReset& pb = gb.p01;
  P01_ClocksReset& pc2 = gc2.p01;

  //----------
  // Reset tree

  pc2.DULA = not(gb.RESET2);
  pc2.XEBE = not(gb.RESET7);
  pc2.UCOB = not(gb.chip.CLKIN_A);
  pc2.CUNU = not(pb.DULA);
  pc2.XODO = and(gb.FF40_D7, pb.XEBE);
  pc2.XORE = not(pb.CUNU);
  pc2.WESY = not(pb.XORE);
  pc2.WALU = not(pb.XORE);
  pc2.XAPO = not(pb.XODO);

  gc2.CLKIN_An = pb.UCOB;
  gc2.RESET6   = pb.CUNU;
  gc2.RESET7   = pb.XORE;
  gc2.RESET7n  = pb.XEBE;
  gc2.RESET8   = pb.WALU;
  gc2.RESET9   = pb.WESY;

  gc2.RESET_VIDEO = pb.XAPO;

  //----------
  // Clock deglitcher + clock tree

  pc2.ARYS = not(gb.chip.CLKIN_B);

  // clock deglitcher
  pc2.ANOS = nand(gb.chip.CLKIN_B, pb.AVET);
  pc2.AVET = nand(pb.ANOS, pb.ARYS);

  pc2.ATAL = not(pb.AVET);
  pc2.AZOF = not(pb.ATAL);

  pc2.ATAG = not(pb.AZOF);
  pc2.ZAXY = not(pb.AZOF);

  pc2.AMUK = not(pb.ATAG);
  pc2.ZEME = not(pb.ZAXY);

  pc2.APUV = not(pb.AMUK);
  pc2.ARYF = not(pb.AMUK);
  pc2.ALET = not(pb.ZEME);
  pc2.LAPE = not(pb.ALET);
  pc2.TAVA = not(pb.LAPE);

  gc2.AMUK_4M = pb.AMUK;
  gc2.APUV_4M = pb.APUV;
  gc2.ARYF_4M = pb.ARYF;
  gc2.ATAL_4M = pb.ATAL;
  gc2.CLK1    = pb.ZEME;
  gc2.CLK2    = pb.ALET;

  //----------
  // random clock

  pc2.BELA = not(gb.APU_RESET);
  pc2.CYBO = not(pb.AMUK);
  pc2.CERY = tock_pos(pa.CYBO, pb.CYBO, pb.BELA, pb.CERY, !pb.CERY);
  gc2.CERY_2M     = pb.CERY;

  //----------
  // Phase generator. These registers tick on _BOTH_EDGES_ of the master clock.

  pc2.AFUR = tock_duo(ga.ATAL_4M, gb.ATAL_4M, gb.T1nT2n, pb.AFUR, !pb.ADYK);
  pc2.ALEF = tock_duo(ga.ATAL_4M, gb.ATAL_4M, gb.T1nT2n, pb.ALEF, pb.AFUR);
  pc2.APUK = tock_duo(ga.ATAL_4M, gb.ATAL_4M, gb.T1nT2n, pb.APUK, pb.ALEF);
  pc2.ADYK = tock_duo(ga.ATAL_4M, gb.ATAL_4M, gb.T1nT2n, pb.ADYK, pb.APUK);

  pc2.ADAR = not(pb.ADYK);
  pc2.ATYP = not(pb.AFUR);
  pc2.AFEP = not(pb.ALEF);
  pc2.AROV = not(pb.APUK);

  pc2.AFAS = nor(pb.ADAR, pb.ATYP);
  pc2.AREV = nand(gb.cpu.FROM_CPU3, pb.AFAS);
  pc2.APOV = not(pb.AREV);

  pc2.AJAX = not(pb.ATYP);
  
  // AGUT = abcd__gh if FROM_CPU4 is high
  pc2.AGUT =  and(or(pb.AJAX, pb.AROV), gb.cpu.FROM_CPU4);

  pc2.AWOD = or(gb.T1nT2, pb.AGUT);
  pc2.ABUZ = not(pb.AWOD);

  pc2.BUGO = not(pb.AFEP);
  pc2.BATE = nor(pb.BUGO, pb.AROV, gb.ABOL);
  pc2.BASU = not(pb.BATE);
  pc2.BUKE = not(pb.BASU);

  gc2.CPU_WR_SYNC = pb.APOV;
  gc2.ABUZ = pb.ABUZ;

  //----------
  // Weird logic feeding into BOGA_1M

  pc2.BAPY = nor(gb.ABOL, pb.AROV, pb.ATYP);
  pc2.BERU = not(pb.BAPY);
  pc2.BUFA = not(pb.BERU);
  pc2.BYLY = not(pb.BERU);
  pc2.BOLO = not(pb.BUFA);
  pc2.BYDA = not(pb.BYLY);

  pc2.NULE = nor(pb.ATYP, gb.ABOL);
  pc2.BYRY = not(pb.NULE);
  pc2.BUDE = not(pb.BYRY);
  pc2.BEVA = not(pb.BYRY);
  pc2.BEKO = not(pb.BUDE);
  pc2.BAVY = not(pb.BEVA);

  pc2.BEJA = nand(pb.BOLO, pb.BYDA, pb.BEKO, pb.BAVY);
  pc2.BANE = not(pb.BEJA);
  pc2.BELO = not(pb.BANE);
  pc2.BAZE = not(pb.BELO);
  pc2.BUTO = nand(pb.AFEP, pb.ATYP, pb.BAZE);
  pc2.BELE = not(pb.BUTO);
  pc2.ATEZ = not(gb.chip.CLKIN_A);
  pc2.BYJU = nor(pb.BELE, pb.ATEZ);
  pc2.BALY = not(pb.BYJU);
  pc2.BOGA = not(pb.BALY);

  gc2.BOGA_1M = pb.BOGA;

  //----------
  // TO_CPU

  pc2.BUTY = not(gb.ABOL);
  pc2.BUVU = and(pb.BUTY, pb.BALY);
  pc2.BYXO = not(pb.BUVU);
  pc2.BEDO = not(pb.BYXO);
  pc2.BOWA = not(pb.BEDO);

  gc2.BEDO   = pb.BEDO;
  gc2.TO_CPU = pb.BOWA;

  //----------
  // Cartridge clock

  pc2.DOVA = not(pb.BEVA);
  pc2.UVYT = not(gb.PHI_OUT);

  gc2.PHI_OUT     = pb.BEVA;
  gc2.PHI_OUTn    = pb.UVYT;
  gc2.PHIn        = pb.DOVA;

  //----------
  // RESET2 register

  pc2.UPYF = or(gb.chip.RST, gb.CLKIN_An);
  pc2.TUBO = unk2(gb.ABOL, pb.UPYF);
  pc2.UNUT = and(pb.TUBO, pb.UPOF_15);
  pc2.TABA = or(gb.T1nT2, gb.T1T2n, pb.UNUT);
  pc2.ALYP = not(pb.TABA);
  pc2.AFAR = nor(pb.ALYP, gb.chip.RST);
  pc2.ASOL = unk2(pb.AFAR, gb.chip.RST);

  pc2.BOMA = not(pb.BOGA);
  pc2.AFER = tock_pos(pa.BOMA, pb.BOMA, gb.T1nT2n, pb.AFER, pb.ASOL);
  pc2.AVOR = or(pb.AFER, pb.ASOL);
  pc2.ALUR = not(pb.AVOR);

  gc2.RESET2 = pb.ALUR;
  gc2.cpu.TABA = pb.TABA;

  //----------
  // FF04 DIV

  pc2.TAPE = and(gb.FF04_FF07, gb.p07.CPU_WR, gb.TOLA_A1n, gb.TOVY_A0n);
  pc2.UFOL = nor(gb.CLKIN_An, gb.chip.RST, pb.TAPE);
  pc2.RESET_DIVn = pb.UFOL;

  pc2.UKUP_00 = tock_pos(ga.BOGA_1M,  gb.BOGA_1M,  pb.RESET_DIVn, pb.UKUP_00, !pb.UKUP_00);
  pc2.UFOR_01 = tock_pos(!pa.UKUP_00, !pb.UKUP_00, pb.RESET_DIVn, pb.UFOR_01, !pb.UFOR_01);
  pc2.UNER_02 = tock_pos(!pa.UFOR_01, !pb.UFOR_01, pb.RESET_DIVn, pb.UNER_02, !pb.UNER_02);
  pc2.TERO_03 = tock_pos(!pa.UNER_02, !pb.UNER_02, pb.RESET_DIVn, pb.TERO_03, !pb.TERO_03);
  pc2.UNYK_04 = tock_pos(!pa.TERO_03, !pb.TERO_03, pb.RESET_DIVn, pb.UNYK_04, !pb.UNYK_04);
  pc2.TAMA_05 = tock_pos(!pa.UNYK_04, !pb.UNYK_04, pb.RESET_DIVn, pb.TAMA_05, !pb.TAMA_05);

  gc2.CLK_256K    = pb.UFOR_01;
  gc2.CLK_64K     = pb.TERO_03;
  gc2.TAMA_16K    = !pb.TAMA_05;

  pc2.UVYN = not(pb.TAMA_05);
  pc2.ULUR = mux2(gb.BOGA_1M, gb.TAMA_16K, gb.p07.FF60_D1);

  gc2.CLK_16K     = pb.UVYN;

  pc2.UGOT_06 = tock_pos( pa.ULUR,     pb.ULUR,    pb.RESET_DIVn, pb.UGOT_06, !pb.UGOT_06);
  pc2.TULU_07 = tock_pos(!pa.UGOT_06, !pb.UGOT_06, pb.RESET_DIVn, pb.TULU_07, !pb.TULU_07);
  pc2.TUGO_08 = tock_pos(!pa.TULU_07, !pb.TULU_07, pb.RESET_DIVn, pb.TUGO_08, !pb.TUGO_08);
  pc2.TOFE_09 = tock_pos(!pa.TUGO_08, !pb.TUGO_08, pb.RESET_DIVn, pb.TOFE_09, !pb.TOFE_09);
  pc2.TERU_10 = tock_pos(!pa.TOFE_09, !pb.TOFE_09, pb.RESET_DIVn, pb.TERU_10, !pb.TERU_10);
  pc2.SOLA_11 = tock_pos(!pa.TERU_10, !pb.TERU_10, pb.RESET_DIVn, pb.SOLA_11, !pb.SOLA_11);
  pc2.SUBU_12 = tock_pos(!pa.SOLA_11, !pb.SOLA_11, pb.RESET_DIVn, pb.SUBU_12, !pb.SUBU_12);
  pc2.TEKA_13 = tock_pos(!pa.SUBU_12, !pb.SUBU_12, pb.RESET_DIVn, pb.TEKA_13, !pb.TEKA_13);
  pc2.UKET_14 = tock_pos(!pa.TEKA_13, !pb.TEKA_13, pb.RESET_DIVn, pb.UKET_14, !pb.UKET_14);
  pc2.UPOF_15 = tock_pos(!pa.UKET_14, !pb.UKET_14, pb.RESET_DIVn, pb.UPOF_15, !pb.UPOF_15);

  pc2.UMEK = not(pb.UGOT_06);
  pc2.UREK = not(pb.TULU_07);
  pc2.UTOK = not(pb.TUGO_08);
  pc2.SAPY = not(pb.TOFE_09);
  pc2.UMER = not(pb.TERU_10);
  pc2.RAVE = not(pb.SOLA_11);
  pc2.RYSO = not(pb.SUBU_12);
  pc2.UDOR = not(pb.TEKA_13);

  pc2.TAGY = and(gb.FF04_FF07, gb.p07.CPU_RD, gb.TOLA_A1n, gb.TOVY_A0n);
  pc2.TAWU = not(pb.UMEK);
  pc2.TAKU = not(pb.UREK);
  pc2.TEMU = not(pb.UTOK);
  pc2.TUSE = not(pb.SAPY);
  pc2.UPUG = not(pb.UMER);
  pc2.SEPU = not(pb.RAVE);
  pc2.SAWA = not(pb.RYSO);
  pc2.TATU = not(pb.UDOR);

  if (pb.TAGY) {
    gc2.D0 = pb.TAWU;
    gc2.D1 = pb.TAKU;
    gc2.D2 = pb.TEMU;
    gc2.D3 = pb.TUSE;
    gc2.D4 = pb.UPUG;
    gc2.D5 = pb.SEPU;
    gc2.D6 = pb.SAWA;
    gc2.D7 = pb.TATU;
  }

  gc2.FF04_D0n = pb.UMEK;
  gc2.FF04_D1n = pb.UREK;

  //----------
  // Clock dividers for APU

  pc2.ATUS = not(gb.APU_RESET);
  pc2.COKE = not(gb.AJER_2M);
  pc2.BURE = not(!pb.BARA);
  pc2.BARA = tock_pos( pa.COKE,  pb.COKE, pb.ATUS, pb.BARA,  pb.UMER);
  pc2.CARU = tock_pos( pa.BURE,  pb.BURE, pb.ATUS, pb.CARU, !pb.CARU);
  pc2.BYLU = tock_pos(!pa.CARU, !pb.CARU, pb.ATUS, pb.BYLU, !pb.BYLU);

  pc2.FYNE = not(pb.BURE);
  pc2.CULO = not(!pb.CARU);
  pc2.APEF = not(!pb.BYLU);

  pc2.GALE = mux2(gb.HAMA_512Kn, pb.FYNE, gb.FERO_Q);
  pc2.BEZE = mux2(gb.HAMA_512Kn, pb.CULO, gb.FERO_Q);
  pc2.BULE = mux2(gb.HAMA_512Kn, pb.APEF, gb.FERO_Q);

  pc2.GEXY = not(pb.GALE);
  pc2.COFU = not(pb.BEZE);
  pc2.BARU = not(pb.BULE);

  pc2.HORU = not(pb.GEXY);
  pc2.BUFY = not(pb.COFU);
  pc2.BYFE = not(pb.BARU);

  pc2.BOPO = not(gb.APU_RESET);
  pc2.BAVU = not(pb.AVOK);

  pc2.ATYK = tock_pos(ga.ARYF_4M, gb.ARYF_4M, pb.BOPO,        pb.ATYK, !pb.ATYK);
  pc2.AVOK = tock_pos(pa.ATYK,    pb.ATYK,    pb.BOPO,        pb.AVOK, !pb.AVOK);
  pc2.JESO = tock_pos(pa.BAVU,    pb.BAVU,    gb.APU_RESET5n, pb.JESO, !pb.JESO);

  pc2.HAMA = not(!pb.JESO);

  gc2.BAVU_1M     = pb.BAVU;
  gc2.BUFY_256    = pb.BUFY;
  gc2.BYFE_128    = pb.BYFE;
  gc2.HAMA_512Kn  = pb.HAMA;
  gc2.HORU_512    = pb.HORU;
  gc2.JESO_512K   = pb.JESO;
}

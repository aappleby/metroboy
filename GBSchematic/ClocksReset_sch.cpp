// This file should contain the schematics as directly translated to C, no modifications or simplifications

#include "Schematics.h"
#include "CpuBus.h"
#include "MemBus.h"
#include "ExtBus.h"
#include "PpuRegs.h"
#include "Clocks.h"
#include "AddressDecoder.h"
#include "Debug.h"

//----------
// inputs

extern bool TOLA_A1n;
extern bool TOVY_A0n;

extern bool FF60_D1;
extern bool FERO_Q;

//----------
// outputs

bool PHI_OUTn; // same as PHI_N?
bool PHIn;

bool FF04_D0n;
bool FF04_D1n;

bool WESY;
bool ABUZ;
bool AFAS;
bool BEDO;
bool BUKE;

//----------
// registers

reg ADYK;
reg AFUR;
reg ALEF;
reg APUK;
reg AFER;
reg CERY;

reg TAMA;
reg UNYK;
reg TERO;
reg UNER;
reg UFOR;
reg UKUP;

reg BARA;
reg CARU;
reg BYLU;

reg ATYK;
reg AVOK;
reg JESO;

// FF04, DIV
reg UGOT;
reg TULU;
reg TUGO;
reg TOFE;
reg TERU;
reg SOLA;
reg SUBU;
reg TEKA;
reg UKET;
reg UPOF;

//-----------------------------------------------------------------------------

void tick_clocks_reset() {

  //----------
  // FF04 DIV

  bool UKUP_Q = UKUP.flip(clk.BOGA1MHZ, rst.RESET_DIVn);
  bool UFOR_Q = UFOR.flip(!UKUP_Q,  rst.RESET_DIVn);
  bool UNER_Q = UNER.flip(!UFOR_Q,  rst.RESET_DIVn);
  bool TERO_Q = TERO.flip(!UNER_Q,  rst.RESET_DIVn);
  bool UNYK_Q = UNYK.flip(!TERO_Q,  rst.RESET_DIVn);
  bool TAMA_Q = TAMA.flip(!UNYK_Q,  rst.RESET_DIVn);

  bool UVYN = not(TAMA_Q);

  clk.CLK_16k = UVYN;
  clk.CLK_64k = TERO_Q;
  clk.CLK_256k = UFOR_Q;

  bool ULUR = mux2(clk.BOGA1MHZ, !TAMA_Q, FF60_D1);

  bool UGOT_Q = UGOT.flip(ULUR,    rst.RESET_DIVn); // FIXME double negative?
  bool TULU_Q = TULU.flip(!UGOT_Q, rst.RESET_DIVn);
  bool TUGO_Q = TUGO.flip(!TULU_Q, rst.RESET_DIVn);
  bool TOFE_Q = TOFE.flip(!TUGO_Q, rst.RESET_DIVn);
  bool TERU_Q = TERU.flip(!TOFE_Q, rst.RESET_DIVn);
  bool SOLA_Q = SOLA.flip(!TERU_Q, rst.RESET_DIVn);
  bool SUBU_Q = SUBU.flip(!SOLA_Q, rst.RESET_DIVn);
  bool TEKA_Q = TEKA.flip(!SUBU_Q, rst.RESET_DIVn);
  bool UKET_Q = UKET.flip(!TEKA_Q, rst.RESET_DIVn);
  bool UPOF_Q = UPOF.flip(!UKET_Q, rst.RESET_DIVn);

  bool UMEK = not(UGOT_Q);
  bool UREK = not(TULU_Q);
  bool UTOK = not(TUGO_Q);
  bool SAPY = not(TOFE_Q);
  bool UMER = not(TERU_Q);
  bool RAVE = not(SOLA_Q);
  bool RYSO = not(SUBU_Q);
  bool UDOR = not(TEKA_Q);

  FF04_D0n = UMEK;
  FF04_D1n = UREK;

  bool TAWU = not(UMEK);
  bool TAKU = not(UREK);
  bool TEMU = not(UTOK);
  bool TUSE = not(SAPY);
  bool UPUG = not(UMER);
  bool SEPU = not(RAVE);
  bool SAWA = not(RYSO);
  bool TATU = not(UDOR);

  bool TAGY = and(dec.FF04_FF07, cpu.CPU_RD, TOLA_A1n, TOVY_A0n);
  if (TAGY) {
    mem.D0 = TAWU;
    mem.D1 = TAKU;
    mem.D2 = TEMU;
    mem.D3 = TUSE;
    // these two are switched on the schematic, def not right...
    mem.D4 = UPUG;
    mem.D5 = SEPU;
    mem.D6 = SAWA;
    mem.D7 = TATU;
  }

  //----------

  wire ADYK_Q = ADYK.q();
  wire AFUR_Q = AFUR.q();
  wire ALEF_Q = ALEF.q();
  wire APUK_Q = APUK.q();

  wire AFER_Q = AFER.q();
  wire CERY_Q = CERY.q();

  bool UPYF = or(ext.RESET, clk.CLKIN_An);
  bool TUBO = unk2(clk.ABOL_1MHZ, UPYF);
  bool UNUT = and(TUBO, UPOF_Q);
  bool TABA = or(dbg.T1nT2, dbg.T1nT2n, UNUT);

  // combi clock deglitcher, skipping it
  //bool ARYS = not(CLKIN_B);
  //bool ANOS = nand(CLKIN_B, AVET);
  //bool AVET = nand(ANOS, ARYS);
  bool AVET = ext.CLKIN_B;

  bool ATAL = not(AVET);
  bool ATAL_4MHZ = ATAL;
  bool AZOF = not(ATAL);
  bool ATAG = not(AZOF);
  bool ZAXY = not(AZOF);
  bool AMUK = not(ATAG);
  //bool AMUK_4MHZ = AMUK;
  bool ZEME = not(ZAXY);
  bool APUV = not(AMUK);
  clk.APUV_4MHZ = APUV;
  bool ARYF = not(AMUK);
  bool ARYF_4MHZ = ARYF;
  clk.CLK1 = ZEME;
  bool ALET = not(ZEME);
  clk.CLK2 = ALET;

  bool BELA = not(rst.APU_RESET);
  clk.CERY_2MHZ = CERY_Q;

  // comment about clock rates on schematic is wrong
  bool ADAR = not(ADYK_Q);
  bool ATYP = not(AFUR_Q);
  bool AFEP = not(ALEF_Q);
  bool AROV = not(APUK_Q);
  AFAS = nor(ADAR, ATYP);
  bool AJAX = not(ATYP);
  bool BUGO = not(AFEP);
  bool AREV = nand(cpu.FROM_CPU3, AFAS);
  bool AGUT = unk3(AJAX, AROV, cpu.FROM_CPU4);
  bool BATE = nor(BUGO, AROV, clk.ABOL_1MHZ);
  bool APOV = not(AREV);
  bool AWOD = or(dbg.T1nT2, AGUT);
  bool BASU = not(BATE);
  ABUZ = not(AWOD);
  BUKE = not(BASU);

  cpu.CPU_RD_SYNC = APOV;

  bool BAPY = nor(clk.ABOL_1MHZ, AROV, ATYP);
  bool NULE = nor(ATYP, clk.ABOL_1MHZ);
  bool BERU = not(BAPY);
  bool BYRY = not(NULE);
  bool BUFA = not(BERU);
  //bool BYLY = not(BERU);
  bool BUDE = not(BYRY);
  //bool BEVA = not(BYRY);
  bool BOLO = not(BUFA);
  bool BYDA = not(BUFA);
  bool BEKO = not(BUDE);
  bool BAVY = not(BUDE);
  bool BEJA = nand(BOLO, BYDA, BEKO, BAVY);
  bool BANE = not(BEJA);
  bool BELO = not(BANE);
  bool BAZE = not(BELO);
  bool BUTO = nand(AFEP, ATYP, BAZE);
  bool BELE = not(BUTO);
  bool ATEZ = not(ext.CLKIN_A);
  bool BYJU = nor(BELE, ATEZ);
  bool ALYP = not(TABA);
  bool BUTY = not(clk.ABOL_1MHZ);
  bool BALY = not(BYJU);
  bool AFAR = nor(ALYP, ext.RESET);
  bool BUVU = and(BUTY, BALY);
  bool BOGA = not(BALY);

  clk.BOGA1MHZ = BOGA;

  bool ASOL = unk2(AFAR, ext.RESET);
  bool BOMA = not(BOGA);
  bool BYXO = not(BUVU);
  BEDO = not(BYXO);
  bool BOWA = not(BEDO);
  cpu.TO_CPU = BOWA;
  
  bool AVOR = or(AFER_Q, ASOL);
  bool ALUR = not(AVOR);

  bool UVYT = not(ext.PHI_OUT);
  PHI_OUTn = UVYT;

  rst.RESET2 = ALUR;

  bool DOVA = not(BUDE);
  PHIn = DOVA;

  bool TAPE = and(dec.FF04_FF07, cpu.CPU_WR, TOLA_A1n, TOVY_A0n);

  bool UFOL = nor(clk.CLKIN_An, ext.RESET, TAPE);
  rst.RESET_DIVn = UFOL;

  bool DULA = not(rst.RESET2);
  bool XEBE = not(rst.RESET7);
  bool UCOB = not(ext.CLKIN_A);
  bool CUNU = not(DULA);
  bool XODO = and(ppu.FF40_D7, XEBE);

  rst.RESET7n = XEBE;
  clk.CLKIN_An = UCOB;

  bool XORE = not(CUNU);

  rst.RESET7 = XORE;

  WESY = not(XORE);
  bool WALU = not(XORE);

  rst.RESET6 = CUNU;
  bool XAPO = not(XODO);

  rst.RESET_VIDEO = XAPO;

  rst.RESET9 = WESY;
  rst.RESET8 = WALU;

  bool BARA_Q = BARA.q();
  bool CARU_Q = CARU.q();
  bool BYLU_Q = BYLU.q();

  bool ATUS = not(rst.APU_RESET);
  bool COKE = not(clk.AJER_2MHZ);
  bool BURE = not(!BARA_Q);
  bool FYNE = not(BURE);
  bool CULO = not(!CARU_Q);
  bool APEF = not(!BYLU_Q);

  bool GALE = mux2(clk.HAMA_512Kn, FYNE, FERO_Q);
  bool BEZE = mux2(clk.HAMA_512Kn, CULO, FERO_Q);
  bool BULE = mux2(clk.HAMA_512Kn, APEF, FERO_Q);

  bool GEXY = not(GALE);
  bool COFU = not(BEZE);
  bool BARU = not(BULE);
  bool HORU = not(GEXY);
  bool BUFY = not(COFU);
  bool BYFE = not(BARU);

  clk.HORU_512 = HORU;
  clk.BUFY_256 = BUFY;
  clk.BYFE_128 = BYFE;

  bool BOPO = not(rst.APU_RESET);
  bool ATYK_Q = ATYK.flip(ARYF_4MHZ, BOPO);
  bool AVOK_Q = AVOK.flip(ATYK_Q, BOPO);
  bool BAVU = not(AVOK_Q);
  bool JESO_Q = JESO.flip(BAVU, rst.APU_RESET5n);
  clk.JESO_512K = JESO_Q;
  clk.BAVU_1MHZ = BAVU;
  bool HAMA = not(JESO_Q);
  clk.HAMA_512Kn = HAMA;

  //----------
  // registers

  ADYK.tock(ATAL_4MHZ, dbg.T1nT2n, APUK_Q);
  AFUR.tock(ATAL_4MHZ, dbg.T1nT2n, !ADYK_Q);
  ALEF.tock(ATAL_4MHZ, dbg.T1nT2n, !AFUR_Q);
  APUK.tock(ATAL_4MHZ, dbg.T1nT2n, !ALEF_Q);

  AFER.tock(BOMA, dbg.T1nT2n, ASOL);
  CERY.flip(clk.CYBO_4MHZ, BELA);

  //----------

  BARA.tock(COKE, ATUS, UMER);
  CARU.flip(BURE, ATUS);
  BYLU.flip(!CARU_Q, ATUS);
}

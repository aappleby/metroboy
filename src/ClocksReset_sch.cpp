// This file should contain the schematics as directly translated to C, no modifications or simplifications

#include "Schematics.h"

//----------
// inputs

extern bool ABOL_1MHZ;
extern bool T1nT2n;
extern bool T1nT2;
extern bool FROM_CPU3;
extern bool FROM_CPU4;
extern bool CLKIN_A;
extern bool RESET;
extern bool FF04_FF07;
extern bool TOLA_A1n;
extern bool TOVY_A0n;
extern bool RESET2;
extern bool FF40_D7;
extern bool RESET7;
extern bool CLKIN_B;
extern bool CYBO_4MHZ;
extern bool APU_RESET;
extern bool AJER_2MHZ;

extern bool FF60_D1;

extern bool FERO_Q;

extern bool APU_RESET5n;

//----------
// outputs

bool PHI_OUT;
bool PHI_OUTn; // same as PHI_N?
bool PHIn;
bool CPU_RD_SYNC;
bool BOGA1MHZ;
bool TO_CPU;
bool RESET2;
bool RESET_DIVn;
bool CLKIN_An;
bool RESET_VIDEO;

bool CLK1;
bool CLK2;
bool CERY_2MHZ;
bool BAVU_1MHZ;
bool JESO_512K;
bool HAMA_512Kn;

bool CLK_16k;
bool CLK_64k;
bool CLK_256k;

bool RESET7n;
bool RESET6;
bool RESET8;
bool RESET9;

bool HORU_512;
bool BUFY_256;
bool BYFE_128;

bool FF04_D0n;
bool FF04_D1n;

bool WESY;
bool ABUZ;
bool AFAS;
bool BEDO;
bool BUKE;

//----------
// registers


static reg ADYK;
static reg AFUR;
static reg ALEF;
static reg APUK;
static reg AFER;
static reg CERY;

static reg TAMA;
static reg UNYK;
static reg TERO;
static reg UNER;
static reg UFOR;
static reg UKUP;

static reg BARA;
static reg CARU;
static reg BYLU;

static reg ATYK;
static reg AVOK;
static reg JESO;

// FF04, DIV
static reg UGOT;
static reg TULU;
static reg TUGO;
static reg TOFE;
static reg TERU;
static reg SOLA;
static reg SUBU;
static reg TEKA;
static reg UKET;
static reg UPOF;

//-----------------------------------------------------------------------------

void tick_clocks_reset() {

  //----------
  // FF04 DIV

  bool UKUP_Q = UKUP.flip(BOGA1MHZ, RESET_DIVn);
  bool UFOR_Q = UFOR.flip(!UKUP_Q,  RESET_DIVn);
  bool UNER_Q = UNER.flip(!UFOR_Q,  RESET_DIVn);
  bool TERO_Q = TERO.flip(!UNER_Q,  RESET_DIVn);
  bool UNYK_Q = UNYK.flip(!TERO_Q,  RESET_DIVn);
  bool TAMA_Q = TAMA.flip(!UNYK_Q,  RESET_DIVn);

  bool UVYN = not(TAMA_Q);

  CLK_16k = UVYN;
  CLK_64k = TERO_Q;
  CLK_256k = UFOR_Q;

  bool ULUR = mux2(BOGA1MHZ, !TAMA_Q, FF60_D1);

  bool UGOT_Q = UGOT.flip(ULUR,    RESET_DIVn); // FIXME double negative?
  bool TULU_Q = TULU.flip(!UGOT_Q, RESET_DIVn);
  bool TUGO_Q = TUGO.flip(!TULU_Q, RESET_DIVn);
  bool TOFE_Q = TOFE.flip(!TUGO_Q, RESET_DIVn);
  bool TERU_Q = TERU.flip(!TOFE_Q, RESET_DIVn);
  bool SOLA_Q = SOLA.flip(!TERU_Q, RESET_DIVn);
  bool SUBU_Q = SUBU.flip(!SOLA_Q, RESET_DIVn);
  bool TEKA_Q = TEKA.flip(!SUBU_Q, RESET_DIVn);
  bool UKET_Q = UKET.flip(!TEKA_Q, RESET_DIVn);
  bool UPOF_Q = UPOF.flip(!UKET_Q, RESET_DIVn);

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

  bool TAGY = and(FF04_FF07, CPU_RD, TOLA_A1n, TOVY_A0n);
  if (TAGY) {
    D0 = TAWU;
    D1 = TAKU;
    D2 = TEMU;
    D3 = TUSE;
    // these two are switched on the schematic, def not right...
    D4 = UPUG;
    D5 = SEPU;
    D6 = SAWA;
    D7 = TATU;
  }

  //----------

  wire ADYK_Q = ADYK.q();
  wire AFUR_Q = AFUR.q();
  wire ALEF_Q = ALEF.q();
  wire APUK_Q = APUK.q();

  wire AFER_Q = AFER.q();
  wire CERY_Q = CERY.q();

  bool UPYF = or(RESET, CLKIN_An);
  bool TUBO = unk2(ABOL_1MHZ, UPYF);
  bool UNUT = and(TUBO, UPOF_Q);
  bool TABA = or(T1nT2, T1nT2n, UNUT);

  // combi clock deglitcher, skipping it
  //bool ARYS = not(CLKIN_B);
  //bool ANOS = nand(CLKIN_B, AVET);
  //bool AVET = nand(ANOS, ARYS);
  bool AVET = CLKIN_B;

  bool ATAL = not(AVET);
  bool ATAL_4MHZ = ATAL;
  bool AZOF = not(ATAL);
  bool ATAG = not(AZOF);
  bool ZAXY = not(AZOF);
  bool AMUK = not(ATAG);
  //bool AMUK_4MHZ = AMUK;
  bool ZEME = not(ZAXY);
  //bool APUV = not(AMUK);
  //bool APUV_4MHZ = APUV;
  bool ARYF = not(AMUK);
  bool ARYF_4MHZ = ARYF;
  CLK1 = ZEME;
  bool ALET = not(ZEME);
  CLK2 = ALET;

  bool BELA = not(APU_RESET);
  CERY_2MHZ = CERY_Q;

  // comment about clock rates on schematic is wrong
  bool ADAR = not(ADYK_Q);
  bool ATYP = not(AFUR_Q);
  bool AFEP = not(ALEF_Q);
  bool AROV = not(APUK_Q);
  AFAS = nor(ADAR, ATYP);
  bool AJAX = not(ATYP);
  bool BUGO = not(AFEP);
  bool AREV = nand(FROM_CPU3, AFAS);
  bool AGUT = unk3(AJAX, AROV, FROM_CPU4);
  bool BATE = nor(BUGO, AROV, ABOL_1MHZ);
  bool APOV = not(AREV);
  bool AWOD = or(T1nT2, AGUT);
  bool BASU = not(BATE);
  ABUZ = not(AWOD);
  BUKE = not(BASU);

  CPU_RD_SYNC = APOV;

  bool BAPY = nor(ABOL_1MHZ, AROV, ATYP);
  bool NULE = nor(ATYP, ABOL_1MHZ);
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
  bool ATEZ = not(CLKIN_A);
  bool BYJU = nor(BELE, ATEZ);
  bool ALYP = not(TABA);
  bool BUTY = not(ABOL_1MHZ);
  bool BALY = not(BYJU);
  bool AFAR = nor(ALYP, RESET);
  bool BUVU = and(BUTY, BALY);
  bool BOGA = not(BALY);

  BOGA1MHZ = BOGA;

  bool ASOL = unk2(AFAR, RESET);
  bool BOMA = not(BOGA);
  bool BYXO = not(BUVU);
  BEDO = not(BYXO);
  bool BOWA = not(BEDO);
  TO_CPU = BOWA;
  
  bool AVOR = or(AFER_Q, ASOL);
  bool ALUR = not(AVOR);

  bool UVYT = not(PHI_OUT);
  PHI_OUTn = UVYT;

  RESET2 = ALUR;

  bool DOVA = not(BUDE);
  PHIn = DOVA;

  bool TAPE = and(FF04_FF07, CPU_WR, TOLA_A1n, TOVY_A0n);

  bool UFOL = nor(CLKIN_An, RESET, TAPE);
  RESET_DIVn = UFOL;

  bool DULA = not(RESET2);
  bool XEBE = not(RESET7);
  bool UCOB = not(CLKIN_A);
  bool CUNU = not(DULA);
  bool XODO = and(FF40_D7, XEBE);

  RESET7n = XEBE;
  CLKIN_An = UCOB;

  bool XORE = not(CUNU);

  RESET7 = XORE;

  WESY = not(XORE);
  bool WALU = not(XORE);

  RESET6 = CUNU;
  bool XAPO = not(XODO);

  RESET_VIDEO = XAPO;

  RESET9 = WESY;
  RESET8 = WALU;

  bool BARA_Q = BARA.q();
  bool CARU_Q = CARU.q();
  bool BYLU_Q = BYLU.q();

  bool ATUS = not(APU_RESET);
  bool COKE = not(AJER_2MHZ);
  bool BURE = not(!BARA_Q);
  bool FYNE = not(BURE);
  bool CULO = not(!CARU_Q);
  bool APEF = not(!BYLU_Q);

  bool GALE = mux2(HAMA_512Kn, FYNE, FERO_Q);
  bool BEZE = mux2(HAMA_512Kn, CULO, FERO_Q);
  bool BULE = mux2(HAMA_512Kn, APEF, FERO_Q);

  bool GEXY = not(GALE);
  bool COFU = not(BEZE);
  bool BARU = not(BULE);
  bool HORU = not(GEXY);
  bool BUFY = not(COFU);
  bool BYFE = not(BARU);

  HORU_512 = HORU;
  BUFY_256 = BUFY;
  BYFE_128 = BYFE;

  bool BOPO = not(APU_RESET);
  bool ATYK_Q = ATYK.flip(ARYF_4MHZ, BOPO);
  bool AVOK_Q = AVOK.flip(ATYK_Q, BOPO);
  bool BAVU = not(AVOK_Q);
  bool JESO_Q = JESO.flip(BAVU, APU_RESET5n);
  JESO_512K = JESO_Q;
  BAVU_1MHZ = BAVU;
  bool HAMA = not(JESO_Q);
  HAMA_512Kn = HAMA;

  //----------
  // registers

  ADYK.tock(ATAL_4MHZ, T1nT2n, APUK_Q);
  AFUR.tock(ATAL_4MHZ, T1nT2n, !ADYK_Q);
  ALEF.tock(ATAL_4MHZ, T1nT2n, !AFUR_Q);
  APUK.tock(ATAL_4MHZ, T1nT2n, !ALEF_Q);

  AFER.tock(BOMA, T1nT2n, ASOL);
  CERY.flip(CYBO_4MHZ, BELA);

  //----------

  BARA.tock(COKE, ATUS, UMER);
  CARU.flip(BURE, ATUS);
  BYLU.flip(!CARU_Q, ATUS);
}

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
bool HAMA_512Kn;

bool CLK_16384;
bool TAMA16384;

bool RESET7n;
bool RESET6;
bool RESET8;
bool RESET9;

bool HORU_512;
bool BUFY_256;
bool BYFE_128;

bool FF04_D0n;
bool FF04_D1n;

//----------
// registers


static bool ADYK_Q, ADYK_CLK;
static bool AFUR_Q, AFUR_CLK;
static bool ALEF_Q, ALEF_CLK;
static bool APUK_Q, APUK_CLK;
static bool AFER_Q, AFER_CLK;
static bool CERY_Q, CERY_CLK;

static bool TAMA_Q, TAMA_CLK;
static bool UNYK_Q, UNYK_CLK;
static bool TERO_Q, TERO_CLK;
static bool UNER_Q, UNER_CLK;
static bool UFOR_Q, UFOR_CLK;
static bool UKUP_Q, UKUP_CLK;

static bool BARA_Q, BARA_CLK;
static bool CARU_Q, CARU_CLK;
static bool BYLU_Q, BYLU_CLK;

static bool ATYK_Q, ATYK_CLK;
static bool AVOK_Q, AVOK_CLK;
static bool JESO_Q, JESO_CLK;


// FF04, DIV

static bool UGOT_Q;
static bool TULU_Q;
static bool TUGO_Q;
static bool TOFE_Q;
static bool TERU_Q;
static bool SOLA_Q;
static bool SUBU_Q;
static bool TEKA_Q;
static bool UKET_Q;
static bool UPOF_Q;
static bool DIV_CLK; // driven from ULUR

//-----------------------------------------------------------------------------

void tick_clocks_reset() {
  bool ULUR = mux2(BOGA1MHZ, TAMA16384, FF60_D1);
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

  bool UVYN = not(TAMA_Q);
  CLK_16384 = UVYN;
  TAMA16384 = !TAMA_Q;

  // comment about clock rates on schematic is wrong
  bool ADAR = not(ADYK_Q);
  bool ATYP = not(AFUR_Q);
  bool AFEP = not(ALEF_Q);
  bool AROV = not(APUK_Q);
  bool AFAS = nor(ADAR, ATYP);
  bool AJAX = not(ATYP);
  bool BUGO = not(AFEP);
  bool AREV = nand(FROM_CPU3, AFAS);
  bool AGUT = unk3(AJAX, AROV, FROM_CPU4);
  bool BATE = nor(BUGO, AROV, ABOL_1MHZ);
  bool APOV = not(AREV);
  bool AWOD = or(T1nT2, AGUT);
  bool BASU = not(BATE);
  bool ABUZ = not(AWOD);
  bool BUKE = not(BASU);

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
  bool BEDO = not(BYXO);
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

  bool WESY = not(XORE);
  bool WALU = not(XORE);

  RESET6 = CUNU;
  bool XAPO = not(XODO);

  RESET_VIDEO = XAPO;

  RESET9 = WESY;
  RESET8 = WALU;

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
  bool BAVU = not(AVOK_Q);
  BAVU_1MHZ = BAVU;
  bool HAMA = not(JESO_Q);
  HAMA_512Kn = HAMA;

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
  // registers

  bool AFER_Q_ = AFER_Q;
  bool ADYK_Q_ = ADYK_Q;
  bool AFUR_Q_ = AFUR_Q;
  bool ALEF_Q_ = ALEF_Q;
  bool APUK_Q_ = APUK_Q;
  bool CERY_Q_ = CERY_Q;

  if (AFER_CLK && !BOMA) AFER_Q_ = ASOL;
  if (ADYK_CLK && !ATAL_4MHZ) ADYK_Q_ = APUK_Q;
  if (AFUR_CLK && !ATAL_4MHZ) AFUR_Q_ = !ADYK_Q;
  if (ALEF_CLK && !ATAL_4MHZ) ALEF_Q_ = AFUR_Q;
  if (APUK_CLK && !ATAL_4MHZ) APUK_Q_ = ALEF_Q;
  if (CERY_CLK && !CYBO_4MHZ) CERY_Q_ = !CERY_Q;

  if (!T1nT2n) AFER_Q_ = 0;
  if (!T1nT2n) ADYK_Q_ = 0;
  if (!T1nT2n) AFUR_Q_ = 0;
  if (!T1nT2n) ALEF_Q_ = 0;
  if (!T1nT2n) APUK_Q_ = 0;
  if (!BELA)    CERY_Q_ = 0;

  AFER_CLK = BOMA;
  ADYK_CLK = ATAL_4MHZ;
  AFUR_CLK = ATAL_4MHZ;
  ALEF_CLK = ATAL_4MHZ;
  APUK_CLK = ATAL_4MHZ;
  CERY_CLK = CYBO_4MHZ;

  AFER_Q = AFER_Q_;
  ADYK_Q = ADYK_Q_;
  AFUR_Q = AFUR_Q_;
  ALEF_Q = ALEF_Q_;
  APUK_Q = APUK_Q_;
  CERY_Q = CERY_Q_;

  //----------
  // registers

  bool TAMA_Q_ = TAMA_Q;
  bool UNYK_Q_ = UNYK_Q;
  bool TERO_Q_ = TERO_Q;
  bool UNER_Q_ = UNER_Q;
  bool UFOR_Q_ = UFOR_Q;
  bool UKUP_Q_ = UKUP_Q;

  if (TAMA_CLK && !!UNYK_Q)  TAMA_Q_ = !TAMA_Q;
  if (UNYK_CLK && !!TERO_Q)  UNYK_Q_ = !UNYK_Q;
  if (TERO_CLK && !!UNER_Q)  TERO_Q_ = !TERO_Q;
  if (UNER_CLK && !!UFOR_Q)  UNER_Q_ = !UNER_Q;
  if (UFOR_CLK && !!UKUP_Q)  UFOR_Q_ = !UFOR_Q;
  if (UKUP_CLK && !BOGA1MHZ) UKUP_Q_ = !UKUP_Q;

  if (!RESET_DIVn) TAMA_Q_ = 0;
  if (!RESET_DIVn) UNYK_Q_ = 0;
  if (!RESET_DIVn) TERO_Q_ = 0;
  if (!RESET_DIVn) UNER_Q_ = 0;
  if (!RESET_DIVn) UFOR_Q_ = 0;
  if (!RESET_DIVn) UKUP_Q_ = 0;

  TAMA_CLK = !UNYK_Q;
  UNYK_CLK = !TERO_Q;
  TERO_CLK = !UNER_Q;
  UNER_CLK = !UFOR_Q;
  UFOR_CLK = !UKUP_Q;
  UKUP_CLK = BOGA1MHZ;

  TAMA_Q = TAMA_Q_;
  UNYK_Q = UNYK_Q_;
  TERO_Q = TERO_Q_;
  UNER_Q = UNER_Q_;
  UFOR_Q = UFOR_Q_;
  UKUP_Q = UKUP_Q_;

  //----------

  bool BARA_Q_ = BARA_Q;
  bool CARU_Q_ = CARU_Q;
  bool BYLU_Q_ = BYLU_Q;

  if (BARA_CLK && !COKE)    BARA_Q_ = UMER;
  if (CARU_CLK && !BURE)    CARU_Q_ = !CARU_Q;
  if (BYLU_CLK && !!CARU_Q) BYLU_Q_ = !BYLU_Q;

  if (!ATUS) BARA_Q_ = 0;
  if (!ATUS) CARU_Q_ = 0;
  if (!ATUS) BYLU_Q_ = 0;

  BARA_CLK = COKE;
  CARU_CLK = BURE;
  BYLU_CLK = !CARU_Q;

  BARA_Q = BARA_Q_;
  CARU_Q = CARU_Q_;
  BYLU_Q = BYLU_Q_;

  //----------

  bool ATYK_Q_ = ATYK_Q;
  bool AVOK_Q_ = AVOK_Q;
  bool JESO_Q_ = JESO_Q;

  if (!BOPO) ATYK_Q_ = 0;
  if (!BOPO) AVOK_Q_ = 0;
  if (!APU_RESET5n) JESO_Q_ = 0;

  if (ATYK_CLK && !ARYF_4MHZ) ATYK_Q_ = !ATYK_Q;
  if (AVOK_CLK && !ATYK_Q) AVOK_Q_ = !AVOK_Q;
  if (JESO_CLK && !BAVU) JESO_Q_ = !JESO_Q;

  ATYK_CLK = ARYF_4MHZ;
  AVOK_CLK = ATYK_Q;
  JESO_CLK = BAVU;

  ATYK_Q = ATYK_Q_;
  AVOK_Q = AVOK_Q_;
  JESO_Q = JESO_Q_;

  //----------
  // div

  if (DIV_CLK && !ULUR) {
    uint8_t da = pack(UGOT_Q, TULU_Q, TUGO_Q, TOFE_Q);
    uint8_t db = pack(TERU_Q, SOLA_Q, SUBU_Q, TEKA_Q);
    uint8_t dc = pack(UKET_Q, UPOF_Q);
    uint8_t dd = 0;

    uint16_t d = pack4(da, db, dc, 0);
    d++;
    unpack4(d, da, db, dc, dd);

    unpack(da, UGOT_Q, TULU_Q, TUGO_Q, TOFE_Q);
    unpack(db, TERU_Q, SOLA_Q, SUBU_Q, TEKA_Q);
    unpack(dc, UKET_Q, UPOF_Q);
  }

  // wait, double-negative? wat?
  if (!RESET_DIVn) {
    UGOT_Q = 0;
    TULU_Q = 0;
    TUGO_Q = 0;
    TOFE_Q = 0;
    TERU_Q = 0;
    SOLA_Q = 0;
    SUBU_Q = 0;
    TEKA_Q = 0;
    UKET_Q = 0;
    UPOF_Q = 0;
  }

  DIV_CLK = ULUR;

  //----------

  (void)BUKE;
  (void)ABUZ;
  (void)APOV;
}














#if 0

#pragma warning(disable : 4100)


void hard_div_tock(int tcycle, Bus bus) {
  const bool FF04_FF07 = (bus.addr & 0xFFFC) == 0xFF04;
  const bool CPU_RD = bus.read;
  const bool CPU_WR = bus.write;

  const bool TOLA_A1N = !(bus.addr & 0x02);
  const bool TOVY_A0N = !(bus.addr & 0x01);

  const bool RESET2 = true;

  const bool _clk_1m  = (tcycle - 1) & 0x002;
  const bool clk_1m   = tcycle & 0x002;
  const bool BOGA1MHZ_NEG = _clk_1m && !clk_1m;

  const bool CLKIN_AN = false; // fixme
  const bool RESET = false;

  /*
  const bool CLKIN_A = false; // fixme;
  const bool FF40_D7 = false; // fixme
  const bool CLKIN_B = false; // fixme
  const bool APU_RESET = false; // fixme
  const bool CYBO_4MHZ_NEG = false; // fixme
  const bool AJER_2MHZ = false; // fixme
  const bool AJER_2MHZ_NEG = false; // fixme
  const bool ABOL_1MHZ = false; // fixme
  const bool ATAL_4MHZ_N = false; // fixme
  const bool FROM_CPU3 = false; // fixme
  const bool FROM_CPU4 = false; // fixme
  const bool BOMA_NEG = false;
  const bool PHI_OUT = false;
  const bool BOGA1MHZ_NEG = false;

  const bool T1nT2n = false; // fixme
  const bool T1nT2 = false; // fixme
  const bool T1T2n = false;
  */

  //----------------------------------------
  // top left of 1_CLOCKS_RESET

  /*
  static bool ADYK = 0;
  static bool AFUR = 0;
  static bool ALEF = 0;
  static bool APUK = 0;

  const bool ADAR = !ADYK; // 2mhz
  const bool ATYP = !AFUR; // 1mhz
  const bool AFEP = !ALEF; // 512khz
  const bool AROV = !APUK; // 256khz

  const bool AFAS = !(ADAR || ATYP);
  const bool AJAX = !ATYP;
  const bool BUGO = !AFEP;

  const bool BAPY = !(ABOL_1MHZ || AROV || ATYP);
  const bool NULE = !(ATYP || ABOL_1MHZ);

  const bool BERU = !BAPY;
  const bool BYRY = !NULE;
  const bool BUFA = !BERU;
  const bool BYLY = !BERU;
  const bool BUDE = !BYRY;
  const bool BEVA = !BYRY;
  const bool BOLO = !BUFA;
  const bool BYDA = !BYLY;
  const bool BEKO = !BUDE;
  const bool BAVY = !BEVA;

  const bool BEJA = !(BOLO && BYDA && BEKO && BAVY);
  const bool DOVA = !BUDE;
  const bool PHI_N = DOVA;

  const bool BANE = !BEJA;
  const bool BELO = !BANE;
  const bool BAZE = !BELO;
  const bool BUTO = !(AFEP && ATYP && BAZE);

  const bool AREV = !(FROM_CPU3 && AFAS);
  const bool APOV = !AREV;
  const bool CPU_RD_SYNC = APOV;

  // UNK3 - don't know what this does
  const bool AGUT = (AJAX ^ AROV ^ FROM_CPU4);

  const bool BATE = !(BUGO || AROV || ABOL_1MHZ);
  const bool AWOD = (T1nT2 || AGUT);
  const bool ABUZ = !AWOD;
  const bool BASU = !BATE;
  const bool BUKE = !BASU;

  const bool UPYF = (RESET || CLKIN_AN);

  // UNK2
  const bool TUBO = ABOL_1MHZ ^ UPYF;
  const bool UPOF = (div & 0x8000);
  const bool UNUT = (UPOF && TUBO);
  const bool TABA = (T1nT2 || T1T2n || UNUT);

  const bool BELE = !BUTO;
  const bool ATEZ = !CLKIN_A;
  const bool BYJU = !(BELE || ATEZ);
  const bool ALYP = !TABA;

  const bool BUTY = !ABOL_1MHZ;
  const bool BALY = !BYJU;
  const bool AFAR = !(ALYP || RESET);

  // UNK2
  const bool BUVU = (BUTY && BALY);
  const bool BOGA = !BALY;
  const bool BOGA1MHZ = BOGA;
  const bool ASOL = AFAR ^ RESET;
  const bool BOMA = !BOGA1MHZ;
  const bool BYXO = !BUVU;
  const bool BEDO = !BYXO;

  static bool AFER = 0;
  if (BOMA_NEG)   AFER = ASOL;
  if (!T1nT2n) AFER = 0;

  const bool BOWA = !BEDO;
  const bool AVOR = (AFER | ASOL);
  const bool ALUR = !AVOR;
  const bool RESET2 = ALUR;

  const bool UVYT = !PHI_OUT;
  const bool PHI_OUT_N = UVYT;
  */

  //----------------------------------------
  // bottom left of 1_CLOCKS_RESET

  /*
  const bool DULA = !RESET2;
  const bool CUNU = !DULA;
  const bool RESET6 = CUNU;
  const bool XORE = !CUNU;
  const bool WALU = !XORE;
  const bool WESY = !XORE;
  const bool RESET7 = XORE;
  const bool RESET9 = WESY;
  //const bool RESET6 = WALU;

  const bool XEBE = !RESET7;
  const bool RESET7_N = XEBE;
  const bool XODO = (FF40_D7) && XEBE;
  const bool XAPO = !XODO;
  const bool RESET_VIDEO = XAPO;

  // clock deglitcher?
  //const bool ARYS = !CLKIN_B;
  //const bool ANOS =
  const bool AVET = CLKIN_B;
  const bool ATAL = !AVET;
  const bool ATAL_4MHZ = ATAL;
  const bool AZOF = !ATAL;
  const bool ATAG = !AZOF;
  const bool ZAXY = !AZOF;
  const bool AMUK = !ATAG;
  const bool AMUK_4MHZ = AMUK;
  const bool ZEME = !ZAXY;
  const bool ARYF = !AMUK;
  const bool APUV = !AMUK;
  const bool CLK1 = ZEME;
  const bool ALET = !ZEME;
  const bool CLK2 = !ALET;

  const bool BELA = !APU_RESET;

  static bool CERY = 0;
  if (CYBO_4MHZ_NEG) {
  CERY = !CERY;
  }
  const bool CERY_2MHZ = CERY;
  */

  //----------------------------------------
  // bottom center of 1_CLOCKS_RESET
  // apu sequencer clocks?

  /*
  const bool ATUS = !APU_RESET;
  const bool COKE = !AJER_2MHZ;
  const bool BOPO = !APU_RESET;
  */

  //----------------------------------------
  // reads

  data_out = 0;
  if (bus.addr = 0xFF04 && bus.read) {
    data_out = uint8_t(div >> 6);
  }

  //----------------------------------------
  // clocks

  if (BOGA1MHZ_NEG) {
    div++;
  }

  //----------------------------------------
  // async resets

  const bool TAPE = (FF04_FF07 && CPU_WR && TOLA_A1N && TOVY_A0N);
  const bool UFOL = !(CLKIN_AN && RESET && TAPE);
  const bool RESET_DIV_N = UFOL;

  if (!RESET_DIV_N) {
    div = 0;
  }
}

#endif
// This file should contain the schematics as directly translated to C, no modifications or simplifications

#include "Schematics.h"

//----------
// inputs

extern bool ABOL_1MHZ;
extern bool T1N_T2N;
extern bool T1N_T2;
extern bool ATAL_4MHZ;
extern bool FROM_CPU3;
extern bool FROM_CPU4;
extern bool TABA;
extern bool CLKIN_A;

//----------
// outputs

bool PHI_OUT;
bool CPU_RD_SYNC;

//----------
// registers


static bool ADYK_Q, ADYK_CLK;
static bool AFUR_Q, AFUR_CLK;
static bool ALEF_Q, ALEF_CLK;
static bool APUK_Q, APUK_CLK;

//-----------------------------------------------------------------------------

void tick_clocks_reset() {

  bool ADAR = not(ADYK_Q); // 2m
  bool ATYP = not(AFUR_Q); // 1m
  bool AFEP = not(ALEF_Q); // 512k
  bool AROV = not(APUK_Q);
  bool AFAS = nor(ADAR, ATYP);
  bool AJAX = not(ATYP);
  bool BUGO = not(AFEP);
  bool AREV = nand(FROM_CPU3, AFAS);
  bool AGUT = unk3(AJAX, AROV, FROM_CPU4);
  bool BATE = nor(BUGO, AROV, ABOL_1MHZ);
  bool APOV = not(AREV);
  bool AWOD = or(T1N_T2, AGUT);
  bool BASU = not(BATE);
  bool ABUZ = not(AWOD);
  bool BUKE = not(BASU);


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

  const bool T1N_T2N = false; // fixme
  const bool T1N_T2 = false; // fixme
  const bool T1_T2N = false;
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
  const bool AWOD = (T1N_T2 || AGUT);
  const bool ABUZ = !AWOD;
  const bool BASU = !BATE;
  const bool BUKE = !BASU;

  const bool UPYF = (RESET || CLKIN_AN);

  // UNK2
  const bool TUBO = ABOL_1MHZ ^ UPYF;
  const bool UPOF = (div & 0x8000);
  const bool UNUT = (UPOF && TUBO);
  const bool TABA = (T1N_T2 || T1_T2N || UNUT);

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
  if (!T1N_T2N) AFER = 0;

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
#include "Types.h"

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

//-----------------------------------------------------------------------------
// mux2 = top = 1, bot = 0
// are ffs edge triggered or level triggered?

void hard_timer_tock(int tcycle, Bus bus, bool div_d1, bool TOLA_A1N, bool FROM_CPU5) {
  const bool RESET2 = true;
  const uint16_t addr = bus.addr;
  const uint8_t data = (uint8_t)bus.data;
  const bool CPU_RD = bus.read;
  const bool CPU_WR = bus.write;

  const bool _clk_1m  = (tcycle - 1) & 0x002;
  const bool clk_1m   = tcycle & 0x002;

  const bool clk_256k = tcycle & 0x008;
  const bool clk_64k  = tcycle & 0x020;
  const bool clk_16k  = tcycle & 0x080;

  //const bool BOGA1MHZ_POSEDGE = !_clk_1m && clk_1m;
  BOGA1MHZ_NEGEDGE = _clk_1m && !clk_1m;

  const bool CLK_256K = clk_256k;
  const bool CLK_64K  = clk_64k;
  const bool CLK_16K  = clk_16k;
  const bool FF04_D1  = div_d1;

  const bool A2 = addr & 0x04;
  const bool A1 = addr & 0x02;
  const bool A0 = addr & 0x01;

  const bool D7 = data & 0x80;
  const bool D6 = data & 0x40;
  const bool D5 = data & 0x20;
  const bool D4 = data & 0x10;
  const bool D3 = data & 0x08;
  const bool D2 = data & 0x04;
  const bool D1 = data & 0x02;
  const bool D0 = data & 0x01;

  const bool FFXX   = (addr & 0xFF00) == 0xFF00;
  const bool A00_07 = (addr & 0x00FF) <= 0x0007;

  const bool RYFO = (A2 && A00_07 && FFXX);
  const bool FF04_FF07 = RYFO;

  const bool SORA = (FF04_FF07 && CPU_RD && A1 && A0);

  const bool SUPE = SORA ? SABO : 0;
  const bool ROTE = SORA ? SAMY : 0;
  const bool RYLA = SORA ? SOPU : 0;

  const bool MULO = !RESET2;
  const bool TOVY = !A0;
  const bool TOVY_NA0 = TOVY;
  const bool TUBY = (FF04_FF07 && CPU_RD && A1 && TOVY);

  const bool REVA = TUBY ? PETO : 0;
  const bool NOLA = TUBY ? MURU : 0;
  const bool PYRE = TUBY ? NYKE : 0;
  const bool SAPU = TUBY ? SETA : 0;
  const bool SETE = TUBY ? SABU : 0;
  const bool SUPO = TUBY ? TYRU : 0;
  const bool SOTU = TUBY ? SUFY : 0;
  const bool SALU = TUBY ? TYVA : 0;

  const bool TOPE_nwrite_tima = !(A0 && TOLA_A1N && CPU_WR && FF04_FF07);
  const bool ROKE = TOPE_nwrite_tima ? SABU : D0;
  const bool PETU = TOPE_nwrite_tima ? NYKE : D1;
  const bool NYKU = TOPE_nwrite_tima ? MURU : D2;
  const bool SOCE = TOPE_nwrite_tima ? TYVA : D3;
  const bool SALA = TOPE_nwrite_tima ? TYRU : D4;
  const bool SYRU = TOPE_nwrite_tima ? SUFY : D5;
  const bool REFU = TOPE_nwrite_tima ? PETO : D6;
  const bool RATO = TOPE_nwrite_tima ? SETA : D7;

  const bool PUXY = !(ROKE || MULO);
  const bool NERO = !(PETU || MULO);
  const bool NADA = !(NYKU || MULO);
  const bool REPA = !(SOCE || MULO);
  const bool ROLU = !(SALA || MULO);
  const bool RUGY = !(SYRU || MULO);
  const bool PYMA = !(REFU || MULO);
  const bool PAGU = !(RATO || MULO);

  const bool UVYR = !CLK_64K;
  const bool UBOT = !CLK_256K;
  const bool UKAP = SOPU ? CLK_16K : UVYR;
  const bool TEKO = SOPU ? UBOT : !FF04_D1;
  const bool TECY = SAMY ? UKAP : TEKO;

  // TIMA reload
  const bool MUZU = FROM_CPU5 || TOPE_nwrite_tima;
  const bool MEKE = !MOBA;

  bool TEDA = (FF04_FF07 && CPU_RD && TOLA_A1N && A0);

  const bool SOKU = TEDA ? REGA : 0;
  const bool RACY = TEDA ? POVY : 0;
  const bool RAVY = TEDA ? PERU : 0;
  const bool SOSY = TEDA ? RATE : 0;
  const bool SOMU = TEDA ? RUBY : 0;
  const bool SURO = TEDA ? RAGE : 0;
  const bool ROWU = TEDA ? PEDA : 0;
  const bool PUSO = TEDA ? NUGA : 0;

  const bool MUGY = !MEXU;
  MERY = !(!NYDU || NUGA_COUT);

  //----------------------------------------
  // TIMA read

  if (TEDA) {
    data_ = (SOKU << 0) | (RACY << 1) | (RAVY << 2) | (SOSY << 3) | (SOMU << 4) | (SURO << 5) | (ROWU << 6) | (PUSO << 7);
  }

  //----------------------------------------
  // TMA read

  if (TUBY) {
    data_ = (SETE << 0) | (PYRE << 1) | (NOLA << 2) | (SALU << 3) | (SUPO << 4) | (SOTU << 5) | (REVA << 6) | (SAPU << 7);
  }

  //----------------------------------------
  // TAC read

  if (SORA) {
    data_ = (SOPU << 0) | (SAMY << 1) | (SABO << 2) | 0xF8;
  }

  //----------------------------------------
  // int_timer clock

  if (BOGA1MHZ_NEGEDGE) NYDU = NUGA_COUT;
  if (BOGA1MHZ_NEGEDGE) {
    MOBA = MERY;
    int_timer_ = MOBA;
  }

  if (!MUGY) NYDU = 0;

  //----------------------------------------
  // TIMA clock
  // going to guess this is clocked on falling edge too...

  const bool SOGU_ = !(TECY || !SABO);
  if (SOGU && !SOGU_) {
    uint16_t x = get_tima();
    x++;
    set_tima((uint8_t)x);
    NUGA_COUT = x & 0x100;
  }
  SOGU = SOGU_;

  //----------------------------------------
  // TIMA reload clock - if this and the above clock fire at the same time, who wins?

  const bool MEXU_ = !(MUZU && RESET2 && MEKE);
  if (!MEXU && MEXU_) {
    printf("tima write or reload\n");
    REGA = !PUXY;
    POVY = !NERO;
    PERU = !NADA;
    RATE = !REPA;
    RUBY = !ROLU;
    RAGE = !RUGY;
    PEDA = !PYMA;
    NUGA = !PAGU;
  }
  MEXU = MEXU_;

  //----------------------------------------
  // TMA clock
  // this is clocked on the _falling_ edge

  const bool TYJU_ = !(TOVY && A1 && CPU_WR && FF04_FF07);
  if (TYJU && !TYJU_) {
    printf("tma write\n");
    SABU = D0;
    NYKE = D1;
    MURU = D2;
    TYVA = D3;
    TYRU = D4;
    SUFY = D5;
    PETO = D6;
    SETA = D7;
  }
  TYJU = TYJU_;

  //----------------------------------------
  // TAC clock
  // this is clocked on the _falling_ edge

  const bool SARA_ = !(A0 && A1 && CPU_WR && FF04_FF07);
  if (SARA && !SARA_) {
    printf("tac write\n");
    SOPU = D0;
    SAMY = D1;
    SABO = D2;
  }
  SARA = SARA_;
}

//-----------------------------------------------------------------------------







/*
if (tphase == 0) {
counter = counter + 2;
old_tima = new_tima;

bool new_tick = (counter & masks[tac & 3]) && (tac & TAC_RUN);
if (tick && !new_tick) new_tima++;
tick = new_tick;
}

if (bus.write && bus.addr == ADDR_TIMA) new_tima = bus.data;
if (bus.write && bus.addr == ADDR_DIV) counter = 0;
if (bus.write && bus.addr == ADDR_TAC) tac = bus.data;
if (bus.write && bus.addr == ADDR_TMA) tma = bus.data;

if (tphase == 1) {
if (overflow) new_tima = tma;
overflow = (old_tima == 0xFF) && (new_tima == 0x00);

}

out.data = 0x00;
out.oe = false;
if (bus.read && bus.addr == ADDR_TAC)  { out.oe = true; out.data = tac | 0b11111000; }
if (bus.read && bus.addr == ADDR_TMA)  { out.oe = true; out.data = tma; }
if (bus.read && bus.addr == ADDR_DIV)  { out.oe = true; out.data = uint8_t(counter >> 7); }
if (bus.read && bus.addr == ADDR_TIMA) { out.oe = true; out.data = new_tima; }

out.overflow = overflow;

return out;
*/

#endif
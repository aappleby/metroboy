// This file should contain the schematics as directly translated to C, no modifications or simplifications

#include <stdint.h>

inline bool nor  (bool a, bool b)                 { return !(a || b); }
inline bool nor  (bool a, bool b, bool c)         { return !(a || b || c); }
inline bool nor  (bool a, bool b, bool c, bool d) { return !(a || b || c || d); }
inline bool nand (bool a, bool b)                 { return !(a && b); }
inline bool nand (bool a, bool b, bool c)         { return !(a && b && c); }
inline bool nand (bool a, bool b, bool c, bool d) { return !(a && b && c && d); }
inline bool or   (bool a, bool b)                 { return (a || b);  }
inline bool or   (bool a, bool b, bool c)         { return (a || b || c);  }
inline bool or   (bool a, bool b, bool c, bool d) { return (a || b || c || d);  }
inline bool and  (bool a, bool b)                 { return (a && b);  }
inline bool and  (bool a, bool b, bool c)         { return (a && b && c);  }
inline bool and  (bool a, bool b, bool c, bool d) { return (a && b && c && d);  }
inline bool not  (bool a)         { return !a; }
inline bool mux2 (bool m, bool a, bool b)         { return m ? a : b; }

// definitely not right...
inline bool un3  (bool a, bool b, bool c)         { return a ^ b ^ c; }

void pack(uint8_t& x, bool d0, bool d1, bool d2, bool d3, bool d4, bool d5, bool d6, bool d7) {
  x = (d0 << 0) | (d1 << 1) | (d2 << 2) | (d3 << 3) | (d4 << 4) | (d5 << 5) | (d6 << 6) | (d7 << 7);
}

void unpack(uint8_t x, bool& d0, bool& d1, bool& d2, bool& d3, bool& d4, bool& d5, bool& d6, bool& d7) {
  d0 = x & 0x01;
  d1 = x & 0x02;
  d2 = x & 0x04;
  d3 = x & 0x08;
  d4 = x & 0x10;
  d5 = x & 0x20;
  d6 = x & 0x40;
  d7 = x & 0x80;
}

//----------
// mystery signals

bool MATU_Q;
bool MOPA_PHI;
bool TUVO;
bool SARO;
bool TACU;
bool ACYL;
bool XYSO;
bool TEXY;
bool TEXO;
bool NET02;
bool MOE_IN;
bool ABUZ;
bool CPU_RD_SYNC;
bool AFAS;
bool MWR_IN;
bool MCS_IN;
bool XYMU;
bool VRAM_TO_OAM;
bool MYMA;
bool LENA;
bool BEDO;
bool P10_B;
bool T1N_T2;
bool RORU;
bool FEXXFFXXN;
bool LULA;

//----------

bool MOE_D;
bool MCS_D;
bool MCS_A;
bool MOE_A;
bool MWR_A;
bool MWR_D;

bool MD_B;

bool A0;
bool A1;
bool A2;
bool A3;
bool A4;
bool A5;
bool A6;
bool A7;
bool A8;
bool A9;
bool A10;
bool A11;
bool A12;
bool A13;
bool A14;
bool A15;

bool D0_IN;
bool D1_IN;
bool D2_IN;
bool D3_IN;
bool D4_IN;
bool D5_IN;
bool D6_IN;
bool D7_IN;

bool MD0_A;
bool MD3_A;
bool MD4_A;
bool MD7_A;
bool MD6_A;
bool MD1_A;
bool MD5_A;
bool MD2_A;

bool MD0_IN;
bool MD1_IN;
bool MD2_IN;
bool MD3_IN;
bool MD4_IN;
bool MD5_IN;
bool MD6_IN;
bool MD7_IN;

bool MD0_OUT;
bool MD1_OUT;
bool MD2_OUT;
bool MD3_OUT;
bool MD4_OUT;
bool MD5_OUT;
bool MD6_OUT;
bool MD7_OUT;

//----------
// tri-buses

bool D0 = 0;
bool D1 = 0;
bool D2 = 0;
bool D3 = 0;
bool D4 = 0;
bool D5 = 0;
bool D6 = 0;
bool D7 = 0;

bool D0_A = 0;
bool D1_A = 0;
bool D2_A = 0;
bool D3_A = 0;
bool D4_A = 0;
bool D5_A = 0;
bool D6_A = 0;
bool D7_A = 0;

bool MD0 = 0;
bool MD1 = 0;
bool MD2 = 0;
bool MD3 = 0;
bool MD4 = 0;
bool MD5 = 0;
bool MD6 = 0;
bool MD7 = 0;

bool MA0 = 0;
bool MA1 = 0;
bool MA2 = 0;
bool MA3 = 0;
bool MA4 = 0;
bool MA5 = 0;
bool MA7 = 0;
bool MA8 = 0;
bool MA9 = 0;
bool MA6 = 0;
bool MA10 = 0;
bool MA11 = 0;
bool MA12 = 0;

//----------
// registers

static bool SOTO_Q = 0;
static bool SOTO_N = 1;
static bool SOTO_CLK = 0;

//----------
// 25_VRAM_INTERFACE.png

void tock_vram() {
  bool RYVO = nand(D5, LULA);
  bool RERA = nand(D3, LULA);
  bool RABY = nand(D2, LULA);
  bool RORY = nand(D4, LULA);
  bool RUJA = nand(D1, LULA);
  bool RAVU = nand(D7, LULA);
  bool RAFY = nand(D6, LULA);
  bool RUXA = nand(D0, LULA);

  D5_A = RYVO;
  D3_A = RERA;
  D2_A = RABY;
  D4_A = RORY;
  D1_A = RUJA;
  D7_A = RAVU;
  D6_A = RAFY;
  D0_A = RUXA;

  bool CUFE = un3(SARO, MATU_Q, MOPA_PHI);
  bool VAPE = and(TACU, TUVO);
  bool AVER = and(ACYL, XYSO);
  bool XUJY = not(VAPE);
  bool BYCU = nor(CUFE, XUJY, AVER);
  bool COTA = not(BYCU);

  bool SYRO = not(FEXXFFXXN);
  bool TEFA = nor(SYRO, TEXO);
  bool SOSE = and(A15, TEFA); // odd...
  bool SOHO = and(TACU, TEXY);
  bool RAWA = not(SOHO);

  bool SYCY = not(NET02);

  bool TUCA = and(SOSE, ABUZ);
  bool TUJA = and(SOSE, CPU_RD_SYNC);
  bool TEGU = and(SOSE, AFAS);
  bool TAVY = not(MOE_IN);
  bool TUTO = and(NET02, SOTO_N);
  bool SUDO = not(MWR_IN);
  bool TEFY = not(MCS_IN);

  bool SALE = mux2(TUTO, TAVY, TEGU);
  bool TYJY = mux2(TUTO, SUDO, TUJA);
  bool TOLE = mux2(TUTO, TEFY, TUCA);

  bool ROPY = not(XYMU);
  bool RYLU = nand(SALE, ROPY);

  bool APAM = not(VRAM_TO_OAM);
  bool RUVY = not(SALE);
  bool SERE = and(TOLE, ROPY);
  bool SOHY = nand(TYJY, SERE);

  bool SUTU = nor(LENA, VRAM_TO_OAM, TEXY, SERE);
  bool SAZO = and(RUVY, SERE);
  bool RACU = and(RYLU, RAWA, MYMA, APAM);
  bool RACO = not(TUTO);

  bool RUTE = or(TUTO, RACO); // wat? or of a signal with its own inverse...
  bool SEWO = or(TUTO, SUTU);
  bool TODE = and(SUTU, RACO);
  bool SEMA = and(RACU, RACO);
  bool TAXY = and(SOHY, RACO);
  bool SOFY = or(TUTO, SOHY);

  bool SAHA = not(RUTE);
  bool SETY = not(SEWO);
  bool SOKY = not(TODE);
  bool REFO = not(SEMA);
  bool SYSY = not(TAXY);
  bool RAGU = not(SOFY);

  MOE_D = SAHA;
  MCS_D = SETY;
  MCS_A = SOKY;
  MOE_A = REFO;
  MWR_A = SYSY;
  MWR_D = RAGU;

  // this is some pulse generator sorta thing?
  bool RYJE = not(SAZO);
  bool REVO = not(RYJE);
  bool RELA = or(REVO, SAZO);
  bool ROCY = and(SAZO, REVO);

  bool RENA = not(RELA);
  bool RAHU = not(ROCY);
  bool ROFA = not(RENA);

  MD_B = ROFA;

  bool RAKU = not(MD7_IN);
  bool ROCE = not(MD4_IN);
  bool REMO = not(MD3_IN);
  bool ROPU = not(MD5_IN);
  bool RETA = not(MD6_IN);
  bool RYDO = not(MD2_IN);
  bool RODY = not(MD0_IN);
  bool REBA = not(MD1_IN);

  if (RENA) {
    MD7 = RAKU;
    MD4 = ROCE;
    MD3 = REMO;
    MD5 = ROPU;
    MD6 = RETA;
    MD2 = RYDO;
    MD0 = RODY;
    MD1 = REBA;
  }

  bool TUSO = nor(NET02, BEDO);
  bool SOLE = not(TUSO);
  bool RUNY = not(P10_B);

  bool TAHY = not(SOLE);
  bool TESU = not(SOLE);
  bool TAXO = not(SOLE);
  bool TOVU = not(SOLE);
  bool TAZU = not(SOLE);
  bool TEWA = not(SOLE);
  bool SOSA = not(SOLE);
  bool SEDU = not(SOLE);

  if (RUNY) {
    D4 = TAHY;
    D5 = TESU;
    D3 = TAXO;
    D0 = TOVU;
    D6 = TAZU;
    D7 = TEWA;
    D1 = SOSA;
    D2 = SEDU;
  }

  bool TEME = not(D0);
  bool TEWU = not(D1);
  bool TYGO = not(D2);
  bool SOTE = not(D3);
  bool SEKE = not(D4);
  bool RUJO = not(D5);
  bool TOFA = not(D6);
  bool SUZA = not(D7);

  if (RAHU) {
    MD0 = TEME;
    MD1 = TEWU;
    MD2 = TYGO;
    MD3 = SOTE;
    MD4 = SEKE;
    MD5 = RUJO;
    MD6 = TOFA;
    MD7 = SUZA;
  }

  bool SYNU = or(RAHU, MD0);
  bool SYMA = or(RAHU, MD1);
  bool ROKO = or(RAHU, MD2);
  bool SYBU = or(RAHU, MD3);
  bool SAKO = or(RAHU, MD4);
  bool SEJY = or(RAHU, MD5);
  bool SEDO = or(RAHU, MD6);
  bool SAWU = or(RAHU, MD7);

  bool RURA = not(SYNU);
  bool RULY = not(SYMA);
  bool RARE = not(ROKO);
  bool RODU = not(SYBU);
  bool RUBE = not(SAKO);
  bool RUMU = not(SEJY);
  bool RYTY = not(SEDO);
  bool RADY = not(SAWU);

  MD0_OUT = RURA;
  MD1_OUT = RULY;
  MD2_OUT = RARE;
  MD3_OUT = RODU;
  MD4_OUT = RUBE;
  MD5_OUT = RUMU;
  MD6_OUT = RYTY;
  MD7_OUT = RADY;

  bool XANE = nor(VRAM_TO_OAM, XYMU);
  bool XEDU = not(XANE);

  bool XECA = !A4;
  bool XYBO = !A7;
  bool RYSU = !A8;
  bool RUSE = !A10;
  bool RUMO = !A12;
  bool XYNE = !A2;
  bool XOBA = !A5;
  bool XODY = !A3;
  bool RYNA = !A11;
  bool RESE = !A9;
  bool XAKY = !A0;
  bool XOPO = !A6;
  bool XUXU = !A1;

  if (XEDU) {
    MA4 = XECA;
    MA7 = XYBO;
    MA8 = RYSU;
    MA10 = RUSE;
    MA12 = RUMO;
    MA2 = XYNE;
    MA5 = XOBA;
    MA3 = XODY;
    MA11 = RYNA;
    MA9 = RESE;
    MA0 = XAKY;
    MA6 = XOPO;
    MA1 = XUXU;
  }

  bool LYRA = nand(T1N_T2, RORU);

  bool RYBA = not(D7_IN);
  bool RUZY = not(D1_IN);
  bool ROME = not(D2_IN);
  bool TEHE = not(D4_IN);
  bool SOCA = not(D6_IN);
  bool RATU = not(D5_IN);
  bool TOVO = not(D0_IN);
  bool SAZA = not(D3_IN);

  bool ROPA = not(RYBA);
  bool SYWA = not(RUZY);
  bool SUGU = not(ROME);
  bool TUTE = not(TEHE);
  bool TEMY = not(SOCA);
  bool SAJO = not(RATU);
  bool TUTY = not(TOVO);
  bool TAWO = not(SAZA);

  if (LYRA) {
    D7 = ROPA;
    D1 = SYWA;
    D2 = SUGU;
    D4 = TUTE;
    D6 = TEMY;
    D5 = SAJO;
    D0 = TUTY;
    D3 = TAWO;
  }

  const bool ROVE = not(RAHU);

  const bool SEFA = and(MD0, ROVE);
  const bool SUNA = and(MD3, ROVE);
  const bool SUMO = and(MD4, ROVE);
  const bool SUKE = and(MD7, ROVE);
  const bool SAMO = and(MD6, ROVE);
  const bool SOGO = and(MD1, ROVE);
  const bool SAZU = and(MD5, ROVE);
  const bool SEFU = and(MD2, ROVE);

  const bool REGE = not(SEFA);
  const bool RADA = not(SUNA);
  const bool RYRO = not(SUMO);
  const bool RYZE = not(SUKE);
  const bool REKU = not(SAMO);
  const bool RYKY = not(SOGO);
  const bool REVU = not(SAZU);
  const bool RAZO = not(SEFU);

  MD0_A = REGE;
  MD3_A = RADA;
  MD4_A = RYRO;
  MD7_A = RYZE;
  MD6_A = REKU;
  MD1_A = RYKY;
  MD5_A = REVU;
  MD2_A = RAZO;

  //----------
  // unsunk signals

  (void)COTA;

  //----------
  // registers

  bool SOTO_CLK_ = SYCY;
  if (SOTO_CLK && !SOTO_CLK_) {
    SOTO_Q = SOTO_N;
    SOTO_N = !SOTO_Q;
  }
  SOTO_CLK = SOTO_CLK_;
}
// This file should contain the schematics as directly translated to C, no modifications or simplifications

#include "Schematics.h"

//----------
// inputs

extern bool WEFE;
extern bool PORE;
extern bool NETA;
extern bool POTU;
extern bool XUHA;
extern bool XEHO;

extern bool FF40_D3;

extern bool FF42_D0;
extern bool FF42_D1;
extern bool FF42_D2;
extern bool FF42_D3;
extern bool FF42_D4;
extern bool FF42_D5;
extern bool FF42_D6;
extern bool FF42_D7;

extern bool FF43_D0;
extern bool FF43_D1;
extern bool FF43_D2;
extern bool FF43_D3;
extern bool FF43_D4;
extern bool FF43_D5;
extern bool FF43_D6;
extern bool FF43_D7;

extern bool XEHO_Q;
extern bool SAVY_Q;
extern bool XODU_Q;
extern bool XYDO_Q;
extern bool TUHU_Q;
extern bool TUKY_Q;
extern bool TAKO_Q;
extern bool SYBE_Q;

extern bool DEPO;
extern bool LESY;
extern bool LOTA;
extern bool LYKU;
extern bool ROBY;
extern bool TYTA;
extern bool TYCO;
extern bool SOKA;
extern bool XOVU;
extern bool CLKPIPE;

//----------
// outputs

//----------
// registers - background pixel pipe?

bool BG_CLK;
bool VEZO_Q;
bool WURU_Q;
bool VOSA_Q;
bool WYFU_Q;
bool XETE_Q;
bool WODA_Q;
bool VUMO_Q;
bool VAVA_Q;

//-----------------------------------------------------------------------------

void tick_background() {
  //----------
  // y + scy;

  bool AXAD = not(PORE);
  bool ASUL = and(AXAD, NETA);
  bool ACEN = and(AXAD, POTU);
  bool BEJE = not(ASUL);
  bool BAFY = not(ACEN);

  bool FAFO_S = add_s(V0, FF42_D0, 0);
  bool FAFO_C = add_c(V0, FF42_D0, 0);

  bool EMUX_S = add_s(V1, FF42_D1, FAFO_C);
  bool EMUX_C = add_c(V1, FF42_D1, FAFO_C);

  bool ECAB_S = add_s(V2, FF42_D2, EMUX_C);
  bool ECAB_C = add_c(V2, FF42_D2, EMUX_C);

  bool ETAM_S = add_s(V3, FF42_D3, ECAB_C);
  bool ETAM_C = add_c(V3, FF42_D3, ECAB_C);

  bool DOTO_S = add_s(V4, FF42_D4, ETAM_C);
  bool DOTO_C = add_c(V4, FF42_D4, ETAM_C);

  bool DABA_S = add_s(V5, FF42_D5, DOTO_C);
  bool DABA_C = add_c(V5, FF42_D5, DOTO_C);

  bool EFYK_S = add_s(V6, FF42_D6, DABA_C);
  bool EFYK_C = add_c(V6, FF42_D6, DABA_C);

  bool EJOK_S = add_s(V7, FF42_D7, EFYK_C);
  //bool EJOK_C = add_c(V7, FF42_D7, EFYK_C);

  bool ASUM = not(XUHA);
  bool EVAD = not(FAFO_S);
  bool DAHU = not(EMUX_S);
  bool DODE = not(ECAB_S);
  bool DUHO = not(ETAM_S);
  bool CASE = not(DOTO_S);
  bool CYPO = not(DABA_S);
  bool CETA = not(EFYK_S);
  bool DAFE = not(EJOK_S);

  if (BEJE) {
    MA0 = ASUM;
    MA1 = EVAD;
    MA2 = DAHU;
    MA3 = DODE;
  }

  if (BAFY) {
    MA5 = DUHO;
    MA6 = CASE;
    MA7 = CYPO;
    MA8 = CETA;
    MA9 = DAFE;
  }

  //----------
  // x + scx

  //bool ATAD_S = add_s(XEHO,   FF43_D0, 0);
  bool ATAD_C = add_c(XEHO,   FF43_D0, 0);

  //bool BEHU_S = add_s(SAVY_Q, FF43_D1, ATAD_C);
  bool BEHU_C = add_c(SAVY_Q, FF43_D1, ATAD_C);

  //bool APYH_S = add_s(XODU_Q, FF43_D2, BEHU_C);
  bool APYH_C = add_c(XODU_Q, FF43_D2, BEHU_C);

  bool BABE_S = add_s(XYDO_Q, FF43_D3, APYH_C);
  bool BABE_C = add_c(XYDO_Q, FF43_D3, APYH_C);

  bool ABOD_S = add_s(TUHU_Q, FF43_D4, BABE_C);
  bool ABOD_C = add_c(TUHU_Q, FF43_D4, BABE_C);

  bool BEWY_S = add_s(TUKY_Q, FF43_D5, ABOD_C);
  bool BEWY_C = add_c(TUKY_Q, FF43_D5, ABOD_C);

  bool BYCA_S = add_s(TAKO_Q, FF43_D6, BEWY_C);
  bool BYCA_C = add_c(TAKO_Q, FF43_D6, BEWY_C);

  bool ACUL_S = add_s(SYBE_Q, FF43_D7, BYCA_C);
  //bool ACUL_C = add_c(SYBE_Q, FF43_D7, BYCA_C);

  bool AMUV = not(FF40_D3);
  bool COXO = not(WEFE);
  bool COVE = not(WEFE);

  // maybe the 'unk1' block inverts its sum out? otherwise we'd be putting the inverted sum on the bus...
  bool AXEP = not(BABE_S);
  bool AFEB = not(ABOD_S);
  bool ALEL = not(BEWY_S);
  bool COLY = not(BYCA_S);
  bool AJAN = not(ACUL_S);

  if (BAFY) {
    MA10 = AMUV;
    MA12 = COXO;
    MA11 = COVE;
    MA0 = AXEP;
    MA1 = AFEB;
    MA2 = ALEL;
    MA3 = COLY;
    MA4 = AJAN;
  }

  //----------
  // some shift register connected to VAVA3?

  bool XOGA = not(DEPO);
  bool XURA = not(DEPO);
  bool TAJO = not(DEPO);
  bool XENU = not(DEPO);
  bool XYKE = not(DEPO);
  bool XABA = not(DEPO);
  bool TAFU = not(DEPO);
  bool XUHO = not(DEPO);

  bool TEDE = nand(DEPO, LESY);
  bool WOKA = nand(XOGA, LESY);

  bool XALA = nand(LOTA, DEPO);
  bool WEDE = nand(LOTA, XURA);

  bool TYRA = nand(DEPO, LYKU);
  bool TUFO = nand(TAJO, LYKU);
  
  bool XYRU = nand(DEPO, ROBY);
  bool WEVO = nand(XENU, ROBY);

  bool XUKU = nand(DEPO, TYTA);
  bool WEDY = nand(XYKE, TYTA);
  
  bool XELY = nand(DEPO, TYCO);
  bool WUJA = nand(TYCO, XABA);

  bool TYKO = nand(DEPO, SOKA);
  bool TENA = nand(TAFU, SOKA);

  bool TUWU = nand(DEPO, XOVU);
  bool WUBU = nand(XUHO, XOVU);

  //----------
  // registers = background pixel pipe?

  bool VEZO_Q_ = VEZO_Q;
  bool WURU_Q_ = WURU_Q;
  bool VOSA_Q_ = VOSA_Q;
  bool WYFU_Q_ = WYFU_Q;
  bool XETE_Q_ = XETE_Q;
  bool WODA_Q_ = WODA_Q;
  bool VUMO_Q_ = VUMO_Q;
  bool VAVA_Q_ = VAVA_Q;

  if (BG_CLK && !CLKPIPE) {
    VEZO_Q_ = 0;
    WURU_Q_ = VEZO_Q;
    VOSA_Q_ = WURU_Q;
    WYFU_Q_ = VOSA_Q;
    XETE_Q_ = WYFU_Q;
    WODA_Q_ = XETE_Q;
    VUMO_Q_ = WODA_Q;
    VAVA_Q_ = VUMO_Q;
  }

  if (!TEDE) VEZO_Q_ = 1;
  if (!WOKA) VEZO_Q_ = 0;
  if (!XALA) WURU_Q_ = 1;
  if (!WEDE) WURU_Q_ = 0;
  if (!TYRA) VOSA_Q_ = 1;
  if (!TUFO) VOSA_Q_ = 0;
  if (!XYRU) WYFU_Q_ = 1;
  if (!WEVO) WYFU_Q_ = 0;
  if (!XUKU) XETE_Q_ = 1;
  if (!WEDY) XETE_Q_ = 0;
  if (!XELY) WODA_Q_ = 1;
  if (!WUJA) WODA_Q_ = 0;
  if (!TYKO) VUMO_Q_ = 1;
  if (!TENA) VUMO_Q_ = 0;
  if (!TUWU) VAVA_Q_ = 1;
  if (!WUBU) VAVA_Q_ = 0;

  BG_CLK = CLKPIPE;

  VEZO_Q = VEZO_Q_;
  WURU_Q = WURU_Q_;
  VOSA_Q = VOSA_Q_;
  WYFU_Q = WYFU_Q_;
  XETE_Q = XETE_Q_;
  WODA_Q = WODA_Q_;
  VUMO_Q = VUMO_Q_;
  VAVA_Q = VAVA_Q_;
}
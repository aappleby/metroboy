// This file should contain the schematics as directly translated to C, no modifications or simplifications

#include "Schematics.h"

//----------
// inputs

extern bool WEFE;
extern bool PORE;
extern bool NETA;
extern bool POTU;
extern bool XUHA;

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

extern reg XEHO;
extern reg SAVY;
extern reg XODU;
extern reg XYDO;
extern reg TUHU;
extern reg TUKY;
extern reg TAKO;
extern reg SYBE;

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

bool VAVA3;

//----------
// registers - background pixel pipe?

static reg VEZO;
static reg WURU;
static reg VOSA;
static reg WYFU;
static reg XETE;
static reg WODA;
static reg VUMO;
static reg VAVA;

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

  bool XEHO_Q = XEHO.q();
  bool SAVY_Q = SAVY.q();
  bool XODU_Q = XODU.q();
  bool XYDO_Q = XYDO.q();
  bool TUHU_Q = TUHU.q();
  bool TUKY_Q = TUKY.q();
  bool TAKO_Q = TAKO.q();
  bool SYBE_Q = SYBE.q();

  //bool ATAD_S = add_s(XEHO_Q, FF43_D0, 0);
  bool ATAD_C = add_c(XEHO_Q, FF43_D0, 0);

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

  bool VEZO_Q = VEZO.srtock(CLKPIPE, TEDE, WOKA, 0);
  bool WURU_Q = WURU.srtock(CLKPIPE, XALA, WEDE, VEZO_Q);
  bool VOSA_Q = VOSA.srtock(CLKPIPE, TYRA, TUFO, WURU_Q);
  bool WYFU_Q = WYFU.srtock(CLKPIPE, XYRU, WEVO, VOSA_Q);
  bool XETE_Q = XETE.srtock(CLKPIPE, XUKU, WEDY, WYFU_Q);
  bool WODA_Q = WODA.srtock(CLKPIPE, XELY, WUJA, XETE_Q);
  bool VUMO_Q = VUMO.srtock(CLKPIPE, TYKO, TENA, WODA_Q);
  bool VAVA_Q = VAVA.srtock(CLKPIPE, TUWU, WUBU, VUMO_Q);
  VAVA3 = VAVA_Q;
}
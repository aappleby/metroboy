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

//----------
// outputs

//----------
// registers

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

}
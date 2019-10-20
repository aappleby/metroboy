#pragma once
#include "Schematics.h"

struct APU {
  bool CH1_AMP_ENn, CH2_AMP_ENn, FF1A_D7n, CH4_AMP_ENn;

  //----------
  // outputs

  bool APU_WR;
  bool AMP_ENn;

  bool ANAP;
  bool DUCE;

  bool CH1_ACTIVEn;
  bool CH2_ACTIVEn;
  bool CH3_ACTIVEn;
  bool CH4_ACTIVEn;

  //----------
  // outputs

  bool FERO_Q;
  bool NET03; // not sure what this is yet

  bool CATE;
  bool GAXO;

  //----------
  // registers

  reg AJER;
  reg CALO;

  reg HADA;
  reg BOWY;
  reg BAZA;
  reg FERO;

  // FF24 NR50
  reg BEDU,COZU,BUMO,BYRE,APOS,AGER,BYGA,APEG;

  // FF25 NR51
  reg BOGU,BAFO,ATUF,ANEV,BEPU,BEFO,BUME,BOFA;
};

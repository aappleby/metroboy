#pragma once
#include "GateBoyLib/Gates.h"

// Placeholder regs for sound so we don't fail the mooneye reg tests

//----------------------------------------

struct NR10 {
};

struct NR11 {
};

struct NR12 {
};

struct NR14 {
};

//----------------------------------------

struct NR21 {
};

struct NR22 {
};

struct NR24 {
};

//----------------------------------------

struct NR30 {
};

struct NR31 {
};

struct NR32 {
};

struct NR34 {
};

//----------------------------------------

struct NR41 {
};

struct NR42 {
};

struct NR43 {
};

struct NR44 {
};

//----------------------------------------

struct NR50 {
  /*p09.APEG*/ DFF9 APEG_VOL_L0;
  /*p09.BYGA*/ DFF9 BYGA_VOL_L1;
  /*p09.AGER*/ DFF9 AGER_VOL_L2;
  /*p09.APOS*/ DFF9 APOS_VIN_TO_L;
  /*p09.BYRE*/ DFF9 BYRE_VOL_R0;
  /*p09.BUMO*/ DFF9 BUMO_VOL_R1;
  /*p09.COZU*/ DFF9 COZU_VOL_R2;
  /*p09.BEDU*/ DFF9 BEDU_VIN_TO_R;
};

struct NR51 {
  /*p09.ANEV*/ DFF9 NR51_0;
  /*p09.BOGU*/ DFF9 NR51_1;
  /*p09.BAFO*/ DFF9 NR51_2;
  /*p09.ATUF*/ DFF9 NR51_3;
  /*p09.BUME*/ DFF9 NR51_4;
  /*p09.BOFA*/ DFF9 NR51_5;
  /*p09.BEFO*/ DFF9 NR51_6;
  /*p09.BEPU*/ DFF9 NR51_7;
};

struct NR52 {
  /*p09.FERO*/ DFF9  FERO_DBG_APUp; // secret debug bit
  /*p09.BOWY*/ DFF17 BOWY_DBG_SWEEPp; // secret debug bit
  /*p09.HADA*/ DFF17 HADA_ALL_SOUND_ONp; //FF26_7
};

//----------------------------------------

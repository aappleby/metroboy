/// plait_noparse

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
  void reset_to_cart() {
    APEG_VOL_L0.state   = 0b00011010;
    BYGA_VOL_L1.state   = 0b00011010;
    AGER_VOL_L2.state   = 0b00011010;
    APOS_VIN_TO_L.state = 0b00011011;
    BYRE_VOL_R0.state   = 0b00011010;
    BUMO_VOL_R1.state   = 0b00011010;
    COZU_VOL_R2.state   = 0b00011010;
    BEDU_VIN_TO_R.state = 0b00011011;
  }

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
  void reset_to_cart() {
    ANEV_NR51_0.state = 0b00011010;
    BOGU_NR51_1.state = 0b00011010;
    BAFO_NR51_2.state = 0b00011011;
    ATUF_NR51_3.state = 0b00011011;
    BUME_NR51_4.state = 0b00011010;
    BOFA_NR51_5.state = 0b00011010;
    BEFO_NR51_6.state = 0b00011010;
    BEPU_NR51_7.state = 0b00011010;
  }

  /*p09.ANEV*/ DFF9 ANEV_NR51_0;
  /*p09.BOGU*/ DFF9 BOGU_NR51_1;
  /*p09.BAFO*/ DFF9 BAFO_NR51_2;
  /*p09.ATUF*/ DFF9 ATUF_NR51_3;
  /*p09.BUME*/ DFF9 BUME_NR51_4;
  /*p09.BOFA*/ DFF9 BOFA_NR51_5;
  /*p09.BEFO*/ DFF9 BEFO_NR51_6;
  /*p09.BEPU*/ DFF9 BEPU_NR51_7;
};

struct NR52 {
  void reset_to_cart() {
    FERO_DBG_APUp      .state = 0b00011011;
    BOWY_DBG_SWEEPp    .state = 0b00011010;
    HADA_ALL_SOUND_ONp .state = 0b00011011;
  }

  /*p09.FERO*/ DFF9  FERO_DBG_APUp; // secret debug bit
  /*p09.BOWY*/ DFF17 BOWY_DBG_SWEEPp; // secret debug bit
  /*p09.HADA*/ DFF17 HADA_ALL_SOUND_ONp; //FF26_7
};

//----------------------------------------

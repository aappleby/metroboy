/// plait_noparse

#pragma once

#include "GateBoyLib/Regs.h"
#include "GateBoyLib/Gates.h"

#include "GateBoyLib/GateBoyCh1.h"
#include "GateBoyLib/GateBoyCh2.h"
#include "GateBoyLib/GateBoyCh3.h"
#include "GateBoyLib/GateBoyCh4.h"

struct GateBoyState;

//==============================================================================
//             TOP
//==============================================================================

struct GateBoySPU {

  void reset_to_cart();

  //----------
  // SPU clocks

  /*#p15.CEMO*/ DFF17 CEMO_xBCDExxx;
  /*_p15.ATEP*/ DFF17 ATEP_AxxDExxH;

  //----------
  // clocks for the apu

  /*_p01.CERY*/ DFF17 CERY_AxxDExxH;
  /*_p01.ATYK*/ DFF17 ATYK_AxxDExxH;
  /*_p01.AVOK*/ DFF17 AVOK_xBCDExxx;
  /*_p09.AJER*/ DFF17 AJER_AxxDExxH;
  /*_p01.JESO*/ DFF17 JESO_CLK_512K;

  /*_p01.BARA*/ DFF17 BARA_CLK_512;
  /*_p01.CARU*/ DFF17 CARU_CLK_256;
  /*_p01.BYLU*/ DFF17 BYLU_CLK_128;

  //----------
  // FF24 NR50
  
  /*_p09.APEG*/ DFF9B APEG_NR50_VOL_L0;
  /*_p09.BYGA*/ DFF9B BYGA_NR50_VOL_L1;
  /*_p09.AGER*/ DFF9B AGER_NR50_VOL_L2;
  /*_p09.APOS*/ DFF9B APOS_NR50_VIN_TO_L;
  /*_p09.BYRE*/ DFF9B BYRE_NR50_VOL_R0;
  /*_p09.BUMO*/ DFF9B BUMO_NR50_VOL_R1;
  /*_p09.COZU*/ DFF9B COZU_NR50_VOL_R2;
  /*_p09.BEDU*/ DFF9B BEDU_NR50_VIN_TO_R;

  //----------
  // FF25 NR51

  /*_p09.ANEV*/ DFF9B ANEV_NR51_RCH1_ENp; // these are mixer control bits
  /*_p09.BOGU*/ DFF9B BOGU_NR51_RCH2_ENp;
  /*_p09.BAFO*/ DFF9B BAFO_NR51_RCH3_ENp;
  /*_p09.ATUF*/ DFF9B ATUF_NR51_RCH4_ENp;
  /*_p09.BUME*/ DFF9B BUME_NR51_LCH1_ENp;
  /*_p09.BOFA*/ DFF9B BOFA_NR51_LCH2_ENp;
  /*_p09.BEFO*/ DFF9B BEFO_NR51_LCH3_ENp;
  /*_p09.BEPU*/ DFF9B BEPU_NR51_LCH4_ENp;

  //----------
  // FF26 NR52

  // DBG_APUn polarity?
  /*_p09.FERO*/ DFF9B FERO_NR52_DBG_APUp;     // secret debug bit
  /*_p09.BOWY*/ DFF17 BOWY_NR52_DBG_SWEEP;    // secret debug bit
  /*_p09.HADA*/ DFF17 HADA_NR52_ALL_SOUND_ON; //FF26_7
};

void tick_spu(const GateBoyState& reg_old, GateBoyState& reg_new, uint8_t* wave_ram);
int spu_audio_out_r(const GateBoyState& reg_new);
int spu_audio_out_l(const GateBoyState& reg_new);

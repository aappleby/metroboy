/// plait_noparse

#pragma once

#include "GateBoyLib/Regs.h"
#include "GateBoyLib/Gates.h"

#include "GateBoyLib/GateBoyCh1.h"
#include "GateBoyLib/GateBoyCh2.h"
#include "GateBoyLib/GateBoyCh3.h"
#include "GateBoyLib/GateBoyCh4.h"

struct GateBoyState;
struct GateBoyCpuABus;
struct GateBoyCpuDBus;
struct GateBoyWaveBus;

//==============================================================================
//             TOP
//==============================================================================

struct GateBoySPU {

  void reset();

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
  
  /*_p09.APEG*/ DFF9 APEG_NR50_VOL_L0;
  /*_p09.BYGA*/ DFF9 BYGA_NR50_VOL_L1;
  /*_p09.AGER*/ DFF9 AGER_NR50_VOL_L2;
  /*_p09.APOS*/ DFF9 APOS_NR50_VIN_TO_L;
  /*_p09.BYRE*/ DFF9 BYRE_NR50_VOL_R0;
  /*_p09.BUMO*/ DFF9 BUMO_NR50_VOL_R1;
  /*_p09.COZU*/ DFF9 COZU_NR50_VOL_R2;
  /*_p09.BEDU*/ DFF9 BEDU_NR50_VIN_TO_R;

  //----------
  // FF25 NR51

  /*_p09.ANEV*/ DFF9 ANEV_NR51_RCH1_ENp; // these are mixer control bits
  /*_p09.BOGU*/ DFF9 BOGU_NR51_RCH2_ENp;
  /*_p09.BAFO*/ DFF9 BAFO_NR51_RCH3_ENp;
  /*_p09.ATUF*/ DFF9 ATUF_NR51_RCH4_ENp;
  /*_p09.BUME*/ DFF9 BUME_NR51_LCH1_ENp;
  /*_p09.BOFA*/ DFF9 BOFA_NR51_LCH2_ENp;
  /*_p09.BEFO*/ DFF9 BEFO_NR51_LCH3_ENp;
  /*_p09.BEPU*/ DFF9 BEPU_NR51_LCH4_ENp;

  //----------
  // FF26 NR52

  // DBG_APUn polarity?
  /*_p09.FERO*/ DFF9 FERO_NR52_DBG_APUp;     // secret debug bit
  /*_p09.BOWY*/ DFF17 BOWY_NR52_DBG_SWEEP;    // secret debug bit
  /*_p09.HADA*/ DFF17 HADA_NR52_ALL_SOUND_ON; //FF26_7
};

void tick_spu(const GateBoyState& reg_old, GateBoyState& reg_new, uint8_t* wave_ram);
sample_t spu_audio_out_r(const GateBoyState& reg_new);
sample_t spu_audio_out_l(const GateBoyState& reg_new);

void tick_spu_fast(
  const GateBoyCpuABus& cpu_abus_new,
  const GateBoyCpuDBus& cpu_dbus_old,
  const GateBoySPU& spu_old,
  const SpuChannel1& ch1_old,
  const SpuChannel2& ch2_old,
  const SpuChannel3& ch3_old,
  const SpuChannel4& ch4_old,

  GateBoyCpuDBus& cpu_dbus_new,
  GateBoyWaveBus& wave_dbus_new,
  GateBoySPU& spu_new,
  SpuChannel1& ch1_new,
  SpuChannel2& ch2_new,
  SpuChannel3& ch3_new,
  SpuChannel4& ch4_new,

  wire SIG_CPU_CLKREQ,
  wire SIG_IN_CPU_DBUS_FREE,
  wire SIG_CPU_UNOR_DBG_old,
  wire TERU_DIV10p_old,

  wire AFER_SYS_RSTp,
  wire ASOL_POR_DONEn,

  wire AVET_AxCxExGx,
  wire AFUR_ABCDxxxx_qn,

  wire TEDO_CPU_RDp,
  wire TAPU_CPU_WRp,

  uint8_t* wave_ram
);

sample_t spu_audio_out_r_fast(const GateBoyState& reg_new);
sample_t spu_audio_out_l_fast(const GateBoyState& reg_new);

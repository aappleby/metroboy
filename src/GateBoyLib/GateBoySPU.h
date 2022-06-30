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
  /*
  wire SYKE_ADDR_FFXXp = 0;
  wire UNOR_MODE_DBG2 = 0;
  wire ALUR_SYS_RSTn = true;
  wire TEDO_CPU_RDp = false;
  wire TAPU_CPU_WRp = false;
  wire AZOF_xBxDxFxH = 0;
  wire BUDE_xxxxEFGH = 0;
  wire UMER_DIV10n_old = 0;
  wire SIG_CPU_HOLD_MEM = 0;
  wire SIG_IN_CPU_DBUS_FREE = 0;
  wire BUKE_ABxxxxxH = 0;
  */

  void tick(const GateBoyState& reg_old, GateBoyState& reg_new);

  void tick_nr10(const GateBoyState& reg_old, GateBoyState& reg_new);
  void tick_nr11(const GateBoyState& reg_old, GateBoyState& reg_new);
  void tick_nr12(const GateBoyState& reg_old, GateBoyState& reg_new);
  void tick_nr13(const GateBoyState& reg_old, GateBoyState& reg_new);
  void tick_nr14(const GateBoyState& reg_old, GateBoyState& reg_new);

  void tick_nr21(const GateBoyState& reg_old, GateBoyState& reg_new);
  void tick_nr22(const GateBoyState& reg_old, GateBoyState& reg_new);
  void tick_nr23(const GateBoyState& reg_old, GateBoyState& reg_new);
  void tick_nr24(const GateBoyState& reg_old, GateBoyState& reg_new);

  void tick_nr30(const GateBoyState& reg_old, GateBoyState& reg_new);
  void tick_nr31(const GateBoyState& reg_old, GateBoyState& reg_new);
  void tick_nr32(const GateBoyState& reg_old, GateBoyState& reg_new);
  void tick_nr33(const GateBoyState& reg_old, GateBoyState& reg_new);
  void tick_nr34(const GateBoyState& reg_old, GateBoyState& reg_new);

  void tick_nr41(const GateBoyState& reg_old, GateBoyState& reg_new);
  void tick_nr42(const GateBoyState& reg_old, GateBoyState& reg_new);
  void tick_nr43(const GateBoyState& reg_old, GateBoyState& reg_new);
  void tick_nr44(const GateBoyState& reg_old, GateBoyState& reg_new);

  void tick_nr50(const GateBoyState& reg_old, GateBoyState& reg_new);
  void tick_nr51(const GateBoyState& reg_old, GateBoyState& reg_new);
  void tick_nr52(const GateBoyState& reg_old, GateBoyState& reg_new);

  void tick_ch1(const GateBoyState& reg_old, GateBoyState& reg_new);
  void tick_ch2(const GateBoyState& reg_old, GateBoyState& reg_new);
  void tick_ch3(const GateBoyState& reg_old, GateBoyState& reg_new);
  void tick_ch4(const GateBoyState& reg_old, GateBoyState& reg_new);

  //----------
  // Debug control signals

  //----------
  // SPU clock tree


  //----------
  // SPU reset tree

  /*#p01.BATA*/ wire BATA_CLK_2M() const { return not1(AJER_CLK_2M.qp_new()); }
  /*_p01.BAVU*/ wire BAVU_CLK_1M() const { return not1(AVOK_CLK_1M.qp_new()); }
  /*#p16.FABO*/ wire FABO_CLK_xxCDxxGH() const { return not1(CERY_CLK_2M.qp_new()); }



  /*#p20.GYBA*/ wire GYBA_CLK_1M() const { return not1(BAVU_CLK_1M()); }


  /*#p01.HAMA*/ wire HAMA_CLK_512K() const { return not1(JESO_CLK_512K.qp_new()); }
  /*_p01.BURE*/ wire BURE_CLK_512() const { return not1(BARA_CLK_512.qp_new()); }

  /*_p01.HORU*/ wire HORU_CLK_512p() const {
    /*_p01.FYNE*/ wire FYNE_CLK_512 = not1(BURE_CLK_512());
    /*_p01.GALE*/ wire GALE_CLK_512 = mux2p(FERO_NR52_DBG_APUn.qp_new(), HAMA_CLK_512K(), FYNE_CLK_512);
    /*_p01.GEXY*/ wire GEXY_CLK_512 = not1(GALE_CLK_512);
    /*_p01.HORU*/ wire HORU_CLK_512p = not1(GEXY_CLK_512);
    return HORU_CLK_512p;
  }

  /*_p01.BUFY*/ wire BUFY_CLK_256n() const {
    /*_p01.CULO*/ wire CULO_CLK_256 = not1(CARU_CLK_256.qp_new());
    /*_p01.BEZE*/ wire BEZE_CLK_256 = mux2p(FERO_NR52_DBG_APUn.qp_new(), HAMA_CLK_512K(), CULO_CLK_256);
    /*_p01.COFU*/ wire COFU_CLK_256 = not1(BEZE_CLK_256);
    /*_p01.BUFY*/ wire BUFY_CLK_256n = not1(COFU_CLK_256);
    return BUFY_CLK_256n;
  }

  /*_p01.BYFE*/ wire BYFE_CLK_128n() const {
    /*_p01.APEF*/ wire APEF_CLK_128 = not1(BYLU_CLK_128.qp_new());
    /*_p01.BULE*/ wire BULE_CLK_128 = mux2p(FERO_NR52_DBG_APUn.qp_new(), HAMA_CLK_512K(), APEF_CLK_128);
    /*_p01.BARU*/ wire BARU_CLK_128 = not1(BULE_CLK_128);
    /*_p01.BYFE*/ wire BYFE_CLK_128n = not1(BARU_CLK_128);
    return BYFE_CLK_128n;
  }

  /*#p13.JONE*/ wire JONE_CLK_128p() const { return not1(BYFE_CLK_128n()); }
  /*#p20.ALOP*/ wire ALOP_CLK_128() const { return not1(BYFE_CLK_128n()); }

  /*#p15.CEMO*/ DFF17 CEMO_CLK_1M;
  /*_p15.ATEP*/ DFF17 ATEP_CLK_2M;

  //----------
  // clocks for the apu

  /*_p01.CERY*/ DFF17 CERY_CLK_2M;
  /*_p01.ATYK*/ DFF17 ATYK_CLK_2M;
  /*_p01.AVOK*/ DFF17 AVOK_CLK_1M;
  /*_p09.AJER*/ DFF17 AJER_CLK_2M;
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

  /*_p09.ANEV*/ DFF9 ANEV_NR51_D0; // these are mixer control bits
  /*_p09.BOGU*/ DFF9 BOGU_NR51_D1;
  /*_p09.BAFO*/ DFF9 BAFO_NR51_D2;
  /*_p09.ATUF*/ DFF9 ATUF_NR51_D3;
  /*_p09.BUME*/ DFF9 BUME_NR51_D4;
  /*_p09.BOFA*/ DFF9 BOFA_NR51_D5;
  /*_p09.BEFO*/ DFF9 BEFO_NR51_D6;
  /*_p09.BEPU*/ DFF9 BEPU_NR51_D7;

  //----------
  // FF26 NR52

  // DBG_APUn polarity?
  /*_p09.FERO*/ DFF9  FERO_NR52_DBG_APUn;     // secret debug bit
  /*_p09.BOWY*/ DFF17 BOWY_NR52_DBG_SWEEP;    // secret debug bit
  /*_p09.HADA*/ DFF17 HADA_NR52_ALL_SOUND_ON; //FF26_7
};

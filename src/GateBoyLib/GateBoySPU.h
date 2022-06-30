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

struct GBSound {
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
  // CPU read/write signals

  /*_p09.AGUZ*/ wire AGUZ_CPU_RDn() const { return not1(TEDO_CPU_RDp); }
  /*#p19.BYLO*/ wire BYLO_CPU_RDp() const { return not1(AGUZ_CPU_RDn()); }
  /*_p09.GAXO*/ wire GAXO_CPU_RDp() const { return not1(AGUZ_CPU_RDn()); }
  /*#p20.COSA*/ wire COSA_CPU_RDp() const { return not1(AGUZ_CPU_RDn()); }
  /*_p14.FOGE*/ wire FOGE_CPU_RDp() const { return not1(AGUZ_CPU_RDn()); }
  /*_p16.DOVO*/ wire DOVO_CPU_RDp() const { return not1(AGUZ_CPU_RDn()); }
  /*#p11.CEGE*/ wire CEGE_CPU_RDp() const { return not1(AGUZ_CPU_RDn()); }
  /*#pXX.GADO*/ wire GADO_CPU_RDp() const { return not1(AGUZ_CPU_RDn()); }
  /*#p17.BOKE*/ wire BOKE_CPU_RDp() const { return not1(AGUZ_CPU_RDn()); }
  /*_p14.BYGO*/ wire BYGO_CPU_RDp() const { return not1(AGUZ_CPU_RDn()); }
  /*_p09.CETO*/ wire CETO_CPU_RDp() const { return not1(AGUZ_CPU_RDn()); }
  /*_p09.KAZO*/ wire KAZO_CPU_RDp() const { return not1(AGUZ_CPU_RDn()); }
  /*_p09.CURU*/ wire CURU_CPU_RDp() const { return not1(AGUZ_CPU_RDn()); }
  /*_p09.KYDU*/ wire KYDU_CPU_RDp() const { return not1(AGUZ_CPU_RDn()); }
  /*_p16.JOTU*/ wire JOTU_CPU_RDp() const { return not1(AGUZ_CPU_RDn()); }
  /*_p18.JECO*/ wire JECO_CPU_RDp() const { return not1(AGUZ_CPU_RDn()); }
  /*_p16.GORY*/ wire GORY_CPU_RDp() const { return not1(AGUZ_CPU_RDn()); }
  /*#p19.KAGE*/ wire KAGE_CPU_RDp() const { return not1(AGUZ_CPU_RDn()); }
  /*#p11.BUDA*/ wire BUDA_CPU_RDp() const { return not1(AGUZ_CPU_RDn()); }
  /*#p11.BUWA*/ wire BUWA_CPU_RDp() const { return not1(AGUZ_CPU_RDn()); }

  /*_p10.BAFU*/ wire BAFU_CPU_WRn() const { return not1(TAPU_CPU_WRp); }
  /*_p10.BOGY*/ wire BOGY_CPU_WRp() const { return not1(BAFU_CPU_WRn()); }

  //----------
  // Debug control signals

  /*_p16.ANUJ*/ wire ANUJ_CPU_WR_WEIRD()  const { return and2(SIG_IN_CPU_DBUS_FREE, BOGY_CPU_WRp()); }
  /*_p09.EDEK*/ wire EDEK_NR52_DBG_APUp() const { return not1(FERO_NR52_DBG_APUn.qn_new()); }
  /*_p14.FAPE*/ wire FAPE_CPU_RDp_DBGp()  const { return and2(FOGE_CPU_RDp(), EDEK_NR52_DBG_APUp()); }
  /*_p16.EGAD*/ wire EGAD_CPU_RDn_DBGn()  const { return nand2(DOVO_CPU_RDp(), EDEK_NR52_DBG_APUp()); }
  /*#p11.DAXA*/ wire DAXA_CPU_RDn_DBGn()  const { return nand2(CEGE_CPU_RDp(), EDEK_NR52_DBG_APUp()); }

  //----------
  // SPU clock tree

  /*_p01.ATAG*/ wire ATAG_AxCxExGx() const { return not1(AZOF_xBxDxFxH); }
  /*_p01.DOVA*/ wire DOVA_ABCDxxxx() const { return not1(BUDE_xxxxEFGH); } // this goes to all the trigger regs

  /*_p01.AMUK*/ wire AMUK_xBxDxFxH() const { return not1(ATAG_AxCxExGx()); } // goes to clock generators and wave ram clock
  /*_p01.CYBO*/ wire CYBO_AxCxExGx() const { return not1(AMUK_xBxDxFxH()); }
  /*_p01.ARYF*/ wire ARYF_AxCxExGx() const { return not1(AMUK_xBxDxFxH()); }
  /*_p01.APUV*/ wire APUV_AxCxExGx() const { return not1(AMUK_xBxDxFxH()); }
  /*_p15.AZEG*/ wire AZEG_AxCxExGx() const { return not1(AMUK_xBxDxFxH()); }


  //----------
  // SPU reset tree

  /*_p09.HAPO*/ wire HAPO_SYS_RSTp() const { return not1(ALUR_SYS_RSTn); }
  /*_p09.GUFO*/ wire GUFO_SYS_RSTn() const { return not1(HAPO_SYS_RSTp()); }

  /*_p09.JYRO*/ wire JYRO_APU_RSTp() const { return  or2(HAPO_SYS_RSTp(), HADA_NR52_ALL_SOUND_ON.qn_new()); }
  /*_p09.KEPY*/ wire KEPY_APU_RSTn() const { return not1(JYRO_APU_RSTp()); }
  /*_p09.KUBY*/ wire KUBY_APU_RSTn() const { return not1(JYRO_APU_RSTp()); }
  /*_p09.KEBA*/ wire KEBA_APU_RSTp() const { return not1(KUBY_APU_RSTn()); }

  /*_p01.ATUS*/ wire ATUS_APU_RSTn() const { return not1(KEBA_APU_RSTp()); }
  /*_p01.BELA*/ wire BELA_APU_RSTn() const { return not1(KEBA_APU_RSTp()); }
  /*_p01.BOPO*/ wire BOPO_APU_RSTn() const { return not1(KEBA_APU_RSTp()); }
  /*_p09.ATYV*/ wire ATYV_APU_RSTn() const { return not1(KEBA_APU_RSTp()); }
  /*_p09.KAME*/ wire KAME_APU_RSTn() const { return not1(KEBA_APU_RSTp()); }
  /*_p14.HUDE*/ wire HUDE_APU_RSTn() const { return not1(KEBA_APU_RSTp()); }
  /*_p14.JYBU*/ wire JYBU_APU_RSTn() const { return not1(KEBA_APU_RSTp()); }
  /*_p15.BUWE*/ wire BUWE_APU_RSTn() const { return not1(KEBA_APU_RSTp()); }
  /*_p15.CEXE*/ wire CEXE_APU_RSTn() const { return not1(KEBA_APU_RSTp()); }
  /*_p15.CYWU*/ wire CYWU_APU_RSTn() const { return not1(KEBA_APU_RSTp()); }
  /*#p09.AFAT*/ wire AFAT_APU_RSTn() const { return not1(KEBA_APU_RSTp()); }
  /*#p09.AGUR*/ wire AGUR_APU_RSTn() const { return not1(KEBA_APU_RSTp()); }
  /*#p11.CAMY*/ wire CAMY_APU_RSTn() const { return not1(KEBA_APU_RSTp()); }
  /*#p11.CEPO*/ wire CEPO_APU_RSTn() const { return not1(KEBA_APU_RSTp()); }
  /*#p19.CABE*/ wire CABE_APU_RSTn() const { return not1(KEBA_APU_RSTp()); }
  /*_p14.KYPU*/ wire KYPU_APU_RSTn() const { return not1(KEBA_APU_RSTp()); }
  /*_p14.FAZO*/ wire FAZO_APU_RSTn() const { return not1(KEBA_APU_RSTp()); }
  /*_p16.GOVE*/ wire GOVE_APU_RSTn() const { return not1(KEBA_APU_RSTp()); }
  /*#p11.HATO*/ wire HATO_APU_RSTn() const { return not1(KEBA_APU_RSTp()); }
  /*_p16.KOPY*/ wire KOPY_APU_RSTn() const { return not1(KEBA_APU_RSTp()); }
  /*_p16.HEKY*/ wire HEKY_APU_RSTn() const { return not1(KEBA_APU_RSTp()); }
  /*_p16.GAZE*/ wire GAZE_APU_RSTn() const { return not1(KEBA_APU_RSTp()); }
  /*#p18.CALU*/ wire CALU_APU_RSTn() const { return not1(KEBA_APU_RSTp()); }
  /*#p17.BAMA*/ wire BAMA_APU_RSTn() const { return not1(KEBA_APU_RSTp()); }
  /*#p17.ACOR*/ wire ACOR_APU_RSTn() const { return not1(KEBA_APU_RSTp()); }
  /*#p09.DAPA*/ wire DAPA_APU_RSTn() const { return not1(KEBA_APU_RSTp()); } // bavu crosses over this? why is it 3-rung?
  /*#p19.FEXO*/ wire FEXO_APU_RSTn() const { return not1(KEBA_APU_RSTp()); }
  /*#p19.HYNE*/ wire HYNE_APU_RSTn() const { return not1(KEBA_APU_RSTp()); }
  /*#p20.GASO*/ wire GASO_APU_RSTn() const { return not1(KEBA_APU_RSTp()); }
  /*#p20.FEBY*/ wire FEBY_APU_RSTn() const { return not1(KEBA_APU_RSTp()); }
  /*#p20.BOKY*/ wire BOKY_APU_RSTn() const { return not1(KEBA_APU_RSTp()); }
  /*#p13.DUKA*/ wire DUKA_APU_RSTn() const { return not1(KEBA_APU_RSTp()); }
  /*#p13.ERUM*/ wire ERUM_APU_RSTn() const { return not1(KEBA_APU_RSTp()); }
  /*#p13.KADO*/ wire KADO_APU_RSTn() const { return not1(KEBA_APU_RSTp()); }
  /*_p15.KATY*/ wire KATY_APU_RSTn() const { return not1(KEBA_APU_RSTp()); }
  /*_p16.GURO*/ wire GURO_APU_RSTn() const { return not1(KEBA_APU_RSTp()); }
  /*_p16.KUHA*/ wire KUHA_APU_RSTn() const { return not1(KEBA_APU_RSTp()); }
  /*#p16.GOMA*/ wire GOMA_APU_RSTn() const { return not1(KEBA_APU_RSTp()); }

  /*#p01.BATA*/ wire BATA_CLK_2M() const { return not1(AJER_CLK_2M.qp_new()); }
  /*_p01.BAVU*/ wire BAVU_CLK_1M() const { return not1(AVOK_CLK_1M.qp_new()); }
  /*#p16.FABO*/ wire FABO_CLK_xxCDxxGH() const { return not1(CERY_CLK_2M.qp_new()); }

  /*#p17.ABUR*/ wire ABUR_xxCDEFGx() const { return not1(BUKE_ABxxxxxH); }
  /*#p17.BORY*/ wire BORY_ABxxxxxH() const { return not1(ABUR_xxCDEFGx()); }


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

  /*_p10.TACE*/ wire TACE_AMP_ENn() const {
    return and4(ch1.HOCA_CH1_AMP_ENn(),
                ch2.FUTE_CH2_AMP_ENn(),
                ch3.GUXE_CH3_AMP_ENn.qp_new(),
                ch4.GEVY_CH4_AMP_ENn());
  }



  ///*#p17.BENA*/ wire BENA_CPU_WAVE_RDp() const { return  nand2(BOKE_CPU_RDp(), BARO_ADDR_FF3Xp()); }
  ///*#p17.CAZU*/ wire CAZU_CPU_WAVE_RDn() const { return  not1(BENA_CPU_WAVE_RDp()); }

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

  SpuChannel1 ch1;
  SpuChannel2 ch2;
  SpuChannel3 ch3;
  SpuChannel4 ch4;

  /*_BUS_CPU_A00p*/ Bus BUS_CPU_A00p;
  /*_BUS_CPU_A01p*/ Bus BUS_CPU_A01p;
  /*_BUS_CPU_A02p*/ Bus BUS_CPU_A02p;
  /*_BUS_CPU_A03p*/ Bus BUS_CPU_A03p;
  /*_BUS_CPU_A04p*/ Bus BUS_CPU_A04p;
  /*_BUS_CPU_A05p*/ Bus BUS_CPU_A05p;
  /*_BUS_CPU_A06p*/ Bus BUS_CPU_A06p;
  /*_BUS_CPU_A07p*/ Bus BUS_CPU_A07p;
  /*_BUS_CPU_A08p*/ Bus BUS_CPU_A08p;
  /*_BUS_CPU_A09p*/ Bus BUS_CPU_A09p;
  /*_BUS_CPU_A10p*/ Bus BUS_CPU_A10p;
  /*_BUS_CPU_A11p*/ Bus BUS_CPU_A11p;
  /*_BUS_CPU_A12p*/ Bus BUS_CPU_A12p;
  /*_BUS_CPU_A13p*/ Bus BUS_CPU_A13p;
  /*_BUS_CPU_A14p*/ Bus BUS_CPU_A14p;
  /*_BUS_CPU_A15p*/ Bus BUS_CPU_A15p;

  /*_BUS_WAVE_D00p*/ Bus BUS_WAVE_D00p;
  /*_BUS_WAVE_D01p*/ Bus BUS_WAVE_D01p;
  /*_BUS_WAVE_D02p*/ Bus BUS_WAVE_D02p;
  /*_BUS_WAVE_D03p*/ Bus BUS_WAVE_D03p;
  /*_BUS_WAVE_D04p*/ Bus BUS_WAVE_D04p;
  /*_BUS_WAVE_D05p*/ Bus BUS_WAVE_D05p;
  /*_BUS_WAVE_D06p*/ Bus BUS_WAVE_D06p;
  /*_BUS_WAVE_D07p*/ Bus BUS_WAVE_D07p;
};

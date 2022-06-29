/// plait_noparse

#pragma once

#include "GateBoyLib/Gates.h"
#include "GateBoyLib/Regs.h"

#include "GateBoyLib/GateBoyCh1.h"
#include "GateBoyLib/GateBoyCh2.h"
#include "GateBoyLib/GateBoyCh3.h"
#include "GateBoyLib/GateBoyCh4.h"

//==============================================================================
//             CONTROL
//==============================================================================

struct SpuControl {
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

  void tick();

  void tick_nr10();
  void tick_nr11();
  void tick_nr12();
  void tick_nr13();
  void tick_nr14();

  void tick_nr21();
  void tick_nr22();
  void tick_nr23();
  void tick_nr24();

  void tick_nr30();
  void tick_nr31();
  void tick_nr32();
  void tick_nr33();
  void tick_nr34();

  void tick_nr41();
  void tick_nr42();
  void tick_nr43();
  void tick_nr44();

  void tick_nr50();
  void tick_nr51();
  void tick_nr52();

  void tick_ch1();
  void tick_ch2();
  void tick_ch3();
  void tick_ch4();

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
  /*_p09.EDEK*/ wire EDEK_NR52_DBG_APUp() const { return not1(spu.FERO_NR52_DBG_APUn.qn_new()); }
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

  /*_p09.JYRO*/ wire JYRO_APU_RSTp() const { return  or2(HAPO_SYS_RSTp(), spu.HADA_NR52_ALL_SOUND_ON.qn_new()); }
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


  //----------

  /*_p10.DYTE*/ wire DYTE_A00n() const { return not1(BUS_CPU_A00p.out_new()); }
  /*_p10.AFOB*/ wire AFOB_A01n() const { return not1(BUS_CPU_A01p.out_new()); }
  /*_p10.ABUB*/ wire ABUB_A02n() const { return not1(BUS_CPU_A02p.out_new()); }
  /*_p10.ACOL*/ wire ACOL_A03n() const { return not1(BUS_CPU_A03p.out_new()); }
  /*#p10.ATUP*/ wire ATUP_A04n() const { return not1(BUS_CPU_A04p.out_new()); }
  /*#p10.BOXY*/ wire BOXY_A05n() const { return not1(BUS_CPU_A05p.out_new()); }
  /*#p10.ASAD*/ wire ASAD_A06n() const { return not1(BUS_CPU_A06p.out_new()); }
  /*#p10.AVUN*/ wire AVUN_A07n() const { return not1(BUS_CPU_A07p.out_new()); }

  /*_p10.DOSO*/ wire DOSO_A00p() const { return not1(DYTE_A00n()); }
  /*_p10.DUPA*/ wire DUPA_A01p() const { return not1(AFOB_A01n()); }
  /*_p10.DENO*/ wire DENO_A02p() const { return not1(ABUB_A02n()); }
  /*_p10.DUCE*/ wire DUCE_A03p() const { return not1(ACOL_A03n()); }

  /*_p10.ATEG*/ wire ATEG_ADDR_XX1Xn() const { return or4(BUS_CPU_A07p.out_new(), BUS_CPU_A06p.out_new(), BUS_CPU_A05p.out_new(), ATUP_A04n()); }
  /*_p10.AWET*/ wire AWET_ADDR_XX2Xn() const { return or4(BUS_CPU_A07p.out_new(), BUS_CPU_A06p.out_new(), BOXY_A05n(), BUS_CPU_A04p.out_new()); }
  /*#p10.ACOM*/ wire ACOM_ADDR_XX3Xn() const { return nand4(AVUN_A07n(), ASAD_A06n(), BUS_CPU_A05p.out_new(), BUS_CPU_A04p.out_new()); }

  /*_p07.BAKO*/ wire BAKO_ADDR_FFXXn() const { return not1(SYKE_ADDR_FFXXp); }

  /*_p10.BUNO*/ wire BUNO_ADDR_FF1Xp() const { return nor2(BAKO_ADDR_FFXXn(), ATEG_ADDR_XX1Xn()); }
  /*_p10.BEZY*/ wire BEZY_ADDR_FF2Xn() const { return  or2(AWET_ADDR_XX2Xn(), BAKO_ADDR_FFXXn()); }
  /*#p10.BARO*/ wire BARO_ADDR_FF3Xp() const { return nor2(ACOM_ADDR_XX3Xn(), BAKO_ADDR_FFXXn()); }

  /*_p10.BANU*/ wire BANU_ADDR_FF1Xn() const { return not1(BUNO_ADDR_FF1Xp()); }
  /*_p10.CONA*/ wire CONA_ADDR_FF2Xp() const { return not1(BEZY_ADDR_FF2Xn()); }

  //----------

  /*#p10.DUPO*/ wire DUPO_ADDR_0000n() const { return nand4(ACOL_A03n(), ABUB_A02n(), AFOB_A01n(), DYTE_A00n()); }
  /*#p10.DUNO*/ wire DUNO_ADDR_0001n() const { return nand4(ACOL_A03n(), ABUB_A02n(), AFOB_A01n(), DOSO_A00p()); }
  /*#p10.DAMY*/ wire DAMY_ADDR_0010n() const { return nand4(ACOL_A03n(), ABUB_A02n(), DUPA_A01p(), DYTE_A00n()); }
  /*#p10.ETUF*/ wire ETUF_ADDR_0011n() const { return nand4(ACOL_A03n(), ABUB_A02n(), DUPA_A01p(), DOSO_A00p()); }
  /*#p10.ESOT*/ wire ESOT_ADDR_0100n() const { return nand4(ACOL_A03n(), DENO_A02p(), AFOB_A01n(), DYTE_A00n()); }

  /*_p10.DAZA*/ wire DAZA_ADDR_0110n() const { return nand4(ACOL_A03n(), DENO_A02p(), DUPA_A01p(), DYTE_A00n()); }
  /*_p10.DUVU*/ wire DUVU_ADDR_0111n() const { return nand4(ACOL_A03n(), DENO_A02p(), DUPA_A01p(), DOSO_A00p()); }
  /*_p10.DAFY*/ wire DAFY_ADDR_1000n() const { return nand4(DUCE_A03p(), ABUB_A02n(), AFOB_A01n(), DYTE_A00n()); }
  /*_p10.DEJY*/ wire DEJY_ADDR_1001n() const { return nand4(DUCE_A03p(), ABUB_A02n(), AFOB_A01n(), DOSO_A00p()); }

  /*_p10.EXAT*/ wire EXAT_ADDR_1010n() const { return nand4(DUCE_A03p(), ABUB_A02n(), DUPA_A01p(), DYTE_A00n()); }
  /*_p10.EMAX*/ wire EMAX_ADDR_1011n() const { return nand4(DOSO_A00p(), DUPA_A01p(), ABUB_A02n(), DUCE_A03p()); }
  /*_p10.GANY*/ wire GANY_ADDR_1100n() const { return nand4(DUCE_A03p(), DENO_A02p(), AFOB_A01n(), DYTE_A00n()); }
  /*_p10.EMOS*/ wire EMOS_ADDR_1101n() const { return nand4(DOSO_A00p(), AFOB_A01n(), DENO_A02p(), DUCE_A03p()); }
  /*_p10.EGEN*/ wire EGEN_ADDR_1110n() const { return nand4(DUCE_A03p(), DENO_A02p(), DUPA_A01p(), DYTE_A00n()); }

  /*#p10.DONA*/ wire DONA_ADDR_0000n() const { return nand4(DYTE_A00n(), AFOB_A01n(), ABUB_A02n(), ACOL_A03n()); }
  /*#p10.DEWA*/ wire DEWA_ADDR_0001n() const { return nand4(DOSO_A00p(), AFOB_A01n(), ABUB_A02n(), ACOL_A03n()); }
  /*#p10.DOFA*/ wire DOFA_ADDR_0010p() const { return  and4(ACOL_A03n(), ABUB_A02n(), DUPA_A01p(), DYTE_A00n()); }
  /*#p10.DUFE*/ wire DUFE_ADDR_0011n() const { return nand4(DOSO_A00p(), DUPA_A01p(), ABUB_A02n(), ACOL_A03n()); }

  /*_p10.DATU*/ wire DATU_ADDR_0100n() const { return nand4(ACOL_A03n(), DENO_A02p(), AFOB_A01n(), DYTE_A00n()); }
  /*_p10.DURA*/ wire DURA_ADDR_0101n() const { return nand4(ACOL_A03n(), DENO_A02p(), AFOB_A01n(), DOSO_A00p()); }
  /*#p10.EKAG*/ wire EKAG_ADDR_0110p() const { return  and4(ACOL_A03n(), DENO_A02p(), DUPA_A01p(), DYTE_A00n()); }

  /*#p10.DYVA*/ wire DYVA_ADDR_FF10p() const { return nor2(BANU_ADDR_FF1Xn(), DUPO_ADDR_0000n()); }
  /*#p10.CAXE*/ wire CAXE_ADDR_FF11p() const { return nor2(BANU_ADDR_FF1Xn(), DUNO_ADDR_0001n()); }
  /*#p10.EDAF*/ wire EDAF_ADDR_FF12p() const { return nor2(BANU_ADDR_FF1Xn(), DAMY_ADDR_0010n()); }
  /*#p10.DECO*/ wire DECO_ADDR_FF13p() const { return nor2(BANU_ADDR_FF1Xn(), ETUF_ADDR_0011n()); }
  /*#p10.DUJA*/ wire DUJA_ADDR_FF14p() const { return nor2(BANU_ADDR_FF1Xn(), ESOT_ADDR_0100n()); }

  /*_p10.COVY*/ wire COVY_ADDR_FF16p() const { return nor2(BANU_ADDR_FF1Xn(), DAZA_ADDR_0110n()); }
  /*_p10.DUTU*/ wire DUTU_ADDR_FF17p() const { return nor2(BANU_ADDR_FF1Xn(), DUVU_ADDR_0111n()); }
  /*_p10.DARA*/ wire DARA_ADDR_FF18p() const { return nor2(BANU_ADDR_FF1Xn(), DAFY_ADDR_1000n()); }
  /*_p10.DOZA*/ wire DOZA_ADDR_FF19p() const { return nor2(BANU_ADDR_FF1Xn(), DEJY_ADDR_1001n()); }

  /*_p10.EMOR*/ wire EMOR_ADDR_FF1Ap() const { return nor2(BANU_ADDR_FF1Xn(), EXAT_ADDR_1010n()); }
  /*_p10.DUSA*/ wire DUSA_ADDR_FF1Bp() const { return nor2(BANU_ADDR_FF1Xn(), EMAX_ADDR_1011n()); }
  /*_p10.GEFO*/ wire GEFO_ADDR_FF1Cp() const { return nor2(BANU_ADDR_FF1Xn(), GANY_ADDR_1100n()); }
  /*_p10.FENY*/ wire FENY_ADDR_FF1Dp() const { return nor2(BANU_ADDR_FF1Xn(), EMOS_ADDR_1101n()); }
  /*_p10.DUGO*/ wire DUGO_ADDR_FF1Ep() const { return nor2(BANU_ADDR_FF1Xn(), EGEN_ADDR_1110n()); }

  /*#p10.DANU*/ wire DANU_ADDR_FF20p() const { return nor2(DONA_ADDR_0000n(), BEZY_ADDR_FF2Xn()); }
  /*#p10.COVO*/ wire COVO_ADDR_FF21p() const { return nor2(DEWA_ADDR_0001n(), BEZY_ADDR_FF2Xn()); }
  /*#p10.EKEZ*/ wire EKEZ_ADDR_FF22p() const { return and2(CONA_ADDR_FF2Xp(), DOFA_ADDR_0010p()); }
  /*#p10.CUGE*/ wire CUGE_ADDR_FF23p() const { return nor2(DUFE_ADDR_0011n(), BEZY_ADDR_FF2Xn()); }

  /*_p10.CAFY*/ wire CAFY_ADDR_FF24p() const { return nor2(BEZY_ADDR_FF2Xn(), DATU_ADDR_0100n()); }
  /*_p10.CORA*/ wire CORA_ADDR_FF25p() const { return nor2(BEZY_ADDR_FF2Xn(), DURA_ADDR_0101n()); }

  /*#p11.BUZE*/ wire BUZE_ADDR_FF10n() const { return not1(DYVA_ADDR_FF10p()); }
  /*#p11.HAXE*/ wire HAXE_ADDR_FF12n() const { return not1(EDAF_ADDR_FF12p()); }
  /*#p11.GAGO*/ wire GAGO_ADDR_FF12n() const { return not1(EDAF_ADDR_FF12p()); }
  /*#p11.CACA*/ wire CACA_ADDR_FF13n() const { return not1(DECO_ADDR_FF13p()); }
  /*#p11.CURE*/ wire CURE_ADDR_FF14n() const { return not1(DUJA_ADDR_FF14p()); }
  /*_p16.HOXA*/ wire HOXA_ADDR_FF1Dn() const { return not1(FENY_ADDR_FF1Dp()); }
  /*_p16.GUNU*/ wire GUNU_ADDR_FF1En() const { return not1(DUGO_ADDR_FF1Ep()); }
  /*_p09.BYMA*/ wire BYMA_ADDR_FF24n() const { return not1(CAFY_ADDR_FF24p()); }
  /*_p09.GEPA*/ wire GEPA_ADDR_FF25n() const { return not1(CORA_ADDR_FF25p()); }





  /*#p01.BATA*/ wire BATA_CLK_2M() const { return not1(spu.AJER_CLK_2M.qp_new()); }
  /*_p01.BAVU*/ wire BAVU_CLK_1M() const { return not1(spu.AVOK_CLK_1M.qp_new()); }
  /*#p16.FABO*/ wire FABO_CLK_xxCDxxGH() const { return not1(spu.CERY_CLK_2M.qp_new()); }

  /*#p17.ABUR*/ wire ABUR_xxCDEFGx() const { return not1(BUKE_ABxxxxxH); }
  /*#p17.BORY*/ wire BORY_ABxxxxxH() const { return not1(ABUR_xxCDEFGx()); }


  /*#p20.GYBA*/ wire GYBA_CLK_1M() const { return not1(BAVU_CLK_1M()); }


  /*#p01.HAMA*/ wire HAMA_CLK_512K() const { return not1(spu.JESO_CLK_512K.qp_new()); }
  /*_p01.BURE*/ wire BURE_CLK_512() const { return not1(spu.BARA_CLK_512.qp_new()); }

  /*_p01.HORU*/ wire HORU_CLK_512p() const {
    /*_p01.FYNE*/ wire FYNE_CLK_512 = not1(BURE_CLK_512());
    /*_p01.GALE*/ wire GALE_CLK_512 = mux2p(spu.FERO_NR52_DBG_APUn.qp_new(), HAMA_CLK_512K(), FYNE_CLK_512);
    /*_p01.GEXY*/ wire GEXY_CLK_512 = not1(GALE_CLK_512);
    /*_p01.HORU*/ wire HORU_CLK_512p = not1(GEXY_CLK_512);
    return HORU_CLK_512p;
  }

  /*_p01.BUFY*/ wire BUFY_CLK_256n() const {
    /*_p01.CULO*/ wire CULO_CLK_256 = not1(spu.CARU_CLK_256.qp_new());
    /*_p01.BEZE*/ wire BEZE_CLK_256 = mux2p(spu.FERO_NR52_DBG_APUn.qp_new(), HAMA_CLK_512K(), CULO_CLK_256);
    /*_p01.COFU*/ wire COFU_CLK_256 = not1(BEZE_CLK_256);
    /*_p01.BUFY*/ wire BUFY_CLK_256n = not1(COFU_CLK_256);
    return BUFY_CLK_256n;
  }

  /*_p01.BYFE*/ wire BYFE_CLK_128n() const {
    /*_p01.APEF*/ wire APEF_CLK_128 = not1(spu.BYLU_CLK_128.qp_new());
    /*_p01.BULE*/ wire BULE_CLK_128 = mux2p(spu.FERO_NR52_DBG_APUn.qp_new(), HAMA_CLK_512K(), APEF_CLK_128);
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



  /*#p17.BENA*/ wire BENA_CPU_WAVE_RDp() const { return  nand2(BOKE_CPU_RDp(), BARO_ADDR_FF3Xp()); }
  /*#p17.CAZU*/ wire CAZU_CPU_WAVE_RDn() const { return  not1(BENA_CPU_WAVE_RDp()); }



  SpuControl  spu;
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

  /*_BUS_CPU_D00p*/ Bus BUS_CPU_D00p;
  /*_BUS_CPU_D01p*/ Bus BUS_CPU_D01p;
  /*_BUS_CPU_D02p*/ Bus BUS_CPU_D02p;
  /*_BUS_CPU_D03p*/ Bus BUS_CPU_D03p;
  /*_BUS_CPU_D04p*/ Bus BUS_CPU_D04p;
  /*_BUS_CPU_D05p*/ Bus BUS_CPU_D05p;
  /*_BUS_CPU_D06p*/ Bus BUS_CPU_D06p;
  /*_BUS_CPU_D07p*/ Bus BUS_CPU_D07p;

  /*_BUS_WAVE_D00p*/ Bus BUS_WAVE_D00p;
  /*_BUS_WAVE_D01p*/ Bus BUS_WAVE_D01p;
  /*_BUS_WAVE_D02p*/ Bus BUS_WAVE_D02p;
  /*_BUS_WAVE_D03p*/ Bus BUS_WAVE_D03p;
  /*_BUS_WAVE_D04p*/ Bus BUS_WAVE_D04p;
  /*_BUS_WAVE_D05p*/ Bus BUS_WAVE_D05p;
  /*_BUS_WAVE_D06p*/ Bus BUS_WAVE_D06p;
  /*_BUS_WAVE_D07p*/ Bus BUS_WAVE_D07p;
};

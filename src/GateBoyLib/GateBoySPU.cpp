#include "GateBoySPU.h"

//-----------------------------------------------------------------------------

void GBSound::tick() {

  {
    //----------
    // SPU clock dividers

    /*_p01.CERY*/ spu.CERY_CLK_2M.dff17(CYBO_AxCxExGx(),          BELA_APU_RSTn(), spu.CERY_CLK_2M.qn_old());
    /*_p01.ATYK*/ spu.ATYK_CLK_2M.dff17(ARYF_AxCxExGx(),          BOPO_APU_RSTn(), spu.ATYK_CLK_2M.qn_old());
    /*_p09.AJER*/ spu.AJER_CLK_2M.dff17(APUV_AxCxExGx(),          ATYV_APU_RSTn(), spu.AJER_CLK_2M.qn_old());
    /*_p01.AVOK*/ spu.AVOK_CLK_1M.dff17(spu.ATYK_CLK_2M.qn_new(), BOPO_APU_RSTn(), spu.AVOK_CLK_1M.qn_old());

    /*#p09.CALO*/ ch1.CALO_CLK_1M.dff17(BATA_CLK_2M(), AGUR_APU_RSTn(), ch1.CALO_CLK_1M.qn_new());

    /*_p01.JESO*/ spu.JESO_CLK_512K.dff17(BAVU_CLK_1M(), KAME_APU_RSTn(), spu.JESO_CLK_512K.qn_old());

    /*_p15.ATEP*/ spu.ATEP_CLK_2M.dff17(AZEG_AxCxExGx(), BUWE_APU_RSTn(), spu.ATEP_CLK_2M.qn_old());
    /*#p15.BUFO*/   wire BUFO = not1(spu.ATEP_CLK_2M.qp_new());
    /*_p15.BYHO*/   wire BYHO_APU_RSTp = not1(BUWE_APU_RSTn());
    /*#p15.CEMO*/ spu.CEMO_CLK_1M.dff17(BUFO, BYHO_APU_RSTp, spu.CEMO_CLK_1M.qn_old());

    //----------
    // Low-speed clocks are picked up from DIV

    /*_p01.COKE*/ wire COKE_CLK_2M = not1(spu.AJER_CLK_2M.qn_new());
    /*_p01.BARA*/ spu.BARA_CLK_512.dff17(COKE_CLK_2M, ATUS_APU_RSTn(), UMER_DIV10n_old);
    /*_p01.CARU*/ spu.CARU_CLK_256.dff17(BURE_CLK_512(), ATUS_APU_RSTn(), spu.CARU_CLK_256.qn_old());
    /*_p01.BYLU*/ spu.BYLU_CLK_128.dff17(spu.CARU_CLK_256.qn_new(), ATUS_APU_RSTn(), spu.BYLU_CLK_128.qn_old());
  }

  tick_nr50();
  tick_nr51();
  tick_nr52();

  tick_ch1();
  tick_ch2();
  tick_ch3();
  tick_ch4();
}

//-----------------------------------------------------------------------------

void GBSound::tick_nr50() {
  /*_p09.BOSU*/ wire BOSU_NR50_WRn = nand2(CAFY_ADDR_FF24p(), BOGY_CPU_WRp());
  /*_p09.BAXY*/ wire BAXY_NR50_WRp = not1(BOSU_NR50_WRn);
  /*_p09.BOWE*/ wire BOWE_NR50_WRp = not1(BOSU_NR50_WRn);
  /*_p09.BUBU*/ wire BUBU_NR50_WRn = not1(BAXY_NR50_WRp);
  /*_p09.ATAF*/ wire ATAF_NR50_WRn = not1(BOWE_NR50_WRp);

  /*_p09.APEG*/ spu.APEG_NR50_VOL_L0   .dff9(ATAF_NR50_WRn, KEPY_APU_RSTn(), BUS_CPU_D00p.out_old());
  /*_p09.BYGA*/ spu.BYGA_NR50_VOL_L1   .dff9(ATAF_NR50_WRn, KEPY_APU_RSTn(), BUS_CPU_D01p.out_old());
  /*_p09.AGER*/ spu.AGER_NR50_VOL_L2   .dff9(ATAF_NR50_WRn, KEPY_APU_RSTn(), BUS_CPU_D02p.out_old());
  /*_p09.APOS*/ spu.APOS_NR50_VIN_TO_L .dff9(ATAF_NR50_WRn, KEPY_APU_RSTn(), BUS_CPU_D03p.out_old());
  /*_p09.BYRE*/ spu.BYRE_NR50_VOL_R0   .dff9(BUBU_NR50_WRn, KEPY_APU_RSTn(), BUS_CPU_D04p.out_old());
  /*_p09.BUMO*/ spu.BUMO_NR50_VOL_R1   .dff9(BUBU_NR50_WRn, KEPY_APU_RSTn(), BUS_CPU_D05p.out_old());
  /*_p09.COZU*/ spu.COZU_NR50_VOL_R2   .dff9(BUBU_NR50_WRn, KEPY_APU_RSTn(), BUS_CPU_D06p.out_old());
  /*_p09.BEDU*/ spu.BEDU_NR50_VIN_TO_R .dff9(BUBU_NR50_WRn, KEPY_APU_RSTn(), BUS_CPU_D07p.out_old());

  /*_p09.BEFU*/ wire BEFU_NR50_RDp = nor2(AGUZ_CPU_RDn(), BYMA_ADDR_FF24n());
  /*_p09.ADAK*/ wire ADAK_NR50_RDn = not1(BEFU_NR50_RDp);

  /*_p09.AKOD*/ triwire AKOD_D00 = tri6_nn(ADAK_NR50_RDn, spu.APEG_NR50_VOL_L0.qp_new()); // polarity?
  /*_p09.AWED*/ triwire AWED_D01 = tri6_nn(ADAK_NR50_RDn, spu.BYGA_NR50_VOL_L1.qp_new());
  /*_p09.AVUD*/ triwire AVUD_D02 = tri6_nn(ADAK_NR50_RDn, spu.AGER_NR50_VOL_L2.qp_new());
  /*_p09.AXEM*/ triwire AXEM_D03 = tri6_nn(ADAK_NR50_RDn, spu.APOS_NR50_VIN_TO_L.qp_new());
  /*_p09.AMAD*/ triwire AMAD_D04 = tri6_nn(ADAK_NR50_RDn, spu.BYRE_NR50_VOL_R0.qp_new());
  /*_p09.ARUX*/ triwire ARUX_D05 = tri6_nn(ADAK_NR50_RDn, spu.BUMO_NR50_VOL_R1.qp_new());
  /*_p09.BOCY*/ triwire BOCY_D06 = tri6_nn(ADAK_NR50_RDn, spu.COZU_NR50_VOL_R2.qp_new());
  /*_p09.ATUM*/ triwire ATUR_D07 = tri6_nn(ADAK_NR50_RDn, spu.BEDU_NR50_VIN_TO_R.qp_new());

  /*_BUS_CPU_D00p*/ BUS_CPU_D00p.tri_bus(AKOD_D00);
  /*_BUS_CPU_D01p*/ BUS_CPU_D01p.tri_bus(AWED_D01);
  /*_BUS_CPU_D02p*/ BUS_CPU_D02p.tri_bus(AVUD_D02);
  /*_BUS_CPU_D03p*/ BUS_CPU_D03p.tri_bus(AXEM_D03);
  /*_BUS_CPU_D04p*/ BUS_CPU_D04p.tri_bus(AMAD_D04);
  /*_BUS_CPU_D05p*/ BUS_CPU_D05p.tri_bus(ARUX_D05);
  /*_BUS_CPU_D06p*/ BUS_CPU_D06p.tri_bus(BOCY_D06);
  /*_BUS_CPU_D07p*/ BUS_CPU_D07p.tri_bus(ATUR_D07);
}

//-----------------------------------------------------------------------------

void GBSound::tick_nr51() {
  /*_p09.BUPO*/ wire BUPO_NR51_WRn = nand2(CORA_ADDR_FF25p(), BOGY_CPU_WRp());
  /*_p09.BONO*/ wire BONO_NR51_WRp = not1(BUPO_NR51_WRn);
  /*_p09.BYFA*/ wire BYFA_NR51_WRp = not1(BUPO_NR51_WRn);

  /*_p09.ANEV*/ spu.ANEV_NR51_D0.dff9(BONO_NR51_WRp, KEPY_APU_RSTn(), BUS_CPU_D00p.out_old());
  /*_p09.BOGU*/ spu.BOGU_NR51_D1.dff9(BONO_NR51_WRp, KEPY_APU_RSTn(), BUS_CPU_D01p.out_old());
  /*_p09.BAFO*/ spu.BAFO_NR51_D2.dff9(BONO_NR51_WRp, KEPY_APU_RSTn(), BUS_CPU_D02p.out_old());
  /*_p09.ATUF*/ spu.ATUF_NR51_D3.dff9(BONO_NR51_WRp, KEPY_APU_RSTn(), BUS_CPU_D03p.out_old());
  /*_p09.BUME*/ spu.BUME_NR51_D4.dff9(BYFA_NR51_WRp, KEPY_APU_RSTn(), BUS_CPU_D04p.out_old());
  /*_p09.BOFA*/ spu.BOFA_NR51_D5.dff9(BYFA_NR51_WRp, KEPY_APU_RSTn(), BUS_CPU_D05p.out_old());
  /*_p09.BEFO*/ spu.BEFO_NR51_D6.dff9(BYFA_NR51_WRp, KEPY_APU_RSTn(), BUS_CPU_D06p.out_old());
  /*_p09.BEPU*/ spu.BEPU_NR51_D7.dff9(BYFA_NR51_WRp, KEPY_APU_RSTn(), BUS_CPU_D07p.out_old());

  /*_p09.HEFA*/ wire HEFA_NR51_RDp = nor2(GEPA_ADDR_FF25n(), AGUZ_CPU_RDn()); // outline wrong color in die
  /*_p09.GUMU*/ wire GUMU_NR51_RDn = not1(HEFA_NR51_RDp);

  /*_p09.BUZU*/ triwire BUZU_D0 = tri6_nn(GUMU_NR51_RDn, spu.ANEV_NR51_D0.qn_new());
  /*_p09.CAPU*/ triwire CAPU_D1 = tri6_nn(GUMU_NR51_RDn, spu.BOGU_NR51_D1.qn_new());
  /*_p09.CAGA*/ triwire CAGA_D2 = tri6_nn(GUMU_NR51_RDn, spu.BAFO_NR51_D2.qn_new());
  /*_p09.BOCA*/ triwire BOCA_D3 = tri6_nn(GUMU_NR51_RDn, spu.ATUF_NR51_D3.qn_new());
  /*_p09.CAVU*/ triwire CAVU_D4 = tri6_nn(GUMU_NR51_RDn, spu.BUME_NR51_D4.qn_new());
  /*_p09.CUDU*/ triwire CUDU_D5 = tri6_nn(GUMU_NR51_RDn, spu.BOFA_NR51_D5.qn_new());
  /*_p09.CADA*/ triwire CADA_D6 = tri6_nn(GUMU_NR51_RDn, spu.BEFO_NR51_D6.qn_new());
  /*_p09.CERE*/ triwire CERE_D7 = tri6_nn(GUMU_NR51_RDn, spu.BEPU_NR51_D7.qn_new());

  /*_BUS_CPU_D00p*/ BUS_CPU_D00p.tri_bus(BUZU_D0);
  /*_BUS_CPU_D01p*/ BUS_CPU_D01p.tri_bus(CAPU_D1);
  /*_BUS_CPU_D02p*/ BUS_CPU_D02p.tri_bus(CAGA_D2);
  /*_BUS_CPU_D03p*/ BUS_CPU_D03p.tri_bus(BOCA_D3);
  /*_BUS_CPU_D04p*/ BUS_CPU_D04p.tri_bus(CAVU_D4);
  /*_BUS_CPU_D05p*/ BUS_CPU_D05p.tri_bus(CUDU_D5);
  /*_BUS_CPU_D06p*/ BUS_CPU_D06p.tri_bus(CADA_D6);
  /*_BUS_CPU_D07p*/ BUS_CPU_D07p.tri_bus(CERE_D7);
}

//-----------------------------------------------------------------------------

void GBSound::tick_nr52() {
  /*#p10.DOXY*/ wire DOXY_ADDR_FF26p = and2(CONA_ADDR_FF2Xp(), EKAG_ADDR_0110p()); // was this wrong on the schematic?

  /*#p09.ETUC*/ wire ETUC_NR52_WRp = and2(BOGY_CPU_WRp(), DOXY_ADDR_FF26p);
  /*#p09.FOKU*/ wire FOKU_NR52_WRn = not1(ETUC_NR52_WRp);
  /*#p09.BOPY*/ wire BOPY_NR52_WRn = nand2(BOGY_CPU_WRp(), DOXY_ADDR_FF26p);
  /*#p09.HAWU*/ wire HAWU_NR52_WRn = nand2(DOXY_ADDR_FF26p, BOGY_CPU_WRp());

  /*#p09.EFOP*/ wire EFOP_NR52_DBG_APU_IN = and2(BUS_CPU_D04p.out_old(), UNOR_MODE_DBG2);
  /*#p09.FERO*/ spu.FERO_NR52_DBG_APUn    .dff9 (FOKU_NR52_WRn, KEPY_APU_RSTn(), EFOP_NR52_DBG_APU_IN);
  /*#p09.BOWY*/ spu.BOWY_NR52_DBG_SWEEP   .dff17(BOPY_NR52_WRn, KEPY_APU_RSTn(), BUS_CPU_D05p.out_old());
  /*#p09.HADA*/ spu.HADA_NR52_ALL_SOUND_ON.dff17(HAWU_NR52_WRn, GUFO_SYS_RSTn(), BUS_CPU_D07p.out_old()); // Since this bit controls APU_RESET*, it is reset by SYS_RESET.

  /*#p09.DOLE*/ wire DOLE_NR52_RDn = nand2(DOXY_ADDR_FF26p, CETO_CPU_RDp());
  /*#p09.DURU*/ wire DURU_NR52_RDn = nand2(DOXY_ADDR_FF26p, CURU_CPU_RDp());
  /*#p09.FEWA*/ wire FEWA_NR52_RDn = nand2(DOXY_ADDR_FF26p, GAXO_CPU_RDp());
  /*#p09.KAMU*/ wire KAMU_NR52_RDn = nand2(DOXY_ADDR_FF26p, KAZO_CPU_RDp());
  /*#p09.JURE*/ wire JURE_NR52_RDn = nand2(KYDU_CPU_RDp(), DOXY_ADDR_FF26p);

  /*#p13.CARA*/ wire CARA_CH1_ACTIVEn = not1(ch1.CYTO_CH1_ACTIVEp.qp_new());
  /*#p15.DEFU*/ wire DEFU_CH2_ACTIVEn = not1(ch2.DANE_CH2_ACTIVEp.qp_new());
  /*#p18.ERED*/ wire ERED_CH3_ACTIVEn = not1(ch3.COKA_CH3_ACTIVEp());
  /*#p20.JUWA*/ wire JUWA_CH4_ACTIVEn = not1(ch4.GENA_CH4_ACTIVEp.qp_new());

  /*#p09.COTO*/ triwire COTO_D0 = tri6_nn(DOLE_NR52_RDn, CARA_CH1_ACTIVEn);
  /*#p09.EFUS*/ triwire EFUS_D1 = tri6_nn(DURU_NR52_RDn, DEFU_CH2_ACTIVEn);
  /*_p09.FATE*/ triwire FATE_D2 = tri6_nn(FEWA_NR52_RDn, ERED_CH3_ACTIVEn);
  /*_p09.KOGE*/ triwire KOGE_D3 = tri6_nn(KAMU_NR52_RDn, JUWA_CH4_ACTIVEn);
  /*_p09.HOPE*/ triwire HOPE_D7 = tri6_nn(JURE_NR52_RDn, spu.HADA_NR52_ALL_SOUND_ON.qn_new());

  /*_BUS_CPU_D00p*/ BUS_CPU_D00p.tri_bus(COTO_D0);
  /*_BUS_CPU_D01p*/ BUS_CPU_D01p.tri_bus(EFUS_D1);
  /*_BUS_CPU_D02p*/ BUS_CPU_D02p.tri_bus(FATE_D2);
  /*_BUS_CPU_D03p*/ BUS_CPU_D03p.tri_bus(KOGE_D3);
  /*_BUS_CPU_D07p*/ BUS_CPU_D07p.tri_bus(HOPE_D7);
}

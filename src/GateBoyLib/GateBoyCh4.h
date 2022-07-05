//==============================================================================
//             CHANNEL 4
//==============================================================================

struct SpuChannel4 {
  void reset_to_poweron() {}
  void reset_to_bootrom() {}
  void reset_to_cart() {}

  /*#p20.GEVY*/ wire GEVY_CH4_AMP_ENn() const { 
    return nor5(GEKY_NR42_ENV_DIR.qn_new(),
                GARU_NR42_ENV0.qn_new(),
                GOKY_NR42_ENV1.qn_new(),
                GOZO_NR42_ENV2.qn_new(),
                GEDU_NR42_ENV3.qn_new());
  }

  /*#p20.FOWA*/ wire FOWA_ENV_OFFp_old() const {
    return nor3(EMOK_NR42_ENV_TIMER0.qn_old(), ETYJ_NR42_ENV_TIMER1.qn_old(), EZYK_NR42_ENV_TIMER2.qn_old());
  }

  /*#p20.FOWA*/ wire FOWA_ENV_OFFp_new() const {
    return nor3(EMOK_NR42_ENV_TIMER0.qn_new(), ETYJ_NR42_ENV_TIMER1.qn_new(), EZYK_NR42_ENV_TIMER2.qn_new());
  }

#if 0

  /*#p20.FEME*/ bool FEME_LFSR_CLKp() const {
    /*_p20.EMOF*/ wire EMOF_LFSR_CLK_MUX_7 = nor3(FETA_NR43_FREQ0.qn_new(), FYTO_NR43_FREQ1.qn_new(), GOGO_NR43_FREQ2.qn_new());
    /*_p20.ELAR*/ wire ELAR_LFSR_CLK_MUX_6 = nor3(FETA_NR43_FREQ0.qn_new(), FYTO_NR43_FREQ1.qn_new(), GOGO_NR43_FREQ2.qp_new());
    /*_p20.DUDU*/ wire DUDU_LFSR_CLK_MUX_5 = nor3(FETA_NR43_FREQ0.qn_new(), FYTO_NR43_FREQ1.qp_new(), GOGO_NR43_FREQ2.qn_new());
    /*_p20.ETAT*/ wire ETAT_LFSR_CLK_MUX_4 = nor3(FETA_NR43_FREQ0.qn_new(), FYTO_NR43_FREQ1.qp_new(), GOGO_NR43_FREQ2.qp_new());
    /*_p20.FURA*/ wire FURA_LFSR_CLK_MUX_3 = nor3(FETA_NR43_FREQ0.qp_new(), FYTO_NR43_FREQ1.qn_new(), GOGO_NR43_FREQ2.qn_new());
    /*_p20.ETAR*/ wire ETAR_LFSR_CLK_MUX_2 = nor3(FETA_NR43_FREQ0.qp_new(), FYTO_NR43_FREQ1.qn_new(), GOGO_NR43_FREQ2.qp_new());
    /*_p20.EVER*/ wire EVER_LFSR_CLK_MUX_1 = nor3(FETA_NR43_FREQ0.qp_new(), FYTO_NR43_FREQ1.qp_new(), GOGO_NR43_FREQ2.qn_new());
    /*_p20.ETOV*/ wire ETOV_LFSR_CLK_MUX_0 = nor3(FETA_NR43_FREQ0.qp_new(), FYTO_NR43_FREQ1.qp_new(), GOGO_NR43_FREQ2.qp_new());

    /*_p20.ETYR*/ wire ETYR_LFSR_CLK_MUX_A = amux6(ESEP_FREQ_13.qp_new(), DUDU_LFSR_CLK_MUX_5,
                                                   DERE_FREQ_12.qp_new(), ETAT_LFSR_CLK_MUX_4,
                                                   DOTA_FREQ_11.qp_new(), FURA_LFSR_CLK_MUX_3,
                                                   ERUT_FREQ_10.qp_new(), ETAR_LFSR_CLK_MUX_2,
                                                   DETE_FREQ_09.qp_new(), EVER_LFSR_CLK_MUX_1,
                                                   DOSE_FREQ_08.qp_new(), ETOV_LFSR_CLK_MUX_0);

    /*_p20.ELYX*/ wire ELYX_LFSR_CLK_MUX_B = amux4(DEMO_FREQ_07.qp_new(), EMOF_LFSR_CLK_MUX_7,
                                                   DOKE_FREQ_06.qp_new(), ELAR_LFSR_CLK_MUX_6,
                                                   DALE_FREQ_05.qp_new(), DUDU_LFSR_CLK_MUX_5,
                                                   DURE_FREQ_04.qp_new(), ETAT_LFSR_CLK_MUX_4);

    /*_p20.DARY*/ wire DARY_LFSR_CLK_MUX_C = amux4(EPOR_FREQ_03.qp_new(), FURA_LFSR_CLK_MUX_3,
                                                   EZEF_FREQ_02.qp_new(), ETAR_LFSR_CLK_MUX_2,
                                                   DEKO_FREQ_01.qp_new(), EVER_LFSR_CLK_MUX_1,
                                                   CEXO_FREQ_00.qp_new(), ETOV_LFSR_CLK_MUX_0);

    /*_p20.ERYF*/ wire ERYF_LFSR_CLK_MUX_D = or2(ELYX_LFSR_CLK_MUX_B, DARY_LFSR_CLK_MUX_C);


    /*#p20.FEME*/ wire FEME_LFSR_CLKp = mux2p(GAFO_NR43_FREQ3.qn_new(), ETYR_LFSR_CLK_MUX_A, ERYF_LFSR_CLK_MUX_D);
    return FEME_LFSR_CLKp;
  }

  /*#p20.JYJA*/ wire JYJA_LFSR_CLKn() const { return not1(FEME_LFSR_CLKp()); }
  /*#p20.GUFA*/ wire GUFA_LFSR_CLKn() const { return not1(FEME_LFSR_CLKp()); }
  /*#p20.GYVE*/ wire GYVE_LFSR_CLKp() const { return not1(GUFA_LFSR_CLKn()); }
  /*#p20.KARA*/ wire KARA_LFSR_CLKn() const { return not1(GYVE_LFSR_CLKp()); }
  /*#p20.KOPA*/ wire KOPA_LFSR_CLKp() const { return not1(KARA_LFSR_CLKn()); }

#endif


  /*_p19.EMOK*/ DFF9 EMOK_NR42_ENV_TIMER0;
  /*_p19.ETYJ*/ DFF9 ETYJ_NR42_ENV_TIMER1;
  /*_p19.EZYK*/ DFF9 EZYK_NR42_ENV_TIMER2;
  /*_p19.GEKY*/ DFF9 GEKY_NR42_ENV_DIR;
  /*_p19.GARU*/ DFF9 GARU_NR42_ENV0;
  /*_p19.GOKY*/ DFF9 GOKY_NR42_ENV1;
  /*_p19.GOZO*/ DFF9 GOZO_NR42_ENV2;
  /*_p19.GEDU*/ DFF9 GEDU_NR42_ENV3;

  //----------

  /*_p19.JARE*/ DFF9 JARE_NR43_DIV0;
  /*_p19.JERO*/ DFF9 JERO_NR43_DIV1;
  /*_p19.JAKY*/ DFF9 JAKY_NR43_DIV2;
  /*_p19.JAMY*/ DFF9 JAMY_NR43_MODE;
  /*_p19.FETA*/ DFF9 FETA_NR43_FREQ0;
  /*_p19.FYTO*/ DFF9 FYTO_NR43_FREQ1;
  /*_p19.GOGO*/ DFF9 GOGO_NR43_FREQ2;
  /*_p19.GAFO*/ DFF9 GAFO_NR43_FREQ3;

  /*_p20.GENA*/ NorLatch GENA_CH4_ACTIVEp;

  //========================================
  //             CHANNEL 4
  //========================================

  /*_p20.GONE*/ DFF17 GONE_CH4_TRIGp;
  /*_p20.GORA*/ DFF17 GORA_CH4_TRIGp;
  /*_p20.GATY*/ DFF17 GATY_CH4_TRIGp;
  /*_p19.FUGO*/ DFF17 FUGO_CH4_LEN_DONEp;

  /*#p19.DANO*/ DFF20 DANO_NR41_LEN0;
  /*#p19.FAVY*/ DFF20 FAVY_NR41_LEN1;
  /*#p19.DENA*/ DFF20 DENA_NR41_LEN2;
  /*#p19.CEDO*/ DFF20 CEDO_NR41_LEN3;
  /*#p19.FYLO*/ DFF20 FYLO_NR41_LEN4;
  /*_p19.EDOP*/ DFF20 EDOP_NR41_LEN5;

  /*#p19.CUNY*/ DFF9 CUNY_NR44_LEN_ENp;
  /*#p19.HOGA*/ DFF9 HOGA_NR44_TRIG;
  /*#p20.GYSU*/ DFF17 GYSU_CH4_TRIG;

  /*#p20.HAZO*/ NorLatch HAZO_CH4_TRIGn;
  /*#p20.JERY*/ NandLatch JERY_DIV_GATE;
  /*#p20.JYCO*/ DFF20 JYCO_DIV0;
  /*#p20.JYRE*/ DFF20 JYRE_DIV1;
  /*#p20.JYFU*/ DFF20 JYFU_DIV2;
  /*#p20.GARY*/ DFF17 GARY_FREQ_GATEp;

  /*_p20.CEXO*/ DFF17 CEXO_FREQ_00;
  /*_p20.DEKO*/ DFF17 DEKO_FREQ_01;
  /*_p20.EZEF*/ DFF17 EZEF_FREQ_02;
  /*_p20.EPOR*/ DFF17 EPOR_FREQ_03;
  /*_p20.DURE*/ DFF17 DURE_FREQ_04;
  /*_p20.DALE*/ DFF17 DALE_FREQ_05;
  /*_p20.DOKE*/ DFF17 DOKE_FREQ_06;
  /*_p20.DEMO*/ DFF17 DEMO_FREQ_07;
  /*_p20.DOSE*/ DFF17 DOSE_FREQ_08;
  /*_p20.DETE*/ DFF17 DETE_FREQ_09;
  /*_p20.ERUT*/ DFF17 ERUT_FREQ_10;
  /*_p20.DOTA*/ DFF17 DOTA_FREQ_11;
  /*_p20.DERE*/ DFF17 DERE_FREQ_12;
  /*_p20.ESEP*/ DFF17 ESEP_FREQ_13;

  /*_p20.JOTO*/ DFF17 JOTO_LFSR_00;
  /*_p20.KOMU*/ DFF17 KOMU_LFSR_01;
  /*_p20.KETU*/ DFF17 KETU_LFSR_02;
  /*_p20.KUTA*/ DFF17 KUTA_LFSR_03;
  /*_p20.KUZY*/ DFF17 KUZY_LFSR_04;
  /*_p20.KYWY*/ DFF17 KYWY_LFSR_05;
  /*_p20.JAJU*/ DFF17 JAJU_LFSR_06;
  /*_p20.HAPE*/ DFF17 HAPE_LFSR_07;
  /*_p20.JUXE*/ DFF17 JUXE_LFSR_08;
  /*_p20.JEPE*/ DFF17 JEPE_LFSR_09;
  /*_p20.JAVO*/ DFF17 JAVO_LFSR_10;
  /*_p20.HEPA*/ DFF17 HEPA_LFSR_11;
  /*_p20.HORY*/ DFF17 HORY_LFSR_12;
  /*_p20.HENO*/ DFF17 HENO_LFSR_13;
  /*_p20.HYRO*/ DFF17 HYRO_LFSR_14;
  /*_p20.HEZU*/ DFF17 HEZU_LFSR_15;

  /*#p20.ABEL*/ DFF17 ABEL_CLK_64;
  /*#p20.CUNA*/ DFF20 CUNA_ENV_DELAY0n;
  /*#p20.COFE*/ DFF20 COFE_ENV_DELAY1n;
  /*#p20.DOGO*/ DFF20 DOGO_ENV_DELAY2n;

  /*#p20.FOSY*/ DFF17 FOSY_ENV_CLKp;
  /*#p20.FYNO*/ DFF17 FYNO_ENV_MAXp;

  /*#p20.FEKO*/ DFF20 FEKO_CH4_VOL0;
  /*#p20.FATY*/ DFF20 FATY_CH4_VOL1;
  /*#p20.FERU*/ DFF20 FERU_CH4_VOL2;
  /*#p20.FYRO*/ DFF20 FYRO_CH4_VOL3;
  /*#p20.EROX*/ NorLatch EROX_ENV_RUNNINGn;
};










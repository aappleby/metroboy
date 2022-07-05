//==============================================================================
//             CHANNEL 2
//==============================================================================

struct SpuChannel2 {
  void reset_to_poweron() {}
  void reset_to_bootrom() {}
  void reset_to_cart() {}

  /*#p15.ETUK*/ wire ETUK_SAMPLE_CLKn_old() const { return not1(GYKO_SAMPLE_CLKp.qp_old()); }
  /*#p15.DAVU*/ wire DAVU_SAMPLE_CLKp_old() const { return not1(ETUK_SAMPLE_CLKn_old()); }
  /*#p15.CULE*/ wire CULE_SAMPLE_CLKn_old() const { return not1(DAVU_SAMPLE_CLKp_old()); }

  /*#p15.ETUK*/ wire ETUK_SAMPLE_CLKn_new() const { return not1(GYKO_SAMPLE_CLKp.qp_new()); }
  /*#p15.DAVU*/ wire DAVU_SAMPLE_CLKp_new() const { return not1(ETUK_SAMPLE_CLKn_new()); }
  /*#p15.CULE*/ wire CULE_SAMPLE_CLKn_new() const { return not1(DAVU_SAMPLE_CLKp_new()); }

  /*#p15.FUTE*/ wire FUTE_CH2_AMP_ENn_new() const {
    return nor5(FORE_NR22_ADD.qn_new(),
                GATA_NR22_V0.qn_new(),
                GUFE_NR22_V1.qn_new(),
                GURA_NR22_V2.qn_new(),
                GAGE_NR22_V3.qn_new());
  }

  /*#p15.JUPU*/ wire JUPU_ENV_OFFp_old() const { return nor3(HYFU_NR22_P0n.qn_old(), HORE_NR22_P1n.qn_old(), HAVA_NR22_P2n.qn_old()); }
  /*#p15.JUPU*/ wire JUPU_ENV_OFFp_new() const { return nor3(HYFU_NR22_P0n.qn_new(), HORE_NR22_P1n.qn_new(), HAVA_NR22_P2n.qn_new()); }


  /*_p15.ERYC*/ DFF20 ERYC_NR21_L0;
  /*_p15.CERA*/ DFF20 CERA_NR21_L1;
  /*_p15.CONU*/ DFF20 CONU_NR21_L2;
  /*_p15.CAME*/ DFF20 CAME_NR21_L3;
  /*_p15.BUVA*/ DFF20 BUVA_NR21_L4;
  /*_p15.AKYD*/ DFF20 AKYD_NR21_L5;
  /*_p14.BERA*/ DFF9  BERA_NR21_D6;
  /*_p14.BAMY*/ DFF9  BAMY_NR21_D7;

  //----------
  // FF17 NR22 - channel 2 vol/env/period

  /*_p14.HYFU*/ DFF9 HYFU_NR22_P0n;
  /*_p14.HORE*/ DFF9 HORE_NR22_P1n;
  /*_p14.HAVA*/ DFF9 HAVA_NR22_P2n;
  /*_p14.FORE*/ DFF9 FORE_NR22_ADD;
  /*_p14.GATA*/ DFF9 GATA_NR22_V0;
  /*_p14.GUFE*/ DFF9 GUFE_NR22_V1;
  /*_p14.GURA*/ DFF9 GURA_NR22_V2;
  /*_p14.GAGE*/ DFF9 GAGE_NR22_V3;

  //----------

  DFF9 FOFE_NR23_FREQ_00;
  DFF9 FOVA_NR23_FREQ_01;
  DFF9 FEDY_NR23_FREQ_02;
  DFF9 FOME_NR23_FREQ_03;
  DFF9 FORA_NR23_FREQ_04;
  DFF9 GODA_NR23_FREQ_05;
  DFF9 GUMY_NR23_FREQ_06;
  DFF9 GUPU_NR23_FREQ_07;

  //----------

  DFF9 JEFU_NR24_FREQ_08;
  DFF9 JANY_NR24_FREQ_09;
  DFF9 JUPY_NR24_FREQ_10;

  /*_p15.DANE*/ NorLatch DANE_CH2_ACTIVEp;

  DFF20 JORE_CH2_ENV_DELAYp;
  DFF20 JONA_CH2_ENV_DELAYp;
  DFF20 JEVY_CH2_ENV_DELAYp;
  DFF17 JOPA_CH2_ENV_TICK;

  DFF20 DONE_CH2_FREQ_00;
  DFF20 DYNU_CH2_FREQ_01;
  DFF20 EZOF_CH2_FREQ_02;
  DFF20 CYVO_CH2_FREQ_03;
  DFF20 FUXO_CH2_FREQ_04;
  DFF20 GANO_CH2_FREQ_05;
  DFF20 GOCA_CH2_FREQ_06;
  DFF20 GANE_CH2_FREQ_07;
  DFF20 HEVY_CH2_FREQ_08;
  DFF20 HEPU_CH2_FREQ_09;
  DFF20 HERO_CH2_FREQ_10;

  DFF17 DOME_CH2_OUTp;

  DFF17 ELOX_CH2_TRIGp;
  DFF17 CAZA_CH2_TRIGp;
  DFF9 EMER_NR24_LENENp;
  DFF17 DOPE_CH2_TRIGp;

  NorLatch DALA_CH2_TRIGp;

  DFF9 ETAP_NR24_TRIGp;
  DFF17 DORY_CH2_TRIGp;
  DFF17 GYKO_SAMPLE_CLKp;
  /*#p15.CANO*/ DFF17 CANO_00;
  /*#p15.CAGY*/ DFF13 CAGY_01;
  /*#p15.DYVE*/ DFF13 DYVE_02;
  NandLatch BUTA_FREQ_GATEp;
  DFF20 FENA_CH2_ENV3;
  DFF20 FOMY_CH2_ENV2;
  DFF20 FETE_CH2_ENV1;
  DFF20 FENO_CH2_ENV0;
  DFF17 HEPO_ENV_MAXp;
  NorLatch JEME_ENV_DONEp;

  DFF17 JYNA_CLK_64n;
  /*_p15.CYRE*/ DFF17 CYRE_CH2_LEN_DONEp;
};












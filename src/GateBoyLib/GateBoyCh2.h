//==============================================================================
//             CHANNEL 2
//==============================================================================

struct SpuChannel2 {
  void reset_to_cart();

  /*#p15.ETUK*/ wire ETUK_SAMPLE_CLKn_old() const { return not1(GYKO_SAMPLE_CLKp.qp_old()); }
  /*#p15.DAVU*/ wire DAVU_SAMPLE_CLKp_old() const { return not1(ETUK_SAMPLE_CLKn_old()); }
  /*#p15.CULE*/ wire CULE_SAMPLE_CLKn_old() const { return not1(DAVU_SAMPLE_CLKp_old()); }

  /*#p15.ETUK*/ wire ETUK_SAMPLE_CLKn_new() const { return not1(GYKO_SAMPLE_CLKp.qp_new()); }
  /*#p15.DAVU*/ wire DAVU_SAMPLE_CLKp_new() const { return not1(ETUK_SAMPLE_CLKn_new()); }
  /*#p15.CULE*/ wire CULE_SAMPLE_CLKn_new() const { return not1(DAVU_SAMPLE_CLKp_new()); }

  /*#p15.FUTE*/ wire FUTE_CH2_AMP_ENn_new() const {
    return nor5(FORE_NR22_ADDp.qp_newB(),
                GATA_NR22_V0p.qp_newB(),
                GUFE_NR22_V1p.qp_newB(),
                GURA_NR22_V2p.qp_newB(),
                GAGE_NR22_V3p.qp_newB());
  }

  /*#p15.JUPU*/ wire JUPU_ENV_OFFp_old() const { return nor3(HYFU_NR22_P0p.qp_oldB(), HORE_NR22_P1p.qp_oldB(), HAVA_NR22_P2p.qp_oldB()); }
  /*#p15.JUPU*/ wire JUPU_ENV_OFFp_new() const { return nor3(HYFU_NR22_P0p.qp_newB(), HORE_NR22_P1p.qp_newB(), HAVA_NR22_P2p.qp_newB()); }


  /*_p15.ERYC*/ DFF20 ERYC_NR21_LEN0;
  /*_p15.CERA*/ DFF20 CERA_NR21_LEN1;
  /*_p15.CONU*/ DFF20 CONU_NR21_LEN2;
  /*_p15.CAME*/ DFF20 CAME_NR21_LEN3;
  /*_p15.BUVA*/ DFF20 BUVA_NR21_LEN4;
  /*_p15.AKYD*/ DFF20 AKYD_NR21_LEN5;
  /*_p14.BERA*/ DFF9B BERA_NR21_DUTY0;
  /*_p14.BAMY*/ DFF9B BAMY_NR21_DUTY1;

  //----------
  // FF17 NR22 - channel 2 vol/env/period

  /*_p14.HYFU*/ DFF9B HYFU_NR22_P0p;
  /*_p14.HORE*/ DFF9B HORE_NR22_P1p;
  /*_p14.HAVA*/ DFF9B HAVA_NR22_P2p;
  /*_p14.FORE*/ DFF9B FORE_NR22_ADDp;
  /*_p14.GATA*/ DFF9B GATA_NR22_V0p;
  /*_p14.GUFE*/ DFF9B GUFE_NR22_V1p;
  /*_p14.GURA*/ DFF9B GURA_NR22_V2p;
  /*_p14.GAGE*/ DFF9B GAGE_NR22_V3p;

  //----------

  DFF9B FOFE_NR23_FREQ00p;
  DFF9B FOVA_NR23_FREQ01p;
  DFF9B FEDY_NR23_FREQ02p;
  DFF9B FOME_NR23_FREQ03p;
  DFF9B FORA_NR23_FREQ04p;
  DFF9B GODA_NR23_FREQ05p;
  DFF9B GUMY_NR23_FREQ06p;
  DFF9B GUPU_NR23_FREQ07p;

  //----------

  DFF9B JEFU_NR24_FREQ08p;
  DFF9B JANY_NR24_FREQ09p;
  DFF9B JUPY_NR24_FREQ10p;

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
  DFF9B EMER_NR24_LENENp;
  DFF17 DOPE_CH2_TRIGp;

  NorLatch DALA_CH2_TRIGp;

  DFF9B ETAP_NR24_TRIGp;
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













//==============================================================================
//             CHANNEL 2
//==============================================================================

struct GateBoyState;
int ch2_audio_out(const GateBoyState& reg_new);

struct SpuChannel2 {
  void reset_to_cart();

  /*#p15.FUTE*/ wire FUTE_CH2_AMP_ENn_new() const {
    return nor5(FORE_NR22_ADDp.qp_newB(),
                GATA_NR22_V0p.qp_newB(),
                GUFE_NR22_V1p.qp_newB(),
                GURA_NR22_V2p.qp_newB(),
                GAGE_NR22_V3p.qp_newB());
  }

  //----------
  // FF16

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
  // FF18

  DFF9B FOFE_NR23_FREQ00p;
  DFF9B FOVA_NR23_FREQ01p;
  DFF9B FEDY_NR23_FREQ02p;
  DFF9B FOME_NR23_FREQ03p;
  DFF9B FORA_NR23_FREQ04p;
  DFF9B GODA_NR23_FREQ05p;
  DFF9B GUMY_NR23_FREQ06p;
  DFF9B GUPU_NR23_FREQ07p;

  //----------
  // FF19

  DFF9B JEFU_NR24_FREQ08p;
  DFF9B JANY_NR24_FREQ09p;
  DFF9B JUPY_NR24_FREQ10p;
  DFF9B EMER_NR24_LENENp;
  DFF9B ETAP_NR24_TRIGp;

  //----------

  DFF20 DONE_COUNT00; 
  DFF20 DYNU_COUNT01; //o30
  DFF20 EZOF_COUNT02;
  DFF20 CYVO_COUNT03;
  DFF20 FUXO_COUNT04;
  DFF20 GANO_COUNT05;
  DFF20 GOCA_COUNT06;
  DFF20 GANE_COUNT07;
  DFF20 HEVY_COUNT08;
  DFF20 HEPU_COUNT09;
  DFF20 HERO_COUNT10;

  /*#p15.ELOX*/ DFF17     ELOX_TRIGp;
  /*#p15.CAZA*/ DFF17     CAZA_TRIGp;
  /*#p15.DOPE*/ DFF17     DOPE_TRIGp;
  /*#p15.DALA*/ NorLatch  DALA_TRIGp;
  /*#p15.DORY*/ DFF17     DORY_TRIGp;

  /*_p15.CYRE*/ DFF17     CYRE_LEN_DONEp;

  /*#p15.JYNA*/ DFF17     JYNA_ENV_CLK64;
  /*#p15.JOPA*/ DFF17     JOPA_ENV_TICK;
  /*#p15.HEPO*/ DFF17     HEPO_ENV_MAXp;
  /*#p15.JEME*/ NorLatch  JEME_ENV_DONEp;

  DFF20 JORE_ENV_DELAY0p;
  DFF20 JONA_ENV_DELAY1p;
  DFF20 JEVY_ENV_DELAY2p;

  DFF20 FENO_ENV_VOL0;
  DFF20 FETE_ENV_VOL1;
  DFF20 FOMY_ENV_VOL2;
  DFF20 FENA_ENV_VOL3;

  /*#p15.CANO*/ DFF17     CANO_PHASE0;
  /*#p15.CAGY*/ DFF13     CAGY_PHASE1;
  /*#p15.DYVE*/ DFF13     DYVE_PHASE2;

  /*_p15.DANE*/ NorLatch  DANE_CH2_ACTIVEp;
  /*_p15.BUTA*/ NandLatch BUTA_FREQ_GATEp;
  /*#p15.DOME*/ DFF17     DOME_CH2_OUTp;
  /*#p15.GYKO*/ DFF17     GYKO_SAMPLE_CLKp;
};













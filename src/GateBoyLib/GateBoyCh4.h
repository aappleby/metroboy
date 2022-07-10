//==============================================================================
//             CHANNEL 4
//==============================================================================

struct GateBoyState;
int ch4_audio_out(const GateBoyState& reg_new);

struct SpuChannel4 {
  void reset_to_cart();

  /*#p20.GEVY*/ wire GEVY_CH4_AMP_ENn() const { 
    return nor5(GEKY_NR42_ENV_DIRp.qp_newB(),
                GARU_NR42_ENV0p.qp_newB(),
                GOKY_NR42_ENV1p.qp_newB(),
                GOZO_NR42_ENV2p.qp_newB(),
                GEDU_NR42_ENV3p.qp_newB());
  }

  /*#p20.FOWA*/ wire FOWA_ENV_OFFp_old() const {
    return nor3(EMOK_NR42_ENV_TIMER0p.qp_oldB(), ETYJ_NR42_ENV_TIMER1p.qp_oldB(), EZYK_NR42_ENV_TIMER2p.qp_oldB());
  }

  /*#p20.FOWA*/ wire FOWA_ENV_OFFp_new() const {
    return nor3(EMOK_NR42_ENV_TIMER0p.qp_newB(), ETYJ_NR42_ENV_TIMER1p.qp_newB(), EZYK_NR42_ENV_TIMER2p.qp_newB());
  }

  /*_p19.EMOK*/ DFF9B EMOK_NR42_ENV_TIMER0p;
  /*_p19.ETYJ*/ DFF9B ETYJ_NR42_ENV_TIMER1p;
  /*_p19.EZYK*/ DFF9B EZYK_NR42_ENV_TIMER2p;
  /*_p19.GEKY*/ DFF9B GEKY_NR42_ENV_DIRp;
  /*_p19.GARU*/ DFF9B GARU_NR42_ENV0p;
  /*_p19.GOKY*/ DFF9B GOKY_NR42_ENV1p;
  /*_p19.GOZO*/ DFF9B GOZO_NR42_ENV2p;
  /*_p19.GEDU*/ DFF9B GEDU_NR42_ENV3p;

  //----------

  /*_p19.JARE*/ DFF9B JARE_NR43_DIV0p;
  /*_p19.JERO*/ DFF9B JERO_NR43_DIV1p;
  /*_p19.JAKY*/ DFF9B JAKY_NR43_DIV2p;
  /*_p19.JAMY*/ DFF9B JAMY_NR43_MODEp;
  /*_p19.FETA*/ DFF9B FETA_NR43_FREQ0p;
  /*_p19.FYTO*/ DFF9B FYTO_NR43_FREQ1p;
  /*_p19.GOGO*/ DFF9B GOGO_NR43_FREQ2p;
  /*_p19.GAFO*/ DFF9B GAFO_NR43_FREQ3p;

  /*_p20.GENA*/ NorLatch GENA_CH4_ACTIVEp;

  //========================================
  //             CHANNEL 4
  //========================================

  /*_p20.GONE*/ DFF17 GONE_CH4_TRIGp;
  /*_p20.GORA*/ DFF17 GORA_CH4_TRIGp;
  /*_p20.GATY*/ DFF17 GATY_CH4_TRIGp;
  /*_p19.FUGO*/ DFF17 FUGO_CH4_LEN_DONEp;

  /*#p19.DANO*/ DFF20 DANO_NR41_LEN0p;
  /*#p19.FAVY*/ DFF20 FAVY_NR41_LEN1p;
  /*#p19.DENA*/ DFF20 DENA_NR41_LEN2p;
  /*#p19.CEDO*/ DFF20 CEDO_NR41_LEN3p;
  /*#p19.FYLO*/ DFF20 FYLO_NR41_LEN4p;
  /*_p19.EDOP*/ DFF20 EDOP_NR41_LEN5p;

  /*#p19.CUNY*/ DFF9B CUNY_NR44_LEN_ENp;
  /*#p19.HOGA*/ DFF9B HOGA_NR44_TRIGp;
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










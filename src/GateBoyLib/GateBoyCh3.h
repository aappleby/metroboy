
struct GateBoyState;

//==============================================================================
//             CHANNEL 3
//==============================================================================

struct SpuChannel3 {
  void reset_to_poweron() {}
  void reset_to_bootrom() {}
  void reset_to_cart() {}

  uint32_t tick_out(const GateBoyState& reg_old, GateBoyState& reg_new);

  /*#p18.COKA*/ wire COKA_CH3_ACTIVEp() const { return not1(DAVO_CH3_ACTIVEp.qn_new()); }


  /*_p16.KOGA*/ DFF9 KOGA_NR33_FREQ00;
  /*_p16.JOVY*/ DFF9 JOVY_NR33_FREQ01;
  /*_p16.JAXA*/ DFF9 JAXA_NR33_FREQ02;
  /*_p16.JEFE*/ DFF9 JEFE_NR33_FREQ03;
  /*_p16.JYPO*/ DFF9 JYPO_NR33_FREQ04;
  /*_p16.JOVE*/ DFF9 JOVE_NR33_FREQ05;
  /*_p16.KANA*/ DFF9 KANA_NR33_FREQ06;
  /*_p16.KOGU*/ DFF9 KOGU_NR33_FREQ07;

  //----------

  /*_p16.JEMO*/ DFF9 JEMO_NR34_FREQ08;
  /*_p16.JETY*/ DFF9 JETY_NR34_FREQ09;
  /*_p16.JACY*/ DFF9 JACY_NR34_FREQ10;
  /*_p16.HOTO*/ DFF9 HOTO_NR34_LENEN;
  /*_p16.GAVU*/ DFF9 GAVU_NR34_TRIG;

  /*_p18.DAVO*/ DFF17    DAVO_CH3_ACTIVEp;

  /*_p16.GOFY*/ NorLatch GOFY_CH3_TRIGn;
  NorLatch FOZU_CH3_ACTIVEp;


  /*_p16.GUXE*/ DFF9 GUXE_CH3_AMP_ENn;

  /*_p18.GEVO*/ DFF20 GEVO_CH3_LEN0p;
  /*_p18.FORY*/ DFF20 FORY_CH3_LEN1p;
  /*_p18.GATU*/ DFF20 GATU_CH3_LEN2p;
  /*_p18.GAPO*/ DFF20 GAPO_CH3_LEN3p;
  /*_p18.GEMO*/ DFF20 GEMO_CH3_LEN4p;
  /*_p18.FORO*/ DFF20 FORO_CH3_LEN5p;
  /*_p18.FAVE*/ DFF20 FAVE_CH3_LEN6p;
  /*_p18.FYRU*/ DFF20 FYRU_CH3_LEN7p;  

  /*_p18.FEXU*/ DFF17 FEXU_LEN_DONEp;

  /*_p16.HUKY*/ DFF9 HUKY_NR32_VOL0;
  /*_p16.HODY*/ DFF9 HODY_NR32_VOL1;

  /*_p16.GARA*/ DFF17 GARA_TRIG_D1;
  /*_p16.GYTA*/ DFF17 GYTA_TRIG_D2;
  /*_p16.GYRA*/ DFF17 GYRA_TRIG_D3;

  /*_p18.KUTU*/ DFF20 KUTU_FREQ_00;
  /*_p18.KUPE*/ DFF20 KUPE_FREQ_01;
  /*_p18.KUNU*/ DFF20 KUNU_FREQ_02;
  /*_p18.KEMU*/ DFF20 KEMU_FREQ_03;
  /*_p18.KYGU*/ DFF20 KYGU_FREQ_04;
  /*_p18.KEPA*/ DFF20 KEPA_FREQ_05;
  /*_p18.KAFO*/ DFF20 KAFO_FREQ_06;
  /*_p18.KENO*/ DFF20 KENO_FREQ_07;
  /*_p18.KEJU*/ DFF20 KEJU_FREQ_08;
  /*_p18.KEZA*/ DFF20 KEZA_FREQ_09;
  /*_p18.JAPU*/ DFF20 JAPU_FREQ_10;

  /*_p18.HUNO*/ DFF17 HUNO_SAMPLE_CLKp;
  /*_p18.EFAR*/ DFF17 EFAR_WAVE_IDX0;
  /*_p18.ERUS*/ DFF17 ERUS_WAVE_IDX1;
  /*_p18.EFUZ*/ DFF17 EFUZ_WAVE_IDX2;
  /*_p18.EXEL*/ DFF17 EXEL_WAVE_IDX3;
  /*_p18.EFAL*/ DFF17 EFAL_WAVE_IDX4;
  /*_p18.FETY*/ DFF17 FETY_WAVE_LOOP;

  /*_p17.CYFO*/ DFF9 CYFO_SAMPLE_0;
  /*_p17.CESY*/ DFF9 CESY_SAMPLE_1;
  /*_p17.BUDY*/ DFF9 BUDY_SAMPLE_2;
  /*_p17.BEGU*/ DFF9 BEGU_SAMPLE_3;
  /*_p17.CUVO*/ DFF9 CUVO_SAMPLE_4;
  /*_p17.CEVO*/ DFF9 CEVO_SAMPLE_5;
  /*_p17.BORA*/ DFF9 BORA_SAMPLE_6;
  /*_p17.BEPA*/ DFF9 BEPA_SAMPLE_7;

  /*_p16.FOBA*/ DFF17 FOBA_CH3_TRIGp;
  NandLatch GUGU_FREQ_GATE;

  // This is a pulse generator for wave ram control line 2/3
  /*_p17.BUSA*/ DFF17 BUSA_WAVE_CLK_D1;
  /*_p17.BANO*/ DFF17 BANO_WAVE_CLK_D2;
  /*_p17.AZUS*/ DFF17 AZUS_WAVE_CLK_D3;
  /*_p17.AZET*/ DFF17 AZET_WAVE_CLK_D4;
};















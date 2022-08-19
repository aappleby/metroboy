
struct GateBoyState;

//==============================================================================
//             CHANNEL 3
//==============================================================================

struct GateBoyState;

struct SpuChannel3 {
  void reset() {
    KOGA_NR33_FREQ00p.state = 0x1a;
    JOVY_NR33_FREQ01p.state = 0x1a;
    JAXA_NR33_FREQ02p.state = 0x1a;
    JEFE_NR33_FREQ03p.state = 0x1a;
    JYPO_NR33_FREQ04p.state = 0x1a;
    JOVE_NR33_FREQ05p.state = 0x1a;
    KANA_NR33_FREQ06p.state = 0x1a;
    KOGU_NR33_FREQ07p.state = 0x1a;
    JEMO_NR34_FREQ08p.state = 0x1a;
    JETY_NR34_FREQ09p.state = 0x1a;
    JACY_NR34_FREQ10p.state = 0x1a;
    HOTO_NR34_LENENp.state = 0x1a;
    GAVU_NR34_TRIGp.state = 0x1a;
    DAVO_CH3_ACTIVEp.state = 0x1a;
    GOFY_CH3_TRIGn.state = 0x19;
    FOZU_CH3_ACTIVEp.state = 0x18;
    GUXE_NR30_AMP_ENp.state = 0x1a;
    GEVO_NR31_LEN0p.state = 0x1a;
    FORY_NR31_LEN1p.state = 0x18;
    GATU_NR31_LEN2p.state = 0x18;
    GAPO_NR31_LEN3p.state = 0x18;
    GEMO_NR31_LEN4p.state = 0x18;
    FORO_NR31_LEN5p.state = 0x18;
    FAVE_NR31_LEN6p.state = 0x18;
    FYRU_NR31_LEN7p.state = 0x18;
    FEXU_LEN_DONEp.state = 0x1a;
    HUKY_NR32_VOL0p.state = 0x1a;
    HODY_NR32_VOL1p.state = 0x1a;
    GARA_TRIG_D1.state = 0x18;
    GYTA_TRIG_D2.state = 0x18;
    GYRA_TRIG_D3.state = 0x1a;
    KUTU_COUNT00.state = 0x1b;
    KUPE_COUNT01.state = 0x1a;
    KUNU_COUNT02.state = 0x18;
    KEMU_COUNT03.state = 0x18;
    KYGU_COUNT04.state = 0x18;
    KEPA_COUNT05.state = 0x18;
    KAFO_COUNT06.state = 0x18;
    KENO_COUNT07.state = 0x18;
    KEJU_COUNT08.state = 0x18;
    KEZA_COUNT09.state = 0x18;
    JAPU_COUNT10.state = 0x18;
    HUNO_SAMPLE_CLKp.state = 0x1a;
    EFAR_WAVE_IDX0.state = 0x1a;
    ERUS_WAVE_IDX1.state = 0x1a;
    EFUZ_WAVE_IDX2.state = 0x1a;
    EXEL_WAVE_IDX3.state = 0x1a;
    EFAL_WAVE_IDX4.state = 0x1a;
    FETY_WAVE_LOOP.state = 0x1a;
    CYFO_SAMPLE0p.state = 0x1a;
    CESY_SAMPLE1p.state = 0x1a;
    BUDY_SAMPLE2p.state = 0x1a;
    BEGU_SAMPLE3p.state = 0x1a;
    CUVO_SAMPLE4p.state = 0x1a;
    CEVO_SAMPLE5p.state = 0x1a;
    BORA_SAMPLE6p.state = 0x1a;
    BEPA_SAMPLE7p.state = 0x1a;
    FOBA_CH3_TRIGp.state = 0x18;
    GUGU_FREQ_GATE.state = 0x19;
    BUSA_WAVE_CLK_D1.state = 0x18;
    BANO_WAVE_CLK_D2.state = 0x1a;
    AZUS_WAVE_CLK_D3.state = 0x18;
    AZET_WAVE_CLK_D4.state = 0x1a;
  }


  //----------

  /*_p16.GUXE*/ DFF9 GUXE_NR30_AMP_ENp;

  /*_p18.GEVO*/ DFF20 GEVO_NR31_LEN0p;
  /*_p18.FORY*/ DFF20 FORY_NR31_LEN1p;
  /*_p18.GATU*/ DFF20 GATU_NR31_LEN2p;
  /*_p18.GAPO*/ DFF20 GAPO_NR31_LEN3p;
  /*_p18.GEMO*/ DFF20 GEMO_NR31_LEN4p;
  /*_p18.FORO*/ DFF20 FORO_NR31_LEN5p;
  /*_p18.FAVE*/ DFF20 FAVE_NR31_LEN6p;
  /*_p18.FYRU*/ DFF20 FYRU_NR31_LEN7p;

  /*_p16.HUKY*/ DFF9 HUKY_NR32_VOL0p;
  /*_p16.HODY*/ DFF9 HODY_NR32_VOL1p; // 10

  /*_p16.KOGA*/ DFF9 KOGA_NR33_FREQ00p;
  /*_p16.JOVY*/ DFF9 JOVY_NR33_FREQ01p;
  /*_p16.JAXA*/ DFF9 JAXA_NR33_FREQ02p;
  /*_p16.JEFE*/ DFF9 JEFE_NR33_FREQ03p;
  /*_p16.JYPO*/ DFF9 JYPO_NR33_FREQ04p;
  /*_p16.JOVE*/ DFF9 JOVE_NR33_FREQ05p;
  /*_p16.KANA*/ DFF9 KANA_NR33_FREQ06p;
  /*_p16.KOGU*/ DFF9 KOGU_NR33_FREQ07p;

  /*_p16.JEMO*/ DFF9 JEMO_NR34_FREQ08p;
  /*_p16.JETY*/ DFF9 JETY_NR34_FREQ09p; // 20
  /*_p16.JACY*/ DFF9 JACY_NR34_FREQ10p;
  /*_p16.HOTO*/ DFF9 HOTO_NR34_LENENp;
  /*_p16.GAVU*/ DFF9 GAVU_NR34_TRIGp;

  //----------

  /*_p18.FEXU*/ DFF17 FEXU_LEN_DONEp;
  /*#p16.GUGU*/ NandLatch GUGU_FREQ_GATE;
  /*_p18.HUNO*/ DFF17 HUNO_SAMPLE_CLKp;
  /*_p18.FETY*/ DFF17 FETY_WAVE_LOOP; // 27

  /*_p16.GARA*/ DFF17    GARA_TRIG_D1;
  /*_p16.GYTA*/ DFF17    GYTA_TRIG_D2;
  /*_p16.GYRA*/ DFF17    GYRA_TRIG_D3;
  /*_p16.GOFY*/ NorLatch GOFY_CH3_TRIGn;
  /*_p16.FOBA*/ DFF17    FOBA_CH3_TRIGp;
  /*_p18.DAVO*/ DFF17    DAVO_CH3_ACTIVEp;
  /*#p18.FOZU*/ NorLatch FOZU_CH3_ACTIVEp;

  /*_p18.KUTU*/ DFF20 KUTU_COUNT00;
  /*_p18.KUPE*/ DFF20 KUPE_COUNT01;
  /*_p18.KUNU*/ DFF20 KUNU_COUNT02;
  /*_p18.KEMU*/ DFF20 KEMU_COUNT03;
  /*_p18.KYGU*/ DFF20 KYGU_COUNT04;
  /*_p18.KEPA*/ DFF20 KEPA_COUNT05;
  /*_p18.KAFO*/ DFF20 KAFO_COUNT06;
  /*_p18.KENO*/ DFF20 KENO_COUNT07;
  /*_p18.KEJU*/ DFF20 KEJU_COUNT08;
  /*_p18.KEZA*/ DFF20 KEZA_COUNT09;
  /*_p18.JAPU*/ DFF20 JAPU_COUNT10;

  /*_p18.EFAR*/ DFF17 EFAR_WAVE_IDX0;
  /*_p18.ERUS*/ DFF17 ERUS_WAVE_IDX1;
  /*_p18.EFUZ*/ DFF17 EFUZ_WAVE_IDX2;
  /*_p18.EXEL*/ DFF17 EXEL_WAVE_IDX3;
  /*_p18.EFAL*/ DFF17 EFAL_WAVE_IDX4; // 50

  // This is a pulse generator for wave ram control line 2/3
  /*_p17.BUSA*/ DFF17 BUSA_WAVE_CLK_D1;
  /*_p17.BANO*/ DFF17 BANO_WAVE_CLK_D2;
  /*_p17.AZUS*/ DFF17 AZUS_WAVE_CLK_D3;
  /*_p17.AZET*/ DFF17 AZET_WAVE_CLK_D4;

  /*_p17.CYFO*/ DFF9 CYFO_SAMPLE0p;
  /*_p17.CESY*/ DFF9 CESY_SAMPLE1p;
  /*_p17.BUDY*/ DFF9 BUDY_SAMPLE2p;
  /*_p17.BEGU*/ DFF9 BEGU_SAMPLE3p;
  /*_p17.CUVO*/ DFF9 CUVO_SAMPLE4p;
  /*_p17.CEVO*/ DFF9 CEVO_SAMPLE5p;
  /*_p17.BORA*/ DFF9 BORA_SAMPLE6p;
  /*_p17.BEPA*/ DFF9 BEPA_SAMPLE7p;
};

sample_t ch3_audio_out(const SpuChannel3& ch3);

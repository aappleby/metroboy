#pragma once

//==============================================================================
//             CHANNEL 1
//==============================================================================

struct GateBoyState;

struct SpuChannel1 {
  void reset() {
    BANY_NR10_SWEEP_SHIFT0p.state = 0x1a;
    ARAX_NR10_SWEEP_SHIFT1p.state = 0x1a;
    ANAZ_NR10_SWEEP_SHIFT2p.state = 0x1a;
    AVAF_NR10_SWEEP_DIRp.state = 0x1a;
    ADEK_NR10_SWEEP_PERIOD0p.state = 0x1a;
    BANA_NR10_SWEEP_PERIOD1p.state = 0x1a;
    BOTU_NR10_SWEEP_PERIOD2p.state = 0x1a;

    BACY_NR11_LEN0.state = 0x1a;
    CAVY_NR11_LEN1.state = 0x18;
    BOVY_NR11_LEN2.state = 0x18;
    CUNO_NR11_LEN3.state = 0x18;
    CURA_NR11_LEN4.state = 0x18;
    ERAM_NR11_LEN5.state = 0x18;
    CENA_NR11_DUTY0p.state = 0x1a;
    DYCA_NR11_DUTY1p.state = 0x1b;

    JUSA_NR12_PERIOD0p.state = 0x1b;
    JUZY_NR12_PERIOD1p.state = 0x1b;
    JOMA_NR12_PERIOD2p.state = 0x1a;
    JAFY_NR12_ENV_DIRp.state = 0x1a;
    JATY_NR12_VOL0p.state = 0x1b;
    JAXO_NR12_VOL1p.state = 0x1b;
    JENA_NR12_VOL2p.state = 0x1b;
    JOPU_NR12_VOL3p.state = 0x1b;

    HYKA_NR13_FREQ00.state = 0x19;
    JYKA_NR13_FREQ01.state = 0x18;
    HAVO_NR13_FREQ02.state = 0x18;
    EDUL_NR13_FREQ03.state = 0x18;
    FELY_NR13_FREQ04.state = 0x18;
    HOLU_NR13_FREQ05.state = 0x18;
    HYXU_NR13_FREQ06.state = 0x19;
    HOPO_NR13_FREQ07.state = 0x19;

    DYGY_NR14_FREQ08.state = 0x19;
    EVAB_NR14_FREQ09.state = 0x19;
    AXAN_NR14_FREQ10.state = 0x19;
    BOKO_NR14_LENENp.state = 0x1a;
    DUPE_NR14_TRIGp.state = 0x1a;

    JOVA_ENV_DELAY0p.state = 0x18;
    KENU_ENV_DELAY1p.state = 0x19;
    KERA_ENV_DELAY2p.state = 0x1b;

    HAFO_CH1_ENV0p.state = 0x1a;
    HEMY_CH1_ENV1p.state = 0x1a;
    HOKO_CH1_ENV2p.state = 0x1a;
    HEVO_CH1_ENV3p.state = 0x1a;

    KOZY_ENV_TICKp.state = 0x1a;
    KYNO_ENV_MAXp.state = 0x19;

    CERO_CH1_LEN_DONE.state = 0x1a;
    KALY_ENV_DELAY_CLK_64n.state = 0x18;
    CYTO_CH1_ACTIVEp.state = 0x19;
    EZEC_CH1_TRIGp.state = 0x18;

    CALO_xBCDExxx.state = 0x18;

    FEKU_CH1_TRIGp.state = 0x18;
    FARE_CH1_TRIGp.state = 0x18;
    FYTE_CH1_TRIGp.state = 0x18;

    BYTE_SHIFT_DONEp.state = 0x1b;

    GAXE_CH1_FREQ_CNT_00.state = 0x19;
    HYFE_CH1_FREQ_CNT_01.state = 0x1a;
    JYTY_CH1_FREQ_CNT_02.state = 0x18;
    KYNA_CH1_FREQ_CNT_03.state = 0x19;
    JEMA_CH1_FREQ_CNT_04.state = 0x1b;
    HYKE_CH1_FREQ_CNT_05.state = 0x1b;
    FEVA_CH1_FREQ_CNT_06.state = 0x1b;
    EKOV_CH1_FREQ_CNT_07.state = 0x1b;
    EMUS_CH1_FREQ_CNT_08.state = 0x1b;
    EVAK_CH1_FREQ_CNT_09.state = 0x1b;
    COPU_CH1_FREQ_CNT_10.state = 0x1b;

    BAZA_DBG_SWEEP_CLK.state = 0x1A;
    BEXA_SWEEP_TRIGp.state = 0x1a;
    CUPO_SWEEP_DELAY0p.state = 0x1a;
    CYPU_SWEEP_DELAY1p.state = 0x19;
    CAXY_SWEEP_DELAY2p.state = 0x1a;

    FABU_CH1_SHIFT00.state = 0x1b;
    JEFA_CH1_SHIFT01.state = 0x1a;
    GOGA_CH1_SHIFT02.state = 0x1a;
    JOLU_CH1_SHIFT03.state = 0x1a;
    JOTA_CH1_SHIFT04.state = 0x1a;
    FUDE_CH1_SHIFT05.state = 0x1a;
    FEDO_CH1_SHIFT06.state = 0x1b;
    EXAC_CH1_SHIFT07.state = 0x1b;
    ELUX_CH1_SHIFT08.state = 0x1b;
    AGEZ_CH1_SHIFT09.state = 0x1b;
    BEKU_CH1_SHIFT10.state = 0x1b;

    GALO_SUM_A00.state = 0x1a;
    JODE_SUM_A01.state = 0x1a;
    KARE_SUM_A02.state = 0x1a;
    JYME_SUM_A03.state = 0x1a;
    GYME_SUM_A04.state = 0x1a;
    FAXO_SUM_A05.state = 0x1a;
    EXAP_SUM_A06.state = 0x1a;
    DELE_SUM_A07.state = 0x1a;
    DEXE_SUM_A08.state = 0x1a;
    DOFY_SUM_A09.state = 0x1a;
    DOLY_SUM_A10.state = 0x1a;

    HORA_SUM_B00.state = 0x1a;
    HOPA_SUM_B01.state = 0x1a;
    HELE_SUM_B02.state = 0x1a;
    JAPE_SUM_B03.state = 0x1a;
    JETE_SUM_B04.state = 0x1a;
    GELE_SUM_B05.state = 0x1a;
    EPYR_SUM_B06.state = 0x1a;
    EDOK_SUM_B07.state = 0x1a;
    DEFA_SUM_B08.state = 0x1a;
    ETER_SUM_B09.state = 0x1a;
    DEVA_SUM_B10.state = 0x1a;

    FEMU_SHIFTINGn.state = 0x19;

    COPA_SHIFTCNT0.state = 0x19;
    CAJA_SHIFTCNT1.state = 0x1b;
    BYRA_SHIFTCNT2.state = 0x1b;

    FYFO_CH1_TRIGn.state = 0x19;
    COMY_SAMPLE_CLKp.state = 0x18;
    GEXU_FREQ_GATEn.state = 0x18;

    ESUT_PHASE_xBxDxFxH.state = 0x1a;
    EROS_PHASE_xxCDxxGH.state = 0x1b;
    DAPE_PHASE_xxxxEFGH.state = 0x18;

    DUWO_RAW_BIT_SYNCp.state = 0x18;
    KEZU_ENV_ACTIVEn.state = 0x19;
  }

  /*#p11.BANY*/ DFF9 BANY_NR10_SWEEP_SHIFT0p;
  /*#p11.ARAX*/ DFF9 ARAX_NR10_SWEEP_SHIFT1p;
  /*#p11.ANAZ*/ DFF9 ANAZ_NR10_SWEEP_SHIFT2p;
  /*#p11.AVAF*/ DFF9 AVAF_NR10_SWEEP_DIRp;
  /*#p11.ADEK*/ DFF9 ADEK_NR10_SWEEP_PERIOD0p;
  /*#p11.BANA*/ DFF9 BANA_NR10_SWEEP_PERIOD1p;
  /*#p11.BOTU*/ DFF9 BOTU_NR10_SWEEP_PERIOD2p;

  //----------

  /*_p13.BACY*/ DFF20 BACY_NR11_LEN0;
  /*_p13.CAVY*/ DFF20 CAVY_NR11_LEN1;
  /*_p13.BOVY*/ DFF20 BOVY_NR11_LEN2;
  /*_p13.CUNO*/ DFF20 CUNO_NR11_LEN3; // 10
  /*_p13.CURA*/ DFF20 CURA_NR11_LEN4;
  /*_p13.ERAM*/ DFF20 ERAM_NR11_LEN5;
  /*_p11.CENA*/ DFF9 CENA_NR11_DUTY0p;
  /*_p11.DYCA*/ DFF9 DYCA_NR11_DUTY1p;

  //----------

  /*_p11.JUSA*/ DFF9 JUSA_NR12_PERIOD0p;
  /*_p11.JUZY*/ DFF9 JUZY_NR12_PERIOD1p;
  /*_p11.JOMA*/ DFF9 JOMA_NR12_PERIOD2p;
  /*_p11.JAFY*/ DFF9 JAFY_NR12_ENV_DIRp;
  /*_p11.JATY*/ DFF9 JATY_NR12_VOL0p;
  /*_p11.JAXO*/ DFF9 JAXO_NR12_VOL1p; // 20
  /*_p11.JENA*/ DFF9 JENA_NR12_VOL2p;
  /*_p11.JOPU*/ DFF9 JOPU_NR12_VOL3p;

  //----------

  /*_p12.HYKA*/ DFF22 HYKA_NR13_FREQ00;
  /*_p12.JYKA*/ DFF22 JYKA_NR13_FREQ01;
  /*_p12.HAVO*/ DFF22 HAVO_NR13_FREQ02;
  /*_p12.EDUL*/ DFF22 EDUL_NR13_FREQ03;
  /*_p12.FELY*/ DFF22 FELY_NR13_FREQ04;
  /*_p12.HOLU*/ DFF22 HOLU_NR13_FREQ05;
  /*_p12.HYXU*/ DFF22 HYXU_NR13_FREQ06;
  /*_p12.HOPO*/ DFF22 HOPO_NR13_FREQ07; // 30

  //----------

  /*_p12.DYGY*/ DFF22 DYGY_NR14_FREQ08;
  /*_p12.EVAB*/ DFF22 EVAB_NR14_FREQ09;
  /*_p12.AXAN*/ DFF22 AXAN_NR14_FREQ10;
  /*_p11.BOKO*/ DFF9 BOKO_NR14_LENENp;
  /*#p13.DUPE*/ DFF9 DUPE_NR14_TRIGp;

  //----------

  /*_p13.JOVA*/ DFF20 JOVA_ENV_DELAY0p; 
  /*_p13.KENU*/ DFF20 KENU_ENV_DELAY1p;
  /*_p13.KERA*/ DFF20 KERA_ENV_DELAY2p;  

  /*_p13.HAFO*/ DFF20 HAFO_CH1_ENV0p;
  /*_p13.HEMY*/ DFF20 HEMY_CH1_ENV1p; // 40
  /*_p13.HOKO*/ DFF20 HOKO_CH1_ENV2p;
  /*_p13.HEVO*/ DFF20 HEVO_CH1_ENV3p;

  //----------

  /*#p13.KOZY*/ DFF17 KOZY_ENV_TICKp;
  /*#p13.KYNO*/ DFF17 KYNO_ENV_MAXp;

  /*_p13.CERO*/ DFF17 CERO_CH1_LEN_DONE;

  /*_p13.KALY*/ DFF17 KALY_ENV_DELAY_CLK_64n;
  /*#p13.CYTO*/ NorLatch CYTO_CH1_ACTIVEp;

  /*#p13.EZEC*/ DFF17 EZEC_CH1_TRIGp;


  /*_p09.CALO*/ DFF17 CALO_xBCDExxx;

  /*_p13.FEKU*/ DFF17 FEKU_CH1_TRIGp; // 50
  /*_p13.FARE*/ DFF17 FARE_CH1_TRIGp;
  /*_p13.FYTE*/ DFF17 FYTE_CH1_TRIGp;

  /*_p13.BYTE*/ DFF17 BYTE_SHIFT_DONEp;

  /*_p11.GAXE*/ DFF20 GAXE_CH1_FREQ_CNT_00;
  /*_p11.HYFE*/ DFF20 HYFE_CH1_FREQ_CNT_01;
  /*_p11.JYTY*/ DFF20 JYTY_CH1_FREQ_CNT_02;
  /*_p11.KYNA*/ DFF20 KYNA_CH1_FREQ_CNT_03;
  /*_p11.JEMA*/ DFF20 JEMA_CH1_FREQ_CNT_04;
  /*_p11.HYKE*/ DFF20 HYKE_CH1_FREQ_CNT_05;
  /*_p11.FEVA*/ DFF20 FEVA_CH1_FREQ_CNT_06; // 60
  /*_p11.EKOV*/ DFF20 EKOV_CH1_FREQ_CNT_07;
  /*_p11.EMUS*/ DFF20 EMUS_CH1_FREQ_CNT_08;
  /*_p11.EVAK*/ DFF20 EVAK_CH1_FREQ_CNT_09;
  /*_p11.COPU*/ DFF20 COPU_CH1_FREQ_CNT_10;  

  /*#p09.BAZA*/ DFF17 BAZA_DBG_SWEEP_CLK;
  /*_p13.BEXA*/ DFF17 BEXA_SWEEP_TRIGp;
  /*_p13.CUPO*/ DFF20 CUPO_SWEEP_DELAY0p;
  /*_p13.CYPU*/ DFF20 CYPU_SWEEP_DELAY1p;
  /*_p13.CAXY*/ DFF20 CAXY_SWEEP_DELAY2p;

  /*_p12.FABU*/ DFF22 FABU_CH1_SHIFT00;
  /*_p12.JEFA*/ DFF22 JEFA_CH1_SHIFT01;
  /*_p12.GOGA*/ DFF22 GOGA_CH1_SHIFT02;
  /*_p12.JOLU*/ DFF22 JOLU_CH1_SHIFT03;
  /*_p12.JOTA*/ DFF22 JOTA_CH1_SHIFT04;
  /*_p12.FUDE*/ DFF22 FUDE_CH1_SHIFT05;
  /*_p12.FEDO*/ DFF22 FEDO_CH1_SHIFT06;
  /*_p12.EXAC*/ DFF22 EXAC_CH1_SHIFT07;
  /*_p12.ELUX*/ DFF22 ELUX_CH1_SHIFT08;
  /*_p12.AGEZ*/ DFF22 AGEZ_CH1_SHIFT09;
  /*_p12.BEKU*/ DFF22 BEKU_CH1_SHIFT10;

  /*_p12.GALO*/ DFF11 GALO_SUM_A00;
  /*_p12.JODE*/ DFF11 JODE_SUM_A01;
  /*_p12.KARE*/ DFF11 KARE_SUM_A02;
  /*_p12.JYME*/ DFF11 JYME_SUM_A03;
  /*_p12.GYME*/ DFF11 GYME_SUM_A04;
  /*_p12.FAXO*/ DFF11 FAXO_SUM_A05;
  /*_p12.EXAP*/ DFF11 EXAP_SUM_A06;
  /*_p12.DELE*/ DFF11 DELE_SUM_A07;
  /*_p12.DEXE*/ DFF11 DEXE_SUM_A08;
  /*_p12.DOFY*/ DFF11 DOFY_SUM_A09;
  /*_p12.DOLY*/ DFF11 DOLY_SUM_A10;

  /*_p12.HORA*/ DFF11 HORA_SUM_B00;
  /*_p12.HOPA*/ DFF11 HOPA_SUM_B01;
  /*_p12.HELE*/ DFF11 HELE_SUM_B02;
  /*_p12.JAPE*/ DFF11 JAPE_SUM_B03;
  /*_p12.JETE*/ DFF11 JETE_SUM_B04;
  /*_p12.GELE*/ DFF11 GELE_SUM_B05;
  /*_p12.EPYR*/ DFF11 EPYR_SUM_B06;
  /*_p12.EDOK*/ DFF11 EDOK_SUM_B07;
  /*_p12.DEFA*/ DFF11 DEFA_SUM_B08;
  /*_p12.ETER*/ DFF11 ETER_SUM_B09;
  /*_p12.DEVA*/ DFF11 DEVA_SUM_B10;

  /*_p13.FEMU*/ NandLatch FEMU_SHIFTINGn;

  // shift is done when this saturates
  /*#p13.COPA*/ DFF20 COPA_SHIFTCNT0;
  /*#p13.CAJA*/ DFF20 CAJA_SHIFTCNT1;
  /*#p13.BYRA*/ DFF20 BYRA_SHIFTCNT2;

  /*_p13.FYFO*/ NorLatch FYFO_CH1_TRIGn;
  /*_p13.COMY*/ DFF17 COMY_SAMPLE_CLKp;
  /*_p13.GEXU*/ NandLatch GEXU_FREQ_GATEn;

  /*_p13.ESUT*/ DFF17 ESUT_PHASE_xBxDxFxH;
  /*_p13.EROS*/ DFF13 EROS_PHASE_xxCDxxGH;
  /*_p13.DAPE*/ DFF13 DAPE_PHASE_xxxxEFGH;

  /*_p13.DUWO*/ DFF17 DUWO_RAW_BIT_SYNCp;
  /*#p13.KEZU*/ NorLatch KEZU_ENV_ACTIVEn;
};

sample_t ch1_audio_out(const SpuChannel1& reg_new);

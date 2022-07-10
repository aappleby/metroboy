#pragma once

//==============================================================================
//             CHANNEL 1
//==============================================================================

struct GateBoyState;
int ch1_audio_out(const GateBoyState& reg_new);

struct SpuChannel1 {
  void reset_to_cart();

  /*#p13.HOCA*/ wire HOCA_CH1_AMP_ENn_new() const {
    return nor5(JAFY_NR12_ENV_DIRp.qp_newB(),
                JATY_NR12_VOL0p   .qp_newB(),
                JAXO_NR12_VOL1p   .qp_newB(),
                JENA_NR12_VOL2p   .qp_newB(),
                JOPU_NR12_VOL3p   .qp_newB());
  }


  /*#p13.BUGE*/ wire BUGE_SWEEP_DONEn_new() const {
    return nand3(ANAZ_NR10_SWEEP_SHIFT2p.qn_newB(),
                 ARAX_NR10_SWEEP_SHIFT1p.qn_newB(),
                 BANY_NR10_SWEEP_SHIFT0p.qn_newB());
  }

  wire EGOR_SHIFT_CLK_new() const {
    /*#p13.EGYP*/ wire EGYP_SHIFT_CLK_new = nor2(FEMU_SHIFTINGn.qn_new(), DYFA_xBCDExxx());
    /*#p13.CELE*/ wire CELE_SWEEP_DONEp_new = not1(BUGE_SWEEP_DONEn_new());
    /*#p13.DODY*/ wire DODY_SHIFT_CLK_new = nor2(EGYP_SHIFT_CLK_new, CELE_SWEEP_DONEp_new); // border color wrong on die
    /*?p13.EGOR*/ wire EGOR_SHIFT_CLK_new = not1(DODY_SHIFT_CLK_new); // This looks like a nor3, but it almost definiteily is a not1.
    return EGOR_SHIFT_CLK_new;
  }

  /*#p12.FAJA*/ wire FAJA_SHIFT_CLK_new() const { return not1(EGOR_SHIFT_CLK_new()); }
  /*#p12.EJYB*/ wire EJYB_SHIFT_CLK_new() const { return not1(FAJA_SHIFT_CLK_new()); }
  /*#p12.CYBE*/ wire CYBE_SHIFT_CLK_new() const { return not1(EJYB_SHIFT_CLK_new()); }
  /*#p12.BECY*/ wire BECY_SHIFT_CLK_new() const { return not1(CYBE_SHIFT_CLK_new()); }

  /*#p09.DYFA*/ wire DYFA_CLK_1M_old() const { return not1(CALO_xBCDExxx.qn_old()); }
  /*#p09.DYFA*/ wire DYFA_xBCDExxx() const { return not1(CALO_xBCDExxx.qn_new()); }

  /*#p12.ARYL*/ wire ARYL_NR10_SWEEP_DIRn_new() const { return not1(AVAF_NR10_SWEEP_DIRp.qn_newB()); }
  /*#p12.ARYL*/ wire ARYL_NR10_SWEEP_DIRn_old() const { return not1(AVAF_NR10_SWEEP_DIRp.qn_oldB()); }

  // so this implies that NR12_DELAY* is DFFn?
  /*#p13.KOMA*/ wire KOMA_ENV_OFFp_old() const { return nor3(JUSA_NR12_PERIOD0p.qp_oldB(), JUZY_NR12_PERIOD1p.qp_oldB(), JOMA_NR12_PERIOD2p.qp_oldB()); }
  /*#p13.KOMA*/ wire KOMA_ENV_OFFp_new() const { return nor3(JUSA_NR12_PERIOD0p.qp_newB(), JUZY_NR12_PERIOD1p.qp_newB(), JOMA_NR12_PERIOD2p.qp_newB()); }

  /*#p11.BANY*/ DFF9B BANY_NR10_SWEEP_SHIFT0p;
  /*#p11.ARAX*/ DFF9B ARAX_NR10_SWEEP_SHIFT1p;
  /*#p11.ANAZ*/ DFF9B ANAZ_NR10_SWEEP_SHIFT2p;
  /*#p11.AVAF*/ DFF9B AVAF_NR10_SWEEP_DIRp;
  /*#p11.ADEK*/ DFF9B ADEK_NR10_SWEEP_PERIOD0p;
  /*#p11.BANA*/ DFF9B BANA_NR10_SWEEP_PERIOD1p;
  /*#p11.BOTU*/ DFF9B BOTU_NR10_SWEEP_PERIOD2p;

  //----------

  /*_p11.CENA*/ DFF9B CENA_NR11_DUTY0p;
  /*_p11.DYCA*/ DFF9B DYCA_NR11_DUTY1p;
  /*_p13.BACY*/ DFF20 BACY_NR11_LEN0;
  /*_p13.CAVY*/ DFF20 CAVY_NR11_LEN1;
  /*_p13.BOVY*/ DFF20 BOVY_NR11_LEN2;
  /*_p13.CUNO*/ DFF20 CUNO_NR11_LEN3;
  /*_p13.CURA*/ DFF20 CURA_NR11_LEN4;
  /*_p13.ERAM*/ DFF20 ERAM_NR11_LEN5;

  //----------

  /*_p11.JUSA*/ DFF9B JUSA_NR12_PERIOD0p;
  /*_p11.JUZY*/ DFF9B JUZY_NR12_PERIOD1p;
  /*_p11.JOMA*/ DFF9B JOMA_NR12_PERIOD2p;
  /*_p11.JAFY*/ DFF9B JAFY_NR12_ENV_DIRp;
  /*_p11.JATY*/ DFF9B JATY_NR12_VOL0p;
  /*_p11.JAXO*/ DFF9B JAXO_NR12_VOL1p;
  /*_p11.JENA*/ DFF9B JENA_NR12_VOL2p;
  /*_p11.JOPU*/ DFF9B JOPU_NR12_VOL3p;

  //----------

  /*_p11.BOKO*/ DFF9B BOKO_NR14_LENENp;
  /*#p13.DUPE*/ DFF9B DUPE_NR14_TRIGp;

  /*#p13.CYTO*/ NorLatch CYTO_CH1_ACTIVEp;

  /*#p13.EZEC*/ DFF17 EZEC_CH1_TRIGp;

  /*_p12.HYKA*/ DFF22 HYKA_CH1_FREQ00;
  /*_p12.JYKA*/ DFF22 JYKA_CH1_FREQ01;
  /*_p12.HAVO*/ DFF22 HAVO_CH1_FREQ02;
  /*_p12.EDUL*/ DFF22 EDUL_CH1_FREQ03;
  /*_p12.FELY*/ DFF22 FELY_CH1_FREQ04;
  /*_p12.HOLU*/ DFF22 HOLU_CH1_FREQ05;
  /*_p12.HYXU*/ DFF22 HYXU_CH1_FREQ06;
  /*_p12.HOPO*/ DFF22 HOPO_CH1_FREQ07;
  /*_p12.DYGY*/ DFF22 DYGY_CH1_FREQ08;
  /*_p12.EVAB*/ DFF22 EVAB_CH1_FREQ09;
  /*_p12.AXAN*/ DFF22 AXAN_CH1_FREQ10;

  /*_p09.CALO*/ DFF17 CALO_xBCDExxx;

  /*_p13.FEKU*/ DFF17 FEKU_CH1_TRIGp;
  /*_p13.FARE*/ DFF17 FARE_CH1_TRIGp;
  /*_p13.FYTE*/ DFF17 FYTE_CH1_TRIGp;

  /*_p13.BYTE*/ DFF17 BYTE_SHIFT_DONEp;

  /*_p13.CERO*/ DFF17 CERO_CH1_LEN_DONE;

  /*_p11.GAXE*/ DFF20 GAXE_CH1_FREQ_CNT_00;
  /*_p11.HYFE*/ DFF20 HYFE_CH1_FREQ_CNT_01;
  /*_p11.JYTY*/ DFF20 JYTY_CH1_FREQ_CNT_02;
  /*_p11.KYNA*/ DFF20 KYNA_CH1_FREQ_CNT_03;
  /*_p11.JEMA*/ DFF20 JEMA_CH1_FREQ_CNT_04;
  /*_p11.HYKE*/ DFF20 HYKE_CH1_FREQ_CNT_05;
  /*_p11.FEVA*/ DFF20 FEVA_CH1_FREQ_CNT_06;
  /*_p11.EKOV*/ DFF20 EKOV_CH1_FREQ_CNT_07;
  /*_p11.EMUS*/ DFF20 EMUS_CH1_FREQ_CNT_08;
  /*_p11.EVAK*/ DFF20 EVAK_CH1_FREQ_CNT_09;
  /*_p11.COPU*/ DFF20 COPU_CH1_FREQ_CNT_10;  

  /*#p09.BAZA*/ DFF17 BAZA_DBG_SWEEP_CLK;
  /*_p13.BEXA*/ DFF17 BEXA_SWEEP_TRIGGERp;
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

  /*#p13.KOZY*/ DFF17 KOZY_ENV_TICKp;
  /*#p13.KYNO*/ DFF17 KYNO_ENV_MAXp;

  /*_p13.JOVA*/ DFF20 JOVA_ENV_DELAY0p;
  /*_p13.KENU*/ DFF20 KENU_ENV_DELAY1p;
  /*_p13.KERA*/ DFF20 KERA_ENV_DELAY2p;  

  /*_p13.HAFO*/ DFF20 HAFO_CH1_ENV0p;
  /*_p13.HEMY*/ DFF20 HEMY_CH1_ENV1p;
  /*_p13.HOKO*/ DFF20 HOKO_CH1_ENV2p;
  /*_p13.HEVO*/ DFF20 HEVO_CH1_ENV3p;

  /*_p13.KALY*/ DFF17 KALY_ENV_DELAY_CLK_64n;

  /*_p13.DUWO*/ DFF17 DUWO_RAW_BIT_SYNCp;
  /*#p13.KEZU*/ NorLatch KEZU_ENV_ACTIVEn;
};
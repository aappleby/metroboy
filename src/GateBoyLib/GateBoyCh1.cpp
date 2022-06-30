#include "GateBoySPU.h"

#include "GateBoyLib/GateBoy.h"

void GBSound::tick_ch1(const GateBoyState& reg_old, GateBoyState& reg_new) {
  tick_nr11(reg_old, reg_new);

  //----------
  // Trigger

  {
    /*#p13.EZEC*/ ch1.EZEC_CH1_TRIGn.dff17(DOVA_ABCDxxxx(), DUKA_APU_RSTn(), ch1.DUPE_NR14_TRIG.qn_new());
    /*#p13.EGET*/ wire EGET_TRIG_RSTn = nor2(KEBA_APU_RSTp(), ch1.FARE_CH1_TRIGp.qp_new());
    /*#p13.GEFE*/ wire GEFE_TRIG_RSTp = not1(EGET_TRIG_RSTn);
    /*#p13.FYFO*/ ch1.FYFO_CH1_TRIGn.nor_latch(GEFE_TRIG_RSTp, ch1.EZEC_CH1_TRIGn.qp_new());
    /*#p13.FEKU*/ ch1.FEKU_CH1_TRIGp.dff17(ch1.DYFA_CLK_1M(), EGET_TRIG_RSTn,  ch1.FYFO_CH1_TRIGn.qn_old()); // schematic wrong?
    /*#p13.FARE*/ ch1.FARE_CH1_TRIGp.dff17(ch1.DYFA_CLK_1M(), ERUM_APU_RSTn(), ch1.FEKU_CH1_TRIGp.qp_old());
    /*#p13.FYTE*/ ch1.FYTE_CH1_TRIGp.dff17(ch1.DYFA_CLK_1M(), ERUM_APU_RSTn(), ch1.FARE_CH1_TRIGp.qp_old());
  }

  //----------
  // Sweep timer

  {
    /*#p09.BAZA*/ ch1.BAZA_DBG_SWEEP_CLK.dff17(AJER_CLK_2M.qn_new(), ATYV_APU_RSTn(), BOWY_NR52_DBG_SWEEP.qp_old());
    /*#p09.CELY*/ wire CELY_SWEEP_DELAY_CLKn = mux2p(EDEK_NR52_DBG_APUp(), ch1.BAZA_DBG_SWEEP_CLK.qp_new(), BYFE_CLK_128n());
    /*#p09.CONE*/ wire CONE_SWEEP_DELAY_CLKp = not1(CELY_SWEEP_DELAY_CLKn);
    /*#p09.CATE*/ wire CATE_SWEEP_DELAY_CLKn = not1(CONE_SWEEP_DELAY_CLKp);

    /*#p13.DAFA*/ wire DAFA_SWEEP_DELAY_LOADn = nor2(ch1.BEXA_SWEEP_TRIGGERp.qp_new(), ch1.FEKU_CH1_TRIGp.qp_new());
    /*#p13.CYMU*/ wire CYMU_SWEEP_DELAY_LOADp = not1(DAFA_SWEEP_DELAY_LOADn);

    /*#p13.CUPO*/ ch1.CUPO_SWEEP_DELAY0p.dff20(CATE_SWEEP_DELAY_CLKn,           CYMU_SWEEP_DELAY_LOADp, ch1.ADEK_NR10_SWEEP_PERIOD0p.qp_old());
    /*#p13.CYPU*/ ch1.CYPU_SWEEP_DELAY1p.dff20(ch1.CUPO_SWEEP_DELAY0p.qp_new(), CYMU_SWEEP_DELAY_LOADp, ch1.BANA_NR10_SWEEP_PERIOD1p.qp_old());
    /*#p13.CAXY*/ ch1.CAXY_SWEEP_DELAY2p.dff20(ch1.CYPU_SWEEP_DELAY1p.qp_new(), CYMU_SWEEP_DELAY_LOADp, ch1.BOTU_NR10_SWEEP_PERIOD2p.qp_old());

    /*#p13.BAVE*/ wire BAVE_NR10_NO_SWEEPp = and3(ch1.BOTU_NR10_SWEEP_PERIOD2p.qp_new(), ch1.BANA_NR10_SWEEP_PERIOD1p.qp_new(), ch1.ADEK_NR10_SWEEP_PERIOD0p.qp_new());
    /*#p13.BURY*/ wire BURY_SWEEP_TRIGGER_RSTn = nor2(BAVE_NR10_NO_SWEEPp, KEBA_APU_RSTp());
    /*#p13.COZE*/ wire COZE_SWEEP_MAX = and3(ch1.CAXY_SWEEP_DELAY2p.qp_new(), ch1.CYPU_SWEEP_DELAY1p.qp_new(), ch1.CUPO_SWEEP_DELAY0p.qp_new());

    /*#p13.BEXA*/ ch1.BEXA_SWEEP_TRIGGERp.dff17(AJER_CLK_2M.qp_new(), BURY_SWEEP_TRIGGER_RSTn, COZE_SWEEP_MAX);
  }

  //----------
  // Frequency shifter

  {
    /*#p13.KALA*/ wire KALA_SHIFTER_LOADn = nor2(ch1.BEXA_SWEEP_TRIGGERp.qp_new(), ch1.FEKU_CH1_TRIGp.qp_new());
    /*#p12.KAPE*/ wire KAPE_SHIFTER_LOADp = not1(KALA_SHIFTER_LOADn);
    /*#p12.JUJU*/ wire JUJU_SHIFTER_LOADp = not1(KALA_SHIFTER_LOADn);
    /*#p12.KEDO*/ wire KEDO_SHIFTER_LOADp = not1(KALA_SHIFTER_LOADn);

    /*_p12.JUTA*/ wire JUTA_CH1_FREQ00n = not1(ch1.HYKA_CH1_FREQ00.qp_new());
    /*_p12.HOXE*/ wire HOXE_CH1_FREQ01n = not1(ch1.JYKA_CH1_FREQ01.qp_new());
    /*_p12.HUNY*/ wire HUNY_CH1_FREQ02n = not1(ch1.HAVO_CH1_FREQ02.qp_new());
    /*_p12.KEKE*/ wire KEKE_CH1_FREQ03n = not1(ch1.EDUL_CH1_FREQ03.qp_new());
    /*_p12.KOVO*/ wire KOVO_CH1_FREQ04n = not1(ch1.FELY_CH1_FREQ04.qp_new());
    /*_p12.FOHY*/ wire FOHY_CH1_FREQ05n = not1(ch1.HOLU_CH1_FREQ05.qp_new());
    /*_p12.DEBO*/ wire DEBO_CH1_FREQ06n = not1(ch1.HYXU_CH1_FREQ06.qp_new());
    /*_p12.CYKY*/ wire CYKY_CH1_FREQ07n = not1(ch1.HOPO_CH1_FREQ07.qp_new());
    /*_p12.BYRU*/ wire BYRU_CH1_FREQ08n = not1(ch1.DYGY_CH1_FREQ08.qp_new());
    /*_p12.APAT*/ wire APAT_CH1_FREQ09n = not1(ch1.EVAB_CH1_FREQ09.qp_new());
    /*_p12.BOJO*/ wire BOJO_CH1_FREQ10n = not1(ch1.AXAN_CH1_FREQ10.qp_new());

    /*_p12.KOKO*/ wire KOKO_CH1_FREQ00n = and2(JUTA_CH1_FREQ00n, KAPE_SHIFTER_LOADp);
    /*_p12.JOCY*/ wire JOCY_CH1_FREQ01n = and2(HOXE_CH1_FREQ01n, KAPE_SHIFTER_LOADp);
    /*_p12.JEHY*/ wire JEHY_CH1_FREQ02n = and2(HUNY_CH1_FREQ02n, KAPE_SHIFTER_LOADp);
    /*_p12.KAXY*/ wire KAXY_CH1_FREQ03n = and2(KEKE_CH1_FREQ03n, JUJU_SHIFTER_LOADp);
    /*_p12.KEVY*/ wire KEVY_CH1_FREQ04n = and2(KOVO_CH1_FREQ04n, JUJU_SHIFTER_LOADp);
    /*_p12.EVOF*/ wire EVOF_CH1_FREQ05n = and2(FOHY_CH1_FREQ05n, JUJU_SHIFTER_LOADp);
    /*_p12.EREG*/ wire EREG_CH1_FREQ06n = and2(DEBO_CH1_FREQ06n, JUJU_SHIFTER_LOADp);
    /*_p12.BAPU*/ wire BAPU_CH1_FREQ07n = and2(CYKY_CH1_FREQ07n, KEDO_SHIFTER_LOADp);
    /*_p12.AFUG*/ wire AFUG_CH1_FREQ08n = and2(BYRU_CH1_FREQ08n, KEDO_SHIFTER_LOADp);
    /*_p12.BUVO*/ wire BUVO_CH1_FREQ09n = and2(APAT_CH1_FREQ09n, KEDO_SHIFTER_LOADp);
    /*_p12.AFYR*/ wire AFYR_CH1_FREQ10n = and2(BOJO_CH1_FREQ10n, KEDO_SHIFTER_LOADp);

    /*_p12.HOZU*/ wire HOZU_SHIFTER_SETn_00 = nand2(ch1.HYKA_CH1_FREQ00.qp_new(), KAPE_SHIFTER_LOADp);
    /*_p12.HOLA*/ wire HOLA_SHIFTER_SETn_01 = nand2(ch1.JYKA_CH1_FREQ01.qp_new(), KAPE_SHIFTER_LOADp);
    /*_p12.HAWY*/ wire HAWY_SHIFTER_SETn_02 = nand2(ch1.HAVO_CH1_FREQ02.qp_new(), KAPE_SHIFTER_LOADp);
    /*_p12.KYRY*/ wire KYRY_SHIFTER_SETn_03 = nand2(ch1.EDUL_CH1_FREQ03.qp_new(), JUJU_SHIFTER_LOADp);
    /*_p12.KOLA*/ wire KOLA_SHIFTER_SETn_04 = nand2(ch1.FELY_CH1_FREQ04.qp_new(), JUJU_SHIFTER_LOADp);
    /*_p12.GOVO*/ wire GOVO_SHIFTER_SETn_05 = nand2(ch1.HOLU_CH1_FREQ05.qp_new(), JUJU_SHIFTER_LOADp);
    /*_p12.EKEM*/ wire EKEM_SHIFTER_SETn_06 = nand2(ch1.HYXU_CH1_FREQ06.qp_new(), JUJU_SHIFTER_LOADp);
    /*_p12.DACE*/ wire DACE_SHIFTER_SETn_07 = nand2(ch1.HOPO_CH1_FREQ07.qp_new(), KEDO_SHIFTER_LOADp);
    /*_p12.BEGE*/ wire BEGE_SHIFTER_SETn_08 = nand2(ch1.DYGY_CH1_FREQ08.qp_new(), KEDO_SHIFTER_LOADp);
    /*_p12.BESO*/ wire BESO_SHIFTER_SETn_09 = nand2(ch1.EVAB_CH1_FREQ09.qp_new(), KEDO_SHIFTER_LOADp);
    /*_p12.BEJU*/ wire BEJU_SHIFTER_SETn_10 = nand2(ch1.AXAN_CH1_FREQ10.qp_new(), KEDO_SHIFTER_LOADp);

    /*_p12.JADO*/ wire JADO_SHIFTER_RSTn_00 = nor2(KEBA_APU_RSTp(), KOKO_CH1_FREQ00n);
    /*_p12.HOBU*/ wire HOBU_SHIFTER_RSTn_01 = nor2(KEBA_APU_RSTp(), JOCY_CH1_FREQ01n);
    /*_p12.HYVU*/ wire HYVU_SHIFTER_RSTn_02 = nor2(KEBA_APU_RSTp(), JEHY_CH1_FREQ02n);
    /*_p12.KETO*/ wire KETO_SHIFTER_RSTn_03 = nor2(KEBA_APU_RSTp(), KAXY_CH1_FREQ03n);
    /*_p12.KYBO*/ wire KYBO_SHIFTER_RSTn_04 = nor2(KEBA_APU_RSTp(), KEVY_CH1_FREQ04n);
    /*_p12.EZUK*/ wire EZUK_SHIFTER_RSTn_05 = nor2(KEBA_APU_RSTp(), EVOF_CH1_FREQ05n);
    /*_p12.ENOK*/ wire ENOK_SHIFTER_RSTn_06 = nor2(KEBA_APU_RSTp(), EREG_CH1_FREQ06n);
    /*_p12.BEWO*/ wire BEWO_SHIFTER_RSTn_07 = nor2(KEBA_APU_RSTp(), BAPU_CH1_FREQ07n);
    /*_p12.AGOR*/ wire AGOR_SHIFTER_RSTn_08 = nor2(KEBA_APU_RSTp(), AFUG_CH1_FREQ08n);
    /*_p12.AFUX*/ wire AFUX_SHIFTER_RSTn_09 = nor2(KEBA_APU_RSTp(), BUVO_CH1_FREQ09n);
    /*_p12.AVUF*/ wire AVUF_SHIFTER_RSTn_10 = nor2(KEBA_APU_RSTp(), AFYR_CH1_FREQ10n);

    /*#p12.FABU*/ ch1.FABU_CH1_SHIFT00.dff22(ch1.EGOR_SHIFT_CLK(), HOZU_SHIFTER_SETn_00, JADO_SHIFTER_RSTn_00, ch1.JEFA_CH1_SHIFT01.qp_old());
    /*_p12.JEFA*/ ch1.JEFA_CH1_SHIFT01.dff22(ch1.EGOR_SHIFT_CLK(), HOLA_SHIFTER_SETn_01, HOBU_SHIFTER_RSTn_01, ch1.GOGA_CH1_SHIFT02.qp_old());
    /*_p12.GOGA*/ ch1.GOGA_CH1_SHIFT02.dff22(ch1.EGOR_SHIFT_CLK(), HAWY_SHIFTER_SETn_02, HYVU_SHIFTER_RSTn_02, ch1.JOLU_CH1_SHIFT03.qp_old());
    /*_p12.JOLU*/ ch1.JOLU_CH1_SHIFT03.dff22(ch1.EJYB_SHIFT_CLK(), KYRY_SHIFTER_SETn_03, KETO_SHIFTER_RSTn_03, ch1.JOTA_CH1_SHIFT04.qp_old());
    /*_p12.JOTA*/ ch1.JOTA_CH1_SHIFT04.dff22(ch1.EJYB_SHIFT_CLK(), KOLA_SHIFTER_SETn_04, KYBO_SHIFTER_RSTn_04, ch1.FUDE_CH1_SHIFT05.qp_old());
    /*_p12.FUDE*/ ch1.FUDE_CH1_SHIFT05.dff22(ch1.EJYB_SHIFT_CLK(), GOVO_SHIFTER_SETn_05, EZUK_SHIFTER_RSTn_05, ch1.FEDO_CH1_SHIFT06.qp_old());
    /*_p12.FEDO*/ ch1.FEDO_CH1_SHIFT06.dff22(ch1.EJYB_SHIFT_CLK(), EKEM_SHIFTER_SETn_06, ENOK_SHIFTER_RSTn_06, ch1.EXAC_CH1_SHIFT07.qp_old());
    /*_p12.EXAC*/ ch1.EXAC_CH1_SHIFT07.dff22(ch1.BECY_SHIFT_CLK(), DACE_SHIFTER_SETn_07, BEWO_SHIFTER_RSTn_07, ch1.ELUX_CH1_SHIFT08.qp_old());
    /*_p12.ELUX*/ ch1.ELUX_CH1_SHIFT08.dff22(ch1.BECY_SHIFT_CLK(), BEGE_SHIFTER_SETn_08, AGOR_SHIFTER_RSTn_08, ch1.AGEZ_CH1_SHIFT09.qp_old());
    /*_p12.AGEZ*/ ch1.AGEZ_CH1_SHIFT09.dff22(ch1.BECY_SHIFT_CLK(), BESO_SHIFTER_SETn_09, AFUX_SHIFTER_RSTn_09, ch1.BEKU_CH1_SHIFT10.qp_old());
    /*_p12.BEKU*/ ch1.BEKU_CH1_SHIFT10.dff22(ch1.BECY_SHIFT_CLK(), BEJU_SHIFTER_SETn_10, AVUF_SHIFTER_RSTn_10, 0);
  }

  //----------
  // Frequency summer

  {
    /*#p13.DAPU*/ wire DAPU_SHIFT_CLK = not1(ch1.EGOR_SHIFT_CLK());
    /*#p13.DACU*/ wire DACU_SHIFTCNT_LOADn = nor2(ch1.FEKU_CH1_TRIGp.qp_new(), ch1.BEXA_SWEEP_TRIGGERp.qp_new());
    /*#p13.CYLU*/ wire CYLU_SHIFTCNT_LOADp = not1(DACU_SHIFTCNT_LOADn);
    /*#p13.COPA*/ ch1.COPA_SHIFTCNT0.dff20(DAPU_SHIFT_CLK,              CYLU_SHIFTCNT_LOADp, ch1.BANY_NR10_SWEEP_SHIFT0.qp_new());
    /*#p13.CAJA*/ ch1.CAJA_SHIFTCNT1.dff20(ch1.COPA_SHIFTCNT0.qp_new(), CYLU_SHIFTCNT_LOADp, ch1.ARAX_NR10_SWEEP_SHIFT1.qp_new());
    /*#p13.BYRA*/ ch1.BYRA_SHIFTCNT2.dff20(ch1.CAJA_SHIFTCNT1.qp_new(), CYLU_SHIFTCNT_LOADp, ch1.ANAZ_NR10_SWEEP_SHIFT2.qp_new());

    /*#p13.ATAT*/ wire ATAT_SWEEP_RSTn = nor2(KEBA_APU_RSTp(), ch1.BEXA_SWEEP_TRIGGERp.qp_new());
    /*#p13.COPY*/ wire COPY_SHIFT_DONEp_old = and3(ch1.BYRA_SHIFTCNT2.qp_old(), ch1.CAJA_SHIFTCNT1.qp_old(), ch1.COPA_SHIFTCNT0.qp_old());
    /*#p13.BYTE*/ ch1.BYTE_SHIFT_DONEp.dff17(AJER_CLK_2M.qp_new(), ATAT_SWEEP_RSTn, COPY_SHIFT_DONEp_old);

    /*#p13.ADAD*/ wire ADAD_SHIFT_DONEp = not1(ch1.BYTE_SHIFT_DONEp.qn_new());

    /*#p13.EPUK*/ wire EPUK_SETn = nor2(KEBA_APU_RSTp(), ADAD_SHIFT_DONEp);
    /*#p13.EVOL*/ wire EVOL_RSTn = nor2(ch1.BEXA_SWEEP_TRIGGERp.qp_new(), ch1.FYTE_CH1_TRIGp.qp_new());
    /*#p13.FEMU*/ ch1.FEMU_SHIFTINGn.nand_latch(EPUK_SETn, EVOL_RSTn);

    /*#p13.KABA*/ wire KABA_SUM_RST = or2(KEBA_APU_RSTp(), ch1.FEKU_CH1_TRIGp.qp_new());
    /*#p13.KYNY*/ wire KYNY_SUM_RSTn = not1(KABA_SUM_RST);

    /*_p12.GALO*/ ch1.GALO_SUM_A00.dff11(ADAD_SHIFT_DONEp, KYNY_SUM_RSTn, ch1.HYKA_CH1_FREQ00.qp_old());
    /*_p12.JODE*/ ch1.JODE_SUM_A01.dff11(ADAD_SHIFT_DONEp, KYNY_SUM_RSTn, ch1.JYKA_CH1_FREQ01.qp_old());
    /*_p12.KARE*/ ch1.KARE_SUM_A02.dff11(ADAD_SHIFT_DONEp, KYNY_SUM_RSTn, ch1.HAVO_CH1_FREQ02.qp_old());
    /*_p12.JYME*/ ch1.JYME_SUM_A03.dff11(ADAD_SHIFT_DONEp, KYNY_SUM_RSTn, ch1.EDUL_CH1_FREQ03.qp_old());
    /*_p12.GYME*/ ch1.GYME_SUM_A04.dff11(ADAD_SHIFT_DONEp, KYNY_SUM_RSTn, ch1.FELY_CH1_FREQ04.qp_old());
    /*_p12.FAXO*/ ch1.FAXO_SUM_A05.dff11(ADAD_SHIFT_DONEp, KYNY_SUM_RSTn, ch1.HOLU_CH1_FREQ05.qp_old());
    /*_p12.EXAP*/ ch1.EXAP_SUM_A06.dff11(ADAD_SHIFT_DONEp, KYNY_SUM_RSTn, ch1.HYXU_CH1_FREQ06.qp_old());
    /*_p12.DELE*/ ch1.DELE_SUM_A07.dff11(ADAD_SHIFT_DONEp, KYNY_SUM_RSTn, ch1.HOPO_CH1_FREQ07.qp_old());
    /*_p12.DEXE*/ ch1.DEXE_SUM_A08.dff11(ADAD_SHIFT_DONEp, KYNY_SUM_RSTn, ch1.DYGY_CH1_FREQ08.qp_old());
    /*_p12.DOFY*/ ch1.DOFY_SUM_A09.dff11(ADAD_SHIFT_DONEp, KYNY_SUM_RSTn, ch1.EVAB_CH1_FREQ09.qp_old());
    /*_p12.DOLY*/ ch1.DOLY_SUM_A10.dff11(ADAD_SHIFT_DONEp, KYNY_SUM_RSTn, ch1.AXAN_CH1_FREQ10.qp_old());

    /*_p12.GYLO*/ wire GYLO_SHIFTER_FLIP_00 = xor2(ch1.ARYL_NR10_SWEEP_DIRn(), ch1.FABU_CH1_SHIFT00.qp_new());
    /*_p12.GELA*/ wire GELA_SHIFTER_FLIP_01 = xor2(ch1.ARYL_NR10_SWEEP_DIRn(), ch1.JEFA_CH1_SHIFT01.qp_new());
    /*_p12.GOPO*/ wire GOPO_SHIFTER_FLIP_02 = xor2(ch1.ARYL_NR10_SWEEP_DIRn(), ch1.GOGA_CH1_SHIFT02.qp_new());
    /*_p12.HEFY*/ wire HEFY_SHIFTER_FLIP_03 = xor2(ch1.ARYL_NR10_SWEEP_DIRn(), ch1.JOLU_CH1_SHIFT03.qp_new());
    /*_p12.KEFE*/ wire KEFE_SHIFTER_FLIP_04 = xor2(ch1.ARYL_NR10_SWEEP_DIRn(), ch1.JOTA_CH1_SHIFT04.qp_new());
    /*_p12.GOLY*/ wire GOLY_SHIFTER_FLIP_05 = xor2(ch1.ARYL_NR10_SWEEP_DIRn(), ch1.FUDE_CH1_SHIFT05.qp_new());
    /*_p12.FURE*/ wire FURE_SHIFTER_FLIP_06 = xor2(ch1.ARYL_NR10_SWEEP_DIRn(), ch1.FEDO_CH1_SHIFT06.qp_new());
    /*_p12.DYME*/ wire DYME_SHIFTER_FLIP_07 = xor2(ch1.ARYL_NR10_SWEEP_DIRn(), ch1.EXAC_CH1_SHIFT07.qp_new());
    /*_p12.CALE*/ wire CALE_SHIFTER_FLIP_08 = xor2(ch1.ARYL_NR10_SWEEP_DIRn(), ch1.ELUX_CH1_SHIFT08.qp_new());
    /*_p12.DOZY*/ wire DOZY_SHIFTER_FLIP_09 = xor2(ch1.ARYL_NR10_SWEEP_DIRn(), ch1.AGEZ_CH1_SHIFT09.qp_new());
    /*_p12.CULU*/ wire CULU_SHIFTER_FLIP_10 = xor2(ch1.ARYL_NR10_SWEEP_DIRn(), ch1.BEKU_CH1_SHIFT10.qp_new());

    /*_p12.HORA*/ ch1.HORA_SUM_B00.dff11(ADAD_SHIFT_DONEp, KYNY_SUM_RSTn, GYLO_SHIFTER_FLIP_00);
    /*_p12.HOPA*/ ch1.HOPA_SUM_B01.dff11(ADAD_SHIFT_DONEp, KYNY_SUM_RSTn, GELA_SHIFTER_FLIP_01);
    /*_p12.HELE*/ ch1.HELE_SUM_B02.dff11(ADAD_SHIFT_DONEp, KYNY_SUM_RSTn, GOPO_SHIFTER_FLIP_02);
    /*_p12.JAPE*/ ch1.JAPE_SUM_B03.dff11(ADAD_SHIFT_DONEp, KYNY_SUM_RSTn, HEFY_SHIFTER_FLIP_03);
    /*_p12.JETE*/ ch1.JETE_SUM_B04.dff11(ADAD_SHIFT_DONEp, KYNY_SUM_RSTn, KEFE_SHIFTER_FLIP_04);
    /*_p12.GELE*/ ch1.GELE_SUM_B05.dff11(ADAD_SHIFT_DONEp, KYNY_SUM_RSTn, GOLY_SHIFTER_FLIP_05);
    /*_p12.EPYR*/ ch1.EPYR_SUM_B06.dff11(ADAD_SHIFT_DONEp, KYNY_SUM_RSTn, FURE_SHIFTER_FLIP_06);
    /*_p12.EDOK*/ ch1.EDOK_SUM_B07.dff11(ADAD_SHIFT_DONEp, KYNY_SUM_RSTn, DYME_SHIFTER_FLIP_07);
    /*_p12.DEFA*/ ch1.DEFA_SUM_B08.dff11(ADAD_SHIFT_DONEp, KYNY_SUM_RSTn, CALE_SHIFTER_FLIP_08);
    /*_p12.ETER*/ ch1.ETER_SUM_B09.dff11(ADAD_SHIFT_DONEp, KYNY_SUM_RSTn, DOZY_SHIFTER_FLIP_09);
    /*_p12.DEVA*/ ch1.DEVA_SUM_B10.dff11(ADAD_SHIFT_DONEp, KYNY_SUM_RSTn, CULU_SHIFTER_FLIP_10);

    /*#p12.GUXA*/ Adder GUXA_SUM00 = add3(ch1.GALO_SUM_A00.qp_new(), ch1.HORA_SUM_B00.qp_new(), ch1.ARYL_NR10_SWEEP_DIRn());
    /*_p12.HALU*/ Adder HALU_SUM01 = add3(ch1.JODE_SUM_A01.qp_new(), ch1.HOPA_SUM_B01.qp_new(), GUXA_SUM00.carry);
    /*_p12.JULE*/ Adder JULE_SUM02 = add3(ch1.KARE_SUM_A02.qp_new(), ch1.HELE_SUM_B02.qp_new(), HALU_SUM01.carry);
    /*_p12.JORY*/ Adder JORY_SUM03 = add3(ch1.JYME_SUM_A03.qp_new(), ch1.JAPE_SUM_B03.qp_new(), JULE_SUM02.carry);
    /*_p12.HEXO*/ Adder HEXO_SUM04 = add3(ch1.GYME_SUM_A04.qp_new(), ch1.JETE_SUM_B04.qp_new(), JORY_SUM03.carry);
    /*_p12.GEVA*/ Adder GEVA_SUM05 = add3(ch1.FAXO_SUM_A05.qp_new(), ch1.GELE_SUM_B05.qp_new(), HEXO_SUM04.carry);
    /*#p12.FEGO*/ Adder FEGO_SUM06 = add3(ch1.EXAP_SUM_A06.qp_new(), ch1.EPYR_SUM_B06.qp_new(), GEVA_SUM05.carry);
    /*_p12.ETEK*/ Adder ETEK_SUM07 = add3(ch1.DELE_SUM_A07.qp_new(), ch1.EDOK_SUM_B07.qp_new(), FEGO_SUM06.carry);
    /*_p12.DYXE*/ Adder DYXE_SUM08 = add3(ch1.DEXE_SUM_A08.qp_new(), ch1.DEFA_SUM_B08.qp_new(), ETEK_SUM07.carry);
    /*_p12.DULE*/ Adder DULE_SUM09 = add3(ch1.DOFY_SUM_A09.qp_new(), ch1.ETER_SUM_B09.qp_new(), DYXE_SUM08.carry);
    /*_p12.CORU*/ Adder CORU_SUM10 = add3(ch1.DOLY_SUM_A10.qp_new(), ch1.DEVA_SUM_B10.qp_new(), DULE_SUM09.carry);

    /*#p12.BYLE*/ wire BYLE_FREQ_OVERFLOWn = nor2(ch1.ARYL_NR10_SWEEP_DIRn(), CORU_SUM10.carry);
    /*#p12.ATYS*/ wire ATYS_FREQ_OVERFLOWn = or2(BYLE_FREQ_OVERFLOWn, ch1.ARYL_NR10_SWEEP_DIRn());

    /*#p12.DEPU*/ wire DEPU_FF13_WRn = nand2(BOGY_CPU_WRp(), reg_new.cpu_abus.DECO_ADDR_FF13p());
    /*#p12.DYLA*/ wire DYLA_FF13_WRp = not1(DEPU_FF13_WRn);
    /*#p12.DEBY*/ wire DEBY_FF14_WRp = and2(BOGY_CPU_WRp(), reg_new.cpu_abus.DUJA_ADDR_FF14p());

    /*#p12.GYLU*/ wire GYLU_FREQ00_SET = nand2(DYLA_FF13_WRp, reg_old.cpu_dbus.BUS_CPU_D00p.qp_old());
    /*#p12.GETA*/ wire GETA_FREQ01_SET = nand2(DYLA_FF13_WRp, reg_old.cpu_dbus.BUS_CPU_D01p.qp_old());
    /*#p12.GOLO*/ wire GOLO_FREQ02_SET = nand2(DYLA_FF13_WRp, reg_old.cpu_dbus.BUS_CPU_D02p.qp_old());
    /*#p12.GOPE*/ wire GOPE_FREQ03_SET = nand2(DYLA_FF13_WRp, reg_old.cpu_dbus.BUS_CPU_D03p.qp_old());
    /*#p12.KOVU*/ wire KOVU_FREQ04_SET = nand2(DYLA_FF13_WRp, reg_old.cpu_dbus.BUS_CPU_D04p.qp_old());
    /*#p12.KYPA*/ wire KYPA_FREQ05_SET = nand2(DYLA_FF13_WRp, reg_old.cpu_dbus.BUS_CPU_D05p.qp_old());
    /*#p12.ELER*/ wire ELER_FREQ06_SET = nand2(DYLA_FF13_WRp, reg_old.cpu_dbus.BUS_CPU_D06p.qp_old());
    /*#p12.ETOL*/ wire ETOL_FREQ07_SET = nand2(DYLA_FF13_WRp, reg_old.cpu_dbus.BUS_CPU_D07p.qp_old());
    /*#p12.BUGU*/ wire BUGU_FREQ08_SET = nand2(DEBY_FF14_WRp, reg_old.cpu_dbus.BUS_CPU_D00p.qp_old());
    /*#p12.BUDO*/ wire BUDO_FREQ09_SET = nand2(DEBY_FF14_WRp, reg_old.cpu_dbus.BUS_CPU_D01p.qp_old());
    /*#p12.AFEG*/ wire AFEG_FREQ10_SET = nand2(DEBY_FF14_WRp, reg_old.cpu_dbus.BUS_CPU_D02p.qp_old());

    /*#p12.DEKE*/ wire DEKE = not1(reg_old.cpu_dbus.BUS_CPU_D00p.qp_old());
    /*#p12.GULU*/ wire GULU = not1(reg_old.cpu_dbus.BUS_CPU_D01p.qp_old());
    /*#p12.FULE*/ wire FULE = not1(reg_old.cpu_dbus.BUS_CPU_D02p.qp_old());
    /*#p12.ETUV*/ wire ETUV = not1(reg_old.cpu_dbus.BUS_CPU_D03p.qp_old());
    /*#p12.KOPU*/ wire KOPU = not1(reg_old.cpu_dbus.BUS_CPU_D04p.qp_old());
    /*#p12.JULO*/ wire JULO = not1(reg_old.cpu_dbus.BUS_CPU_D05p.qp_old());
    /*#p12.DYLU*/ wire DYLU = not1(reg_old.cpu_dbus.BUS_CPU_D06p.qp_old());
    /*#p12.DULO*/ wire DULO = not1(reg_old.cpu_dbus.BUS_CPU_D07p.qp_old());
    /*#p12.BYSU*/ wire BYSU = not1(reg_old.cpu_dbus.BUS_CPU_D00p.qp_old());
    /*#p12.BOFU*/ wire BOFU = not1(reg_old.cpu_dbus.BUS_CPU_D01p.qp_old());
    /*#p12.BYFU*/ wire BYFU = not1(reg_old.cpu_dbus.BUS_CPU_D02p.qp_old());

    /*#p12.EJYF*/ wire EJYF = and2(DYLA_FF13_WRp, DEKE);
    /*#p12.FOPU*/ wire FOPU = and2(DYLA_FF13_WRp, GULU);
    /*#p12.FOKE*/ wire FOKE = and2(DYLA_FF13_WRp, FULE);
    /*#p12.FEGA*/ wire FEGA = and2(DYLA_FF13_WRp, ETUV);
    /*#p12.KAVO*/ wire KAVO = and2(DYLA_FF13_WRp, KOPU);
    /*#p12.KYFU*/ wire KYFU = and2(DYLA_FF13_WRp, JULO);
    /*#p12.ETOK*/ wire ETOK = and2(DYLA_FF13_WRp, DYLU);
    /*#p12.EMAR*/ wire EMAR = and2(DYLA_FF13_WRp, DULO);
    /*#p12.BASO*/ wire BASO = and2(DEBY_FF14_WRp, BYSU);
    /*#p12.AMAC*/ wire AMAC = and2(DEBY_FF14_WRp, BOFU);
    /*#p12.AJUX*/ wire AJUX = and2(DEBY_FF14_WRp, BYFU);

    /*#p12.EFOR*/ wire EFOR_FREQ00_RST = nor2(EJYF, KEBA_APU_RSTp()); 
    /*#p12.GATO*/ wire GATO_FREQ01_RST = nor2(FOPU, KEBA_APU_RSTp()); 
    /*#p12.GYFU*/ wire GYFU_FREQ02_RST = nor2(FOKE, KEBA_APU_RSTp()); 
    /*#p12.GAMO*/ wire GAMO_FREQ03_RST = nor2(FEGA, KEBA_APU_RSTp()); 
    /*#p12.KAPO*/ wire KAPO_FREQ04_RST = nor2(KAVO, KEBA_APU_RSTp()); 
    /*#p12.KAJU*/ wire KAJU_FREQ05_RST = nor2(KYFU, KEBA_APU_RSTp()); 
    /*#p12.ELUF*/ wire ELUF_FREQ06_RST = nor2(ETOK, KEBA_APU_RSTp()); 
    /*#p12.ESEL*/ wire ESEL_FREQ07_RST = nor2(EMAR, KEBA_APU_RSTp()); 
    /*#p12.BOXU*/ wire BOXU_FREQ08_RST = nor2(BASO, KEBA_APU_RSTp()); 
    /*#p12.BOVU*/ wire BOVU_FREQ09_RST = nor2(AMAC, KEBA_APU_RSTp()); 
    /*#p12.APAJ*/ wire APAJ_FREQ10_RST = nor2(AJUX, KEBA_APU_RSTp()); 

    /*#p13.ATUV*/ wire ATUV_FREQ_CLK = and2(ch1.BEXA_SWEEP_TRIGGERp.qp_new(), ATYS_FREQ_OVERFLOWn);
    /*#p13.BOJE*/ wire BOJE_FREQ_CLK = and2(ATUV_FREQ_CLK, ch1.BUGE_SWEEP_DONEn());
    /*#p13.BUSO*/ wire BUSO_FREQ_CLK = and3(ch1.BEXA_SWEEP_TRIGGERp.qp_new(), ATYS_FREQ_OVERFLOWn, ch1.BUGE_SWEEP_DONEn());

    /*_p12.HYKA*/ ch1.HYKA_CH1_FREQ00.dff22(BOJE_FREQ_CLK, GYLU_FREQ00_SET, EFOR_FREQ00_RST, GUXA_SUM00.sum);
    /*_p12.JYKA*/ ch1.JYKA_CH1_FREQ01.dff22(BOJE_FREQ_CLK, GETA_FREQ01_SET, GATO_FREQ01_RST, HALU_SUM01.sum);
    /*_p12.HAVO*/ ch1.HAVO_CH1_FREQ02.dff22(BOJE_FREQ_CLK, GOLO_FREQ02_SET, GYFU_FREQ02_RST, JULE_SUM02.sum);
    /*_p12.EDUL*/ ch1.EDUL_CH1_FREQ03.dff22(BOJE_FREQ_CLK, GOPE_FREQ03_SET, GAMO_FREQ03_RST, JORY_SUM03.sum);
    /*_p12.FELY*/ ch1.FELY_CH1_FREQ04.dff22(BOJE_FREQ_CLK, KOVU_FREQ04_SET, KAPO_FREQ04_RST, HEXO_SUM04.sum);
    /*_p12.HOLU*/ ch1.HOLU_CH1_FREQ05.dff22(BOJE_FREQ_CLK, KYPA_FREQ05_SET, KAJU_FREQ05_RST, GEVA_SUM05.sum);
    /*_p12.HYXU*/ ch1.HYXU_CH1_FREQ06.dff22(BOJE_FREQ_CLK, ELER_FREQ06_SET, ELUF_FREQ06_RST, FEGO_SUM06.sum);
    /*_p12.HOPO*/ ch1.HOPO_CH1_FREQ07.dff22(BOJE_FREQ_CLK, ETOL_FREQ07_SET, ESEL_FREQ07_RST, ETEK_SUM07.sum);
    /*_p12.DYGY*/ ch1.DYGY_CH1_FREQ08.dff22(BUSO_FREQ_CLK, BUGU_FREQ08_SET, BOXU_FREQ08_RST, DYXE_SUM08.sum);
    /*_p12.EVAB*/ ch1.EVAB_CH1_FREQ09.dff22(BUSO_FREQ_CLK, BUDO_FREQ09_SET, BOVU_FREQ09_RST, DULE_SUM09.sum);
    /*_p12.AXAN*/ ch1.AXAN_CH1_FREQ10.dff22(BUSO_FREQ_CLK, AFEG_FREQ10_SET, APAJ_FREQ10_RST, CORU_SUM10.sum);

    /*#p13.BONE*/ wire BONE_FREQ_OVERFLOWp = not1(ATYS_FREQ_OVERFLOWn);
    /*#p13.CYFA*/ wire CYFA_LEN_DONEp = and2(ch1.CERO_CH1_LEN_DONE.qp_new(), ch1.BOKO_NR14_LEN_EN.qn_new());
    /*#p13.BERY*/ wire BERY_CH1_STOPp = or4(BONE_FREQ_OVERFLOWp, KEBA_APU_RSTp(), CYFA_LEN_DONEp, ch1.HOCA_CH1_AMP_ENn());
    /*#p13.CYTO*/ ch1.CYTO_CH1_ACTIVEp.nor_latch(ch1.FEKU_CH1_TRIGp.qp_new(), BERY_CH1_STOPp);
  }

  //----------
  // Envelope generator.

  {
    /*#p13.KALY*/ ch1.KALY_ENV_DELAY_CLK_64n.dff17(JONE_CLK_128p(), KADO_APU_RSTn(), ch1.KALY_ENV_DELAY_CLK_64n.qn_old());
    /*#p13.KERE*/ wire KERE_ENV_DELAY_CLK_64p = not1(ch1.KALY_ENV_DELAY_CLK_64n.qp_new());
    /*#p13.JOLA*/ wire JOLA_ENV_DELAY_CLK_64n = not1(KERE_ENV_DELAY_CLK_64p);

    /*#p13.KAZA*/ wire KAZA_ENV_DELAY_LOADn = nor2(ch1.FEKU_CH1_TRIGp.qp_new(), ch1.KOZY_ENV_TICKp.qp_new());
    /*#p13.KUXU*/ wire KUXU_ENV_DELAY_LOADp = not1(KAZA_ENV_DELAY_LOADn);

    /*#p13.JOVA*/ ch1.JOVA_ENV_DELAY0p.dff20(JOLA_ENV_DELAY_CLK_64n,        KUXU_ENV_DELAY_LOADp, ch1.JUSA_NR12_DELAY0n.qp_old());
    /*#p13.KENU*/ ch1.KENU_ENV_DELAY1p.dff20(ch1.JOVA_ENV_DELAY0p.qp_new(), KUXU_ENV_DELAY_LOADp, ch1.JUZY_NR12_DELAY1n.qp_old());
    /*#p13.KERA*/ ch1.KERA_ENV_DELAY2p.dff20(ch1.KENU_ENV_DELAY1p.qp_new(), KUXU_ENV_DELAY_LOADp, ch1.JOMA_NR12_DELAY2n.qp_old());

    // When the delay expires, we generate one env tick.
    /*#p13.KURY*/ wire KURY_ENV_CLKn = not1(ch1.KOZY_ENV_TICKp.qp_new());
    /*#p13.KUKU*/ wire KUKU_ENV_CLKp = nor2(HORU_CLK_512p(), KURY_ENV_CLKn);
    /*#p13.KORO*/ wire KORO_ENV_CLK_RSTn = nor4(KUKU_ENV_CLKp, ch1.KOMA_ENV_OFFp(), ch1.FEKU_CH1_TRIGp.qp_new(), KEBA_APU_RSTp());
    /*#p13.KOTE*/ wire KOTE_ENV_DELAY_DONEp = and3(ch1.KERA_ENV_DELAY2p.qp_new(), ch1.KENU_ENV_DELAY1p.qp_new(), ch1.JOVA_ENV_DELAY0p.qp_new());
    /*#p13.KOZY*/ ch1.KOZY_ENV_TICKp.dff17(HORU_CLK_512p(), KORO_ENV_CLK_RSTn, KOTE_ENV_DELAY_DONEp);

    // Check to see if our env is 0b0000 or 0b1111
    /*#p13.KORU*/ wire KORU_ENV_MAX_RSTn = nor2(ch1.FEKU_CH1_TRIGp.qp_new(), KEBA_APU_RSTp());
    /*#p13.HUFU*/ wire HUFU_ENV_TOPn = nand5(ch1.JAFY_NR12_ENV_DIR.qp_new(), ch1.HAFO_CH1_ENV0p.qp_new(), ch1.HEMY_CH1_ENV1p.qp_new(), ch1.HOKO_CH1_ENV2p.qp_new(), ch1.HEVO_CH1_ENV3p.qp_new());
    /*#p13.HAKE*/ wire HAKE_ENV_TOPp = not1(HUFU_ENV_TOPn);
    /*#p13.HANO*/ wire HANO_ENV_BOTp = nor5(ch1.JAFY_NR12_ENV_DIR.qp_new(), ch1.HAFO_CH1_ENV0p.qp_new(), ch1.HEMY_CH1_ENV1p.qp_new(), ch1.HOKO_CH1_ENV2p.qp_new(), ch1.HEVO_CH1_ENV3p.qp_new());
    /*#p13.JADE*/ wire JADE_ENV_MAXp = or2(HANO_ENV_BOTp, HAKE_ENV_TOPp);
    /*#p13.KYNO*/ ch1.KYNO_ENV_MAXp.dff17(ch1.KOZY_ENV_TICKp.qp_new(), KORU_ENV_MAX_RSTn, JADE_ENV_MAXp);

    // If our env maxes out, don't tick the envelope anymore.
    /*#p13.KEKO*/ wire KEKO_ENV_STARTp = or2(KEBA_APU_RSTp(), ch1.FEKU_CH1_TRIGp.qp_new());
    /*#p13.KEZU*/ ch1.KEZU_ENV_ACTIVEn.nor_latch(ch1.KYNO_ENV_MAXp.qp_new(), KEKO_ENV_STARTp);
    /*#p13.KAKE*/ wire KAKE_ENV_CLK = and3(ch1.KOZY_ENV_TICKp.qp_new(), ch1.KOMA_ENV_OFFp(), ch1.KEZU_ENV_ACTIVEn.qp_new());

    // The muxes select posedge or negedge for the env counter so it can count up _or_ down?
    // these were connected wrong in schematic

    /*#p13.JUFY*/ wire JUFY_ENV_CLK = amux2(ch1.JAFY_NR12_ENV_DIR.qn_new(), KAKE_ENV_CLK, KAKE_ENV_CLK, ch1.JAFY_NR12_ENV_DIR.qp_new());
    /*#p13.HAFO*/ ch1.HAFO_CH1_ENV0p.dff20(JUFY_ENV_CLK, ch1.FEKU_CH1_TRIGp.qp_new(), ch1.JATY_NR12_VOL0.qn_old());

    /*#p13.HYTO*/ wire HYTO_ENV_CLK = amux2(ch1.JAFY_NR12_ENV_DIR.qn_new(), ch1.HAFO_CH1_ENV0p.qp_new(), ch1.HAFO_CH1_ENV0p.qn_new(), ch1.JAFY_NR12_ENV_DIR.qp_new());
    /*#p13.HEMY*/ ch1.HEMY_CH1_ENV1p.dff20(HYTO_ENV_CLK, ch1.FEKU_CH1_TRIGp.qp_new(), ch1.JAXO_NR12_VOL1.qn_old());

    /*#p13.HETO*/ wire HETO_ENV_CLK = amux2(ch1.JAFY_NR12_ENV_DIR.qn_new(), ch1.HEMY_CH1_ENV1p.qp_new(), ch1.HEMY_CH1_ENV1p.qn_new(), ch1.JAFY_NR12_ENV_DIR.qp_new());
    /*#p13.HOKO*/ ch1.HOKO_CH1_ENV2p.dff20(HETO_ENV_CLK, ch1.FEKU_CH1_TRIGp.qp_new(), ch1.JENA_NR12_VOL2.qn_old());

    /*#p13.HESU*/ wire HESU_ENV_CLK = amux2(ch1.JAFY_NR12_ENV_DIR.qn_new(), ch1.HOKO_CH1_ENV2p.qp_new(), ch1.HOKO_CH1_ENV2p.qn_new(), ch1.JAFY_NR12_ENV_DIR.qp_new());
    /*#p13.HEVO*/ ch1.HEVO_CH1_ENV3p.dff20(HESU_ENV_CLK, ch1.FEKU_CH1_TRIGp.qp_new(), ch1.JOPU_NR12_VOL3.qn_old());
  }

  //----------
  // Waveform generator

  {
    // This is going to be an interesting bit of circular stuff to unwind...

    /*#p13.CALA*/ wire CALA_FREQ_OVERFLOWp = not1(ch1.COPU_CH1_FREQ_CNT_10.qp_new());
    /*#p13.DOKA*/ wire DOKA_COMY_RSTp = and2(ch1.COMY_SAMPLE_CLKp.qp_new(), ch1.DYFA_CLK_1M());
    /*#p13.DYRU*/ wire DYRU_COMY_RSTn = nor3(KEBA_APU_RSTp(), ch1.FEKU_CH1_TRIGp.qp_new(), DOKA_COMY_RSTp);
    /*_p13.COMY*/ ch1.COMY_SAMPLE_CLKp.dff17(CALA_FREQ_OVERFLOWp, DYRU_COMY_RSTn, ch1.COMY_SAMPLE_CLKp.qn_old());

    /*#p13.CYTE*/ wire CYTE_SAMPLE_CLKn = not1(ch1.COMY_SAMPLE_CLKp.qp_new());
    /*#p13.COPE*/ wire COPE_SAMPLE_CLKp = not1(CYTE_SAMPLE_CLKn);
    /*#p13.DAJO*/ wire DAJO_SAMPLE_CLKn = not1(COPE_SAMPLE_CLKp);

    /*#p11.EPYK*/ wire EPYK_FREQ_LOADn = nor2(COPE_SAMPLE_CLKp, ch1.FEKU_CH1_TRIGp.qp_new());
    /*#p11.FUME*/ wire FUME_FREQ_LOADp = not1(EPYK_FREQ_LOADn);
    /*#p11.DEGA*/ wire DEGA_FREQ_LOADp = not1(EPYK_FREQ_LOADn);
    /*#p11.DAKO*/ wire DAKO_FREQ_LOADp = not1(EPYK_FREQ_LOADn);

    // Frequency counter

    /*#p13.FEMY*/ wire FEMY_CH2_OFFn = nor2(ch1.HOCA_CH1_AMP_ENn(), KEBA_APU_RSTp());
    /*#p13.GEPU*/ wire GEPU_CH1_TRIGn = not1(ch1.FYTE_CH1_TRIGp.qp_new());
    /*#p13.GEXU*/ ch1.GEXU_FREQ_GATEn.nand_latch(FEMY_CH2_OFFn, GEPU_CH1_TRIGn);

    /*#p11.FULO*/ wire FULO_FREQ_CLK = nor2(ch1.DYFA_CLK_1M(), ch1.GEXU_FREQ_GATEn.qp_new());
    /*#p11.GEKU*/ wire GEKU_FREQ_CLK = not1(FULO_FREQ_CLK);
    /*#p11.GAXE*/ ch1.GAXE_CH1_FREQ_CNT_00.dff20(GEKU_FREQ_CLK,                     FUME_FREQ_LOADp, ch1.HYKA_CH1_FREQ00.qp_new());
    /*#p11.HYFE*/ ch1.HYFE_CH1_FREQ_CNT_01.dff20(ch1.GAXE_CH1_FREQ_CNT_00.qp_new(), FUME_FREQ_LOADp, ch1.JYKA_CH1_FREQ01.qp_new());
    /*_p11.JYTY*/ ch1.JYTY_CH1_FREQ_CNT_02.dff20(ch1.HYFE_CH1_FREQ_CNT_01.qp_new(), FUME_FREQ_LOADp, ch1.HAVO_CH1_FREQ02.qp_new());
    /*_p11.KYNA*/ ch1.KYNA_CH1_FREQ_CNT_03.dff20(ch1.JYTY_CH1_FREQ_CNT_02.qp_new(), FUME_FREQ_LOADp, ch1.EDUL_CH1_FREQ03.qp_new());

    /*#p11.KYPE*/ wire KYPE_FREQ_CLK = not1(ch1.KYNA_CH1_FREQ_CNT_03.qn_new());
    /*_p11.JEMA*/ ch1.JEMA_CH1_FREQ_CNT_04.dff20(KYPE_FREQ_CLK,                     DEGA_FREQ_LOADp, ch1.FELY_CH1_FREQ04.qp_new());
    /*_p11.HYKE*/ ch1.HYKE_CH1_FREQ_CNT_05.dff20(ch1.JEMA_CH1_FREQ_CNT_04.qp_new(), DEGA_FREQ_LOADp, ch1.HOLU_CH1_FREQ05.qp_new());
    /*_p11.FEVA*/ ch1.FEVA_CH1_FREQ_CNT_06.dff20(ch1.HYKE_CH1_FREQ_CNT_05.qp_new(), DEGA_FREQ_LOADp, ch1.HYXU_CH1_FREQ06.qp_new());
    /*_p11.EKOV*/ ch1.EKOV_CH1_FREQ_CNT_07.dff20(ch1.FEVA_CH1_FREQ_CNT_06.qp_new(), DEGA_FREQ_LOADp, ch1.HOPO_CH1_FREQ07.qp_new());

    /*#p11.DERU*/ wire DERU_FREQ_CLK = not1(ch1.EKOV_CH1_FREQ_CNT_07.qn_new());
    /*#p11.EMUS*/ ch1.EMUS_CH1_FREQ_CNT_08.dff20(DERU_FREQ_CLK,                     DAKO_FREQ_LOADp, ch1.DYGY_CH1_FREQ08.qp_new());
    /*#p11.EVAK*/ ch1.EVAK_CH1_FREQ_CNT_09.dff20(ch1.EMUS_CH1_FREQ_CNT_08.qp_new(), DAKO_FREQ_LOADp, ch1.EVAB_CH1_FREQ09.qp_new());
    /*#p11.COPU*/ ch1.COPU_CH1_FREQ_CNT_10.dff20(ch1.EVAK_CH1_FREQ_CNT_09.qp_new(), DAKO_FREQ_LOADp, ch1.AXAN_CH1_FREQ10.qp_new());

    // Duty cycle generator

    /*#p13.ESUT*/ ch1.ESUT_PHASE_xBxDxFxH.dff17(DAJO_SAMPLE_CLKn,                 CEPO_APU_RSTn(), ch1.ESUT_PHASE_xBxDxFxH.qn_old());
    /*#p13.EROS*/ ch1.EROS_PHASE_xxCDxxGH.dff13(ch1.ESUT_PHASE_xBxDxFxH.qn_new(), CEPO_APU_RSTn(), ch1.EROS_PHASE_xxCDxxGH.qn_old());
    /*#p13.DAPE*/ ch1.DAPE_PHASE_xxxxEFGH.dff13(ch1.EROS_PHASE_xxCDxxGH.qn_new(), CEPO_APU_RSTn(), ch1.DAPE_PHASE_xxxxEFGH.qn_old());

    /*#p13.DUVO*/ wire DUVO_PHASE_AxCxExGx = not1(ch1.ESUT_PHASE_xBxDxFxH.qp_new());
    /*#p13.EZOZ*/ wire EZOZ_PHASE_xxxxxxGH = and2(ch1.DAPE_PHASE_xxxxEFGH.qp_new(), ch1.EROS_PHASE_xxCDxxGH.qp_new());
    /*#p13.ENEK*/ wire ENEK_PHASE_xxxxxxGx = and2(EZOZ_PHASE_xxxxxxGH, DUVO_PHASE_AxCxExGx);
    /*#p13.CODO*/ wire CODO_PHASE_ABCDEFxx = not1(EZOZ_PHASE_xxxxxxGH);

    /*#p13.COSO*/ wire COSO = nor2(ch1.CENA_NR11_DUTY_0.qn_new(), ch1.DYCA_NR11_DUTY_1.qn_new());
    /*#p13.CAVA*/ wire CAVA = nor2(ch1.CENA_NR11_DUTY_0.qp_new(), ch1.DYCA_NR11_DUTY_1.qn_new());
    /*#p13.CEVU*/ wire CEVU = nor2(ch1.CENA_NR11_DUTY_0.qn_new(), ch1.DYCA_NR11_DUTY_1.qp_new());
    /*#p13.CAXO*/ wire CAXO = nor2(ch1.CENA_NR11_DUTY_0.qp_new(), ch1.DYCA_NR11_DUTY_1.qp_new());
    /*#p13.DUNA*/ wire DUNA_RAW_BIT = amux4(ENEK_PHASE_xxxxxxGx, COSO,
                                            EZOZ_PHASE_xxxxxxGH, CAVA,
                                            ch1.DAPE_PHASE_xxxxEFGH.qp_new(), CEVU,
                                            CODO_PHASE_ABCDEFxx, CAXO);
    /*_p13.DUWO*/ ch1.DUWO_RAW_BIT_SYNCp.dff17(COPE_SAMPLE_CLKp, CEPO_APU_RSTn(), DUNA_RAW_BIT);
  }

  //----------
  // Audio output

  {
    /*#p13.COWE*/ wire COWE_BIT_OUTp = and2(ch1.CYTO_CH1_ACTIVEp.qp_new(), ch1.DUWO_RAW_BIT_SYNCp.qp_new());
    /*#p13.BOTO*/ wire BOTO_BIT_OUTp = or2(COWE_BIT_OUTp, EDEK_NR52_DBG_APUp());
    /*#p13.AMOP*/ wire AMOP_CH1_OUT0 = and2(ch1.HAFO_CH1_ENV0p.qp_new(), BOTO_BIT_OUTp);
    /*#p13.ASON*/ wire ASON_CH1_OUT1 = and2(ch1.HEMY_CH1_ENV1p.qp_new(), BOTO_BIT_OUTp);
    /*#p13.AGOF*/ wire AGOF_CH1_OUT2 = and2(ch1.HOKO_CH1_ENV2p.qp_new(), BOTO_BIT_OUTp);
    /*#p13.ACEG*/ wire ACEG_CH1_OUT3 = and2(ch1.HEVO_CH1_ENV3p.qp_new(), BOTO_BIT_OUTp);
  }
}

//-----------------------------------------------------------------------------

void GBSound::tick_nr10(const GateBoyState& reg_old, GateBoyState& reg_new) {
  /*#p11.CENU*/ wire CENU_NR10_WRp = and2(BOGY_CPU_WRp(), reg_new.cpu_abus.DYVA_ADDR_FF10p());
  /*#p11.CENU*/ wire CENU_NR10_WRn = not1(CENU_NR10_WRp);

  /*#p11.BANY*/ ch1.BANY_NR10_SWEEP_SHIFT0  .dff9(CENU_NR10_WRn, AGUR_APU_RSTn(), reg_old.cpu_dbus.BUS_CPU_D00p.qp_old());
  /*#p11.ARAX*/ ch1.ARAX_NR10_SWEEP_SHIFT1  .dff9(CENU_NR10_WRn, AGUR_APU_RSTn(), reg_old.cpu_dbus.BUS_CPU_D01p.qp_old());
  /*#p11.ANAZ*/ ch1.ANAZ_NR10_SWEEP_SHIFT2  .dff9(CENU_NR10_WRn, AGUR_APU_RSTn(), reg_old.cpu_dbus.BUS_CPU_D02p.qp_old());
  /*#p11.AVAF*/ ch1.AVAF_NR10_SWEEP_DIR_p   .dff9(CENU_NR10_WRn, AGUR_APU_RSTn(), reg_old.cpu_dbus.BUS_CPU_D03p.qp_old());
  /*#p11.ADEK*/ ch1.ADEK_NR10_SWEEP_PERIOD0p.dff9(CENU_NR10_WRn, AGUR_APU_RSTn(), reg_old.cpu_dbus.BUS_CPU_D04p.qp_old());
  /*#p11.BANA*/ ch1.BANA_NR10_SWEEP_PERIOD1p.dff9(CENU_NR10_WRn, AGUR_APU_RSTn(), reg_old.cpu_dbus.BUS_CPU_D05p.qp_old());
  /*#p11.BOTU*/ ch1.BOTU_NR10_SWEEP_PERIOD2p.dff9(CENU_NR10_WRn, AGUR_APU_RSTn(), reg_old.cpu_dbus.BUS_CPU_D06p.qp_old());

  /*#p11.ATYN*/ wire ATYN_NR10_RDp = nor2(AGUZ_CPU_RDn(), reg_new.cpu_abus.BUZE_ADDR_FF10n());
  /*#p11.ASOP*/ wire ASOP_NR10_RDn = not1(ATYN_NR10_RDp);

  // Isn't the data polarity here backwards?
  /*#p11.AMYD*/ triwire AMYD_NR10_SWEEP_SHIFT0  = tri6_nn(ASOP_NR10_RDn, ch1.BANY_NR10_SWEEP_SHIFT0.qp_new());
  /*#p11.ATAX*/ triwire ATAX_NR10_SWEEP_SHIFT1  = tri6_nn(ASOP_NR10_RDn, ch1.ARAX_NR10_SWEEP_SHIFT1.qp_new());
  /*#p11.AZYP*/ triwire AZYP_NR10_SWEEP_SHIFT2  = tri6_nn(ASOP_NR10_RDn, ch1.ANAZ_NR10_SWEEP_SHIFT2.qp_new());
  /*#p11.AFOX*/ triwire AFOX_NR10_SWEEP_NEGATE  = tri6_nn(ASOP_NR10_RDn, ch1.AVAF_NR10_SWEEP_DIR_p.qp_new());
  /*#p11.AVEK*/ triwire AVEK_NR10_SWEEP_PERIOD0 = tri6_nn(ASOP_NR10_RDn, ch1.ADEK_NR10_SWEEP_PERIOD0p.qp_new());
  /*#p11.AKUX*/ triwire AKUX_NR10_SWEEP_PERIOD1 = tri6_nn(ASOP_NR10_RDn, ch1.BANA_NR10_SWEEP_PERIOD1p.qp_new());
  /*#p11.AWOS*/ triwire AWOS_NR10_SWEEP_PERIOD2 = tri6_nn(ASOP_NR10_RDn, ch1.BOTU_NR10_SWEEP_PERIOD2p.qp_new());
}

//-----------------------------------------------------------------------------

void GBSound::tick_nr11(const GateBoyState& reg_old, GateBoyState& reg_new) {

  /*#p11.BEXU*/ wire BEXU_NR11_RDn = nand2(BUWA_CPU_RDp(), reg_new.cpu_abus.CAXE_ADDR_FF11p());
  /*#p11.COVU*/ wire COVU_NR11_WR = and2(BOGY_CPU_WRp(), reg_new.cpu_abus.CAXE_ADDR_FF11p());

  /*#p11.DAFO*/ wire DAFO_NR11_CLK = not1(COVU_NR11_WR);
  /*#p11.CENA*/ ch1.CENA_NR11_DUTY_0.dff9(DAFO_NR11_CLK, CEPO_APU_RSTn(), reg_old.cpu_dbus.BUS_CPU_D06p.qp_old());
  /*#p11.DYCA*/ ch1.DYCA_NR11_DUTY_1.dff9(DAFO_NR11_CLK, CEPO_APU_RSTn(), reg_old.cpu_dbus.BUS_CPU_D07p.qp_old());

  /*#p13.CAPY*/ wire CAPY_CH1_LEN_CLKn = nor3(ch1.CERO_CH1_LEN_DONE.qp_new(), BUFY_CLK_256n(), ch1.BOKO_NR14_LEN_EN.qp_new());
  /*#p13.CANU*/ wire CANU_CH1_LEN_CLK = not1(CAPY_CH1_LEN_CLKn);

  /*#p13.BORO*/ wire BORO_FF11_WRna = nand2(BOGY_CPU_WRp(), reg_new.cpu_abus.CAXE_ADDR_FF11p());
  /*#p13.BEPE*/ wire BEPE_FF11_WRa = not1(BORO_FF11_WRna);
  /*#p13.BUGY*/ wire BUGY_FF11_WRc = not1(BORO_FF11_WRna);
  /*#p13.BOKA*/ wire BOKA_FF11_WRb = not1(BORO_FF11_WRna);

  /*#p13.CORY*/ wire CORY_CH1_LEN_DONE_RST = nor3(BOKA_FF11_WRb, KEBA_APU_RSTp(), ch1.FEKU_CH1_TRIGp.qp_new());
  
  /*_p13.CERO*/ ch1.CERO_CH1_LEN_DONE.dff17(ch1.ERAM_NR11_LEN5.qn_new(), CORY_CH1_LEN_DONE_RST, ch1.CERO_CH1_LEN_DONE.qn_old());

  /*#p13.BACY*/ ch1.BACY_NR11_LEN0.dff20(CANU_CH1_LEN_CLK,              BUGY_FF11_WRc, reg_old.cpu_dbus.BUS_CPU_D00p.qp_old());
  /*#p13.CAVY*/ ch1.CAVY_NR11_LEN1.dff20(ch1.BACY_NR11_LEN0.qp_new(),   BUGY_FF11_WRc, reg_old.cpu_dbus.BUS_CPU_D01p.qp_old());
  /*#p13.BOVY*/ ch1.BOVY_NR11_LEN2.dff20(ch1.CAVY_NR11_LEN1.qp_new(),   BUGY_FF11_WRc, reg_old.cpu_dbus.BUS_CPU_D02p.qp_old());
  /*#p13.CUNO*/ ch1.CUNO_NR11_LEN3.dff20(ch1.BOVY_NR11_LEN2.qp_new(),   BUGY_FF11_WRc, reg_old.cpu_dbus.BUS_CPU_D03p.qp_old());

  /*#p13.CUSO*/ wire CUSO_NR11_LEN3p = not1(ch1.CUNO_NR11_LEN3.qn_new());
  /*#p13.CURA*/ ch1.CURA_NR11_LEN4.dff20(CUSO_NR11_LEN3p,               BEPE_FF11_WRa, reg_old.cpu_dbus.BUS_CPU_D04p.qp_old());
  /*#p13.ERAM*/ ch1.ERAM_NR11_LEN5.dff20(ch1.CURA_NR11_LEN4.qp_new(),   BEPE_FF11_WRa, reg_old.cpu_dbus.BUS_CPU_D05p.qp_old());

  /*#p11.BOWO*/ triwire BOWO = tri6_nn(BEXU_NR11_RDn, ch1.CENA_NR11_DUTY_0.qp_new());
  /*#p11.CUDA*/ triwire CUDA = tri6_nn(BEXU_NR11_RDn, ch1.DYCA_NR11_DUTY_1.qp_new());
}

//-----------------------------------------------------------------------------

void GBSound::tick_nr12(const GateBoyState& reg_old, GateBoyState& reg_new) {

  /*#p11.HAFU*/ wire HAFU_NR12_WRp = and2(BOGY_CPU_WRp(), reg_new.cpu_abus.EDAF_ADDR_FF12p());
  /*#p11.KYGY*/ wire KYGY_NR12_WRn = not1(HAFU_NR12_WRp);
  /*#p11.GAXU*/ wire GAXU_NR12_WRn = nand2(reg_new.cpu_abus.EDAF_ADDR_FF12p(), BOGY_CPU_WRp());
  wire KAGY_NR12_WRp = not1(GAXU_NR12_WRn); // not on schematic?

  /*#p11.JUSA*/ ch1.JUSA_NR12_DELAY0n .dff9(KYGY_NR12_WRn, HATO_APU_RSTn(), reg_old.cpu_dbus.BUS_CPU_D00p.qp_old());
  /*#p11.JUZY*/ ch1.JUZY_NR12_DELAY1n .dff9(KYGY_NR12_WRn, HATO_APU_RSTn(), reg_old.cpu_dbus.BUS_CPU_D01p.qp_old());
  /*#p11.JOMA*/ ch1.JOMA_NR12_DELAY2n .dff9(KYGY_NR12_WRn, HATO_APU_RSTn(), reg_old.cpu_dbus.BUS_CPU_D02p.qp_old());
  /*#p11.JAFY*/ ch1.JAFY_NR12_ENV_DIR .dff9(KAGY_NR12_WRp, HATO_APU_RSTn(), reg_old.cpu_dbus.BUS_CPU_D03p.qp_old());
  /*#p11.JATY*/ ch1.JATY_NR12_VOL0    .dff9(KAGY_NR12_WRp, HATO_APU_RSTn(), reg_old.cpu_dbus.BUS_CPU_D04p.qp_old());
  /*#p11.JAXO*/ ch1.JAXO_NR12_VOL1    .dff9(KAGY_NR12_WRp, HATO_APU_RSTn(), reg_old.cpu_dbus.BUS_CPU_D05p.qp_old());
  /*#p11.JENA*/ ch1.JENA_NR12_VOL2    .dff9(KAGY_NR12_WRp, HATO_APU_RSTn(), reg_old.cpu_dbus.BUS_CPU_D06p.qp_old());
  /*#p11.JOPU*/ ch1.JOPU_NR12_VOL3    .dff9(KAGY_NR12_WRp, HATO_APU_RSTn(), reg_old.cpu_dbus.BUS_CPU_D07p.qp_old());

  /*#p11.HAMY*/ wire HAMY_NR12_RDn = or2(reg_new.cpu_abus.HAXE_ADDR_FF12n(), AGUZ_CPU_RDn());
  /*#p11.HOCU*/ wire HOCU_NR12_RDn = or2(reg_new.cpu_abus.GAGO_ADDR_FF12n(), AGUZ_CPU_RDn());

  // data polarity backwards again?
  /*#p11.JYNE*/ triwire JYNE = tri6_nn(HAMY_NR12_RDn, ch1.JUSA_NR12_DELAY0n.qp_new());
  /*#p11.JACA*/ triwire JACA = tri6_nn(HAMY_NR12_RDn, ch1.JUZY_NR12_DELAY1n.qp_new());
  /*#p11.JOKU*/ triwire JOKU = tri6_nn(HAMY_NR12_RDn, ch1.JOMA_NR12_DELAY2n.qp_new());
  /*#p11.HONO*/ triwire HONO = tri6_nn(HOCU_NR12_RDn, ch1.JAFY_NR12_ENV_DIR.qp_new());
  /*#p11.HOWU*/ triwire HOWU = tri6_nn(HOCU_NR12_RDn, ch1.JATY_NR12_VOL0.qp_new());
  /*#p11.HEWA*/ triwire HEWA = tri6_nn(HOCU_NR12_RDn, ch1.JAXO_NR12_VOL1.qp_new());
  /*#p11.HEVE*/ triwire HEVE = tri6_nn(HOCU_NR12_RDn, ch1.JENA_NR12_VOL2.qp_new());
  /*#p11.JYSE*/ triwire JYSE = tri6_nn(HOCU_NR12_RDn, ch1.JOPU_NR12_VOL3.qp_new());

  /*_BUS_CPU_D00p*/ reg_new.cpu_dbus.BUS_CPU_D00p.tri_bus(JYNE);
  /*_BUS_CPU_D01p*/ reg_new.cpu_dbus.BUS_CPU_D01p.tri_bus(JACA);
  /*_BUS_CPU_D02p*/ reg_new.cpu_dbus.BUS_CPU_D02p.tri_bus(JOKU);
  /*_BUS_CPU_D03p*/ reg_new.cpu_dbus.BUS_CPU_D03p.tri_bus(HONO);
  /*_BUS_CPU_D04p*/ reg_new.cpu_dbus.BUS_CPU_D04p.tri_bus(HOWU);
  /*_BUS_CPU_D05p*/ reg_new.cpu_dbus.BUS_CPU_D05p.tri_bus(HEWA);
  /*_BUS_CPU_D06p*/ reg_new.cpu_dbus.BUS_CPU_D06p.tri_bus(HEVE);
  /*_BUS_CPU_D07p*/ reg_new.cpu_dbus.BUS_CPU_D07p.tri_bus(JYSE);
}

//-----------------------------------------------------------------------------

void GBSound::tick_nr13(const GateBoyState& reg_old, GateBoyState& reg_new) {
  // Frequency can be read from FF13/FF14 in debug mode

  /*#p11.DYPU*/ wire DYPU_FF13_RDn = nor2(reg_new.cpu_abus.CACA_ADDR_FF13n(), DAXA_CPU_RDn_DBGn());
  /*#p11.EVAJ*/ wire EVAJ_FF13_RDa = not1(DYPU_FF13_RDn);

  /*_p11.FORU*/ triwire FORU = tri6_nn(EVAJ_FF13_RDa, ch1.GAXE_CH1_FREQ_CNT_00.qn_new());
  /*_p11.GEFU*/ triwire GEFU = tri6_nn(EVAJ_FF13_RDa, ch1.HYFE_CH1_FREQ_CNT_01.qn_new());
  /*_p11.KYVU*/ triwire KYVU = tri6_nn(EVAJ_FF13_RDa, ch1.JYTY_CH1_FREQ_CNT_02.qn_new());
  /*_p11.KUMO*/ triwire KUMO = tri6_nn(EVAJ_FF13_RDa, ch1.KYNA_CH1_FREQ_CNT_03.qn_new());
  /*_p11.KARY*/ triwire KARY = tri6_nn(EVAJ_FF13_RDa, ch1.JEMA_CH1_FREQ_CNT_04.qn_new());
  /*_p11.GODE*/ triwire GODE = tri6_nn(EVAJ_FF13_RDa, ch1.HYKE_CH1_FREQ_CNT_05.qn_new());
  /*_p11.GOJE*/ triwire GOJE = tri6_nn(EVAJ_FF13_RDa, ch1.FEVA_CH1_FREQ_CNT_06.qn_new());
  /*_p11.FOZE*/ triwire FOZE = tri6_nn(EVAJ_FF13_RDa, ch1.EKOV_CH1_FREQ_CNT_07.qn_new());
}

//-----------------------------------------------------------------------------
// Frequency can be read from FF13/FF14 in debug mode

void GBSound::tick_nr14(const GateBoyState& reg_old, GateBoyState& reg_new) {
  {
    /*#p13.DADO*/ wire DADO_NR14_RSTb = nor2(KEBA_APU_RSTp(), ch1.EZEC_CH1_TRIGn.qp_new());

    /*#p11.BAGE*/ wire BAGE_NR14_WRn = nand2(ANUJ_CPU_WR_WEIRD(), reg_new.cpu_abus.DUJA_ADDR_FF14p());
    /*#p13.DOGE*/ wire DOGE_FF14_WRn = nand2(BOGY_CPU_WRp(), reg_new.cpu_abus.DUJA_ADDR_FF14p());

    /*#p11.BOKO*/ ch1.BOKO_NR14_LEN_EN.dff9(BAGE_NR14_WRn, CAMY_APU_RSTn(), reg_old.cpu_dbus.BUS_CPU_D06p.qp_old());
    /*#p13.DUPE*/ ch1.DUPE_NR14_TRIG  .dff9(DOGE_FF14_WRn, DADO_NR14_RSTb,  reg_old.cpu_dbus.BUS_CPU_D07p.qp_old());
  }

  {
    /*#p11.BALE*/ wire BALE_NR14_RDn = nand2(reg_new.cpu_abus.DUJA_ADDR_FF14p(), BUDA_CPU_RDp());
    /*#p11.DUPY*/ wire DUPY_FF14_RDn = or2(reg_new.cpu_abus.CURE_ADDR_FF14n(), DAXA_CPU_RDn_DBGn());
 
    /*#p11.BYTU*/ triwire BYTU = tri6_nn(BALE_NR14_RDn, ch1.BOKO_NR14_LEN_EN.qp_new());
    /*_p11.DOPA*/ triwire DOPA = tri6_nn(DUPY_FF14_RDn, ch1.EMUS_CH1_FREQ_CNT_08.qn_new());
    /*_p11.DEMU*/ triwire DEMU = tri6_nn(DUPY_FF14_RDn, ch1.EVAK_CH1_FREQ_CNT_09.qn_new());
    /*_p11.DEXO*/ triwire DEXO = tri6_nn(DUPY_FF14_RDn, ch1.COPU_CH1_FREQ_CNT_10.qn_new());
  }
}















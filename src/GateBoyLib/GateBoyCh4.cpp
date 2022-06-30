#include "GateBoySPU.h"

#include "GateBoyState.h"

void GBSound::tick_ch4(const GateBoyState& reg_old, GateBoyState& reg_new) {
  {
    /*#p20.GYSU*/ ch4.GYSU_CH4_TRIG.dff17(DOVA_ABCDxxxx(), GASO_APU_RSTn(), ch4.HOGA_NR44_TRIG.qn_old());
  }

  {
    /*#p20.EFOT*/ wire EFOT_CH4_STOPp = and2(ch4.CUNY_NR44_LEN_ENp.qn_new(), ch4.FUGO_CH4_LEN_DONEp.qp_new());
    /*#p20.FEGY*/ wire FEGY_CH4_STOPp = or3(KEBA_APU_RSTp(), EFOT_CH4_STOPp, ch4.GEVY_CH4_AMP_ENn());
    /*#p20.GENA*/ ch4.GENA_CH4_ACTIVEp.nor_latch(ch4.GONE_CH4_TRIGp.qp_new(), FEGY_CH4_STOPp);
  }

  {
    /*#p20.FALE*/ wire FALE_RESET_LATCHp = nor2(KEBA_APU_RSTp(), ch4.GORA_CH4_TRIGp.qp_new());
    /*#p20.HELU*/ wire HELU_RESET_LATCHp  = not1(FALE_RESET_LATCHp);
    /*#p20.HAZO*/ ch4.HAZO_CH4_TRIGn.nor_latch(HELU_RESET_LATCHp, ch4.GYSU_CH4_TRIG.qp_new());

    /*#p20.GONE*/ ch4.GONE_CH4_TRIGp.dff17(HAMA_CLK_512K(), FALE_RESET_LATCHp, ch4.HAZO_CH4_TRIGn.qn_new());
    /*#p20.GORA*/ ch4.GORA_CH4_TRIGp.dff17(HAMA_CLK_512K(), FEBY_APU_RSTn(),   ch4.GONE_CH4_TRIGp.qp_old());
    /*#p20.GATY*/ ch4.GATY_CH4_TRIGp.dff17(HAMA_CLK_512K(), FEBY_APU_RSTn(),   ch4.GORA_CH4_TRIGp.qp_old());
  }

  {
    /*#p20.HERY*/ wire HERY_DIV_GATE1n = nor2(ch4.GEVY_CH4_AMP_ENn(), KEBA_APU_RSTp());
    /*#p20.HAPU*/ wire HAPU_CH4_TRIGn = not1(ch4.GATY_CH4_TRIGp.qp_new());
    /*#p20.JERY*/ ch4.JERY_DIV_GATE.nand_latch(HERY_DIV_GATE1n, HAPU_CH4_TRIGn);
  }

  {
    /*#p20.KYKU*/ wire KYKU_DIV_CLKb  = or2(ch4.JERY_DIV_GATE.qp_new(), JESO_CLK_512K.qp_new());
    /*#p20.KONY*/ wire KONY_DIV_CLKn  = not1(KYKU_DIV_CLKb);
    /*#p20.KANU*/ wire KANU_DIV_CLKa  = not1(KONY_DIV_CLKn);

    /*#p20.GOFU*/ wire GOFU_DIV_LOADn = nor2(ch4.GONE_CH4_TRIGp.qp_new(), ch4.GARY_FREQ_GATEp.qp_new());
    /*#p20.HUCE*/ wire HUCE_DIV_LOAD  = not1(GOFU_DIV_LOADn);

    /*#p20.JYCO*/ ch4.JYCO_DIV0.dff20(KANU_DIV_CLKa,          HUCE_DIV_LOAD, ch4.JARE_NR43_DIV0.qp_old());
    /*#p20.JYRE*/ ch4.JYRE_DIV1.dff20(ch4.JYCO_DIV0.qp_new(), HUCE_DIV_LOAD, ch4.JERO_NR43_DIV1.qp_old());
    /*_p20.JYFU*/ ch4.JYFU_DIV2.dff20(ch4.JYRE_DIV1.qp_new(), HUCE_DIV_LOAD, ch4.JAKY_NR43_DIV2.qp_old());
  }

  {
    /*#p20.GUNY*/ wire GUNY_FREQ_GATE_RSTn = nor2(KEBA_APU_RSTp(), ch4.GONE_CH4_TRIGp.qp_new());
    /*#p20.HYNO*/ wire HYNO_DIV_MAX = or3(ch4.JYFU_DIV2.qp_new(), ch4.JYRE_DIV1.qp_new(), ch4.JYCO_DIV0.qp_new());
    /*#p20.GARY*/ ch4.GARY_FREQ_GATEp.dff17(GYBA_CLK_1M(), GUNY_FREQ_GATE_RSTn, HYNO_DIV_MAX);
  }

  {

    /*#p20.CARY*/ wire CARY_FREQ_CLK = and2(BAVU_CLK_1M(), ch4.GARY_FREQ_GATEp.qp_new());
    /*#p20.CEXO*/ ch4.CEXO_FREQ_00.dff17(CARY_FREQ_CLK,             DAPA_APU_RSTn(), ch4.CEXO_FREQ_00.qn_old());
    /*_p20.DEKO*/ ch4.DEKO_FREQ_01.dff17(ch4.CEXO_FREQ_00.qn_new(), DAPA_APU_RSTn(), ch4.DEKO_FREQ_01.qn_old());
    /*_p20.EZEF*/ ch4.EZEF_FREQ_02.dff17(ch4.DEKO_FREQ_01.qn_new(), DAPA_APU_RSTn(), ch4.EZEF_FREQ_02.qn_old());
    /*_p20.EPOR*/ ch4.EPOR_FREQ_03.dff17(ch4.EZEF_FREQ_02.qn_new(), DAPA_APU_RSTn(), ch4.EPOR_FREQ_03.qn_old());
    /*_p20.DURE*/ ch4.DURE_FREQ_04.dff17(ch4.EPOR_FREQ_03.qn_new(), DAPA_APU_RSTn(), ch4.DURE_FREQ_04.qn_old());
    /*_p20.DALE*/ ch4.DALE_FREQ_05.dff17(ch4.DURE_FREQ_04.qn_new(), DAPA_APU_RSTn(), ch4.DALE_FREQ_05.qn_old());
    /*_p20.DOKE*/ ch4.DOKE_FREQ_06.dff17(ch4.DALE_FREQ_05.qn_new(), DAPA_APU_RSTn(), ch4.DOKE_FREQ_06.qn_old());
    /*_p20.DEMO*/ ch4.DEMO_FREQ_07.dff17(ch4.DOKE_FREQ_06.qn_new(), DAPA_APU_RSTn(), ch4.DEMO_FREQ_07.qn_old());
    /*_p20.DOSE*/ ch4.DOSE_FREQ_08.dff17(ch4.DEMO_FREQ_07.qn_new(), DAPA_APU_RSTn(), ch4.DOSE_FREQ_08.qn_old());
    /*_p20.DETE*/ ch4.DETE_FREQ_09.dff17(ch4.DOSE_FREQ_08.qn_new(), DAPA_APU_RSTn(), ch4.DETE_FREQ_09.qn_old());
    /*_p20.ERUT*/ ch4.ERUT_FREQ_10.dff17(ch4.DETE_FREQ_09.qn_new(), DAPA_APU_RSTn(), ch4.ERUT_FREQ_10.qn_old());
    /*_p20.DOTA*/ ch4.DOTA_FREQ_11.dff17(ch4.ERUT_FREQ_10.qn_new(), DAPA_APU_RSTn(), ch4.DOTA_FREQ_11.qn_old());
    /*_p20.DERE*/ ch4.DERE_FREQ_12.dff17(ch4.DOTA_FREQ_11.qn_new(), DAPA_APU_RSTn(), ch4.DERE_FREQ_12.qn_old());
    /*_p20.ESEP*/ ch4.ESEP_FREQ_13.dff17(ch4.DERE_FREQ_12.qn_new(), DAPA_APU_RSTn(), ch4.ESEP_FREQ_13.qn_old());
  }

  //----------
  // The actual LFSR

  // 14-to-1 mux to select LFSR clock

  // FIXME did you just copy-paste this block and not finish it?

  {
    /*#p20.GEPO*/ wire GEPO_LFSR_RSTa = or2(ch4.GONE_CH4_TRIGp.qp_new(), KEBA_APU_RSTp());
    /*#p20.GOGE*/ wire GOGE_LFSR_RSTn = not1(GEPO_LFSR_RSTa);

    /*#p20.HURA*/ wire HURA_LFSR_IN = xor2(ch4.HEZU_LFSR_15.qp_old(), ch4.HYRO_LFSR_14.qp_old());
    /*#p20.JOTO*/ ch4.JOTO_LFSR_00.dff17(ch4.JYJA_LFSR_CLKn(), GOGE_LFSR_RSTn, HURA_LFSR_IN);
    /*#p20.KOMU*/ ch4.KOMU_LFSR_01.dff17(ch4.KOPA_LFSR_CLKp(), GOGE_LFSR_RSTn, ch4.JOTO_LFSR_00.qp_new());
    /*#p20.KETU*/ ch4.KETU_LFSR_02.dff17(ch4.KOPA_LFSR_CLKp(), GOGE_LFSR_RSTn, ch4.KOMU_LFSR_01.qp_new());
    /*#p20.KUTA*/ ch4.KUTA_LFSR_03.dff17(ch4.KOPA_LFSR_CLKp(), GOGE_LFSR_RSTn, ch4.KETU_LFSR_02.qp_new());
    /*#p20.KUZY*/ ch4.KUZY_LFSR_04.dff17(ch4.KOPA_LFSR_CLKp(), GOGE_LFSR_RSTn, ch4.KUTA_LFSR_03.qp_new());
    /*#p20.KYWY*/ ch4.KYWY_LFSR_05.dff17(ch4.KOPA_LFSR_CLKp(), GOGE_LFSR_RSTn, ch4.KUZY_LFSR_04.qp_new());
    /*#p20.JAJU*/ ch4.JAJU_LFSR_06.dff17(ch4.GYVE_LFSR_CLKp(), GOGE_LFSR_RSTn, ch4.KYWY_LFSR_05.qp_new());
    /*#p20.HAPE*/ ch4.HAPE_LFSR_07.dff17(ch4.GYVE_LFSR_CLKp(), GOGE_LFSR_RSTn, ch4.JAJU_LFSR_06.qp_new());
    /*#p20.JUXE*/ ch4.JUXE_LFSR_08.dff17(ch4.GYVE_LFSR_CLKp(), GOGE_LFSR_RSTn, ch4.HAPE_LFSR_07.qp_new());

    /*#p20.KAVU*/ wire KAVU_LFSR_FB = amux2(ch4.JOTO_LFSR_00.qp_old(), ch4.JAMY_NR43_MODE.qn_old(), ch4.JAMY_NR43_MODE.qp_old(), ch4.JUXE_LFSR_08.qp_old());
    /*#p20.JEPE*/ ch4.JEPE_LFSR_09.dff17(ch4.GYVE_LFSR_CLKp(), GOGE_LFSR_RSTn, KAVU_LFSR_FB);
    /*#p20.JAVO*/ ch4.JAVO_LFSR_10.dff17(ch4.GYVE_LFSR_CLKp(), GOGE_LFSR_RSTn, ch4.JEPE_LFSR_09.qp_new());
    /*#p20.HEPA*/ ch4.HEPA_LFSR_11.dff17(ch4.FEME_LFSR_CLKp(), GOGE_LFSR_RSTn, ch4.JAVO_LFSR_10.qp_new());
    /*#p20.HORY*/ ch4.HORY_LFSR_12.dff17(ch4.FEME_LFSR_CLKp(), GOGE_LFSR_RSTn, ch4.HEPA_LFSR_11.qp_new());
    /*#p20.HENO*/ ch4.HENO_LFSR_13.dff17(ch4.FEME_LFSR_CLKp(), GOGE_LFSR_RSTn, ch4.HORY_LFSR_12.qp_new());
    /*#p20.HYRO*/ ch4.HYRO_LFSR_14.dff17(ch4.FEME_LFSR_CLKp(), GOGE_LFSR_RSTn, ch4.HENO_LFSR_13.qp_new());
    /*_p20.HEZU*/ ch4.HEZU_LFSR_15.dff17(ch4.FEME_LFSR_CLKp(), GOGE_LFSR_RSTn, ch4.HYRO_LFSR_14.qp_new());
  }

  //----------
  // Env

  {
    /*#p20.ABEL*/ ch4.ABEL_CLK_64.dff17(ALOP_CLK_128(), BOKY_APU_RSTn(), ch4.ABEL_CLK_64.qn_old());
    /*#p20.BAWA*/ wire BAWA_CLK_64  = not1(ch4.ABEL_CLK_64.qp_new());
    /*#p20.BUXO*/ wire BUXO_CLK_64  = not1(BAWA_CLK_64);

    /*#p20.ENEC*/ wire ENEC_ENV_TIMER_LOADn = nor2(ch4.GONE_CH4_TRIGp.qp_new(), ch4.FOSY_ENV_CLKp.qp_new());
    /*#p20.DAPY*/ wire DAPY_ENV_TIMER_LOADp  = not1(ENEC_ENV_TIMER_LOADn);

    /*#p20.CUNA*/ ch4.CUNA_ENV_DELAY0n.dff20(BUXO_CLK_64,                   DAPY_ENV_TIMER_LOADp, ch4.EMOK_NR42_ENV_TIMER0.qp_old());
    /*#p20.COFE*/ ch4.COFE_ENV_DELAY1n.dff20(ch4.CUNA_ENV_DELAY0n.qp_new(), DAPY_ENV_TIMER_LOADp, ch4.ETYJ_NR42_ENV_TIMER1.qp_old());
    /*#p20.DOGO*/ ch4.DOGO_ENV_DELAY2n.dff20(ch4.COFE_ENV_DELAY1n.qp_new(), DAPY_ENV_TIMER_LOADp, ch4.EZYK_NR42_ENV_TIMER2.qp_old());
  }

  {
    // Generates a 1 usec pulse when the env timer hits 111
    /*#p20.GEXE*/ wire GEXE_ENV_PULSEn  = not1(ch4.FOSY_ENV_CLKp.qp_new());
    /*#p20.HURY*/ wire HURY_ENV_PULSE_RSTp = nor2(HORU_CLK_512p(), GEXE_ENV_PULSEn);
    /*#p20.GOPA*/ wire GOPA_ENV_PULSE_RSTn = nor4(HURY_ENV_PULSE_RSTp, ch4.FOWA_ENV_OFFp(), ch4.GONE_CH4_TRIGp.qp_new(), KEBA_APU_RSTp());
    /*#p20.EJEX*/ wire EJEX_ENV_TIMER_MAX = or3(ch4.DOGO_ENV_DELAY2n.qp_new(), ch4.COFE_ENV_DELAY1n.qp_new(), ch4.CUNA_ENV_DELAY0n.qp_new());
    /*#p20.FOSY*/ ch4.FOSY_ENV_CLKp.dff17(HORU_CLK_512p(), GOPA_ENV_PULSE_RSTn, EJEX_ENV_TIMER_MAX);
  }

  {
    /*#p20.EMET*/ wire EMET_ENV_STOP_RSTn = nor2(ch4.GONE_CH4_TRIGp.qp_new(), KEBA_APU_RSTp());
    /*#p20.DARO*/ wire DARO_ENV_BOT  = nor5 (ch4.GEKY_NR42_ENV_DIR.qn_new(), ch4.FEKO_CH4_VOL0.qp_new(), ch4.FATY_CH4_VOL1.qp_new(), ch4.FERU_CH4_VOL2.qp_new(), ch4.FYRO_CH4_VOL3.qp_new());
    /*#p20.CUTY*/ wire CUTY_ENV_TOPn = nand5(ch4.GEKY_NR42_ENV_DIR.qn_new(), ch4.FEKO_CH4_VOL0.qp_new(), ch4.FATY_CH4_VOL1.qp_new(), ch4.FERU_CH4_VOL2.qp_new(), ch4.FYRO_CH4_VOL3.qp_new());
    /*#p20.DUBO*/ wire DUBO_ENV_TOPa = not1(CUTY_ENV_TOPn);
    /*#p20.EVUR*/ wire EVUR_ENV_MAXp  = or2(DARO_ENV_BOT, DUBO_ENV_TOPa);
    /*#p20.FYNO*/ ch4.FYNO_ENV_MAXp.dff17(ch4.FOSY_ENV_CLKp.qp_new(), EMET_ENV_STOP_RSTn, EVUR_ENV_MAXp);
  }





  {
    /*#p20.ENUR*/ wire ENUR_ENV_STARTp = or2(KEBA_APU_RSTp(), ch4.GONE_CH4_TRIGp.qp_new());
    /*#p20.EROX*/ ch4.EROX_ENV_RUNNINGn.nor_latch(ch4.FYNO_ENV_MAXp.qp_new(), ENUR_ENV_STARTp);

    /*#p20.FELO*/ wire FELO_ENV_CLK  = or3(ch4.FOSY_ENV_CLKp.qp_new(), ch4.FOWA_ENV_OFFp(), ch4.EROX_ENV_RUNNINGn.qp_new());
    /*#p20.FOLE*/ wire FOLE_VOL_CLK0n = amux2(ch4.GEKY_NR42_ENV_DIR.qn_new(), FELO_ENV_CLK,               FELO_ENV_CLK,               ch4.GEKY_NR42_ENV_DIR.qp_new());
    /*#p20.ETEF*/ wire ETEF_VOL_CLK1n = amux2(ch4.GEKY_NR42_ENV_DIR.qn_new(), ch4.FEKO_CH4_VOL0.qp_new(), ch4.FEKO_CH4_VOL0.qn_new(), ch4.GEKY_NR42_ENV_DIR.qp_new());
    /*#p20.EDYF*/ wire EDYF_VOL_CLK2n = amux2(ch4.GEKY_NR42_ENV_DIR.qn_new(), ch4.FATY_CH4_VOL1.qp_new(), ch4.FATY_CH4_VOL1.qn_new(), ch4.GEKY_NR42_ENV_DIR.qp_new());
    /*#p20.ELAF*/ wire ELAF_VOL_CLK3n = amux2(ch4.GEKY_NR42_ENV_DIR.qn_new(), ch4.FERU_CH4_VOL2.qp_new(), ch4.FERU_CH4_VOL2.qn_new(), ch4.GEKY_NR42_ENV_DIR.qp_new());

    /*#p20.FEKO*/ ch4.FEKO_CH4_VOL0.dff20(FOLE_VOL_CLK0n, ch4.GONE_CH4_TRIGp.qp_new(), ch4.GARU_NR42_ENV0.qn_old());
    /*#p20.FATY*/ ch4.FATY_CH4_VOL1.dff20(ETEF_VOL_CLK1n, ch4.GONE_CH4_TRIGp.qp_new(), ch4.GOKY_NR42_ENV1.qn_old());
    /*#p20.FERU*/ ch4.FERU_CH4_VOL2.dff20(EDYF_VOL_CLK2n, ch4.GONE_CH4_TRIGp.qp_new(), ch4.GOZO_NR42_ENV2.qn_old());
    /*#p20.FYRO*/ ch4.FYRO_CH4_VOL3.dff20(ELAF_VOL_CLK3n, ch4.GONE_CH4_TRIGp.qp_new(), ch4.GEDU_NR42_ENV3.qn_old());
  }

  {
    //----------
    // Output

    /*#p20.DYRY*/ wire DYRY_DBG = and2(ch4.CUNY_NR44_LEN_ENp.qn_new(), EDEK_NR52_DBG_APUp());
    /*#p20.GAME*/ wire GAME_LFSR_OUT = and2(ch4.GENA_CH4_ACTIVEp.qp_new(), ch4.HEZU_LFSR_15.qp_new());
    /*#p20.EZUL*/ wire EZUL_CH4_BIT_MUX = mux2p(DYRY_DBG, ch4.FEME_LFSR_CLKp(), GAME_LFSR_OUT);
    /*#p20.CEPY*/ wire CEPY_NR44_STOPn = not1(ch4.CUNY_NR44_LEN_ENp.qn_new());
    /*#p20.COTE*/ wire COTE_DBG_CH4_MUTE = and2(CEPY_NR44_STOPn, EDEK_NR52_DBG_APUp());
    /*#p20.DATO*/ wire DATO_CH4_RAW_BIT  = or2(EZUL_CH4_BIT_MUX, COTE_DBG_CH4_MUTE);

    /*#p20.AKOF*/ wire AKOF_CH4_DAC0 = and2(ch4.FEKO_CH4_VOL0.qp_new(), DATO_CH4_RAW_BIT);
    /*#p20.BYZY*/ wire BYZY_CH4_DAC1 = and2(ch4.FATY_CH4_VOL1.qp_new(), DATO_CH4_RAW_BIT);
    /*#p20.APYR*/ wire APYR_CH4_DAC2 = and2(ch4.FERU_CH4_VOL2.qp_new(), DATO_CH4_RAW_BIT);
    /*#p20.BOZA*/ wire BOZA_CH4_DAC3 = and2(ch4.FYRO_CH4_VOL3.qp_new(), DATO_CH4_RAW_BIT);
  }
}


//-----------------------------------------------------------------------------

void GBSound::tick_nr41(const GateBoyState& reg_old, GateBoyState& reg_new) {
  /*#p19.CAZE*/ wire CAZE_FF20_WRn = nand2(BOGY_CPU_WRp(), reg_new.cpu_abus.DANU_ADDR_FF20p());
  /*#p19.FURU*/ wire FURU_FF20_WRa = not1(CAZE_FF20_WRn);
  /*#p19.DOTU*/ wire DOTU_FF20_WRb = not1(CAZE_FF20_WRn);
  /*#p19.EPEK*/ wire EPEK_FF20_WRc = not1(CAZE_FF20_WRn);

  /*#p19.GAPY*/ wire GAPY_LEN_RSTn = nor3(FURU_FF20_WRa, KEBA_APU_RSTp(), ch4.GONE_CH4_TRIGp.qp_new());

  /*#p19.DODA*/ wire DODA_LEN_CLKn = nor3(ch4.FUGO_CH4_LEN_DONEp.qp_new(), BUFY_CLK_256n(), ch4.CUNY_NR44_LEN_ENp.qp_new());
  /*#p19.CUWA*/ wire CUWA_LEN_CLKa = not1(DODA_LEN_CLKn);

  /*#p19.DANO*/ ch4.DANO_NR41_LEN0.dff20(CUWA_LEN_CLKa,   DOTU_FF20_WRb, reg_old.cpu_dbus.BUS_CPU_D00p.qp_old());
  /*#p19.FAVY*/ ch4.FAVY_NR41_LEN1.dff20(ch4.DANO_NR41_LEN0.qp_new(),   DOTU_FF20_WRb, reg_old.cpu_dbus.BUS_CPU_D01p.qp_old());
  /*#p19.DENA*/ ch4.DENA_NR41_LEN2.dff20(ch4.FAVY_NR41_LEN1.qp_new(),   DOTU_FF20_WRb, reg_old.cpu_dbus.BUS_CPU_D02p.qp_old());
  /*#p19.CEDO*/ ch4.CEDO_NR41_LEN3.dff20(ch4.DENA_NR41_LEN2.qp_new(),   DOTU_FF20_WRb, reg_old.cpu_dbus.BUS_CPU_D03p.qp_old());

  /*#p19.DOPU*/ wire DOPU_NR41_LEN3p = not1(ch4.CEDO_NR41_LEN3.qn_new());
  /*#p19.FYLO*/ ch4.FYLO_NR41_LEN4.dff20(DOPU_NR41_LEN3p, EPEK_FF20_WRc, reg_old.cpu_dbus.BUS_CPU_D04p.qp_old());
  /*#p19.EDOP*/ ch4.EDOP_NR41_LEN5.dff20(ch4.FYLO_NR41_LEN4.qp_new(),   EPEK_FF20_WRc, reg_old.cpu_dbus.BUS_CPU_D05p.qp_old());

  /*#p19.FUGO*/ ch4.FUGO_CH4_LEN_DONEp.dff17(ch4.EDOP_NR41_LEN5.qn_new(), GAPY_LEN_RSTn, ch4.FUGO_CH4_LEN_DONEp.qn_old());
}

//-----------------------------------------------------------------------------

void GBSound::tick_nr42(const GateBoyState& reg_old, GateBoyState& reg_new) {
  /*#p19.DACO*/ wire DACO_FF21_WRp = and2(BOGY_CPU_WRp(), reg_new.cpu_abus.COVO_ADDR_FF21p());
  /*#p19.GOKO*/ wire GOKO_FF21_WRp = and2(reg_new.cpu_abus.COVO_ADDR_FF21p(), BOGY_CPU_WRp());
  /*#p19.DYKE*/ wire DYKE_FF21_WRn = not1(DACO_FF21_WRp);
  /*#p19.FUPA*/ wire FUPA_FF21_WRn = not1(GOKO_FF21_WRp);
  
  /*_p19.EMOK*/ ch4.EMOK_NR42_ENV_TIMER0.dff9(DYKE_FF21_WRn, FEXO_APU_RSTn(), reg_old.cpu_dbus.BUS_CPU_D00p.qp_old());
  /*_p19.ETYJ*/ ch4.ETYJ_NR42_ENV_TIMER1.dff9(DYKE_FF21_WRn, FEXO_APU_RSTn(), reg_old.cpu_dbus.BUS_CPU_D01p.qp_old());
  /*_p19.EZYK*/ ch4.EZYK_NR42_ENV_TIMER2.dff9(DYKE_FF21_WRn, FEXO_APU_RSTn(), reg_old.cpu_dbus.BUS_CPU_D02p.qp_old());
  /*_p19.GEKY*/ ch4.GEKY_NR42_ENV_DIR   .dff9(FUPA_FF21_WRn, FEXO_APU_RSTn(), reg_old.cpu_dbus.BUS_CPU_D03p.qp_old());
  /*_p19.GARU*/ ch4.GARU_NR42_ENV0      .dff9(FUPA_FF21_WRn, FEXO_APU_RSTn(), reg_old.cpu_dbus.BUS_CPU_D04p.qp_old());
  /*_p19.GOKY*/ ch4.GOKY_NR42_ENV1      .dff9(FUPA_FF21_WRn, FEXO_APU_RSTn(), reg_old.cpu_dbus.BUS_CPU_D05p.qp_old());
  /*_p19.GOZO*/ ch4.GOZO_NR42_ENV2      .dff9(FUPA_FF21_WRn, FEXO_APU_RSTn(), reg_old.cpu_dbus.BUS_CPU_D06p.qp_old());
  /*_p19.GEDU*/ ch4.GEDU_NR42_ENV3      .dff9(FUPA_FF21_WRn, FEXO_APU_RSTn(), reg_old.cpu_dbus.BUS_CPU_D07p.qp_old());

  /*#p19.BOFY*/ wire BOFY_FF21n    = not1(reg_new.cpu_abus.COVO_ADDR_FF21p());
  /*#p19.GONY*/ wire GONY_FF21n    = not1(reg_new.cpu_abus.COVO_ADDR_FF21p());
  /*#p19.BOXE*/ wire BOXE_FF21_RDn = or2(BOFY_FF21n, AGUZ_CPU_RDn());
  /*#p19.HASU*/ wire HASU_FF21_RDn = or2(GONY_FF21n, AGUZ_CPU_RDn());

  /*#p19.DEMY*/ triwire DEMY = tri6_nn(BOXE_FF21_RDn, ch4.EMOK_NR42_ENV_TIMER0.qp_new());
  /*#p19.COCE*/ triwire COCE = tri6_nn(BOXE_FF21_RDn, ch4.ETYJ_NR42_ENV_TIMER1.qp_new());
  /*#p19.CUZU*/ triwire CUZU = tri6_nn(BOXE_FF21_RDn, ch4.EZYK_NR42_ENV_TIMER2.qp_new());
  /*#p19.GOME*/ triwire GOME = tri6_nn(HASU_FF21_RDn, ch4.GEKY_NR42_ENV_DIR.qp_new());
  /*#p19.HEDA*/ triwire HEDA = tri6_nn(HASU_FF21_RDn, ch4.GARU_NR42_ENV0.qp_new());
  /*#p19.GODU*/ triwire GODU = tri6_nn(HASU_FF21_RDn, ch4.GOKY_NR42_ENV1.qp_new());
  /*#p19.HOGE*/ triwire HOGE = tri6_nn(HASU_FF21_RDn, ch4.GOZO_NR42_ENV2.qp_new());
  /*#p19.HACU*/ triwire HACU = tri6_nn(HASU_FF21_RDn, ch4.GEDU_NR42_ENV3.qp_new());
}

//-----------------------------------------------------------------------------

void GBSound::tick_nr43(const GateBoyState& reg_old, GateBoyState& reg_new) {
  /*#p19.HUMO*/ wire HUMO_FF22_WRp =  and2(BOGY_CPU_WRp(), reg_new.cpu_abus.EKEZ_ADDR_FF22p());
  /*#p19.GETU*/ wire GETU_FF22_WRp =  and2(BOGY_CPU_WRp(), reg_new.cpu_abus.EKEZ_ADDR_FF22p());
  /*#p19.HOVA*/ wire HOVA_FF22_WRn = not1(HUMO_FF22_WRp);
  /*#p19.HOSO*/ wire HOSO_FF22_WRn = nand2(reg_new.cpu_abus.EKEZ_ADDR_FF22p(), BOGY_CPU_WRp());
  /*#p19.EFUG*/ wire EFUG_FF22_WRn = not1(GETU_FF22_WRp);

  /*#p19.JARE*/ ch4.JARE_NR43_DIV0 .dff9(HOVA_FF22_WRn, KAME_APU_RSTn(), reg_old.cpu_dbus.BUS_CPU_D00p.qp_new());
  /*#p19.JERO*/ ch4.JERO_NR43_DIV1 .dff9(HOVA_FF22_WRn, KAME_APU_RSTn(), reg_old.cpu_dbus.BUS_CPU_D01p.qp_new());
  /*#p19.JAKY*/ ch4.JAKY_NR43_DIV2 .dff9(HOVA_FF22_WRn, KAME_APU_RSTn(), reg_old.cpu_dbus.BUS_CPU_D02p.qp_new());
  /*#p19.JAMY*/ ch4.JAMY_NR43_MODE .dff9(HOSO_FF22_WRn, HYNE_APU_RSTn(), reg_old.cpu_dbus.BUS_CPU_D03p.qp_new());
  /*#p19.FETA*/ ch4.FETA_NR43_FREQ0.dff9(EFUG_FF22_WRn, DAPA_APU_RSTn(), reg_old.cpu_dbus.BUS_CPU_D04p.qp_new());
  /*#p19.FYTO*/ ch4.FYTO_NR43_FREQ1.dff9(EFUG_FF22_WRn, DAPA_APU_RSTn(), reg_old.cpu_dbus.BUS_CPU_D05p.qp_new());
  /*_p19.GOGO*/ ch4.GOGO_NR43_FREQ2.dff9(EFUG_FF22_WRn, DAPA_APU_RSTn(), reg_old.cpu_dbus.BUS_CPU_D06p.qp_new());
  /*_p19.GAFO*/ ch4.GAFO_NR43_FREQ3.dff9(EFUG_FF22_WRn, DAPA_APU_RSTn(), reg_old.cpu_dbus.BUS_CPU_D07p.qp_new());

  /*#p19.KOKU*/ wire KOKU_ADDR_FF22n = not1(reg_new.cpu_abus.EKEZ_ADDR_FF22p());
  /*#p19.GUGO*/ wire GUGO_ADDR_FF22n = not1(reg_new.cpu_abus.EKEZ_ADDR_FF22p());
  /*#p19.KEKA*/ wire KEKA_FF22_RDn =   or2(KOKU_ADDR_FF22n, AGUZ_CPU_RDn());
  /*#p19.JORA*/ wire JORA_FF22_RDn = nand2(KAGE_CPU_RDp(), reg_new.cpu_abus.EKEZ_ADDR_FF22p());
  /*#p19.HEZE*/ wire HEZE_FF22_RDn =   or2(GUGO_ADDR_FF22n, AGUZ_CPU_RDn());

  /*#p19.KAMO*/ triwire KAMO = tri6_nn(KEKA_FF22_RDn, ch4.JARE_NR43_DIV0.qp_new());
  /*#p19.KAKU*/ triwire KAKU = tri6_nn(KEKA_FF22_RDn, ch4.JERO_NR43_DIV1.qp_new());
  /*_p19.KYRO*/ triwire KYRO = tri6_nn(KEKA_FF22_RDn, ch4.JAKY_NR43_DIV2.qp_new());
  /*_p19.KETA*/ triwire KETA = tri6_nn(JORA_FF22_RDn, ch4.JAMY_NR43_MODE.qp_new());
  /*_p19.GEDA*/ triwire GEDA = tri6_nn(HEZE_FF22_RDn, ch4.FETA_NR43_FREQ0.qp_new());
  /*_p19.GYPE*/ triwire GYPE = tri6_nn(HEZE_FF22_RDn, ch4.FYTO_NR43_FREQ1.qp_new());
  /*_p19.GAKA*/ triwire GAKA = tri6_nn(HEZE_FF22_RDn, ch4.GOGO_NR43_FREQ2.qp_new());
  /*_p19.HAPY*/ triwire HAPY = tri6_nn(HEZE_FF22_RDn, ch4.GAFO_NR43_FREQ3.qp_new());
}

//-----------------------------------------------------------------------------
// Some weird debug voodoo here.

void GBSound::tick_nr44(const GateBoyState& reg_old, GateBoyState& reg_new) {
  /*#p20.DYRY*/ wire DYRY_DBG = and2(ch4.CUNY_NR44_LEN_ENp.qn_new(), EDEK_NR52_DBG_APUp());

  /*#p19.BARE*/ wire BARE_FF23_RDn = nand2(reg_new.cpu_abus.CUGE_ADDR_FF23p(), BYLO_CPU_RDp());
  /*#p19.DULU*/ wire DULU_FF23_WRn = nand2(ANUJ_CPU_WR_WEIRD(), reg_new.cpu_abus.CUGE_ADDR_FF23p());
  /*#p19.CUNY*/ ch4.CUNY_NR44_LEN_ENp.dff9(DULU_FF23_WRn, CABE_APU_RSTn(), reg_old.cpu_dbus.BUS_CPU_D06p.qp_new());

  /*#p19.FOXE*/ wire FOXE_FF23_WRn = nand2(BOGY_CPU_WRp(), reg_new.cpu_abus.CUGE_ADDR_FF23p());
  /*#p20.GUZY*/ wire GUZY_NR44_TRIG_RST = nor2(KEBA_APU_RSTp(), ch4.GYSU_CH4_TRIG.qp_new());
  /*#p19.HOGA*/ ch4.HOGA_NR44_TRIG.dff9(FOXE_FF23_WRn, GUZY_NR44_TRIG_RST, reg_old.cpu_dbus.BUS_CPU_D07p.qp_old());

  /*#p19.CURY*/ triwire CURY = tri6_nn(BARE_FF23_RDn, ch4.CUNY_NR44_LEN_ENp.qp_new());

#if 0
  /*_p20.COMO*/ wire COMO_DBG = and2(DYRY_DBG, COSA_CPU_RDp());
  /*_p20.BAGU*/ wire BAGU_DBG = nand2(CUGE_ADDR_FF23p(), COMO_DBG);
  /*_p20.BEFA*/ wire BEFA_DBG = not1(CARY_FREQ_CLK);
  /*_p20.ATEL*/ triwire ATEL = tri6_nn(BAGU_DBG, BEFA_DBG); // goes to data bus D0
#endif
}








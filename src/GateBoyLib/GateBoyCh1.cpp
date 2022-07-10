#include "GateBoySPU.h"

#include "GateBoyLib/GateBoy.h"

#ifdef SIM_AUDIO

void SpuChannel1::reset_to_cart() {
  BANY_NR10_SWEEP_SHIFT0p.state = 0x1a;
  ARAX_NR10_SWEEP_SHIFT1p.state = 0x1a;
  ANAZ_NR10_SWEEP_SHIFT2p.state = 0x1a;
  AVAF_NR10_SWEEP_DIRp.state = 0x1a;
  ADEK_NR10_SWEEP_PERIOD0p.state = 0x1a;
  BANA_NR10_SWEEP_PERIOD1p.state = 0x1a;
  BOTU_NR10_SWEEP_PERIOD2p.state = 0x1a;
  CENA_NR11_DUTY0p.state = 0x1a;
  DYCA_NR11_DUTY1p.state = 0x1b;
  BACY_NR11_LEN0.state = 0x1a;
  CAVY_NR11_LEN1.state = 0x18;
  BOVY_NR11_LEN2.state = 0x18;
  CUNO_NR11_LEN3.state = 0x18;
  CURA_NR11_LEN4.state = 0x18;
  ERAM_NR11_LEN5.state = 0x18;
  JUSA_NR12_PERIOD0p.state = 0x1b;
  JUZY_NR12_PERIOD1p.state = 0x1b;
  JOMA_NR12_PERIOD2p.state = 0x1a;
  JAFY_NR12_ENV_DIRp.state = 0x1a;
  JATY_NR12_VOL0p.state = 0x1b;
  JAXO_NR12_VOL1p.state = 0x1b;
  JENA_NR12_VOL2p.state = 0x1b;
  JOPU_NR12_VOL3p.state = 0x1b;
  BOKO_NR14_LENENp.state = 0x1a;
  DUPE_NR14_TRIGp.state = 0x1a;
  CYTO_CH1_ACTIVEp.state = 0x19;
  EZEC_CH1_TRIGp.state = 0x18;
  HYKA_CH1_FREQ00.state = 0x19;
  JYKA_CH1_FREQ01.state = 0x18;
  HAVO_CH1_FREQ02.state = 0x18;
  EDUL_CH1_FREQ03.state = 0x18;
  FELY_CH1_FREQ04.state = 0x18;
  HOLU_CH1_FREQ05.state = 0x18;
  HYXU_CH1_FREQ06.state = 0x19;
  HOPO_CH1_FREQ07.state = 0x19;
  DYGY_CH1_FREQ08.state = 0x19;
  EVAB_CH1_FREQ09.state = 0x19;
  AXAN_CH1_FREQ10.state = 0x19;
  CALO_xBCDExxx.state = 0x18;
  FEKU_CH1_TRIGp.state = 0x18;
  FARE_CH1_TRIGp.state = 0x18;
  FYTE_CH1_TRIGp.state = 0x18;
  BYTE_SHIFT_DONEp.state = 0x1b;
  CERO_CH1_LEN_DONE.state = 0x1a;
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
  COPU_CH1_FREQ_CNT_10.state =   0x1b;
  BAZA_DBG_SWEEP_CLK.state = 0x18;
  BEXA_SWEEP_TRIGGERp.state = 0x1a;
  CUPO_SWEEP_DELAY0p.state = 0x1b;
  CYPU_SWEEP_DELAY1p.state = 0x1a;
  CAXY_SWEEP_DELAY2p.state = 0x18;
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
  KOZY_ENV_TICKp.state = 0x1a;
  KYNO_ENV_MAXp.state = 0x19;
  JOVA_ENV_DELAY0p.state = 0x1a;
  KENU_ENV_DELAY1p.state = 0x19;
  KERA_ENV_DELAY2p.state =   0x1b;
  
  HEVO_CH1_ENV3p.state = 0x19;
  HOKO_CH1_ENV2p.state = 0x19;
  HEMY_CH1_ENV1p.state = 0x19;
  HAFO_CH1_ENV0p.state = 0x19;
  
  KALY_ENV_DELAY_CLK_64n.state = 0x19;
  DUWO_RAW_BIT_SYNCp.state = 0x18;
  KEZU_ENV_ACTIVEn.state = 0x19;
}

void tick_ch1(const GateBoyState& reg_old, GateBoyState& reg_new) {

  //----------
  // Trigger

  {
    /*#p13.EZEC*/ reg_new.ch1.EZEC_CH1_TRIGp.dff17(reg_new.sys_clk.DOVA_ABCDxxxx(), reg_new.DUKA_APU_RSTn_new(), reg_old.ch1.DUPE_NR14_TRIGp.qp_oldB());

    /*#p13.DOGE*/ wire DOGE_FF14_WRn = nand2(reg_new.cpu_signals.BOGY_CPU_WRp(), reg_new.cpu_abus.DUJA_ADDR_FF14p());
    /*#p13.DADO*/ wire DADO_NR14_RSTb = nor2(reg_new.KEBA_APU_RSTp_new(), reg_new.ch1.EZEC_CH1_TRIGp.qp_new());
    /*#p13.DUPE*/ reg_new.ch1.DUPE_NR14_TRIGp.dff9b(DOGE_FF14_WRn, DADO_NR14_RSTb, reg_old.cpu_dbus.BUS_CPU_D07p.qp_old());

    /*#p13.FYTE*/ reg_new.ch1.FYTE_CH1_TRIGp.dff17(reg_new.ch1.DYFA_xBCDExxx(), reg_new.ERUM_APU_RSTn_new(), reg_old.ch1.FARE_CH1_TRIGp.qp_old());
    /*#p13.FARE*/ reg_new.ch1.FARE_CH1_TRIGp.dff17(reg_new.ch1.DYFA_xBCDExxx(), reg_new.ERUM_APU_RSTn_new(), reg_old.ch1.FEKU_CH1_TRIGp.qp_old());

    /*#p13.EGET*/ wire EGET_TRIG_RSTn = nor2(reg_new.KEBA_APU_RSTp_new(), reg_new.ch1.FARE_CH1_TRIGp.qp_new());
    /*#p13.FEKU*/ reg_new.ch1.FEKU_CH1_TRIGp.dff17(reg_new.ch1.DYFA_xBCDExxx(), EGET_TRIG_RSTn, reg_old.ch1.FYFO_CH1_TRIGn.qn_old()); // schematic wrong?

    /*#p13.GEFE*/ wire GEFE_TRIG_RSTp = not1(EGET_TRIG_RSTn);
    /*#p13.FYFO*/ reg_new.ch1.FYFO_CH1_TRIGn.nor_latch(GEFE_TRIG_RSTp, reg_new.ch1.EZEC_CH1_TRIGp.qp_new());
  }


#if 0
  /*#p15.CEMO*/ DFF17 CEMO_CLK_1M;
  /*_p15.ATEP*/ DFF17 ATEP_AxxDExxH;

  //----------
  // clocks for the apu

  /*_p01.CERY*/ DFF17 CERY_CLK_2M;
  /*_p01.ATYK*/ DFF17 ATYK_CLK_2M;
  /*_p01.AVOK*/ DFF17 AVOK_xBCDExxx;
  /*_p09.AJER*/ DFF17 AJER_AxxDExxH;
  /*_p01.JESO*/ DFF17 JESO_CLK_512K;

  /*_p01.BARA*/ DFF17 BARA_CLK_512;
  /*_p01.CARU*/ DFF17 CARU_CLK_256;
  /*_p01.BYLU*/ DFF17 BYLU_CLK_128;
#endif

  //----------
  // Sweep timer

  {
    /*#p11.CENU*/ wire CENU_NR10_WRp = and2(reg_new.cpu_signals.BOGY_CPU_WRp(), reg_new.cpu_abus.DYVA_ADDR_FF10p());
    /*#p11.CENU*/ wire CENU_NR10_WRn = not1(CENU_NR10_WRp);
    /*#p11.BANY*/ reg_new.ch1.BANY_NR10_SWEEP_SHIFT0p .dff9b(CENU_NR10_WRn, reg_new.AGUR_APU_RSTn_new(), reg_old.cpu_dbus.BUS_CPU_D00p.qp_old());
    /*#p11.ARAX*/ reg_new.ch1.ARAX_NR10_SWEEP_SHIFT1p .dff9b(CENU_NR10_WRn, reg_new.AGUR_APU_RSTn_new(), reg_old.cpu_dbus.BUS_CPU_D01p.qp_old());
    /*#p11.ANAZ*/ reg_new.ch1.ANAZ_NR10_SWEEP_SHIFT2p .dff9b(CENU_NR10_WRn, reg_new.AGUR_APU_RSTn_new(), reg_old.cpu_dbus.BUS_CPU_D02p.qp_old());
    /*#p11.AVAF*/ reg_new.ch1.AVAF_NR10_SWEEP_DIRp    .dff9b(CENU_NR10_WRn, reg_new.AGUR_APU_RSTn_new(), reg_old.cpu_dbus.BUS_CPU_D03p.qp_old());
    /*#p11.ADEK*/ reg_new.ch1.ADEK_NR10_SWEEP_PERIOD0p.dff9b(CENU_NR10_WRn, reg_new.AGUR_APU_RSTn_new(), reg_old.cpu_dbus.BUS_CPU_D04p.qp_old());
    /*#p11.BANA*/ reg_new.ch1.BANA_NR10_SWEEP_PERIOD1p.dff9b(CENU_NR10_WRn, reg_new.AGUR_APU_RSTn_new(), reg_old.cpu_dbus.BUS_CPU_D05p.qp_old());
    /*#p11.BOTU*/ reg_new.ch1.BOTU_NR10_SWEEP_PERIOD2p.dff9b(CENU_NR10_WRn, reg_new.AGUR_APU_RSTn_new(), reg_old.cpu_dbus.BUS_CPU_D06p.qp_old());
  }

  {
    /*#p09.BAZA*/ reg_new.ch1.BAZA_DBG_SWEEP_CLK.dff17(reg_new.spu.AJER_AxxDExxH.qn_new(), reg_new.ATYV_APU_RSTn_new(), reg_old.spu.BOWY_NR52_DBG_SWEEP.qp_old());
  }


  {
    /*#p13.BAVE*/ wire BAVE_NR10_NO_SWEEPp = and3(reg_new.ch1.BOTU_NR10_SWEEP_PERIOD2p.qn_newB(), reg_new.ch1.BANA_NR10_SWEEP_PERIOD1p.qn_newB(), reg_new.ch1.ADEK_NR10_SWEEP_PERIOD0p.qn_newB());
    /*#p13.BURY*/ wire BURY_SWEEP_TRIGGER_RSTn = nor2(BAVE_NR10_NO_SWEEPp, reg_new.KEBA_APU_RSTp_new());
    /*#p13.COZE*/ wire COZE_SWEEP_MAX_old = and3(reg_old.ch1.CAXY_SWEEP_DELAY2p.qp_old(), reg_old.ch1.CYPU_SWEEP_DELAY1p.qp_old(), reg_old.ch1.CUPO_SWEEP_DELAY0p.qp_old());
    /*#p13.BEXA*/ reg_new.ch1.BEXA_SWEEP_TRIGGERp.dff17(reg_new.spu.AJER_AxxDExxH.qp_new(), BURY_SWEEP_TRIGGER_RSTn, COZE_SWEEP_MAX_old);
  }

  {
    /*#p09.CELY*/ wire CELY_SWEEP_DELAY_CLKn = mux2p(reg_new.EDEK_NR52_DBG_APUp(), reg_new.ch1.BAZA_DBG_SWEEP_CLK.qp_new(), reg_new.spu.BYFE_CLK_128n());
    /*#p09.CONE*/ wire CONE_SWEEP_DELAY_CLKp = not1(CELY_SWEEP_DELAY_CLKn);
    /*#p09.CATE*/ wire CATE_SWEEP_DELAY_CLKn = not1(CONE_SWEEP_DELAY_CLKp);
    /*#p13.DAFA*/ wire DAFA_SWEEP_DELAY_LOADn = nor2(reg_new.ch1.BEXA_SWEEP_TRIGGERp.qp_new(), reg_new.ch1.FEKU_CH1_TRIGp.qp_new());
    /*#p13.CYMU*/ wire CYMU_SWEEP_DELAY_LOADp = not1(DAFA_SWEEP_DELAY_LOADn);
    /*#p13.CUPO*/ reg_new.ch1.CUPO_SWEEP_DELAY0p.dff20(CATE_SWEEP_DELAY_CLKn,                   CYMU_SWEEP_DELAY_LOADp, reg_old.ch1.ADEK_NR10_SWEEP_PERIOD0p.qn_oldB());
    /*#p13.CYPU*/ reg_new.ch1.CYPU_SWEEP_DELAY1p.dff20(reg_new.ch1.CUPO_SWEEP_DELAY0p.qp_new(), CYMU_SWEEP_DELAY_LOADp, reg_old.ch1.BANA_NR10_SWEEP_PERIOD1p.qn_oldB());
    /*#p13.CAXY*/ reg_new.ch1.CAXY_SWEEP_DELAY2p.dff20(reg_new.ch1.CYPU_SWEEP_DELAY1p.qp_new(), CYMU_SWEEP_DELAY_LOADp, reg_old.ch1.BOTU_NR10_SWEEP_PERIOD2p.qn_oldB());
  }

  {
    /*#p13.ATAT*/ wire ATAT_SWEEP_RSTn = nor2(reg_new.KEBA_APU_RSTp_new(), reg_new.ch1.BEXA_SWEEP_TRIGGERp.qp_new());
    /*#p13.COPY*/ wire COPY_SHIFT_DONEp_old = and3(reg_old.ch1.BYRA_SHIFTCNT2.qp_old(), reg_old.ch1.CAJA_SHIFTCNT1.qp_old(), reg_old.ch1.COPA_SHIFTCNT0.qp_old());
    /*#p13.BYTE*/ reg_new.ch1.BYTE_SHIFT_DONEp.dff17(reg_new.spu.AJER_AxxDExxH.qp_new(), ATAT_SWEEP_RSTn, COPY_SHIFT_DONEp_old);
  }

  {
    /*#p13.ADAD*/ wire ADAD_SHIFT_DONEp = not1(reg_new.ch1.BYTE_SHIFT_DONEp.qn_new());
    /*#p13.EPUK*/ wire EPUK_SETn = nor2(reg_new.KEBA_APU_RSTp_new(), ADAD_SHIFT_DONEp);
    /*#p13.EVOL*/ wire EVOL_RSTn = nor2(reg_new.ch1.BEXA_SWEEP_TRIGGERp.qp_new(), reg_new.ch1.FYTE_CH1_TRIGp.qp_new());
    /*#p13.FEMU*/ reg_new.ch1.FEMU_SHIFTINGn.nand_latch(EPUK_SETn, EVOL_RSTn);
  }

  //----------
  // Frequency summer

  {
    /*#p13.DAPU*/ wire DAPU_SHIFT_CLK = not1(reg_new.ch1.EGOR_SHIFT_CLK_new());
    /*#p13.DACU*/ wire DACU_SHIFTCNT_LOADn = nor2(reg_new.ch1.FEKU_CH1_TRIGp.qp_new(), reg_new.ch1.BEXA_SWEEP_TRIGGERp.qp_new());
    /*#p13.CYLU*/ wire CYLU_SHIFTCNT_LOADp = not1(DACU_SHIFTCNT_LOADn);
    /*#p13.COPA*/ reg_new.ch1.COPA_SHIFTCNT0.dff20(DAPU_SHIFT_CLK,                      CYLU_SHIFTCNT_LOADp, reg_old.ch1.BANY_NR10_SWEEP_SHIFT0p.qn_oldB());
    /*#p13.CAJA*/ reg_new.ch1.CAJA_SHIFTCNT1.dff20(reg_new.ch1.COPA_SHIFTCNT0.qp_new(), CYLU_SHIFTCNT_LOADp, reg_old.ch1.ARAX_NR10_SWEEP_SHIFT1p.qn_oldB());
    /*#p13.BYRA*/ reg_new.ch1.BYRA_SHIFTCNT2.dff20(reg_new.ch1.CAJA_SHIFTCNT1.qp_new(), CYLU_SHIFTCNT_LOADp, reg_old.ch1.ANAZ_NR10_SWEEP_SHIFT2p.qn_oldB());
  }


  {
    /*#p13.ADAD*/ wire ADAD_SHIFT_DONEp_new = not1(reg_new.ch1.BYTE_SHIFT_DONEp.qn_new());
    /*#p13.KABA*/ wire KABA_SUM_RSTp_new = or2(reg_new.KEBA_APU_RSTp_new(), reg_new.ch1.FEKU_CH1_TRIGp.qp_new());
    /*#p13.KYNY*/ wire KYNY_SUM_RSTn_new = not1(KABA_SUM_RSTp_new);

    /*_p12.GALO*/ reg_new.ch1.GALO_SUM_A00.dff11(ADAD_SHIFT_DONEp_new, KYNY_SUM_RSTn_new, reg_old.ch1.HYKA_CH1_FREQ00.qp_old());
    /*_p12.JODE*/ reg_new.ch1.JODE_SUM_A01.dff11(ADAD_SHIFT_DONEp_new, KYNY_SUM_RSTn_new, reg_old.ch1.JYKA_CH1_FREQ01.qp_old());
    /*_p12.KARE*/ reg_new.ch1.KARE_SUM_A02.dff11(ADAD_SHIFT_DONEp_new, KYNY_SUM_RSTn_new, reg_old.ch1.HAVO_CH1_FREQ02.qp_old());
    /*_p12.JYME*/ reg_new.ch1.JYME_SUM_A03.dff11(ADAD_SHIFT_DONEp_new, KYNY_SUM_RSTn_new, reg_old.ch1.EDUL_CH1_FREQ03.qp_old());
    /*_p12.GYME*/ reg_new.ch1.GYME_SUM_A04.dff11(ADAD_SHIFT_DONEp_new, KYNY_SUM_RSTn_new, reg_old.ch1.FELY_CH1_FREQ04.qp_old());
    /*_p12.FAXO*/ reg_new.ch1.FAXO_SUM_A05.dff11(ADAD_SHIFT_DONEp_new, KYNY_SUM_RSTn_new, reg_old.ch1.HOLU_CH1_FREQ05.qp_old());
    /*_p12.EXAP*/ reg_new.ch1.EXAP_SUM_A06.dff11(ADAD_SHIFT_DONEp_new, KYNY_SUM_RSTn_new, reg_old.ch1.HYXU_CH1_FREQ06.qp_old());
    /*_p12.DELE*/ reg_new.ch1.DELE_SUM_A07.dff11(ADAD_SHIFT_DONEp_new, KYNY_SUM_RSTn_new, reg_old.ch1.HOPO_CH1_FREQ07.qp_old());
    /*_p12.DEXE*/ reg_new.ch1.DEXE_SUM_A08.dff11(ADAD_SHIFT_DONEp_new, KYNY_SUM_RSTn_new, reg_old.ch1.DYGY_CH1_FREQ08.qp_old());
    /*_p12.DOFY*/ reg_new.ch1.DOFY_SUM_A09.dff11(ADAD_SHIFT_DONEp_new, KYNY_SUM_RSTn_new, reg_old.ch1.EVAB_CH1_FREQ09.qp_old());
    /*_p12.DOLY*/ reg_new.ch1.DOLY_SUM_A10.dff11(ADAD_SHIFT_DONEp_new, KYNY_SUM_RSTn_new, reg_old.ch1.AXAN_CH1_FREQ10.qp_old());

    /*_p12.GYLO*/ wire GYLO_SHIFTER_FLIP_00_old = xor2(reg_old.ch1.ARYL_NR10_SWEEP_DIRn_old(), reg_old.ch1.FABU_CH1_SHIFT00.qp_old());
    /*_p12.GELA*/ wire GELA_SHIFTER_FLIP_01_old = xor2(reg_old.ch1.ARYL_NR10_SWEEP_DIRn_old(), reg_old.ch1.JEFA_CH1_SHIFT01.qp_old());
    /*_p12.GOPO*/ wire GOPO_SHIFTER_FLIP_02_old = xor2(reg_old.ch1.ARYL_NR10_SWEEP_DIRn_old(), reg_old.ch1.GOGA_CH1_SHIFT02.qp_old());
    /*_p12.HEFY*/ wire HEFY_SHIFTER_FLIP_03_old = xor2(reg_old.ch1.ARYL_NR10_SWEEP_DIRn_old(), reg_old.ch1.JOLU_CH1_SHIFT03.qp_old());
    /*_p12.KEFE*/ wire KEFE_SHIFTER_FLIP_04_old = xor2(reg_old.ch1.ARYL_NR10_SWEEP_DIRn_old(), reg_old.ch1.JOTA_CH1_SHIFT04.qp_old());
    /*_p12.GOLY*/ wire GOLY_SHIFTER_FLIP_05_old = xor2(reg_old.ch1.ARYL_NR10_SWEEP_DIRn_old(), reg_old.ch1.FUDE_CH1_SHIFT05.qp_old());
    /*_p12.FURE*/ wire FURE_SHIFTER_FLIP_06_old = xor2(reg_old.ch1.ARYL_NR10_SWEEP_DIRn_old(), reg_old.ch1.FEDO_CH1_SHIFT06.qp_old());
    /*_p12.DYME*/ wire DYME_SHIFTER_FLIP_07_old = xor2(reg_old.ch1.ARYL_NR10_SWEEP_DIRn_old(), reg_old.ch1.EXAC_CH1_SHIFT07.qp_old());
    /*_p12.CALE*/ wire CALE_SHIFTER_FLIP_08_old = xor2(reg_old.ch1.ARYL_NR10_SWEEP_DIRn_old(), reg_old.ch1.ELUX_CH1_SHIFT08.qp_old());
    /*_p12.DOZY*/ wire DOZY_SHIFTER_FLIP_09_old = xor2(reg_old.ch1.ARYL_NR10_SWEEP_DIRn_old(), reg_old.ch1.AGEZ_CH1_SHIFT09.qp_old());
    /*_p12.CULU*/ wire CULU_SHIFTER_FLIP_10_old = xor2(reg_old.ch1.ARYL_NR10_SWEEP_DIRn_old(), reg_old.ch1.BEKU_CH1_SHIFT10.qp_old());

    /*_p12.HORA*/ reg_new.ch1.HORA_SUM_B00.dff11(ADAD_SHIFT_DONEp_new, KYNY_SUM_RSTn_new, GYLO_SHIFTER_FLIP_00_old);
    /*_p12.HOPA*/ reg_new.ch1.HOPA_SUM_B01.dff11(ADAD_SHIFT_DONEp_new, KYNY_SUM_RSTn_new, GELA_SHIFTER_FLIP_01_old);
    /*_p12.HELE*/ reg_new.ch1.HELE_SUM_B02.dff11(ADAD_SHIFT_DONEp_new, KYNY_SUM_RSTn_new, GOPO_SHIFTER_FLIP_02_old);
    /*_p12.JAPE*/ reg_new.ch1.JAPE_SUM_B03.dff11(ADAD_SHIFT_DONEp_new, KYNY_SUM_RSTn_new, HEFY_SHIFTER_FLIP_03_old);
    /*_p12.JETE*/ reg_new.ch1.JETE_SUM_B04.dff11(ADAD_SHIFT_DONEp_new, KYNY_SUM_RSTn_new, KEFE_SHIFTER_FLIP_04_old);
    /*_p12.GELE*/ reg_new.ch1.GELE_SUM_B05.dff11(ADAD_SHIFT_DONEp_new, KYNY_SUM_RSTn_new, GOLY_SHIFTER_FLIP_05_old);
    /*_p12.EPYR*/ reg_new.ch1.EPYR_SUM_B06.dff11(ADAD_SHIFT_DONEp_new, KYNY_SUM_RSTn_new, FURE_SHIFTER_FLIP_06_old);
    /*_p12.EDOK*/ reg_new.ch1.EDOK_SUM_B07.dff11(ADAD_SHIFT_DONEp_new, KYNY_SUM_RSTn_new, DYME_SHIFTER_FLIP_07_old);
    /*_p12.DEFA*/ reg_new.ch1.DEFA_SUM_B08.dff11(ADAD_SHIFT_DONEp_new, KYNY_SUM_RSTn_new, CALE_SHIFTER_FLIP_08_old);
    /*_p12.ETER*/ reg_new.ch1.ETER_SUM_B09.dff11(ADAD_SHIFT_DONEp_new, KYNY_SUM_RSTn_new, DOZY_SHIFTER_FLIP_09_old);
    /*_p12.DEVA*/ reg_new.ch1.DEVA_SUM_B10.dff11(ADAD_SHIFT_DONEp_new, KYNY_SUM_RSTn_new, CULU_SHIFTER_FLIP_10_old);
  }

  /*#p12.GUXA*/ Adder GUXA_SUM00 = add3(reg_new.ch1.GALO_SUM_A00.qp_new(), reg_new.ch1.HORA_SUM_B00.qp_new(), reg_new.ch1.ARYL_NR10_SWEEP_DIRn_new());
  /*_p12.HALU*/ Adder HALU_SUM01 = add3(reg_new.ch1.JODE_SUM_A01.qp_new(), reg_new.ch1.HOPA_SUM_B01.qp_new(), GUXA_SUM00.carry);
  /*_p12.JULE*/ Adder JULE_SUM02 = add3(reg_new.ch1.KARE_SUM_A02.qp_new(), reg_new.ch1.HELE_SUM_B02.qp_new(), HALU_SUM01.carry);
  /*_p12.JORY*/ Adder JORY_SUM03 = add3(reg_new.ch1.JYME_SUM_A03.qp_new(), reg_new.ch1.JAPE_SUM_B03.qp_new(), JULE_SUM02.carry);
  /*_p12.HEXO*/ Adder HEXO_SUM04 = add3(reg_new.ch1.GYME_SUM_A04.qp_new(), reg_new.ch1.JETE_SUM_B04.qp_new(), JORY_SUM03.carry);
  /*_p12.GEVA*/ Adder GEVA_SUM05 = add3(reg_new.ch1.FAXO_SUM_A05.qp_new(), reg_new.ch1.GELE_SUM_B05.qp_new(), HEXO_SUM04.carry);
  /*#p12.FEGO*/ Adder FEGO_SUM06 = add3(reg_new.ch1.EXAP_SUM_A06.qp_new(), reg_new.ch1.EPYR_SUM_B06.qp_new(), GEVA_SUM05.carry);
  /*_p12.ETEK*/ Adder ETEK_SUM07 = add3(reg_new.ch1.DELE_SUM_A07.qp_new(), reg_new.ch1.EDOK_SUM_B07.qp_new(), FEGO_SUM06.carry);
  /*_p12.DYXE*/ Adder DYXE_SUM08 = add3(reg_new.ch1.DEXE_SUM_A08.qp_new(), reg_new.ch1.DEFA_SUM_B08.qp_new(), ETEK_SUM07.carry);
  /*_p12.DULE*/ Adder DULE_SUM09 = add3(reg_new.ch1.DOFY_SUM_A09.qp_new(), reg_new.ch1.ETER_SUM_B09.qp_new(), DYXE_SUM08.carry);
  /*_p12.CORU*/ Adder CORU_SUM10 = add3(reg_new.ch1.DOLY_SUM_A10.qp_new(), reg_new.ch1.DEVA_SUM_B10.qp_new(), DULE_SUM09.carry);

  /*#p12.BYLE*/ wire BYLE_FREQ_OVERFLOWn_new = nor2(reg_new.ch1.ARYL_NR10_SWEEP_DIRn_new(), CORU_SUM10.carry);
  /*#p12.ATYS*/ wire ATYS_FREQ_OVERFLOWn_new = or2(BYLE_FREQ_OVERFLOWn_new, reg_new.ch1.ARYL_NR10_SWEEP_DIRn_new());

  /*#p12.DEPU*/ wire DEPU_FF13_WRn = nand2(reg_new.cpu_signals.BOGY_CPU_WRp(), reg_new.cpu_abus.DECO_ADDR_FF13p());
  /*#p12.DYLA*/ wire DYLA_FF13_WRp = not1(DEPU_FF13_WRn);
  /*#p12.DEBY*/ wire DEBY_FF14_WRp = and2(reg_new.cpu_signals.BOGY_CPU_WRp(), reg_new.cpu_abus.DUJA_ADDR_FF14p());

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

  /*#p12.EFOR*/ wire EFOR_FREQ00_RST = nor2(EJYF, reg_new.KEBA_APU_RSTp_new()); 
  /*#p12.GATO*/ wire GATO_FREQ01_RST = nor2(FOPU, reg_new.KEBA_APU_RSTp_new()); 
  /*#p12.GYFU*/ wire GYFU_FREQ02_RST = nor2(FOKE, reg_new.KEBA_APU_RSTp_new()); 
  /*#p12.GAMO*/ wire GAMO_FREQ03_RST = nor2(FEGA, reg_new.KEBA_APU_RSTp_new()); 
  /*#p12.KAPO*/ wire KAPO_FREQ04_RST = nor2(KAVO, reg_new.KEBA_APU_RSTp_new()); 
  /*#p12.KAJU*/ wire KAJU_FREQ05_RST = nor2(KYFU, reg_new.KEBA_APU_RSTp_new()); 
  /*#p12.ELUF*/ wire ELUF_FREQ06_RST = nor2(ETOK, reg_new.KEBA_APU_RSTp_new()); 
  /*#p12.ESEL*/ wire ESEL_FREQ07_RST = nor2(EMAR, reg_new.KEBA_APU_RSTp_new()); 
  /*#p12.BOXU*/ wire BOXU_FREQ08_RST = nor2(BASO, reg_new.KEBA_APU_RSTp_new()); 
  /*#p12.BOVU*/ wire BOVU_FREQ09_RST = nor2(AMAC, reg_new.KEBA_APU_RSTp_new()); 
  /*#p12.APAJ*/ wire APAJ_FREQ10_RST = nor2(AJUX, reg_new.KEBA_APU_RSTp_new()); 

  /*#p13.ATUV*/ wire ATUV_FREQ_CLK = and2(reg_new.ch1.BEXA_SWEEP_TRIGGERp.qp_new(), ATYS_FREQ_OVERFLOWn_new);
  /*#p13.BOJE*/ wire BOJE_FREQ_CLK = and2(ATUV_FREQ_CLK, reg_new.ch1.BUGE_SWEEP_DONEn_new());
  /*#p13.BUSO*/ wire BUSO_FREQ_CLK = and3(reg_new.ch1.BEXA_SWEEP_TRIGGERp.qp_new(), ATYS_FREQ_OVERFLOWn_new, reg_new.ch1.BUGE_SWEEP_DONEn_new());

  {
    /*_p12.HYKA*/ reg_new.ch1.HYKA_CH1_FREQ00.dff22(BOJE_FREQ_CLK, GYLU_FREQ00_SET, EFOR_FREQ00_RST, GUXA_SUM00.sum);
    /*_p12.JYKA*/ reg_new.ch1.JYKA_CH1_FREQ01.dff22(BOJE_FREQ_CLK, GETA_FREQ01_SET, GATO_FREQ01_RST, HALU_SUM01.sum);
    /*_p12.HAVO*/ reg_new.ch1.HAVO_CH1_FREQ02.dff22(BOJE_FREQ_CLK, GOLO_FREQ02_SET, GYFU_FREQ02_RST, JULE_SUM02.sum);
    /*_p12.EDUL*/ reg_new.ch1.EDUL_CH1_FREQ03.dff22(BOJE_FREQ_CLK, GOPE_FREQ03_SET, GAMO_FREQ03_RST, JORY_SUM03.sum);
    /*_p12.FELY*/ reg_new.ch1.FELY_CH1_FREQ04.dff22(BOJE_FREQ_CLK, KOVU_FREQ04_SET, KAPO_FREQ04_RST, HEXO_SUM04.sum);
    /*_p12.HOLU*/ reg_new.ch1.HOLU_CH1_FREQ05.dff22(BOJE_FREQ_CLK, KYPA_FREQ05_SET, KAJU_FREQ05_RST, GEVA_SUM05.sum);
    /*_p12.HYXU*/ reg_new.ch1.HYXU_CH1_FREQ06.dff22(BOJE_FREQ_CLK, ELER_FREQ06_SET, ELUF_FREQ06_RST, FEGO_SUM06.sum);
    /*_p12.HOPO*/ reg_new.ch1.HOPO_CH1_FREQ07.dff22(BOJE_FREQ_CLK, ETOL_FREQ07_SET, ESEL_FREQ07_RST, ETEK_SUM07.sum);
    /*_p12.DYGY*/ reg_new.ch1.DYGY_CH1_FREQ08.dff22(BUSO_FREQ_CLK, BUGU_FREQ08_SET, BOXU_FREQ08_RST, DYXE_SUM08.sum);
    /*_p12.EVAB*/ reg_new.ch1.EVAB_CH1_FREQ09.dff22(BUSO_FREQ_CLK, BUDO_FREQ09_SET, BOVU_FREQ09_RST, DULE_SUM09.sum);
    /*_p12.AXAN*/ reg_new.ch1.AXAN_CH1_FREQ10.dff22(BUSO_FREQ_CLK, AFEG_FREQ10_SET, APAJ_FREQ10_RST, CORU_SUM10.sum);
  }

  {
    /*#p13.BORO*/ wire BORO_FF11_WRn = nand2(reg_new.cpu_signals.BOGY_CPU_WRp(), reg_new.cpu_abus.CAXE_ADDR_FF11p());
    /*#p13.BEPE*/ wire BEPE_FF11_WRp = not1(BORO_FF11_WRn);
    /*#p13.BUGY*/ wire BUGY_FF11_WRp = not1(BORO_FF11_WRn);
    /*#p13.BOKA*/ wire BOKA_FF11_WRp = not1(BORO_FF11_WRn);


    {
      /*#p11.BAGE*/ wire BAGE_NR14_WRn = nand2(reg_new.ANUJ_CPU_WR_WEIRD(), reg_new.cpu_abus.DUJA_ADDR_FF14p());
      /*#p11.BOKO*/ reg_new.ch1.BOKO_NR14_LENENp.dff9b(BAGE_NR14_WRn, reg_new.CAMY_APU_RSTn_new(), reg_old.cpu_dbus.BUS_CPU_D06p.qp_old());
    }
    
    // loop


    {
      // using CERO_old here to break the loop

      /*#p13.CAPY*/ wire CAPY_CH1_LEN_CLKn = nor3(reg_old.ch1.CERO_CH1_LEN_DONE.qp_old(), reg_new.spu.BUFY_CLK_256n(), reg_new.ch1.BOKO_NR14_LENENp.qn_newB());
      /*#p13.CANU*/ wire CANU_CH1_LEN_CLKp = not1(CAPY_CH1_LEN_CLKn);
      /*#p13.BACY*/ reg_new.ch1.BACY_NR11_LEN0.dff20(CANU_CH1_LEN_CLKp,                     BUGY_FF11_WRp, reg_old.cpu_dbus.BUS_CPU_D00p.qp_old());
      /*#p13.CAVY*/ reg_new.ch1.CAVY_NR11_LEN1.dff20(reg_new.ch1.BACY_NR11_LEN0.qp_new(),   BUGY_FF11_WRp, reg_old.cpu_dbus.BUS_CPU_D01p.qp_old());
      /*#p13.BOVY*/ reg_new.ch1.BOVY_NR11_LEN2.dff20(reg_new.ch1.CAVY_NR11_LEN1.qp_new(),   BUGY_FF11_WRp, reg_old.cpu_dbus.BUS_CPU_D02p.qp_old());
      /*#p13.CUNO*/ reg_new.ch1.CUNO_NR11_LEN3.dff20(reg_new.ch1.BOVY_NR11_LEN2.qp_new(),   BUGY_FF11_WRp, reg_old.cpu_dbus.BUS_CPU_D03p.qp_old());
      /*#p13.CUSO*/ wire CUSO_NR11_LEN3p = not1(reg_new.ch1.CUNO_NR11_LEN3.qn_new());
      /*#p13.CURA*/ reg_new.ch1.CURA_NR11_LEN4.dff20(CUSO_NR11_LEN3p,                       BEPE_FF11_WRp, reg_old.cpu_dbus.BUS_CPU_D04p.qp_old());
      /*#p13.ERAM*/ reg_new.ch1.ERAM_NR11_LEN5.dff20(reg_new.ch1.CURA_NR11_LEN4.qp_new(),   BEPE_FF11_WRp, reg_old.cpu_dbus.BUS_CPU_D05p.qp_old());

      /*#p13.CORY*/ wire CORY_CH1_LEN_DONE_RST_new = nor3(BOKA_FF11_WRp, reg_new.KEBA_APU_RSTp_new(), reg_new.ch1.FEKU_CH1_TRIGp.qp_new());
      /*_p13.CERO*/ reg_new.ch1.CERO_CH1_LEN_DONE.dff17(reg_old.ch1.ERAM_NR11_LEN5.qn_old(), CORY_CH1_LEN_DONE_RST_new, reg_old.ch1.CERO_CH1_LEN_DONE.qn_old());
    }

  }

  {
    /*#p11.HAFU*/ wire HAFU_NR12_WRp = and2(reg_new.cpu_signals.BOGY_CPU_WRp(), reg_new.cpu_abus.EDAF_ADDR_FF12p());
    /*#p11.KYGY*/ wire KYGY_NR12_WRn = not1(HAFU_NR12_WRp);
    /*#p11.GAXU*/ wire GAXU_NR12_WRn = nand2(reg_new.cpu_abus.EDAF_ADDR_FF12p(), reg_new.cpu_signals.BOGY_CPU_WRp());

    // another weird dff9 clock
    wire KAGY_NR12_WRp = not1(GAXU_NR12_WRn); // not on schematic?
    wire KAGY_NR12_WRn = not1(KAGY_NR12_WRp); // not on schematic?

    /*#p11.JUSA*/ reg_new.ch1.JUSA_NR12_PERIOD0p.dff9b(KYGY_NR12_WRn, reg_new.HATO_APU_RSTn_new(), reg_old.cpu_dbus.BUS_CPU_D00p.qp_old());
    /*#p11.JUZY*/ reg_new.ch1.JUZY_NR12_PERIOD1p.dff9b(KYGY_NR12_WRn, reg_new.HATO_APU_RSTn_new(), reg_old.cpu_dbus.BUS_CPU_D01p.qp_old());
    /*#p11.JOMA*/ reg_new.ch1.JOMA_NR12_PERIOD2p.dff9b(KYGY_NR12_WRn, reg_new.HATO_APU_RSTn_new(), reg_old.cpu_dbus.BUS_CPU_D02p.qp_old());
    /*#p11.JAFY*/ reg_new.ch1.JAFY_NR12_ENV_DIRp.dff9b(KAGY_NR12_WRn, reg_new.HATO_APU_RSTn_new(), reg_old.cpu_dbus.BUS_CPU_D03p.qp_old());
    /*#p11.JATY*/ reg_new.ch1.JATY_NR12_VOL0p   .dff9b(KAGY_NR12_WRn, reg_new.HATO_APU_RSTn_new(), reg_old.cpu_dbus.BUS_CPU_D04p.qp_old());
    /*#p11.JAXO*/ reg_new.ch1.JAXO_NR12_VOL1p   .dff9b(KAGY_NR12_WRn, reg_new.HATO_APU_RSTn_new(), reg_old.cpu_dbus.BUS_CPU_D05p.qp_old());
    /*#p11.JENA*/ reg_new.ch1.JENA_NR12_VOL2p   .dff9b(KAGY_NR12_WRn, reg_new.HATO_APU_RSTn_new(), reg_old.cpu_dbus.BUS_CPU_D06p.qp_old());
    /*#p11.JOPU*/ reg_new.ch1.JOPU_NR12_VOL3p   .dff9b(KAGY_NR12_WRn, reg_new.HATO_APU_RSTn_new(), reg_old.cpu_dbus.BUS_CPU_D07p.qp_old());
  }



  {
    /*#p13.BONE*/ wire BONE_FREQ_OVERFLOWp_new = not1(ATYS_FREQ_OVERFLOWn_new); 
    /*#p13.CYFA*/ wire CYFA_LEN_DONEp_new = and2(reg_new.ch1.CERO_CH1_LEN_DONE.qp_new(), reg_new.ch1.BOKO_NR14_LENENp.qp_newB());
    /*#p13.BERY*/ wire BERY_CH1_STOPp_new = or4(BONE_FREQ_OVERFLOWp_new, reg_new.KEBA_APU_RSTp_new(), CYFA_LEN_DONEp_new, reg_new.ch1.HOCA_CH1_AMP_ENn_new());
    /*#p13.CYTO*/ reg_new.ch1.CYTO_CH1_ACTIVEp.nor_latch(reg_new.ch1.FEKU_CH1_TRIGp.qp_new(), BERY_CH1_STOPp_new);
  }

  //----------
  // Frequency shifter

  {
    /*#p13.KALA*/ wire KALA_SHIFTER_LOADn = nor2(reg_new.ch1.BEXA_SWEEP_TRIGGERp.qp_new(), reg_new.ch1.FEKU_CH1_TRIGp.qp_new());
    /*#p12.KAPE*/ wire KAPE_SHIFTER_LOADp = not1(KALA_SHIFTER_LOADn);
    /*#p12.JUJU*/ wire JUJU_SHIFTER_LOADp = not1(KALA_SHIFTER_LOADn);
    /*#p12.KEDO*/ wire KEDO_SHIFTER_LOADp = not1(KALA_SHIFTER_LOADn);

    /*_p12.JUTA*/ wire JUTA_CH1_FREQ00n = not1(reg_new.ch1.HYKA_CH1_FREQ00.qp_new());
    /*_p12.HOXE*/ wire HOXE_CH1_FREQ01n = not1(reg_new.ch1.JYKA_CH1_FREQ01.qp_new());
    /*_p12.HUNY*/ wire HUNY_CH1_FREQ02n = not1(reg_new.ch1.HAVO_CH1_FREQ02.qp_new());
    /*_p12.KEKE*/ wire KEKE_CH1_FREQ03n = not1(reg_new.ch1.EDUL_CH1_FREQ03.qp_new());
    /*_p12.KOVO*/ wire KOVO_CH1_FREQ04n = not1(reg_new.ch1.FELY_CH1_FREQ04.qp_new());
    /*_p12.FOHY*/ wire FOHY_CH1_FREQ05n = not1(reg_new.ch1.HOLU_CH1_FREQ05.qp_new());
    /*_p12.DEBO*/ wire DEBO_CH1_FREQ06n = not1(reg_new.ch1.HYXU_CH1_FREQ06.qp_new());
    /*_p12.CYKY*/ wire CYKY_CH1_FREQ07n = not1(reg_new.ch1.HOPO_CH1_FREQ07.qp_new());
    /*_p12.BYRU*/ wire BYRU_CH1_FREQ08n = not1(reg_new.ch1.DYGY_CH1_FREQ08.qp_new());
    /*_p12.APAT*/ wire APAT_CH1_FREQ09n = not1(reg_new.ch1.EVAB_CH1_FREQ09.qp_new());
    /*_p12.BOJO*/ wire BOJO_CH1_FREQ10n = not1(reg_new.ch1.AXAN_CH1_FREQ10.qp_new());

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

    /*_p12.HOZU*/ wire HOZU_SHIFTER_SETn_00 = nand2(reg_new.ch1.HYKA_CH1_FREQ00.qp_new(), KAPE_SHIFTER_LOADp);
    /*_p12.HOLA*/ wire HOLA_SHIFTER_SETn_01 = nand2(reg_new.ch1.JYKA_CH1_FREQ01.qp_new(), KAPE_SHIFTER_LOADp);
    /*_p12.HAWY*/ wire HAWY_SHIFTER_SETn_02 = nand2(reg_new.ch1.HAVO_CH1_FREQ02.qp_new(), KAPE_SHIFTER_LOADp);
    /*_p12.KYRY*/ wire KYRY_SHIFTER_SETn_03 = nand2(reg_new.ch1.EDUL_CH1_FREQ03.qp_new(), JUJU_SHIFTER_LOADp);
    /*_p12.KOLA*/ wire KOLA_SHIFTER_SETn_04 = nand2(reg_new.ch1.FELY_CH1_FREQ04.qp_new(), JUJU_SHIFTER_LOADp);
    /*_p12.GOVO*/ wire GOVO_SHIFTER_SETn_05 = nand2(reg_new.ch1.HOLU_CH1_FREQ05.qp_new(), JUJU_SHIFTER_LOADp);
    /*_p12.EKEM*/ wire EKEM_SHIFTER_SETn_06 = nand2(reg_new.ch1.HYXU_CH1_FREQ06.qp_new(), JUJU_SHIFTER_LOADp);
    /*_p12.DACE*/ wire DACE_SHIFTER_SETn_07 = nand2(reg_new.ch1.HOPO_CH1_FREQ07.qp_new(), KEDO_SHIFTER_LOADp);
    /*_p12.BEGE*/ wire BEGE_SHIFTER_SETn_08 = nand2(reg_new.ch1.DYGY_CH1_FREQ08.qp_new(), KEDO_SHIFTER_LOADp);
    /*_p12.BESO*/ wire BESO_SHIFTER_SETn_09 = nand2(reg_new.ch1.EVAB_CH1_FREQ09.qp_new(), KEDO_SHIFTER_LOADp);
    /*_p12.BEJU*/ wire BEJU_SHIFTER_SETn_10 = nand2(reg_new.ch1.AXAN_CH1_FREQ10.qp_new(), KEDO_SHIFTER_LOADp);

    /*_p12.JADO*/ wire JADO_SHIFTER_RSTn_00 = nor2(reg_new.KEBA_APU_RSTp_new(), KOKO_CH1_FREQ00n);
    /*_p12.HOBU*/ wire HOBU_SHIFTER_RSTn_01 = nor2(reg_new.KEBA_APU_RSTp_new(), JOCY_CH1_FREQ01n);
    /*_p12.HYVU*/ wire HYVU_SHIFTER_RSTn_02 = nor2(reg_new.KEBA_APU_RSTp_new(), JEHY_CH1_FREQ02n);
    /*_p12.KETO*/ wire KETO_SHIFTER_RSTn_03 = nor2(reg_new.KEBA_APU_RSTp_new(), KAXY_CH1_FREQ03n);
    /*_p12.KYBO*/ wire KYBO_SHIFTER_RSTn_04 = nor2(reg_new.KEBA_APU_RSTp_new(), KEVY_CH1_FREQ04n);
    /*_p12.EZUK*/ wire EZUK_SHIFTER_RSTn_05 = nor2(reg_new.KEBA_APU_RSTp_new(), EVOF_CH1_FREQ05n);
    /*_p12.ENOK*/ wire ENOK_SHIFTER_RSTn_06 = nor2(reg_new.KEBA_APU_RSTp_new(), EREG_CH1_FREQ06n);
    /*_p12.BEWO*/ wire BEWO_SHIFTER_RSTn_07 = nor2(reg_new.KEBA_APU_RSTp_new(), BAPU_CH1_FREQ07n);
    /*_p12.AGOR*/ wire AGOR_SHIFTER_RSTn_08 = nor2(reg_new.KEBA_APU_RSTp_new(), AFUG_CH1_FREQ08n);
    /*_p12.AFUX*/ wire AFUX_SHIFTER_RSTn_09 = nor2(reg_new.KEBA_APU_RSTp_new(), BUVO_CH1_FREQ09n);
    /*_p12.AVUF*/ wire AVUF_SHIFTER_RSTn_10 = nor2(reg_new.KEBA_APU_RSTp_new(), AFYR_CH1_FREQ10n);

    /*#p12.FABU*/ reg_new.ch1.FABU_CH1_SHIFT00.dff22(reg_new.ch1.EGOR_SHIFT_CLK_new(), HOZU_SHIFTER_SETn_00, JADO_SHIFTER_RSTn_00, reg_old.ch1.JEFA_CH1_SHIFT01.qp_old());
    /*_p12.JEFA*/ reg_new.ch1.JEFA_CH1_SHIFT01.dff22(reg_new.ch1.EGOR_SHIFT_CLK_new(), HOLA_SHIFTER_SETn_01, HOBU_SHIFTER_RSTn_01, reg_old.ch1.GOGA_CH1_SHIFT02.qp_old());
    /*_p12.GOGA*/ reg_new.ch1.GOGA_CH1_SHIFT02.dff22(reg_new.ch1.EGOR_SHIFT_CLK_new(), HAWY_SHIFTER_SETn_02, HYVU_SHIFTER_RSTn_02, reg_old.ch1.JOLU_CH1_SHIFT03.qp_old());
    /*_p12.JOLU*/ reg_new.ch1.JOLU_CH1_SHIFT03.dff22(reg_new.ch1.EJYB_SHIFT_CLK_new(), KYRY_SHIFTER_SETn_03, KETO_SHIFTER_RSTn_03, reg_old.ch1.JOTA_CH1_SHIFT04.qp_old());
    /*_p12.JOTA*/ reg_new.ch1.JOTA_CH1_SHIFT04.dff22(reg_new.ch1.EJYB_SHIFT_CLK_new(), KOLA_SHIFTER_SETn_04, KYBO_SHIFTER_RSTn_04, reg_old.ch1.FUDE_CH1_SHIFT05.qp_old());
    /*_p12.FUDE*/ reg_new.ch1.FUDE_CH1_SHIFT05.dff22(reg_new.ch1.EJYB_SHIFT_CLK_new(), GOVO_SHIFTER_SETn_05, EZUK_SHIFTER_RSTn_05, reg_old.ch1.FEDO_CH1_SHIFT06.qp_old());
    /*_p12.FEDO*/ reg_new.ch1.FEDO_CH1_SHIFT06.dff22(reg_new.ch1.EJYB_SHIFT_CLK_new(), EKEM_SHIFTER_SETn_06, ENOK_SHIFTER_RSTn_06, reg_old.ch1.EXAC_CH1_SHIFT07.qp_old());
    /*_p12.EXAC*/ reg_new.ch1.EXAC_CH1_SHIFT07.dff22(reg_new.ch1.BECY_SHIFT_CLK_new(), DACE_SHIFTER_SETn_07, BEWO_SHIFTER_RSTn_07, reg_old.ch1.ELUX_CH1_SHIFT08.qp_old());
    /*_p12.ELUX*/ reg_new.ch1.ELUX_CH1_SHIFT08.dff22(reg_new.ch1.BECY_SHIFT_CLK_new(), BEGE_SHIFTER_SETn_08, AGOR_SHIFTER_RSTn_08, reg_old.ch1.AGEZ_CH1_SHIFT09.qp_old());
    /*_p12.AGEZ*/ reg_new.ch1.AGEZ_CH1_SHIFT09.dff22(reg_new.ch1.BECY_SHIFT_CLK_new(), BESO_SHIFTER_SETn_09, AFUX_SHIFTER_RSTn_09, reg_old.ch1.BEKU_CH1_SHIFT10.qp_old());
    /*_p12.BEKU*/ reg_new.ch1.BEKU_CH1_SHIFT10.dff22(reg_new.ch1.BECY_SHIFT_CLK_new(), BEJU_SHIFTER_SETn_10, AVUF_SHIFTER_RSTn_10, 0);
  }

  {
    // When the delay expires, we generate one env tick.

    /*#p13.KURY*/ wire KURY_ENV_CLKn_old = not1(reg_old.ch1.KOZY_ENV_TICKp.qp_old());
    /*#p13.KUKU*/ wire KUKU_ENV_CLKp_old = nor2(reg_old.spu.HORU_CLK_512_old(), KURY_ENV_CLKn_old);
    /*#p13.KORO*/ wire KORO_ENV_CLK_RSTn_old = nor4(KUKU_ENV_CLKp_old, reg_old.ch1.KOMA_ENV_OFFp_old(), reg_old.ch1.FEKU_CH1_TRIGp.qp_old(), reg_old.KEBA_APU_RSTp_old());

    /*#p13.KOTE*/ wire KOTE_ENV_DELAY_DONEp_old = and3(reg_old.ch1.KERA_ENV_DELAY2p.qp_old(), reg_old.ch1.KENU_ENV_DELAY1p.qp_old(), reg_old.ch1.JOVA_ENV_DELAY0p.qp_old());
    
    /*#p13.KOZY*/ reg_new.ch1.KOZY_ENV_TICKp.dff17(reg_new.spu.HORU_CLK_512_new(), KORO_ENV_CLK_RSTn_old, KOTE_ENV_DELAY_DONEp_old);

    /*#p13.KURY*/ wire KURY_ENV_CLKn_new = not1(reg_new.ch1.KOZY_ENV_TICKp.qp_new());
    /*#p13.KUKU*/ wire KUKU_ENV_CLKp_new = nor2(reg_new.spu.HORU_CLK_512_new(), KURY_ENV_CLKn_new);
    /*#p13.KORO*/ wire KORO_ENV_CLK_RSTn_new = nor4(KUKU_ENV_CLKp_new, reg_new.ch1.KOMA_ENV_OFFp_new(), reg_new.ch1.FEKU_CH1_TRIGp.qp_new(), reg_new.KEBA_APU_RSTp_new());

    /*#p13.KOZY*/ reg_new.ch1.KOZY_ENV_TICKp.dff17_any(reg_new.spu.HORU_CLK_512_new(), KORO_ENV_CLK_RSTn_new, KOTE_ENV_DELAY_DONEp_old);
  }


  /*#p13.KALY*/ reg_new.ch1.KALY_ENV_DELAY_CLK_64n.dff17(reg_new.spu.JONE_CLK_128p(), reg_new.KADO_APU_RSTn_new(), reg_old.ch1.KALY_ENV_DELAY_CLK_64n.qn_old());
  /*#p13.KERE*/ wire KERE_ENV_DELAY_CLK_64p = not1(reg_new.ch1.KALY_ENV_DELAY_CLK_64n.qp_new());
  /*#p13.JOLA*/ wire JOLA_ENV_DELAY_CLK_64n = not1(KERE_ENV_DELAY_CLK_64p);

  /*#p13.KAZA*/ wire KAZA_ENV_DELAY_LOADn = nor2(reg_new.ch1.FEKU_CH1_TRIGp.qp_new(), reg_new.ch1.KOZY_ENV_TICKp.qp_new());
  /*#p13.KUXU*/ wire KUXU_ENV_DELAY_LOADp = not1(KAZA_ENV_DELAY_LOADn);

  /*#p13.JOVA*/ reg_new.ch1.JOVA_ENV_DELAY0p.dff20(JOLA_ENV_DELAY_CLK_64n,                KUXU_ENV_DELAY_LOADp, reg_old.ch1.JUSA_NR12_PERIOD0p.qn_oldB());
  /*#p13.KENU*/ reg_new.ch1.KENU_ENV_DELAY1p.dff20(reg_new.ch1.JOVA_ENV_DELAY0p.qp_new(), KUXU_ENV_DELAY_LOADp, reg_old.ch1.JUZY_NR12_PERIOD1p.qn_oldB());
  /*#p13.KERA*/ reg_new.ch1.KERA_ENV_DELAY2p.dff20(reg_new.ch1.KENU_ENV_DELAY1p.qp_new(), KUXU_ENV_DELAY_LOADp, reg_old.ch1.JOMA_NR12_PERIOD2p.qn_oldB());

  {
    // Check to see if our env is 0b0000 or 0b1111
    /*#p13.KORU*/ wire KORU_ENV_MAX_RSTn_new = nor2(reg_new.ch1.FEKU_CH1_TRIGp.qp_new(), reg_new.KEBA_APU_RSTp_new());
    /*#p13.HUFU*/ wire HUFU_ENV_TOPn_old = nand5(reg_old.ch1.JAFY_NR12_ENV_DIRp.qn_oldB(), reg_old.ch1.HAFO_CH1_ENV0p.qp_old(), reg_old.ch1.HEMY_CH1_ENV1p.qp_old(), reg_old.ch1.HOKO_CH1_ENV2p.qp_old(), reg_old.ch1.HEVO_CH1_ENV3p.qp_old());
    /*#p13.HANO*/ wire HANO_ENV_BOTp_old = nor5 (reg_old.ch1.JAFY_NR12_ENV_DIRp.qn_oldB(), reg_old.ch1.HAFO_CH1_ENV0p.qp_old(), reg_old.ch1.HEMY_CH1_ENV1p.qp_old(), reg_old.ch1.HOKO_CH1_ENV2p.qp_old(), reg_old.ch1.HEVO_CH1_ENV3p.qp_old());
    /*#p13.HAKE*/ wire HAKE_ENV_TOPp_old = not1(HUFU_ENV_TOPn_old);
    /*#p13.JADE*/ wire JADE_ENV_MAXp_old = or2(HANO_ENV_BOTp_old, HAKE_ENV_TOPp_old);
    /*#p13.KYNO*/ reg_new.ch1.KYNO_ENV_MAXp.dff17(reg_new.ch1.KOZY_ENV_TICKp.qp_new(), KORU_ENV_MAX_RSTn_new, JADE_ENV_MAXp_old);
  }

  {
    // If our env maxes out, don't tick the envelope anymore.
    /*#p13.KEKO*/ wire KEKO_ENV_STARTp = or2(reg_new.KEBA_APU_RSTp_new(), reg_new.ch1.FEKU_CH1_TRIGp.qp_new());
    /*#p13.KEZU*/ reg_new.ch1.KEZU_ENV_ACTIVEn.nor_latch(reg_new.ch1.KYNO_ENV_MAXp.qp_new(), KEKO_ENV_STARTp);
  }


  /*#p13.KAKE*/ wire KAKE_ENV_CLK = and3(reg_new.ch1.KOZY_ENV_TICKp.qp_new(), reg_new.ch1.KOMA_ENV_OFFp_new(), reg_new.ch1.KEZU_ENV_ACTIVEn.qp_new());

  // The muxes select posedge or negedge for the env counter so it can count up _or_ down?
  // these were connected wrong in schematic

  /*#p13.JUFY*/ wire JUFY_ENV_CLK = amux2(reg_new.ch1.JAFY_NR12_ENV_DIRp.qp_newB(), KAKE_ENV_CLK, KAKE_ENV_CLK, reg_new.ch1.JAFY_NR12_ENV_DIRp.qn_newB());
  /*#p13.HAFO*/ reg_new.ch1.HAFO_CH1_ENV0p.dff20(JUFY_ENV_CLK, reg_new.ch1.FEKU_CH1_TRIGp.qp_new(), reg_old.ch1.JATY_NR12_VOL0p.qp_oldB());

  /*#p13.HYTO*/ wire HYTO_ENV_CLK = amux2(reg_new.ch1.JAFY_NR12_ENV_DIRp.qp_newB(), reg_new.ch1.HAFO_CH1_ENV0p.qp_new(), reg_new.ch1.HAFO_CH1_ENV0p.qn_new(), reg_new.ch1.JAFY_NR12_ENV_DIRp.qn_newB());
  /*#p13.HEMY*/ reg_new.ch1.HEMY_CH1_ENV1p.dff20(HYTO_ENV_CLK, reg_new.ch1.FEKU_CH1_TRIGp.qp_new(), reg_old.ch1.JAXO_NR12_VOL1p.qp_oldB());

  /*#p13.HETO*/ wire HETO_ENV_CLK = amux2(reg_new.ch1.JAFY_NR12_ENV_DIRp.qp_newB(), reg_new.ch1.HEMY_CH1_ENV1p.qp_new(), reg_new.ch1.HEMY_CH1_ENV1p.qn_new(), reg_new.ch1.JAFY_NR12_ENV_DIRp.qn_newB());
  /*#p13.HOKO*/ reg_new.ch1.HOKO_CH1_ENV2p.dff20(HETO_ENV_CLK, reg_new.ch1.FEKU_CH1_TRIGp.qp_new(), reg_old.ch1.JENA_NR12_VOL2p.qp_oldB());

  /*#p13.HESU*/ wire HESU_ENV_CLK = amux2(reg_new.ch1.JAFY_NR12_ENV_DIRp.qp_newB(), reg_new.ch1.HOKO_CH1_ENV2p.qp_new(), reg_new.ch1.HOKO_CH1_ENV2p.qn_new(), reg_new.ch1.JAFY_NR12_ENV_DIRp.qn_newB());
  /*#p13.HEVO*/ reg_new.ch1.HEVO_CH1_ENV3p.dff20(HESU_ENV_CLK, reg_new.ch1.FEKU_CH1_TRIGp.qp_new(), reg_old.ch1.JOPU_NR12_VOL3p.qp_oldB());

  //----------
  // Waveform generator

  {
    // This is going to be an interesting bit of circular stuff to unwind...

    {
      /*#p13.FEMY*/ wire FEMY_CH2_OFFn_new = nor2(reg_new.ch1.HOCA_CH1_AMP_ENn_new(), reg_new.KEBA_APU_RSTp_new());
      /*#p13.GEPU*/ wire GEPU_CH1_TRIGn_new = not1(reg_new.ch1.FYTE_CH1_TRIGp.qp_new());
      /*#p13.GEXU*/ reg_new.ch1.GEXU_FREQ_GATEn.nand_latch(FEMY_CH2_OFFn_new, GEPU_CH1_TRIGn_new);
    }

    /*#p11.FULO*/ wire FULO_FREQ_CLK_new = nor2(reg_new.ch1.DYFA_xBCDExxx(), reg_new.ch1.GEXU_FREQ_GATEn.qp_new());
    /*#p11.GEKU*/ wire GEKU_FREQ_CLK_new = not1(FULO_FREQ_CLK_new);






    // Frequency counter

    {
      /*#p13.CALA*/ wire CALA_FREQ_OVERFLOWp_old = not1(reg_old.ch1.COPU_CH1_FREQ_CNT_10.qp_old());
      /*#p13.DOKA*/ wire DOKA_COMY_RSTp_old = and2(reg_old.ch1.COMY_SAMPLE_CLKp.qp_old(), reg_old.ch1.DYFA_CLK_1M_old());
      /*#p13.DYRU*/ wire DYRU_COMY_RSTn_old = nor3(reg_old.KEBA_APU_RSTp_old(), reg_old.ch1.FEKU_CH1_TRIGp.qp_old(), DOKA_COMY_RSTp_old);
      /*#p13.CYTE*/ wire CYTE_SAMPLE_CLKn_old = not1(reg_old.ch1.COMY_SAMPLE_CLKp.qp_old());
      /*#p13.COPE*/ wire COPE_SAMPLE_CLKp_old = not1(CYTE_SAMPLE_CLKn_old);

      /*#p11.EPYK*/ wire EPYK_FREQ_LOADn_old = nor2(COPE_SAMPLE_CLKp_old, reg_old.ch1.FEKU_CH1_TRIGp.qp_old());
      /*#p11.FUME*/ wire FUME_FREQ_LOADp_old = not1(EPYK_FREQ_LOADn_old);
      /*#p11.DEGA*/ wire DEGA_FREQ_LOADp_old = not1(EPYK_FREQ_LOADn_old);
      /*#p11.DAKO*/ wire DAKO_FREQ_LOADp_old = not1(EPYK_FREQ_LOADn_old);

      /*#p11.GAXE*/ reg_new.ch1.GAXE_CH1_FREQ_CNT_00.dff20(GEKU_FREQ_CLK_new,                         FUME_FREQ_LOADp_old, reg_old.ch1.HYKA_CH1_FREQ00.qp_old());
      /*#p11.HYFE*/ reg_new.ch1.HYFE_CH1_FREQ_CNT_01.dff20(reg_new.ch1.GAXE_CH1_FREQ_CNT_00.qp_new(), FUME_FREQ_LOADp_old, reg_old.ch1.JYKA_CH1_FREQ01.qp_old());
      /*_p11.JYTY*/ reg_new.ch1.JYTY_CH1_FREQ_CNT_02.dff20(reg_new.ch1.HYFE_CH1_FREQ_CNT_01.qp_new(), FUME_FREQ_LOADp_old, reg_old.ch1.HAVO_CH1_FREQ02.qp_old());
      /*_p11.KYNA*/ reg_new.ch1.KYNA_CH1_FREQ_CNT_03.dff20(reg_new.ch1.JYTY_CH1_FREQ_CNT_02.qp_new(), FUME_FREQ_LOADp_old, reg_old.ch1.EDUL_CH1_FREQ03.qp_old());

      /*#p11.KYPE*/ wire KYPE_FREQ_CLK_new = not1(reg_new.ch1.KYNA_CH1_FREQ_CNT_03.qn_new());
      /*_p11.JEMA*/ reg_new.ch1.JEMA_CH1_FREQ_CNT_04.dff20(KYPE_FREQ_CLK_new,                         DEGA_FREQ_LOADp_old, reg_old.ch1.FELY_CH1_FREQ04.qp_old());
      /*_p11.HYKE*/ reg_new.ch1.HYKE_CH1_FREQ_CNT_05.dff20(reg_new.ch1.JEMA_CH1_FREQ_CNT_04.qp_new(), DEGA_FREQ_LOADp_old, reg_old.ch1.HOLU_CH1_FREQ05.qp_old());
      /*_p11.FEVA*/ reg_new.ch1.FEVA_CH1_FREQ_CNT_06.dff20(reg_new.ch1.HYKE_CH1_FREQ_CNT_05.qp_new(), DEGA_FREQ_LOADp_old, reg_old.ch1.HYXU_CH1_FREQ06.qp_old());
      /*_p11.EKOV*/ reg_new.ch1.EKOV_CH1_FREQ_CNT_07.dff20(reg_new.ch1.FEVA_CH1_FREQ_CNT_06.qp_new(), DEGA_FREQ_LOADp_old, reg_old.ch1.HOPO_CH1_FREQ07.qp_old());

      /*#p11.DERU*/ wire DERU_FREQ_CLK_new = not1(reg_new.ch1.EKOV_CH1_FREQ_CNT_07.qn_new());
      /*#p11.EMUS*/ reg_new.ch1.EMUS_CH1_FREQ_CNT_08.dff20(DERU_FREQ_CLK_new,                         DAKO_FREQ_LOADp_old, reg_old.ch1.DYGY_CH1_FREQ08.qp_old());
      /*#p11.EVAK*/ reg_new.ch1.EVAK_CH1_FREQ_CNT_09.dff20(reg_new.ch1.EMUS_CH1_FREQ_CNT_08.qp_new(), DAKO_FREQ_LOADp_old, reg_old.ch1.EVAB_CH1_FREQ09.qp_old());
      /*#p11.COPU*/ reg_new.ch1.COPU_CH1_FREQ_CNT_10.dff20(reg_new.ch1.EVAK_CH1_FREQ_CNT_09.qp_new(), DAKO_FREQ_LOADp_old, reg_old.ch1.AXAN_CH1_FREQ10.qp_old());
    }

    {
      /*#p13.CALA*/ wire CALA_FREQ_OVERFLOWp_new = not1(reg_new.ch1.COPU_CH1_FREQ_CNT_10.qp_new());
      /*#p13.DOKA*/ wire DOKA_COMY_RSTp_old = and2(reg_old.ch1.COMY_SAMPLE_CLKp.qp_old(), reg_old.ch1.DYFA_CLK_1M_old());
      /*#p13.DYRU*/ wire DYRU_COMY_RSTn_old = nor3(reg_old.KEBA_APU_RSTp_old(), reg_old.ch1.FEKU_CH1_TRIGp.qp_old(), DOKA_COMY_RSTp_old);
      /*_p13.COMY*/ reg_new.ch1.COMY_SAMPLE_CLKp.dff17(CALA_FREQ_OVERFLOWp_new, DYRU_COMY_RSTn_old, reg_old.ch1.COMY_SAMPLE_CLKp.qn_old());
    }

    {
      /*#p13.CALA*/ wire CALA_FREQ_OVERFLOWp_new = not1(reg_new.ch1.COPU_CH1_FREQ_CNT_10.qp_new());
      /*#p13.DOKA*/ wire DOKA_COMY_RSTp_new = and2(reg_new.ch1.COMY_SAMPLE_CLKp.qp_new(), reg_new.ch1.DYFA_xBCDExxx());
      /*#p13.DYRU*/ wire DYRU_COMY_RSTn_new = nor3(reg_new.KEBA_APU_RSTp_new(), reg_new.ch1.FEKU_CH1_TRIGp.qp_new(), DOKA_COMY_RSTp_new);
      /*_p13.COMY*/ reg_new.ch1.COMY_SAMPLE_CLKp.dff17_any(CALA_FREQ_OVERFLOWp_new, DYRU_COMY_RSTn_new, reg_old.ch1.COMY_SAMPLE_CLKp.qn_old());
    }

    {
      /*#p13.CYTE*/ wire CYTE_SAMPLE_CLKn_new = not1(reg_new.ch1.COMY_SAMPLE_CLKp.qp_new());
      /*#p13.COPE*/ wire COPE_SAMPLE_CLKp_new = not1(CYTE_SAMPLE_CLKn_new);

      /*#p11.EPYK*/ wire EPYK_FREQ_LOADn_new = nor2(COPE_SAMPLE_CLKp_new, reg_new.ch1.FEKU_CH1_TRIGp.qp_new());
      /*#p11.FUME*/ wire FUME_FREQ_LOADp_new = not1(EPYK_FREQ_LOADn_new);
      /*#p11.DEGA*/ wire DEGA_FREQ_LOADp_new = not1(EPYK_FREQ_LOADn_new);
      /*#p11.DAKO*/ wire DAKO_FREQ_LOADp_new = not1(EPYK_FREQ_LOADn_new);

      /*#p11.GAXE*/ reg_new.ch1.GAXE_CH1_FREQ_CNT_00.dff20_any(GEKU_FREQ_CLK_new,                         FUME_FREQ_LOADp_new, reg_old.ch1.HYKA_CH1_FREQ00.qp_old());
      /*#p11.HYFE*/ reg_new.ch1.HYFE_CH1_FREQ_CNT_01.dff20_any(reg_new.ch1.GAXE_CH1_FREQ_CNT_00.qp_new(), FUME_FREQ_LOADp_new, reg_old.ch1.JYKA_CH1_FREQ01.qp_old());
      /*_p11.JYTY*/ reg_new.ch1.JYTY_CH1_FREQ_CNT_02.dff20_any(reg_new.ch1.HYFE_CH1_FREQ_CNT_01.qp_new(), FUME_FREQ_LOADp_new, reg_old.ch1.HAVO_CH1_FREQ02.qp_old());
      /*_p11.KYNA*/ reg_new.ch1.KYNA_CH1_FREQ_CNT_03.dff20_any(reg_new.ch1.JYTY_CH1_FREQ_CNT_02.qp_new(), FUME_FREQ_LOADp_new, reg_old.ch1.EDUL_CH1_FREQ03.qp_old());

      /*#p11.KYPE*/ wire KYPE_FREQ_CLK_new = not1(reg_new.ch1.KYNA_CH1_FREQ_CNT_03.qn_new());
      /*_p11.JEMA*/ reg_new.ch1.JEMA_CH1_FREQ_CNT_04.dff20_any(KYPE_FREQ_CLK_new,                         DEGA_FREQ_LOADp_new, reg_old.ch1.FELY_CH1_FREQ04.qp_old());
      /*_p11.HYKE*/ reg_new.ch1.HYKE_CH1_FREQ_CNT_05.dff20_any(reg_new.ch1.JEMA_CH1_FREQ_CNT_04.qp_new(), DEGA_FREQ_LOADp_new, reg_old.ch1.HOLU_CH1_FREQ05.qp_old());
      /*_p11.FEVA*/ reg_new.ch1.FEVA_CH1_FREQ_CNT_06.dff20_any(reg_new.ch1.HYKE_CH1_FREQ_CNT_05.qp_new(), DEGA_FREQ_LOADp_new, reg_old.ch1.HYXU_CH1_FREQ06.qp_old());
      /*_p11.EKOV*/ reg_new.ch1.EKOV_CH1_FREQ_CNT_07.dff20_any(reg_new.ch1.FEVA_CH1_FREQ_CNT_06.qp_new(), DEGA_FREQ_LOADp_new, reg_old.ch1.HOPO_CH1_FREQ07.qp_old());

      /*#p11.DERU*/ wire DERU_FREQ_CLK_new = not1(reg_new.ch1.EKOV_CH1_FREQ_CNT_07.qn_new());
      /*#p11.EMUS*/ reg_new.ch1.EMUS_CH1_FREQ_CNT_08.dff20_any(DERU_FREQ_CLK_new,                         DAKO_FREQ_LOADp_new, reg_old.ch1.DYGY_CH1_FREQ08.qp_old());
      /*#p11.EVAK*/ reg_new.ch1.EVAK_CH1_FREQ_CNT_09.dff20_any(reg_new.ch1.EMUS_CH1_FREQ_CNT_08.qp_new(), DAKO_FREQ_LOADp_new, reg_old.ch1.EVAB_CH1_FREQ09.qp_old());
      /*#p11.COPU*/ reg_new.ch1.COPU_CH1_FREQ_CNT_10.dff20_any(reg_new.ch1.EVAK_CH1_FREQ_CNT_09.qp_new(), DAKO_FREQ_LOADp_new, reg_old.ch1.AXAN_CH1_FREQ10.qp_old());
    }

    // Duty cycle generator

    {
      /*#p13.CYTE*/ wire CYTE_SAMPLE_CLKn_new = not1(reg_new.ch1.COMY_SAMPLE_CLKp.qp_new());
      /*#p13.COPE*/ wire COPE_SAMPLE_CLKp_new = not1(CYTE_SAMPLE_CLKn_new);
      /*#p13.DAJO*/ wire DAJO_SAMPLE_CLKn_new = not1(COPE_SAMPLE_CLKp_new);
      /*#p13.ESUT*/ reg_new.ch1.ESUT_PHASE_xBxDxFxH.dff17(DAJO_SAMPLE_CLKn_new,                     reg_new.CEPO_APU_RSTn_new(), reg_old.ch1.ESUT_PHASE_xBxDxFxH.qn_old());
      /*#p13.EROS*/ reg_new.ch1.EROS_PHASE_xxCDxxGH.dff13(reg_new.ch1.ESUT_PHASE_xBxDxFxH.qn_new(), reg_new.CEPO_APU_RSTn_new(), reg_old.ch1.EROS_PHASE_xxCDxxGH.qn_old());
      /*#p13.DAPE*/ reg_new.ch1.DAPE_PHASE_xxxxEFGH.dff13(reg_new.ch1.EROS_PHASE_xxCDxxGH.qn_new(), reg_new.CEPO_APU_RSTn_new(), reg_old.ch1.DAPE_PHASE_xxxxEFGH.qn_old());

      /*#p13.DUVO*/ wire DUVO_PHASE_AxCxExGx_old = not1(reg_old.ch1.ESUT_PHASE_xBxDxFxH.qp_old());
      /*#p13.EZOZ*/ wire EZOZ_PHASE_xxxxxxGH_old = and2(reg_old.ch1.DAPE_PHASE_xxxxEFGH.qp_old(), reg_old.ch1.EROS_PHASE_xxCDxxGH.qp_old());
      /*#p13.ENEK*/ wire ENEK_PHASE_xxxxxxGx_old = and2(EZOZ_PHASE_xxxxxxGH_old, DUVO_PHASE_AxCxExGx_old);
      /*#p13.CODO*/ wire CODO_PHASE_ABCDEFxx_old = not1(EZOZ_PHASE_xxxxxxGH_old);


      /*#p13.COSO*/ wire COSO_old = nor2(reg_old.ch1.CENA_NR11_DUTY0p.qp_oldB(), reg_old.ch1.DYCA_NR11_DUTY1p.qp_oldB());
      /*#p13.CAVA*/ wire CAVA_old = nor2(reg_old.ch1.CENA_NR11_DUTY0p.qn_oldB(), reg_old.ch1.DYCA_NR11_DUTY1p.qp_oldB());
      /*#p13.CEVU*/ wire CEVU_old = nor2(reg_old.ch1.CENA_NR11_DUTY0p.qp_oldB(), reg_old.ch1.DYCA_NR11_DUTY1p.qn_oldB());
      /*#p13.CAXO*/ wire CAXO_old = nor2(reg_old.ch1.CENA_NR11_DUTY0p.qn_oldB(), reg_old.ch1.DYCA_NR11_DUTY1p.qn_oldB());
      /*#p13.DUNA*/ wire DUNA_RAW_BIT_old = amux4(ENEK_PHASE_xxxxxxGx_old, COSO_old,
                                                  EZOZ_PHASE_xxxxxxGH_old, CAVA_old,
                                                  reg_old.ch1.DAPE_PHASE_xxxxEFGH.qp_old(), CEVU_old,
                                                  CODO_PHASE_ABCDEFxx_old, CAXO_old);
      /*_p13.DUWO*/ reg_new.ch1.DUWO_RAW_BIT_SYNCp.dff17(COPE_SAMPLE_CLKp_new, reg_new.CEPO_APU_RSTn_new(), DUNA_RAW_BIT_old);
    }
  }

  {
    /*#p11.COVU*/ wire COVU_NR11_WR = and2(reg_new.cpu_signals.BOGY_CPU_WRp(), reg_new.cpu_abus.CAXE_ADDR_FF11p());
    /*#p11.DAFO*/ wire DAFO_NR11_CLK = not1(COVU_NR11_WR);
    /*#p11.CENA*/ reg_new.ch1.CENA_NR11_DUTY0p.dff9b(DAFO_NR11_CLK, reg_new.CEPO_APU_RSTn_new(), reg_old.cpu_dbus.BUS_CPU_D06p.qp_old());
    /*#p11.DYCA*/ reg_new.ch1.DYCA_NR11_DUTY1p.dff9b(DAFO_NR11_CLK, reg_new.CEPO_APU_RSTn_new(), reg_old.cpu_dbus.BUS_CPU_D07p.qp_old());
  }

  //----------
  // Audio output

  {
    /*#p13.COWE*/ wire COWE_BIT_OUTp = and2(reg_new.ch1.CYTO_CH1_ACTIVEp.qp_new(), reg_new.ch1.DUWO_RAW_BIT_SYNCp.qp_new());
    /*#p13.BOTO*/ wire BOTO_BIT_OUTp = or2(COWE_BIT_OUTp, reg_new.EDEK_NR52_DBG_APUp());
    /*#p13.AMOP*/ wire AMOP_CH1_OUT0 = and2(reg_new.ch1.HAFO_CH1_ENV0p.qp_new(), BOTO_BIT_OUTp);
    /*#p13.ASON*/ wire ASON_CH1_OUT1 = and2(reg_new.ch1.HEMY_CH1_ENV1p.qp_new(), BOTO_BIT_OUTp);
    /*#p13.AGOF*/ wire AGOF_CH1_OUT2 = and2(reg_new.ch1.HOKO_CH1_ENV2p.qp_new(), BOTO_BIT_OUTp);
    /*#p13.ACEG*/ wire ACEG_CH1_OUT3 = and2(reg_new.ch1.HEVO_CH1_ENV3p.qp_new(), BOTO_BIT_OUTp);
  }





























  {
    /*#p11.ATYN*/ wire ATYN_NR10_RDp = nor2(reg_new.cpu_signals.AGUZ_CPU_RDn(), reg_new.cpu_abus.BUZE_ADDR_FF10n());
    /*#p11.ASOP*/ wire ASOP_NR10_RDn = not1(ATYN_NR10_RDp);

    // Isn't the data polarity here backwards?
    /*#p11.AMYD*/ triwire AMYD_NR10_SWEEP_SHIFT0  = tri6_nn(ASOP_NR10_RDn, reg_new.ch1.BANY_NR10_SWEEP_SHIFT0p .qn_newB());
    /*#p11.ATAX*/ triwire ATAX_NR10_SWEEP_SHIFT1  = tri6_nn(ASOP_NR10_RDn, reg_new.ch1.ARAX_NR10_SWEEP_SHIFT1p .qn_newB());
    /*#p11.AZYP*/ triwire AZYP_NR10_SWEEP_SHIFT2  = tri6_nn(ASOP_NR10_RDn, reg_new.ch1.ANAZ_NR10_SWEEP_SHIFT2p .qn_newB());
    /*#p11.AFOX*/ triwire AFOX_NR10_SWEEP_NEGATE  = tri6_nn(ASOP_NR10_RDn, reg_new.ch1.AVAF_NR10_SWEEP_DIRp    .qn_newB());
    /*#p11.AVEK*/ triwire AVEK_NR10_SWEEP_PERIOD0 = tri6_nn(ASOP_NR10_RDn, reg_new.ch1.ADEK_NR10_SWEEP_PERIOD0p.qn_newB());
    /*#p11.AKUX*/ triwire AKUX_NR10_SWEEP_PERIOD1 = tri6_nn(ASOP_NR10_RDn, reg_new.ch1.BANA_NR10_SWEEP_PERIOD1p.qn_newB());
    /*#p11.AWOS*/ triwire AWOS_NR10_SWEEP_PERIOD2 = tri6_nn(ASOP_NR10_RDn, reg_new.ch1.BOTU_NR10_SWEEP_PERIOD2p.qn_newB());

    /*_BUS_CPU_D00p*/ reg_new.cpu_dbus.BUS_CPU_D00p.tri_bus(AMYD_NR10_SWEEP_SHIFT0);
    /*_BUS_CPU_D01p*/ reg_new.cpu_dbus.BUS_CPU_D01p.tri_bus(ATAX_NR10_SWEEP_SHIFT1);
    /*_BUS_CPU_D02p*/ reg_new.cpu_dbus.BUS_CPU_D02p.tri_bus(AZYP_NR10_SWEEP_SHIFT2);
    /*_BUS_CPU_D03p*/ reg_new.cpu_dbus.BUS_CPU_D03p.tri_bus(AFOX_NR10_SWEEP_NEGATE);
    /*_BUS_CPU_D04p*/ reg_new.cpu_dbus.BUS_CPU_D04p.tri_bus(AVEK_NR10_SWEEP_PERIOD0);
    /*_BUS_CPU_D05p*/ reg_new.cpu_dbus.BUS_CPU_D05p.tri_bus(AKUX_NR10_SWEEP_PERIOD1);
    /*_BUS_CPU_D06p*/ reg_new.cpu_dbus.BUS_CPU_D06p.tri_bus(AWOS_NR10_SWEEP_PERIOD2);
  }

  {
    /*#p11.BEXU*/ wire BEXU_NR11_RDn = nand2(reg_new.cpu_signals.BUWA_CPU_RDp(), reg_new.cpu_abus.CAXE_ADDR_FF11p());
    /*#p11.BOWO*/ triwire BOWO = tri6_nn(BEXU_NR11_RDn, reg_new.ch1.CENA_NR11_DUTY0p.qn_newB());
    /*#p11.CUDA*/ triwire CUDA = tri6_nn(BEXU_NR11_RDn, reg_new.ch1.DYCA_NR11_DUTY1p.qn_newB());

    /*_BUS_CPU_D06p*/ reg_new.cpu_dbus.BUS_CPU_D06p.tri_bus(BOWO);
    /*_BUS_CPU_D07p*/ reg_new.cpu_dbus.BUS_CPU_D07p.tri_bus(CUDA);
  }

  {
    /*#p11.HAMY*/ wire HAMY_NR12_RDn = or2(reg_new.cpu_abus.HAXE_ADDR_FF12n(), reg_new.cpu_signals.AGUZ_CPU_RDn());
    /*#p11.HOCU*/ wire HOCU_NR12_RDn = or2(reg_new.cpu_abus.GAGO_ADDR_FF12n(), reg_new.cpu_signals.AGUZ_CPU_RDn());

    /*#p11.JYNE*/ triwire JYNE = tri6_nn(HAMY_NR12_RDn, reg_new.ch1.JUSA_NR12_PERIOD0p .qn_newB());
    /*#p11.JACA*/ triwire JACA = tri6_nn(HAMY_NR12_RDn, reg_new.ch1.JUZY_NR12_PERIOD1p .qn_newB());
    /*#p11.JOKU*/ triwire JOKU = tri6_nn(HAMY_NR12_RDn, reg_new.ch1.JOMA_NR12_PERIOD2p .qn_newB());
    /*#p11.HONO*/ triwire HONO = tri6_nn(HOCU_NR12_RDn, reg_new.ch1.JAFY_NR12_ENV_DIRp.qn_newB());
    /*#p11.HOWU*/ triwire HOWU = tri6_nn(HOCU_NR12_RDn, reg_new.ch1.JATY_NR12_VOL0p   .qn_newB());
    /*#p11.HEWA*/ triwire HEWA = tri6_nn(HOCU_NR12_RDn, reg_new.ch1.JAXO_NR12_VOL1p   .qn_newB());
    /*#p11.HEVE*/ triwire HEVE = tri6_nn(HOCU_NR12_RDn, reg_new.ch1.JENA_NR12_VOL2p   .qn_newB());
    /*#p11.JYSE*/ triwire JYSE = tri6_nn(HOCU_NR12_RDn, reg_new.ch1.JOPU_NR12_VOL3p   .qn_newB());

    /*_BUS_CPU_D00p*/ reg_new.cpu_dbus.BUS_CPU_D00p.tri_bus(JYNE);
    /*_BUS_CPU_D01p*/ reg_new.cpu_dbus.BUS_CPU_D01p.tri_bus(JACA);
    /*_BUS_CPU_D02p*/ reg_new.cpu_dbus.BUS_CPU_D02p.tri_bus(JOKU);
    /*_BUS_CPU_D03p*/ reg_new.cpu_dbus.BUS_CPU_D03p.tri_bus(HONO);
    /*_BUS_CPU_D04p*/ reg_new.cpu_dbus.BUS_CPU_D04p.tri_bus(HOWU);
    /*_BUS_CPU_D05p*/ reg_new.cpu_dbus.BUS_CPU_D05p.tri_bus(HEWA);
    /*_BUS_CPU_D06p*/ reg_new.cpu_dbus.BUS_CPU_D06p.tri_bus(HEVE);
    /*_BUS_CPU_D07p*/ reg_new.cpu_dbus.BUS_CPU_D07p.tri_bus(JYSE);
  }

  // Frequency can be read from FF13/FF14 in debug mode

  {
    /*#p11.DYPU*/ wire DYPU_FF13_RDn = nor2(reg_new.cpu_abus.CACA_ADDR_FF13n(), reg_new.DAXA_CPU_RDn_DBGn());
    /*#p11.EVAJ*/ wire EVAJ_FF13_RDa = not1(DYPU_FF13_RDn);

    /*_p11.FORU*/ triwire FORU = tri6_nn(EVAJ_FF13_RDa, reg_new.ch1.GAXE_CH1_FREQ_CNT_00.qn_new());
    /*_p11.GEFU*/ triwire GEFU = tri6_nn(EVAJ_FF13_RDa, reg_new.ch1.HYFE_CH1_FREQ_CNT_01.qn_new());
    /*_p11.KYVU*/ triwire KYVU = tri6_nn(EVAJ_FF13_RDa, reg_new.ch1.JYTY_CH1_FREQ_CNT_02.qn_new());
    /*_p11.KUMO*/ triwire KUMO = tri6_nn(EVAJ_FF13_RDa, reg_new.ch1.KYNA_CH1_FREQ_CNT_03.qn_new());
    /*_p11.KARY*/ triwire KARY = tri6_nn(EVAJ_FF13_RDa, reg_new.ch1.JEMA_CH1_FREQ_CNT_04.qn_new());
    /*_p11.GODE*/ triwire GODE = tri6_nn(EVAJ_FF13_RDa, reg_new.ch1.HYKE_CH1_FREQ_CNT_05.qn_new());
    /*_p11.GOJE*/ triwire GOJE = tri6_nn(EVAJ_FF13_RDa, reg_new.ch1.FEVA_CH1_FREQ_CNT_06.qn_new());
    /*_p11.FOZE*/ triwire FOZE = tri6_nn(EVAJ_FF13_RDa, reg_new.ch1.EKOV_CH1_FREQ_CNT_07.qn_new());

    /*_BUS_CPU_D00p*/ reg_new.cpu_dbus.BUS_CPU_D00p.tri_bus(FORU);
    /*_BUS_CPU_D01p*/ reg_new.cpu_dbus.BUS_CPU_D01p.tri_bus(GEFU);
    /*_BUS_CPU_D02p*/ reg_new.cpu_dbus.BUS_CPU_D02p.tri_bus(KYVU);
    /*_BUS_CPU_D03p*/ reg_new.cpu_dbus.BUS_CPU_D03p.tri_bus(KUMO);
    /*_BUS_CPU_D04p*/ reg_new.cpu_dbus.BUS_CPU_D04p.tri_bus(KARY);
    /*_BUS_CPU_D05p*/ reg_new.cpu_dbus.BUS_CPU_D05p.tri_bus(GODE);
    /*_BUS_CPU_D06p*/ reg_new.cpu_dbus.BUS_CPU_D06p.tri_bus(GOJE);
    /*_BUS_CPU_D07p*/ reg_new.cpu_dbus.BUS_CPU_D07p.tri_bus(FOZE);
  }

  {
    /*#p11.DUPY*/ wire DUPY_FF14_RDn = or2(reg_new.cpu_abus.CURE_ADDR_FF14n(), reg_new.DAXA_CPU_RDn_DBGn());
    /*_p11.DOPA*/ triwire DOPA = tri6_nn(DUPY_FF14_RDn, reg_new.ch1.EMUS_CH1_FREQ_CNT_08.qn_new());
    /*_p11.DEMU*/ triwire DEMU = tri6_nn(DUPY_FF14_RDn, reg_new.ch1.EVAK_CH1_FREQ_CNT_09.qn_new());
    /*_p11.DEXO*/ triwire DEXO = tri6_nn(DUPY_FF14_RDn, reg_new.ch1.COPU_CH1_FREQ_CNT_10.qn_new());

    /*_BUS_CPU_D00p*/ reg_new.cpu_dbus.BUS_CPU_D00p.tri_bus(DOPA);
    /*_BUS_CPU_D01p*/ reg_new.cpu_dbus.BUS_CPU_D01p.tri_bus(DEMU);
    /*_BUS_CPU_D02p*/ reg_new.cpu_dbus.BUS_CPU_D02p.tri_bus(DEXO);

    /*#p11.BALE*/ wire BALE_NR14_RDn = nand2(reg_new.cpu_abus.DUJA_ADDR_FF14p(), reg_new.cpu_signals.BUDA_CPU_RDp());
    /*#p11.BYTU*/ triwire BYTU = tri6_nn(BALE_NR14_RDn, reg_new.ch1.BOKO_NR14_LENENp.qn_newB());
    /*_BUS_CPU_D06p*/ reg_new.cpu_dbus.BUS_CPU_D06p.tri_bus(BYTU);
  }

}

#endif
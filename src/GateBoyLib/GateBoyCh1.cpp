#include "GateBoySPU.h"

#include "GateBoyLib/GateBoy.h"

#ifdef SIM_AUDIO

int ch1_audio_out(const SpuChannel1& ch1) {
  /*#p13.COWE*/ wire COWE_BIT_OUTp = and2(ch1.CYTO_CH1_ACTIVEp.state & 1, ch1.DUWO_RAW_BIT_SYNCp.state & 1);

  ///*_p09.EDEK*/ wire EDEK_NR52_DBG_APUp = not1(reg_new.spu.FERO_NR52_DBG_APUp.qn_any());
  ///*#p13.BOTO*/ wire BOTO_BIT_OUTp = or2(COWE_BIT_OUTp, EDEK_NR52_DBG_APUp);
  wire BOTO_BIT_OUTp = COWE_BIT_OUTp;

  /*#p13.AMOP*/ wire AMOP_CH1_OUT0 = and2(ch1.HAFO_CH1_ENV0p.state & 1, BOTO_BIT_OUTp);
  /*#p13.ASON*/ wire ASON_CH1_OUT1 = and2(ch1.HEMY_CH1_ENV1p.state & 1, BOTO_BIT_OUTp);
  /*#p13.AGOF*/ wire AGOF_CH1_OUT2 = and2(ch1.HOKO_CH1_ENV2p.state & 1, BOTO_BIT_OUTp);
  /*#p13.ACEG*/ wire ACEG_CH1_OUT3 = and2(ch1.HEVO_CH1_ENV3p.state & 1, BOTO_BIT_OUTp);

  return ((AMOP_CH1_OUT0 & 1) << 0) |
         ((ASON_CH1_OUT1 & 1) << 1) |
         ((AGOF_CH1_OUT2 & 1) << 2) |
         ((ACEG_CH1_OUT3 & 1) << 3);
}


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
  BEXA_SWEEP_TRIGp.state = 0x1a;
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












//==============================================================================


void tick_ch1(const GateBoyState& reg_old, GateBoyState& reg_new) {
  auto& ch1_old  = reg_old.ch1;
  auto& dbus_old = reg_old.cpu_dbus;

  auto& ch1_new  = reg_new.ch1;
  auto& dbus_new = reg_new.cpu_dbus;
  auto& abus_new = reg_new.cpu_abus;

  /*_p09.AGUZ*/ wire AGUZ_CPU_RDn = not1(reg_new.cpu_signals.TEDO_CPU_RDp.qp_new());
  /*_p10.BAFU*/ wire BAFU_CPU_WRn = not1(reg_new.cpu_signals.TAPU_CPU_WRp.qp_new());
  /*_p10.BOGY*/ wire BOGY_CPU_WRp = not1(BAFU_CPU_WRn);

  /*_p10.DYTE*/ wire DYTE_A00n = not1(abus_new.BUS_CPU_A00p.out_new());
  /*_p10.AFOB*/ wire AFOB_A01n = not1(abus_new.BUS_CPU_A01p.out_new());
  /*_p10.ABUB*/ wire ABUB_A02n = not1(abus_new.BUS_CPU_A02p.out_new());
  /*_p10.ACOL*/ wire ACOL_A03n = not1(abus_new.BUS_CPU_A03p.out_new());
  /*#p10.ATUP*/ wire ATUP_A04n = not1(abus_new.BUS_CPU_A04p.out_new());
  /*#p10.BOXY*/ wire BOXY_A05n = not1(abus_new.BUS_CPU_A05p.out_new());
  /*#p10.ASAD*/ wire ASAD_A06n = not1(abus_new.BUS_CPU_A06p.out_new());
  /*#p10.AVUN*/ wire AVUN_A07n = not1(abus_new.BUS_CPU_A07p.out_new());
  /*_p10.DOSO*/ wire DOSO_A00p = not1(DYTE_A00n);
  /*_p10.DUPA*/ wire DUPA_A01p = not1(AFOB_A01n);
  /*_p10.DENO*/ wire DENO_A02p = not1(ABUB_A02n);
  /*#p10.DUCE*/ wire DUCE_A03p = not1(ACOL_A03n);

  /*_p07.BAKO*/ wire BAKO_ADDR_FFXXn = not1(abus_new.SYKE_ADDR_HIp_new());
  /*_p10.ATEG*/ wire ATEG_ADDR_XX1Xn = or4(abus_new.BUS_CPU_A07p.out_new(), abus_new.BUS_CPU_A06p.out_new(), abus_new.BUS_CPU_A05p.out_new(), ATUP_A04n);
  /*_p10.BUNO*/ wire BUNO_ADDR_FF1Xp = nor2(BAKO_ADDR_FFXXn, ATEG_ADDR_XX1Xn);
  /*_p10.BANU*/ wire BANU_ADDR_FF1Xn = not1(BUNO_ADDR_FF1Xp);

  /*#p10.DUPO*/ wire DUPO_ADDR_0000n = nand4(ACOL_A03n, ABUB_A02n, AFOB_A01n, DYTE_A00n);
  /*#p10.DUNO*/ wire DUNO_ADDR_0001n = nand4(ACOL_A03n, ABUB_A02n, AFOB_A01n, DOSO_A00p);
  /*#p10.DAMY*/ wire DAMY_ADDR_0010n = nand4(ACOL_A03n, ABUB_A02n, DUPA_A01p, DYTE_A00n);
  /*#p10.ETUF*/ wire ETUF_ADDR_0011n = nand4(ACOL_A03n, ABUB_A02n, DUPA_A01p, DOSO_A00p);
  /*#p10.ESOT*/ wire ESOT_ADDR_0100n = nand4(ACOL_A03n, DENO_A02p, AFOB_A01n, DYTE_A00n);

  /*#p10.DYVA*/ wire DYVA_ADDR_FF10p = nor2(BANU_ADDR_FF1Xn, DUPO_ADDR_0000n);
  /*#p10.CAXE*/ wire CAXE_ADDR_FF11p = nor2(BANU_ADDR_FF1Xn, DUNO_ADDR_0001n);
  /*#p10.EDAF*/ wire EDAF_ADDR_FF12p = nor2(BANU_ADDR_FF1Xn, DAMY_ADDR_0010n);
  /*#p10.DECO*/ wire DECO_ADDR_FF13p = nor2(BANU_ADDR_FF1Xn, ETUF_ADDR_0011n);
  /*#p10.DUJA*/ wire DUJA_ADDR_FF14p = nor2(BANU_ADDR_FF1Xn, ESOT_ADDR_0100n);


  auto KEBA_APU_RSTp = reg_new.KEBA_APU_RSTp_new();
  auto AJER_AxxDExxH = reg_new.spu.AJER_AxxDExxH.qp_new();
  auto DOVA_ABCDxxxx = reg_new.sys_clk.DOVA_ABCDxxxx();
  auto FERO_NR52_DBG_APUp = reg_new.spu.FERO_NR52_DBG_APUp.qn_any();
  auto SIG_IN_CPU_DBUS_FREE = reg_new.cpu_signals.SIG_IN_CPU_DBUS_FREE.qp_new();

  auto BYFE_CLK_128 = reg_new.spu.BYFE_CLK_128n();
  auto BUFY_CLK_256 = reg_new.spu.BUFY_CLK_256n();
  auto HORU_CLK_512_new = reg_new.spu.HORU_CLK_512_new();

  /*#p09.DYFA*/ wire DYFA_xBCDExxx_new = not1(ch1_new.CALO_xBCDExxx.qn_new());

  //----------
  // Trigger

  {
    /*#p13.DUKA*/ wire DUKA_APU_RSTn_new = not1(KEBA_APU_RSTp);
    /*#p13.EZEC*/ ch1_new.EZEC_CH1_TRIGp.dff17(DOVA_ABCDxxxx, DUKA_APU_RSTn_new, ch1_old.DUPE_NR14_TRIGp.qp_oldB());

    /*#p13.DOGE*/ wire DOGE_FF14_WRn = nand2(BOGY_CPU_WRp, DUJA_ADDR_FF14p);
    /*#p13.DADO*/ wire DADO_NR14_RSTb = nor2(KEBA_APU_RSTp, ch1_new.EZEC_CH1_TRIGp.qp_new());
    /*#p13.DUPE*/ ch1_new.DUPE_NR14_TRIGp.dff9b(DOGE_FF14_WRn, DADO_NR14_RSTb, dbus_old.BUS_CPU_D07p.qp_old());

    /*#p13.ERUM*/ wire ERUM_APU_RSTn_new =not1(KEBA_APU_RSTp);
    /*#p13.FYTE*/ ch1_new.FYTE_CH1_TRIGp.dff17(DYFA_xBCDExxx_new, ERUM_APU_RSTn_new, ch1_old.FARE_CH1_TRIGp.qp_old());
    /*#p13.FARE*/ ch1_new.FARE_CH1_TRIGp.dff17(DYFA_xBCDExxx_new, ERUM_APU_RSTn_new, ch1_old.FEKU_CH1_TRIGp.qp_old());

    /*#p13.EGET*/ wire EGET_TRIG_RSTn = nor2(KEBA_APU_RSTp, ch1_new.FARE_CH1_TRIGp.qp_new());
    /*#p13.FEKU*/ ch1_new.FEKU_CH1_TRIGp.dff17(DYFA_xBCDExxx_new, EGET_TRIG_RSTn, ch1_old.FYFO_CH1_TRIGn.qn_old()); // schematic wrong?

    /*#p13.GEFE*/ wire GEFE_TRIG_RSTp = not1(EGET_TRIG_RSTn);
    /*#p13.FYFO*/ ch1_new.FYFO_CH1_TRIGn.nor_latch(GEFE_TRIG_RSTp, ch1_new.EZEC_CH1_TRIGp.qp_new());
  }

  //----------
  // Sweep timer

  {
    /*#p11.CENU*/ wire CENU_NR10_WRp = and2(BOGY_CPU_WRp, DYVA_ADDR_FF10p);
    /*#p11.CENU*/ wire CENU_NR10_WRn = not1(CENU_NR10_WRp);
    /*#p09.AGUR*/ wire AGUR_APU_RSTn_new = not1(KEBA_APU_RSTp);
    /*#p11.BANY*/ ch1_new.BANY_NR10_SWEEP_SHIFT0p .dff9b(CENU_NR10_WRn, AGUR_APU_RSTn_new, dbus_old.BUS_CPU_D00p.qp_old());
    /*#p11.ARAX*/ ch1_new.ARAX_NR10_SWEEP_SHIFT1p .dff9b(CENU_NR10_WRn, AGUR_APU_RSTn_new, dbus_old.BUS_CPU_D01p.qp_old());
    /*#p11.ANAZ*/ ch1_new.ANAZ_NR10_SWEEP_SHIFT2p .dff9b(CENU_NR10_WRn, AGUR_APU_RSTn_new, dbus_old.BUS_CPU_D02p.qp_old());
    /*#p11.AVAF*/ ch1_new.AVAF_NR10_SWEEP_DIRp    .dff9b(CENU_NR10_WRn, AGUR_APU_RSTn_new, dbus_old.BUS_CPU_D03p.qp_old());
    /*#p11.ADEK*/ ch1_new.ADEK_NR10_SWEEP_PERIOD0p.dff9b(CENU_NR10_WRn, AGUR_APU_RSTn_new, dbus_old.BUS_CPU_D04p.qp_old());
    /*#p11.BANA*/ ch1_new.BANA_NR10_SWEEP_PERIOD1p.dff9b(CENU_NR10_WRn, AGUR_APU_RSTn_new, dbus_old.BUS_CPU_D05p.qp_old());
    /*#p11.BOTU*/ ch1_new.BOTU_NR10_SWEEP_PERIOD2p.dff9b(CENU_NR10_WRn, AGUR_APU_RSTn_new, dbus_old.BUS_CPU_D06p.qp_old());
  }

  {
    /*#p11.HAFU*/ wire HAFU_NR12_WRp = and2(BOGY_CPU_WRp, EDAF_ADDR_FF12p);
    /*#p11.KYGY*/ wire KYGY_NR12_WRn = not1(HAFU_NR12_WRp);
    /*#p11.GAXU*/ wire GAXU_NR12_WRn = nand2(EDAF_ADDR_FF12p, BOGY_CPU_WRp);

    // another weird dff9 clock
    wire KAGY_NR12_WRp = not1(GAXU_NR12_WRn); // not on schematic?
    wire KAGY_NR12_WRn = not1(KAGY_NR12_WRp); // not on schematic?

    /*#p11.HATO*/ wire HATO_APU_RSTn_new = not1(KEBA_APU_RSTp);

    /*#p11.JUSA*/ ch1_new.JUSA_NR12_PERIOD0p.dff9b(KYGY_NR12_WRn, HATO_APU_RSTn_new, dbus_old.BUS_CPU_D00p.qp_old());
    /*#p11.JUZY*/ ch1_new.JUZY_NR12_PERIOD1p.dff9b(KYGY_NR12_WRn, HATO_APU_RSTn_new, dbus_old.BUS_CPU_D01p.qp_old());
    /*#p11.JOMA*/ ch1_new.JOMA_NR12_PERIOD2p.dff9b(KYGY_NR12_WRn, HATO_APU_RSTn_new, dbus_old.BUS_CPU_D02p.qp_old());
    /*#p11.JAFY*/ ch1_new.JAFY_NR12_ENV_DIRp.dff9b(KAGY_NR12_WRn, HATO_APU_RSTn_new, dbus_old.BUS_CPU_D03p.qp_old());
    /*#p11.JATY*/ ch1_new.JATY_NR12_VOL0p   .dff9b(KAGY_NR12_WRn, HATO_APU_RSTn_new, dbus_old.BUS_CPU_D04p.qp_old());
    /*#p11.JAXO*/ ch1_new.JAXO_NR12_VOL1p   .dff9b(KAGY_NR12_WRn, HATO_APU_RSTn_new, dbus_old.BUS_CPU_D05p.qp_old());
    /*#p11.JENA*/ ch1_new.JENA_NR12_VOL2p   .dff9b(KAGY_NR12_WRn, HATO_APU_RSTn_new, dbus_old.BUS_CPU_D06p.qp_old());
    /*#p11.JOPU*/ ch1_new.JOPU_NR12_VOL3p   .dff9b(KAGY_NR12_WRn, HATO_APU_RSTn_new, dbus_old.BUS_CPU_D07p.qp_old());
  }
















































  
  
  {
    /*#p13.BAVE*/ wire BAVE_SWEEP_ENn_new = and3(ch1_new.BOTU_NR10_SWEEP_PERIOD2p.qn_newB(), ch1_new.BANA_NR10_SWEEP_PERIOD1p.qn_newB(), ch1_new.ADEK_NR10_SWEEP_PERIOD0p.qn_newB());
    /*#p13.BURY*/ wire BURY_SWEEP_TRIG_RSTn = nor2(BAVE_SWEEP_ENn_new, KEBA_APU_RSTp);
    /*#p13.COZE*/ wire COZE_SWEEP_DELAY_MAX_old = and3(ch1_old.CAXY_SWEEP_DELAY2p.qp_old(), ch1_old.CYPU_SWEEP_DELAY1p.qp_old(), ch1_old.CUPO_SWEEP_DELAY0p.qp_old());
    /*#p13.BEXA*/ ch1_new.BEXA_SWEEP_TRIGp.dff17(AJER_AxxDExxH, BURY_SWEEP_TRIG_RSTn, COZE_SWEEP_DELAY_MAX_old);
  }

  {
    /*_p09.ATYV*/ wire ATYV_APU_RSTn_new = not1(KEBA_APU_RSTp);
    /*#p09.BAZA*/ ch1_new.BAZA_DBG_SWEEP_CLK.dff17(AJER_AxxDExxH, ATYV_APU_RSTn_new, reg_old.spu.BOWY_NR52_DBG_SWEEP.qp_old());
    /*_p09.EDEK*/ wire EDEK_NR52_DBG_APUp = not1(FERO_NR52_DBG_APUp);
    /*#p09.CELY*/ wire CELY_SWEEP_DELAY_CLKn = mux2p(EDEK_NR52_DBG_APUp, ch1_new.BAZA_DBG_SWEEP_CLK.qp_new(), BYFE_CLK_128);
    /*#p09.CONE*/ wire CONE_SWEEP_DELAY_CLKp = not1(CELY_SWEEP_DELAY_CLKn);
    /*#p09.CATE*/ wire CATE_SWEEP_DELAY_CLKn = not1(CONE_SWEEP_DELAY_CLKp);

    /*#p13.DAFA*/ wire DAFA_SWEEP_DELAY_LOADn = nor2(ch1_new.BEXA_SWEEP_TRIGp.qp_new(), ch1_new.FEKU_CH1_TRIGp.qp_new());
    /*#p13.CYMU*/ wire CYMU_SWEEP_DELAY_LOADp = not1(DAFA_SWEEP_DELAY_LOADn);

    /*#p13.CUPO*/ ch1_new.CUPO_SWEEP_DELAY0p.dff20(CATE_SWEEP_DELAY_CLKn,                   CYMU_SWEEP_DELAY_LOADp, ch1_new.ADEK_NR10_SWEEP_PERIOD0p.qn_newB());
    /*#p13.CYPU*/ ch1_new.CYPU_SWEEP_DELAY1p.dff20(ch1_new.CUPO_SWEEP_DELAY0p.qp_new(), CYMU_SWEEP_DELAY_LOADp, ch1_new.BANA_NR10_SWEEP_PERIOD1p.qn_newB());
    /*#p13.CAXY*/ ch1_new.CAXY_SWEEP_DELAY2p.dff20(ch1_new.CYPU_SWEEP_DELAY1p.qp_new(), CYMU_SWEEP_DELAY_LOADp, ch1_new.BOTU_NR10_SWEEP_PERIOD2p.qn_newB());
  }

  {
    /*#p13.ATAT*/ wire ATAT_SWEEP_TRIGn = nor2(KEBA_APU_RSTp, ch1_new.BEXA_SWEEP_TRIGp.qp_new());
    /*#p13.COPY*/ wire COPY_SHIFT_DONEp_old = and3(ch1_old.BYRA_SHIFTCNT2.qp_old(), ch1_old.CAJA_SHIFTCNT1.qp_old(), ch1_old.COPA_SHIFTCNT0.qp_old());
    /*#p13.BYTE*/ ch1_new.BYTE_SHIFT_DONEp.dff17(AJER_AxxDExxH, ATAT_SWEEP_TRIGn, COPY_SHIFT_DONEp_old);
  }

  {
    /*#p13.ADAD*/ wire ADAD_SHIFT_DONEp = not1(ch1_new.BYTE_SHIFT_DONEp.qn_new());
    /*#p13.EPUK*/ wire EPUK_STOP_SHIFTINGn = nor2(KEBA_APU_RSTp, ADAD_SHIFT_DONEp);
    /*#p13.EVOL*/ wire EVOL_START_SHIFTINGn = nor2(ch1_new.BEXA_SWEEP_TRIGp.qp_new(), ch1_new.FYTE_CH1_TRIGp.qp_new());
    /*#p13.FEMU*/ ch1_new.FEMU_SHIFTINGn.nand_latch(EPUK_STOP_SHIFTINGn, EVOL_START_SHIFTINGn);
  }

  {
    /*#p13.BUGE*/ wire BUGE_SWEEP_ENp_new = nand3(ch1_new.ANAZ_NR10_SWEEP_SHIFT2p.qn_newB(),
                                                  ch1_new.ARAX_NR10_SWEEP_SHIFT1p.qn_newB(),
                                                  ch1_new.BANY_NR10_SWEEP_SHIFT0p.qn_newB());
    /*#p13.EGYP*/ wire EGYP_SHIFT_CLK_new = nor2(ch1_new.FEMU_SHIFTINGn.qp_new(), DYFA_xBCDExxx_new);
    /*#p13.CELE*/   wire CELE_SWEEP_ENn_new = not1(BUGE_SWEEP_ENp_new);
    /*#p13.DODY*/ wire DODY_SHIFT_CLK_new = nor2(EGYP_SHIFT_CLK_new, CELE_SWEEP_ENn_new); // border color wrong on die
    /*?p13.EGOR*/ wire EGOR_SHIFT_CLK_new = not1(DODY_SHIFT_CLK_new); // This looks like a nor3, but it almost definiteily is a not1.

    /*#p13.DAPU*/ wire DAPU_SHIFT_CLK = not1(EGOR_SHIFT_CLK_new);
    /*#p13.DACU*/ wire DACU_SHIFTCNT_LOADn = nor2(ch1_new.FEKU_CH1_TRIGp.qp_new(), ch1_new.BEXA_SWEEP_TRIGp.qp_new());
    /*#p13.CYLU*/ wire CYLU_SHIFTCNT_LOADp = not1(DACU_SHIFTCNT_LOADn);
    /*#p13.COPA*/ ch1_new.COPA_SHIFTCNT0.dff20(DAPU_SHIFT_CLK,                      CYLU_SHIFTCNT_LOADp, ch1_new.BANY_NR10_SWEEP_SHIFT0p.qn_newB());
    /*#p13.CAJA*/ ch1_new.CAJA_SHIFTCNT1.dff20(ch1_new.COPA_SHIFTCNT0.qp_new(), CYLU_SHIFTCNT_LOADp, ch1_new.ARAX_NR10_SWEEP_SHIFT1p.qn_newB());
    /*#p13.BYRA*/ ch1_new.BYRA_SHIFTCNT2.dff20(ch1_new.CAJA_SHIFTCNT1.qp_new(), CYLU_SHIFTCNT_LOADp, ch1_new.ANAZ_NR10_SWEEP_SHIFT2p.qn_newB());
  }








  // Frequency summer

  /*#p13.ADAD*/ wire ADAD_SHIFT_DONEp_new = not1(ch1_new.BYTE_SHIFT_DONEp.qn_new());
  /*#p13.KABA*/   wire KABA_SUM_RSTp_new = or2(KEBA_APU_RSTp, ch1_new.FEKU_CH1_TRIGp.qp_new());
  /*#p13.KYNY*/ wire KYNY_SUM_RSTn_new = not1(KABA_SUM_RSTp_new);

  /*_p12.GALO*/ ch1_new.GALO_SUM_A00.dff11(ADAD_SHIFT_DONEp_new, KYNY_SUM_RSTn_new, ch1_old.HYKA_NR13_FREQ00.qp_old());
  /*_p12.JODE*/ ch1_new.JODE_SUM_A01.dff11(ADAD_SHIFT_DONEp_new, KYNY_SUM_RSTn_new, ch1_old.JYKA_NR13_FREQ01.qp_old());
  /*_p12.KARE*/ ch1_new.KARE_SUM_A02.dff11(ADAD_SHIFT_DONEp_new, KYNY_SUM_RSTn_new, ch1_old.HAVO_NR13_FREQ02.qp_old());
  /*_p12.JYME*/ ch1_new.JYME_SUM_A03.dff11(ADAD_SHIFT_DONEp_new, KYNY_SUM_RSTn_new, ch1_old.EDUL_NR13_FREQ03.qp_old());
  /*_p12.GYME*/ ch1_new.GYME_SUM_A04.dff11(ADAD_SHIFT_DONEp_new, KYNY_SUM_RSTn_new, ch1_old.FELY_NR13_FREQ04.qp_old());
  /*_p12.FAXO*/ ch1_new.FAXO_SUM_A05.dff11(ADAD_SHIFT_DONEp_new, KYNY_SUM_RSTn_new, ch1_old.HOLU_NR13_FREQ05.qp_old());
  /*_p12.EXAP*/ ch1_new.EXAP_SUM_A06.dff11(ADAD_SHIFT_DONEp_new, KYNY_SUM_RSTn_new, ch1_old.HYXU_NR13_FREQ06.qp_old());
  /*_p12.DELE*/ ch1_new.DELE_SUM_A07.dff11(ADAD_SHIFT_DONEp_new, KYNY_SUM_RSTn_new, ch1_old.HOPO_NR13_FREQ07.qp_old());
  /*_p12.DEXE*/ ch1_new.DEXE_SUM_A08.dff11(ADAD_SHIFT_DONEp_new, KYNY_SUM_RSTn_new, ch1_old.DYGY_NR14_FREQ08.qp_old());
  /*_p12.DOFY*/ ch1_new.DOFY_SUM_A09.dff11(ADAD_SHIFT_DONEp_new, KYNY_SUM_RSTn_new, ch1_old.EVAB_NR14_FREQ09.qp_old());
  /*_p12.DOLY*/ ch1_new.DOLY_SUM_A10.dff11(ADAD_SHIFT_DONEp_new, KYNY_SUM_RSTn_new, ch1_old.AXAN_NR14_FREQ10.qp_old());

  /*#p12.ARYL*/ wire ARYL_NR10_SWEEP_DIRn_old = not1(ch1_old.AVAF_NR10_SWEEP_DIRp.qn_oldB());
  /*#p12.ARYL*/ wire ARYL_NR10_SWEEP_DIRn_new = not1(ch1_new.AVAF_NR10_SWEEP_DIRp.qn_newB());

  /*_p12.GYLO*/ wire GYLO_SHIFTER_FLIP_00_old = xor2(ARYL_NR10_SWEEP_DIRn_old, ch1_old.FABU_CH1_SHIFT00.qp_old());
  /*_p12.GELA*/ wire GELA_SHIFTER_FLIP_01_old = xor2(ARYL_NR10_SWEEP_DIRn_old, ch1_old.JEFA_CH1_SHIFT01.qp_old());
  /*_p12.GOPO*/ wire GOPO_SHIFTER_FLIP_02_old = xor2(ARYL_NR10_SWEEP_DIRn_old, ch1_old.GOGA_CH1_SHIFT02.qp_old());
  /*_p12.HEFY*/ wire HEFY_SHIFTER_FLIP_03_old = xor2(ARYL_NR10_SWEEP_DIRn_old, ch1_old.JOLU_CH1_SHIFT03.qp_old());
  /*_p12.KEFE*/ wire KEFE_SHIFTER_FLIP_04_old = xor2(ARYL_NR10_SWEEP_DIRn_old, ch1_old.JOTA_CH1_SHIFT04.qp_old());
  /*_p12.GOLY*/ wire GOLY_SHIFTER_FLIP_05_old = xor2(ARYL_NR10_SWEEP_DIRn_old, ch1_old.FUDE_CH1_SHIFT05.qp_old());
  /*_p12.FURE*/ wire FURE_SHIFTER_FLIP_06_old = xor2(ARYL_NR10_SWEEP_DIRn_old, ch1_old.FEDO_CH1_SHIFT06.qp_old());
  /*_p12.DYME*/ wire DYME_SHIFTER_FLIP_07_old = xor2(ARYL_NR10_SWEEP_DIRn_old, ch1_old.EXAC_CH1_SHIFT07.qp_old());
  /*_p12.CALE*/ wire CALE_SHIFTER_FLIP_08_old = xor2(ARYL_NR10_SWEEP_DIRn_old, ch1_old.ELUX_CH1_SHIFT08.qp_old());
  /*_p12.DOZY*/ wire DOZY_SHIFTER_FLIP_09_old = xor2(ARYL_NR10_SWEEP_DIRn_old, ch1_old.AGEZ_CH1_SHIFT09.qp_old());
  /*_p12.CULU*/ wire CULU_SHIFTER_FLIP_10_old = xor2(ARYL_NR10_SWEEP_DIRn_old, ch1_old.BEKU_CH1_SHIFT10.qp_old());

  /*_p12.HORA*/ ch1_new.HORA_SUM_B00.dff11(ADAD_SHIFT_DONEp_new, KYNY_SUM_RSTn_new, GYLO_SHIFTER_FLIP_00_old);
  /*_p12.HOPA*/ ch1_new.HOPA_SUM_B01.dff11(ADAD_SHIFT_DONEp_new, KYNY_SUM_RSTn_new, GELA_SHIFTER_FLIP_01_old);
  /*_p12.HELE*/ ch1_new.HELE_SUM_B02.dff11(ADAD_SHIFT_DONEp_new, KYNY_SUM_RSTn_new, GOPO_SHIFTER_FLIP_02_old);
  /*_p12.JAPE*/ ch1_new.JAPE_SUM_B03.dff11(ADAD_SHIFT_DONEp_new, KYNY_SUM_RSTn_new, HEFY_SHIFTER_FLIP_03_old);
  /*_p12.JETE*/ ch1_new.JETE_SUM_B04.dff11(ADAD_SHIFT_DONEp_new, KYNY_SUM_RSTn_new, KEFE_SHIFTER_FLIP_04_old);
  /*_p12.GELE*/ ch1_new.GELE_SUM_B05.dff11(ADAD_SHIFT_DONEp_new, KYNY_SUM_RSTn_new, GOLY_SHIFTER_FLIP_05_old);
  /*_p12.EPYR*/ ch1_new.EPYR_SUM_B06.dff11(ADAD_SHIFT_DONEp_new, KYNY_SUM_RSTn_new, FURE_SHIFTER_FLIP_06_old);
  /*_p12.EDOK*/ ch1_new.EDOK_SUM_B07.dff11(ADAD_SHIFT_DONEp_new, KYNY_SUM_RSTn_new, DYME_SHIFTER_FLIP_07_old);
  /*_p12.DEFA*/ ch1_new.DEFA_SUM_B08.dff11(ADAD_SHIFT_DONEp_new, KYNY_SUM_RSTn_new, CALE_SHIFTER_FLIP_08_old);
  /*_p12.ETER*/ ch1_new.ETER_SUM_B09.dff11(ADAD_SHIFT_DONEp_new, KYNY_SUM_RSTn_new, DOZY_SHIFTER_FLIP_09_old);
  /*_p12.DEVA*/ ch1_new.DEVA_SUM_B10.dff11(ADAD_SHIFT_DONEp_new, KYNY_SUM_RSTn_new, CULU_SHIFTER_FLIP_10_old);

  /*#p12.GUXA*/ Adder GUXA_SUM00 = add3(ch1_new.GALO_SUM_A00.qp_new(), ch1_new.HORA_SUM_B00.qp_new(), ARYL_NR10_SWEEP_DIRn_new);
  /*_p12.HALU*/ Adder HALU_SUM01 = add3(ch1_new.JODE_SUM_A01.qp_new(), ch1_new.HOPA_SUM_B01.qp_new(), GUXA_SUM00.carry);
  /*_p12.JULE*/ Adder JULE_SUM02 = add3(ch1_new.KARE_SUM_A02.qp_new(), ch1_new.HELE_SUM_B02.qp_new(), HALU_SUM01.carry);
  /*_p12.JORY*/ Adder JORY_SUM03 = add3(ch1_new.JYME_SUM_A03.qp_new(), ch1_new.JAPE_SUM_B03.qp_new(), JULE_SUM02.carry);
  /*_p12.HEXO*/ Adder HEXO_SUM04 = add3(ch1_new.GYME_SUM_A04.qp_new(), ch1_new.JETE_SUM_B04.qp_new(), JORY_SUM03.carry);
  /*_p12.GEVA*/ Adder GEVA_SUM05 = add3(ch1_new.FAXO_SUM_A05.qp_new(), ch1_new.GELE_SUM_B05.qp_new(), HEXO_SUM04.carry);
  /*#p12.FEGO*/ Adder FEGO_SUM06 = add3(ch1_new.EXAP_SUM_A06.qp_new(), ch1_new.EPYR_SUM_B06.qp_new(), GEVA_SUM05.carry);
  /*_p12.ETEK*/ Adder ETEK_SUM07 = add3(ch1_new.DELE_SUM_A07.qp_new(), ch1_new.EDOK_SUM_B07.qp_new(), FEGO_SUM06.carry);
  /*_p12.DYXE*/ Adder DYXE_SUM08 = add3(ch1_new.DEXE_SUM_A08.qp_new(), ch1_new.DEFA_SUM_B08.qp_new(), ETEK_SUM07.carry);
  /*_p12.DULE*/ Adder DULE_SUM09 = add3(ch1_new.DOFY_SUM_A09.qp_new(), ch1_new.ETER_SUM_B09.qp_new(), DYXE_SUM08.carry);
  /*_p12.CORU*/ Adder CORU_SUM10 = add3(ch1_new.DOLY_SUM_A10.qp_new(), ch1_new.DEVA_SUM_B10.qp_new(), DULE_SUM09.carry);

  /*#p12.BYLE*/ wire BYLE_FREQ_OVERFLOWn_new = nor2(ARYL_NR10_SWEEP_DIRn_new, CORU_SUM10.carry);
  /*#p12.ATYS*/ wire ATYS_FREQ_OVERFLOWn_new = or2(BYLE_FREQ_OVERFLOWn_new, ARYL_NR10_SWEEP_DIRn_new);



  // Frequency register

  {
    /*#p13.BUGE*/ wire BUGE_SWEEP_ENp_new = nand3(ch1_new.ANAZ_NR10_SWEEP_SHIFT2p.qn_newB(),
                                                  ch1_new.ARAX_NR10_SWEEP_SHIFT1p.qn_newB(),
                                                  ch1_new.BANY_NR10_SWEEP_SHIFT0p.qn_newB());

    /*#p12.DEPU*/ wire DEPU_FF13_WRn = nand2(BOGY_CPU_WRp, DECO_ADDR_FF13p);
    /*#p12.DYLA*/ wire DYLA_FF13_WRp = not1(DEPU_FF13_WRn);
    /*#p12.DEBY*/ wire DEBY_FF14_WRp = and2(BOGY_CPU_WRp, DUJA_ADDR_FF14p);

    /*#p13.ATUV*/ wire ATUV_FREQ_CLK = and2(ch1_new.BEXA_SWEEP_TRIGp.qp_new(), ATYS_FREQ_OVERFLOWn_new);
    /*#p13.BOJE*/ wire BOJE_FREQ_CLK = and2(ATUV_FREQ_CLK, BUGE_SWEEP_ENp_new);
    /*#p13.BUSO*/ wire BUSO_FREQ_CLK = and3(ch1_new.BEXA_SWEEP_TRIGp.qp_new(), ATYS_FREQ_OVERFLOWn_new, BUGE_SWEEP_ENp_new);

    /*#p12.GYLU*/ wire GYLU_FREQ00_SET = nand2(DYLA_FF13_WRp, dbus_new.BUS_CPU_D00p.qp_new());
    /*#p12.GETA*/ wire GETA_FREQ01_SET = nand2(DYLA_FF13_WRp, dbus_new.BUS_CPU_D01p.qp_new());
    /*#p12.GOLO*/ wire GOLO_FREQ02_SET = nand2(DYLA_FF13_WRp, dbus_new.BUS_CPU_D02p.qp_new());
    /*#p12.GOPE*/ wire GOPE_FREQ03_SET = nand2(DYLA_FF13_WRp, dbus_new.BUS_CPU_D03p.qp_new());
    /*#p12.KOVU*/ wire KOVU_FREQ04_SET = nand2(DYLA_FF13_WRp, dbus_new.BUS_CPU_D04p.qp_new());
    /*#p12.KYPA*/ wire KYPA_FREQ05_SET = nand2(DYLA_FF13_WRp, dbus_new.BUS_CPU_D05p.qp_new());
    /*#p12.ELER*/ wire ELER_FREQ06_SET = nand2(DYLA_FF13_WRp, dbus_new.BUS_CPU_D06p.qp_new());
    /*#p12.ETOL*/ wire ETOL_FREQ07_SET = nand2(DYLA_FF13_WRp, dbus_new.BUS_CPU_D07p.qp_new());
    /*#p12.BUGU*/ wire BUGU_FREQ08_SET = nand2(DEBY_FF14_WRp, dbus_new.BUS_CPU_D00p.qp_new());
    /*#p12.BUDO*/ wire BUDO_FREQ09_SET = nand2(DEBY_FF14_WRp, dbus_new.BUS_CPU_D01p.qp_new());
    /*#p12.AFEG*/ wire AFEG_FREQ10_SET = nand2(DEBY_FF14_WRp, dbus_new.BUS_CPU_D02p.qp_new());

    /*#p12.DEKE*/ wire DEKE = not1(dbus_new.BUS_CPU_D00p.qp_new());
    /*#p12.GULU*/ wire GULU = not1(dbus_new.BUS_CPU_D01p.qp_new());
    /*#p12.FULE*/ wire FULE = not1(dbus_new.BUS_CPU_D02p.qp_new());
    /*#p12.ETUV*/ wire ETUV = not1(dbus_new.BUS_CPU_D03p.qp_new());
    /*#p12.KOPU*/ wire KOPU = not1(dbus_new.BUS_CPU_D04p.qp_new());
    /*#p12.JULO*/ wire JULO = not1(dbus_new.BUS_CPU_D05p.qp_new());
    /*#p12.DYLU*/ wire DYLU = not1(dbus_new.BUS_CPU_D06p.qp_new());
    /*#p12.DULO*/ wire DULO = not1(dbus_new.BUS_CPU_D07p.qp_new());
    /*#p12.BYSU*/ wire BYSU = not1(dbus_new.BUS_CPU_D00p.qp_new());
    /*#p12.BOFU*/ wire BOFU = not1(dbus_new.BUS_CPU_D01p.qp_new());
    /*#p12.BYFU*/ wire BYFU = not1(dbus_new.BUS_CPU_D02p.qp_new());

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

    /*#p12.EFOR*/ wire EFOR_FREQ00_RST = nor2(EJYF, KEBA_APU_RSTp); 
    /*#p12.GATO*/ wire GATO_FREQ01_RST = nor2(FOPU, KEBA_APU_RSTp); 
    /*#p12.GYFU*/ wire GYFU_FREQ02_RST = nor2(FOKE, KEBA_APU_RSTp); 
    /*#p12.GAMO*/ wire GAMO_FREQ03_RST = nor2(FEGA, KEBA_APU_RSTp); 
    /*#p12.KAPO*/ wire KAPO_FREQ04_RST = nor2(KAVO, KEBA_APU_RSTp); 
    /*#p12.KAJU*/ wire KAJU_FREQ05_RST = nor2(KYFU, KEBA_APU_RSTp); 
    /*#p12.ELUF*/ wire ELUF_FREQ06_RST = nor2(ETOK, KEBA_APU_RSTp); 
    /*#p12.ESEL*/ wire ESEL_FREQ07_RST = nor2(EMAR, KEBA_APU_RSTp); 
    /*#p12.BOXU*/ wire BOXU_FREQ08_RST = nor2(BASO, KEBA_APU_RSTp); 
    /*#p12.BOVU*/ wire BOVU_FREQ09_RST = nor2(AMAC, KEBA_APU_RSTp); 
    /*#p12.APAJ*/ wire APAJ_FREQ10_RST = nor2(AJUX, KEBA_APU_RSTp); 

    /*_p12.HYKA*/ ch1_new.HYKA_NR13_FREQ00.dff22(BOJE_FREQ_CLK, GYLU_FREQ00_SET, EFOR_FREQ00_RST, GUXA_SUM00.sum);
    /*_p12.JYKA*/ ch1_new.JYKA_NR13_FREQ01.dff22(BOJE_FREQ_CLK, GETA_FREQ01_SET, GATO_FREQ01_RST, HALU_SUM01.sum);
    /*_p12.HAVO*/ ch1_new.HAVO_NR13_FREQ02.dff22(BOJE_FREQ_CLK, GOLO_FREQ02_SET, GYFU_FREQ02_RST, JULE_SUM02.sum);
    /*_p12.EDUL*/ ch1_new.EDUL_NR13_FREQ03.dff22(BOJE_FREQ_CLK, GOPE_FREQ03_SET, GAMO_FREQ03_RST, JORY_SUM03.sum);
    /*_p12.FELY*/ ch1_new.FELY_NR13_FREQ04.dff22(BOJE_FREQ_CLK, KOVU_FREQ04_SET, KAPO_FREQ04_RST, HEXO_SUM04.sum);
    /*_p12.HOLU*/ ch1_new.HOLU_NR13_FREQ05.dff22(BOJE_FREQ_CLK, KYPA_FREQ05_SET, KAJU_FREQ05_RST, GEVA_SUM05.sum);
    /*_p12.HYXU*/ ch1_new.HYXU_NR13_FREQ06.dff22(BOJE_FREQ_CLK, ELER_FREQ06_SET, ELUF_FREQ06_RST, FEGO_SUM06.sum);
    /*_p12.HOPO*/ ch1_new.HOPO_NR13_FREQ07.dff22(BOJE_FREQ_CLK, ETOL_FREQ07_SET, ESEL_FREQ07_RST, ETEK_SUM07.sum);
    /*_p12.DYGY*/ ch1_new.DYGY_NR14_FREQ08.dff22(BUSO_FREQ_CLK, BUGU_FREQ08_SET, BOXU_FREQ08_RST, DYXE_SUM08.sum);
    /*_p12.EVAB*/ ch1_new.EVAB_NR14_FREQ09.dff22(BUSO_FREQ_CLK, BUDO_FREQ09_SET, BOVU_FREQ09_RST, DULE_SUM09.sum);
    /*_p12.AXAN*/ ch1_new.AXAN_NR14_FREQ10.dff22(BUSO_FREQ_CLK, AFEG_FREQ10_SET, APAJ_FREQ10_RST, CORU_SUM10.sum);
  }

  // Shifter

  {
    /*#p13.KALA*/ wire KALA_SHIFTER_LOADn = nor2(ch1_new.BEXA_SWEEP_TRIGp.qp_new(), ch1_new.FEKU_CH1_TRIGp.qp_new());
    /*#p12.KAPE*/ wire KAPE_SHIFTER_LOADp = not1(KALA_SHIFTER_LOADn);
    /*#p12.JUJU*/ wire JUJU_SHIFTER_LOADp = not1(KALA_SHIFTER_LOADn);
    /*#p12.KEDO*/ wire KEDO_SHIFTER_LOADp = not1(KALA_SHIFTER_LOADn);

    /*_p12.JUTA*/ wire JUTA_CH1_FREQ00n = not1(ch1_new.HYKA_NR13_FREQ00.qp_new());
    /*_p12.HOXE*/ wire HOXE_CH1_FREQ01n = not1(ch1_new.JYKA_NR13_FREQ01.qp_new());
    /*_p12.HUNY*/ wire HUNY_CH1_FREQ02n = not1(ch1_new.HAVO_NR13_FREQ02.qp_new());
    /*_p12.KEKE*/ wire KEKE_CH1_FREQ03n = not1(ch1_new.EDUL_NR13_FREQ03.qp_new());
    /*_p12.KOVO*/ wire KOVO_CH1_FREQ04n = not1(ch1_new.FELY_NR13_FREQ04.qp_new());
    /*_p12.FOHY*/ wire FOHY_CH1_FREQ05n = not1(ch1_new.HOLU_NR13_FREQ05.qp_new());
    /*_p12.DEBO*/ wire DEBO_CH1_FREQ06n = not1(ch1_new.HYXU_NR13_FREQ06.qp_new());
    /*_p12.CYKY*/ wire CYKY_CH1_FREQ07n = not1(ch1_new.HOPO_NR13_FREQ07.qp_new());
    /*_p12.BYRU*/ wire BYRU_CH1_FREQ08n = not1(ch1_new.DYGY_NR14_FREQ08.qp_new());
    /*_p12.APAT*/ wire APAT_CH1_FREQ09n = not1(ch1_new.EVAB_NR14_FREQ09.qp_new());
    /*_p12.BOJO*/ wire BOJO_CH1_FREQ10n = not1(ch1_new.AXAN_NR14_FREQ10.qp_new());

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

    /*_p12.HOZU*/ wire HOZU_SHIFTER_SETn_00 = nand2(ch1_new.HYKA_NR13_FREQ00.qp_new(), KAPE_SHIFTER_LOADp);
    /*_p12.HOLA*/ wire HOLA_SHIFTER_SETn_01 = nand2(ch1_new.JYKA_NR13_FREQ01.qp_new(), KAPE_SHIFTER_LOADp);
    /*_p12.HAWY*/ wire HAWY_SHIFTER_SETn_02 = nand2(ch1_new.HAVO_NR13_FREQ02.qp_new(), KAPE_SHIFTER_LOADp);
    /*_p12.KYRY*/ wire KYRY_SHIFTER_SETn_03 = nand2(ch1_new.EDUL_NR13_FREQ03.qp_new(), JUJU_SHIFTER_LOADp);
    /*_p12.KOLA*/ wire KOLA_SHIFTER_SETn_04 = nand2(ch1_new.FELY_NR13_FREQ04.qp_new(), JUJU_SHIFTER_LOADp);
    /*_p12.GOVO*/ wire GOVO_SHIFTER_SETn_05 = nand2(ch1_new.HOLU_NR13_FREQ05.qp_new(), JUJU_SHIFTER_LOADp);
    /*_p12.EKEM*/ wire EKEM_SHIFTER_SETn_06 = nand2(ch1_new.HYXU_NR13_FREQ06.qp_new(), JUJU_SHIFTER_LOADp);
    /*_p12.DACE*/ wire DACE_SHIFTER_SETn_07 = nand2(ch1_new.HOPO_NR13_FREQ07.qp_new(), KEDO_SHIFTER_LOADp);
    /*_p12.BEGE*/ wire BEGE_SHIFTER_SETn_08 = nand2(ch1_new.DYGY_NR14_FREQ08.qp_new(), KEDO_SHIFTER_LOADp);
    /*_p12.BESO*/ wire BESO_SHIFTER_SETn_09 = nand2(ch1_new.EVAB_NR14_FREQ09.qp_new(), KEDO_SHIFTER_LOADp);
    /*_p12.BEJU*/ wire BEJU_SHIFTER_SETn_10 = nand2(ch1_new.AXAN_NR14_FREQ10.qp_new(), KEDO_SHIFTER_LOADp);

    /*_p12.JADO*/ wire JADO_SHIFTER_RSTn_00 = nor2(KEBA_APU_RSTp, KOKO_CH1_FREQ00n);
    /*_p12.HOBU*/ wire HOBU_SHIFTER_RSTn_01 = nor2(KEBA_APU_RSTp, JOCY_CH1_FREQ01n);
    /*_p12.HYVU*/ wire HYVU_SHIFTER_RSTn_02 = nor2(KEBA_APU_RSTp, JEHY_CH1_FREQ02n);
    /*_p12.KETO*/ wire KETO_SHIFTER_RSTn_03 = nor2(KEBA_APU_RSTp, KAXY_CH1_FREQ03n);
    /*_p12.KYBO*/ wire KYBO_SHIFTER_RSTn_04 = nor2(KEBA_APU_RSTp, KEVY_CH1_FREQ04n);
    /*_p12.EZUK*/ wire EZUK_SHIFTER_RSTn_05 = nor2(KEBA_APU_RSTp, EVOF_CH1_FREQ05n);
    /*_p12.ENOK*/ wire ENOK_SHIFTER_RSTn_06 = nor2(KEBA_APU_RSTp, EREG_CH1_FREQ06n);
    /*_p12.BEWO*/ wire BEWO_SHIFTER_RSTn_07 = nor2(KEBA_APU_RSTp, BAPU_CH1_FREQ07n);
    /*_p12.AGOR*/ wire AGOR_SHIFTER_RSTn_08 = nor2(KEBA_APU_RSTp, AFUG_CH1_FREQ08n);
    /*_p12.AFUX*/ wire AFUX_SHIFTER_RSTn_09 = nor2(KEBA_APU_RSTp, BUVO_CH1_FREQ09n);
    /*_p12.AVUF*/ wire AVUF_SHIFTER_RSTn_10 = nor2(KEBA_APU_RSTp, AFYR_CH1_FREQ10n);

    /*#p13.BUGE*/ wire BUGE_SWEEP_ENp_new = nand3(ch1_new.ANAZ_NR10_SWEEP_SHIFT2p.qn_newB(),
                                                  ch1_new.ARAX_NR10_SWEEP_SHIFT1p.qn_newB(),
                                                  ch1_new.BANY_NR10_SWEEP_SHIFT0p.qn_newB());
    
    /*#p13.EGYP*/ wire EGYP_SHIFT_CLK_new = nor2(ch1_new.FEMU_SHIFTINGn.qp_new(), DYFA_xBCDExxx_new);

    /*#p13.CELE*/   wire CELE_SWEEP_ENn_new = not1(BUGE_SWEEP_ENp_new);
    /*#p13.DODY*/ wire DODY_SHIFT_CLK_new = nor2(EGYP_SHIFT_CLK_new, CELE_SWEEP_ENn_new); // border color wrong on die
    /*?p13.EGOR*/ wire EGOR_SHIFT_CLK_new = not1(DODY_SHIFT_CLK_new); // This looks like a nor3, but it almost definiteily is a not1.
    /*#p12.FAJA*/ wire FAJA_SHIFT_CLK_new = not1(EGOR_SHIFT_CLK_new);
    /*#p12.EJYB*/ wire EJYB_SHIFT_CLK_new = not1(FAJA_SHIFT_CLK_new);
    /*#p12.CYBE*/ wire CYBE_SHIFT_CLK_new = not1(EJYB_SHIFT_CLK_new);
    /*#p12.BECY*/ wire BECY_SHIFT_CLK_new = not1(CYBE_SHIFT_CLK_new);

    /*#p12.FABU*/ ch1_new.FABU_CH1_SHIFT00.dff22(EGOR_SHIFT_CLK_new, HOZU_SHIFTER_SETn_00, JADO_SHIFTER_RSTn_00, ch1_old.JEFA_CH1_SHIFT01.qp_old());
    /*_p12.JEFA*/ ch1_new.JEFA_CH1_SHIFT01.dff22(EGOR_SHIFT_CLK_new, HOLA_SHIFTER_SETn_01, HOBU_SHIFTER_RSTn_01, ch1_old.GOGA_CH1_SHIFT02.qp_old());
    /*_p12.GOGA*/ ch1_new.GOGA_CH1_SHIFT02.dff22(EGOR_SHIFT_CLK_new, HAWY_SHIFTER_SETn_02, HYVU_SHIFTER_RSTn_02, ch1_old.JOLU_CH1_SHIFT03.qp_old());
    /*_p12.JOLU*/ ch1_new.JOLU_CH1_SHIFT03.dff22(EJYB_SHIFT_CLK_new, KYRY_SHIFTER_SETn_03, KETO_SHIFTER_RSTn_03, ch1_old.JOTA_CH1_SHIFT04.qp_old());
    /*_p12.JOTA*/ ch1_new.JOTA_CH1_SHIFT04.dff22(EJYB_SHIFT_CLK_new, KOLA_SHIFTER_SETn_04, KYBO_SHIFTER_RSTn_04, ch1_old.FUDE_CH1_SHIFT05.qp_old());
    /*_p12.FUDE*/ ch1_new.FUDE_CH1_SHIFT05.dff22(EJYB_SHIFT_CLK_new, GOVO_SHIFTER_SETn_05, EZUK_SHIFTER_RSTn_05, ch1_old.FEDO_CH1_SHIFT06.qp_old());
    /*_p12.FEDO*/ ch1_new.FEDO_CH1_SHIFT06.dff22(EJYB_SHIFT_CLK_new, EKEM_SHIFTER_SETn_06, ENOK_SHIFTER_RSTn_06, ch1_old.EXAC_CH1_SHIFT07.qp_old());
    /*_p12.EXAC*/ ch1_new.EXAC_CH1_SHIFT07.dff22(BECY_SHIFT_CLK_new, DACE_SHIFTER_SETn_07, BEWO_SHIFTER_RSTn_07, ch1_old.ELUX_CH1_SHIFT08.qp_old());
    /*_p12.ELUX*/ ch1_new.ELUX_CH1_SHIFT08.dff22(BECY_SHIFT_CLK_new, BEGE_SHIFTER_SETn_08, AGOR_SHIFTER_RSTn_08, ch1_old.AGEZ_CH1_SHIFT09.qp_old());
    /*_p12.AGEZ*/ ch1_new.AGEZ_CH1_SHIFT09.dff22(BECY_SHIFT_CLK_new, BESO_SHIFTER_SETn_09, AFUX_SHIFTER_RSTn_09, ch1_old.BEKU_CH1_SHIFT10.qp_old());
    /*_p12.BEKU*/ ch1_new.BEKU_CH1_SHIFT10.dff22(BECY_SHIFT_CLK_new, BEJU_SHIFTER_SETn_10, AVUF_SHIFTER_RSTn_10, 0);
  }
















































  // This goes to all the CHN_LENENp registers...why?
  /*_p16.ANUJ*/ wire ANUJ_CPU_WR_WEIRD = and2(SIG_IN_CPU_DBUS_FREE, BOGY_CPU_WRp);
  /*#p11.BAGE*/ wire BAGE_NR14_WRn = nand2(ANUJ_CPU_WR_WEIRD, DUJA_ADDR_FF14p);
  /*#p11.CAMY*/ wire CAMY_APU_RSTn_new = not1(KEBA_APU_RSTp);
  /*#p11.BOKO*/ ch1_new.BOKO_NR14_LENENp.dff9b(BAGE_NR14_WRn, CAMY_APU_RSTn_new, dbus_old.BUS_CPU_D06p.qp_old());

  /*#p13.BORO*/ wire BORO_FF11_WRn = nand2(BOGY_CPU_WRp, CAXE_ADDR_FF11p);
  /*#p13.BEPE*/ wire BEPE_FF11_WRp = not1(BORO_FF11_WRn);
  /*#p13.BUGY*/ wire BUGY_FF11_WRp = not1(BORO_FF11_WRn);
  /*#p13.BOKA*/ wire BOKA_FF11_WRp = not1(BORO_FF11_WRn);








  {
    /*#p13.CAPY*/ wire CAPY_CH1_LEN_CLKn = nor3(ch1_old.CERO_CH1_LEN_DONE.qp_old(), BUFY_CLK_256, ch1_new.BOKO_NR14_LENENp.qn_newB());
    /*#p13.CANU*/ wire CANU_CH1_LEN_CLKp = not1(CAPY_CH1_LEN_CLKn);
    /*#p13.BACY*/ ch1_new.BACY_NR11_LEN0.dff20(CANU_CH1_LEN_CLKp,                     BUGY_FF11_WRp, dbus_new.BUS_CPU_D00p.qp_new());
    /*#p13.CAVY*/ ch1_new.CAVY_NR11_LEN1.dff20(ch1_new.BACY_NR11_LEN0.qp_new(),   BUGY_FF11_WRp, dbus_new.BUS_CPU_D01p.qp_new());
    /*#p13.BOVY*/ ch1_new.BOVY_NR11_LEN2.dff20(ch1_new.CAVY_NR11_LEN1.qp_new(),   BUGY_FF11_WRp, dbus_new.BUS_CPU_D02p.qp_new());
    /*#p13.CUNO*/ ch1_new.CUNO_NR11_LEN3.dff20(ch1_new.BOVY_NR11_LEN2.qp_new(),   BUGY_FF11_WRp, dbus_new.BUS_CPU_D03p.qp_new());
    /*#p13.CUSO*/ wire CUSO_NR11_LEN3p = not1(ch1_new.CUNO_NR11_LEN3.qn_new());
    /*#p13.CURA*/ ch1_new.CURA_NR11_LEN4.dff20(CUSO_NR11_LEN3p,                       BEPE_FF11_WRp, dbus_new.BUS_CPU_D04p.qp_new());
    /*#p13.ERAM*/ ch1_new.ERAM_NR11_LEN5.dff20(ch1_new.CURA_NR11_LEN4.qp_new(),   BEPE_FF11_WRp, dbus_new.BUS_CPU_D05p.qp_new());

    /*#p13.CORY*/ wire CORY_CH1_LEN_DONE_RST_new = nor3(BOKA_FF11_WRp, KEBA_APU_RSTp, ch1_new.FEKU_CH1_TRIGp.qp_new());
    /*_p13.CERO*/ ch1_new.CERO_CH1_LEN_DONE.dff17(ch1_new.ERAM_NR11_LEN5.qn_new(), CORY_CH1_LEN_DONE_RST_new, ch1_old.CERO_CH1_LEN_DONE.qn_old());
  }

  {
    /*#p13.CAPY*/ wire CAPY_CH1_LEN_CLKn = nor3(ch1_new.CERO_CH1_LEN_DONE.qp_new(), BUFY_CLK_256, ch1_new.BOKO_NR14_LENENp.qn_newB());
    /*#p13.CANU*/ wire CANU_CH1_LEN_CLKp = not1(CAPY_CH1_LEN_CLKn);
    /*#p13.BACY*/ ch1_new.BACY_NR11_LEN0.dff20_any(CANU_CH1_LEN_CLKp,                     BUGY_FF11_WRp, dbus_new.BUS_CPU_D00p.qp_new());
    /*#p13.CAVY*/ ch1_new.CAVY_NR11_LEN1.dff20_any(ch1_new.BACY_NR11_LEN0.qp_new(),   BUGY_FF11_WRp, dbus_new.BUS_CPU_D01p.qp_new());
    /*#p13.BOVY*/ ch1_new.BOVY_NR11_LEN2.dff20_any(ch1_new.CAVY_NR11_LEN1.qp_new(),   BUGY_FF11_WRp, dbus_new.BUS_CPU_D02p.qp_new());
    /*#p13.CUNO*/ ch1_new.CUNO_NR11_LEN3.dff20_any(ch1_new.BOVY_NR11_LEN2.qp_new(),   BUGY_FF11_WRp, dbus_new.BUS_CPU_D03p.qp_new());
    /*#p13.CUSO*/ wire CUSO_NR11_LEN3p = not1(ch1_new.CUNO_NR11_LEN3.qn_new());
    /*#p13.CURA*/ ch1_new.CURA_NR11_LEN4.dff20_any(CUSO_NR11_LEN3p,                       BEPE_FF11_WRp, dbus_new.BUS_CPU_D04p.qp_new());
    /*#p13.ERAM*/ ch1_new.ERAM_NR11_LEN5.dff20_any(ch1_new.CURA_NR11_LEN4.qp_new(),   BEPE_FF11_WRp, dbus_new.BUS_CPU_D05p.qp_new());

    /*#p13.CORY*/ wire CORY_CH1_LEN_DONE_RST_new = nor3(BOKA_FF11_WRp, KEBA_APU_RSTp, ch1_new.FEKU_CH1_TRIGp.qp_new());
    /*_p13.CERO*/ ch1_new.CERO_CH1_LEN_DONE.dff17_any(ch1_new.ERAM_NR11_LEN5.qn_new(), CORY_CH1_LEN_DONE_RST_new, ch1_new.CERO_CH1_LEN_DONE.qn_new());
  }

  {
    /*#p13.CAPY*/ wire CAPY_CH1_LEN_CLKn = nor3(ch1_new.CERO_CH1_LEN_DONE.qp_new(), BUFY_CLK_256, ch1_new.BOKO_NR14_LENENp.qn_newB());
    /*#p13.CANU*/ wire CANU_CH1_LEN_CLKp = not1(CAPY_CH1_LEN_CLKn);
    /*#p13.BACY*/ ch1_new.BACY_NR11_LEN0.dff20_any(CANU_CH1_LEN_CLKp,                     BUGY_FF11_WRp, dbus_new.BUS_CPU_D00p.qp_new());
    /*#p13.CAVY*/ ch1_new.CAVY_NR11_LEN1.dff20_any(ch1_new.BACY_NR11_LEN0.qp_new(),   BUGY_FF11_WRp, dbus_new.BUS_CPU_D01p.qp_new());
    /*#p13.BOVY*/ ch1_new.BOVY_NR11_LEN2.dff20_any(ch1_new.CAVY_NR11_LEN1.qp_new(),   BUGY_FF11_WRp, dbus_new.BUS_CPU_D02p.qp_new());
    /*#p13.CUNO*/ ch1_new.CUNO_NR11_LEN3.dff20_any(ch1_new.BOVY_NR11_LEN2.qp_new(),   BUGY_FF11_WRp, dbus_new.BUS_CPU_D03p.qp_new());
    /*#p13.CUSO*/ wire CUSO_NR11_LEN3p = not1(ch1_new.CUNO_NR11_LEN3.qn_new());
    /*#p13.CURA*/ ch1_new.CURA_NR11_LEN4.dff20_any(CUSO_NR11_LEN3p,                       BEPE_FF11_WRp, dbus_new.BUS_CPU_D04p.qp_new());
    /*#p13.ERAM*/ ch1_new.ERAM_NR11_LEN5.dff20_any(ch1_new.CURA_NR11_LEN4.qp_new(),   BEPE_FF11_WRp, dbus_new.BUS_CPU_D05p.qp_new());

    /*#p13.CORY*/ wire CORY_CH1_LEN_DONE_RST_new = nor3(BOKA_FF11_WRp, KEBA_APU_RSTp, ch1_new.FEKU_CH1_TRIGp.qp_new());
    /*_p13.CERO*/ ch1_new.CERO_CH1_LEN_DONE.dff17_any(ch1_new.ERAM_NR11_LEN5.qn_new(), CORY_CH1_LEN_DONE_RST_new, ch1_new.CERO_CH1_LEN_DONE.qn_new());
  }






































  /*#p13.HOCA*/ wire HOCA_CH1_AMP_ENn_new = nor5(ch1_new.JAFY_NR12_ENV_DIRp.qp_newB(),
                                                 ch1_new.JATY_NR12_VOL0p.qp_newB(),
                                                 ch1_new.JAXO_NR12_VOL1p.qp_newB(),
                                                 ch1_new.JENA_NR12_VOL2p.qp_newB(),
                                                 ch1_new.JOPU_NR12_VOL3p.qp_newB());



  {
    /*#p13.BONE*/ wire BONE_FREQ_OVERFLOWp_new = not1(ATYS_FREQ_OVERFLOWn_new); 
    /*#p13.CYFA*/ wire CYFA_LEN_DONEp_new = and2(ch1_new.CERO_CH1_LEN_DONE.qp_new(), ch1_new.BOKO_NR14_LENENp.qp_newB());
    /*#p13.BERY*/ wire BERY_CH1_STOPp_new = or4(BONE_FREQ_OVERFLOWp_new, KEBA_APU_RSTp, CYFA_LEN_DONEp_new, HOCA_CH1_AMP_ENn_new);

    /*#p13.CYTO*/ ch1_new.CYTO_CH1_ACTIVEp.nor_latch(ch1_new.FEKU_CH1_TRIGp.qp_new(), BERY_CH1_STOPp_new);
  }


  // so this implies that NR12_DELAY* is DFFn?
  /*#p13.KOMA*/ wire KOMA_ENV_OFFp_old = nor3(ch1_old.JUSA_NR12_PERIOD0p.qp_oldB(), ch1_old.JUZY_NR12_PERIOD1p.qp_oldB(), ch1_old.JOMA_NR12_PERIOD2p.qp_oldB());
  /*#p13.KOMA*/ wire KOMA_ENV_OFFp_new = nor3(ch1_new.JUSA_NR12_PERIOD0p.qp_newB(), ch1_new.JUZY_NR12_PERIOD1p.qp_newB(), ch1_new.JOMA_NR12_PERIOD2p.qp_newB());


  {
    // When the delay expires, we generate one env tick.

    /*#p13.KOTE*/ wire KOTE_ENV_DELAY_DONEp_old = and3(ch1_old.KERA_ENV_DELAY2p.qp_old(), ch1_old.KENU_ENV_DELAY1p.qp_old(), ch1_old.JOVA_ENV_DELAY0p.qp_old());
    /*#p13.KOZY*/ ch1_new.KOZY_ENV_TICKp.dff17_clk(HORU_CLK_512_new, KOTE_ENV_DELAY_DONEp_old);

    /*#p13.KURY*/ wire KURY_ENV_CLKn_new = not1(ch1_new.KOZY_ENV_TICKp.qp_new());
    /*#p13.KUKU*/ wire KUKU_ENV_CLKp_new = nor2(HORU_CLK_512_new, KURY_ENV_CLKn_new);
    /*#p13.KORO*/ wire KORO_ENV_CLK_RSTn_new = nor4(KUKU_ENV_CLKp_new, KOMA_ENV_OFFp_new, ch1_new.FEKU_CH1_TRIGp.qp_new(), KEBA_APU_RSTp);

    /*#p13.KOZY*/ ch1_new.KOZY_ENV_TICKp.dff17_rst(KORO_ENV_CLK_RSTn_new);
  }


  /*#p13.JONE*/ wire JONE_CLK_128p = not1(BYFE_CLK_128);
  /*#p13.KADO*/ wire KADO_APU_RSTn_new = not1(KEBA_APU_RSTp);
  /*#p13.KALY*/ ch1_new.KALY_ENV_DELAY_CLK_64n.dff17(JONE_CLK_128p, KADO_APU_RSTn_new, ch1_old.KALY_ENV_DELAY_CLK_64n.qn_old());
  /*#p13.KERE*/ wire KERE_ENV_DELAY_CLK_64p = not1(ch1_new.KALY_ENV_DELAY_CLK_64n.qp_new());
  /*#p13.JOLA*/ wire JOLA_ENV_DELAY_CLK_64n = not1(KERE_ENV_DELAY_CLK_64p);

  /*#p13.KAZA*/ wire KAZA_ENV_DELAY_LOADn = nor2(ch1_new.FEKU_CH1_TRIGp.qp_new(), ch1_new.KOZY_ENV_TICKp.qp_new());
  /*#p13.KUXU*/ wire KUXU_ENV_DELAY_LOADp = not1(KAZA_ENV_DELAY_LOADn);

  /*#p13.JOVA*/ ch1_new.JOVA_ENV_DELAY0p.dff20(JOLA_ENV_DELAY_CLK_64n,                KUXU_ENV_DELAY_LOADp, ch1_new.JUSA_NR12_PERIOD0p.qn_newB());
  /*#p13.KENU*/ ch1_new.KENU_ENV_DELAY1p.dff20(ch1_new.JOVA_ENV_DELAY0p.qp_new(), KUXU_ENV_DELAY_LOADp, ch1_new.JUZY_NR12_PERIOD1p.qn_newB());
  /*#p13.KERA*/ ch1_new.KERA_ENV_DELAY2p.dff20(ch1_new.KENU_ENV_DELAY1p.qp_new(), KUXU_ENV_DELAY_LOADp, ch1_new.JOMA_NR12_PERIOD2p.qn_newB());

  {
    // Check to see if our env is 0b0000 or 0b1111
    /*#p13.KORU*/ wire KORU_ENV_MAX_RSTn_new = nor2(ch1_new.FEKU_CH1_TRIGp.qp_new(), KEBA_APU_RSTp);
    /*#p13.HUFU*/ wire HUFU_ENV_TOPn_old = nand5(ch1_old.JAFY_NR12_ENV_DIRp.qp_oldB(), ch1_old.HAFO_CH1_ENV0p.qp_old(), ch1_old.HEMY_CH1_ENV1p.qp_old(), ch1_old.HOKO_CH1_ENV2p.qp_old(), ch1_old.HEVO_CH1_ENV3p.qp_old());
    /*#p13.HANO*/ wire HANO_ENV_BOTp_old = nor5 (ch1_old.JAFY_NR12_ENV_DIRp.qp_oldB(), ch1_old.HAFO_CH1_ENV0p.qp_old(), ch1_old.HEMY_CH1_ENV1p.qp_old(), ch1_old.HOKO_CH1_ENV2p.qp_old(), ch1_old.HEVO_CH1_ENV3p.qp_old());
    
    /*#p13.HAKE*/ wire HAKE_ENV_TOPp_old = not1(HUFU_ENV_TOPn_old);
    /*#p13.JADE*/ wire JADE_ENV_MAXp_old = or2(HANO_ENV_BOTp_old, HAKE_ENV_TOPp_old);
    /*#p13.KYNO*/ ch1_new.KYNO_ENV_MAXp.dff17(ch1_new.KOZY_ENV_TICKp.qp_new(), KORU_ENV_MAX_RSTn_new, JADE_ENV_MAXp_old);
  }

  {
    // If our env maxes out, don't tick the envelope anymore.
    /*#p13.KEKO*/ wire KEKO_ENV_STARTp = or2(KEBA_APU_RSTp, ch1_new.FEKU_CH1_TRIGp.qp_new());
    /*#p13.KEZU*/ ch1_new.KEZU_ENV_ACTIVEn.nor_latch(ch1_new.KYNO_ENV_MAXp.qp_new(), KEKO_ENV_STARTp);
  }


  /*#p13.KAKE*/ wire KAKE_ENV_CLK = or3(ch1_new.KOZY_ENV_TICKp.qp_new(), KOMA_ENV_OFFp_new, ch1_new.KEZU_ENV_ACTIVEn.qp_new()); // Die has this as and, but it's definitely or

  // The muxes select posedge or negedge for the env counter so it can count up _or_ down?
  // these were connected wrong in schematic

  /*#p13.JUFY*/ wire JUFY_ENV_CLK = amux2(ch1_new.JAFY_NR12_ENV_DIRp.qp_newB(), KAKE_ENV_CLK, KAKE_ENV_CLK, ch1_new.JAFY_NR12_ENV_DIRp.qn_newB());
  /*#p13.HAFO*/ ch1_new.HAFO_CH1_ENV0p.dff20(JUFY_ENV_CLK, ch1_new.FEKU_CH1_TRIGp.qp_new(), ch1_new.JATY_NR12_VOL0p.qp_newB());

  /*#p13.HYTO*/ wire HYTO_ENV_CLK = amux2(ch1_new.JAFY_NR12_ENV_DIRp.qp_newB(), ch1_new.HAFO_CH1_ENV0p.qp_new(), ch1_new.HAFO_CH1_ENV0p.qn_new(), ch1_new.JAFY_NR12_ENV_DIRp.qn_newB());
  /*#p13.HEMY*/ ch1_new.HEMY_CH1_ENV1p.dff20(HYTO_ENV_CLK, ch1_new.FEKU_CH1_TRIGp.qp_new(), ch1_new.JAXO_NR12_VOL1p.qp_newB());

  /*#p13.HETO*/ wire HETO_ENV_CLK = amux2(ch1_new.JAFY_NR12_ENV_DIRp.qp_newB(), ch1_new.HEMY_CH1_ENV1p.qp_new(), ch1_new.HEMY_CH1_ENV1p.qn_new(), ch1_new.JAFY_NR12_ENV_DIRp.qn_newB());
  /*#p13.HOKO*/ ch1_new.HOKO_CH1_ENV2p.dff20(HETO_ENV_CLK, ch1_new.FEKU_CH1_TRIGp.qp_new(), ch1_new.JENA_NR12_VOL2p.qp_newB());

  /*#p13.HESU*/ wire HESU_ENV_CLK = amux2(ch1_new.JAFY_NR12_ENV_DIRp.qp_newB(), ch1_new.HOKO_CH1_ENV2p.qp_new(), ch1_new.HOKO_CH1_ENV2p.qn_new(), ch1_new.JAFY_NR12_ENV_DIRp.qn_newB());
  /*#p13.HEVO*/ ch1_new.HEVO_CH1_ENV3p.dff20(HESU_ENV_CLK, ch1_new.FEKU_CH1_TRIGp.qp_new(), ch1_new.JOPU_NR12_VOL3p.qp_newB());

  //----------
  // Waveform generator

  // This is going to be an interesting bit of circular stuff to unwind...

  {
    /*#p13.FEMY*/ wire FEMY_CH2_OFFn_new = nor2(HOCA_CH1_AMP_ENn_new, KEBA_APU_RSTp);
    /*#p13.GEPU*/ wire GEPU_CH1_TRIGn_new = not1(ch1_new.FYTE_CH1_TRIGp.qp_new());
    /*#p13.GEXU*/ ch1_new.GEXU_FREQ_GATEn.nand_latch(FEMY_CH2_OFFn_new, GEPU_CH1_TRIGn_new);
  }

  /*#p11.FULO*/ wire FULO_FREQ_CLK_new = nor2(DYFA_xBCDExxx_new, ch1_new.GEXU_FREQ_GATEn.qp_new());
  /*#p11.GEKU*/ wire GEKU_FREQ_CLK_new = not1(FULO_FREQ_CLK_new);

  // Frequency counter

  //{
    /*#p11.GAXE*/ ch1_new.GAXE_CH1_FREQ_CNT_00.dff20_clk(GEKU_FREQ_CLK_new);
    /*#p11.HYFE*/ ch1_new.HYFE_CH1_FREQ_CNT_01.dff20_clk(ch1_new.GAXE_CH1_FREQ_CNT_00.qp_new());
    /*_p11.JYTY*/ ch1_new.JYTY_CH1_FREQ_CNT_02.dff20_clk(ch1_new.HYFE_CH1_FREQ_CNT_01.qp_new());
    /*_p11.KYNA*/ ch1_new.KYNA_CH1_FREQ_CNT_03.dff20_clk(ch1_new.JYTY_CH1_FREQ_CNT_02.qp_new());

    /*#p11.KYPE*/ wire KYPE_FREQ_CLK_new = not1(ch1_new.KYNA_CH1_FREQ_CNT_03.qn_new());
    /*_p11.JEMA*/ ch1_new.JEMA_CH1_FREQ_CNT_04.dff20_clk(KYPE_FREQ_CLK_new);
    /*_p11.HYKE*/ ch1_new.HYKE_CH1_FREQ_CNT_05.dff20_clk(ch1_new.JEMA_CH1_FREQ_CNT_04.qp_new());
    /*_p11.FEVA*/ ch1_new.FEVA_CH1_FREQ_CNT_06.dff20_clk(ch1_new.HYKE_CH1_FREQ_CNT_05.qp_new());
    /*_p11.EKOV*/ ch1_new.EKOV_CH1_FREQ_CNT_07.dff20_clk(ch1_new.FEVA_CH1_FREQ_CNT_06.qp_new());

    /*#p11.DERU*/ wire DERU_FREQ_CLK_new = not1(ch1_new.EKOV_CH1_FREQ_CNT_07.qn_new());
    /*#p11.EMUS*/ ch1_new.EMUS_CH1_FREQ_CNT_08.dff20_clk(DERU_FREQ_CLK_new);
    /*#p11.EVAK*/ ch1_new.EVAK_CH1_FREQ_CNT_09.dff20_clk(ch1_new.EMUS_CH1_FREQ_CNT_08.qp_new());
    /*#p11.COPU*/ ch1_new.COPU_CH1_FREQ_CNT_10.dff20_clk(ch1_new.EVAK_CH1_FREQ_CNT_09.qp_new());
  //}

  {
    /*#p13.CALA*/ wire CALA_FREQ_OVERFLOWp_new = not1(ch1_new.COPU_CH1_FREQ_CNT_10.qp_new());
    /*_p13.COMY*/ ch1_new.COMY_SAMPLE_CLKp.dff17_clk(CALA_FREQ_OVERFLOWp_new, ch1_old.COMY_SAMPLE_CLKp.qn_old());
  }

  {
    /*#p13.CYTE*/ wire CYTE_SAMPLE_CLKn_new = not1(ch1_new.COMY_SAMPLE_CLKp.qp_new());
    /*#p13.COPE*/ wire COPE_SAMPLE_CLKp_new = not1(CYTE_SAMPLE_CLKn_new);

    /*#p11.EPYK*/ wire EPYK_FREQ_LOADn_new = nor2(COPE_SAMPLE_CLKp_new, ch1_new.FEKU_CH1_TRIGp.qp_new());
    /*#p11.FUME*/ wire FUME_FREQ_LOADp_new = not1(EPYK_FREQ_LOADn_new);
    /*#p11.DEGA*/ wire DEGA_FREQ_LOADp_new = not1(EPYK_FREQ_LOADn_new);
    /*#p11.DAKO*/ wire DAKO_FREQ_LOADp_new = not1(EPYK_FREQ_LOADn_new);

    /*#p11.GAXE*/ ch1_new.GAXE_CH1_FREQ_CNT_00.dff20_any(GEKU_FREQ_CLK_new,                         FUME_FREQ_LOADp_new, ch1_new.HYKA_NR13_FREQ00.qp_new());
    /*#p11.HYFE*/ ch1_new.HYFE_CH1_FREQ_CNT_01.dff20_any(ch1_new.GAXE_CH1_FREQ_CNT_00.qp_new(), FUME_FREQ_LOADp_new, ch1_new.JYKA_NR13_FREQ01.qp_new());
    /*_p11.JYTY*/ ch1_new.JYTY_CH1_FREQ_CNT_02.dff20_any(ch1_new.HYFE_CH1_FREQ_CNT_01.qp_new(), FUME_FREQ_LOADp_new, ch1_new.HAVO_NR13_FREQ02.qp_new());
    /*_p11.KYNA*/ ch1_new.KYNA_CH1_FREQ_CNT_03.dff20_any(ch1_new.JYTY_CH1_FREQ_CNT_02.qp_new(), FUME_FREQ_LOADp_new, ch1_new.EDUL_NR13_FREQ03.qp_new());

    /*#p11.KYPE*/ wire KYPE_FREQ_CLK_new = not1(ch1_new.KYNA_CH1_FREQ_CNT_03.qn_new());
    /*_p11.JEMA*/ ch1_new.JEMA_CH1_FREQ_CNT_04.dff20_any(KYPE_FREQ_CLK_new,                         DEGA_FREQ_LOADp_new, ch1_new.FELY_NR13_FREQ04.qp_new());
    /*_p11.HYKE*/ ch1_new.HYKE_CH1_FREQ_CNT_05.dff20_any(ch1_new.JEMA_CH1_FREQ_CNT_04.qp_new(), DEGA_FREQ_LOADp_new, ch1_new.HOLU_NR13_FREQ05.qp_new());
    /*_p11.FEVA*/ ch1_new.FEVA_CH1_FREQ_CNT_06.dff20_any(ch1_new.HYKE_CH1_FREQ_CNT_05.qp_new(), DEGA_FREQ_LOADp_new, ch1_new.HYXU_NR13_FREQ06.qp_new());
    /*_p11.EKOV*/ ch1_new.EKOV_CH1_FREQ_CNT_07.dff20_any(ch1_new.FEVA_CH1_FREQ_CNT_06.qp_new(), DEGA_FREQ_LOADp_new, ch1_new.HOPO_NR13_FREQ07.qp_new());

    /*#p11.DERU*/ wire DERU_FREQ_CLK_new = not1(ch1_new.EKOV_CH1_FREQ_CNT_07.qn_new());
    /*#p11.EMUS*/ ch1_new.EMUS_CH1_FREQ_CNT_08.dff20_any(DERU_FREQ_CLK_new,                         DAKO_FREQ_LOADp_new, ch1_new.DYGY_NR14_FREQ08.qp_new());
    /*#p11.EVAK*/ ch1_new.EVAK_CH1_FREQ_CNT_09.dff20_any(ch1_new.EMUS_CH1_FREQ_CNT_08.qp_new(), DAKO_FREQ_LOADp_new, ch1_new.EVAB_NR14_FREQ09.qp_new());
    /*#p11.COPU*/ ch1_new.COPU_CH1_FREQ_CNT_10.dff20_any(ch1_new.EVAK_CH1_FREQ_CNT_09.qp_new(), DAKO_FREQ_LOADp_new, ch1_new.AXAN_NR14_FREQ10.qp_new());
  }

  {
    /*#p13.CALA*/ wire CALA_FREQ_OVERFLOWp_new = not1(ch1_new.COPU_CH1_FREQ_CNT_10.qp_new());
    /*#p13.DOKA*/ wire DOKA_COMY_RSTp_new = and2(ch1_new.COMY_SAMPLE_CLKp.qp_new(), DYFA_xBCDExxx_new);
    /*#p13.DYRU*/ wire DYRU_COMY_RSTn_new = nor3(KEBA_APU_RSTp, ch1_new.FEKU_CH1_TRIGp.qp_new(), DOKA_COMY_RSTp_new);
    /*_p13.COMY*/ ch1_new.COMY_SAMPLE_CLKp.dff17_any(CALA_FREQ_OVERFLOWp_new, DYRU_COMY_RSTn_new, ch1_new.COMY_SAMPLE_CLKp.qn_new());
  }

  // Duty cycle generator

  /*#p11.CEPO*/ wire CEPO_APU_RSTn_new = not1(KEBA_APU_RSTp);

  {
    /*#p13.CYTE*/ wire CYTE_SAMPLE_CLKn_new = not1(ch1_new.COMY_SAMPLE_CLKp.qp_new());
    /*#p13.COPE*/ wire COPE_SAMPLE_CLKp_new = not1(CYTE_SAMPLE_CLKn_new);
    /*#p13.DAJO*/ wire DAJO_SAMPLE_CLKn_new = not1(COPE_SAMPLE_CLKp_new);
    /*#p13.ESUT*/ ch1_new.ESUT_PHASE_xBxDxFxH.dff17(DAJO_SAMPLE_CLKn_new,                 CEPO_APU_RSTn_new, ch1_old.ESUT_PHASE_xBxDxFxH.qn_old());
    /*#p13.EROS*/ ch1_new.EROS_PHASE_xxCDxxGH.dff13(ch1_new.ESUT_PHASE_xBxDxFxH.qn_new(), CEPO_APU_RSTn_new, ch1_old.EROS_PHASE_xxCDxxGH.qn_old());
    /*#p13.DAPE*/ ch1_new.DAPE_PHASE_xxxxEFGH.dff13(ch1_new.EROS_PHASE_xxCDxxGH.qn_new(), CEPO_APU_RSTn_new, ch1_old.DAPE_PHASE_xxxxEFGH.qn_old());

    /*#p13.DUVO*/ wire DUVO_PHASE_AxCxExGx_old = not1(ch1_old.ESUT_PHASE_xBxDxFxH.qp_old());
    /*#p13.EZOZ*/ wire EZOZ_PHASE_xxxxxxGH_old = and2(ch1_old.DAPE_PHASE_xxxxEFGH.qp_old(), ch1_old.EROS_PHASE_xxCDxxGH.qp_old());
    /*#p13.ENEK*/ wire ENEK_PHASE_xxxxxxGx_old = and2(EZOZ_PHASE_xxxxxxGH_old, DUVO_PHASE_AxCxExGx_old);
    /*#p13.CODO*/ wire CODO_PHASE_ABCDEFxx_old = not1(EZOZ_PHASE_xxxxxxGH_old);


    /*#p13.COSO*/ wire COSO_old = nor2(ch1_old.CENA_NR11_DUTY0p.qp_oldB(), ch1_old.DYCA_NR11_DUTY1p.qp_oldB());
    /*#p13.CAVA*/ wire CAVA_old = nor2(ch1_old.CENA_NR11_DUTY0p.qn_oldB(), ch1_old.DYCA_NR11_DUTY1p.qp_oldB());
    /*#p13.CEVU*/ wire CEVU_old = nor2(ch1_old.CENA_NR11_DUTY0p.qp_oldB(), ch1_old.DYCA_NR11_DUTY1p.qn_oldB());
    /*#p13.CAXO*/ wire CAXO_old = nor2(ch1_old.CENA_NR11_DUTY0p.qn_oldB(), ch1_old.DYCA_NR11_DUTY1p.qn_oldB());
    /*#p13.DUNA*/ wire DUNA_RAW_BIT_old = amux4(ENEK_PHASE_xxxxxxGx_old, COSO_old,
                                                EZOZ_PHASE_xxxxxxGH_old, CAVA_old,
                                                ch1_old.DAPE_PHASE_xxxxEFGH.qp_old(), CEVU_old,
                                                CODO_PHASE_ABCDEFxx_old, CAXO_old);
    /*_p13.DUWO*/ ch1_new.DUWO_RAW_BIT_SYNCp.dff17(COPE_SAMPLE_CLKp_new, CEPO_APU_RSTn_new, DUNA_RAW_BIT_old);
  }

  {
    /*#p11.COVU*/ wire COVU_NR11_WR = and2(BOGY_CPU_WRp, CAXE_ADDR_FF11p);
    /*#p11.DAFO*/ wire DAFO_NR11_CLK = not1(COVU_NR11_WR);
    /*#p11.CENA*/ ch1_new.CENA_NR11_DUTY0p.dff9b(DAFO_NR11_CLK, CEPO_APU_RSTn_new, dbus_old.BUS_CPU_D06p.qp_old());
    /*#p11.DYCA*/ ch1_new.DYCA_NR11_DUTY1p.dff9b(DAFO_NR11_CLK, CEPO_APU_RSTn_new, dbus_old.BUS_CPU_D07p.qp_old());
  }

  {
    /*#p11.BUZE*/ wire BUZE_ADDR_FF10n = not1(DYVA_ADDR_FF10p);
    /*#p11.ATYN*/ wire ATYN_NR10_RDp = nor2(AGUZ_CPU_RDn, BUZE_ADDR_FF10n);
    /*#p11.ASOP*/ wire ASOP_NR10_RDn = not1(ATYN_NR10_RDp);

    // Isn't the data polarity here backwards?
    /*#p11.AMYD*/ triwire AMYD_NR10_SWEEP_SHIFT0  = tri6_nn(ASOP_NR10_RDn, ch1_new.BANY_NR10_SWEEP_SHIFT0p .qn_newB());
    /*#p11.ATAX*/ triwire ATAX_NR10_SWEEP_SHIFT1  = tri6_nn(ASOP_NR10_RDn, ch1_new.ARAX_NR10_SWEEP_SHIFT1p .qn_newB());
    /*#p11.AZYP*/ triwire AZYP_NR10_SWEEP_SHIFT2  = tri6_nn(ASOP_NR10_RDn, ch1_new.ANAZ_NR10_SWEEP_SHIFT2p .qn_newB());
    /*#p11.AFOX*/ triwire AFOX_NR10_SWEEP_NEGATE  = tri6_nn(ASOP_NR10_RDn, ch1_new.AVAF_NR10_SWEEP_DIRp    .qn_newB());
    /*#p11.AVEK*/ triwire AVEK_NR10_SWEEP_PERIOD0 = tri6_nn(ASOP_NR10_RDn, ch1_new.ADEK_NR10_SWEEP_PERIOD0p.qn_newB());
    /*#p11.AKUX*/ triwire AKUX_NR10_SWEEP_PERIOD1 = tri6_nn(ASOP_NR10_RDn, ch1_new.BANA_NR10_SWEEP_PERIOD1p.qn_newB());
    /*#p11.AWOS*/ triwire AWOS_NR10_SWEEP_PERIOD2 = tri6_nn(ASOP_NR10_RDn, ch1_new.BOTU_NR10_SWEEP_PERIOD2p.qn_newB());

    /*_BUS_CPU_D00p*/ dbus_new.BUS_CPU_D00p.tri_bus(AMYD_NR10_SWEEP_SHIFT0);
    /*_BUS_CPU_D01p*/ dbus_new.BUS_CPU_D01p.tri_bus(ATAX_NR10_SWEEP_SHIFT1);
    /*_BUS_CPU_D02p*/ dbus_new.BUS_CPU_D02p.tri_bus(AZYP_NR10_SWEEP_SHIFT2);
    /*_BUS_CPU_D03p*/ dbus_new.BUS_CPU_D03p.tri_bus(AFOX_NR10_SWEEP_NEGATE);
    /*_BUS_CPU_D04p*/ dbus_new.BUS_CPU_D04p.tri_bus(AVEK_NR10_SWEEP_PERIOD0);
    /*_BUS_CPU_D05p*/ dbus_new.BUS_CPU_D05p.tri_bus(AKUX_NR10_SWEEP_PERIOD1);
    /*_BUS_CPU_D06p*/ dbus_new.BUS_CPU_D06p.tri_bus(AWOS_NR10_SWEEP_PERIOD2);
  }

  {
    /*#p11.BUWA*/ wire BUWA_CPU_RDp = not1(AGUZ_CPU_RDn);
    /*#p11.BEXU*/ wire BEXU_NR11_RDn = nand2(BUWA_CPU_RDp, CAXE_ADDR_FF11p);
    /*#p11.BOWO*/ triwire BOWO = tri6_nn(BEXU_NR11_RDn, ch1_new.CENA_NR11_DUTY0p.qn_newB());
    /*#p11.CUDA*/ triwire CUDA = tri6_nn(BEXU_NR11_RDn, ch1_new.DYCA_NR11_DUTY1p.qn_newB());

    /*_BUS_CPU_D06p*/ dbus_new.BUS_CPU_D06p.tri_bus(BOWO);
    /*_BUS_CPU_D07p*/ dbus_new.BUS_CPU_D07p.tri_bus(CUDA);
  }

  {
    /*#p11.HAXE*/ wire HAXE_ADDR_FF12n = not1(EDAF_ADDR_FF12p);
    /*#p11.GAGO*/ wire GAGO_ADDR_FF12n = not1(EDAF_ADDR_FF12p);

    /*#p11.HAMY*/ wire HAMY_NR12_RDn = or2(HAXE_ADDR_FF12n, AGUZ_CPU_RDn);
    /*#p11.HOCU*/ wire HOCU_NR12_RDn = or2(GAGO_ADDR_FF12n, AGUZ_CPU_RDn);

    /*#p11.JYNE*/ triwire JYNE = tri6_nn(HAMY_NR12_RDn, ch1_new.JUSA_NR12_PERIOD0p .qn_newB());
    /*#p11.JACA*/ triwire JACA = tri6_nn(HAMY_NR12_RDn, ch1_new.JUZY_NR12_PERIOD1p .qn_newB());
    /*#p11.JOKU*/ triwire JOKU = tri6_nn(HAMY_NR12_RDn, ch1_new.JOMA_NR12_PERIOD2p .qn_newB());
    /*#p11.HONO*/ triwire HONO = tri6_nn(HOCU_NR12_RDn, ch1_new.JAFY_NR12_ENV_DIRp.qn_newB());
    /*#p11.HOWU*/ triwire HOWU = tri6_nn(HOCU_NR12_RDn, ch1_new.JATY_NR12_VOL0p   .qn_newB());
    /*#p11.HEWA*/ triwire HEWA = tri6_nn(HOCU_NR12_RDn, ch1_new.JAXO_NR12_VOL1p   .qn_newB());
    /*#p11.HEVE*/ triwire HEVE = tri6_nn(HOCU_NR12_RDn, ch1_new.JENA_NR12_VOL2p   .qn_newB());
    /*#p11.JYSE*/ triwire JYSE = tri6_nn(HOCU_NR12_RDn, ch1_new.JOPU_NR12_VOL3p   .qn_newB());

    /*_BUS_CPU_D00p*/ dbus_new.BUS_CPU_D00p.tri_bus(JYNE);
    /*_BUS_CPU_D01p*/ dbus_new.BUS_CPU_D01p.tri_bus(JACA);
    /*_BUS_CPU_D02p*/ dbus_new.BUS_CPU_D02p.tri_bus(JOKU);
    /*_BUS_CPU_D03p*/ dbus_new.BUS_CPU_D03p.tri_bus(HONO);
    /*_BUS_CPU_D04p*/ dbus_new.BUS_CPU_D04p.tri_bus(HOWU);
    /*_BUS_CPU_D05p*/ dbus_new.BUS_CPU_D05p.tri_bus(HEWA);
    /*_BUS_CPU_D06p*/ dbus_new.BUS_CPU_D06p.tri_bus(HEVE);
    /*_BUS_CPU_D07p*/ dbus_new.BUS_CPU_D07p.tri_bus(JYSE);
  }

  // Frequency can be read from FF13/FF14 in debug mode

  /*_p09.EDEK*/ wire EDEK_NR52_DBG_APUp = not1(FERO_NR52_DBG_APUp);
  /*#p11.CEGE*/ wire CEGE_CPU_RDp = not1(AGUZ_CPU_RDn);
  /*#p11.DAXA*/ wire DAXA_CPU_RDn_DBGn = nand2(CEGE_CPU_RDp, EDEK_NR52_DBG_APUp);

  {
    /*#p11.CACA*/ wire CACA_ADDR_FF13n = not1(DECO_ADDR_FF13p);
    /*#p11.DYPU*/ wire DYPU_FF13_RDn = nor2(CACA_ADDR_FF13n, DAXA_CPU_RDn_DBGn);
    /*#p11.EVAJ*/ wire EVAJ_FF13_RDa = not1(DYPU_FF13_RDn);

    /*_p11.FORU*/ triwire FORU = tri6_nn(EVAJ_FF13_RDa, ch1_new.GAXE_CH1_FREQ_CNT_00.qn_new());
    /*_p11.GEFU*/ triwire GEFU = tri6_nn(EVAJ_FF13_RDa, ch1_new.HYFE_CH1_FREQ_CNT_01.qn_new());
    /*_p11.KYVU*/ triwire KYVU = tri6_nn(EVAJ_FF13_RDa, ch1_new.JYTY_CH1_FREQ_CNT_02.qn_new());
    /*_p11.KUMO*/ triwire KUMO = tri6_nn(EVAJ_FF13_RDa, ch1_new.KYNA_CH1_FREQ_CNT_03.qn_new());
    /*_p11.KARY*/ triwire KARY = tri6_nn(EVAJ_FF13_RDa, ch1_new.JEMA_CH1_FREQ_CNT_04.qn_new());
    /*_p11.GODE*/ triwire GODE = tri6_nn(EVAJ_FF13_RDa, ch1_new.HYKE_CH1_FREQ_CNT_05.qn_new());
    /*_p11.GOJE*/ triwire GOJE = tri6_nn(EVAJ_FF13_RDa, ch1_new.FEVA_CH1_FREQ_CNT_06.qn_new());
    /*_p11.FOZE*/ triwire FOZE = tri6_nn(EVAJ_FF13_RDa, ch1_new.EKOV_CH1_FREQ_CNT_07.qn_new());

    /*_BUS_CPU_D00p*/ dbus_new.BUS_CPU_D00p.tri_bus(FORU);
    /*_BUS_CPU_D01p*/ dbus_new.BUS_CPU_D01p.tri_bus(GEFU);
    /*_BUS_CPU_D02p*/ dbus_new.BUS_CPU_D02p.tri_bus(KYVU);
    /*_BUS_CPU_D03p*/ dbus_new.BUS_CPU_D03p.tri_bus(KUMO);
    /*_BUS_CPU_D04p*/ dbus_new.BUS_CPU_D04p.tri_bus(KARY);
    /*_BUS_CPU_D05p*/ dbus_new.BUS_CPU_D05p.tri_bus(GODE);
    /*_BUS_CPU_D06p*/ dbus_new.BUS_CPU_D06p.tri_bus(GOJE);
    /*_BUS_CPU_D07p*/ dbus_new.BUS_CPU_D07p.tri_bus(FOZE);
  }

  {
    /*#p11.CURE*/ wire CURE_ADDR_FF14n = not1(DUJA_ADDR_FF14p);
    /*#p11.DUPY*/ wire DUPY_FF14_RDn = or2(CURE_ADDR_FF14n, DAXA_CPU_RDn_DBGn);
    /*_p11.DOPA*/ triwire DOPA = tri6_nn(DUPY_FF14_RDn, ch1_new.EMUS_CH1_FREQ_CNT_08.qn_new());
    /*_p11.DEMU*/ triwire DEMU = tri6_nn(DUPY_FF14_RDn, ch1_new.EVAK_CH1_FREQ_CNT_09.qn_new());
    /*_p11.DEXO*/ triwire DEXO = tri6_nn(DUPY_FF14_RDn, ch1_new.COPU_CH1_FREQ_CNT_10.qn_new());

    /*_BUS_CPU_D00p*/ dbus_new.BUS_CPU_D00p.tri_bus(DOPA);
    /*_BUS_CPU_D01p*/ dbus_new.BUS_CPU_D01p.tri_bus(DEMU);
    /*_BUS_CPU_D02p*/ dbus_new.BUS_CPU_D02p.tri_bus(DEXO);

    /*#p11.BUDA*/ wire BUDA_CPU_RDp = not1(AGUZ_CPU_RDn);
    /*#p11.BALE*/ wire BALE_NR14_RDn = nand2(DUJA_ADDR_FF14p, BUDA_CPU_RDp);
    /*#p11.BYTU*/ triwire BYTU = tri6_nn(BALE_NR14_RDn, ch1_new.BOKO_NR14_LENENp.qn_newB());
    /*_BUS_CPU_D06p*/ dbus_new.BUS_CPU_D06p.tri_bus(BYTU);
  }

  /*#p13.COWE*/ wire COWE_BIT_OUTp = and2(ch1_new.CYTO_CH1_ACTIVEp.state & 1, ch1_new.DUWO_RAW_BIT_SYNCp.state & 1);
}

#endif
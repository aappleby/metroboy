#include "GateBoySPU.h"

#include "GateBoyLib/GateBoyState.h"

#ifdef SIM_AUDIO

int ch2_audio_out(const SpuChannel2& ch2) {

  /*#p15.CYSE*/ wire CYSE_CH2_OUTp = and2(ch2.DANE_CH2_ACTIVEp.qp_any(), ch2.DOME_CH2_OUTp.qp_any());
  ///*_p09.EDEK*/ wire EDEK_NR52_DBG_APUp = not1(reg_new.spu.FERO_NR52_DBG_APUp.qn_any());
  ///*#p15.BONU*/ wire BONU_CH2_OUTp = or2(CYSE_CH2_OUTp, EDEK_NR52_DBG_APUp);
  wire BONU_CH2_OUTp = CYSE_CH2_OUTp;
  /*#p15.ANAN*/ wire ANAN_CH2_BIT0 = and2(ch2.FENO_ENV_VOL0.qp_any(), BONU_CH2_OUTp);
  /*#p15.ANYV*/ wire ANYV_CH2_BIT1 = and2(ch2.FETE_ENV_VOL1.qp_any(), BONU_CH2_OUTp);
  /*#p15.ASOG*/ wire ASOG_CH2_BIT2 = and2(ch2.FOMY_ENV_VOL2.qp_any(), BONU_CH2_OUTp);
  /*#p15.AMOV*/ wire AMOV_CH2_BIT3 = and2(ch2.FENA_ENV_VOL3.qp_any(), BONU_CH2_OUTp);

  return ((ANAN_CH2_BIT0 & 1) << 0) |
         ((ANYV_CH2_BIT1 & 1) << 1) |
         ((ASOG_CH2_BIT2 & 1) << 2) |
         ((AMOV_CH2_BIT3 & 1) << 3);
}


void SpuChannel2::reset_to_cart() {
  ERYC_NR21_LEN0.state = 0x1a;
  CERA_NR21_LEN1.state = 0x18;
  CONU_NR21_LEN2.state = 0x18;
  CAME_NR21_LEN3.state = 0x18;
  BUVA_NR21_LEN4.state = 0x18;
  AKYD_NR21_LEN5.state = 0x18;
  BERA_NR21_DUTY0.state = 0x1a;
  BAMY_NR21_DUTY1.state = 0x1a;
  HYFU_NR22_P0p.state = 0x1a;
  HORE_NR22_P1p.state = 0x1a;
  HAVA_NR22_P2p.state = 0x1a;
  FORE_NR22_ADDp.state = 0x1a;
  GATA_NR22_V0p.state = 0x1a;
  GUFE_NR22_V1p.state = 0x1a;
  GURA_NR22_V2p.state = 0x1a;
  GAGE_NR22_V3p.state = 0x1a;
  FOFE_NR23_FREQ00p.state = 0x1a;
  FOVA_NR23_FREQ01p.state = 0x1a;
  FEDY_NR23_FREQ02p.state = 0x1a;
  FOME_NR23_FREQ03p.state = 0x1a;
  FORA_NR23_FREQ04p.state = 0x1a;
  GODA_NR23_FREQ05p.state = 0x1a;
  GUMY_NR23_FREQ06p.state = 0x1a;
  GUPU_NR23_FREQ07p.state = 0x1a;
  JEFU_NR24_FREQ08p.state = 0x1a;
  JANY_NR24_FREQ09p.state = 0x1a;
  JUPY_NR24_FREQ10p.state = 0x1a;
  DANE_CH2_ACTIVEp.state = 0x18;
  JORE_ENV_DELAY0p.state = 0x1a;
  JONA_ENV_DELAY1p.state = 0x18;
  JEVY_ENV_DELAY2p.state = 0x18;
  JOPA_ENV_TICK.state = 0x1a;
  DONE_COUNT00.state = 0x1a;
  DYNU_COUNT01.state = 0x18;
  EZOF_COUNT02.state = 0x18;
  CYVO_COUNT03.state = 0x18;
  FUXO_COUNT04.state = 0x18;
  GANO_COUNT05.state = 0x18;
  GOCA_COUNT06.state = 0x18;
  GANE_COUNT07.state = 0x18;
  HEVY_COUNT08.state = 0x18;
  HEPU_COUNT09.state = 0x18;
  HERO_COUNT10.state = 0x18;
  DOME_CH2_OUTp.state = 0x18;
  ELOX_TRIGp_d01.state = 0x18;
  CAZA_TRIGp_d17.state = 0x18;
  EMER_NR24_LENENp.state = 0x1a;
  DOPE_TRIGp_d00.state = 0x18;
  DALA_TRIGp_d00.state = 0x19;
  ETAP_NR24_TRIGp.state = 0x1a;
  DORY_TRIGp_d09.state = 0x18;
  GYKO_SAMPLE_CLKp.state = 0x1a;
  CANO_PHASE0.state = 0x1a;
  CAGY_PHASE1.state = 0x1a;
  DYVE_PHASE2.state = 0x1a;
  BUTA_FREQ_GATEp.state = 0x19;
  FENA_ENV_VOL3.state = 0x1a;
  FOMY_ENV_VOL2.state = 0x1a;
  FETE_ENV_VOL1.state = 0x1a;
  FENO_ENV_VOL0.state = 0x1a;
  HEPO_ENV_MAXp.state = 0x18;
  JEME_ENV_DONEp.state = 0x18;
  JYNA_ENV_CLK64.state = 0x19;
  CYRE_LEN_DONEp.state = 0x1a;
}

#if 0
  /*#p15.DAVU*/ wire DAVU_SAMPLE_CLKp_old() const { return not1(ETUK_SAMPLE_CLKn_old()); }
  /*#p15.ETUK*/ wire ETUK_SAMPLE_CLKn_new() const { return not1(GYKO_SAMPLE_CLKp.qp_new()); }
  /*#p15.DAVU*/ wire DAVU_SAMPLE_CLKp_new() const { return not1(ETUK_SAMPLE_CLKn_new()); }
  /*#p15.CULE*/ wire CULE_SAMPLE_CLKn_new() const { return not1(DAVU_SAMPLE_CLKp_new()); }
  /*#p15.JUPU*/ wire JUPU_ENV_OFFp_old() const { return nor3(HYFU_NR22_P0p.qp_oldB(), HORE_NR22_P1p.qp_oldB(), HAVA_NR22_P2p.qp_oldB()); }
  /*#p15.JUPU*/ wire JUPU_ENV_OFFp_new() const { return nor3(HYFU_NR22_P0p.qp_newB(), HORE_NR22_P1p.qp_newB(), HAVA_NR22_P2p.qp_newB()); }
#endif

void tick_ch2(const GateBoyState& reg_old, GateBoyState& reg_new) {
  auto& ch2_old = reg_old.ch2;
  auto& ch2_new = reg_new.ch2;

  auto& abus_new = reg_new.cpu_abus;

  /*#p01.AVOR*/ wire AVOR_SYS_RSTp =  or2(reg_new.sys_rst.AFER_SYS_RSTp.qp_new(), reg_new.sys_rst.ASOL_POR_DONEn.qp_new());
  /*#p01.ALUR*/ wire ALUR_SYS_RSTn = not1(AVOR_SYS_RSTp);
  /*_p09.HAPO*/ wire HAPO_SYS_RSTp = not1(ALUR_SYS_RSTn);
  /*_p09.JYRO*/ wire JYRO_APU_RSTp = or2(HAPO_SYS_RSTp, reg_new.spu.HADA_NR52_ALL_SOUND_ON.qn_new());
  /*_p09.KUBY*/ wire KUBY_APU_RSTn = not1(JYRO_APU_RSTp);
  /*_p09.KEBA*/ wire KEBA_APU_RSTp = not1(KUBY_APU_RSTn);

  /*_p01.ABOL*/ wire ABOL_CLKREQn  = not1(reg_new.sys_clk.SIG_CPU_CLKREQ.out_new());
  /*#p01.ATYP*/ wire ATYP_ABCDxxxx = not1(reg_new.sys_clk.AFUR_ABCDxxxx.qn_newB());
  /*#p01.BELU*/ wire BELU_xxxxEFGH = nor2(ATYP_ABCDxxxx, ABOL_CLKREQn);
  /*#p01.BYRY*/ wire BYRY_ABCDxxxx = not1(BELU_xxxxEFGH);
  /*#p01.BUDE*/ wire BUDE_xxxxEFGH = not1(BYRY_ABCDxxxx);
  /*_p01.DOVA*/ wire DOVA_ABCDxxxx = not1(BUDE_xxxxEFGH);

  /*_p10.DYTE*/ wire DYTE_A00n = not1(abus_new.BUS_CPU_A00p.out_new());
  /*_p10.AFOB*/ wire AFOB_A01n = not1(abus_new.BUS_CPU_A01p.out_new());
  /*_p10.ABUB*/ wire ABUB_A02n = not1(abus_new.BUS_CPU_A02p.out_new());
  /*_p10.ACOL*/ wire ACOL_A03n = not1(abus_new.BUS_CPU_A03p.out_new());
  /*#p10.ATUP*/ wire ATUP_A04n = not1(abus_new.BUS_CPU_A04p.out_new());
  /*#p10.BOXY*/ wire BOXY_A05n = not1(abus_new.BUS_CPU_A05p.out_new());
  /*#p10.ASAD*/ wire ASAD_A06n = not1(abus_new.BUS_CPU_A06p.out_new());
  /*#p10.AVUN*/ wire AVUN_A07n = not1(abus_new.BUS_CPU_A07p.out_new());
  /*_p07.TONA*/ wire TONA_A08n = not1(abus_new.BUS_CPU_A08p.out_new());
  /*_p10.DOSO*/ wire DOSO_A00p = not1(DYTE_A00n);
  /*_p10.DUPA*/ wire DUPA_A01p = not1(AFOB_A01n);
  /*_p10.DENO*/ wire DENO_A02p = not1(ABUB_A02n);
  /*#p10.DUCE*/ wire DUCE_A03p = not1(ACOL_A03n);

  /*_p10.DAZA*/ wire DAZA_ADDR_0110n = nand4(ACOL_A03n, DENO_A02p, DUPA_A01p, DYTE_A00n);
  /*_p10.DUVU*/ wire DUVU_ADDR_0111n = nand4(ACOL_A03n, DENO_A02p, DUPA_A01p, DOSO_A00p);
  /*_p10.DAFY*/ wire DAFY_ADDR_1000n = nand4(DUCE_A03p, ABUB_A02n, AFOB_A01n, DYTE_A00n);
  /*_p10.DEJY*/ wire DEJY_ADDR_1001n = nand4(DUCE_A03p, ABUB_A02n, AFOB_A01n, DOSO_A00p);

  /*_p07.BAKO*/ wire BAKO_ADDR_FFXXn = not1(abus_new.SYKE_ADDR_HIp_new());
  /*_p10.ATEG*/ wire ATEG_ADDR_XX1Xn = or4(abus_new.BUS_CPU_A07p.out_new(), abus_new.BUS_CPU_A06p.out_new(), abus_new.BUS_CPU_A05p.out_new(), ATUP_A04n);
  /*_p10.BUNO*/ wire BUNO_ADDR_FF1Xp = nor2(BAKO_ADDR_FFXXn, ATEG_ADDR_XX1Xn);
  /*_p10.BANU*/ wire BANU_ADDR_FF1Xn = not1(BUNO_ADDR_FF1Xp);
  /*_p10.COVY*/ wire COVY_ADDR_FF16p = nor2(BANU_ADDR_FF1Xn, DAZA_ADDR_0110n);
  /*_p10.DUTU*/ wire DUTU_ADDR_FF17p = nor2(BANU_ADDR_FF1Xn, DUVU_ADDR_0111n);
  /*_p10.DARA*/ wire DARA_ADDR_FF18p = nor2(BANU_ADDR_FF1Xn, DAFY_ADDR_1000n);
  /*_p10.DOZA*/ wire DOZA_ADDR_FF19p = nor2(BANU_ADDR_FF1Xn, DEJY_ADDR_1001n);

  /*_p09.AGUZ*/ wire AGUZ_CPU_RDn = not1(reg_new.cpu_signals.TEDO_CPU_RDp.qp_new());
  /*_p10.BAFU*/ wire BAFU_CPU_WRn = not1(reg_new.cpu_signals.TAPU_CPU_WRp.qp_new());
  /*_p10.BOGY*/ wire BOGY_CPU_WRp = not1(BAFU_CPU_WRn);

  /*#p01.HAMA*/ wire HAMA_CLK_512K = not1(reg_new.spu.JESO_CLK_512K.qp_new());
  /*_p01.BURE*/ wire BURE_CLK_512 = not1(reg_new.spu.BARA_CLK_512.qp_new());
  /*#p01.FYNE*/ wire FYNE_CLK_512 = not1(BURE_CLK_512);
  /*_p01.GALE*/ wire GALE_CLK_512 = mux2p(reg_new.spu.FERO_NR52_DBG_APUp.qp_newB(), HAMA_CLK_512K, FYNE_CLK_512);
  /*_p01.GEXY*/ wire GEXY_CLK_512 = not1(GALE_CLK_512);
  /*_p01.HORU*/ wire HORU_CLK_512 = not1(GEXY_CLK_512);

  /*_p01.CULO*/ wire CULO_CLK_256 = not1(reg_new.spu.CARU_CLK_256.qp_new());
  /*_p01.BEZE*/ wire BEZE_CLK_256 = mux2p(reg_new.spu.FERO_NR52_DBG_APUp.qp_newB(), HAMA_CLK_512K, CULO_CLK_256);
  /*_p01.COFU*/ wire COFU_CLK_256 = not1(BEZE_CLK_256);
  /*_p01.BUFY*/ wire BUFY_CLK_256 = not1(COFU_CLK_256);

  /*_p01.APEF*/ wire APEF_CLK_128 = not1(reg_new.spu.BYLU_CLK_128.qp_new());
  /*_p01.BULE*/ wire BULE_CLK_128 = mux2p(reg_new.spu.FERO_NR52_DBG_APUp.qp_newB(), HAMA_CLK_512K, APEF_CLK_128);
  /*_p01.BARU*/ wire BARU_CLK_128 = not1(BULE_CLK_128);
  /*_p01.BYFE*/ wire BYFE_CLK_128 = not1(BARU_CLK_128);












  /*_p15.CYWU*/ wire CYWU_APU_RSTn_new = not1(KEBA_APU_RSTp);
  /*#p15.DOPE*/ ch2_new.DOPE_TRIGp_d00.dff17(DOVA_ABCDxxxx, CYWU_APU_RSTn_new, ch2_old.ETAP_NR24_TRIGp.qp_oldB());

  {
    /*#p14.ENUF*/ wire ENUF_FF17_WRp = and2(DUTU_ADDR_FF17p, BOGY_CPU_WRp);
    /*#p14.GERE*/ wire GERE_FF17_WRp = and2(BOGY_CPU_WRp, DUTU_ADDR_FF17p);
    /*#p14.JEDE*/ wire JEDE_FF17_WRn = not1(GERE_FF17_WRp);

    // Clock polarities wrong on die again
    /*#p14.ENUF*/ wire ENUF_FF17_WRn = not1(ENUF_FF17_WRp);

    /*_p14.JYBU*/ wire JYBU_APU_RSTn_new = not1(KEBA_APU_RSTp);
    /*_p14.HYFU*/ ch2_new.HYFU_NR22_P0p .dff9b(JEDE_FF17_WRn, JYBU_APU_RSTn_new, reg_old.cpu_dbus.BUS_CPU_D00p.out_old());
    /*_p14.HORE*/ ch2_new.HORE_NR22_P1p .dff9b(JEDE_FF17_WRn, JYBU_APU_RSTn_new, reg_old.cpu_dbus.BUS_CPU_D01p.out_old());
    /*_p14.HAVA*/ ch2_new.HAVA_NR22_P2p .dff9b(JEDE_FF17_WRn, JYBU_APU_RSTn_new, reg_old.cpu_dbus.BUS_CPU_D02p.out_old());
    /*_p14.FORE*/ ch2_new.FORE_NR22_ADDp.dff9b(ENUF_FF17_WRn, JYBU_APU_RSTn_new, reg_old.cpu_dbus.BUS_CPU_D03p.out_old());
    /*_p14.GATA*/ ch2_new.GATA_NR22_V0p .dff9b(ENUF_FF17_WRn, JYBU_APU_RSTn_new, reg_old.cpu_dbus.BUS_CPU_D04p.out_old());
    /*_p14.GUFE*/ ch2_new.GUFE_NR22_V1p .dff9b(ENUF_FF17_WRn, JYBU_APU_RSTn_new, reg_old.cpu_dbus.BUS_CPU_D05p.out_old());
    /*_p14.GURA*/ ch2_new.GURA_NR22_V2p .dff9b(ENUF_FF17_WRn, JYBU_APU_RSTn_new, reg_old.cpu_dbus.BUS_CPU_D06p.out_old());
    /*_p14.GAGE*/ ch2_new.GAGE_NR22_V3p .dff9b(ENUF_FF17_WRn, JYBU_APU_RSTn_new, reg_old.cpu_dbus.BUS_CPU_D07p.out_old());
  }

  {
    /*_p14.DOSA*/ wire DOSA_NR23_WR1 = and2(DARA_ADDR_FF18p, BOGY_CPU_WRp);
    /*_p14.EXUC*/ wire EXUC_NR23_WR2 = and2(DARA_ADDR_FF18p, BOGY_CPU_WRp);
    /*_p14.ESUR*/ wire ESUR_NR23_WRn1 = not1(DOSA_NR23_WR1);
    /*_p14.FYXO*/ wire FYXO_NR23_WRn2 = not1(EXUC_NR23_WR2);
    /*_p14.HUDE*/ wire HUDE_APU_RSTn_new = not1(KEBA_APU_RSTp);
    /*_p14.FOFE*/ ch2_new.FOFE_NR23_FREQ00p.dff9b(ESUR_NR23_WRn1, HUDE_APU_RSTn_new, reg_old.cpu_dbus.BUS_CPU_D00p.out_old());
    /*_p14.FOVA*/ ch2_new.FOVA_NR23_FREQ01p.dff9b(ESUR_NR23_WRn1, HUDE_APU_RSTn_new, reg_old.cpu_dbus.BUS_CPU_D01p.out_old());
    /*_p14.FEDY*/ ch2_new.FEDY_NR23_FREQ02p.dff9b(ESUR_NR23_WRn1, HUDE_APU_RSTn_new, reg_old.cpu_dbus.BUS_CPU_D02p.out_old());
    /*_p14.FOME*/ ch2_new.FOME_NR23_FREQ03p.dff9b(ESUR_NR23_WRn1, HUDE_APU_RSTn_new, reg_old.cpu_dbus.BUS_CPU_D03p.out_old());
    /*_p14.FORA*/ ch2_new.FORA_NR23_FREQ04p.dff9b(ESUR_NR23_WRn1, HUDE_APU_RSTn_new, reg_old.cpu_dbus.BUS_CPU_D04p.out_old());
    /*_p14.GODA*/ ch2_new.GODA_NR23_FREQ05p.dff9b(FYXO_NR23_WRn2, HUDE_APU_RSTn_new, reg_old.cpu_dbus.BUS_CPU_D05p.out_old());
    /*_p14.GUMY*/ ch2_new.GUMY_NR23_FREQ06p.dff9b(FYXO_NR23_WRn2, HUDE_APU_RSTn_new, reg_old.cpu_dbus.BUS_CPU_D06p.out_old());
    /*_p14.GUPU*/ ch2_new.GUPU_NR23_FREQ07p.dff9b(FYXO_NR23_WRn2, HUDE_APU_RSTn_new, reg_old.cpu_dbus.BUS_CPU_D07p.out_old());
  }

  {
    /*_pXX.JENU*/ wire JENU_NR24_WRp = and2(DOZA_ADDR_FF19p, BOGY_CPU_WRp);
    /*_pXX.KYSA*/ wire KYSA_NR24_WRn = not1(JENU_NR24_WRp);
    /*_p14.KYPU*/ wire KYPU_APU_RSTn_new = not1(KEBA_APU_RSTp);
    /*_pXX.JEFU*/ ch2_new.JEFU_NR24_FREQ08p.dff9b(KYSA_NR24_WRn, KYPU_APU_RSTn_new, reg_old.cpu_dbus.BUS_CPU_D00p.out_old());
    /*_pXX.JANY*/ ch2_new.JANY_NR24_FREQ09p.dff9b(KYSA_NR24_WRn, KYPU_APU_RSTn_new, reg_old.cpu_dbus.BUS_CPU_D01p.out_old());
    /*_pXX.JUPY*/ ch2_new.JUPY_NR24_FREQ10p.dff9b(KYSA_NR24_WRn, KYPU_APU_RSTn_new, reg_old.cpu_dbus.BUS_CPU_D02p.out_old());

    /*#p??.DETA*/ wire DETA_NR24_WRn = nand2(BOGY_CPU_WRp, DOZA_ADDR_FF19p);
    /*#p15.DERA*/ wire DERA_TRIG_RSTn = nor2(KEBA_APU_RSTp, ch2_new.DOPE_TRIGp_d00.qp_new());
    /*#p??.ETAP*/ ch2_new.ETAP_NR24_TRIGp.dff9b(DETA_NR24_WRn, DERA_TRIG_RSTn, reg_old.cpu_dbus.BUS_CPU_D07p.out_old());

    // This goes to all the CHN_LENENp registers...why?
    /*_p16.ANUJ*/ wire ANUJ_CPU_WR_WEIRD = and2(reg_new.cpu_signals.SIG_IN_CPU_DBUS_FREE.qp_new(), BOGY_CPU_WRp);
    /*#pXX.EVYF*/ wire EVYF = nand2(ANUJ_CPU_WR_WEIRD, DOZA_ADDR_FF19p);
    /*_p14.FAZO*/ wire FAZO_APU_RSTn_new = not1(KEBA_APU_RSTp);
    /*#pXX.EMER*/ ch2_new.EMER_NR24_LENENp.dff9b(EVYF, FAZO_APU_RSTn_new, reg_old.cpu_dbus.BUS_CPU_D06p.out_old());
  }

  //----------
  // Trigger

  {
    /*_p15.CEXE*/ wire CEXE_APU_RSTn_new = not1(KEBA_APU_RSTp);
    /*#p15.DORY*/ ch2_new.DORY_TRIGp_d09.dff17(reg_new.spu.CEMO_xBCDExxx.qp_new(), CEXE_APU_RSTn_new, ch2_old.ELOX_TRIGp_d01.qp_old());
    /*#p15.CAZA*/ ch2_new.CAZA_TRIGp_d17.dff17(reg_new.spu.CEMO_xBCDExxx.qp_new(), CEXE_APU_RSTn_new, ch2_old.DORY_TRIGp_d09.qp_old());

    /*#p15.DOXA*/ wire DOXA_TRIG_RSTn_new = nor2(KEBA_APU_RSTp, ch2_new.DORY_TRIGp_d09.qp_new());
    /*#p15.ELOX*/ ch2_new.ELOX_TRIGp_d01.dff17(reg_new.spu.CEMO_xBCDExxx.qp_new(), DOXA_TRIG_RSTn_new, ch2_old.DALA_TRIGp_d00.qn_old());

    /*#p15.CELO*/ wire CELO_CH2_TRIGp_new = not1(DOXA_TRIG_RSTn_new);   
    /*#p15.DALA*/ ch2_new.DALA_TRIGp_d00.nor_latch(CELO_CH2_TRIGp_new, ch2_new.DOPE_TRIGp_d00.qp_new());
  }

  //----------
  // Clocks

  {
    /*#p15.HOTA*/ wire HOTA_CLK_128 = not1(BYFE_CLK_128);
    /*_p15.KATY*/ wire KATY_APU_RSTn_new = not1(KEBA_APU_RSTp);
    /*#p15.JYNA*/ ch2_new.JYNA_ENV_CLK64.dff17(HOTA_CLK_128, KATY_APU_RSTn_new, ch2_old.JYNA_ENV_CLK64.qn_old());
  }

  //----------------------------------------
  // Freq counter

  /*#p15.FUTE*/ wire FUTE_CH2_AMP_ENn_new = nor5(ch2_new.FORE_NR22_ADDp.qp_newB(),
                                                 ch2_new.GATA_NR22_V0p.qp_newB(),
                                                 ch2_new.GUFE_NR22_V1p.qp_newB(),
                                                 ch2_new.GURA_NR22_V2p.qp_newB(),
                                                 ch2_new.GAGE_NR22_V3p.qp_newB());


  /*#p15.ARES*/ wire ARES = nor2(FUTE_CH2_AMP_ENn_new, KEBA_APU_RSTp); // looks like nor on the die?
  /*#p15.BODO*/ wire BODO = not1(ch2_new.CAZA_TRIGp_d17.qp_new());
  /*_p15.BUTA*/ ch2_new.BUTA_FREQ_GATEp.nand_latch(ARES, BODO);
  /*#p15.CAMA*/ wire CAMA_FREQ_TICKp = nor2(reg_new.spu.CEMO_xBCDExxx.qp_new(), ch2_new.BUTA_FREQ_GATEp.qp_new());
  /*_p15.DOCA*/ wire DOCA_FREQ_TICKn = not1(CAMA_FREQ_TICKp);

  //----------

  // clk/async split around GYKO here
  {
    /*_p14.DONE*/ ch2_new.DONE_COUNT00.dff20_clk(DOCA_FREQ_TICKn                 );
    /*_p14.DYNU*/ ch2_new.DYNU_COUNT01.dff20_clk(ch2_new.DONE_COUNT00.qp_new());
    /*_p14.EZOF*/ ch2_new.EZOF_COUNT02.dff20_clk(ch2_new.DYNU_COUNT01.qp_new());
    /*_p14.CYVO*/ ch2_new.CYVO_COUNT03.dff20_clk(ch2_new.EZOF_COUNT02.qp_new());

    /*#p14.EDEP*/ wire EDEP_CH2_FREQ_03 = not1(ch2_new.CYVO_COUNT03.qn_new());
    /*_p14.FUXO*/ ch2_new.FUXO_COUNT04.dff20_clk(EDEP_CH2_FREQ_03);
    /*_p14.GANO*/ ch2_new.GANO_COUNT05.dff20_clk(ch2_new.FUXO_COUNT04.qp_new());
    /*_p14.GOCA*/ ch2_new.GOCA_COUNT06.dff20_clk(ch2_new.GANO_COUNT05.qp_new());
    /*_p14.GANE*/ ch2_new.GANE_COUNT07.dff20_clk(ch2_new.GOCA_COUNT06.qp_new());

    /*#p14.GALA*/ wire GALA_CH2_FREQ_07 = not1(ch2_new.GANE_COUNT07.qn_new());
    /*_p14.HEVY*/ ch2_new.HEVY_COUNT08.dff20_clk(GALA_CH2_FREQ_07);
    /*_p14.HEPU*/ ch2_new.HEPU_COUNT09.dff20_clk(ch2_new.HEVY_COUNT08.qp_new());
    /*_p14.HERO*/ ch2_new.HERO_COUNT10.dff20_clk(ch2_new.HEPU_COUNT09.qp_new());
  }

  {
    /*#p15.GALU*/ wire GALU_CH2_FREQ_OVERFLOWp_new = not1(ch2_new.HERO_COUNT10.qp_new());
    /*#p15.GYKO*/ ch2_new.GYKO_SAMPLE_CLKp.dff17_clk(GALU_CH2_FREQ_OVERFLOWp_new, ch2_old.GYKO_SAMPLE_CLKp.qn_old());

    /*#p15.FUJY*/ wire FUJY_new = and2(ch2_new.GYKO_SAMPLE_CLKp.qp_new(), reg_new.spu.CEMO_xBCDExxx.qp_new());
    /*#p15.GYRE*/ wire GYRE_RSTn_new = nor3(KEBA_APU_RSTp, ch2_new.ELOX_TRIGp_d01.qp_new(), FUJY_new);
    /*#p15.GYKO*/ ch2_new.GYKO_SAMPLE_CLKp.dff17_rst(GYRE_RSTn_new);

    /*#p15.ETUK*/ wire ETUK_SAMPLE_CLKn_new = not1(ch2_new.GYKO_SAMPLE_CLKp.qp_new());
    /*#p15.DAVU*/ wire DAVU_SAMPLE_CLKp_new = not1(ETUK_SAMPLE_CLKn_new);
    /*#p15.CULE*/ wire CULE_SAMPLE_CLKn_new = not1(DAVU_SAMPLE_CLKp_new);
    /*#p15.DUJU*/ wire DUJU_LOADn_new = nor2(DAVU_SAMPLE_CLKp_new, ch2_new.ELOX_TRIGp_d01.qp_new());
    /*#p15.COGU*/ wire COGU_LOADp_new = not1(DUJU_LOADn_new);
    /*#p15.EROG*/ wire EROG_LOADp_new = not1(DUJU_LOADn_new);
    /*#p15.GYPA*/ wire GYPA_LOADp_new = not1(DUJU_LOADn_new);

    /*_p14.DONE*/ ch2_new.DONE_COUNT00.dff20_any(DOCA_FREQ_TICKn                  , COGU_LOADp_new, ch2_new.FOFE_NR23_FREQ00p.qp_newB());
    /*_p14.DYNU*/ ch2_new.DYNU_COUNT01.dff20_any(ch2_new.DONE_COUNT00.qp_new(), COGU_LOADp_new, ch2_new.FOVA_NR23_FREQ01p.qp_newB());
    /*_p14.EZOF*/ ch2_new.EZOF_COUNT02.dff20_any(ch2_new.DYNU_COUNT01.qp_new(), COGU_LOADp_new, ch2_new.FEDY_NR23_FREQ02p.qp_newB());
    /*_p14.CYVO*/ ch2_new.CYVO_COUNT03.dff20_any(ch2_new.EZOF_COUNT02.qp_new(), COGU_LOADp_new, ch2_new.FOME_NR23_FREQ03p.qp_newB());

    /*#p14.EDEP*/ wire EDEP_CH2_FREQ_03 = not1(ch2_new.CYVO_COUNT03.qn_new());
    /*_p14.FUXO*/ ch2_new.FUXO_COUNT04.dff20_any(EDEP_CH2_FREQ_03                 , EROG_LOADp_new, ch2_new.FORA_NR23_FREQ04p.qp_newB());
    /*_p14.GANO*/ ch2_new.GANO_COUNT05.dff20_any(ch2_new.FUXO_COUNT04.qp_new(), EROG_LOADp_new, ch2_new.GODA_NR23_FREQ05p.qp_newB());
    /*_p14.GOCA*/ ch2_new.GOCA_COUNT06.dff20_any(ch2_new.GANO_COUNT05.qp_new(), EROG_LOADp_new, ch2_new.GUMY_NR23_FREQ06p.qp_newB());
    /*_p14.GANE*/ ch2_new.GANE_COUNT07.dff20_any(ch2_new.GOCA_COUNT06.qp_new(), EROG_LOADp_new, ch2_new.GUPU_NR23_FREQ07p.qp_newB());

    /*#p14.GALA*/ wire GALA_CH2_FREQ_07 = not1(ch2_new.GANE_COUNT07.qn_new());
    /*_p14.HEVY*/ ch2_new.HEVY_COUNT08.dff20_any(GALA_CH2_FREQ_07                 , GYPA_LOADp_new, ch2_new.JEFU_NR24_FREQ08p.qp_newB());
    /*_p14.HEPU*/ ch2_new.HEPU_COUNT09.dff20_any(ch2_new.HEVY_COUNT08.qp_new(), GYPA_LOADp_new, ch2_new.JANY_NR24_FREQ09p.qp_newB());
    /*_p14.HERO*/ ch2_new.HERO_COUNT10.dff20_any(ch2_new.HEPU_COUNT09.qp_new(), GYPA_LOADp_new, ch2_new.JUPY_NR24_FREQ10p.qp_newB());
  }

  {
    /*#p15.GALU*/ wire GALU_CH2_FREQ_OVERFLOWp_new = not1(ch2_new.HERO_COUNT10.qp_new());
    /*#p15.GYKO*/ ch2_new.GYKO_SAMPLE_CLKp.dff17_clk(GALU_CH2_FREQ_OVERFLOWp_new, ch2_old.GYKO_SAMPLE_CLKp.qn_old());
  }

  //----------------------------------------
  // Env tick

  {
    /*#p15.KYVO*/ wire KYVO_CH2_ENV_TICK_old = and3(ch2_old.JEVY_ENV_DELAY2p.qp_old(), ch2_old.JONA_ENV_DELAY1p.qp_old(), ch2_old.JORE_ENV_DELAY0p.qp_old());
    /*#p15.JOPA*/ ch2_new.JOPA_ENV_TICK.dff17_clk(HORU_CLK_512, KYVO_CH2_ENV_TICK_old);

    /*#p15.GADE*/ wire GADE_new = not1(ch2_new.JOPA_ENV_TICK.qp_new());
    /*#p15.HOLY*/ wire HOLY_CLKp_new = nor2(HORU_CLK_512, GADE_new);
    /*#p15.JUPU*/ wire JUPU_ENV_OFFp_new = nor3(ch2_new.HYFU_NR22_P0p.qp_newB(), ch2_new.HORE_NR22_P1p.qp_newB(), ch2_new.HAVA_NR22_P2p.qp_newB());
    /*#p15.HAFE*/ wire HAFE_RSTn_new = nor4(HOLY_CLKp_new, JUPU_ENV_OFFp_new, ch2_new.ELOX_TRIGp_d01.qp_new(), KEBA_APU_RSTp); // schematic wrong this is nor4
    /*#p15.JOPA*/ ch2_new.JOPA_ENV_TICK.dff17_rst(HAFE_RSTn_new);
  }

  //----------------------------------------
  // Env delay

  {
    /*#p15.KYLO*/ wire KYLO_ENV_CLK64_new = not1(ch2_new.JYNA_ENV_CLK64.qp_new());
    /*#p15.KENE*/ wire KENE_ENV_CLK64_new = not1(KYLO_ENV_CLK64_new);
    
    /*_p15.HYLY*/ wire HYLY_ENV_LOADn_new = nor2(ch2_new.ELOX_TRIGp_d01.qp_new(), ch2_new.JOPA_ENV_TICK.qp_new());
    /*_p15.JAKE*/ wire JAKE_ENV_LOADp_new = not1(HYLY_ENV_LOADn_new);
    
    /*#p15.JORE*/ ch2_new.JORE_ENV_DELAY0p.dff20(KENE_ENV_CLK64_new,                    JAKE_ENV_LOADp_new, ch2_new.HYFU_NR22_P0p.qn_newB());
    /*#p15.JONA*/ ch2_new.JONA_ENV_DELAY1p.dff20(ch2_new.JORE_ENV_DELAY0p.qp_new(), JAKE_ENV_LOADp_new, ch2_new.HORE_NR22_P1p.qn_newB());
    /*#p15.JEVY*/ ch2_new.JEVY_ENV_DELAY2p.dff20(ch2_new.JONA_ENV_DELAY1p.qp_new(), JAKE_ENV_LOADp_new, ch2_new.HAVA_NR22_P2p.qn_newB());
  }

  //----------------------------------------
  // Env stop

  {
    /*_p15.HYPA*/ wire HYPA_ENV_MAX_RSTn_new = nor2(ch2_new.ELOX_TRIGp_d01.qp_new(), KEBA_APU_RSTp);

    /*#p15.EMYR*/ wire EMYR_ENV_BOTp_old = nor5 (ch2_old.FORE_NR22_ADDp.qp_oldB(), ch2_old.FENO_ENV_VOL0.qp_old(), ch2_old.FETE_ENV_VOL1.qp_old(), ch2_old.FOMY_ENV_VOL2.qp_old(), ch2_old.FENA_ENV_VOL3.qp_old());
    /*#p15.ERAT*/ wire ERAT_ENV_TOPn_old = nand5(ch2_old.FORE_NR22_ADDp.qp_oldB(), ch2_old.FENO_ENV_VOL0.qp_old(), ch2_old.FETE_ENV_VOL1.qp_old(), ch2_old.FOMY_ENV_VOL2.qp_old(), ch2_old.FENA_ENV_VOL3.qp_old());
    /*#p15.FYRE*/ wire FYRE_ENV_TOPp_old = not1(ERAT_ENV_TOPn_old);
    /*#p15.GUFY*/ wire GUFY_ENV_MAXp_old = or2(EMYR_ENV_BOTp_old, FYRE_ENV_TOPp_old);

    /*#p15.HEPO*/ ch2_new.HEPO_ENV_MAXp.dff17(ch2_new.JOPA_ENV_TICK.qp_new(), HYPA_ENV_MAX_RSTn_new, GUFY_ENV_MAXp_old);
  }

  //----------------------------------------
  // Env increment


  {
    /*_p15.HYLE*/ wire HYLE_ENV_RSTp = or2(KEBA_APU_RSTp, ch2_new.ELOX_TRIGp_d01.qp_new());
    /*#p15.JUPU*/ wire JUPU_ENV_OFFp_new = nor3(ch2_new.HYFU_NR22_P0p.qp_newB(), ch2_new.HORE_NR22_P1p.qp_newB(), ch2_new.HAVA_NR22_P2p.qp_newB());
    /*#p15.JEME*/ ch2_new.JEME_ENV_DONEp.nor_latch(ch2_new.HEPO_ENV_MAXp.qp_new(), HYLE_ENV_RSTp);
    /*_p15.HOFO*/ wire HOFO_ENV_CLK = or3(ch2_new.JOPA_ENV_TICK.qp_new(), JUPU_ENV_OFFp_new, ch2_new.JEME_ENV_DONEp.qp_new());

    /*#p15.GAFA*/ wire GAFA_ENV_CLK = amux2(ch2_new.FORE_NR22_ADDp.qp_newB(), HOFO_ENV_CLK,                       HOFO_ENV_CLK,                       ch2_new.FORE_NR22_ADDp.qn_newB());
    /*#p15.FENO*/ ch2_new.FENO_ENV_VOL0.dff20(GAFA_ENV_CLK, ch2_new.ELOX_TRIGp_d01.qp_new(), ch2_new.GATA_NR22_V0p.qp_newB());

    /*#p15.FARU*/ wire FARU_ENV_CLK = amux2(ch2_new.FORE_NR22_ADDp.qp_newB(), ch2_new.FENO_ENV_VOL0.qp_new(), ch2_new.FENO_ENV_VOL0.qn_new(), ch2_new.FORE_NR22_ADDp.qn_newB());
    /*#p15.FETE*/ ch2_new.FETE_ENV_VOL1.dff20(FARU_ENV_CLK, ch2_new.ELOX_TRIGp_d01.qp_new(), ch2_new.GUFE_NR22_V1p.qp_newB());

    /*#p15.ETUP*/ wire ETUP_ENV_CLK = amux2(ch2_new.FORE_NR22_ADDp.qp_newB(), ch2_new.FETE_ENV_VOL1.qp_new(), ch2_new.FETE_ENV_VOL1.qn_new(), ch2_new.FORE_NR22_ADDp.qn_newB());
    /*#p15.FOMY*/ ch2_new.FOMY_ENV_VOL2.dff20(ETUP_ENV_CLK, ch2_new.ELOX_TRIGp_d01.qp_new(), ch2_new.GURA_NR22_V2p.qp_newB());

    /*#p15.FOPY*/ wire FOPY_ENV_CLK = amux2(ch2_new.FORE_NR22_ADDp.qp_newB(), ch2_new.FOMY_ENV_VOL2.qp_new(), ch2_new.FOMY_ENV_VOL2.qn_new(), ch2_new.FORE_NR22_ADDp.qn_newB());
    /*#p15.FENA*/ ch2_new.FENA_ENV_VOL3.dff20(FOPY_ENV_CLK, ch2_new.ELOX_TRIGp_d01.qp_new(), ch2_new.GAGE_NR22_V3p.qp_newB());
  }

  //----------------------------------------
  // Len tick

  {
    // using CYRE_old to break the loop

    /*_p15.DEME*/ wire DEME_LEN_CLKn_new = nor3(ch2_old.CYRE_LEN_DONEp.qp_old(), BUFY_CLK_256, ch2_new.EMER_NR24_LENENp.qn_newB()); // why was this or2?
    /*_p15.DYRO*/ wire DYRO_LEN_CLKp_new = not1(DEME_LEN_CLKn_new);

    /*_p14.AGYN*/ wire AGYN_FF16_WRn_new = nand2(BOGY_CPU_WRp, COVY_ADDR_FF16p);
    /*_p15.BYMO*/ wire BYMO_FF16_WRp_new = not1(AGYN_FF16_WRn_new);
    /*_p15.AGET*/ wire AGET_FF16_WRp_new = not1(AGYN_FF16_WRn_new);
    /*_p14.ASYP*/ wire ASYP_FF16_WRp_new = not1(AGYN_FF16_WRn_new);

    /*_p15.ERYC*/ ch2_new.ERYC_NR21_LEN0.dff20(DYRO_LEN_CLKp_new,                   BYMO_FF16_WRp_new, reg_new.cpu_dbus.BUS_CPU_D00p.out_new());
    /*_p15.CERA*/ ch2_new.CERA_NR21_LEN1.dff20(ch2_new.ERYC_NR21_LEN0.qp_new(), BYMO_FF16_WRp_new, reg_new.cpu_dbus.BUS_CPU_D01p.out_new());
    /*_p15.CONU*/ ch2_new.CONU_NR21_LEN2.dff20(ch2_new.CERA_NR21_LEN1.qp_new(), BYMO_FF16_WRp_new, reg_new.cpu_dbus.BUS_CPU_D02p.out_new());
    /*_p15.CAME*/ ch2_new.CAME_NR21_LEN3.dff20(ch2_new.CONU_NR21_LEN2.qp_new(), BYMO_FF16_WRp_new, reg_new.cpu_dbus.BUS_CPU_D03p.out_new());

    /*#p15.BUKO*/ wire BUKO_CH2_LEN_CLK = not1(ch2_new.CAME_NR21_LEN3.qn_new());
    /*_p15.BUVA*/ ch2_new.BUVA_NR21_LEN4.dff20(BUKO_CH2_LEN_CLK,                    AGET_FF16_WRp_new, reg_new.cpu_dbus.BUS_CPU_D04p.out_new());
    /*_p15.AKYD*/ ch2_new.AKYD_NR21_LEN5.dff20(ch2_new.BUVA_NR21_LEN4.qp_new(), AGET_FF16_WRp_new, reg_new.cpu_dbus.BUS_CPU_D05p.out_new());

    /*_p14.BENY*/ wire BENY_RSTn = nor3(ASYP_FF16_WRp_new, KEBA_APU_RSTp, ch2_new.ELOX_TRIGp_d01.qp_new());
    /*_p15.CYRE*/ ch2_new.CYRE_LEN_DONEp.dff17(ch2_new.AKYD_NR21_LEN5.qn_new(), BENY_RSTn, ch2_old.CYRE_LEN_DONEp.qn_old());
  }

  //----------------------------------------
  // CH2 duty cycle & output to DAC

  {
    /*#p09.AFAT*/ wire AFAT_APU_RSTn_new = not1(KEBA_APU_RSTp);
    /*_p14.BACU*/ wire BACU_FF16_WRp = and2(COVY_ADDR_FF16p, BOGY_CPU_WRp);
    /*_p14.BUDU*/ wire BUDU_FF16_WRn = not1(BACU_FF16_WRp);
    /*_p14.BERA*/ ch2_new.BERA_NR21_DUTY0.dff9b(BUDU_FF16_WRn, AFAT_APU_RSTn_new, reg_old.cpu_dbus.BUS_CPU_D06p.out_old());
    /*_p14.BAMY*/ ch2_new.BAMY_NR21_DUTY1.dff9b(BUDU_FF16_WRn, AFAT_APU_RSTn_new, reg_old.cpu_dbus.BUS_CPU_D07p.out_old());

    /*#p15.ETUK*/ wire ETUK_SAMPLE_CLKn_new = not1(ch2_new.GYKO_SAMPLE_CLKp.qp_new());
    /*#p15.DAVU*/ wire DAVU_SAMPLE_CLKp_new = not1(ETUK_SAMPLE_CLKn_new);
    /*#p15.CULE*/ wire CULE_SAMPLE_CLKn_new = not1(DAVU_SAMPLE_CLKp_new);

    /*#p15.CANO*/ ch2_new.CANO_PHASE0.dff17(CULE_SAMPLE_CLKn_new,             AFAT_APU_RSTn_new, ch2_old.CANO_PHASE0.qn_old());
    /*#p15.CAGY*/ ch2_new.CAGY_PHASE1.dff13(ch2_new.CANO_PHASE0.qn_new(), AFAT_APU_RSTn_new, ch2_old.CAGY_PHASE1.qn_old());
    /*#p15.DYVE*/ ch2_new.DYVE_PHASE2.dff13(ch2_new.CAGY_PHASE1.qn_new(), AFAT_APU_RSTn_new, ch2_old.DYVE_PHASE2.qn_old());

    /*#p15.DOJU*/ wire DOJU = nor2(ch2_new.BERA_NR21_DUTY0.qp_newB(), ch2_new.BAMY_NR21_DUTY1.qn_newB());
    /*#p15.DYTA*/ wire DYTA = nor2(ch2_new.BERA_NR21_DUTY0.qn_newB(), ch2_new.BAMY_NR21_DUTY1.qp_newB());
    /*#p15.DOMO*/ wire DOMO = nor2(ch2_new.BERA_NR21_DUTY0.qp_newB(), ch2_new.BAMY_NR21_DUTY1.qp_newB());
    /*#p15.DOVE*/ wire DOVE = nor2(ch2_new.BERA_NR21_DUTY0.qn_newB(), ch2_new.BAMY_NR21_DUTY1.qn_newB());
    /*#p15.DUGE*/ wire DUGE = not1(ch2_new.CANO_PHASE0.qp_new());
    /*#p15.DYMU*/ wire DYMU = and2(ch2_new.DYVE_PHASE2.qp_new(), ch2_new.CAGY_PHASE1.qp_new());
    /*#p15.EGOG*/ wire EGOG = and2(DYMU, DUGE);
    /*#p15.DARE*/ wire DARE = not1(DYMU);
    /*#p15.EXES*/ wire EXES_WAVE = amux4(EGOG, DOMO, DYMU, DYTA, ch2_new.DYVE_PHASE2.qp_new(), DOJU, DARE, DOVE);
    /*#p15.DOME*/ ch2_new.DOME_CH2_OUTp.dff17(DAVU_SAMPLE_CLKp_new, AFAT_APU_RSTn_new, EXES_WAVE);

    // dac 2 inputs ANAN ANYV ASOG AMOV
    /*_p15.DORA*/ wire DORA_CH2_STOPp_new = and2(ch2_new.CYRE_LEN_DONEp.qp_new(), ch2_new.EMER_NR24_LENENp.qp_newB());
    /*_p15.ESYK*/ wire ESYK_CH2_STOPp_new = or3(KEBA_APU_RSTp, DORA_CH2_STOPp_new, FUTE_CH2_AMP_ENn_new);
    /*_p15.DANE*/ ch2_new.DANE_CH2_ACTIVEp.nor_latch(ch2_new.ELOX_TRIGp_d01.qp_new(), ESYK_CH2_STOPp_new);
  }



  //----------------------------------------
  // NR21 read

  {
    /*_p14.BYGO*/ wire BYGO_CPU_RDp = not1(AGUZ_CPU_RDn);
    /*_p14.CORO*/ wire CORO_NR21_RDn = nand2(COVY_ADDR_FF16p, BYGO_CPU_RDp);
    /*_p14.CECY*/ triwire CECY_D6 = tri6_nn(CORO_NR21_RDn, ch2_new.BERA_NR21_DUTY0.qn_newB()); // polarity?
    /*_p14.CEKA*/ triwire CEKA_D7 = tri6_nn(CORO_NR21_RDn, ch2_new.BAMY_NR21_DUTY1.qn_newB());
    
    reg_new.cpu_dbus.BUS_CPU_D06p.tri_bus(CECY_D6);
    reg_new.cpu_dbus.BUS_CPU_D07p.tri_bus(CEKA_D7);
  }

  //----------------------------------------
  // NR22 read

  {
    /*_p14.GURE*/ wire GURE = not1(DUTU_ADDR_FF17p);
    /*_p14.FYRY*/ wire FYRY = not1(DUTU_ADDR_FF17p);
    /*_p14.GEXA*/ wire GEXA = or2(GURE, AGUZ_CPU_RDn);
    /*_p14.GURU*/ wire GURU = or2(FYRY, AGUZ_CPU_RDn);

    /*_p14.HUVU*/ triwire HUVU_D0 = tri6_nn(GEXA, ch2_new.HYFU_NR22_P0p .qn_newB());
    /*_p14.HYRE*/ triwire HYRE_D1 = tri6_nn(GEXA, ch2_new.HORE_NR22_P1p .qn_newB());
    /*_p14.HAVU*/ triwire HAVU_D2 = tri6_nn(GEXA, ch2_new.HAVA_NR22_P2p .qn_newB());
    /*_p14.GENE*/ triwire GENE_D3 = tri6_nn(GURU, ch2_new.FORE_NR22_ADDp.qn_newB());
    /*_p14.HUPE*/ triwire HUPE_D4 = tri6_nn(GURU, ch2_new.GATA_NR22_V0p .qn_newB());
    /*_p14.HERE*/ triwire HERE_D5 = tri6_nn(GURU, ch2_new.GUFE_NR22_V1p .qn_newB());
    /*_p14.HORO*/ triwire HORO_D6 = tri6_nn(GURU, ch2_new.GURA_NR22_V2p .qn_newB());
    /*_p14.HYRY*/ triwire HYRY_D7 = tri6_nn(GURU, ch2_new.GAGE_NR22_V3p .qn_newB());

    reg_new.cpu_dbus.BUS_CPU_D00p.tri_bus(HUVU_D0);
    reg_new.cpu_dbus.BUS_CPU_D01p.tri_bus(HYRE_D1);
    reg_new.cpu_dbus.BUS_CPU_D02p.tri_bus(HAVU_D2);
    reg_new.cpu_dbus.BUS_CPU_D03p.tri_bus(GENE_D3);
    reg_new.cpu_dbus.BUS_CPU_D04p.tri_bus(HUPE_D4);
    reg_new.cpu_dbus.BUS_CPU_D05p.tri_bus(HERE_D5);
    reg_new.cpu_dbus.BUS_CPU_D06p.tri_bus(HORO_D6);
    reg_new.cpu_dbus.BUS_CPU_D07p.tri_bus(HYRY_D7);
  }

  //----------------------------------------
  // NR23 read? this can't be right...

  /*_p14.FOGE*/ wire FOGE_CPU_RDp = not1(AGUZ_CPU_RDn);
  /*_p09.EDEK*/ wire EDEK_NR52_DBG_APUp = not1(reg_new.spu.FERO_NR52_DBG_APUp.qn_any());
  /*#p14.FAPE*/ wire FAPE_CPU_RDp_DBGn = nand2(FOGE_CPU_RDp, EDEK_NR52_DBG_APUp); // schematic wrong? was and2

  {
    /*#p14.FERY*/ wire FERY_A03n = not1(DUCE_A03p);
    /*#p14.GUZA*/ wire GUZA = nor2(FERY_A03n, FAPE_CPU_RDp_DBGn);
    /*#p14.FUTY*/ wire FUTY = not1(GUZA);

    /*#p14.FAVA*/ triwire FAVA_D0 = tri6_nn(FUTY, ch2_new.DONE_COUNT00.qn_new());
    /*#p14.FAJY*/ triwire FAJY_D1 = tri6_nn(FUTY, ch2_new.DYNU_COUNT01.qn_new());
    /*#p14.FEGU*/ triwire FEGU_D2 = tri6_nn(FUTY, ch2_new.EZOF_COUNT02.qn_new());
    /*#p14.FOSE*/ triwire FOSE_D3 = tri6_nn(FUTY, ch2_new.CYVO_COUNT03.qn_new());
    /*#p14.GERO*/ triwire GERO_D4 = tri6_nn(FUTY, ch2_new.FUXO_COUNT04.qn_new()); // d4 and d5 are switched on the schematic
    /*#p14.GAKY*/ triwire GAKY_D5 = tri6_nn(FUTY, ch2_new.GANO_COUNT05.qn_new());
    /*#p14.GADU*/ triwire GADU_D6 = tri6_nn(FUTY, ch2_new.GOCA_COUNT06.qn_new());
    /*#p14.GAZO*/ triwire GAZO_D7 = tri6_nn(FUTY, ch2_new.GANE_COUNT07.qn_new());

    reg_new.cpu_dbus.BUS_CPU_D00p.tri_bus(FAVA_D0);
    reg_new.cpu_dbus.BUS_CPU_D01p.tri_bus(FAJY_D1);
    reg_new.cpu_dbus.BUS_CPU_D02p.tri_bus(FEGU_D2);
    reg_new.cpu_dbus.BUS_CPU_D03p.tri_bus(FOSE_D3);
    reg_new.cpu_dbus.BUS_CPU_D04p.tri_bus(GERO_D4);
    reg_new.cpu_dbus.BUS_CPU_D05p.tri_bus(GAKY_D5);
    reg_new.cpu_dbus.BUS_CPU_D06p.tri_bus(GADU_D6);
    reg_new.cpu_dbus.BUS_CPU_D07p.tri_bus(GAZO_D7);
  }

  //----------------------------------------
  // NR24 read

  {
    /*#pXX.GADO*/ wire GADO_CPU_RDp = not1(AGUZ_CPU_RDn);
    /*#p14.GOTE*/ wire GOTE_ADDR_FF19n = not1(DOZA_ADDR_FF19p);
    /*#p14.HYPO*/ wire HYPO_NR24_RDn = or2(GOTE_ADDR_FF19n, FAPE_CPU_RDp_DBGn);
    /*#pXX.HUMA*/ wire HUMA_NR24_RDn = nand2(DOZA_ADDR_FF19p, GADO_CPU_RDp);

    /*#p14.HUNA*/ triwire HUNA_D0 = tri6_nn(HYPO_NR24_RDn, ch2_new.HEVY_COUNT08.qn_new());
    /*#p14.JARO*/ triwire JARO_D1 = tri6_nn(HYPO_NR24_RDn, ch2_new.HEPU_COUNT09.qn_new());
    /*#p14.JEKE*/ triwire JEKE_D2 = tri6_nn(HYPO_NR24_RDn, ch2_new.HERO_COUNT10.qn_new());
    /*#pXX.GOJY*/ triwire GOJY_D6 = tri6_nn(HUMA_NR24_RDn, ch2_new.EMER_NR24_LENENp.qn_newB());

    reg_new.cpu_dbus.BUS_CPU_D00p.tri_bus(HUNA_D0);
    reg_new.cpu_dbus.BUS_CPU_D01p.tri_bus(JARO_D1);
    reg_new.cpu_dbus.BUS_CPU_D02p.tri_bus(JEKE_D2);
    reg_new.cpu_dbus.BUS_CPU_D06p.tri_bus(GOJY_D6);
  }
}

#endif
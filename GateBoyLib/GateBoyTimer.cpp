#include "GateBoyLib/GateBoyTimer.h"

#include "GateBoyLib/GateBoy.h"

//------------------------------------------------------------------------------------------------------------------------
// this is hacked up because we're ignoring the debug reg for the moment
///* p01.ULUR*/ wire _ULUR_DIV_06_clknew = /*mux2p(FF60_1, _BOGA_Axxxxxxx,*/ TAMA_DIV05p.qn_new() /*)*/;
///* p01.UGOT*/ UGOT_DIV06p.dff17_clk(_ULUR_DIV_06_clknew,  UGOT_DIV06p.qn_old());

void GateBoy::tock_div() {
  /* p01.TAPE*/ wire _TAPE_FF04_WRp = and4(cpu_bus.TAPU_CPU_WRp.qp_new(), new_bus.RYFO_FF04_FF07p(), new_bus.TOLA_A01n(), new_bus.TOVY_A00n());
  /* p01.UFOL*/ wire _UFOL_DIV_RSTn = nor3(UCOB_CLKBADp(), rst.PIN_71_RST.qp_new(), _TAPE_FF04_WRp);

  /* p01.UKUP*/ div.UKUP_DIV00p.dff17(BOGA_Axxxxxxx(),          _UFOL_DIV_RSTn, div.UKUP_DIV00p.qn_old());
  /* p01.UFOR*/ div.UFOR_DIV01p.dff17(div.UKUP_DIV00p.qn_any(), _UFOL_DIV_RSTn, div.UFOR_DIV01p.qn_old());
  /* p01.UNER*/ div.UNER_DIV02p.dff17(div.UFOR_DIV01p.qn_any(), _UFOL_DIV_RSTn, div.UNER_DIV02p.qn_old());
  /*#p01.TERO*/ div.TERO_DIV03p.dff17(div.UNER_DIV02p.qn_any(), _UFOL_DIV_RSTn, div.TERO_DIV03p.qn_old());
  /* p01.UNYK*/ div.UNYK_DIV04p.dff17(div.TERO_DIV03p.qn_any(), _UFOL_DIV_RSTn, div.UNYK_DIV04p.qn_old());
  /* p01.TAMA*/ div.TAMA_DIV05p.dff17(div.UNYK_DIV04p.qn_any(), _UFOL_DIV_RSTn, div.TAMA_DIV05p.qn_old());
  /* p01.UGOT*/ div.UGOT_DIV06p.dff17(div.TAMA_DIV05p.qn_any(), _UFOL_DIV_RSTn, div.UGOT_DIV06p.qn_old());
  /* p01.TULU*/ div.TULU_DIV07p.dff17(div.UGOT_DIV06p.qn_any(), _UFOL_DIV_RSTn, div.TULU_DIV07p.qn_old());
  /* p01.TUGO*/ div.TUGO_DIV08p.dff17(div.TULU_DIV07p.qn_any(), _UFOL_DIV_RSTn, div.TUGO_DIV08p.qn_old());
  /* p01.TOFE*/ div.TOFE_DIV09p.dff17(div.TUGO_DIV08p.qn_any(), _UFOL_DIV_RSTn, div.TOFE_DIV09p.qn_old());
  /* p01.TERU*/ div.TERU_DIV10p.dff17(div.TOFE_DIV09p.qn_any(), _UFOL_DIV_RSTn, div.TERU_DIV10p.qn_old());
  /* p01.SOLA*/ div.SOLA_DIV11p.dff17(div.TERU_DIV10p.qn_any(), _UFOL_DIV_RSTn, div.SOLA_DIV11p.qn_old());
  /* p01.SUBU*/ div.SUBU_DIV12p.dff17(div.SOLA_DIV11p.qn_any(), _UFOL_DIV_RSTn, div.SUBU_DIV12p.qn_old());
  /* p01.TEKA*/ div.TEKA_DIV13p.dff17(div.SUBU_DIV12p.qn_any(), _UFOL_DIV_RSTn, div.TEKA_DIV13p.qn_old());
  /* p01.UKET*/ div.UKET_DIV14p.dff17(div.TEKA_DIV13p.qn_any(), _UFOL_DIV_RSTn, div.UKET_DIV14p.qn_old());
  /* p01.UPOF*/ div.UPOF_DIV15p.dff17(div.UKET_DIV14p.qn_any(), _UFOL_DIV_RSTn, div.UPOF_DIV15p.qn_old());

  /* p01.UMEK*/ wire _UMEK_DIV06n = not1(div.UGOT_DIV06p.qp_new());
  /* p01.UREK*/ wire _UREK_DIV07n = not1(div.TULU_DIV07p.qp_new());
  /* p01.UTOK*/ wire _UTOK_DIV08n = not1(div.TUGO_DIV08p.qp_new());
  /* p01.SAPY*/ wire _SAPY_DIV09n = not1(div.TOFE_DIV09p.qp_new());
  /* p01.UMER*/ wire _UMER_DIV10n = not1(div.TERU_DIV10p.qp_new());
  /* p01.RAVE*/ wire _RAVE_DIV11n = not1(div.SOLA_DIV11p.qp_new());
  /* p01.RYSO*/ wire _RYSO_DIV12n = not1(div.SUBU_DIV12p.qp_new());
  /* p01.UDOR*/ wire _UDOR_DIV13n = not1(div.TEKA_DIV13p.qp_new());

  /* p01.TAGY*/ wire _TAGY_FF04_RDp_ext = and4(cpu_bus.TEDO_CPU_RDp.qp_new(), new_bus.RYFO_FF04_FF07p(), new_bus.TOLA_A01n(), new_bus.TOVY_A00n());
  /* p01.TAWU_DIV06_TO_CD0*/ new_bus.BUS_CPU_D00p.tri6_pn(_TAGY_FF04_RDp_ext, _UMEK_DIV06n);
  /* p01.TAKU_DIV07_TO_CD1*/ new_bus.BUS_CPU_D01p.tri6_pn(_TAGY_FF04_RDp_ext, _UREK_DIV07n);
  /* p01.TEMU_DIV08_TO_CD2*/ new_bus.BUS_CPU_D02p.tri6_pn(_TAGY_FF04_RDp_ext, _UTOK_DIV08n);
  /* p01.TUSE_DIV09_TO_CD3*/ new_bus.BUS_CPU_D03p.tri6_pn(_TAGY_FF04_RDp_ext, _SAPY_DIV09n);
  /* p01.UPUG_DIV10_TO_CD4*/ new_bus.BUS_CPU_D04p.tri6_pn(_TAGY_FF04_RDp_ext, _UMER_DIV10n); // Schematic wrong, UPUG/SEPU driving D5/D4
  /* p01.SEPU_DIV11_TO_CD5*/ new_bus.BUS_CPU_D05p.tri6_pn(_TAGY_FF04_RDp_ext, _RAVE_DIV11n);
  /* p01.SAWA_DIV12_TO_CD6*/ new_bus.BUS_CPU_D06p.tri6_pn(_TAGY_FF04_RDp_ext, _RYSO_DIV12n);
  /* p01.TATU_DIV13_TO_CD7*/ new_bus.BUS_CPU_D07p.tri6_pn(_TAGY_FF04_RDp_ext, _UDOR_DIV13n);
}

//------------------------------------------------------------------------------------------------------------------------

void GateBoy::tock_timer() {
  /* p03.TYJU*/ wire _TYJU_FF06_WRn = nand4(cpu_bus.TAPU_CPU_WRp.qp_new(), new_bus.RYFO_FF04_FF07p(), new_bus.BUS_CPU_A01p.qp_new(), new_bus.TOVY_A00n());
  /* p03.SABU*/ timer.SABU_TMA0p.dff17(_TYJU_FF06_WRn, ALUR_SYS_RSTn(), old_bus.BUS_CPU_D00p.qp_old());
  /* p03.NYKE*/ timer.NYKE_TMA1p.dff17(_TYJU_FF06_WRn, ALUR_SYS_RSTn(), old_bus.BUS_CPU_D01p.qp_old());
  /* p03.MURU*/ timer.MURU_TMA2p.dff17(_TYJU_FF06_WRn, ALUR_SYS_RSTn(), old_bus.BUS_CPU_D02p.qp_old());
  /* p03.TYVA*/ timer.TYVA_TMA3p.dff17(_TYJU_FF06_WRn, ALUR_SYS_RSTn(), old_bus.BUS_CPU_D03p.qp_old());
  /* p03.TYRU*/ timer.TYRU_TMA4p.dff17(_TYJU_FF06_WRn, ALUR_SYS_RSTn(), old_bus.BUS_CPU_D04p.qp_old());
  /* p03.SUFY*/ timer.SUFY_TMA5p.dff17(_TYJU_FF06_WRn, ALUR_SYS_RSTn(), old_bus.BUS_CPU_D05p.qp_old());
  /* p03.PETO*/ timer.PETO_TMA6p.dff17(_TYJU_FF06_WRn, ALUR_SYS_RSTn(), old_bus.BUS_CPU_D06p.qp_old());
  /* p03.SETA*/ timer.SETA_TMA7p.dff17(_TYJU_FF06_WRn, ALUR_SYS_RSTn(), old_bus.BUS_CPU_D07p.qp_old());

  /* p03.SARA*/ wire _SARA_FF07_WRn = nand4(cpu_bus.TAPU_CPU_WRp.qp_new(), new_bus.RYFO_FF04_FF07p(), new_bus.BUS_CPU_A01p.qp_new(), new_bus.BUS_CPU_A00p.qp_new());
  /* p03.SOPU*/ timer.SOPU_TAC0p.dff17(_SARA_FF07_WRn, ALUR_SYS_RSTn(), old_bus.BUS_CPU_D00p.qp_old());
  /* p03.SAMY*/ timer.SAMY_TAC1p.dff17(_SARA_FF07_WRn, ALUR_SYS_RSTn(), old_bus.BUS_CPU_D01p.qp_old());
  /* p03.SABO*/ timer.SABO_TAC2p.dff17(_SARA_FF07_WRn, ALUR_SYS_RSTn(), old_bus.BUS_CPU_D02p.qp_old());

  /*#p03.MERY*/ wire _MERY_TIMER_OVERFLOWp_old = nor2(timer.NUGA_TIMA7p.qp_old(), timer.NYDU_TIMA7p_DELAY.qn_old());
  /*#p03.MOBA*/ timer.MOBA_TIMER_OVERFLOWp.dff17(BOGA_Axxxxxxx(), ALUR_SYS_RSTn(), _MERY_TIMER_OVERFLOWp_old);

  /*#p03.TOPE*/ wire _TOPE_FF05_WRn = nand4(cpu_bus.TAPU_CPU_WRp.qp_new(), new_bus.RYFO_FF04_FF07p(),  new_bus.TOLA_A01n(), new_bus.BUS_CPU_A00p.qp_new());
  /*#p03.MUZU*/ wire _MUZU_CPU_LOAD_TIMAn  = or2(cpu_bus.SIG_CPU_LATCH_EXT.qp_new(), _TOPE_FF05_WRn);
  /*#p03.MEKE*/ wire _MEKE_TIMER_OVERFLOWn = not1(timer.MOBA_TIMER_OVERFLOWp.qp_new());
  /*#p03.MEXU*/ wire _MEXU_TIMA_LOADp      = nand3(_MUZU_CPU_LOAD_TIMAn, ALUR_SYS_RSTn(), _MEKE_TIMER_OVERFLOWn);
  /*#p03.MUGY*/ wire _MUGY_TIMA_MAX_RSTn   = not1(_MEXU_TIMA_LOADp);
  /*#p03.NYDU*/ timer.NYDU_TIMA7p_DELAY.dff17(BOGA_Axxxxxxx(), _MUGY_TIMA_MAX_RSTn, timer.NUGA_TIMA7p.qp_old());

  /*#p03.UBOT*/ wire _UBOT_DIV01n = not1(div.UFOR_DIV01p.qp_new());
  /*#p03.UVYR*/ wire _UVYR_DIV03n = not1(div.TERO_DIV03p.qp_new());
  /*#p01.UVYN*/ wire _UVYN_DIV05n = not1(div.TAMA_DIV05p.qp_new());
  /* p01.UREK*/ wire _UREK_DIV07n = not1(div.TULU_DIV07p.qp_new());

  /*#p03.UKAP*/ wire _UKAP_CLK_MUXa  = mux2n(timer.SOPU_TAC0p.qp_new(), _UVYN_DIV05n, _UVYR_DIV03n);
  /*#p03.TEKO*/ wire _TEKO_CLK_MUXb  = mux2n(timer.SOPU_TAC0p.qp_new(), _UBOT_DIV01n, _UREK_DIV07n);
  /*#p03.TECY*/ wire _TECY_CLK_MUXc  = mux2n(timer.SAMY_TAC1p.qp_new(), _UKAP_CLK_MUXa, _TEKO_CLK_MUXb);
  /*#p03.SOGU*/ wire _SOGU_TIMA_CLKn = nor2(_TECY_CLK_MUXc, timer.SABO_TAC2p.qn_new());

  ///*#p03.TOPE*/ wire _TOPE_FF05_WRn = nand4(cpu_bus.TAPU_CPU_WRp.qp_new(), cpu_bus.RYFO_FF04_FF07p(),  cpu_bus.TOLA_A01n(), cpu_bus.BUS_CPU_A00p.qp_new());

  /*#p03.ROKE*/ wire _ROKE_TIMA_D0 = mux2n(_TOPE_FF05_WRn, timer.SABU_TMA0p.qp_new(), new_bus.BUS_CPU_D00p.qp_new());
  /*#p03.PETU*/ wire _PETU_TIMA_D1 = mux2n(_TOPE_FF05_WRn, timer.NYKE_TMA1p.qp_new(), new_bus.BUS_CPU_D01p.qp_new());
  /*#p03.NYKU*/ wire _NYKU_TIMA_D2 = mux2n(_TOPE_FF05_WRn, timer.MURU_TMA2p.qp_new(), new_bus.BUS_CPU_D02p.qp_new());
  /*#p03.SOCE*/ wire _SOCE_TIMA_D3 = mux2n(_TOPE_FF05_WRn, timer.TYVA_TMA3p.qp_new(), new_bus.BUS_CPU_D03p.qp_new());
  /*#p03.SALA*/ wire _SALA_TIMA_D4 = mux2n(_TOPE_FF05_WRn, timer.TYRU_TMA4p.qp_new(), new_bus.BUS_CPU_D04p.qp_new());
  /*#p03.SYRU*/ wire _SYRU_TIMA_D5 = mux2n(_TOPE_FF05_WRn, timer.SUFY_TMA5p.qp_new(), new_bus.BUS_CPU_D05p.qp_new());
  /*#p03.REFU*/ wire _REFU_TIMA_D6 = mux2n(_TOPE_FF05_WRn, timer.PETO_TMA6p.qp_new(), new_bus.BUS_CPU_D06p.qp_new());
  /*#p03.RATO*/ wire _RATO_TIMA_D7 = mux2n(_TOPE_FF05_WRn, timer.SETA_TMA7p.qp_new(), new_bus.BUS_CPU_D07p.qp_new());

  /*#p03.PUXY*/ wire _PUXY_TIMA_D0 = nor2(MULO_SYS_RSTn(), _ROKE_TIMA_D0);
  /*#p03.NERO*/ wire _NERO_TIMA_D1 = nor2(MULO_SYS_RSTn(), _PETU_TIMA_D1);
  /*#p03.NADA*/ wire _NADA_TIMA_D2 = nor2(MULO_SYS_RSTn(), _NYKU_TIMA_D2);
  /*#p03.REPA*/ wire _REPA_TIMA_D3 = nor2(MULO_SYS_RSTn(), _SOCE_TIMA_D3);
  /*#p03.ROLU*/ wire _ROLU_TIMA_D4 = nor2(MULO_SYS_RSTn(), _SALA_TIMA_D4);
  /*#p03.RUGY*/ wire _RUGY_TIMA_D5 = nor2(MULO_SYS_RSTn(), _SYRU_TIMA_D5);
  /*#p03.PYMA*/ wire _PYMA_TIMA_D6 = nor2(MULO_SYS_RSTn(), _REFU_TIMA_D6);
  /*#p03.PAGU*/ wire _PAGU_TIMA_D7 = nor2(MULO_SYS_RSTn(), _RATO_TIMA_D7);

  // FIXME puxy etc should be old

  /*#p03.REGA*/ timer.REGA_TIMA0p.dff20(_SOGU_TIMA_CLKn,            _MEXU_TIMA_LOADp, _PUXY_TIMA_D0);
  /*#p03.POVY*/ timer.POVY_TIMA1p.dff20(timer.REGA_TIMA0p.qp_any(), _MEXU_TIMA_LOADp, _NERO_TIMA_D1);
  /*#p03.PERU*/ timer.PERU_TIMA2p.dff20(timer.POVY_TIMA1p.qp_any(), _MEXU_TIMA_LOADp, _NADA_TIMA_D2);
  /*#p03.RATE*/ timer.RATE_TIMA3p.dff20(timer.PERU_TIMA2p.qp_any(), _MEXU_TIMA_LOADp, _REPA_TIMA_D3);
  /*#p03.RUBY*/ timer.RUBY_TIMA4p.dff20(timer.RATE_TIMA3p.qp_any(), _MEXU_TIMA_LOADp, _ROLU_TIMA_D4);
  /*#p03.RAGE*/ timer.RAGE_TIMA5p.dff20(timer.RUBY_TIMA4p.qp_any(), _MEXU_TIMA_LOADp, _RUGY_TIMA_D5);
  /*#p03.PEDA*/ timer.PEDA_TIMA6p.dff20(timer.RAGE_TIMA5p.qp_any(), _MEXU_TIMA_LOADp, _PYMA_TIMA_D6);
  /*#p03.NUGA*/ timer.NUGA_TIMA7p.dff20(timer.PEDA_TIMA6p.qp_any(), _MEXU_TIMA_LOADp, _PAGU_TIMA_D7);

  /*#p03.TEDA*/ wire _TEDA_FF05_RDp =  and4(cpu_bus.TEDO_CPU_RDp.qp_new(), new_bus.RYFO_FF04_FF07p(),  new_bus.TOLA_A01n(), new_bus.BUS_CPU_A00p.qp_new());
  /*#p03.SOKU_TIMA0_TO_CD0*/ new_bus.BUS_CPU_D00p.tri6_pn(_TEDA_FF05_RDp, timer.REGA_TIMA0p.qn_new());
  /*#p03.RACY_TIMA1_TO_CD1*/ new_bus.BUS_CPU_D01p.tri6_pn(_TEDA_FF05_RDp, timer.POVY_TIMA1p.qn_new());
  /*#p03.RAVY_TIMA2_TO_CD2*/ new_bus.BUS_CPU_D02p.tri6_pn(_TEDA_FF05_RDp, timer.PERU_TIMA2p.qn_new());
  /*#p03.SOSY_TIMA3_TO_CD3*/ new_bus.BUS_CPU_D03p.tri6_pn(_TEDA_FF05_RDp, timer.RATE_TIMA3p.qn_new());
  /*#p03.SOMU_TIMA4_TO_CD4*/ new_bus.BUS_CPU_D04p.tri6_pn(_TEDA_FF05_RDp, timer.RUBY_TIMA4p.qn_new());
  /*#p03.SURO_TIMA5_TO_CD5*/ new_bus.BUS_CPU_D05p.tri6_pn(_TEDA_FF05_RDp, timer.RAGE_TIMA5p.qn_new());
  /*#p03.ROWU_TIMA6_TO_CD6*/ new_bus.BUS_CPU_D06p.tri6_pn(_TEDA_FF05_RDp, timer.PEDA_TIMA6p.qn_new());
  /*#p03.PUSO_TIMA7_TO_CD7*/ new_bus.BUS_CPU_D07p.tri6_pn(_TEDA_FF05_RDp, timer.NUGA_TIMA7p.qn_new());

  /* p03.TUBY*/ wire _TUBY_FF06_RDp =  and4(cpu_bus.TEDO_CPU_RDp.qp_new(), new_bus.RYFO_FF04_FF07p(), new_bus.BUS_CPU_A01p.qp_new(), new_bus.TOVY_A00n());
  /*#p03.SETE_TMA0_TO_CD0*/ new_bus.BUS_CPU_D00p.tri6_pn(_TUBY_FF06_RDp, timer.SABU_TMA0p.qn_new());
  /*#p03.PYRE_TMA1_TO_CD1*/ new_bus.BUS_CPU_D01p.tri6_pn(_TUBY_FF06_RDp, timer.NYKE_TMA1p.qn_new());
  /*#p03.NOLA_TMA2_TO_CD2*/ new_bus.BUS_CPU_D02p.tri6_pn(_TUBY_FF06_RDp, timer.MURU_TMA2p.qn_new());
  /*#p03.SALU_TMA3_TO_CD3*/ new_bus.BUS_CPU_D03p.tri6_pn(_TUBY_FF06_RDp, timer.TYVA_TMA3p.qn_new());
  /*#p03.SUPO_TMA4_TO_CD4*/ new_bus.BUS_CPU_D04p.tri6_pn(_TUBY_FF06_RDp, timer.TYRU_TMA4p.qn_new());
  /*#p03.SOTU_TMA5_TO_CD5*/ new_bus.BUS_CPU_D05p.tri6_pn(_TUBY_FF06_RDp, timer.SUFY_TMA5p.qn_new());
  /*#p03.REVA_TMA6_TO_CD6*/ new_bus.BUS_CPU_D06p.tri6_pn(_TUBY_FF06_RDp, timer.PETO_TMA6p.qn_new());
  /*#p03.SAPU_TMA7_TO_CD7*/ new_bus.BUS_CPU_D07p.tri6_pn(_TUBY_FF06_RDp, timer.SETA_TMA7p.qn_new());

  /* p03.SORA*/ wire _SORA_FF07_RDp =  and4(cpu_bus.TEDO_CPU_RDp.qp_new(), new_bus.RYFO_FF04_FF07p(), new_bus.BUS_CPU_A01p.qp_new(), new_bus.BUS_CPU_A00p.qp_new());
  /*#p03.RYLA_TAC0_TO_CD0*/ new_bus.BUS_CPU_D00p.tri6_pn(_SORA_FF07_RDp, timer.SOPU_TAC0p.qn_new());
  /*#p03.ROTE_TAC1_TO_CD1*/ new_bus.BUS_CPU_D01p.tri6_pn(_SORA_FF07_RDp, timer.SAMY_TAC1p.qn_new());
  /*#p03.SUPE_TAC2_TO_CD2*/ new_bus.BUS_CPU_D02p.tri6_pn(_SORA_FF07_RDp, timer.SABO_TAC2p.qn_new());
}

//------------------------------------------------------------------------------------------------------------------------
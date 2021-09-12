#include "GateBoyLib/GateBoyTimer.h"

#include "GateBoyLib/GateBoy.h"

//------------------------------------------------------------------------------------------------------------------------
// this is hacked up because we're ignoring the debug reg for the moment
///*_p01.ULUR*/ wire ULUR_DIV_06_clknew = /*mux2p(FF60_1, BOGA_Axxxxxxx,*/ TAMA_DIV05p.qn_new() /*)*/;
///*_p01.UGOT*/ UGOT_DIV06p.dff17_clk(ULUR_DIV_06_clknew,  UGOT_DIV06p.qn_old());

void GateBoy::tock_div_gates() {
  /*_p01.TAPE*/ wire TAPE_FF04_WRp = and4(gbr.cpu_signals.TAPU_CPU_WRp.out_new(), gbr.cpu_abus_new.RYFO_FF04_FF07p(), gbr.cpu_abus_new.TOLA_A01n(), gbr.cpu_abus_new.TOVY_A00n());
  /*_p01.UFOL*/ wire UFOL_DIV_RSTn = nor3(UCOB_CLKBADp(), gbr.rst.PIN_71_RST.qp_int_new(), TAPE_FF04_WRp);

  /*_p01.UKUP*/ gbr.div.UKUP_DIV00p.dff17(BOGA_Axxxxxxx(),          UFOL_DIV_RSTn, gbr.div.UKUP_DIV00p.qn_old());
  /*_p01.UFOR*/ gbr.div.UFOR_DIV01p.dff17(gbr.div.UKUP_DIV00p.qn_any(), UFOL_DIV_RSTn, gbr.div.UFOR_DIV01p.qn_old());
  /*_p01.UNER*/ gbr.div.UNER_DIV02p.dff17(gbr.div.UFOR_DIV01p.qn_any(), UFOL_DIV_RSTn, gbr.div.UNER_DIV02p.qn_old());
  /*#p01.TERO*/ gbr.div.TERO_DIV03p.dff17(gbr.div.UNER_DIV02p.qn_any(), UFOL_DIV_RSTn, gbr.div.TERO_DIV03p.qn_old());
  /*_p01.UNYK*/ gbr.div.UNYK_DIV04p.dff17(gbr.div.TERO_DIV03p.qn_any(), UFOL_DIV_RSTn, gbr.div.UNYK_DIV04p.qn_old());
  /*_p01.TAMA*/ gbr.div.TAMA_DIV05p.dff17(gbr.div.UNYK_DIV04p.qn_any(), UFOL_DIV_RSTn, gbr.div.TAMA_DIV05p.qn_old());
  /*_p01.UGOT*/ gbr.div.UGOT_DIV06p.dff17(gbr.div.TAMA_DIV05p.qn_any(), UFOL_DIV_RSTn, gbr.div.UGOT_DIV06p.qn_old());
  /*_p01.TULU*/ gbr.div.TULU_DIV07p.dff17(gbr.div.UGOT_DIV06p.qn_any(), UFOL_DIV_RSTn, gbr.div.TULU_DIV07p.qn_old());
  /*_p01.TUGO*/ gbr.div.TUGO_DIV08p.dff17(gbr.div.TULU_DIV07p.qn_any(), UFOL_DIV_RSTn, gbr.div.TUGO_DIV08p.qn_old());
  /*_p01.TOFE*/ gbr.div.TOFE_DIV09p.dff17(gbr.div.TUGO_DIV08p.qn_any(), UFOL_DIV_RSTn, gbr.div.TOFE_DIV09p.qn_old());
  /*_p01.TERU*/ gbr.div.TERU_DIV10p.dff17(gbr.div.TOFE_DIV09p.qn_any(), UFOL_DIV_RSTn, gbr.div.TERU_DIV10p.qn_old());
  /*_p01.SOLA*/ gbr.div.SOLA_DIV11p.dff17(gbr.div.TERU_DIV10p.qn_any(), UFOL_DIV_RSTn, gbr.div.SOLA_DIV11p.qn_old());
  /*_p01.SUBU*/ gbr.div.SUBU_DIV12p.dff17(gbr.div.SOLA_DIV11p.qn_any(), UFOL_DIV_RSTn, gbr.div.SUBU_DIV12p.qn_old());
  /*_p01.TEKA*/ gbr.div.TEKA_DIV13p.dff17(gbr.div.SUBU_DIV12p.qn_any(), UFOL_DIV_RSTn, gbr.div.TEKA_DIV13p.qn_old());
  /*_p01.UKET*/ gbr.div.UKET_DIV14p.dff17(gbr.div.TEKA_DIV13p.qn_any(), UFOL_DIV_RSTn, gbr.div.UKET_DIV14p.qn_old());
  /*_p01.UPOF*/ gbr.div.UPOF_DIV15p.dff17(gbr.div.UKET_DIV14p.qn_any(), UFOL_DIV_RSTn, gbr.div.UPOF_DIV15p.qn_old());

  /*_p01.UMEK*/ wire UMEK_DIV06n = not1(gbr.div.UGOT_DIV06p.qp_new());
  /*_p01.UREK*/ wire UREK_DIV07n = not1(gbr.div.TULU_DIV07p.qp_new());
  /*_p01.UTOK*/ wire UTOK_DIV08n = not1(gbr.div.TUGO_DIV08p.qp_new());
  /*_p01.SAPY*/ wire SAPY_DIV09n = not1(gbr.div.TOFE_DIV09p.qp_new());
  /*_p01.UMER*/ wire UMER_DIV10n = not1(gbr.div.TERU_DIV10p.qp_new());
  /*_p01.RAVE*/ wire RAVE_DIV11n = not1(gbr.div.SOLA_DIV11p.qp_new());
  /*_p01.RYSO*/ wire RYSO_DIV12n = not1(gbr.div.SUBU_DIV12p.qp_new());
  /*_p01.UDOR*/ wire UDOR_DIV13n = not1(gbr.div.TEKA_DIV13p.qp_new());

  /*_p01.TAGY*/ wire TAGY_FF04_RDp_ext = and4(gbr.cpu_signals.TEDO_CPU_RDp.out_new(), gbr.cpu_abus_new.RYFO_FF04_FF07p(), gbr.cpu_abus_new.TOLA_A01n(), gbr.cpu_abus_new.TOVY_A00n());
  /*_p01.TAWU*/ triwire TAWU_DIV06_TO_CD0 = tri6_pn(TAGY_FF04_RDp_ext, UMEK_DIV06n);
  /*_p01.TAKU*/ triwire TAKU_DIV07_TO_CD1 = tri6_pn(TAGY_FF04_RDp_ext, UREK_DIV07n);
  /*_p01.TEMU*/ triwire TEMU_DIV08_TO_CD2 = tri6_pn(TAGY_FF04_RDp_ext, UTOK_DIV08n);
  /*_p01.TUSE*/ triwire TUSE_DIV09_TO_CD3 = tri6_pn(TAGY_FF04_RDp_ext, SAPY_DIV09n);
  /*_p01.UPUG*/ triwire UPUG_DIV10_TO_CD4 = tri6_pn(TAGY_FF04_RDp_ext, UMER_DIV10n); // Schematic wrong, UPUG/SEPU driving D5/D4
  /*_p01.SEPU*/ triwire SEPU_DIV11_TO_CD5 = tri6_pn(TAGY_FF04_RDp_ext, RAVE_DIV11n);
  /*_p01.SAWA*/ triwire SAWA_DIV12_TO_CD6 = tri6_pn(TAGY_FF04_RDp_ext, RYSO_DIV12n);
  /*_p01.TATU*/ triwire TATU_DIV13_TO_CD7 = tri6_pn(TAGY_FF04_RDp_ext, UDOR_DIV13n);

  /*_BUS_CPU_D00p*/ gbr.cpu_dbus_new.BUS_CPU_D00p.tri_bus(TAWU_DIV06_TO_CD0);
  /*_BUS_CPU_D01p*/ gbr.cpu_dbus_new.BUS_CPU_D01p.tri_bus(TAKU_DIV07_TO_CD1);
  /*_BUS_CPU_D02p*/ gbr.cpu_dbus_new.BUS_CPU_D02p.tri_bus(TEMU_DIV08_TO_CD2);
  /*_BUS_CPU_D03p*/ gbr.cpu_dbus_new.BUS_CPU_D03p.tri_bus(TUSE_DIV09_TO_CD3);
  /*_BUS_CPU_D04p*/ gbr.cpu_dbus_new.BUS_CPU_D04p.tri_bus(UPUG_DIV10_TO_CD4);
  /*_BUS_CPU_D05p*/ gbr.cpu_dbus_new.BUS_CPU_D05p.tri_bus(SEPU_DIV11_TO_CD5);
  /*_BUS_CPU_D06p*/ gbr.cpu_dbus_new.BUS_CPU_D06p.tri_bus(SAWA_DIV12_TO_CD6);
  /*_BUS_CPU_D07p*/ gbr.cpu_dbus_new.BUS_CPU_D07p.tri_bus(TATU_DIV13_TO_CD7);
}

//------------------------------------------------------------------------------------------------------------------------

void GateBoy::tock_timer_gates() {
  /*_p03.TYJU*/ wire TYJU_FF06_WRn = nand4(gbr.cpu_signals.TAPU_CPU_WRp.out_new(), gbr.cpu_abus_new.RYFO_FF04_FF07p(), gbr.cpu_abus_new.BUS_CPU_A01p.out_new(), gbr.cpu_abus_new.TOVY_A00n());
  /*_p03.SABU*/ gbr.tma.SABU_TMA0p.dff17(TYJU_FF06_WRn, ALUR_SYS_RSTn(), gbr.cpu_dbus_old.BUS_CPU_D00p.out_old());
  /*_p03.NYKE*/ gbr.tma.NYKE_TMA1p.dff17(TYJU_FF06_WRn, ALUR_SYS_RSTn(), gbr.cpu_dbus_old.BUS_CPU_D01p.out_old());
  /*_p03.MURU*/ gbr.tma.MURU_TMA2p.dff17(TYJU_FF06_WRn, ALUR_SYS_RSTn(), gbr.cpu_dbus_old.BUS_CPU_D02p.out_old());
  /*_p03.TYVA*/ gbr.tma.TYVA_TMA3p.dff17(TYJU_FF06_WRn, ALUR_SYS_RSTn(), gbr.cpu_dbus_old.BUS_CPU_D03p.out_old());
  /*_p03.TYRU*/ gbr.tma.TYRU_TMA4p.dff17(TYJU_FF06_WRn, ALUR_SYS_RSTn(), gbr.cpu_dbus_old.BUS_CPU_D04p.out_old());
  /*_p03.SUFY*/ gbr.tma.SUFY_TMA5p.dff17(TYJU_FF06_WRn, ALUR_SYS_RSTn(), gbr.cpu_dbus_old.BUS_CPU_D05p.out_old());
  /*_p03.PETO*/ gbr.tma.PETO_TMA6p.dff17(TYJU_FF06_WRn, ALUR_SYS_RSTn(), gbr.cpu_dbus_old.BUS_CPU_D06p.out_old());
  /*_p03.SETA*/ gbr.tma.SETA_TMA7p.dff17(TYJU_FF06_WRn, ALUR_SYS_RSTn(), gbr.cpu_dbus_old.BUS_CPU_D07p.out_old());

  /*_p03.SARA*/ wire SARA_FF07_WRn = nand4(gbr.cpu_signals.TAPU_CPU_WRp.out_new(), gbr.cpu_abus_new.RYFO_FF04_FF07p(), gbr.cpu_abus_new.BUS_CPU_A01p.out_new(), gbr.cpu_abus_new.BUS_CPU_A00p.out_new());
  /*_p03.SOPU*/ gbr.tac.SOPU_TAC0p.dff17(SARA_FF07_WRn, ALUR_SYS_RSTn(), gbr.cpu_dbus_old.BUS_CPU_D00p.out_old());
  /*_p03.SAMY*/ gbr.tac.SAMY_TAC1p.dff17(SARA_FF07_WRn, ALUR_SYS_RSTn(), gbr.cpu_dbus_old.BUS_CPU_D01p.out_old());
  /*_p03.SABO*/ gbr.tac.SABO_TAC2p.dff17(SARA_FF07_WRn, ALUR_SYS_RSTn(), gbr.cpu_dbus_old.BUS_CPU_D02p.out_old());

  /*#p03.MERY*/ wire MERY_TIMER_OVERFLOWp_old = nor2(gbr.tima.NUGA_TIMA7p.qp_old(), gbr.int_ctrl.NYDU_TIMA7p_DELAY.qn_old());
  /*#p03.MOBA*/ gbr.int_ctrl.MOBA_TIMER_OVERFLOWp.dff17(BOGA_Axxxxxxx(), ALUR_SYS_RSTn(), MERY_TIMER_OVERFLOWp_old);

  /*#p03.TOPE*/ wire TOPE_FF05_WRn = nand4(gbr.cpu_signals.TAPU_CPU_WRp.out_new(), gbr.cpu_abus_new.RYFO_FF04_FF07p(),  gbr.cpu_abus_new.TOLA_A01n(), gbr.cpu_abus_new.BUS_CPU_A00p.out_new());
  /*#p03.MUZU*/ wire MUZU_CPU_LOAD_TIMAn  = or2(gbr.cpu_signals.SIG_IN_CPU_LATCH_EXT.out_new(), TOPE_FF05_WRn);
  /*#p03.MEKE*/ wire MEKE_TIMER_OVERFLOWn = not1(gbr.int_ctrl.MOBA_TIMER_OVERFLOWp.qp_new());
  /*#p03.MEXU*/ wire MEXU_TIMA_LOADp      = nand3(MUZU_CPU_LOAD_TIMAn, ALUR_SYS_RSTn(), MEKE_TIMER_OVERFLOWn);
  /*#p03.MUGY*/ wire MUGY_TIMA_MAX_RSTn   = not1(MEXU_TIMA_LOADp);
  /*#p03.NYDU*/ gbr.int_ctrl.NYDU_TIMA7p_DELAY.dff17(BOGA_Axxxxxxx(), MUGY_TIMA_MAX_RSTn, gbr.tima.NUGA_TIMA7p.qp_old());

  /*#p03.UBOT*/ wire UBOT_DIV01n = not1(gbr.div.UFOR_DIV01p.qp_new());
  /*#p03.UVYR*/ wire UVYR_DIV03n = not1(gbr.div.TERO_DIV03p.qp_new());
  /*#p01.UVYN*/ wire UVYN_DIV05n = not1(gbr.div.TAMA_DIV05p.qp_new());
  /*_p01.UREK*/ wire UREK_DIV07n = not1(gbr.div.TULU_DIV07p.qp_new());

  /*#p03.UKAP*/ wire UKAP_CLK_MUXa  = mux2n(gbr.tac.SOPU_TAC0p.qp_new(), UVYN_DIV05n, UVYR_DIV03n);
  /*#p03.TEKO*/ wire TEKO_CLK_MUXb  = mux2n(gbr.tac.SOPU_TAC0p.qp_new(), UBOT_DIV01n, UREK_DIV07n);
  /*#p03.TECY*/ wire TECY_CLK_MUXc  = mux2n(gbr.tac.SAMY_TAC1p.qp_new(), UKAP_CLK_MUXa, TEKO_CLK_MUXb);
  /*#p03.SOGU*/ wire SOGU_TIMA_CLKn = nor2(TECY_CLK_MUXc, gbr.tac.SABO_TAC2p.qn_new());

  ///*#p03.TOPE*/ wire TOPE_FF05_WRn = nand4(cpu_signals.TAPU_CPU_WRp.qp_new(), cpu_signals.RYFO_FF04_FF07p(),  cpu_signals.TOLA_A01n(), cpu_signals.BUS_CPU_A00p.qp_new());

  /*#p03.ROKE*/ wire ROKE_TIMA_D0 = mux2n(TOPE_FF05_WRn, gbr.tma.SABU_TMA0p.qp_new(), gbr.cpu_dbus_new.BUS_CPU_D00p.out_new());
  /*#p03.PETU*/ wire PETU_TIMA_D1 = mux2n(TOPE_FF05_WRn, gbr.tma.NYKE_TMA1p.qp_new(), gbr.cpu_dbus_new.BUS_CPU_D01p.out_new());
  /*#p03.NYKU*/ wire NYKU_TIMA_D2 = mux2n(TOPE_FF05_WRn, gbr.tma.MURU_TMA2p.qp_new(), gbr.cpu_dbus_new.BUS_CPU_D02p.out_new());
  /*#p03.SOCE*/ wire SOCE_TIMA_D3 = mux2n(TOPE_FF05_WRn, gbr.tma.TYVA_TMA3p.qp_new(), gbr.cpu_dbus_new.BUS_CPU_D03p.out_new());
  /*#p03.SALA*/ wire SALA_TIMA_D4 = mux2n(TOPE_FF05_WRn, gbr.tma.TYRU_TMA4p.qp_new(), gbr.cpu_dbus_new.BUS_CPU_D04p.out_new());
  /*#p03.SYRU*/ wire SYRU_TIMA_D5 = mux2n(TOPE_FF05_WRn, gbr.tma.SUFY_TMA5p.qp_new(), gbr.cpu_dbus_new.BUS_CPU_D05p.out_new());
  /*#p03.REFU*/ wire REFU_TIMA_D6 = mux2n(TOPE_FF05_WRn, gbr.tma.PETO_TMA6p.qp_new(), gbr.cpu_dbus_new.BUS_CPU_D06p.out_new());
  /*#p03.RATO*/ wire RATO_TIMA_D7 = mux2n(TOPE_FF05_WRn, gbr.tma.SETA_TMA7p.qp_new(), gbr.cpu_dbus_new.BUS_CPU_D07p.out_new());

  /*#p03.PUXY*/ wire PUXY_TIMA_D0 = nor2(MULO_SYS_RSTn(), ROKE_TIMA_D0);
  /*#p03.NERO*/ wire NERO_TIMA_D1 = nor2(MULO_SYS_RSTn(), PETU_TIMA_D1);
  /*#p03.NADA*/ wire NADA_TIMA_D2 = nor2(MULO_SYS_RSTn(), NYKU_TIMA_D2);
  /*#p03.REPA*/ wire REPA_TIMA_D3 = nor2(MULO_SYS_RSTn(), SOCE_TIMA_D3);
  /*#p03.ROLU*/ wire ROLU_TIMA_D4 = nor2(MULO_SYS_RSTn(), SALA_TIMA_D4);
  /*#p03.RUGY*/ wire RUGY_TIMA_D5 = nor2(MULO_SYS_RSTn(), SYRU_TIMA_D5);
  /*#p03.PYMA*/ wire PYMA_TIMA_D6 = nor2(MULO_SYS_RSTn(), REFU_TIMA_D6);
  /*#p03.PAGU*/ wire PAGU_TIMA_D7 = nor2(MULO_SYS_RSTn(), RATO_TIMA_D7);

  // FIXME puxy etc should be old

  /*#p03.REGA*/ gbr.tima.REGA_TIMA0p.dff20(SOGU_TIMA_CLKn,                MEXU_TIMA_LOADp, PUXY_TIMA_D0);
  /*#p03.POVY*/ gbr.tima.POVY_TIMA1p.dff20(gbr.tima.REGA_TIMA0p.qp_any(), MEXU_TIMA_LOADp, NERO_TIMA_D1);
  /*#p03.PERU*/ gbr.tima.PERU_TIMA2p.dff20(gbr.tima.POVY_TIMA1p.qp_any(), MEXU_TIMA_LOADp, NADA_TIMA_D2);
  /*#p03.RATE*/ gbr.tima.RATE_TIMA3p.dff20(gbr.tima.PERU_TIMA2p.qp_any(), MEXU_TIMA_LOADp, REPA_TIMA_D3);
  /*#p03.RUBY*/ gbr.tima.RUBY_TIMA4p.dff20(gbr.tima.RATE_TIMA3p.qp_any(), MEXU_TIMA_LOADp, ROLU_TIMA_D4);
  /*#p03.RAGE*/ gbr.tima.RAGE_TIMA5p.dff20(gbr.tima.RUBY_TIMA4p.qp_any(), MEXU_TIMA_LOADp, RUGY_TIMA_D5);
  /*#p03.PEDA*/ gbr.tima.PEDA_TIMA6p.dff20(gbr.tima.RAGE_TIMA5p.qp_any(), MEXU_TIMA_LOADp, PYMA_TIMA_D6);
  /*#p03.NUGA*/ gbr.tima.NUGA_TIMA7p.dff20(gbr.tima.PEDA_TIMA6p.qp_any(), MEXU_TIMA_LOADp, PAGU_TIMA_D7);

  /*#p03.TEDA*/ wire TEDA_FF05_RDp =  and4(gbr.cpu_signals.TEDO_CPU_RDp.out_new(), gbr.cpu_abus_new.RYFO_FF04_FF07p(),  gbr.cpu_abus_new.TOLA_A01n(), gbr.cpu_abus_new.BUS_CPU_A00p.out_new());
  /*#p03.SOKU*/ triwire SOKU_TIMA0_TO_CD0 = tri6_pn(TEDA_FF05_RDp, gbr.tima.REGA_TIMA0p.qn_new());
  /*#p03.RACY*/ triwire RACY_TIMA1_TO_CD1 = tri6_pn(TEDA_FF05_RDp, gbr.tima.POVY_TIMA1p.qn_new());
  /*#p03.RAVY*/ triwire RAVY_TIMA2_TO_CD2 = tri6_pn(TEDA_FF05_RDp, gbr.tima.PERU_TIMA2p.qn_new());
  /*#p03.SOSY*/ triwire SOSY_TIMA3_TO_CD3 = tri6_pn(TEDA_FF05_RDp, gbr.tima.RATE_TIMA3p.qn_new());
  /*#p03.SOMU*/ triwire SOMU_TIMA4_TO_CD4 = tri6_pn(TEDA_FF05_RDp, gbr.tima.RUBY_TIMA4p.qn_new());
  /*#p03.SURO*/ triwire SURO_TIMA5_TO_CD5 = tri6_pn(TEDA_FF05_RDp, gbr.tima.RAGE_TIMA5p.qn_new());
  /*#p03.ROWU*/ triwire ROWU_TIMA6_TO_CD6 = tri6_pn(TEDA_FF05_RDp, gbr.tima.PEDA_TIMA6p.qn_new());
  /*#p03.PUSO*/ triwire PUSO_TIMA7_TO_CD7 = tri6_pn(TEDA_FF05_RDp, gbr.tima.NUGA_TIMA7p.qn_new());

  /*_BUS_CPU_D00p*/ gbr.cpu_dbus_new.BUS_CPU_D00p.tri_bus(SOKU_TIMA0_TO_CD0);
  /*_BUS_CPU_D01p*/ gbr.cpu_dbus_new.BUS_CPU_D01p.tri_bus(RACY_TIMA1_TO_CD1);
  /*_BUS_CPU_D02p*/ gbr.cpu_dbus_new.BUS_CPU_D02p.tri_bus(RAVY_TIMA2_TO_CD2);
  /*_BUS_CPU_D03p*/ gbr.cpu_dbus_new.BUS_CPU_D03p.tri_bus(SOSY_TIMA3_TO_CD3);
  /*_BUS_CPU_D04p*/ gbr.cpu_dbus_new.BUS_CPU_D04p.tri_bus(SOMU_TIMA4_TO_CD4);
  /*_BUS_CPU_D05p*/ gbr.cpu_dbus_new.BUS_CPU_D05p.tri_bus(SURO_TIMA5_TO_CD5);
  /*_BUS_CPU_D06p*/ gbr.cpu_dbus_new.BUS_CPU_D06p.tri_bus(ROWU_TIMA6_TO_CD6);
  /*_BUS_CPU_D07p*/ gbr.cpu_dbus_new.BUS_CPU_D07p.tri_bus(PUSO_TIMA7_TO_CD7);

  /*_p03.TUBY*/ wire TUBY_FF06_RDp =  and4(gbr.cpu_signals.TEDO_CPU_RDp.out_new(), gbr.cpu_abus_new.RYFO_FF04_FF07p(), gbr.cpu_abus_new.BUS_CPU_A01p.out_new(), gbr.cpu_abus_new.TOVY_A00n());
  /*#p03.SETE*/ triwire SETE_TMA0_TO_CD0 = tri6_pn(TUBY_FF06_RDp, gbr.tma.SABU_TMA0p.qn_new());
  /*#p03.PYRE*/ triwire PYRE_TMA1_TO_CD1 = tri6_pn(TUBY_FF06_RDp, gbr.tma.NYKE_TMA1p.qn_new());
  /*#p03.NOLA*/ triwire NOLA_TMA2_TO_CD2 = tri6_pn(TUBY_FF06_RDp, gbr.tma.MURU_TMA2p.qn_new());
  /*#p03.SALU*/ triwire SALU_TMA3_TO_CD3 = tri6_pn(TUBY_FF06_RDp, gbr.tma.TYVA_TMA3p.qn_new());
  /*#p03.SUPO*/ triwire SUPO_TMA4_TO_CD4 = tri6_pn(TUBY_FF06_RDp, gbr.tma.TYRU_TMA4p.qn_new());
  /*#p03.SOTU*/ triwire SOTU_TMA5_TO_CD5 = tri6_pn(TUBY_FF06_RDp, gbr.tma.SUFY_TMA5p.qn_new());
  /*#p03.REVA*/ triwire REVA_TMA6_TO_CD6 = tri6_pn(TUBY_FF06_RDp, gbr.tma.PETO_TMA6p.qn_new());
  /*#p03.SAPU*/ triwire SAPU_TMA7_TO_CD7 = tri6_pn(TUBY_FF06_RDp, gbr.tma.SETA_TMA7p.qn_new());

  /*_BUS_CPU_D00p*/ gbr.cpu_dbus_new.BUS_CPU_D00p.tri_bus(SETE_TMA0_TO_CD0);
  /*_BUS_CPU_D01p*/ gbr.cpu_dbus_new.BUS_CPU_D01p.tri_bus(PYRE_TMA1_TO_CD1);
  /*_BUS_CPU_D02p*/ gbr.cpu_dbus_new.BUS_CPU_D02p.tri_bus(NOLA_TMA2_TO_CD2);
  /*_BUS_CPU_D03p*/ gbr.cpu_dbus_new.BUS_CPU_D03p.tri_bus(SALU_TMA3_TO_CD3);
  /*_BUS_CPU_D04p*/ gbr.cpu_dbus_new.BUS_CPU_D04p.tri_bus(SUPO_TMA4_TO_CD4);
  /*_BUS_CPU_D05p*/ gbr.cpu_dbus_new.BUS_CPU_D05p.tri_bus(SOTU_TMA5_TO_CD5);
  /*_BUS_CPU_D06p*/ gbr.cpu_dbus_new.BUS_CPU_D06p.tri_bus(REVA_TMA6_TO_CD6);
  /*_BUS_CPU_D07p*/ gbr.cpu_dbus_new.BUS_CPU_D07p.tri_bus(SAPU_TMA7_TO_CD7);

  /*_p03.SORA*/ wire SORA_FF07_RDp =  and4(gbr.cpu_signals.TEDO_CPU_RDp.out_new(), gbr.cpu_abus_new.RYFO_FF04_FF07p(), gbr.cpu_abus_new.BUS_CPU_A01p.out_new(), gbr.cpu_abus_new.BUS_CPU_A00p.out_new());
  /*#p03.RYLA*/ triwire RYLA_TAC0_TO_CD0 = tri6_pn(SORA_FF07_RDp, gbr.tac.SOPU_TAC0p.qn_new());
  /*#p03.ROTE*/ triwire ROTE_TAC1_TO_CD1 = tri6_pn(SORA_FF07_RDp, gbr.tac.SAMY_TAC1p.qn_new());
  /*#p03.SUPE*/ triwire SUPE_TAC2_TO_CD2 = tri6_pn(SORA_FF07_RDp, gbr.tac.SABO_TAC2p.qn_new());

  /*_BUS_CPU_D00p*/ gbr.cpu_dbus_new.BUS_CPU_D00p.tri_bus(RYLA_TAC0_TO_CD0);
  /*_BUS_CPU_D01p*/ gbr.cpu_dbus_new.BUS_CPU_D01p.tri_bus(ROTE_TAC1_TO_CD1);
  /*_BUS_CPU_D02p*/ gbr.cpu_dbus_new.BUS_CPU_D02p.tri_bus(SUPE_TAC2_TO_CD2);
}

//------------------------------------------------------------------------------------------------------------------------
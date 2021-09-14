#include "GateBoyLib/GateBoyTimer.h"

#include "GateBoyLib/GateBoy.h"

//-----------------------------------------------------------------------------
// this is hacked up because we're ignoring the debug reg for the moment
///*_p01.ULUR*/ wire ULUR_DIV_06_clknew = /*mux2p(FF60_1, BOGA_Axxxxxxx,*/ TAMA_DIV05p.qn_new() /*)*/;
///*_p01.UGOT*/ UGOT_DIV06p.dff17_clk(ULUR_DIV_06_clknew,  UGOT_DIV06p.qn_old());

void GateBoy::tock_div_gates() {
  /*_p01.TAPE*/ wire TAPE_FF04_WRp = and4(reg.cpu_signals.TAPU_CPU_WRp.out_new(), reg.cpu_abus.RYFO_FF04_FF07p(), reg.cpu_abus.TOLA_A01n(), reg.cpu_abus.TOVY_A00n());
  /*_p01.UFOL*/ wire UFOL_DIV_RSTn = nor3(reg.sys_clk.UCOB_CLKBADp(), reg.sys_rst.PIN_71_RST.qp_int_new(), TAPE_FF04_WRp);

  /*_p01.UKUP*/ reg.reg_div.UKUP_DIV00p.dff17(reg.sys_clk.BOGA_Axxxxxxx(),          UFOL_DIV_RSTn, reg.reg_div.UKUP_DIV00p.qn_old());
  /*_p01.UFOR*/ reg.reg_div.UFOR_DIV01p.dff17(reg.reg_div.UKUP_DIV00p.qn_any(), UFOL_DIV_RSTn, reg.reg_div.UFOR_DIV01p.qn_old());
  /*_p01.UNER*/ reg.reg_div.UNER_DIV02p.dff17(reg.reg_div.UFOR_DIV01p.qn_any(), UFOL_DIV_RSTn, reg.reg_div.UNER_DIV02p.qn_old());
  /*#p01.TERO*/ reg.reg_div.TERO_DIV03p.dff17(reg.reg_div.UNER_DIV02p.qn_any(), UFOL_DIV_RSTn, reg.reg_div.TERO_DIV03p.qn_old());
  /*_p01.UNYK*/ reg.reg_div.UNYK_DIV04p.dff17(reg.reg_div.TERO_DIV03p.qn_any(), UFOL_DIV_RSTn, reg.reg_div.UNYK_DIV04p.qn_old());
  /*_p01.TAMA*/ reg.reg_div.TAMA_DIV05p.dff17(reg.reg_div.UNYK_DIV04p.qn_any(), UFOL_DIV_RSTn, reg.reg_div.TAMA_DIV05p.qn_old());
  /*_p01.UGOT*/ reg.reg_div.UGOT_DIV06p.dff17(reg.reg_div.TAMA_DIV05p.qn_any(), UFOL_DIV_RSTn, reg.reg_div.UGOT_DIV06p.qn_old());
  /*_p01.TULU*/ reg.reg_div.TULU_DIV07p.dff17(reg.reg_div.UGOT_DIV06p.qn_any(), UFOL_DIV_RSTn, reg.reg_div.TULU_DIV07p.qn_old());
  /*_p01.TUGO*/ reg.reg_div.TUGO_DIV08p.dff17(reg.reg_div.TULU_DIV07p.qn_any(), UFOL_DIV_RSTn, reg.reg_div.TUGO_DIV08p.qn_old());
  /*_p01.TOFE*/ reg.reg_div.TOFE_DIV09p.dff17(reg.reg_div.TUGO_DIV08p.qn_any(), UFOL_DIV_RSTn, reg.reg_div.TOFE_DIV09p.qn_old());
  /*_p01.TERU*/ reg.reg_div.TERU_DIV10p.dff17(reg.reg_div.TOFE_DIV09p.qn_any(), UFOL_DIV_RSTn, reg.reg_div.TERU_DIV10p.qn_old());
  /*_p01.SOLA*/ reg.reg_div.SOLA_DIV11p.dff17(reg.reg_div.TERU_DIV10p.qn_any(), UFOL_DIV_RSTn, reg.reg_div.SOLA_DIV11p.qn_old());
  /*_p01.SUBU*/ reg.reg_div.SUBU_DIV12p.dff17(reg.reg_div.SOLA_DIV11p.qn_any(), UFOL_DIV_RSTn, reg.reg_div.SUBU_DIV12p.qn_old());
  /*_p01.TEKA*/ reg.reg_div.TEKA_DIV13p.dff17(reg.reg_div.SUBU_DIV12p.qn_any(), UFOL_DIV_RSTn, reg.reg_div.TEKA_DIV13p.qn_old());
  /*_p01.UKET*/ reg.reg_div.UKET_DIV14p.dff17(reg.reg_div.TEKA_DIV13p.qn_any(), UFOL_DIV_RSTn, reg.reg_div.UKET_DIV14p.qn_old());
  /*_p01.UPOF*/ reg.reg_div.UPOF_DIV15p.dff17(reg.reg_div.UKET_DIV14p.qn_any(), UFOL_DIV_RSTn, reg.reg_div.UPOF_DIV15p.qn_old());

  /*_p01.UMEK*/ wire UMEK_DIV06n = not1(reg.reg_div.UGOT_DIV06p.qp_new());
  /*_p01.UREK*/ wire UREK_DIV07n = not1(reg.reg_div.TULU_DIV07p.qp_new());
  /*_p01.UTOK*/ wire UTOK_DIV08n = not1(reg.reg_div.TUGO_DIV08p.qp_new());
  /*_p01.SAPY*/ wire SAPY_DIV09n = not1(reg.reg_div.TOFE_DIV09p.qp_new());
  /*_p01.UMER*/ wire UMER_DIV10n = not1(reg.reg_div.TERU_DIV10p.qp_new());
  /*_p01.RAVE*/ wire RAVE_DIV11n = not1(reg.reg_div.SOLA_DIV11p.qp_new());
  /*_p01.RYSO*/ wire RYSO_DIV12n = not1(reg.reg_div.SUBU_DIV12p.qp_new());
  /*_p01.UDOR*/ wire UDOR_DIV13n = not1(reg.reg_div.TEKA_DIV13p.qp_new());

  /*_p01.TAGY*/ wire TAGY_FF04_RDp_ext = and4(reg.cpu_signals.TEDO_CPU_RDp.out_new(), reg.cpu_abus.RYFO_FF04_FF07p(), reg.cpu_abus.TOLA_A01n(), reg.cpu_abus.TOVY_A00n());
  /*_p01.TAWU*/ triwire TAWU_DIV06_TO_CD0 = tri6_pn(TAGY_FF04_RDp_ext, UMEK_DIV06n);
  /*_p01.TAKU*/ triwire TAKU_DIV07_TO_CD1 = tri6_pn(TAGY_FF04_RDp_ext, UREK_DIV07n);
  /*_p01.TEMU*/ triwire TEMU_DIV08_TO_CD2 = tri6_pn(TAGY_FF04_RDp_ext, UTOK_DIV08n);
  /*_p01.TUSE*/ triwire TUSE_DIV09_TO_CD3 = tri6_pn(TAGY_FF04_RDp_ext, SAPY_DIV09n);
  /*_p01.UPUG*/ triwire UPUG_DIV10_TO_CD4 = tri6_pn(TAGY_FF04_RDp_ext, UMER_DIV10n); // Schematic wrong, UPUG/SEPU driving D5/D4
  /*_p01.SEPU*/ triwire SEPU_DIV11_TO_CD5 = tri6_pn(TAGY_FF04_RDp_ext, RAVE_DIV11n);
  /*_p01.SAWA*/ triwire SAWA_DIV12_TO_CD6 = tri6_pn(TAGY_FF04_RDp_ext, RYSO_DIV12n);
  /*_p01.TATU*/ triwire TATU_DIV13_TO_CD7 = tri6_pn(TAGY_FF04_RDp_ext, UDOR_DIV13n);

  /*_BUS_CPU_D00p*/ reg.cpu_dbus.BUS_CPU_D00p.tri_bus(TAWU_DIV06_TO_CD0);
  /*_BUS_CPU_D01p*/ reg.cpu_dbus.BUS_CPU_D01p.tri_bus(TAKU_DIV07_TO_CD1);
  /*_BUS_CPU_D02p*/ reg.cpu_dbus.BUS_CPU_D02p.tri_bus(TEMU_DIV08_TO_CD2);
  /*_BUS_CPU_D03p*/ reg.cpu_dbus.BUS_CPU_D03p.tri_bus(TUSE_DIV09_TO_CD3);
  /*_BUS_CPU_D04p*/ reg.cpu_dbus.BUS_CPU_D04p.tri_bus(UPUG_DIV10_TO_CD4);
  /*_BUS_CPU_D05p*/ reg.cpu_dbus.BUS_CPU_D05p.tri_bus(SEPU_DIV11_TO_CD5);
  /*_BUS_CPU_D06p*/ reg.cpu_dbus.BUS_CPU_D06p.tri_bus(SAWA_DIV12_TO_CD6);
  /*_BUS_CPU_D07p*/ reg.cpu_dbus.BUS_CPU_D07p.tri_bus(TATU_DIV13_TO_CD7);
}

//-----------------------------------------------------------------------------

void GateBoy::tock_timer_gates(const GateBoyReg& reg_old) {
  /*_p03.TYJU*/ wire TYJU_FF06_WRn = nand4(reg.cpu_signals.TAPU_CPU_WRp.out_new(), reg.cpu_abus.RYFO_FF04_FF07p(), reg.cpu_abus.BUS_CPU_A01p.out_new(), reg.cpu_abus.TOVY_A00n());
  /*_p03.SABU*/ reg.reg_tma.SABU_TMA0p.dff17(TYJU_FF06_WRn, reg.sys_rst.ALUR_SYS_RSTn(), reg_old.cpu_dbus.BUS_CPU_D00p.out_old());
  /*_p03.NYKE*/ reg.reg_tma.NYKE_TMA1p.dff17(TYJU_FF06_WRn, reg.sys_rst.ALUR_SYS_RSTn(), reg_old.cpu_dbus.BUS_CPU_D01p.out_old());
  /*_p03.MURU*/ reg.reg_tma.MURU_TMA2p.dff17(TYJU_FF06_WRn, reg.sys_rst.ALUR_SYS_RSTn(), reg_old.cpu_dbus.BUS_CPU_D02p.out_old());
  /*_p03.TYVA*/ reg.reg_tma.TYVA_TMA3p.dff17(TYJU_FF06_WRn, reg.sys_rst.ALUR_SYS_RSTn(), reg_old.cpu_dbus.BUS_CPU_D03p.out_old());
  /*_p03.TYRU*/ reg.reg_tma.TYRU_TMA4p.dff17(TYJU_FF06_WRn, reg.sys_rst.ALUR_SYS_RSTn(), reg_old.cpu_dbus.BUS_CPU_D04p.out_old());
  /*_p03.SUFY*/ reg.reg_tma.SUFY_TMA5p.dff17(TYJU_FF06_WRn, reg.sys_rst.ALUR_SYS_RSTn(), reg_old.cpu_dbus.BUS_CPU_D05p.out_old());
  /*_p03.PETO*/ reg.reg_tma.PETO_TMA6p.dff17(TYJU_FF06_WRn, reg.sys_rst.ALUR_SYS_RSTn(), reg_old.cpu_dbus.BUS_CPU_D06p.out_old());
  /*_p03.SETA*/ reg.reg_tma.SETA_TMA7p.dff17(TYJU_FF06_WRn, reg.sys_rst.ALUR_SYS_RSTn(), reg_old.cpu_dbus.BUS_CPU_D07p.out_old());

  /*_p03.SARA*/ wire SARA_FF07_WRn = nand4(reg.cpu_signals.TAPU_CPU_WRp.out_new(), reg.cpu_abus.RYFO_FF04_FF07p(), reg.cpu_abus.BUS_CPU_A01p.out_new(), reg.cpu_abus.BUS_CPU_A00p.out_new());
  /*_p03.SOPU*/ reg.reg_tac.SOPU_TAC0p.dff17(SARA_FF07_WRn, reg.sys_rst.ALUR_SYS_RSTn(), reg_old.cpu_dbus.BUS_CPU_D00p.out_old());
  /*_p03.SAMY*/ reg.reg_tac.SAMY_TAC1p.dff17(SARA_FF07_WRn, reg.sys_rst.ALUR_SYS_RSTn(), reg_old.cpu_dbus.BUS_CPU_D01p.out_old());
  /*_p03.SABO*/ reg.reg_tac.SABO_TAC2p.dff17(SARA_FF07_WRn, reg.sys_rst.ALUR_SYS_RSTn(), reg_old.cpu_dbus.BUS_CPU_D02p.out_old());

  /*#p03.MERY*/ wire MERY_TIMER_OVERFLOWp_old = nor2(reg.reg_tima.NUGA_TIMA7p.qp_old(), reg.int_ctrl.NYDU_TIMA7p_DELAY.qn_old());
  /*#p03.MOBA*/ reg.int_ctrl.MOBA_TIMER_OVERFLOWp.dff17(reg.sys_clk.BOGA_Axxxxxxx(), reg.sys_rst.ALUR_SYS_RSTn(), MERY_TIMER_OVERFLOWp_old);

  /*#p03.TOPE*/ wire TOPE_FF05_WRn = nand4(reg.cpu_signals.TAPU_CPU_WRp.out_new(), reg.cpu_abus.RYFO_FF04_FF07p(),  reg.cpu_abus.TOLA_A01n(), reg.cpu_abus.BUS_CPU_A00p.out_new());
  /*#p03.MUZU*/ wire MUZU_CPU_LOAD_TIMAn  = or2(reg.cpu_signals.SIG_IN_CPU_DBUS_FREE.out_new(), TOPE_FF05_WRn);
  /*#p03.MEKE*/ wire MEKE_TIMER_OVERFLOWn = not1(reg.int_ctrl.MOBA_TIMER_OVERFLOWp.qp_new());
  /*#p03.MEXU*/ wire MEXU_TIMA_LOADp      = nand3(MUZU_CPU_LOAD_TIMAn, reg.sys_rst.ALUR_SYS_RSTn(), MEKE_TIMER_OVERFLOWn);
  /*#p03.MUGY*/ wire MUGY_TIMA_MAX_RSTn   = not1(MEXU_TIMA_LOADp);
  /*#p03.NYDU*/ reg.int_ctrl.NYDU_TIMA7p_DELAY.dff17(reg.sys_clk.BOGA_Axxxxxxx(), MUGY_TIMA_MAX_RSTn, reg.reg_tima.NUGA_TIMA7p.qp_old());

  /*#p03.UBOT*/ wire UBOT_DIV01n = not1(reg.reg_div.UFOR_DIV01p.qp_new());
  /*#p03.UVYR*/ wire UVYR_DIV03n = not1(reg.reg_div.TERO_DIV03p.qp_new());
  /*#p01.UVYN*/ wire UVYN_DIV05n = not1(reg.reg_div.TAMA_DIV05p.qp_new());
  /*_p01.UREK*/ wire UREK_DIV07n = not1(reg.reg_div.TULU_DIV07p.qp_new());

  /*#p03.UKAP*/ wire UKAP_CLK_MUXa  = mux2n(reg.reg_tac.SOPU_TAC0p.qp_new(), UVYN_DIV05n, UVYR_DIV03n);
  /*#p03.TEKO*/ wire TEKO_CLK_MUXb  = mux2n(reg.reg_tac.SOPU_TAC0p.qp_new(), UBOT_DIV01n, UREK_DIV07n);
  /*#p03.TECY*/ wire TECY_CLK_MUXc  = mux2n(reg.reg_tac.SAMY_TAC1p.qp_new(), UKAP_CLK_MUXa, TEKO_CLK_MUXb);
  /*#p03.SOGU*/ wire SOGU_TIMA_CLKn = nor2(TECY_CLK_MUXc, reg.reg_tac.SABO_TAC2p.qn_new());

  ///*#p03.TOPE*/ wire TOPE_FF05_WRn = nand4(cpu_signals.TAPU_CPU_WRp.qp_new(), cpu_signals.RYFO_FF04_FF07p(),  cpu_signals.TOLA_A01n(), cpu_signals.BUS_CPU_A00p.qp_new());

  /*#p03.ROKE*/ wire ROKE_TIMA_D0 = mux2n(TOPE_FF05_WRn, reg.reg_tma.SABU_TMA0p.qp_new(), reg.cpu_dbus.BUS_CPU_D00p.out_new());
  /*#p03.PETU*/ wire PETU_TIMA_D1 = mux2n(TOPE_FF05_WRn, reg.reg_tma.NYKE_TMA1p.qp_new(), reg.cpu_dbus.BUS_CPU_D01p.out_new());
  /*#p03.NYKU*/ wire NYKU_TIMA_D2 = mux2n(TOPE_FF05_WRn, reg.reg_tma.MURU_TMA2p.qp_new(), reg.cpu_dbus.BUS_CPU_D02p.out_new());
  /*#p03.SOCE*/ wire SOCE_TIMA_D3 = mux2n(TOPE_FF05_WRn, reg.reg_tma.TYVA_TMA3p.qp_new(), reg.cpu_dbus.BUS_CPU_D03p.out_new());
  /*#p03.SALA*/ wire SALA_TIMA_D4 = mux2n(TOPE_FF05_WRn, reg.reg_tma.TYRU_TMA4p.qp_new(), reg.cpu_dbus.BUS_CPU_D04p.out_new());
  /*#p03.SYRU*/ wire SYRU_TIMA_D5 = mux2n(TOPE_FF05_WRn, reg.reg_tma.SUFY_TMA5p.qp_new(), reg.cpu_dbus.BUS_CPU_D05p.out_new());
  /*#p03.REFU*/ wire REFU_TIMA_D6 = mux2n(TOPE_FF05_WRn, reg.reg_tma.PETO_TMA6p.qp_new(), reg.cpu_dbus.BUS_CPU_D06p.out_new());
  /*#p03.RATO*/ wire RATO_TIMA_D7 = mux2n(TOPE_FF05_WRn, reg.reg_tma.SETA_TMA7p.qp_new(), reg.cpu_dbus.BUS_CPU_D07p.out_new());

  /*#p03.PUXY*/ wire PUXY_TIMA_D0 = nor2(reg.sys_rst.MULO_SYS_RSTn(), ROKE_TIMA_D0);
  /*#p03.NERO*/ wire NERO_TIMA_D1 = nor2(reg.sys_rst.MULO_SYS_RSTn(), PETU_TIMA_D1);
  /*#p03.NADA*/ wire NADA_TIMA_D2 = nor2(reg.sys_rst.MULO_SYS_RSTn(), NYKU_TIMA_D2);
  /*#p03.REPA*/ wire REPA_TIMA_D3 = nor2(reg.sys_rst.MULO_SYS_RSTn(), SOCE_TIMA_D3);
  /*#p03.ROLU*/ wire ROLU_TIMA_D4 = nor2(reg.sys_rst.MULO_SYS_RSTn(), SALA_TIMA_D4);
  /*#p03.RUGY*/ wire RUGY_TIMA_D5 = nor2(reg.sys_rst.MULO_SYS_RSTn(), SYRU_TIMA_D5);
  /*#p03.PYMA*/ wire PYMA_TIMA_D6 = nor2(reg.sys_rst.MULO_SYS_RSTn(), REFU_TIMA_D6);
  /*#p03.PAGU*/ wire PAGU_TIMA_D7 = nor2(reg.sys_rst.MULO_SYS_RSTn(), RATO_TIMA_D7);

  // FIXME puxy etc should be old

  /*#p03.REGA*/ reg.reg_tima.REGA_TIMA0p.dff20(SOGU_TIMA_CLKn,                MEXU_TIMA_LOADp, PUXY_TIMA_D0);
  /*#p03.POVY*/ reg.reg_tima.POVY_TIMA1p.dff20(reg.reg_tima.REGA_TIMA0p.qp_any(), MEXU_TIMA_LOADp, NERO_TIMA_D1);
  /*#p03.PERU*/ reg.reg_tima.PERU_TIMA2p.dff20(reg.reg_tima.POVY_TIMA1p.qp_any(), MEXU_TIMA_LOADp, NADA_TIMA_D2);
  /*#p03.RATE*/ reg.reg_tima.RATE_TIMA3p.dff20(reg.reg_tima.PERU_TIMA2p.qp_any(), MEXU_TIMA_LOADp, REPA_TIMA_D3);
  /*#p03.RUBY*/ reg.reg_tima.RUBY_TIMA4p.dff20(reg.reg_tima.RATE_TIMA3p.qp_any(), MEXU_TIMA_LOADp, ROLU_TIMA_D4);
  /*#p03.RAGE*/ reg.reg_tima.RAGE_TIMA5p.dff20(reg.reg_tima.RUBY_TIMA4p.qp_any(), MEXU_TIMA_LOADp, RUGY_TIMA_D5);
  /*#p03.PEDA*/ reg.reg_tima.PEDA_TIMA6p.dff20(reg.reg_tima.RAGE_TIMA5p.qp_any(), MEXU_TIMA_LOADp, PYMA_TIMA_D6);
  /*#p03.NUGA*/ reg.reg_tima.NUGA_TIMA7p.dff20(reg.reg_tima.PEDA_TIMA6p.qp_any(), MEXU_TIMA_LOADp, PAGU_TIMA_D7);

  /*#p03.TEDA*/ wire TEDA_FF05_RDp =  and4(reg.cpu_signals.TEDO_CPU_RDp.out_new(), reg.cpu_abus.RYFO_FF04_FF07p(),  reg.cpu_abus.TOLA_A01n(), reg.cpu_abus.BUS_CPU_A00p.out_new());
  /*#p03.SOKU*/ triwire SOKU_TIMA0_TO_CD0 = tri6_pn(TEDA_FF05_RDp, reg.reg_tima.REGA_TIMA0p.qn_new());
  /*#p03.RACY*/ triwire RACY_TIMA1_TO_CD1 = tri6_pn(TEDA_FF05_RDp, reg.reg_tima.POVY_TIMA1p.qn_new());
  /*#p03.RAVY*/ triwire RAVY_TIMA2_TO_CD2 = tri6_pn(TEDA_FF05_RDp, reg.reg_tima.PERU_TIMA2p.qn_new());
  /*#p03.SOSY*/ triwire SOSY_TIMA3_TO_CD3 = tri6_pn(TEDA_FF05_RDp, reg.reg_tima.RATE_TIMA3p.qn_new());
  /*#p03.SOMU*/ triwire SOMU_TIMA4_TO_CD4 = tri6_pn(TEDA_FF05_RDp, reg.reg_tima.RUBY_TIMA4p.qn_new());
  /*#p03.SURO*/ triwire SURO_TIMA5_TO_CD5 = tri6_pn(TEDA_FF05_RDp, reg.reg_tima.RAGE_TIMA5p.qn_new());
  /*#p03.ROWU*/ triwire ROWU_TIMA6_TO_CD6 = tri6_pn(TEDA_FF05_RDp, reg.reg_tima.PEDA_TIMA6p.qn_new());
  /*#p03.PUSO*/ triwire PUSO_TIMA7_TO_CD7 = tri6_pn(TEDA_FF05_RDp, reg.reg_tima.NUGA_TIMA7p.qn_new());

  /*_BUS_CPU_D00p*/ reg.cpu_dbus.BUS_CPU_D00p.tri_bus(SOKU_TIMA0_TO_CD0);
  /*_BUS_CPU_D01p*/ reg.cpu_dbus.BUS_CPU_D01p.tri_bus(RACY_TIMA1_TO_CD1);
  /*_BUS_CPU_D02p*/ reg.cpu_dbus.BUS_CPU_D02p.tri_bus(RAVY_TIMA2_TO_CD2);
  /*_BUS_CPU_D03p*/ reg.cpu_dbus.BUS_CPU_D03p.tri_bus(SOSY_TIMA3_TO_CD3);
  /*_BUS_CPU_D04p*/ reg.cpu_dbus.BUS_CPU_D04p.tri_bus(SOMU_TIMA4_TO_CD4);
  /*_BUS_CPU_D05p*/ reg.cpu_dbus.BUS_CPU_D05p.tri_bus(SURO_TIMA5_TO_CD5);
  /*_BUS_CPU_D06p*/ reg.cpu_dbus.BUS_CPU_D06p.tri_bus(ROWU_TIMA6_TO_CD6);
  /*_BUS_CPU_D07p*/ reg.cpu_dbus.BUS_CPU_D07p.tri_bus(PUSO_TIMA7_TO_CD7);

  /*_p03.TUBY*/ wire TUBY_FF06_RDp =  and4(reg.cpu_signals.TEDO_CPU_RDp.out_new(), reg.cpu_abus.RYFO_FF04_FF07p(), reg.cpu_abus.BUS_CPU_A01p.out_new(), reg.cpu_abus.TOVY_A00n());
  /*#p03.SETE*/ triwire SETE_TMA0_TO_CD0 = tri6_pn(TUBY_FF06_RDp, reg.reg_tma.SABU_TMA0p.qn_new());
  /*#p03.PYRE*/ triwire PYRE_TMA1_TO_CD1 = tri6_pn(TUBY_FF06_RDp, reg.reg_tma.NYKE_TMA1p.qn_new());
  /*#p03.NOLA*/ triwire NOLA_TMA2_TO_CD2 = tri6_pn(TUBY_FF06_RDp, reg.reg_tma.MURU_TMA2p.qn_new());
  /*#p03.SALU*/ triwire SALU_TMA3_TO_CD3 = tri6_pn(TUBY_FF06_RDp, reg.reg_tma.TYVA_TMA3p.qn_new());
  /*#p03.SUPO*/ triwire SUPO_TMA4_TO_CD4 = tri6_pn(TUBY_FF06_RDp, reg.reg_tma.TYRU_TMA4p.qn_new());
  /*#p03.SOTU*/ triwire SOTU_TMA5_TO_CD5 = tri6_pn(TUBY_FF06_RDp, reg.reg_tma.SUFY_TMA5p.qn_new());
  /*#p03.REVA*/ triwire REVA_TMA6_TO_CD6 = tri6_pn(TUBY_FF06_RDp, reg.reg_tma.PETO_TMA6p.qn_new());
  /*#p03.SAPU*/ triwire SAPU_TMA7_TO_CD7 = tri6_pn(TUBY_FF06_RDp, reg.reg_tma.SETA_TMA7p.qn_new());

  /*_BUS_CPU_D00p*/ reg.cpu_dbus.BUS_CPU_D00p.tri_bus(SETE_TMA0_TO_CD0);
  /*_BUS_CPU_D01p*/ reg.cpu_dbus.BUS_CPU_D01p.tri_bus(PYRE_TMA1_TO_CD1);
  /*_BUS_CPU_D02p*/ reg.cpu_dbus.BUS_CPU_D02p.tri_bus(NOLA_TMA2_TO_CD2);
  /*_BUS_CPU_D03p*/ reg.cpu_dbus.BUS_CPU_D03p.tri_bus(SALU_TMA3_TO_CD3);
  /*_BUS_CPU_D04p*/ reg.cpu_dbus.BUS_CPU_D04p.tri_bus(SUPO_TMA4_TO_CD4);
  /*_BUS_CPU_D05p*/ reg.cpu_dbus.BUS_CPU_D05p.tri_bus(SOTU_TMA5_TO_CD5);
  /*_BUS_CPU_D06p*/ reg.cpu_dbus.BUS_CPU_D06p.tri_bus(REVA_TMA6_TO_CD6);
  /*_BUS_CPU_D07p*/ reg.cpu_dbus.BUS_CPU_D07p.tri_bus(SAPU_TMA7_TO_CD7);

  /*_p03.SORA*/ wire SORA_FF07_RDp =  and4(reg.cpu_signals.TEDO_CPU_RDp.out_new(), reg.cpu_abus.RYFO_FF04_FF07p(), reg.cpu_abus.BUS_CPU_A01p.out_new(), reg.cpu_abus.BUS_CPU_A00p.out_new());
  /*#p03.RYLA*/ triwire RYLA_TAC0_TO_CD0 = tri6_pn(SORA_FF07_RDp, reg.reg_tac.SOPU_TAC0p.qn_new());
  /*#p03.ROTE*/ triwire ROTE_TAC1_TO_CD1 = tri6_pn(SORA_FF07_RDp, reg.reg_tac.SAMY_TAC1p.qn_new());
  /*#p03.SUPE*/ triwire SUPE_TAC2_TO_CD2 = tri6_pn(SORA_FF07_RDp, reg.reg_tac.SABO_TAC2p.qn_new());

  /*_BUS_CPU_D00p*/ reg.cpu_dbus.BUS_CPU_D00p.tri_bus(RYLA_TAC0_TO_CD0);
  /*_BUS_CPU_D01p*/ reg.cpu_dbus.BUS_CPU_D01p.tri_bus(ROTE_TAC1_TO_CD1);
  /*_BUS_CPU_D02p*/ reg.cpu_dbus.BUS_CPU_D02p.tri_bus(SUPE_TAC2_TO_CD2);
}

//-----------------------------------------------------------------------------
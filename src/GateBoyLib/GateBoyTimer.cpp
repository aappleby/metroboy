#include "GateBoyLib/GateBoyTimer.h"

#include "GateBoyLib/GateBoy.h"
#include "GateBoyLib/Gates.h"

//-----------------------------------------------------------------------------
// this is hacked up because we're ignoring the debug reg for the moment
///*_p01.ULUR*/ wire ULUR_DIV_06_clknew = /*mux2p(FF60_1, BOGA_Axxxxxxx,*/ TAMA_DIV05p.qn_new() /*)*/;
///*_p01.UGOT*/ UGOT_DIV06p.dff17_clk(ULUR_DIV_06_clknew,  UGOT_DIV06p.qn_old());

void GateBoy::tock_div_gates(const GateBoyState& reg_old) {
  GateBoyState& reg_new = gb_state;

  /*_p01.ABOL*/ wire ABOL_CLKREQn  = not1(reg_new.sys_clk.SIG_CPU_CLKREQ.out_new());
  /*#p01.AROV*/ wire AROV_xxCDEFxx = not1(reg_new.sys_clk.APUK_xxCDEFxx.qn_newB());
  /*#p01.AFEP*/ wire AFEP_AxxxxFGH = not1(reg_new.sys_clk.ALEF_xBCDExxx.qp_newB());
  /*#p01.ATYP*/ wire ATYP_ABCDxxxx = not1(reg_new.sys_clk.AFUR_ABCDxxxx.qn_newB());

  /*#p01.BELU*/ wire BELU_xxxxEFGH = nor2(ATYP_ABCDxxxx, ABOL_CLKREQn);
  /*#p01.BYRY*/ wire BYRY_ABCDxxxx = not1(BELU_xxxxEFGH);
  /*#p01.BUDE*/ wire BUDE_xxxxEFGH = not1(BYRY_ABCDxxxx);
  /*#p01.BAPY*/ wire BAPY_xxxxxxGH = nor3(ABOL_CLKREQn, AROV_xxCDEFxx, ATYP_ABCDxxxx);
  /*#p01.BERU*/ wire BERU_ABCDEFxx = not1(BAPY_xxxxxxGH);
  /*#p01.BUFA*/ wire BUFA_xxxxxxGH = not1(BERU_ABCDEFxx);
  /*#p01.BOLO*/ wire BOLO_ABCDEFxx = not1(BUFA_xxxxxxGH);
  /*#p01.BEKO*/ wire BEKO_ABCDxxxx = not1(BUDE_xxxxEFGH); // BEKO+BAVY parallel
  /*#p01.BEJA*/ wire BEJA_xxxxEFGH = nand4(BOLO_ABCDEFxx, BOLO_ABCDEFxx, BEKO_ABCDxxxx, BEKO_ABCDxxxx);
  /*#p01.BANE*/ wire BANE_ABCDxxxx = not1(BEJA_xxxxEFGH);
  /*#p01.BELO*/ wire BELO_xxxxEFGH = not1(BANE_ABCDxxxx);
  /*#p01.BAZE*/ wire BAZE_ABCDxxxx = not1(BELO_xxxxEFGH);
  /*#p01.BUTO*/ wire BUTO_xBCDEFGH = nand3(AFEP_AxxxxFGH, ATYP_ABCDxxxx, BAZE_ABCDxxxx);
  /*#p01.BELE*/ wire BELE_Axxxxxxx = not1(BUTO_xBCDEFGH);
  /*#p01.BYJU*/ wire BYJU_Axxxxxxx = or2(BELE_Axxxxxxx, pins.sys.ATEZ_CLKBADp_new());
  /*#p01.BALY*/ wire BALY_xBCDEFGH = not1(BYJU_Axxxxxxx);
  /*_p01.BOGA*/ wire BOGA_Axxxxxxx = not1(BALY_xBCDEFGH);

  /*_p01.TAPE*/ wire TAPE_FF04_WRp_new = and4(reg_new.cpu_signals.TAPU_CPU_WRp.out_new(), reg_new.cpu_abus.RYFO_FF04_FF07p_new(), reg_new.cpu_abus.TOLA_A01n_new(), reg_new.cpu_abus.TOVY_A00n_new());
  /*_p01.UFOL*/ wire UFOL_DIV_RSTn_new = nor3(pins.sys.UCOB_CLKBADp_new(), pins.sys.PIN_71_RST.qp_int_new(), TAPE_FF04_WRp_new);

  /*_p01.UKUP*/ reg_new.reg_div.UKUP_DIV00p.dff17(BOGA_Axxxxxxx,                    UFOL_DIV_RSTn_new, reg_old.reg_div.UKUP_DIV00p.qn_old());
  /*_p01.UFOR*/ reg_new.reg_div.UFOR_DIV01p.dff17(reg_new.reg_div.UKUP_DIV00p.qn_any(), UFOL_DIV_RSTn_new, reg_old.reg_div.UFOR_DIV01p.qn_old());
  /*_p01.UNER*/ reg_new.reg_div.UNER_DIV02p.dff17(reg_new.reg_div.UFOR_DIV01p.qn_any(), UFOL_DIV_RSTn_new, reg_old.reg_div.UNER_DIV02p.qn_old());
  /*#p01.TERO*/ reg_new.reg_div.TERO_DIV03p.dff17(reg_new.reg_div.UNER_DIV02p.qn_any(), UFOL_DIV_RSTn_new, reg_old.reg_div.TERO_DIV03p.qn_old());
  /*_p01.UNYK*/ reg_new.reg_div.UNYK_DIV04p.dff17(reg_new.reg_div.TERO_DIV03p.qn_any(), UFOL_DIV_RSTn_new, reg_old.reg_div.UNYK_DIV04p.qn_old());
  /*_p01.TAMA*/ reg_new.reg_div.TAMA_DIV05p.dff17(reg_new.reg_div.UNYK_DIV04p.qn_any(), UFOL_DIV_RSTn_new, reg_old.reg_div.TAMA_DIV05p.qn_old());
  /*_p01.UGOT*/ reg_new.reg_div.UGOT_DIV06p.dff17(reg_new.reg_div.TAMA_DIV05p.qn_any(), UFOL_DIV_RSTn_new, reg_old.reg_div.UGOT_DIV06p.qn_old());
  /*_p01.TULU*/ reg_new.reg_div.TULU_DIV07p.dff17(reg_new.reg_div.UGOT_DIV06p.qn_any(), UFOL_DIV_RSTn_new, reg_old.reg_div.TULU_DIV07p.qn_old());
  /*_p01.TUGO*/ reg_new.reg_div.TUGO_DIV08p.dff17(reg_new.reg_div.TULU_DIV07p.qn_any(), UFOL_DIV_RSTn_new, reg_old.reg_div.TUGO_DIV08p.qn_old());
  /*_p01.TOFE*/ reg_new.reg_div.TOFE_DIV09p.dff17(reg_new.reg_div.TUGO_DIV08p.qn_any(), UFOL_DIV_RSTn_new, reg_old.reg_div.TOFE_DIV09p.qn_old());
  /*_p01.TERU*/ reg_new.reg_div.TERU_DIV10p.dff17(reg_new.reg_div.TOFE_DIV09p.qn_any(), UFOL_DIV_RSTn_new, reg_old.reg_div.TERU_DIV10p.qn_old());
  /*_p01.SOLA*/ reg_new.reg_div.SOLA_DIV11p.dff17(reg_new.reg_div.TERU_DIV10p.qn_any(), UFOL_DIV_RSTn_new, reg_old.reg_div.SOLA_DIV11p.qn_old());
  /*_p01.SUBU*/ reg_new.reg_div.SUBU_DIV12p.dff17(reg_new.reg_div.SOLA_DIV11p.qn_any(), UFOL_DIV_RSTn_new, reg_old.reg_div.SUBU_DIV12p.qn_old());
  /*_p01.TEKA*/ reg_new.reg_div.TEKA_DIV13p.dff17(reg_new.reg_div.SUBU_DIV12p.qn_any(), UFOL_DIV_RSTn_new, reg_old.reg_div.TEKA_DIV13p.qn_old());
  /*_p01.UKET*/ reg_new.reg_div.UKET_DIV14p.dff17(reg_new.reg_div.TEKA_DIV13p.qn_any(), UFOL_DIV_RSTn_new, reg_old.reg_div.UKET_DIV14p.qn_old());
  /*_p01.UPOF*/ reg_new.reg_div.UPOF_DIV15p.dff17(reg_new.reg_div.UKET_DIV14p.qn_any(), UFOL_DIV_RSTn_new, reg_old.reg_div.UPOF_DIV15p.qn_old());

  /*_p01.UMEK*/ wire UMEK_DIV06n_new = not1(reg_new.reg_div.UGOT_DIV06p.qp_new());
  /*_p01.UREK*/ wire UREK_DIV07n_new = not1(reg_new.reg_div.TULU_DIV07p.qp_new());
  /*_p01.UTOK*/ wire UTOK_DIV08n_new = not1(reg_new.reg_div.TUGO_DIV08p.qp_new());
  /*_p01.SAPY*/ wire SAPY_DIV09n_new = not1(reg_new.reg_div.TOFE_DIV09p.qp_new());
  /*_p01.UMER*/ wire UMER_DIV10n_new = not1(reg_new.reg_div.TERU_DIV10p.qp_new());
  /*_p01.RAVE*/ wire RAVE_DIV11n_new = not1(reg_new.reg_div.SOLA_DIV11p.qp_new());
  /*_p01.RYSO*/ wire RYSO_DIV12n_new = not1(reg_new.reg_div.SUBU_DIV12p.qp_new());
  /*_p01.UDOR*/ wire UDOR_DIV13n_new = not1(reg_new.reg_div.TEKA_DIV13p.qp_new());

  /*_p01.TAGY*/ wire TAGY_FF04_RDp_ext_new = and4(reg_new.cpu_signals.TEDO_CPU_RDp.out_new(), reg_new.cpu_abus.RYFO_FF04_FF07p_new(), reg_new.cpu_abus.TOLA_A01n_new(), reg_new.cpu_abus.TOVY_A00n_new());
  /*_p01.TAWU*/ triwire TAWU_DIV06_TO_CD0_new = tri6_pn(TAGY_FF04_RDp_ext_new, UMEK_DIV06n_new);
  /*_p01.TAKU*/ triwire TAKU_DIV07_TO_CD1_new = tri6_pn(TAGY_FF04_RDp_ext_new, UREK_DIV07n_new);
  /*_p01.TEMU*/ triwire TEMU_DIV08_TO_CD2_new = tri6_pn(TAGY_FF04_RDp_ext_new, UTOK_DIV08n_new);
  /*_p01.TUSE*/ triwire TUSE_DIV09_TO_CD3_new = tri6_pn(TAGY_FF04_RDp_ext_new, SAPY_DIV09n_new);
  /*_p01.UPUG*/ triwire UPUG_DIV10_TO_CD4_new = tri6_pn(TAGY_FF04_RDp_ext_new, UMER_DIV10n_new); // Schematic wrong, UPUG/SEPU driving D5/D4
  /*_p01.SEPU*/ triwire SEPU_DIV11_TO_CD5_new = tri6_pn(TAGY_FF04_RDp_ext_new, RAVE_DIV11n_new);
  /*_p01.SAWA*/ triwire SAWA_DIV12_TO_CD6_new = tri6_pn(TAGY_FF04_RDp_ext_new, RYSO_DIV12n_new);
  /*_p01.TATU*/ triwire TATU_DIV13_TO_CD7_new = tri6_pn(TAGY_FF04_RDp_ext_new, UDOR_DIV13n_new);

  /*_BUS_CPU_D00p*/ reg_new.cpu_dbus.BUS_CPU_D00p.tri_bus(TAWU_DIV06_TO_CD0_new);
  /*_BUS_CPU_D01p*/ reg_new.cpu_dbus.BUS_CPU_D01p.tri_bus(TAKU_DIV07_TO_CD1_new);
  /*_BUS_CPU_D02p*/ reg_new.cpu_dbus.BUS_CPU_D02p.tri_bus(TEMU_DIV08_TO_CD2_new);
  /*_BUS_CPU_D03p*/ reg_new.cpu_dbus.BUS_CPU_D03p.tri_bus(TUSE_DIV09_TO_CD3_new);
  /*_BUS_CPU_D04p*/ reg_new.cpu_dbus.BUS_CPU_D04p.tri_bus(UPUG_DIV10_TO_CD4_new);
  /*_BUS_CPU_D05p*/ reg_new.cpu_dbus.BUS_CPU_D05p.tri_bus(SEPU_DIV11_TO_CD5_new);
  /*_BUS_CPU_D06p*/ reg_new.cpu_dbus.BUS_CPU_D06p.tri_bus(SAWA_DIV12_TO_CD6_new);
  /*_BUS_CPU_D07p*/ reg_new.cpu_dbus.BUS_CPU_D07p.tri_bus(TATU_DIV13_TO_CD7_new);
}

//-----------------------------------------------------------------------------

void GateBoy::tock_timer_gates(const GateBoyState& reg_old) {
  GateBoyState& reg_new = gb_state;

  /*#p01.AVOR*/ wire AVOR_SYS_RSTp =  or2(reg_new.sys_rst.AFER_SYS_RSTp.qp_new(), reg_new.sys_rst.ASOL_POR_DONEn.qp_new());
  /*#p01.ALUR*/ wire ALUR_SYS_RSTn = not1(AVOR_SYS_RSTp);

  /*_p01.ABOL*/ wire ABOL_CLKREQn  = not1(reg_new.sys_clk.SIG_CPU_CLKREQ.out_new());
  /*#p01.AROV*/ wire AROV_xxCDEFxx = not1(reg_new.sys_clk.APUK_xxCDEFxx.qn_newB());
  /*#p01.AFEP*/ wire AFEP_AxxxxFGH = not1(reg_new.sys_clk.ALEF_xBCDExxx.qp_newB());
  /*#p01.ATYP*/ wire ATYP_ABCDxxxx = not1(reg_new.sys_clk.AFUR_ABCDxxxx.qn_newB());

  /*#p01.BAPY*/ wire BAPY_xxxxxxGH = nor3(ABOL_CLKREQn, AROV_xxCDEFxx, ATYP_ABCDxxxx);
  /*#p01.BELU*/ wire BELU_xxxxEFGH = nor2(ATYP_ABCDxxxx, ABOL_CLKREQn);

  /*#p01.BERU*/ wire BERU_ABCDEFxx = not1(BAPY_xxxxxxGH);
  /*#p01.BUFA*/ wire BUFA_xxxxxxGH = not1(BERU_ABCDEFxx);
  /*#p01.BOLO*/ wire BOLO_ABCDEFxx = not1(BUFA_xxxxxxGH);
  /*#p01.BYRY*/ wire BYRY_ABCDxxxx = not1(BELU_xxxxEFGH);
  /*#p01.BUDE*/ wire BUDE_xxxxEFGH = not1(BYRY_ABCDxxxx);
  /*#p01.BEKO*/ wire BEKO_ABCDxxxx = not1(BUDE_xxxxEFGH); // BEKO+BAVY parallel
  /*#p01.BEJA*/ wire BEJA_xxxxEFGH = nand4(BOLO_ABCDEFxx, BOLO_ABCDEFxx, BEKO_ABCDxxxx, BEKO_ABCDxxxx);
  /*#p01.BANE*/ wire BANE_ABCDxxxx = not1(BEJA_xxxxEFGH);
  /*#p01.BELO*/ wire BELO_xxxxEFGH = not1(BANE_ABCDxxxx);
  /*#p01.BAZE*/ wire BAZE_ABCDxxxx = not1(BELO_xxxxEFGH);
  /*#p01.BUTO*/ wire BUTO_xBCDEFGH = nand3(AFEP_AxxxxFGH, ATYP_ABCDxxxx, BAZE_ABCDxxxx);
  /*#p01.BELE*/ wire BELE_Axxxxxxx = not1(BUTO_xBCDEFGH);
  /*#p01.BYJU*/ wire BYJU_Axxxxxxx = or2(BELE_Axxxxxxx, pins.sys.ATEZ_CLKBADp_new());
  /*#p01.BALY*/ wire BALY_xBCDEFGH = not1(BYJU_Axxxxxxx);
  /*_p01.BOGA*/ wire BOGA_Axxxxxxx = not1(BALY_xBCDEFGH);

  /*_p03.TYJU*/ wire TYJU_FF06_WRn_new = nand4(reg_new.cpu_signals.TAPU_CPU_WRp.out_new(), reg_new.cpu_abus.RYFO_FF04_FF07p_new(), reg_new.cpu_abus.BUS_CPU_A01p.out_new(), reg_new.cpu_abus.TOVY_A00n_new());
  /*_p03.SABU*/ reg_new.reg_tma.SABU_TMA0p.dff17(TYJU_FF06_WRn_new, ALUR_SYS_RSTn, reg_old.cpu_dbus.BUS_CPU_D00p.out_old());
  /*_p03.NYKE*/ reg_new.reg_tma.NYKE_TMA1p.dff17(TYJU_FF06_WRn_new, ALUR_SYS_RSTn, reg_old.cpu_dbus.BUS_CPU_D01p.out_old());
  /*_p03.MURU*/ reg_new.reg_tma.MURU_TMA2p.dff17(TYJU_FF06_WRn_new, ALUR_SYS_RSTn, reg_old.cpu_dbus.BUS_CPU_D02p.out_old());
  /*_p03.TYVA*/ reg_new.reg_tma.TYVA_TMA3p.dff17(TYJU_FF06_WRn_new, ALUR_SYS_RSTn, reg_old.cpu_dbus.BUS_CPU_D03p.out_old());
  /*_p03.TYRU*/ reg_new.reg_tma.TYRU_TMA4p.dff17(TYJU_FF06_WRn_new, ALUR_SYS_RSTn, reg_old.cpu_dbus.BUS_CPU_D04p.out_old());
  /*_p03.SUFY*/ reg_new.reg_tma.SUFY_TMA5p.dff17(TYJU_FF06_WRn_new, ALUR_SYS_RSTn, reg_old.cpu_dbus.BUS_CPU_D05p.out_old());
  /*_p03.PETO*/ reg_new.reg_tma.PETO_TMA6p.dff17(TYJU_FF06_WRn_new, ALUR_SYS_RSTn, reg_old.cpu_dbus.BUS_CPU_D06p.out_old());
  /*_p03.SETA*/ reg_new.reg_tma.SETA_TMA7p.dff17(TYJU_FF06_WRn_new, ALUR_SYS_RSTn, reg_old.cpu_dbus.BUS_CPU_D07p.out_old());

  /*_p03.SARA*/ wire SARA_FF07_WRn_new = nand4(reg_new.cpu_signals.TAPU_CPU_WRp.out_new(), reg_new.cpu_abus.RYFO_FF04_FF07p_new(), reg_new.cpu_abus.BUS_CPU_A01p.out_new(), reg_new.cpu_abus.BUS_CPU_A00p.out_new());
  /*_p03.SOPU*/ reg_new.reg_tac.SOPU_TAC0p.dff17(SARA_FF07_WRn_new, ALUR_SYS_RSTn, reg_old.cpu_dbus.BUS_CPU_D00p.out_old());
  /*_p03.SAMY*/ reg_new.reg_tac.SAMY_TAC1p.dff17(SARA_FF07_WRn_new, ALUR_SYS_RSTn, reg_old.cpu_dbus.BUS_CPU_D01p.out_old());
  /*_p03.SABO*/ reg_new.reg_tac.SABO_TAC2p.dff17(SARA_FF07_WRn_new, ALUR_SYS_RSTn, reg_old.cpu_dbus.BUS_CPU_D02p.out_old());

  /*#p03.MERY*/ wire MERY_TIMER_OVERFLOWp_old = nor2(reg_old.reg_tima.NUGA_TIMA7p.qp_old(), reg_old.int_ctrl.NYDU_TIMA7p_DELAY.qn_old());
  /*#p03.MOBA*/ reg_new.int_ctrl.MOBA_TIMER_OVERFLOWp.dff17(BOGA_Axxxxxxx, ALUR_SYS_RSTn, MERY_TIMER_OVERFLOWp_old);

  /*#p03.TOPE*/ wire TOPE_FF05_WRn_new        = nand4(reg_new.cpu_signals.TAPU_CPU_WRp.out_new(), reg_new.cpu_abus.RYFO_FF04_FF07p_new(), reg_new.cpu_abus.TOLA_A01n_new(), reg_new.cpu_abus.BUS_CPU_A00p.out_new());
  /*#p03.MUZU*/ wire MUZU_CPU_LOAD_TIMAn_new  = or2(reg_new.cpu_signals.SIG_IN_CPU_DBUS_FREE.out_new(), TOPE_FF05_WRn_new);
  /*#p03.MEKE*/ wire MEKE_TIMER_OVERFLOWn_new = not1(reg_new.int_ctrl.MOBA_TIMER_OVERFLOWp.qp_new());
  /*#p03.MEXU*/ wire MEXU_TIMA_LOADp_new      = nand3(MUZU_CPU_LOAD_TIMAn_new, ALUR_SYS_RSTn, MEKE_TIMER_OVERFLOWn_new);
  /*#p03.MUGY*/ wire MUGY_TIMA_MAX_RSTn_new   = not1(MEXU_TIMA_LOADp_new);
  /*#p03.NYDU*/ reg_new.int_ctrl.NYDU_TIMA7p_DELAY.dff17(BOGA_Axxxxxxx, MUGY_TIMA_MAX_RSTn_new, reg_old.reg_tima.NUGA_TIMA7p.qp_old());

  /*#p03.UBOT*/ wire UBOT_DIV01n_new = not1(reg_new.reg_div.UFOR_DIV01p.qp_new());
  /*#p03.UVYR*/ wire UVYR_DIV03n_new = not1(reg_new.reg_div.TERO_DIV03p.qp_new());
  /*#p01.UVYN*/ wire UVYN_DIV05n_new = not1(reg_new.reg_div.TAMA_DIV05p.qp_new());
  /*_p01.UREK*/ wire UREK_DIV07n_new = not1(reg_new.reg_div.TULU_DIV07p.qp_new());

  /*#p03.UKAP*/ wire UKAP_CLK_MUXa_new  = mux2n(reg_new.reg_tac.SOPU_TAC0p.qp_new(), UVYN_DIV05n_new, UVYR_DIV03n_new);
  /*#p03.TEKO*/ wire TEKO_CLK_MUXb_new  = mux2n(reg_new.reg_tac.SOPU_TAC0p.qp_new(), UBOT_DIV01n_new, UREK_DIV07n_new);
  /*#p03.TECY*/ wire TECY_CLK_MUXc_new  = mux2n(reg_new.reg_tac.SAMY_TAC1p.qp_new(), UKAP_CLK_MUXa_new, TEKO_CLK_MUXb_new);
  /*#p03.SOGU*/ wire SOGU_TIMA_CLKn_new = nor2(TECY_CLK_MUXc_new, reg_new.reg_tac.SABO_TAC2p.qn_new());

  /*#p03.ROKE*/ wire ROKE_TIMA_D0_new = mux2n(TOPE_FF05_WRn_new, reg_new.reg_tma.SABU_TMA0p.qp_new(), reg_new.cpu_dbus.BUS_CPU_D00p.out_new());
  /*#p03.PETU*/ wire PETU_TIMA_D1_new = mux2n(TOPE_FF05_WRn_new, reg_new.reg_tma.NYKE_TMA1p.qp_new(), reg_new.cpu_dbus.BUS_CPU_D01p.out_new());
  /*#p03.NYKU*/ wire NYKU_TIMA_D2_new = mux2n(TOPE_FF05_WRn_new, reg_new.reg_tma.MURU_TMA2p.qp_new(), reg_new.cpu_dbus.BUS_CPU_D02p.out_new());
  /*#p03.SOCE*/ wire SOCE_TIMA_D3_new = mux2n(TOPE_FF05_WRn_new, reg_new.reg_tma.TYVA_TMA3p.qp_new(), reg_new.cpu_dbus.BUS_CPU_D03p.out_new());
  /*#p03.SALA*/ wire SALA_TIMA_D4_new = mux2n(TOPE_FF05_WRn_new, reg_new.reg_tma.TYRU_TMA4p.qp_new(), reg_new.cpu_dbus.BUS_CPU_D04p.out_new());
  /*#p03.SYRU*/ wire SYRU_TIMA_D5_new = mux2n(TOPE_FF05_WRn_new, reg_new.reg_tma.SUFY_TMA5p.qp_new(), reg_new.cpu_dbus.BUS_CPU_D05p.out_new());
  /*#p03.REFU*/ wire REFU_TIMA_D6_new = mux2n(TOPE_FF05_WRn_new, reg_new.reg_tma.PETO_TMA6p.qp_new(), reg_new.cpu_dbus.BUS_CPU_D06p.out_new());
  /*#p03.RATO*/ wire RATO_TIMA_D7_new = mux2n(TOPE_FF05_WRn_new, reg_new.reg_tma.SETA_TMA7p.qp_new(), reg_new.cpu_dbus.BUS_CPU_D07p.out_new());

  /*_p03.MULO*/ wire MULO_SYS_RSTn = not1(ALUR_SYS_RSTn);
  /*#p03.PUXY*/ wire PUXY_TIMA_D0_new = nor2(MULO_SYS_RSTn, ROKE_TIMA_D0_new);
  /*#p03.NERO*/ wire NERO_TIMA_D1_new = nor2(MULO_SYS_RSTn, PETU_TIMA_D1_new);
  /*#p03.NADA*/ wire NADA_TIMA_D2_new = nor2(MULO_SYS_RSTn, NYKU_TIMA_D2_new);
  /*#p03.REPA*/ wire REPA_TIMA_D3_new = nor2(MULO_SYS_RSTn, SOCE_TIMA_D3_new);
  /*#p03.ROLU*/ wire ROLU_TIMA_D4_new = nor2(MULO_SYS_RSTn, SALA_TIMA_D4_new);
  /*#p03.RUGY*/ wire RUGY_TIMA_D5_new = nor2(MULO_SYS_RSTn, SYRU_TIMA_D5_new);
  /*#p03.PYMA*/ wire PYMA_TIMA_D6_new = nor2(MULO_SYS_RSTn, REFU_TIMA_D6_new);
  /*#p03.PAGU*/ wire PAGU_TIMA_D7_new = nor2(MULO_SYS_RSTn, RATO_TIMA_D7_new);

  /*#p03.REGA*/ reg_new.reg_tima.REGA_TIMA0p.dff20(SOGU_TIMA_CLKn_new,                    MEXU_TIMA_LOADp_new, PUXY_TIMA_D0_new); // _Async_load, data must be _new_
  /*#p03.POVY*/ reg_new.reg_tima.POVY_TIMA1p.dff20(reg_new.reg_tima.REGA_TIMA0p.qp_any(), MEXU_TIMA_LOADp_new, NERO_TIMA_D1_new);
  /*#p03.PERU*/ reg_new.reg_tima.PERU_TIMA2p.dff20(reg_new.reg_tima.POVY_TIMA1p.qp_any(), MEXU_TIMA_LOADp_new, NADA_TIMA_D2_new);
  /*#p03.RATE*/ reg_new.reg_tima.RATE_TIMA3p.dff20(reg_new.reg_tima.PERU_TIMA2p.qp_any(), MEXU_TIMA_LOADp_new, REPA_TIMA_D3_new);
  /*#p03.RUBY*/ reg_new.reg_tima.RUBY_TIMA4p.dff20(reg_new.reg_tima.RATE_TIMA3p.qp_any(), MEXU_TIMA_LOADp_new, ROLU_TIMA_D4_new);
  /*#p03.RAGE*/ reg_new.reg_tima.RAGE_TIMA5p.dff20(reg_new.reg_tima.RUBY_TIMA4p.qp_any(), MEXU_TIMA_LOADp_new, RUGY_TIMA_D5_new);
  /*#p03.PEDA*/ reg_new.reg_tima.PEDA_TIMA6p.dff20(reg_new.reg_tima.RAGE_TIMA5p.qp_any(), MEXU_TIMA_LOADp_new, PYMA_TIMA_D6_new);
  /*#p03.NUGA*/ reg_new.reg_tima.NUGA_TIMA7p.dff20(reg_new.reg_tima.PEDA_TIMA6p.qp_any(), MEXU_TIMA_LOADp_new, PAGU_TIMA_D7_new);


  /*#p03.TEDA*/ wire TEDA_FF05_RDp_new =  and4(reg_new.cpu_signals.TEDO_CPU_RDp.out_new(), reg_new.cpu_abus.RYFO_FF04_FF07p_new(),  reg_new.cpu_abus.TOLA_A01n_new(), reg_new.cpu_abus.BUS_CPU_A00p.out_new());
  /*#p03.SOKU*/ triwire SOKU_TIMA0_TO_CD0_new = tri6_pn(TEDA_FF05_RDp_new, reg_new.reg_tima.REGA_TIMA0p.qn_new());
  /*#p03.RACY*/ triwire RACY_TIMA1_TO_CD1_new = tri6_pn(TEDA_FF05_RDp_new, reg_new.reg_tima.POVY_TIMA1p.qn_new());
  /*#p03.RAVY*/ triwire RAVY_TIMA2_TO_CD2_new = tri6_pn(TEDA_FF05_RDp_new, reg_new.reg_tima.PERU_TIMA2p.qn_new());
  /*#p03.SOSY*/ triwire SOSY_TIMA3_TO_CD3_new = tri6_pn(TEDA_FF05_RDp_new, reg_new.reg_tima.RATE_TIMA3p.qn_new());
  /*#p03.SOMU*/ triwire SOMU_TIMA4_TO_CD4_new = tri6_pn(TEDA_FF05_RDp_new, reg_new.reg_tima.RUBY_TIMA4p.qn_new());
  /*#p03.SURO*/ triwire SURO_TIMA5_TO_CD5_new = tri6_pn(TEDA_FF05_RDp_new, reg_new.reg_tima.RAGE_TIMA5p.qn_new());
  /*#p03.ROWU*/ triwire ROWU_TIMA6_TO_CD6_new = tri6_pn(TEDA_FF05_RDp_new, reg_new.reg_tima.PEDA_TIMA6p.qn_new());
  /*#p03.PUSO*/ triwire PUSO_TIMA7_TO_CD7_new = tri6_pn(TEDA_FF05_RDp_new, reg_new.reg_tima.NUGA_TIMA7p.qn_new());

  /*_BUS_CPU_D00p*/ reg_new.cpu_dbus.BUS_CPU_D00p.tri_bus(SOKU_TIMA0_TO_CD0_new);
  /*_BUS_CPU_D01p*/ reg_new.cpu_dbus.BUS_CPU_D01p.tri_bus(RACY_TIMA1_TO_CD1_new);
  /*_BUS_CPU_D02p*/ reg_new.cpu_dbus.BUS_CPU_D02p.tri_bus(RAVY_TIMA2_TO_CD2_new);
  /*_BUS_CPU_D03p*/ reg_new.cpu_dbus.BUS_CPU_D03p.tri_bus(SOSY_TIMA3_TO_CD3_new);
  /*_BUS_CPU_D04p*/ reg_new.cpu_dbus.BUS_CPU_D04p.tri_bus(SOMU_TIMA4_TO_CD4_new);
  /*_BUS_CPU_D05p*/ reg_new.cpu_dbus.BUS_CPU_D05p.tri_bus(SURO_TIMA5_TO_CD5_new);
  /*_BUS_CPU_D06p*/ reg_new.cpu_dbus.BUS_CPU_D06p.tri_bus(ROWU_TIMA6_TO_CD6_new);
  /*_BUS_CPU_D07p*/ reg_new.cpu_dbus.BUS_CPU_D07p.tri_bus(PUSO_TIMA7_TO_CD7_new);

  /*_p03.TUBY*/ wire TUBY_FF06_RDp_new =  and4(reg_new.cpu_signals.TEDO_CPU_RDp.out_new(), reg_new.cpu_abus.RYFO_FF04_FF07p_new(), reg_new.cpu_abus.BUS_CPU_A01p.out_new(), reg_new.cpu_abus.TOVY_A00n_new());
  /*#p03.SETE*/ triwire SETE_TMA0_TO_CD0_new = tri6_pn(TUBY_FF06_RDp_new, reg_new.reg_tma.SABU_TMA0p.qn_new());
  /*#p03.PYRE*/ triwire PYRE_TMA1_TO_CD1_new = tri6_pn(TUBY_FF06_RDp_new, reg_new.reg_tma.NYKE_TMA1p.qn_new());
  /*#p03.NOLA*/ triwire NOLA_TMA2_TO_CD2_new = tri6_pn(TUBY_FF06_RDp_new, reg_new.reg_tma.MURU_TMA2p.qn_new());
  /*#p03.SALU*/ triwire SALU_TMA3_TO_CD3_new = tri6_pn(TUBY_FF06_RDp_new, reg_new.reg_tma.TYVA_TMA3p.qn_new());
  /*#p03.SUPO*/ triwire SUPO_TMA4_TO_CD4_new = tri6_pn(TUBY_FF06_RDp_new, reg_new.reg_tma.TYRU_TMA4p.qn_new());
  /*#p03.SOTU*/ triwire SOTU_TMA5_TO_CD5_new = tri6_pn(TUBY_FF06_RDp_new, reg_new.reg_tma.SUFY_TMA5p.qn_new());
  /*#p03.REVA*/ triwire REVA_TMA6_TO_CD6_new = tri6_pn(TUBY_FF06_RDp_new, reg_new.reg_tma.PETO_TMA6p.qn_new());
  /*#p03.SAPU*/ triwire SAPU_TMA7_TO_CD7_new = tri6_pn(TUBY_FF06_RDp_new, reg_new.reg_tma.SETA_TMA7p.qn_new());

  /*_BUS_CPU_D00p*/ reg_new.cpu_dbus.BUS_CPU_D00p.tri_bus(SETE_TMA0_TO_CD0_new);
  /*_BUS_CPU_D01p*/ reg_new.cpu_dbus.BUS_CPU_D01p.tri_bus(PYRE_TMA1_TO_CD1_new);
  /*_BUS_CPU_D02p*/ reg_new.cpu_dbus.BUS_CPU_D02p.tri_bus(NOLA_TMA2_TO_CD2_new);
  /*_BUS_CPU_D03p*/ reg_new.cpu_dbus.BUS_CPU_D03p.tri_bus(SALU_TMA3_TO_CD3_new);
  /*_BUS_CPU_D04p*/ reg_new.cpu_dbus.BUS_CPU_D04p.tri_bus(SUPO_TMA4_TO_CD4_new);
  /*_BUS_CPU_D05p*/ reg_new.cpu_dbus.BUS_CPU_D05p.tri_bus(SOTU_TMA5_TO_CD5_new);
  /*_BUS_CPU_D06p*/ reg_new.cpu_dbus.BUS_CPU_D06p.tri_bus(REVA_TMA6_TO_CD6_new);
  /*_BUS_CPU_D07p*/ reg_new.cpu_dbus.BUS_CPU_D07p.tri_bus(SAPU_TMA7_TO_CD7_new);

  /*_p03.SORA*/ wire SORA_FF07_RDp_new =  and4(reg_new.cpu_signals.TEDO_CPU_RDp.out_new(), reg_new.cpu_abus.RYFO_FF04_FF07p_new(), reg_new.cpu_abus.BUS_CPU_A01p.out_new(), reg_new.cpu_abus.BUS_CPU_A00p.out_new());
  /*#p03.RYLA*/ triwire RYLA_TAC0_TO_CD0_new = tri6_pn(SORA_FF07_RDp_new, reg_new.reg_tac.SOPU_TAC0p.qn_new());
  /*#p03.ROTE*/ triwire ROTE_TAC1_TO_CD1_new = tri6_pn(SORA_FF07_RDp_new, reg_new.reg_tac.SAMY_TAC1p.qn_new());
  /*#p03.SUPE*/ triwire SUPE_TAC2_TO_CD2_new = tri6_pn(SORA_FF07_RDp_new, reg_new.reg_tac.SABO_TAC2p.qn_new());

  /*_BUS_CPU_D00p*/ reg_new.cpu_dbus.BUS_CPU_D00p.tri_bus(RYLA_TAC0_TO_CD0_new);
  /*_BUS_CPU_D01p*/ reg_new.cpu_dbus.BUS_CPU_D01p.tri_bus(ROTE_TAC1_TO_CD1_new);
  /*_BUS_CPU_D02p*/ reg_new.cpu_dbus.BUS_CPU_D02p.tri_bus(SUPE_TAC2_TO_CD2_new);
}

//-----------------------------------------------------------------------------

void RegDIV::reset() {
  UKUP_DIV00p.state = 0b00011000;
  UFOR_DIV01p.state = 0b00011011;
  UNER_DIV02p.state = 0b00011000;
  TERO_DIV03p.state = 0b00011010;
  UNYK_DIV04p.state = 0b00011011;
  TAMA_DIV05p.state = 0b00011001;
  UGOT_DIV06p.state = 0b00011001;
  TULU_DIV07p.state = 0b00011001;
  TUGO_DIV08p.state = 0b00011000;
  TOFE_DIV09p.state = 0b00011011;
  TERU_DIV10p.state = 0b00011000;
  SOLA_DIV11p.state = 0b00011011;
  SUBU_DIV12p.state = 0b00011000;
  TEKA_DIV13p.state = 0b00011011;
  UKET_DIV14p.state = 0b00011001;
  UPOF_DIV15p.state = 0b00011001;
}

void RegDIV::force_set_div(uint16_t div) {
  uint16_t div_a = div;
  uint16_t div_b = ((~div) << 2);

  UKUP_DIV00p.state = ((div_a >>  0) & 1) | ((div_b >>  0) & 2);
  UFOR_DIV01p.state = ((div_a >>  1) & 1) | ((div_b >>  1) & 2);
  UNER_DIV02p.state = ((div_a >>  2) & 1) | ((div_b >>  2) & 2);
  TERO_DIV03p.state = ((div_a >>  3) & 1) | ((div_b >>  3) & 2);
  UNYK_DIV04p.state = ((div_a >>  4) & 1) | ((div_b >>  4) & 2);
  TAMA_DIV05p.state = ((div_a >>  5) & 1) | ((div_b >>  5) & 2);
  UGOT_DIV06p.state = ((div_a >>  6) & 1) | ((div_b >>  6) & 2);
  TULU_DIV07p.state = ((div_a >>  7) & 1) | ((div_b >>  7) & 2);
  TUGO_DIV08p.state = ((div_a >>  8) & 1) | ((div_b >>  8) & 2);
  TOFE_DIV09p.state = ((div_a >>  9) & 1) | ((div_b >>  9) & 2);
  TERU_DIV10p.state = ((div_a >> 10) & 1) | ((div_b >> 10) & 2);
  SOLA_DIV11p.state = ((div_a >> 11) & 1) | ((div_b >> 11) & 2);
  SUBU_DIV12p.state = ((div_a >> 12) & 1) | ((div_b >> 12) & 2);
  TEKA_DIV13p.state = ((div_a >> 13) & 1) | ((div_b >> 13) & 2);
  UKET_DIV14p.state = ((div_a >> 14) & 1) | ((div_b >> 14) & 2);
  UPOF_DIV15p.state = ((div_a >> 15) & 1) | ((div_b >> 15) & 2);
}

//-----------------------------------------------------------------------------

void RegTIMA::reset() {
  memset(this, BIT_OLD | BIT_DRIVEN, sizeof(*this));
}

void RegTIMA::force_set_tima(uint8_t tima) {
  uint16_t tima_a = tima;
  uint16_t tima_b = ((~tima) << 2);

  REGA_TIMA0p.state = ((tima_a >>  0) & 1) | ((tima_b >>  0) & 2);
  POVY_TIMA1p.state = ((tima_a >>  1) & 1) | ((tima_b >>  1) & 2);
  PERU_TIMA2p.state = ((tima_a >>  2) & 1) | ((tima_b >>  2) & 2);
  RATE_TIMA3p.state = ((tima_a >>  3) & 1) | ((tima_b >>  3) & 2);
  RUBY_TIMA4p.state = ((tima_a >>  4) & 1) | ((tima_b >>  4) & 2);
  RAGE_TIMA5p.state = ((tima_a >>  5) & 1) | ((tima_b >>  5) & 2);
  PEDA_TIMA6p.state = ((tima_a >>  6) & 1) | ((tima_b >>  6) & 2);
  NUGA_TIMA7p.state = ((tima_a >>  7) & 1) | ((tima_b >>  7) & 2);
}

//-----------------------------------------------------------------------------

void RegTMA::reset() {
  memset(this, BIT_OLD | BIT_DRIVEN | BIT_CLOCK, sizeof(*this));
}

//-----------------------------------------------------------------------------

void RegTAC::reset() {
  memset(this, BIT_OLD | BIT_DRIVEN | BIT_CLOCK, sizeof(*this));
}

//-----------------------------------------------------------------------------

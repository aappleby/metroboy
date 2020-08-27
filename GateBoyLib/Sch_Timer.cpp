#include "GateBoyLib/Sch_Timer.h"

#include "GateBoyLib/Sch_Top.h"
#include "GateBoyLib/Probe.h"

using namespace Schematics;

void Timer::dump(Dumper& d) const {
  d("----------  Timer   ----------\n");
  d("DIV        0x%04x %d\n", get_div(), get_div());
  d("TIMA       0x%02x %d\n", get_tima(), get_tima());
  d("TMA        0x%02x %d\n", get_tma(), get_tma());
  d("TAC        0x%02x %d\n", get_tac(), get_tac());
  d("TIMA_MAX   %c\n", NYDU_TIMA_D7_DELAY.c());
  d("INT_TIMERp %c\n", MOBA_INT_TIMER_TRIGp.c());
  d("\n");
}


//------------------------------------------------------------------------------

void Timer::tick(const SchematicTop& /*top*/) {
}

void Timer::tock(wire RST, const SchematicTop& top, CpuBus& cpu_bus) {
  /*p03.MULO*/ wire MULO_SYS_RSTn = not1(top.clk_reg.ALUR_SYS_RSTn);
  /*p01.BOGA*/ wire BOGA_xBCDEFGH = not1(top.clk_reg.BALY_Axxxxxxx);
  /*p03.RYFO*/ wire RYFO_FF04_FF07p = and3(top.cpu_bus.CPU_BUS_A02.tp(), top.cpu_bus.SARE_XX00_XX07p(), top.cpu_bus.SYKE_FF00_FFFFp());

  /*p01.UVYN*/ wire UVYN_DIV_05n = not1(TAMA_DIV_05.qp());
  /*p01.UTOK*/ wire UMEK_DIV_06n = not1(UGOT_DIV_06.qp());
  /*p01.SAPY*/ wire UREK_DIV_07n = not1(TULU_DIV_07.qp());
  /*p01.UTOK*/ wire UTOK_DIV_08n = not1(TUGO_DIV_08.qp());
  /*p01.SAPY*/ wire SAPY_DIV_09n = not1(TOFE_DIV_09.qp());
  /*p01.UMER*/ wire UMER_DIV_10n = not1(TERU_DIV_10.qp());
  /*p01.RAVE*/ wire RAVE_DIV_11n = not1(SOLA_DIV_11.qp());
  /*p01.RYSO*/ wire RYSO_DIV_12n = not1(SUBU_DIV_12.qp());
  /*p01.UDOR*/ wire UDOR_DIV_13n = not1(TEKA_DIV_13.qp());

  // FF04 DIV
  {
    /*p01.TAGY*/ wire TAGY_FF04_RDp = and4(top.TEDO_CPU_RDp(), RYFO_FF04_FF07p, top.cpu_bus.TOLA_A01n(), top.cpu_bus.TOVY_A00n());
    /*p01.TAPE*/ wire TAPE_FF04_WRp = and4(top.TAPU_CPU_WRp_xxxxEFGx, RYFO_FF04_FF07p, top.cpu_bus.TOLA_A01n(), top.cpu_bus.TOVY_A00n());
    /*p01.UFOL*/ wire UFOL_DIV_RSTn = nor3(top.clk_reg.UCOB_CLKBADp, RST, TAPE_FF04_WRp);

    /*p01.UKUP*/ UKUP_DIV_00 = dff17_AB(BOGA_xBCDEFGH,    UFOL_DIV_RSTn, UKUP_DIV_00.qn());
    /*p01.UFOR*/ UFOR_DIV_01 = dff17_AB(UKUP_DIV_00.qn(), UFOL_DIV_RSTn, UFOR_DIV_01.qn());
    /*p01.UNER*/ UNER_DIV_02 = dff17_AB(UFOR_DIV_01.qn(), UFOL_DIV_RSTn, UNER_DIV_02.qn());
    /*p01.TERO*/ TERO_DIV_03 = dff17_AB(UNER_DIV_02.qn(), UFOL_DIV_RSTn, TERO_DIV_03.qn());
    /*p01.UNYK*/ UNYK_DIV_04 = dff17_AB(TERO_DIV_03.qn(), UFOL_DIV_RSTn, UNYK_DIV_04.qn());
    /*p01.TAMA*/ TAMA_DIV_05 = dff17_AB(UNYK_DIV_04.qn(), UFOL_DIV_RSTn, TAMA_DIV_05.qn());

    ///*p01.ULUR*/ wire ULUR_DIV_06_CLK = mux2p(BOGA_AxCDEFGH, DIV_05, FF60_1);
    /*p01.ULUR*/ wire ULUR_DIV_06_CLK = TAMA_DIV_05.qn();
    /*p01.UGOT*/ UGOT_DIV_06 = dff17_AB(ULUR_DIV_06_CLK,  UFOL_DIV_RSTn, UGOT_DIV_06.qn());
    /*p01.TULU*/ TULU_DIV_07 = dff17_AB(UGOT_DIV_06.qn(), UFOL_DIV_RSTn, TULU_DIV_07.qn());
    /*p01.TUGO*/ TUGO_DIV_08 = dff17_AB(TULU_DIV_07.qn(), UFOL_DIV_RSTn, TUGO_DIV_08.qn());
    /*p01.TOFE*/ TOFE_DIV_09 = dff17_AB(TUGO_DIV_08.qn(), UFOL_DIV_RSTn, TOFE_DIV_09.qn());
    /*p01.TERU*/ TERU_DIV_10 = dff17_AB(TOFE_DIV_09.qn(), UFOL_DIV_RSTn, TERU_DIV_10.qn());
    /*p01.SOLA*/ SOLA_DIV_11 = dff17_AB(TERU_DIV_10.qn(), UFOL_DIV_RSTn, SOLA_DIV_11.qn());
    /*p01.SUBU*/ SUBU_DIV_12 = dff17_AB(SOLA_DIV_11.qn(), UFOL_DIV_RSTn, SUBU_DIV_12.qn());
    /*p01.TEKA*/ TEKA_DIV_13 = dff17_AB(SUBU_DIV_12.qn(), UFOL_DIV_RSTn, TEKA_DIV_13.qn());
    /*p01.UKET*/ UKET_DIV_14 = dff17_AB(TEKA_DIV_13.qn(), UFOL_DIV_RSTn, UKET_DIV_14.qn());
    /*p01.UPOF*/ UPOF_DIV_15 = dff17_AB(UKET_DIV_14.qn(), UFOL_DIV_RSTn, UPOF_DIV_15.qn());

    /*p01.TAWU*/ cpu_bus.CPU_BUS_D0p = tribuf_6pn(TAGY_FF04_RDp, UMEK_DIV_06n);
    /*p01.TAKU*/ cpu_bus.CPU_BUS_D1p = tribuf_6pn(TAGY_FF04_RDp, UREK_DIV_07n);
    /*p01.TEMU*/ cpu_bus.CPU_BUS_D2p = tribuf_6pn(TAGY_FF04_RDp, UTOK_DIV_08n);
    /*p01.TUSE*/ cpu_bus.CPU_BUS_D3p = tribuf_6pn(TAGY_FF04_RDp, SAPY_DIV_09n);
    /*p01.UPUG*/ cpu_bus.CPU_BUS_D4p = tribuf_6pn(TAGY_FF04_RDp, UMER_DIV_10n);
    /*p01.SEPU*/ cpu_bus.CPU_BUS_D5p = tribuf_6pn(TAGY_FF04_RDp, RAVE_DIV_11n);
    /*p01.SAWA*/ cpu_bus.CPU_BUS_D6p = tribuf_6pn(TAGY_FF04_RDp, RYSO_DIV_12n);
    /*p01.TATU*/ cpu_bus.CPU_BUS_D7p = tribuf_6pn(TAGY_FF04_RDp, UDOR_DIV_13n);
  }

  // FF05 TIMA
  {
    /*p03.TEDA*/ wire _TEDA_FF05_RDp = and4(top.TEDO_CPU_RDp(), RYFO_FF04_FF07p, top.cpu_bus.TOLA_A01n(), top.cpu_bus.CPU_BUS_A00.tp());
    /*p03.TOPE*/ wire _TOPE_FF05_WRn = nand4(top.TAPU_CPU_WRp_xxxxEFGx, RYFO_FF04_FF07p, top.cpu_bus.TOLA_A01n(), top.cpu_bus.CPU_BUS_A00.tp());

    /*p03.UBOT*/ wire _UBOT_CLK_256Kn = not1(UFOR_DIV_01.qp());
    /*p03.UVYR*/ wire _UVYR_CLK_64Kn = not1(TERO_DIV_03.qp());

    /*p03.UKAP*/ wire _UKAP_CLK_MUXa = mux2_n(UVYN_DIV_05n, _UVYR_CLK_64Kn, SOPU_TAC_0.qp());
    /*p03.TEKO*/ wire _TEKO_CLK_MUXb = mux2_n(_UBOT_CLK_256Kn, UREK_DIV_07n, SOPU_TAC_0.qp());
    /*p03.TECY*/ wire _TECY_CLK_MUXc = mux2_n(_UKAP_CLK_MUXa, _TEKO_CLK_MUXb, SAMY_TAC_1.qp());

    /*p03.SOGU*/ wire _SOGU_TIMA_CLK = nor2(_TECY_CLK_MUXc, SABO_TAC_2.qn());

    /*p03.ROKE*/ wire _ROKE_TIMA_MUX_0 = mux2_n(SABU_TMA_0.qp(), top.cpu_bus.CPU_BUS_D0p.tp(), _TOPE_FF05_WRn);
    /*p03.PETU*/ wire _PETU_TIMA_MUX_1 = mux2_n(NYKE_TMA_1.qp(), top.cpu_bus.CPU_BUS_D1p.tp(), _TOPE_FF05_WRn);
    /*p03.NYKU*/ wire _NYKU_TIMA_MUX_2 = mux2_n(MURU_TMA_2.qp(), top.cpu_bus.CPU_BUS_D2p.tp(), _TOPE_FF05_WRn); // die annotation box wrong, this is a 5-rung
    /*p03.SOCE*/ wire _SOCE_TIMA_MUX_3 = mux2_n(TYVA_TMA_3.qp(), top.cpu_bus.CPU_BUS_D3p.tp(), _TOPE_FF05_WRn);
    /*p03.SALA*/ wire _SALA_TIMA_MUX_4 = mux2_n(TYRU_TMA_4.qp(), top.cpu_bus.CPU_BUS_D4p.tp(), _TOPE_FF05_WRn);
    /*p03.SYRU*/ wire _SYRU_TIMA_MUX_5 = mux2_n(SUFY_TMA_5.qp(), top.cpu_bus.CPU_BUS_D5p.tp(), _TOPE_FF05_WRn);
    /*p03.REFU*/ wire _REFU_TIMA_MUX_6 = mux2_n(PETO_TMA_6.qp(), top.cpu_bus.CPU_BUS_D6p.tp(), _TOPE_FF05_WRn);
    /*p03.RATO*/ wire _RATO_TIMA_MUX_7 = mux2_n(SETA_TMA_7.qp(), top.cpu_bus.CPU_BUS_D7p.tp(), _TOPE_FF05_WRn);

    /*p03.PUXY*/ wire _PUXY_TIMA_LD_0 = nor2(MULO_SYS_RSTn, _ROKE_TIMA_MUX_0);
    /*p03.NERO*/ wire _NERO_TIMA_LD_1 = nor2(MULO_SYS_RSTn, _PETU_TIMA_MUX_1);
    /*p03.NADA*/ wire _NADA_TIMA_LD_2 = nor2(MULO_SYS_RSTn, _NYKU_TIMA_MUX_2);
    /*p03.REPA*/ wire _REPA_TIMA_LD_3 = nor2(MULO_SYS_RSTn, _SOCE_TIMA_MUX_3);
    /*p03.ROLU*/ wire _ROLU_TIMA_LD_4 = nor2(MULO_SYS_RSTn, _SALA_TIMA_MUX_4);
    /*p03.RUGY*/ wire _RUGY_TIMA_LD_5 = nor2(MULO_SYS_RSTn, _SYRU_TIMA_MUX_5);
    /*p03.PYMA*/ wire _PYMA_TIMA_LD_6 = nor2(MULO_SYS_RSTn, _REFU_TIMA_MUX_6);
    /*p03.PAGU*/ wire _PAGU_TIMA_LD_7 = nor2(MULO_SYS_RSTn, _RATO_TIMA_MUX_7);

    /*p03.MERY*/ wire MERY_TIMER_INT_TRIGp = nor2(NYDU_TIMA_D7_DELAY.qn(), NUGA_TIMA_D7.qp());
    /*p03.MOBA*/ MOBA_INT_TIMER_TRIGp = dff17_B(BOGA_xBCDEFGH, top.clk_reg.ALUR_SYS_RSTn, MERY_TIMER_INT_TRIGp);
    /*p03.MEKE*/ wire _MEKE_INT_TIMERn = not1(MOBA_INT_TIMER_TRIGp.qp());
    /*p03.MUZU*/ wire _MUZU_TIMA_LOADn = or2(top.cpu_bus.CPU_PIN_LATCH_EXT.tp(), _TOPE_FF05_WRn);
    /*p03.MEXU*/ wire _MEXU_TIMA_LOADp = nand3(_MUZU_TIMA_LOADn, top.clk_reg.ALUR_SYS_RSTn, _MEKE_INT_TIMERn);

    /*p03.REGA*/ REGA_TIMA_D0 = dff20(_SOGU_TIMA_CLK,    _MEXU_TIMA_LOADp, _PUXY_TIMA_LD_0, REGA_TIMA_D0.qn());
    /*p03.POVY*/ POVY_TIMA_D1 = dff20(REGA_TIMA_D0.qn(), _MEXU_TIMA_LOADp, _NERO_TIMA_LD_1, POVY_TIMA_D1.qn());
    /*p03.PERU*/ PERU_TIMA_D2 = dff20(POVY_TIMA_D1.qn(), _MEXU_TIMA_LOADp, _NADA_TIMA_LD_2, PERU_TIMA_D2.qn());
    /*p03.RATE*/ RATE_TIMA_D3 = dff20(PERU_TIMA_D2.qn(), _MEXU_TIMA_LOADp, _REPA_TIMA_LD_3, RATE_TIMA_D3.qn());
    /*p03.RUBY*/ RUBY_TIMA_D4 = dff20(RATE_TIMA_D3.qn(), _MEXU_TIMA_LOADp, _ROLU_TIMA_LD_4, RUBY_TIMA_D4.qn());
    /*p03.RAGE*/ RAGE_TIMA_D5 = dff20(RUBY_TIMA_D4.qn(), _MEXU_TIMA_LOADp, _RUGY_TIMA_LD_5, RAGE_TIMA_D5.qn());
    /*p03.PEDA*/ PEDA_TIMA_D6 = dff20(RAGE_TIMA_D5.qn(), _MEXU_TIMA_LOADp, _PYMA_TIMA_LD_6, PEDA_TIMA_D6.qn());
    /*p03.NUGA*/ NUGA_TIMA_D7 = dff20(PEDA_TIMA_D6.qn(), _MEXU_TIMA_LOADp, _PAGU_TIMA_LD_7, NUGA_TIMA_D7.qn());

    /*p03.MUGY*/ wire _MUGY_TIMA_MAX_RSTn = not1(_MEXU_TIMA_LOADp);
    /*p03.NYDU*/ NYDU_TIMA_D7_DELAY = dff17_A(BOGA_xBCDEFGH, _MUGY_TIMA_MAX_RSTn, NUGA_TIMA_D7.qp());

    // FIXME inversion
    /*p03.SOKU*/ cpu_bus.CPU_BUS_D0p = tribuf_6pn(_TEDA_FF05_RDp, !REGA_TIMA_D0.qp());
    /*p03.RACY*/ cpu_bus.CPU_BUS_D1p = tribuf_6pn(_TEDA_FF05_RDp, !POVY_TIMA_D1.qp());
    /*p03.RAVY*/ cpu_bus.CPU_BUS_D2p = tribuf_6pn(_TEDA_FF05_RDp, !PERU_TIMA_D2.qp());
    /*p03.SOSY*/ cpu_bus.CPU_BUS_D3p = tribuf_6pn(_TEDA_FF05_RDp, !RATE_TIMA_D3.qp());
    /*p03.SOMU*/ cpu_bus.CPU_BUS_D4p = tribuf_6pn(_TEDA_FF05_RDp, !RUBY_TIMA_D4.qp());
    /*p03.SURO*/ cpu_bus.CPU_BUS_D5p = tribuf_6pn(_TEDA_FF05_RDp, !RAGE_TIMA_D5.qp());
    /*p03.ROWU*/ cpu_bus.CPU_BUS_D6p = tribuf_6pn(_TEDA_FF05_RDp, !PEDA_TIMA_D6.qp());
    /*p03.PUSO*/ cpu_bus.CPU_BUS_D7p = tribuf_6pn(_TEDA_FF05_RDp, !NUGA_TIMA_D7.qp());
  }

  // FF06 TMA
  {
    /*p03.TUBY*/ wire _TUBY_FF06_RDp = and4(top.TEDO_CPU_RDp(), RYFO_FF04_FF07p, top.cpu_bus.CPU_BUS_A01.tp(), top.cpu_bus.TOVY_A00n());
    /*p03.TYJU*/ wire _TYJU_FF06_WRn = nand4(top.TAPU_CPU_WRp_xxxxEFGx, RYFO_FF04_FF07p, top.cpu_bus.CPU_BUS_A01.tp(), top.cpu_bus.TOVY_A00n());

    /*p03.SABU*/ SABU_TMA_0 = dff17_AB(_TYJU_FF06_WRn, top.clk_reg.ALUR_SYS_RSTn, top.cpu_bus.CPU_BUS_D0p.tp());
    /*p03.NYKE*/ NYKE_TMA_1 = dff17_AB(_TYJU_FF06_WRn, top.clk_reg.ALUR_SYS_RSTn, top.cpu_bus.CPU_BUS_D1p.tp());
    /*p03.MURU*/ MURU_TMA_2 = dff17_AB(_TYJU_FF06_WRn, top.clk_reg.ALUR_SYS_RSTn, top.cpu_bus.CPU_BUS_D2p.tp());
    /*p03.TYVA*/ TYVA_TMA_3 = dff17_AB(_TYJU_FF06_WRn, top.clk_reg.ALUR_SYS_RSTn, top.cpu_bus.CPU_BUS_D3p.tp());
    /*p03.TYRU*/ TYRU_TMA_4 = dff17_AB(_TYJU_FF06_WRn, top.clk_reg.ALUR_SYS_RSTn, top.cpu_bus.CPU_BUS_D4p.tp());
    /*p03.SUFY*/ SUFY_TMA_5 = dff17_AB(_TYJU_FF06_WRn, top.clk_reg.ALUR_SYS_RSTn, top.cpu_bus.CPU_BUS_D5p.tp());
    /*p03.PETO*/ PETO_TMA_6 = dff17_AB(_TYJU_FF06_WRn, top.clk_reg.ALUR_SYS_RSTn, top.cpu_bus.CPU_BUS_D6p.tp());
    /*p03.SETA*/ SETA_TMA_7 = dff17_AB(_TYJU_FF06_WRn, top.clk_reg.ALUR_SYS_RSTn, top.cpu_bus.CPU_BUS_D7p.tp());

    /*#p03.SETE*/ cpu_bus.CPU_BUS_D0p = tribuf_6pn(_TUBY_FF06_RDp, SABU_TMA_0.qn());
    /*#p03.PYRE*/ cpu_bus.CPU_BUS_D1p = tribuf_6pn(_TUBY_FF06_RDp, NYKE_TMA_1.qn());
    /*#p03.NOLA*/ cpu_bus.CPU_BUS_D2p = tribuf_6pn(_TUBY_FF06_RDp, MURU_TMA_2.qn());
    /*#p03.SALU*/ cpu_bus.CPU_BUS_D3p = tribuf_6pn(_TUBY_FF06_RDp, TYVA_TMA_3.qn());
    /*#p03.SUPO*/ cpu_bus.CPU_BUS_D4p = tribuf_6pn(_TUBY_FF06_RDp, TYRU_TMA_4.qn());
    /*#p03.SOTU*/ cpu_bus.CPU_BUS_D5p = tribuf_6pn(_TUBY_FF06_RDp, SUFY_TMA_5.qn());
    /*#p03.REVA*/ cpu_bus.CPU_BUS_D6p = tribuf_6pn(_TUBY_FF06_RDp, PETO_TMA_6.qn());
    /*#p03.SAPU*/ cpu_bus.CPU_BUS_D7p = tribuf_6pn(_TUBY_FF06_RDp, SETA_TMA_7.qn());
  }

  // FF07 TAC
  {
    /*p03.SORA*/ wire _SORA_FF07_RDp = and4(top.TEDO_CPU_RDp(), RYFO_FF04_FF07p, top.cpu_bus.CPU_BUS_A01.tp(), top.cpu_bus.CPU_BUS_A00.tp());
    /*p03.SARA*/ wire _SARA_FF07_WRn = nand4(top.TAPU_CPU_WRp_xxxxEFGx, RYFO_FF04_FF07p, top.cpu_bus.CPU_BUS_A01.tp(), top.cpu_bus.CPU_BUS_A00.tp());

    /*p03.SOPU*/ SOPU_TAC_0 = dff17_AB(_SARA_FF07_WRn, top.clk_reg.ALUR_SYS_RSTn, top.cpu_bus.CPU_BUS_D0p.tp());
    /*p03.SAMY*/ SAMY_TAC_1 = dff17_AB(_SARA_FF07_WRn, top.clk_reg.ALUR_SYS_RSTn, top.cpu_bus.CPU_BUS_D1p.tp());
    /*p03.SABO*/ SABO_TAC_2 = dff17_AB(_SARA_FF07_WRn, top.clk_reg.ALUR_SYS_RSTn, top.cpu_bus.CPU_BUS_D2p.tp());

    /*#p03.RYLA*/ cpu_bus.CPU_BUS_D0p = tribuf_6pn(_SORA_FF07_RDp, SOPU_TAC_0.qn());
    /*#p03.ROTE*/ cpu_bus.CPU_BUS_D1p = tribuf_6pn(_SORA_FF07_RDp, SAMY_TAC_1.qn());
    /*#p03.SUPE*/ cpu_bus.CPU_BUS_D2p = tribuf_6pn(_SORA_FF07_RDp, SABO_TAC_2.qn());
  }
}

//------------------------------------------------------------------------------

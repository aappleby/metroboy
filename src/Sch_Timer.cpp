#include "Sch_Timer.h"

#include "Sch_Top.h"

using namespace Schematics;

//------------------------------------------------------------------------------

void Timer::tick(const SchematicTop& /*top*/) {
}

void Timer::tock(const SchematicTop& top, CpuBus& cpu_bus) {
  /*p03.RYFO*/ wire _RYFO_FF04_FF07p = and3(top.cpu_bus.CPU_BUS_A02.qp(), top.cpu_bus.SARE_XX00_XX07p(), top.cpu_bus.SYKE_FF00_FFFFp());

  // FF04 DIV
  {
    /*p01.TAGY*/ wire _TAGY_FF04_RDp = and4(top.TEDO_CPU_RDp(), _RYFO_FF04_FF07p, top.cpu_bus.TOLA_A01n(), top.cpu_bus.TOVY_A00n());
    /*p01.TAPE*/ wire _TAPE_FF04_WRp = and4(top.TAPU_CPU_WRp_xxxDxxxx(), _RYFO_FF04_FF07p, top.cpu_bus.TOLA_A01n(), top.cpu_bus.TOVY_A00n());

    /*p01.UFOL*/ wire _UFOL_DIV_RSTn = nor3(top.clk_reg.UCOB_CLKBADp(), top.clk_reg.SYS_PIN_RSTp(), _TAPE_FF04_WRp);

    /*p01.UKUP*/ _UKUP_DIV_00 = dff17_A (top.clk_reg.BOGA_ABCDExxx(), _UFOL_DIV_RSTn, _UKUP_DIV_00.qn());
    /*p01.UFOR*/ _UFOR_DIV_01 = dff17_AB(_UKUP_DIV_00.qn(),           _UFOL_DIV_RSTn, _UFOR_DIV_01.qn());
    /*p01.UNER*/ _UNER_DIV_02 = dff17_A (_UFOR_DIV_01.qn(),           _UFOL_DIV_RSTn, _UNER_DIV_02.qn());
    /*p01.TERO*/ _TERO_DIV_03 = dff17_AB(_UNER_DIV_02.qn(),           _UFOL_DIV_RSTn, _TERO_DIV_03.qn());
    /*p01.UNYK*/ _UNYK_DIV_04 = dff17_A (_TERO_DIV_03.qn(),           _UFOL_DIV_RSTn, _UNYK_DIV_04.qn());
    /*p01.TAMA*/ _TAMA_DIV_05 = dff17_AB(_UNYK_DIV_04.qn(),           _UFOL_DIV_RSTn, _TAMA_DIV_05.qn());

    ///*p01.ULUR*/ wire DIV_06_CLK = mux2p(BOGA_AxCDEFGH, DIV_05, FF60_1);
    /*p01.ULUR*/ wire _DIV_06_CLK = _TAMA_DIV_05.qn();
    /*p01.UGOT*/ _UGOT_DIV_06 = dff17_AB(_DIV_06_CLK,      _UFOL_DIV_RSTn,  _UGOT_DIV_06.qn());
    /*p01.TULU*/ _TULU_DIV_07 = dff17_AB(_UGOT_DIV_06.qn(), _UFOL_DIV_RSTn, _TULU_DIV_07.qn());
    /*p01.TUGO*/ _TUGO_DIV_08 = dff17_AB(_TULU_DIV_07.qn(), _UFOL_DIV_RSTn, _TUGO_DIV_08.qn());
    /*p01.TOFE*/ _TOFE_DIV_09 = dff17_AB(_TUGO_DIV_08.qn(), _UFOL_DIV_RSTn, _TOFE_DIV_09.qn());
    /*p01.TERU*/ _TERU_DIV_10 = dff17_AB(_TOFE_DIV_09.qn(), _UFOL_DIV_RSTn, _TERU_DIV_10.qn());
    /*p01.SOLA*/ _SOLA_DIV_11 = dff17_AB(_TERU_DIV_10.qn(), _UFOL_DIV_RSTn, _SOLA_DIV_11.qn());
    /*p01.SUBU*/ _SUBU_DIV_12 = dff17_AB(_SOLA_DIV_11.qn(), _UFOL_DIV_RSTn, _SUBU_DIV_12.qn());
    /*p01.TEKA*/ _TEKA_DIV_13 = dff17_AB(_SUBU_DIV_12.qn(), _UFOL_DIV_RSTn, _TEKA_DIV_13.qn());
    /*p01.UKET*/ _UKET_DIV_14 = dff17_A (_TEKA_DIV_13.qn(), _UFOL_DIV_RSTn, _UKET_DIV_14.qn());
    /*p01.UPOF*/ _UPOF_DIV_15 = dff17_AB(_UKET_DIV_14.qn(), _UFOL_DIV_RSTn, _UPOF_DIV_15.qn());

    /*p01.UTOK*/ wire _UMEK_DIV_06n = not1(_UGOT_DIV_06.qp());
    /*p01.SAPY*/ wire _UREK_DIV_07n = not1(_TULU_DIV_07.qp());
    /*p01.UTOK*/ wire _UTOK_DIV_08n = not1(_TUGO_DIV_08.qp());
    /*p01.SAPY*/ wire _SAPY_DIV_09n = not1(_TOFE_DIV_09.qp());
    /*p01.UMER*/ wire _UMER_DIV_10n = not1(_TERU_DIV_10.qp());
    /*p01.RAVE*/ wire _RAVE_DIV_11n = not1(_SOLA_DIV_11.qp());
    /*p01.RYSO*/ wire _RYSO_DIV_12n = not1(_SUBU_DIV_12.qp());
    /*p01.UDOR*/ wire _UDOR_DIV_13n = not1(_TEKA_DIV_13.qp());
    /*p01.TAWU*/ cpu_bus.CPU_BUS_D0 = tribuf_6p(_TAGY_FF04_RDp, not1(_UMEK_DIV_06n));
    /*p01.TAKU*/ cpu_bus.CPU_BUS_D1 = tribuf_6p(_TAGY_FF04_RDp, not1(_UREK_DIV_07n));
    /*p01.TEMU*/ cpu_bus.CPU_BUS_D2 = tribuf_6p(_TAGY_FF04_RDp, not1(_UTOK_DIV_08n));
    /*p01.TUSE*/ cpu_bus.CPU_BUS_D3 = tribuf_6p(_TAGY_FF04_RDp, not1(_SAPY_DIV_09n));
    /*p01.UPUG*/ cpu_bus.CPU_BUS_D4 = tribuf_6p(_TAGY_FF04_RDp, not1(_UMER_DIV_10n));
    /*p01.SEPU*/ cpu_bus.CPU_BUS_D5 = tribuf_6p(_TAGY_FF04_RDp, not1(_RAVE_DIV_11n));
    /*p01.SAWA*/ cpu_bus.CPU_BUS_D6 = tribuf_6p(_TAGY_FF04_RDp, not1(_RYSO_DIV_12n));
    /*p01.TATU*/ cpu_bus.CPU_BUS_D7 = tribuf_6p(_TAGY_FF04_RDp, not1(_UDOR_DIV_13n));
  }

  // FF05 TIMA
  {
    /*p03.TEDA*/ wire _TEDA_FF05_RDp = and4(top.TEDO_CPU_RDp(), _RYFO_FF04_FF07p, top.cpu_bus.TOLA_A01n(), top.cpu_bus.CPU_BUS_A00.qp());
    /*p03.TOPE*/ wire _TOPE_FF05_WRn = nand4(top.TAPU_CPU_WRp_xxxDxxxx(), _RYFO_FF04_FF07p, top.cpu_bus.TOLA_A01n(), top.cpu_bus.CPU_BUS_A00.qp());

    /*p03.MEKE*/ wire _MEKE_INT_TIMERn = not1(MOBA_INT_TIMERp.qp());
    /*p03.UBOT*/ wire _UBOT_CLK_256Kn = not1(_UFOR_DIV_01.qp());
    /*p03.UVYR*/ wire _UVYR_CLK_64Kn = not1(_TERO_DIV_03.qp());

    /*p03.UKAP*/ wire _UKAP_CLK_MUXa = mux2_n(UVYN_DIV_05n(), _UVYR_CLK_64Kn, SOPU_TAC_0.qp());
    /*p03.TEKO*/ wire _TEKO_CLK_MUXb = mux2_n(_UBOT_CLK_256Kn, UREK_DIV_07n(), SOPU_TAC_0.qp());
    /*p03.TECY*/ wire _TECY_CLK_MUXc = mux2_n(_UKAP_CLK_MUXa, _TEKO_CLK_MUXb, SAMY_TAC_1.qp());

    /*p03.SOGU*/ wire _SOGU_TIMA_CLK = nor2(_TECY_CLK_MUXc, SABO_TAC_2.qn());
    /*p03.ROKE*/ wire _ROKE_TIMA_MUX_0 = mux2_n(SABU_TMA_0.qp(), top.cpu_bus.CPU_BUS_D0.qp(), _TOPE_FF05_WRn); // suggests WRn
    /*p03.PETU*/ wire _PETU_TIMA_MUX_1 = mux2_n(NYKE_TMA_1.qp(), top.cpu_bus.CPU_BUS_D1.qp(), _TOPE_FF05_WRn);
    /*p03.NYKU*/ wire _NYKU_TIMA_MUX_2 = mux2_n(MURU_TMA_2.qp(), top.cpu_bus.CPU_BUS_D2.qp(), _TOPE_FF05_WRn); // die annotation box wrong, this is a 5-rung
    /*p03.SOCE*/ wire _SOCE_TIMA_MUX_3 = mux2_n(TYVA_TMA_3.qp(), top.cpu_bus.CPU_BUS_D3.qp(), _TOPE_FF05_WRn);
    /*p03.SALA*/ wire _SALA_TIMA_MUX_4 = mux2_n(TYRU_TMA_4.qp(), top.cpu_bus.CPU_BUS_D4.qp(), _TOPE_FF05_WRn);
    /*p03.SYRU*/ wire _SYRU_TIMA_MUX_5 = mux2_n(SUFY_TMA_5.qp(), top.cpu_bus.CPU_BUS_D5.qp(), _TOPE_FF05_WRn);
    /*p03.REFU*/ wire _REFU_TIMA_MUX_6 = mux2_n(PETO_TMA_6.qp(), top.cpu_bus.CPU_BUS_D6.qp(), _TOPE_FF05_WRn);
    /*p03.RATO*/ wire _RATO_TIMA_MUX_7 = mux2_n(SETA_TMA_7.qp(), top.cpu_bus.CPU_BUS_D7.qp(), _TOPE_FF05_WRn);

    /*p03.PUXY*/ wire _PUXY_TIMA_LD_0 = nor2(top.clk_reg.MULO_SYS_RSTn(), _ROKE_TIMA_MUX_0);
    /*p03.NERO*/ wire _NERO_TIMA_LD_1 = nor2(top.clk_reg.MULO_SYS_RSTn(), _PETU_TIMA_MUX_1);
    /*p03.NADA*/ wire _NADA_TIMA_LD_2 = nor2(top.clk_reg.MULO_SYS_RSTn(), _NYKU_TIMA_MUX_2);
    /*p03.REPA*/ wire _REPA_TIMA_LD_3 = nor2(top.clk_reg.MULO_SYS_RSTn(), _SOCE_TIMA_MUX_3);
    /*p03.ROLU*/ wire _ROLU_TIMA_LD_4 = nor2(top.clk_reg.MULO_SYS_RSTn(), _SALA_TIMA_MUX_4);
    /*p03.RUGY*/ wire _RUGY_TIMA_LD_5 = nor2(top.clk_reg.MULO_SYS_RSTn(), _SYRU_TIMA_MUX_5);
    /*p03.PYMA*/ wire _PYMA_TIMA_LD_6 = nor2(top.clk_reg.MULO_SYS_RSTn(), _REFU_TIMA_MUX_6);
    /*p03.PAGU*/ wire _PAGU_TIMA_LD_7 = nor2(top.clk_reg.MULO_SYS_RSTn(), _RATO_TIMA_MUX_7);

#if 0

    TIMA_LOADp = or2(and(!CPU_PIN_HOLD_MEM, CPU_PIN_WRp, xxxxEFGx, FF05), !ALUR_SYS_RSTn, !MEKE_INT_TIMERn);

    MUZU := or2
    MUZU_01 << CPU_PIN_HOLD_MEM
    MUZU_02 << TOPE
    MUZU_03 nc 
    MUZU_04 >> 


#endif

    /*p03.MUZU*/ wire _MUZU_TIMA_LOADn = or2(top.cpu_bus.CPU_PIN_HOLD_MEM.qp(), _TOPE_FF05_WRn);
    /*p03.MEXU*/ wire _MEXU_TIMA_LOADp = nand3(_MUZU_TIMA_LOADn, top.clk_reg.ALUR_SYS_RSTn(), _MEKE_INT_TIMERn);

    /*p03.REGA*/ REGA_TIMA_0 = ff20(_SOGU_TIMA_CLK,   _MEXU_TIMA_LOADp, _PUXY_TIMA_LD_0);
    /*p03.POVY*/ POVY_TIMA_1 = ff20(REGA_TIMA_0.qn(), _MEXU_TIMA_LOADp, _NERO_TIMA_LD_1);
    /*p03.PERU*/ PERU_TIMA_2 = ff20(POVY_TIMA_1.qn(), _MEXU_TIMA_LOADp, _NADA_TIMA_LD_2);
    /*p03.RATE*/ RATE_TIMA_3 = ff20(PERU_TIMA_2.qn(), _MEXU_TIMA_LOADp, _REPA_TIMA_LD_3);
    /*p03.RUBY*/ RUBY_TIMA_4 = ff20(RATE_TIMA_3.qn(), _MEXU_TIMA_LOADp, _ROLU_TIMA_LD_4);
    /*p03.RAGE*/ RAGE_TIMA_5 = ff20(RUBY_TIMA_4.qn(), _MEXU_TIMA_LOADp, _RUGY_TIMA_LD_5);
    /*p03.PEDA*/ PEDA_TIMA_6 = ff20(RAGE_TIMA_5.qn(), _MEXU_TIMA_LOADp, _PYMA_TIMA_LD_6);
    /*p03.NUGA*/ NUGA_TIMA_7 = ff20(PEDA_TIMA_6.qn(), _MEXU_TIMA_LOADp, _PAGU_TIMA_LD_7);

    /*p03.SOKU*/ cpu_bus.CPU_BUS_D0 = tribuf_6p(_TEDA_FF05_RDp, REGA_TIMA_0.qp());
    /*p03.RACY*/ cpu_bus.CPU_BUS_D1 = tribuf_6p(_TEDA_FF05_RDp, POVY_TIMA_1.qp());
    /*p03.RAVY*/ cpu_bus.CPU_BUS_D2 = tribuf_6p(_TEDA_FF05_RDp, PERU_TIMA_2.qp());
    /*p03.SOSY*/ cpu_bus.CPU_BUS_D3 = tribuf_6p(_TEDA_FF05_RDp, RATE_TIMA_3.qp());
    /*p03.SOMU*/ cpu_bus.CPU_BUS_D4 = tribuf_6p(_TEDA_FF05_RDp, RUBY_TIMA_4.qp());
    /*p03.SURO*/ cpu_bus.CPU_BUS_D5 = tribuf_6p(_TEDA_FF05_RDp, RAGE_TIMA_5.qp());
    /*p03.ROWU*/ cpu_bus.CPU_BUS_D6 = tribuf_6p(_TEDA_FF05_RDp, PEDA_TIMA_6.qp());
    /*p03.PUSO*/ cpu_bus.CPU_BUS_D7 = tribuf_6p(_TEDA_FF05_RDp, NUGA_TIMA_7.qp());

    /*p03.MUGY*/ wire _MUGY_TIMA_MAX_RSTn = not1(_MEXU_TIMA_LOADp);

    /*p03.NYDU*/ NYDU_TIMA_MAX = dff17_A(top.clk_reg.BOGA_ABCDExxx(), _MUGY_TIMA_MAX_RSTn, NUGA_TIMA_7.qp());
  }

  {
    /*p03.MERY*/ wire _MERY_INT_TIMER_IN = nor2(NYDU_TIMA_MAX.qn(), NUGA_TIMA_7.qp());

    /*p03.MOBA*/ MOBA_INT_TIMERp = dff17_B(top.clk_reg.BOGA_ABCDExxx(), top.clk_reg.ALUR_SYS_RSTn(), _MERY_INT_TIMER_IN);
  }

  // FF06 TMA
  {
    /*p03.TUBY*/ wire _TUBY_FF06_RDp = and4(top.TEDO_CPU_RDp(), _RYFO_FF04_FF07p, top.cpu_bus.CPU_BUS_A01.qp(), top.cpu_bus.TOVY_A00n());
    /*p03.TYJU*/ wire _TYJU_FF06_WRn = nand4(top.TAPU_CPU_WRp_xxxDxxxx(), _RYFO_FF04_FF07p, top.cpu_bus.CPU_BUS_A01.qp(), top.cpu_bus.TOVY_A00n());

    /*p03.SABU*/ SABU_TMA_0 = dff17_AB(_TYJU_FF06_WRn, top.clk_reg.ALUR_SYS_RSTn(), top.cpu_bus.CPU_BUS_D0.qp());
    /*p03.NYKE*/ NYKE_TMA_1 = dff17_AB(_TYJU_FF06_WRn, top.clk_reg.ALUR_SYS_RSTn(), top.cpu_bus.CPU_BUS_D1.qp());
    /*p03.MURU*/ MURU_TMA_2 = dff17_AB(_TYJU_FF06_WRn, top.clk_reg.ALUR_SYS_RSTn(), top.cpu_bus.CPU_BUS_D2.qp());
    /*p03.TYVA*/ TYVA_TMA_3 = dff17_AB(_TYJU_FF06_WRn, top.clk_reg.ALUR_SYS_RSTn(), top.cpu_bus.CPU_BUS_D3.qp());
    /*p03.TYRU*/ TYRU_TMA_4 = dff17_AB(_TYJU_FF06_WRn, top.clk_reg.ALUR_SYS_RSTn(), top.cpu_bus.CPU_BUS_D4.qp());
    /*p03.SUFY*/ SUFY_TMA_5 = dff17_AB(_TYJU_FF06_WRn, top.clk_reg.ALUR_SYS_RSTn(), top.cpu_bus.CPU_BUS_D5.qp());
    /*p03.PETO*/ PETO_TMA_6 = dff17_AB(_TYJU_FF06_WRn, top.clk_reg.ALUR_SYS_RSTn(), top.cpu_bus.CPU_BUS_D6.qp());
    /*p03.SETA*/ SETA_TMA_7 = dff17_AB(_TYJU_FF06_WRn, top.clk_reg.ALUR_SYS_RSTn(), top.cpu_bus.CPU_BUS_D7.qp());

    /*p03.SETE*/ cpu_bus.CPU_BUS_D0 = tribuf_6p(_TUBY_FF06_RDp, SABU_TMA_0.qp());
    /*p03.PYRE*/ cpu_bus.CPU_BUS_D1 = tribuf_6p(_TUBY_FF06_RDp, NYKE_TMA_1.qp());
    /*p03.NOLA*/ cpu_bus.CPU_BUS_D2 = tribuf_6p(_TUBY_FF06_RDp, MURU_TMA_2.qp());
    /*p03.SALU*/ cpu_bus.CPU_BUS_D3 = tribuf_6p(_TUBY_FF06_RDp, TYVA_TMA_3.qp());
    /*p03.SUPO*/ cpu_bus.CPU_BUS_D4 = tribuf_6p(_TUBY_FF06_RDp, TYRU_TMA_4.qp());
    /*p03.SOTU*/ cpu_bus.CPU_BUS_D5 = tribuf_6p(_TUBY_FF06_RDp, SUFY_TMA_5.qp());
    /*p03.REVA*/ cpu_bus.CPU_BUS_D6 = tribuf_6p(_TUBY_FF06_RDp, PETO_TMA_6.qp());
    /*p03.SAPU*/ cpu_bus.CPU_BUS_D7 = tribuf_6p(_TUBY_FF06_RDp, SETA_TMA_7.qp());
  }

  // FF07 TAC
  {
    /*p03.SORA*/ wire _SORA_FF07_RDp = and4(top.TEDO_CPU_RDp(), _RYFO_FF04_FF07p, top.cpu_bus.CPU_BUS_A01.qp(), top.cpu_bus.CPU_BUS_A00.qp());
    /*p03.SARA*/ wire _SARA_FF07_WRn = nand4(top.TAPU_CPU_WRp_xxxDxxxx(), _RYFO_FF04_FF07p, top.cpu_bus.CPU_BUS_A01.qp(), top.cpu_bus.CPU_BUS_A00.qp());

    /*p03.SOPU*/ SOPU_TAC_0 = dff17_AB(_SARA_FF07_WRn, top.clk_reg.ALUR_SYS_RSTn(), top.cpu_bus.CPU_BUS_D0.qp());
    /*p03.SAMY*/ SAMY_TAC_1 = dff17_AB(_SARA_FF07_WRn, top.clk_reg.ALUR_SYS_RSTn(), top.cpu_bus.CPU_BUS_D1.qp());
    /*p03.SABO*/ SABO_TAC_2 = dff17_AB(_SARA_FF07_WRn, top.clk_reg.ALUR_SYS_RSTn(), top.cpu_bus.CPU_BUS_D2.qp());

    /*p03.RYLA*/ cpu_bus.CPU_BUS_D0 = tribuf_6p(_SORA_FF07_RDp, SOPU_TAC_0.qp());
    /*p03.ROTE*/ cpu_bus.CPU_BUS_D1 = tribuf_6p(_SORA_FF07_RDp, SAMY_TAC_1.qp());
    /*p03.SUPE*/ cpu_bus.CPU_BUS_D2 = tribuf_6p(_SORA_FF07_RDp, SABO_TAC_2.qp());
  }
}

//------------------------------------------------------------------------------

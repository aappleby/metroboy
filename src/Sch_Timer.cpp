#include "Sch_Timer.h"

#include "Sch_Top.h"

using namespace Schematics;

//------------------------------------------------------------------------------

void Timer::tick(const SchematicTop& /*top*/) {
}

void Timer::tock(const SchematicTop& top, CpuBus& cpu_bus) {
  /*p03.RYFO*/ wire _RYFO_FF04_FF07p = and (top.cpu_bus.CPU_BUS_A02, top.cpu_bus.SARE_XX00_XX07p(), top.cpu_bus.SYKE_FF00_FFFFp());

  // FF04 DIV
  {
    /*p01.TAGY*/ wire _TAGY_FF04_RDp = and (top.TEDO_CPU_RDp(), _RYFO_FF04_FF07p, top.cpu_bus.TOLA_A01n(), top.cpu_bus.TOVY_A00n());
    /*p01.TAPE*/ wire _TAPE_FF04_WRp = and(top.TAPU_CPU_WRp_xxxxEFGx(), _RYFO_FF04_FF07p, top.cpu_bus.TOLA_A01n(), top.cpu_bus.TOVY_A00n());

    /*p01.UFOL*/ wire _UFOL_DIV_RSTn = nor(top.clk_reg.UCOB_CLKBADp(), top.clk_reg.SYS_PIN_RSTp(), _TAPE_FF04_WRp);

    /*p01.UKUP*/ _UKUP_DIV_00 = dff17(top.clk_reg.BOGA_xBCDEFGH(), _UFOL_DIV_RSTn, !_UKUP_DIV_00.q());
    /*p01.UFOR*/ _UFOR_DIV_01 = dff17(!_UKUP_DIV_00.q(),           _UFOL_DIV_RSTn, !_UFOR_DIV_01.q());
    /*p01.UNER*/ _UNER_DIV_02 = dff17(!_UFOR_DIV_01.q(),           _UFOL_DIV_RSTn, !_UNER_DIV_02.q());
    /*p01.TERO*/ _TERO_DIV_03 = dff17(!_UNER_DIV_02.q(),           _UFOL_DIV_RSTn, !_TERO_DIV_03.q());
    /*p01.UNYK*/ _UNYK_DIV_04 = dff17(!_TERO_DIV_03.q(),           _UFOL_DIV_RSTn, !_UNYK_DIV_04.q());
    /*p01.TAMA*/ _TAMA_DIV_05 = dff17(!_UNYK_DIV_04.q(),           _UFOL_DIV_RSTn, !_TAMA_DIV_05.q());

    ///*p01.ULUR*/ wire DIV_06_CLK = mux2(BOGA_AxCDEFGH, DIV_05, FF60_1);
    /*p01.ULUR*/ wire _DIV_06_CLK = _TAMA_DIV_05.q();
    /*p01.UGOT*/ _UGOT_DIV_06 = dff17(!_DIV_06_CLK,      _UFOL_DIV_RSTn, !_UGOT_DIV_06.q());
    /*p01.TULU*/ _TULU_DIV_07 = dff17(!_UGOT_DIV_06.q(), _UFOL_DIV_RSTn, !_TULU_DIV_07.q());
    /*p01.TUGO*/ _TUGO_DIV_08 = dff17(!_TULU_DIV_07.q(), _UFOL_DIV_RSTn, !_TUGO_DIV_08.q());
    /*p01.TOFE*/ _TOFE_DIV_09 = dff17(!_TUGO_DIV_08.q(), _UFOL_DIV_RSTn, !_TOFE_DIV_09.q());
    /*p01.TERU*/ _TERU_DIV_10 = dff17(!_TOFE_DIV_09.q(), _UFOL_DIV_RSTn, !_TERU_DIV_10.q());
    /*p01.SOLA*/ _SOLA_DIV_11 = dff17(!_TERU_DIV_10.q(), _UFOL_DIV_RSTn, !_SOLA_DIV_11.q());
    /*p01.SUBU*/ _SUBU_DIV_12 = dff17(!_SOLA_DIV_11.q(), _UFOL_DIV_RSTn, !_SUBU_DIV_12.q());
    /*p01.TEKA*/ _TEKA_DIV_13 = dff17(!_SUBU_DIV_12.q(), _UFOL_DIV_RSTn, !_TEKA_DIV_13.q());
    /*p01.UKET*/ _UKET_DIV_14 = dff17(!_TEKA_DIV_13.q(), _UFOL_DIV_RSTn, !_UKET_DIV_14.q());
    /*p01.UPOF*/ _UPOF_DIV_15 = dff17(!_UKET_DIV_14.q(), _UFOL_DIV_RSTn, !_UPOF_DIV_15.q());

    /*p01.UTOK*/ wire _UMEK_DIV_06n = not(_UGOT_DIV_06.q());
    /*p01.SAPY*/ wire _UREK_DIV_07n = not(_TULU_DIV_07.q());
    /*p01.UTOK*/ wire _UTOK_DIV_08n = not(_TUGO_DIV_08.q());
    /*p01.SAPY*/ wire _SAPY_DIV_09n = not(_TOFE_DIV_09.q());
    /*p01.UMER*/ wire _UMER_DIV_10n = not(_TERU_DIV_10.q());
    /*p01.RAVE*/ wire _RAVE_DIV_11n = not(_SOLA_DIV_11.q());
    /*p01.RYSO*/ wire _RYSO_DIV_12n = not(_SUBU_DIV_12.q());
    /*p01.UDOR*/ wire _UDOR_DIV_13n = not(_TEKA_DIV_13.q());
    /*p01.TAWU*/ cpu_bus.CPU_BUS_D0 = tribuf_6p(_TAGY_FF04_RDp, not(_UMEK_DIV_06n));
    /*p01.TAKU*/ cpu_bus.CPU_BUS_D1 = tribuf_6p(_TAGY_FF04_RDp, not(_UREK_DIV_07n));
    /*p01.TEMU*/ cpu_bus.CPU_BUS_D2 = tribuf_6p(_TAGY_FF04_RDp, not(_UTOK_DIV_08n));
    /*p01.TUSE*/ cpu_bus.CPU_BUS_D3 = tribuf_6p(_TAGY_FF04_RDp, not(_SAPY_DIV_09n));
    /*p01.UPUG*/ cpu_bus.CPU_BUS_D4 = tribuf_6p(_TAGY_FF04_RDp, not(_UMER_DIV_10n));
    /*p01.SEPU*/ cpu_bus.CPU_BUS_D5 = tribuf_6p(_TAGY_FF04_RDp, not(_RAVE_DIV_11n));
    /*p01.SAWA*/ cpu_bus.CPU_BUS_D6 = tribuf_6p(_TAGY_FF04_RDp, not(_RYSO_DIV_12n));
    /*p01.TATU*/ cpu_bus.CPU_BUS_D7 = tribuf_6p(_TAGY_FF04_RDp, not(_UDOR_DIV_13n));
  }

  // FF05 TIMA
  {
    /*p03.TEDA*/ wire _TEDA_FF05_RDp = and (top.TEDO_CPU_RDp(), _RYFO_FF04_FF07p, top.cpu_bus.TOLA_A01n(), top.cpu_bus.CPU_BUS_A00);
    /*p03.TOPE*/ wire _TOPE_FF05_WRn = nand(top.TAPU_CPU_WRp_xxxxEFGx(), _RYFO_FF04_FF07p, top.cpu_bus.TOLA_A01n(), top.cpu_bus.CPU_BUS_A00);

    /*p03.MEKE*/ wire _MEKE_INT_TIMERn = not(_MOBA_INT_TIMERp.q());
    /*p03.UBOT*/ wire _UBOT_CLK_256Kn = not(_UFOR_DIV_01.q());
    /*p03.UVYR*/ wire _UVYR_CLK_64Kn = not(_TERO_DIV_03.q());

    /*p03.UKAP*/ wire _UKAP_CLK_MUXa = mux2_n(UVYN_DIV_05n(), _UVYR_CLK_64Kn, SOPU_TAC_0.q());
    /*p03.TEKO*/ wire _TEKO_CLK_MUXb = mux2_n(_UBOT_CLK_256Kn, UREK_DIV_07n(), SOPU_TAC_0.q());
    /*p03.TECY*/ wire _TECY_CLK_MUXc = mux2_n(_UKAP_CLK_MUXa, _TEKO_CLK_MUXb, SAMY_TAC_1.q());

    /*p03.SOGU*/ wire _SOGU_TIMA_CLK = nor(_TECY_CLK_MUXc, SABO_TAC_2.qn());
    /*p03.ROKE*/ wire _ROKE_TIMA_MUX_0 = mux2_n(SABU_TMA_0.q(), top.cpu_bus.CPU_BUS_D0, _TOPE_FF05_WRn); // suggests WRn
    /*p03.PETU*/ wire _PETU_TIMA_MUX_1 = mux2_n(NYKE_TMA_1.q(), top.cpu_bus.CPU_BUS_D1, _TOPE_FF05_WRn);
    /*p03.NYKU*/ wire _NYKU_TIMA_MUX_2 = mux2_n(MURU_TMA_2.q(), top.cpu_bus.CPU_BUS_D2, _TOPE_FF05_WRn); // die annotation box wrong, this is a 5-rung
    /*p03.SOCE*/ wire _SOCE_TIMA_MUX_3 = mux2_n(TYVA_TMA_3.q(), top.cpu_bus.CPU_BUS_D3, _TOPE_FF05_WRn);
    /*p03.SALA*/ wire _SALA_TIMA_MUX_4 = mux2_n(TYRU_TMA_4.q(), top.cpu_bus.CPU_BUS_D4, _TOPE_FF05_WRn);
    /*p03.SYRU*/ wire _SYRU_TIMA_MUX_5 = mux2_n(SUFY_TMA_5.q(), top.cpu_bus.CPU_BUS_D5, _TOPE_FF05_WRn);
    /*p03.REFU*/ wire _REFU_TIMA_MUX_6 = mux2_n(PETO_TMA_6.q(), top.cpu_bus.CPU_BUS_D6, _TOPE_FF05_WRn);
    /*p03.RATO*/ wire _RATO_TIMA_MUX_7 = mux2_n(SETA_TMA_7.q(), top.cpu_bus.CPU_BUS_D7, _TOPE_FF05_WRn);

    /*p03.PUXY*/ wire _PUXY_TIMA_LD_0 = nor(top.clk_reg.MULO_SYS_RSTn(), _ROKE_TIMA_MUX_0);
    /*p03.NERO*/ wire _NERO_TIMA_LD_1 = nor(top.clk_reg.MULO_SYS_RSTn(), _PETU_TIMA_MUX_1);
    /*p03.NADA*/ wire _NADA_TIMA_LD_2 = nor(top.clk_reg.MULO_SYS_RSTn(), _NYKU_TIMA_MUX_2);
    /*p03.REPA*/ wire _REPA_TIMA_LD_3 = nor(top.clk_reg.MULO_SYS_RSTn(), _SOCE_TIMA_MUX_3);
    /*p03.ROLU*/ wire _ROLU_TIMA_LD_4 = nor(top.clk_reg.MULO_SYS_RSTn(), _SALA_TIMA_MUX_4);
    /*p03.RUGY*/ wire _RUGY_TIMA_LD_5 = nor(top.clk_reg.MULO_SYS_RSTn(), _SYRU_TIMA_MUX_5);
    /*p03.PYMA*/ wire _PYMA_TIMA_LD_6 = nor(top.clk_reg.MULO_SYS_RSTn(), _REFU_TIMA_MUX_6);
    /*p03.PAGU*/ wire _PAGU_TIMA_LD_7 = nor(top.clk_reg.MULO_SYS_RSTn(), _RATO_TIMA_MUX_7);

#if 0

    TIMA_LOADp = or(and(!CPU_PIN_READ_MEM, CPU_PIN_WRp, xxxxEFGx, FF05), !ALUR_SYS_RSTn, !MEKE_INT_TIMERn);

    MUZU := or2
    MUZU_01 << CPU_PIN_READ_MEM
    MUZU_02 << TOPE
    MUZU_03 nc 
    MUZU_04 >> 


#endif

    /*p03.MUZU*/ wire _MUZU_TIMA_LOADn = or(top.cpu_bus.CPU_PIN_READ_MEM, _TOPE_FF05_WRn);
    /*p03.MEXU*/ wire _MEXU_TIMA_LOADp = nand(_MUZU_TIMA_LOADn, top.clk_reg.ALUR_SYS_RSTn(), _MEKE_INT_TIMERn);

    /*p03.REGA*/ REGA_TIMA_0 = ff20(_SOGU_TIMA_CLK,   _MEXU_TIMA_LOADp, _PUXY_TIMA_LD_0);
    /*p03.POVY*/ POVY_TIMA_1 = ff20(REGA_TIMA_0.qn(), _MEXU_TIMA_LOADp, _NERO_TIMA_LD_1);
    /*p03.PERU*/ PERU_TIMA_2 = ff20(POVY_TIMA_1.qn(), _MEXU_TIMA_LOADp, _NADA_TIMA_LD_2);
    /*p03.RATE*/ RATE_TIMA_3 = ff20(PERU_TIMA_2.qn(), _MEXU_TIMA_LOADp, _REPA_TIMA_LD_3);
    /*p03.RUBY*/ RUBY_TIMA_4 = ff20(RATE_TIMA_3.qn(), _MEXU_TIMA_LOADp, _ROLU_TIMA_LD_4);
    /*p03.RAGE*/ RAGE_TIMA_5 = ff20(RUBY_TIMA_4.qn(), _MEXU_TIMA_LOADp, _RUGY_TIMA_LD_5);
    /*p03.PEDA*/ PEDA_TIMA_6 = ff20(RAGE_TIMA_5.qn(), _MEXU_TIMA_LOADp, _PYMA_TIMA_LD_6);
    /*p03.NUGA*/ NUGA_TIMA_7 = ff20(PEDA_TIMA_6.qn(), _MEXU_TIMA_LOADp, _PAGU_TIMA_LD_7);

    /*p03.SOKU*/ cpu_bus.CPU_BUS_D0 = tribuf_6p(_TEDA_FF05_RDp, REGA_TIMA_0.q());
    /*p03.RACY*/ cpu_bus.CPU_BUS_D1 = tribuf_6p(_TEDA_FF05_RDp, POVY_TIMA_1.q());
    /*p03.RAVY*/ cpu_bus.CPU_BUS_D2 = tribuf_6p(_TEDA_FF05_RDp, PERU_TIMA_2.q());
    /*p03.SOSY*/ cpu_bus.CPU_BUS_D3 = tribuf_6p(_TEDA_FF05_RDp, RATE_TIMA_3.q());
    /*p03.SOMU*/ cpu_bus.CPU_BUS_D4 = tribuf_6p(_TEDA_FF05_RDp, RUBY_TIMA_4.q());
    /*p03.SURO*/ cpu_bus.CPU_BUS_D5 = tribuf_6p(_TEDA_FF05_RDp, RAGE_TIMA_5.q());
    /*p03.ROWU*/ cpu_bus.CPU_BUS_D6 = tribuf_6p(_TEDA_FF05_RDp, PEDA_TIMA_6.q());
    /*p03.PUSO*/ cpu_bus.CPU_BUS_D7 = tribuf_6p(_TEDA_FF05_RDp, NUGA_TIMA_7.q());

    /*p03.MUGY*/ wire _MUGY_TIMA_MAX_RSTn = not(_MEXU_TIMA_LOADp);

    /*p03.NYDU*/ NYDU_TIMA_MAX = dff17(top.clk_reg.BOGA_xBCDEFGH(), _MUGY_TIMA_MAX_RSTn, NUGA_TIMA_7.q());
  }

  {
    /*p03.MERY*/ wire _MERY_INT_TIMER_IN = nor(!NYDU_TIMA_MAX.q(), NUGA_TIMA_7.q());

    /*p03.MOBA*/ _MOBA_INT_TIMERp = dff17(top.clk_reg.BOGA_xBCDEFGH(), top.clk_reg.ALUR_SYS_RSTn(), _MERY_INT_TIMER_IN);
  }

  // FF06 TMA
  {
    /*p03.TUBY*/ wire _TUBY_FF06_RDp = and (top.TEDO_CPU_RDp(), _RYFO_FF04_FF07p, top.cpu_bus.CPU_BUS_A01, top.cpu_bus.TOVY_A00n());
    /*p03.TYJU*/ wire _TYJU_FF06_WRn = nand(top.TAPU_CPU_WRp_xxxxEFGx(), _RYFO_FF04_FF07p, top.cpu_bus.CPU_BUS_A01, top.cpu_bus.TOVY_A00n());

    /*p03.SABU*/ SABU_TMA_0 = dff17(_TYJU_FF06_WRn, top.clk_reg.ALUR_SYS_RSTn(), top.cpu_bus.CPU_BUS_D0);
    /*p03.NYKE*/ NYKE_TMA_1 = dff17(_TYJU_FF06_WRn, top.clk_reg.ALUR_SYS_RSTn(), top.cpu_bus.CPU_BUS_D1);
    /*p03.MURU*/ MURU_TMA_2 = dff17(_TYJU_FF06_WRn, top.clk_reg.ALUR_SYS_RSTn(), top.cpu_bus.CPU_BUS_D2);
    /*p03.TYVA*/ TYVA_TMA_3 = dff17(_TYJU_FF06_WRn, top.clk_reg.ALUR_SYS_RSTn(), top.cpu_bus.CPU_BUS_D3);
    /*p03.TYRU*/ TYRU_TMA_4 = dff17(_TYJU_FF06_WRn, top.clk_reg.ALUR_SYS_RSTn(), top.cpu_bus.CPU_BUS_D4);
    /*p03.SUFY*/ SUFY_TMA_5 = dff17(_TYJU_FF06_WRn, top.clk_reg.ALUR_SYS_RSTn(), top.cpu_bus.CPU_BUS_D5);
    /*p03.PETO*/ PETO_TMA_6 = dff17(_TYJU_FF06_WRn, top.clk_reg.ALUR_SYS_RSTn(), top.cpu_bus.CPU_BUS_D6);
    /*p03.SETA*/ SETA_TMA_7 = dff17(_TYJU_FF06_WRn, top.clk_reg.ALUR_SYS_RSTn(), top.cpu_bus.CPU_BUS_D7);

    /*p03.SETE*/ cpu_bus.CPU_BUS_D0 = tribuf_6p(_TUBY_FF06_RDp, SABU_TMA_0.q());
    /*p03.PYRE*/ cpu_bus.CPU_BUS_D1 = tribuf_6p(_TUBY_FF06_RDp, NYKE_TMA_1.q());
    /*p03.NOLA*/ cpu_bus.CPU_BUS_D2 = tribuf_6p(_TUBY_FF06_RDp, MURU_TMA_2.q());
    /*p03.SALU*/ cpu_bus.CPU_BUS_D3 = tribuf_6p(_TUBY_FF06_RDp, TYVA_TMA_3.q());
    /*p03.SUPO*/ cpu_bus.CPU_BUS_D4 = tribuf_6p(_TUBY_FF06_RDp, TYRU_TMA_4.q());
    /*p03.SOTU*/ cpu_bus.CPU_BUS_D5 = tribuf_6p(_TUBY_FF06_RDp, SUFY_TMA_5.q());
    /*p03.REVA*/ cpu_bus.CPU_BUS_D6 = tribuf_6p(_TUBY_FF06_RDp, PETO_TMA_6.q());
    /*p03.SAPU*/ cpu_bus.CPU_BUS_D7 = tribuf_6p(_TUBY_FF06_RDp, SETA_TMA_7.q());
  }

  // FF07 TAC
  {
    /*p03.SORA*/ wire _SORA_FF07_RDp = and (top.TEDO_CPU_RDp(), _RYFO_FF04_FF07p, top.cpu_bus.CPU_BUS_A01, top.cpu_bus.CPU_BUS_A00);
    /*p03.SARA*/ wire _SARA_FF07_WRn = nand(top.TAPU_CPU_WRp_xxxxEFGx(), _RYFO_FF04_FF07p, top.cpu_bus.CPU_BUS_A01, top.cpu_bus.CPU_BUS_A00);

    /*p03.SOPU*/ SOPU_TAC_0 = dff17(_SARA_FF07_WRn, top.clk_reg.ALUR_SYS_RSTn(), top.cpu_bus.CPU_BUS_D0);
    /*p03.SAMY*/ SAMY_TAC_1 = dff17(_SARA_FF07_WRn, top.clk_reg.ALUR_SYS_RSTn(), top.cpu_bus.CPU_BUS_D1);
    /*p03.SABO*/ SABO_TAC_2 = dff17(_SARA_FF07_WRn, top.clk_reg.ALUR_SYS_RSTn(), top.cpu_bus.CPU_BUS_D2);

    /*p03.RYLA*/ cpu_bus.CPU_BUS_D0 = tribuf_6p(_SORA_FF07_RDp, SOPU_TAC_0.q());
    /*p03.ROTE*/ cpu_bus.CPU_BUS_D1 = tribuf_6p(_SORA_FF07_RDp, SAMY_TAC_1.q());
    /*p03.SUPE*/ cpu_bus.CPU_BUS_D2 = tribuf_6p(_SORA_FF07_RDp, SABO_TAC_2.q());
  }
}

//------------------------------------------------------------------------------

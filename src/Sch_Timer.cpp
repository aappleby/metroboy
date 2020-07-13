#include "Sch_Timer.h"

#include "Sch_Top.h"

using namespace Schematics;

//------------------------------------------------------------------------------

void Timer::tick(SchematicTop& /*top*/) {
}

void Timer::tock(SchematicTop& top) {
  /*p03.RYFO*/ wire _RYFO_FF04_FF07p = and (top.CPU_PIN_A02, top.SARE_XX00_XX07p(), top.SYKE_FF00_FFFFp());

  /*p01.TAGY*/ wire _TAGY_FF04_RD = and (top.TEDO_CPU_RDp(), _RYFO_FF04_FF07p, top.TOLA_A01n(), top.TOVY_A00n());
  /*p03.TEDA*/ wire _TEDA_FF05_RD = and (top.TEDO_CPU_RDp(), _RYFO_FF04_FF07p, top.TOLA_A01n(), top.CPU_PIN_A00);
  /*p03.TUBY*/ wire _TUBY_FF06_RD = and (top.TEDO_CPU_RDp(), _RYFO_FF04_FF07p, top.CPU_PIN_A01, top.TOVY_A00n());
  /*p03.SORA*/ wire _SORA_FF07_RD = and (top.TEDO_CPU_RDp(), _RYFO_FF04_FF07p, top.CPU_PIN_A01, top.CPU_PIN_A00);

  /*p01.TAPE*/ wire _TAPE_FF04_WRp =  and(top.TAPU_CPU_WRp_xxxxEFGx(), _RYFO_FF04_FF07p, top.TOLA_A01n(), top.TOVY_A00n());
  /*p03.TOPE*/ wire _TOPE_FF05_WRn = nand(top.TAPU_CPU_WRp_xxxxEFGx(), _RYFO_FF04_FF07p, top.TOLA_A01n(), top.CPU_PIN_A00);
  /*p03.TYJU*/ wire _TYJU_FF06_WRn = nand(top.TAPU_CPU_WRp_xxxxEFGx(), _RYFO_FF04_FF07p, top.CPU_PIN_A01, top.TOVY_A00n());
  /*p03.SARA*/ wire _SARA_FF07_WRn = nand(top.TAPU_CPU_WRp_xxxxEFGx(), _RYFO_FF04_FF07p, top.CPU_PIN_A01, top.CPU_PIN_A00);

  // FF04 DIV
  {
    /*p01.UFOL*/ wire _UFOL_DIV_RSTn = nor(top.clk_reg.UCOB_CLKBADp(), top.SYS_PIN_RSTp, _TAPE_FF04_WRp);

    /*p01.UKUP*/ _UKUP_DIV_00.set(top.clk_reg.BOGA_xBCDEFGH(), _UFOL_DIV_RSTn, !_UKUP_DIV_00.q());
    /*p01.UFOR*/ _UFOR_DIV_01.set(!_UKUP_DIV_00.q(),           _UFOL_DIV_RSTn, !_UFOR_DIV_01.q());
    /*p01.UNER*/ _UNER_DIV_02.set(!_UFOR_DIV_01.q(),           _UFOL_DIV_RSTn, !_UNER_DIV_02.q());
    /*p01.TERO*/ _TERO_DIV_03.set(!_UNER_DIV_02.q(),           _UFOL_DIV_RSTn, !_TERO_DIV_03.q());
    /*p01.UNYK*/ _UNYK_DIV_04.set(!_TERO_DIV_03.q(),           _UFOL_DIV_RSTn, !_UNYK_DIV_04.q());
    /*p01.TAMA*/ _TAMA_DIV_05.set(!_UNYK_DIV_04.q(),           _UFOL_DIV_RSTn, !_TAMA_DIV_05.q());

    ///*p01.ULUR*/ wire DIV_06_CLK = mux2(BOGA_AxCDEFGH, DIV_05, FF60_1);
    /*p01.ULUR*/ wire _DIV_06_CLK = _TAMA_DIV_05.q();
    /*p01.UGOT*/ _UGOT_DIV_06.set(!_DIV_06_CLK,     _UFOL_DIV_RSTn, !_UGOT_DIV_06.q());
    /*p01.TULU*/ _TULU_DIV_07.set(!_UGOT_DIV_06.q(), _UFOL_DIV_RSTn, !_TULU_DIV_07.q());
    /*p01.TUGO*/ _TUGO_DIV_08.set(!_TULU_DIV_07.q(), _UFOL_DIV_RSTn, !_TUGO_DIV_08.q());
    /*p01.TOFE*/ _TOFE_DIV_09.set(!_TUGO_DIV_08.q(), _UFOL_DIV_RSTn, !_TOFE_DIV_09.q());
    /*p01.TERU*/ _TERU_DIV_10.set(!_TOFE_DIV_09.q(), _UFOL_DIV_RSTn, !_TERU_DIV_10.q());
    /*p01.SOLA*/ _SOLA_DIV_11.set(!_TERU_DIV_10.q(), _UFOL_DIV_RSTn, !_SOLA_DIV_11.q());
    /*p01.SUBU*/ _SUBU_DIV_12.set(!_SOLA_DIV_11.q(), _UFOL_DIV_RSTn, !_SUBU_DIV_12.q());
    /*p01.TEKA*/ _TEKA_DIV_13.set(!_SUBU_DIV_12.q(), _UFOL_DIV_RSTn, !_TEKA_DIV_13.q());
    /*p01.UKET*/ _UKET_DIV_14.set(!_TEKA_DIV_13.q(), _UFOL_DIV_RSTn, !_UKET_DIV_14.q());
    /*p01.UPOF*/ _UPOF_DIV_15.set(!_UKET_DIV_14.q(), _UFOL_DIV_RSTn, !_UPOF_DIV_15.q());


    /*p01.UTOK*/ wire _UTOK_DIV_08n = not(_TUGO_DIV_08.q());
    /*p01.SAPY*/ wire _SAPY_DIV_09n = not(_TOFE_DIV_09.q());
    /*p01.UMER*/ wire _UMER_DIV_10n = not(_TERU_DIV_10.q());
    /*p01.RAVE*/ wire _RAVE_DIV_11n = not(_SOLA_DIV_11.q());
    /*p01.RYSO*/ wire _RYSO_DIV_12n = not(_SUBU_DIV_12.q());
    /*p01.UDOR*/ wire _UDOR_DIV_13n = not(_TEKA_DIV_13.q());
    /*p01.TAWU*/ top.int_bus.INT_TRI_D0.set_tribuf_6p(_TAGY_FF04_RD, not(UMEK_DIV_06n()));
    /*p01.TAKU*/ top.int_bus.INT_TRI_D1.set_tribuf_6p(_TAGY_FF04_RD, not(UREK_DIV_07n()));
    /*p01.TEMU*/ top.int_bus.INT_TRI_D2.set_tribuf_6p(_TAGY_FF04_RD, not(_UTOK_DIV_08n));
    /*p01.TUSE*/ top.int_bus.INT_TRI_D3.set_tribuf_6p(_TAGY_FF04_RD, not(_SAPY_DIV_09n));
    /*p01.UPUG*/ top.int_bus.INT_TRI_D4.set_tribuf_6p(_TAGY_FF04_RD, not(_UMER_DIV_10n));
    /*p01.SEPU*/ top.int_bus.INT_TRI_D5.set_tribuf_6p(_TAGY_FF04_RD, not(_RAVE_DIV_11n));
    /*p01.SAWA*/ top.int_bus.INT_TRI_D6.set_tribuf_6p(_TAGY_FF04_RD, not(_RYSO_DIV_12n));
    /*p01.TATU*/ top.int_bus.INT_TRI_D7.set_tribuf_6p(_TAGY_FF04_RD, not(_UDOR_DIV_13n));
  }

  // FF05 TIMA
  {
    /*p03.MEKE*/ wire _MEKE_INT_TIMERn = not(_MOBA_INT_TIMERp.q());
    /*p03.UBOT*/ wire _UBOT_CLK_256Kn = not(_UFOR_DIV_01.q());
    /*p03.UVYR*/ wire _UVYR_CLK_64Kn = not(_TERO_DIV_03.q());

    /*p03.UKAP*/ wire _UKAP_CLK_MUXa = mux2_n(UVYN_DIV_05n(), _UVYR_CLK_64Kn, SOPU_TAC_0.q());
    /*p03.TEKO*/ wire _TEKO_CLK_MUXb = mux2_n(_UBOT_CLK_256Kn, UREK_DIV_07n(), SOPU_TAC_0.q());
    /*p03.TECY*/ wire _TECY_CLK_MUXc = mux2_n(_UKAP_CLK_MUXa, _TEKO_CLK_MUXb, SAMY_TAC_1.q());

    /*p03.SOGU*/ wire _SOGU_TIMA_CLK = nor(_TECY_CLK_MUXc, SABO_TAC_2.qn());
    /*p03.ROKE*/ wire _ROKE_TIMA_MUX_0 = mux2_n(SABU_TMA_0.q(), top.int_bus.INT_TRI_D0, _TOPE_FF05_WRn); // suggests WRn
    /*p03.PETU*/ wire _PETU_TIMA_MUX_1 = mux2_n(NYKE_TMA_1.q(), top.int_bus.INT_TRI_D1, _TOPE_FF05_WRn);
    /*p03.NYKU*/ wire _NYKU_TIMA_MUX_2 = mux2_n(MURU_TMA_2.q(), top.int_bus.INT_TRI_D2, _TOPE_FF05_WRn); // die annotation box wrong, this is a 5-rung
    /*p03.SOCE*/ wire _SOCE_TIMA_MUX_3 = mux2_n(TYVA_TMA_3.q(), top.int_bus.INT_TRI_D3, _TOPE_FF05_WRn);
    /*p03.SALA*/ wire _SALA_TIMA_MUX_4 = mux2_n(TYRU_TMA_4.q(), top.int_bus.INT_TRI_D4, _TOPE_FF05_WRn);
    /*p03.SYRU*/ wire _SYRU_TIMA_MUX_5 = mux2_n(SUFY_TMA_5.q(), top.int_bus.INT_TRI_D5, _TOPE_FF05_WRn);
    /*p03.REFU*/ wire _REFU_TIMA_MUX_6 = mux2_n(PETO_TMA_6.q(), top.int_bus.INT_TRI_D6, _TOPE_FF05_WRn);
    /*p03.RATO*/ wire _RATO_TIMA_MUX_7 = mux2_n(SETA_TMA_7.q(), top.int_bus.INT_TRI_D7, _TOPE_FF05_WRn);

    /*p03.PUXY*/ wire _PUXY_TIMA_LD_0 = nor(top.rst_reg.MULO_TIMA_RST(), _ROKE_TIMA_MUX_0);
    /*p03.NERO*/ wire _NERO_TIMA_LD_1 = nor(top.rst_reg.MULO_TIMA_RST(), _PETU_TIMA_MUX_1);
    /*p03.NADA*/ wire _NADA_TIMA_LD_2 = nor(top.rst_reg.MULO_TIMA_RST(), _NYKU_TIMA_MUX_2);
    /*p03.REPA*/ wire _REPA_TIMA_LD_3 = nor(top.rst_reg.MULO_TIMA_RST(), _SOCE_TIMA_MUX_3);
    /*p03.ROLU*/ wire _ROLU_TIMA_LD_4 = nor(top.rst_reg.MULO_TIMA_RST(), _SALA_TIMA_MUX_4);
    /*p03.RUGY*/ wire _RUGY_TIMA_LD_5 = nor(top.rst_reg.MULO_TIMA_RST(), _SYRU_TIMA_MUX_5);
    /*p03.PYMA*/ wire _PYMA_TIMA_LD_6 = nor(top.rst_reg.MULO_TIMA_RST(), _REFU_TIMA_MUX_6);
    /*p03.PAGU*/ wire _PAGU_TIMA_LD_7 = nor(top.rst_reg.MULO_TIMA_RST(), _RATO_TIMA_MUX_7);

    /*p03.MUZU*/ wire _MUZU_TIMA_LOADn = or(top.CPU_PIN5, _TOPE_FF05_WRn); // suggests CPU_PIN5 = DATA_VALIDn
    /*p03.MEXU*/ wire _MEXU_TIMA_LOADp = nand(_MUZU_TIMA_LOADn, top.rst_reg.ALUR_SYS_RSTn(), _MEKE_INT_TIMERn);

    /*p03.REGA*/ REGA_TIMA_0.clk_n(_SOGU_TIMA_CLK,   _MEXU_TIMA_LOADp, _PUXY_TIMA_LD_0);
    /*p03.POVY*/ POVY_TIMA_1.clk_n(REGA_TIMA_0.q(), _MEXU_TIMA_LOADp, _NERO_TIMA_LD_1);
    /*p03.PERU*/ PERU_TIMA_2.clk_n(POVY_TIMA_1.q(), _MEXU_TIMA_LOADp, _NADA_TIMA_LD_2);
    /*p03.RATE*/ RATE_TIMA_3.clk_n(PERU_TIMA_2.q(), _MEXU_TIMA_LOADp, _REPA_TIMA_LD_3);
    /*p03.RUBY*/ RUBY_TIMA_4.clk_n(RATE_TIMA_3.q(), _MEXU_TIMA_LOADp, _ROLU_TIMA_LD_4);
    /*p03.RAGE*/ RAGE_TIMA_5.clk_n(RUBY_TIMA_4.q(), _MEXU_TIMA_LOADp, _RUGY_TIMA_LD_5);
    /*p03.PEDA*/ PEDA_TIMA_6.clk_n(RAGE_TIMA_5.q(), _MEXU_TIMA_LOADp, _PYMA_TIMA_LD_6);
    /*p03.NUGA*/ NUGA_TIMA_7.clk_n(PEDA_TIMA_6.q(), _MEXU_TIMA_LOADp, _PAGU_TIMA_LD_7);

    /*p03.SOKU*/ top.int_bus.INT_TRI_D0.set_tribuf_6p(_TEDA_FF05_RD, REGA_TIMA_0.q());
    /*p03.RACY*/ top.int_bus.INT_TRI_D1.set_tribuf_6p(_TEDA_FF05_RD, POVY_TIMA_1.q());
    /*p03.RAVY*/ top.int_bus.INT_TRI_D2.set_tribuf_6p(_TEDA_FF05_RD, PERU_TIMA_2.q());
    /*p03.SOSY*/ top.int_bus.INT_TRI_D3.set_tribuf_6p(_TEDA_FF05_RD, RATE_TIMA_3.q());
    /*p03.SOMU*/ top.int_bus.INT_TRI_D4.set_tribuf_6p(_TEDA_FF05_RD, RUBY_TIMA_4.q());
    /*p03.SURO*/ top.int_bus.INT_TRI_D5.set_tribuf_6p(_TEDA_FF05_RD, RAGE_TIMA_5.q());
    /*p03.ROWU*/ top.int_bus.INT_TRI_D6.set_tribuf_6p(_TEDA_FF05_RD, PEDA_TIMA_6.q());
    /*p03.PUSO*/ top.int_bus.INT_TRI_D7.set_tribuf_6p(_TEDA_FF05_RD, NUGA_TIMA_7.q());

    /*p03.MUGY*/ wire _MUGY_TIMA_MAX_RSTn = not(_MEXU_TIMA_LOADp);

    /*p03.NYDU*/ NYDU_TIMA_MAX.set(top.clk_reg.BOGA_xBCDEFGH(), _MUGY_TIMA_MAX_RSTn, NUGA_TIMA_7.q());
  }

  {
    /*p03.MERY*/ wire _MERY_INT_TIMER_IN = nor(!NYDU_TIMA_MAX.q(), NUGA_TIMA_7.q());

    /*p03.MOBA*/ _MOBA_INT_TIMERp.set(top.clk_reg.BOGA_xBCDEFGH(), top.rst_reg.ALUR_SYS_RSTn(), _MERY_INT_TIMER_IN);
  }

  // FF06 TMA
  {
    /*p03.SABU*/ SABU_TMA_0.set(_TYJU_FF06_WRn, top.rst_reg.ALUR_SYS_RSTn(), top.int_bus.INT_TRI_D0);
    /*p03.NYKE*/ NYKE_TMA_1.set(_TYJU_FF06_WRn, top.rst_reg.ALUR_SYS_RSTn(), top.int_bus.INT_TRI_D1);
    /*p03.MURU*/ MURU_TMA_2.set(_TYJU_FF06_WRn, top.rst_reg.ALUR_SYS_RSTn(), top.int_bus.INT_TRI_D2);
    /*p03.TYVA*/ TYVA_TMA_3.set(_TYJU_FF06_WRn, top.rst_reg.ALUR_SYS_RSTn(), top.int_bus.INT_TRI_D3);
    /*p03.TYRU*/ TYRU_TMA_4.set(_TYJU_FF06_WRn, top.rst_reg.ALUR_SYS_RSTn(), top.int_bus.INT_TRI_D4);
    /*p03.SUFY*/ SUFY_TMA_5.set(_TYJU_FF06_WRn, top.rst_reg.ALUR_SYS_RSTn(), top.int_bus.INT_TRI_D5);
    /*p03.PETO*/ PETO_TMA_6.set(_TYJU_FF06_WRn, top.rst_reg.ALUR_SYS_RSTn(), top.int_bus.INT_TRI_D6);
    /*p03.SETA*/ SETA_TMA_7.set(_TYJU_FF06_WRn, top.rst_reg.ALUR_SYS_RSTn(), top.int_bus.INT_TRI_D7);

    /*p03.SETE*/ top.int_bus.INT_TRI_D0.set_tribuf_6p(_TUBY_FF06_RD, SABU_TMA_0.q());
    /*p03.PYRE*/ top.int_bus.INT_TRI_D1.set_tribuf_6p(_TUBY_FF06_RD, NYKE_TMA_1.q());
    /*p03.NOLA*/ top.int_bus.INT_TRI_D2.set_tribuf_6p(_TUBY_FF06_RD, MURU_TMA_2.q());
    /*p03.SALU*/ top.int_bus.INT_TRI_D3.set_tribuf_6p(_TUBY_FF06_RD, TYVA_TMA_3.q());
    /*p03.SUPO*/ top.int_bus.INT_TRI_D4.set_tribuf_6p(_TUBY_FF06_RD, TYRU_TMA_4.q());
    /*p03.SOTU*/ top.int_bus.INT_TRI_D5.set_tribuf_6p(_TUBY_FF06_RD, SUFY_TMA_5.q());
    /*p03.REVA*/ top.int_bus.INT_TRI_D6.set_tribuf_6p(_TUBY_FF06_RD, PETO_TMA_6.q());
    /*p03.SAPU*/ top.int_bus.INT_TRI_D7.set_tribuf_6p(_TUBY_FF06_RD, SETA_TMA_7.q());
  }

  // FF07 TAC
  {
    /*p03.SOPU*/ SOPU_TAC_0.set(_SARA_FF07_WRn, top.rst_reg.ALUR_SYS_RSTn(), top.int_bus.INT_TRI_D0);
    /*p03.SAMY*/ SAMY_TAC_1.set(_SARA_FF07_WRn, top.rst_reg.ALUR_SYS_RSTn(), top.int_bus.INT_TRI_D1);
    /*p03.SABO*/ SABO_TAC_2.set(_SARA_FF07_WRn, top.rst_reg.ALUR_SYS_RSTn(), top.int_bus.INT_TRI_D2);

    /*p03.RYLA*/ top.int_bus.INT_TRI_D0.set_tribuf_6p(_SORA_FF07_RD, SOPU_TAC_0.q());
    /*p03.ROTE*/ top.int_bus.INT_TRI_D1.set_tribuf_6p(_SORA_FF07_RD, SAMY_TAC_1.q());
    /*p03.SUPE*/ top.int_bus.INT_TRI_D2.set_tribuf_6p(_SORA_FF07_RD, SABO_TAC_2.q());
  }
}

//------------------------------------------------------------------------------

SignalHash Timer::commit() {
  SignalHash hash;
  /*p01.UKUP*/ hash << _UKUP_DIV_00.commit();
  /*p01.UFOR*/ hash << _UFOR_DIV_01.commit();
  /*p01.UNER*/ hash << _UNER_DIV_02.commit();
  /*p01.TERO*/ hash << _TERO_DIV_03.commit();
  /*p01.UNYK*/ hash << _UNYK_DIV_04.commit();
  /*p01.TAMA*/ hash << _TAMA_DIV_05.commit();
  /*p01.UGOT*/ hash << _UGOT_DIV_06.commit();
  /*p01.TULU*/ hash << _TULU_DIV_07.commit();
  /*p01.TUGO*/ hash << _TUGO_DIV_08.commit();
  /*p01.TOFE*/ hash << _TOFE_DIV_09.commit();
  /*p01.TERU*/ hash << _TERU_DIV_10.commit();
  /*p01.SOLA*/ hash << _SOLA_DIV_11.commit();
  /*p01.SUBU*/ hash << _SUBU_DIV_12.commit();
  /*p01.TEKA*/ hash << _TEKA_DIV_13.commit();
  /*p01.UKET*/ hash << _UKET_DIV_14.commit();
  /*p01.UPOF*/ hash << _UPOF_DIV_15.commit();
  /*p03.REGA*/ hash << REGA_TIMA_0.commit();
  /*p03.POVY*/ hash << POVY_TIMA_1.commit();
  /*p03.PERU*/ hash << PERU_TIMA_2.commit();
  /*p03.RATE*/ hash << RATE_TIMA_3.commit();
  /*p03.RUBY*/ hash << RUBY_TIMA_4.commit();
  /*p03.RAGE*/ hash << RAGE_TIMA_5.commit();
  /*p03.PEDA*/ hash << PEDA_TIMA_6.commit();
  /*p03.NUGA*/ hash << NUGA_TIMA_7.commit();
  /*p03.NYDU*/ hash << NYDU_TIMA_MAX.commit();
  /*p03.MOBA*/ hash << _MOBA_INT_TIMERp.commit();
  /*p03.SABU*/ hash << SABU_TMA_0.commit();
  /*p03.NYKE*/ hash << NYKE_TMA_1.commit();
  /*p03.MURU*/ hash << MURU_TMA_2.commit();
  /*p03.TYVA*/ hash << TYVA_TMA_3.commit();
  /*p03.TYRU*/ hash << TYRU_TMA_4.commit();
  /*p03.SUFY*/ hash << SUFY_TMA_5.commit();
  /*p03.PETO*/ hash << PETO_TMA_6.commit();
  /*p03.SETA*/ hash << SETA_TMA_7.commit();
  /*p03.SOPU*/ hash << SOPU_TAC_0.commit();
  /*p03.SAMY*/ hash << SAMY_TAC_1.commit();
  /*p03.SABO*/ hash << SABO_TAC_2.commit();
  return hash;
}

//------------------------------------------------------------------------------

#if 0
void dump_regs(TextPainter& text_painter) {
  text_painter.dprintf("----- TIM_REG -----\n");
  text_painter.dprintf("DIV       %d\n", get_div());
  text_painter.dprintf("TIMA      %d\n", get_tima());
  text_painter.dprintf("TMA       %d\n", get_tma());
  text_painter.newline();

  UKUP_DIV_00.dump(text_painter, "UKUP_DIV_00    ");
  UFOR_DIV_01.dump(text_painter, "UFOR_DIV_01    ");
  UNER_DIV_02.dump(text_painter, "UNER_DIV_02    ");
  TERO_DIV_03.dump(text_painter, "TERO_DIV_03    ");
  UNYK_DIV_04.dump(text_painter, "UNYK_DIV_04    ");
  TAMA_DIV_05.dump(text_painter, "TAMA_DIV_05    ");
  UGOT_DIV_06.dump(text_painter, "UGOT_DIV_06    ");
  TULU_DIV_07.dump(text_painter, "TULU_DIV_07    ");
  TUGO_DIV_08.dump(text_painter, "TUGO_DIV_08    ");
  TOFE_DIV_09.dump(text_painter, "TOFE_DIV_09    ");
  TERU_DIV_10.dump(text_painter, "TERU_DIV_10    ");
  SOLA_DIV_11.dump(text_painter, "SOLA_DIV_11    ");
  SUBU_DIV_12.dump(text_painter, "SUBU_DIV_12    ");
  TEKA_DIV_13.dump(text_painter, "TEKA_DIV_13    ");
  UKET_DIV_14.dump(text_painter, "UKET_DIV_14    ");
  UPOF_DIV_15.dump(text_painter, "UPOF_DIV_15    ");
  text_painter.newline();

  REGA_TIMA_0.dump(text_painter, "REGA_TIMA_0    ");
  POVY_TIMA_1.dump(text_painter, "POVY_TIMA_1    ");
  PERU_TIMA_2.dump(text_painter, "PERU_TIMA_2    ");
  RATE_TIMA_3.dump(text_painter, "RATE_TIMA_3    ");
  RUBY_TIMA_4.dump(text_painter, "RUBY_TIMA_4    ");
  RAGE_TIMA_5.dump(text_painter, "RAGE_TIMA_5    ");
  PEDA_TIMA_6.dump(text_painter, "PEDA_TIMA_6    ");
  NUGA_TIMA_7.dump(text_painter, "NUGA_TIMA_7    ");
  text_painter.newline();

  SABU_TMA_0.dump(text_painter, "SABU_TMA_0     ");
  NYKE_TMA_1.dump(text_painter, "NYKE_TMA_1     ");
  MURU_TMA_2.dump(text_painter, "MURU_TMA_2     ");
  TYVA_TMA_3.dump(text_painter, "TYVA_TMA_3     ");
  TYRU_TMA_4.dump(text_painter, "TYRU_TMA_4     ");
  SUFY_TMA_5.dump(text_painter, "SUFY_TMA_5     ");
  PETO_TMA_6.dump(text_painter, "PETO_TMA_6     ");
  SETA_TMA_7.dump(text_painter, "SETA_TMA_7     ");
  text_painter.newline();

  SOPU_TAC_0.dump(text_painter, "SOPU_TAC_0     ");
  SAMY_TAC_1.dump(text_painter, "SAMY_TAC_1     ");
  SABO_TAC_2.dump(text_painter, "SABO_TAC_2     ");
  text_painter.newline();

  NYDU_TIMA_MAX.dump(text_painter, "NYDU_TIMA_MAX  ");
  MOBA_INT_TIMERp.dump(text_painter, "MOBA_INT_TIMERp ");
  text_painter.newline();
}

int get_div() const {
  return pack(UKUP_DIV_00.q(), UFOR_DIV_01.q(), UNER_DIV_02.q(), TERO_DIV_03.q(), UNYK_DIV_04.q(), TAMA_DIV_05.q(), UGOT_DIV_06.q(), TULU_DIV_07.q(),
    TUGO_DIV_08.q(), TOFE_DIV_09.q(), TERU_DIV_10.q(), SOLA_DIV_11.q(), SUBU_DIV_12.q(), TEKA_DIV_13.q(), UKET_DIV_14.q(), UPOF_DIV_15.q());
}

/*
void set_div(uint16_t x) {
DIV_00.set2(x & 0x0001); DIV_01.set2(x & 0x0002); DIV_02.set2(x & 0x0004); DIV_03.set2(x & 0x0008);
DIV_04.set2(x & 0x0010); DIV_05.set2(x & 0x0020); DIV_06.set2(x & 0x0040); DIV_07.set2(x & 0x0080);
DIV_08.set2(x & 0x0100); DIV_09.set2(x & 0x0200); DIV_10.set2(x & 0x0400); DIV_11.set2(x & 0x0800);
DIV_12.set2(x & 0x1000); DIV_13.set2(x & 0x2000); DIV_14.set2(x & 0x4000); DIV_15.set2(x & 0x8000);
}
*/

int get_tima() const {
  return pack(REGA_TIMA_0.q(), POVY_TIMA_1.q(), PERU_TIMA_2.q(), RATE_TIMA_3.q(), RUBY_TIMA_4.q(), RAGE_TIMA_5.q(), PEDA_TIMA_6.q(), NUGA_TIMA_7.q());
}

/*
void set_tima(uint8_t x) {
REGA_TIMA_0.set2(x & 0x01); POVY_TIMA_1.set2(x & 0x02); PERU_TIMA_2.set2(x & 0x04); RATE_TIMA_3.set2(x & 0x08);
RUBY_TIMA_4.set2(x & 0x10); RAGE_TIMA_5.set2(x & 0x20); PEDA_TIMA_6.set2(x & 0x40); NUGA_TIMA_7.set2(x & 0x80);
}
*/

wire get_tima_max() { return NYDU_TIMA_MAX.q(); }
wire get_int_timer() { return MOBA_INT_TIMERp.q(); }

int get_tma() const {
  return pack(SABU_TMA_0.q(), NYKE_TMA_1.q(), MURU_TMA_2.q(), TYVA_TMA_3.q(), TYRU_TMA_4.q(), SUFY_TMA_5.q(), PETO_TMA_6.q(), SETA_TMA_7.q());
}

/*
void set_tma(uint8_t x) {
SABU_TMA_0.set2(x & 0x01); NYKE_TMA_1.set2(x & 0x02); MURU_TMA_2.set2(x & 0x04); TYVA_TMA_3.set2(x & 0x08);
TYRU_TMA_4.set2(x & 0x10); SUFY_TMA_5.set2(x & 0x20); PETO_TMA_6.set2(x & 0x40); SETA_TMA_7.set2(x & 0x80);
}
*/

int get_tac() const {
  return pack(SOPU_TAC_0.q(), SAMY_TAC_1.q(), SABO_TAC_2.q());
}

/*
void set_tac(uint8_t x) {
SOPU_TAC_0.set2(x & 1);
SAMY_TAC_1.set2(x & 2);
SABO_TAC_2.set2(x & 4);
}
*/

#endif
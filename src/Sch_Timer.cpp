#include "Sch_Timer.h"

#include "Sch_Top.h"

using namespace Schematics;

//------------------------------------------------------------------------------

void TimerRegisters::tick(SchematicTop& top) {
  /*p07.TEDO*/ wire TEDO_CPU_RD = not(top.UJYV_CPU_RD());
  /*p07.TAPU*/ wire TAPU_CPU_WR_xxxxxFGH = not(top.UBAL_CPU_WRp_ABCDExxx());

  /*p07.TUNA*/ wire TUNA_0000_FDFFp = nand(top.CPU_PIN_A15, top.CPU_PIN_A14, top.CPU_PIN_A13, top.CPU_PIN_A12, top.CPU_PIN_A11, top.CPU_PIN_A10, top.CPU_PIN_A09);
  /*p07.TONA*/ wire TONA_A08n = not(top.CPU_PIN_A08);
  /*p06.SARE*/ wire SARE_XX00_XX07p = nor(top.CPU_PIN_A07, top.CPU_PIN_A06, top.CPU_PIN_A05, top.CPU_PIN_A04, top.CPU_PIN_A03);
  /*p07.SYKE*/ wire SYKE_FF00_FFFFp = nor(TUNA_0000_FDFFp, TONA_A08n);
  /*p03.RYFO*/ wire RYFO_FF04_FF07p = and (top.CPU_PIN_A02, SARE_XX00_XX07p, SYKE_FF00_FFFFp);
  /*p03.TOVY*/ wire TOVY_A00n = not(top.CPU_PIN_A00);
  /*p08.TOLA*/ wire TOLA_A01n = not(top.CPU_PIN_A01);

  /*p01.TAGY*/ wire TAGY_FF04_RD = and (TEDO_CPU_RD, RYFO_FF04_FF07p, TOLA_A01n,       TOVY_A00n);
  /*p03.TEDA*/ wire TEDA_FF05_RD = and (TEDO_CPU_RD, RYFO_FF04_FF07p, TOLA_A01n,       top.CPU_PIN_A00);
  /*p03.TUBY*/ wire TUBY_FF06_RD = and (TEDO_CPU_RD, RYFO_FF04_FF07p, top.CPU_PIN_A01, TOVY_A00n);
  /*p03.SORA*/ wire SORA_FF07_RD = and (TEDO_CPU_RD, RYFO_FF04_FF07p, top.CPU_PIN_A01, top.CPU_PIN_A00);

  /*p01.TAPE*/ wire TAPE_FF04_WR =  and(TAPU_CPU_WR_xxxxxFGH, RYFO_FF04_FF07p, TOLA_A01n,       TOVY_A00n);
  /*p03.TOPE*/ wire TOPE_FF05_WRn = nand(TAPU_CPU_WR_xxxxxFGH, RYFO_FF04_FF07p, TOLA_A01n,       top.CPU_PIN_A00);
  /*p03.TYJU*/ wire TYJU_FF06_WR = nand(TAPU_CPU_WR_xxxxxFGH, RYFO_FF04_FF07p, top.CPU_PIN_A01, TOVY_A00n);
  /*p03.SARA*/ wire SARA_FF07_WR = nand(TAPU_CPU_WR_xxxxxFGH, RYFO_FF04_FF07p, top.CPU_PIN_A01, top.CPU_PIN_A00);

  // FF04 DIV
  {
    /*p01.UCOB*/ wire UCOB_CLKBAD = not(top.SYS_PIN_CLK_A);
    /*p01.UFOL*/ wire UFOL_DIV_RSTn = nor(UCOB_CLKBAD, top.SYS_PIN_RSTn, TAPE_FF04_WR);

    /*p01.BALY*/ wire BALY_xBxxxxxx = not(top.BYJU_AxCDEFGH());
    /*p01.BOGA*/ wire BOGA_AxCDEFGH = not(BALY_xBxxxxxx);
    /*p01.UKUP*/ UKUP_DIV_00.set(BOGA_AxCDEFGH,    UFOL_DIV_RSTn, !UKUP_DIV_00.q());
    /*p01.UFOR*/ UFOR_DIV_01.set(!UKUP_DIV_00.q(), UFOL_DIV_RSTn, !UFOR_DIV_01.q());
    /*p01.UNER*/ UNER_DIV_02.set(!UFOR_DIV_01.q(), UFOL_DIV_RSTn, !UNER_DIV_02.q());
    
    // TERO second from bottom rung - feedback, so this is QN
    // TERO bottom rung             - UVYR, so this is Q
    
    /*p01.TERO*/ TERO_DIV_03.set(!UNER_DIV_02.q(), UFOL_DIV_RSTn, !TERO_DIV_03.q());
    /*p01.UNYK*/ UNYK_DIV_04.set(!TERO_DIV_03.q(), UFOL_DIV_RSTn, !UNYK_DIV_04.q());
    /*p01.TAMA*/ TAMA_DIV_05.set(!UNYK_DIV_04.q(), UFOL_DIV_RSTn, !TAMA_DIV_05.q());

    ///*p01.ULUR*/ wire DIV_06_CLK = mux2(BOGA_AxCDEFGH, DIV_05, FF60_1);
    /*p01.ULUR*/ wire _DIV_06_CLK = TAMA_DIV_05.q();
    /*p01.UGOT*/ UGOT_DIV_06.set(!_DIV_06_CLK, UFOL_DIV_RSTn, !UGOT_DIV_06.q());
    /*p01.TULU*/ TULU_DIV_07.set(!UGOT_DIV_06.q(), UFOL_DIV_RSTn, !TULU_DIV_07.q());
    /*p01.TUGO*/ TUGO_DIV_08.set(!TULU_DIV_07.q(), UFOL_DIV_RSTn, !TUGO_DIV_08.q());
    /*p01.TOFE*/ TOFE_DIV_09.set(!TUGO_DIV_08.q(), UFOL_DIV_RSTn, !TOFE_DIV_09.q());
    /*p01.TERU*/ TERU_DIV_10.set(!TOFE_DIV_09.q(), UFOL_DIV_RSTn, !TERU_DIV_10.q());
    /*p01.SOLA*/ SOLA_DIV_11.set(!TERU_DIV_10.q(), UFOL_DIV_RSTn, !SOLA_DIV_11.q());
    /*p01.SUBU*/ SUBU_DIV_12.set(!SOLA_DIV_11.q(), UFOL_DIV_RSTn, !SUBU_DIV_12.q());
    /*p01.TEKA*/ TEKA_DIV_13.set(!SUBU_DIV_12.q(), UFOL_DIV_RSTn, !TEKA_DIV_13.q());
    /*p01.UKET*/ UKET_DIV_14.set(!TEKA_DIV_13.q(), UFOL_DIV_RSTn, !UKET_DIV_14.q());
    /*p01.UPOF*/ UPOF_DIV_15.set(!UKET_DIV_14.q(), UFOL_DIV_RSTn, !UPOF_DIV_15.q());


    /*p01.UTOK*/ wire DIV_08n = not(TUGO_DIV_08.q());
    /*p01.SAPY*/ wire DIV_09n = not(TOFE_DIV_09.q());
    /*p01.UMER*/ wire DIV_10n = not(TERU_DIV_10.q());
    /*p01.RAVE*/ wire DIV_11n = not(SOLA_DIV_11.q());
    /*p01.RYSO*/ wire DIV_12n = not(SUBU_DIV_12.q());
    /*p01.UDOR*/ wire DIV_13n = not(TEKA_DIV_13.q());
    /*p01.TAWU*/ top.CPU_TRI_D0.set_tribuf(TAGY_FF04_RD, not(top.UMEK_DIV_06n()));
    /*p01.TAKU*/ top.CPU_TRI_D1.set_tribuf(TAGY_FF04_RD, not(top.UREK_DIV_07n()));
    /*p01.TEMU*/ top.CPU_TRI_D2.set_tribuf(TAGY_FF04_RD, not(DIV_08n));
    /*p01.TUSE*/ top.CPU_TRI_D3.set_tribuf(TAGY_FF04_RD, not(DIV_09n));
    /*p01.UPUG*/ top.CPU_TRI_D4.set_tribuf(TAGY_FF04_RD, not(DIV_10n));
    /*p01.SEPU*/ top.CPU_TRI_D5.set_tribuf(TAGY_FF04_RD, not(DIV_11n));
    /*p01.SAWA*/ top.CPU_TRI_D6.set_tribuf(TAGY_FF04_RD, not(DIV_12n));
    /*p01.TATU*/ top.CPU_TRI_D7.set_tribuf(TAGY_FF04_RD, not(DIV_13n));
  }

  // FF05 TIMA
  {
    /*p03.MEKE*/ wire MEKE_INT_TIMERn = not(MOBA_INT_TIMERp.q());
    /*p01.ALUR*/ wire ALUR_RSTn = not(top.AVOR_RSTp());
    /*p03.UBOT*/ wire CLK_256Kn = not(UFOR_DIV_01.q());
    /*p03.UVYR*/ wire CLK_64Kn = not(TERO_DIV_03.q());

    /*p03.UKAP*/ wire CLK_MUXa = mux2_n(top.UVYN_DIV_05n(), CLK_64Kn, SOPU_TAC_0.q());
    /*p03.TEKO*/ wire CLK_MUXb = mux2_n(CLK_256Kn, top.UREK_DIV_07n(), SOPU_TAC_0.q());
    /*p03.TECY*/ wire CLK_MUXc = mux2_n(CLK_MUXa, CLK_MUXb, SAMY_TAC_1.q());

    /*p03.SOGU*/ wire SOGU_TIMA_CLK = nor(CLK_MUXc, SABO_TAC_2.qn());
    /*p03.ROKE*/ wire ROKE_TIMA_MUX_0 = mux2_n(SABU_TMA_0.q(), top.CPU_TRI_D0, TOPE_FF05_WRn); // suggests WRn
    /*p03.PETU*/ wire PETU_TIMA_MUX_1 = mux2_n(NYKE_TMA_1.q(), top.CPU_TRI_D1, TOPE_FF05_WRn);
    /*p03.NYKU*/ wire NYKU_TIMA_MUX_2 = mux2_n(MURU_TMA_2.q(), top.CPU_TRI_D2, TOPE_FF05_WRn); // die annotation box wrong, this is a 5-rung
    /*p03.SOCE*/ wire SOCE_TIMA_MUX_3 = mux2_n(TYVA_TMA_3.q(), top.CPU_TRI_D3, TOPE_FF05_WRn);
    /*p03.SALA*/ wire SALA_TIMA_MUX_4 = mux2_n(TYRU_TMA_4.q(), top.CPU_TRI_D4, TOPE_FF05_WRn);
    /*p03.SYRU*/ wire SYRU_TIMA_MUX_5 = mux2_n(SUFY_TMA_5.q(), top.CPU_TRI_D5, TOPE_FF05_WRn);
    /*p03.REFU*/ wire REFU_TIMA_MUX_6 = mux2_n(PETO_TMA_6.q(), top.CPU_TRI_D6, TOPE_FF05_WRn);
    /*p03.RATO*/ wire RATO_TIMA_MUX_7 = mux2_n(SETA_TMA_7.q(), top.CPU_TRI_D7, TOPE_FF05_WRn);

    /*p03.MULO*/ wire MULO_TIMA_RST = not(ALUR_RSTn);
    /*p03.PUXY*/ wire PUXY_TIMA_LD_0 = nor(MULO_TIMA_RST, ROKE_TIMA_MUX_0);
    /*p03.NERO*/ wire NERO_TIMA_LD_1 = nor(MULO_TIMA_RST, PETU_TIMA_MUX_1);
    /*p03.NADA*/ wire NADA_TIMA_LD_2 = nor(MULO_TIMA_RST, NYKU_TIMA_MUX_2);
    /*p03.REPA*/ wire REPA_TIMA_LD_3 = nor(MULO_TIMA_RST, SOCE_TIMA_MUX_3);
    /*p03.ROLU*/ wire ROLU_TIMA_LD_4 = nor(MULO_TIMA_RST, SALA_TIMA_MUX_4);
    /*p03.RUGY*/ wire RUGY_TIMA_LD_5 = nor(MULO_TIMA_RST, SYRU_TIMA_MUX_5);
    /*p03.PYMA*/ wire PYMA_TIMA_LD_6 = nor(MULO_TIMA_RST, REFU_TIMA_MUX_6);
    /*p03.PAGU*/ wire PAGU_TIMA_LD_7 = nor(MULO_TIMA_RST, RATO_TIMA_MUX_7);

    /*p03.MUZU*/ wire MUZU_TIMA_LOADn = or(top.CPU_PIN5, TOPE_FF05_WRn); // suggests CPU_PIN5 = DATA_VALIDn
    /*p03.MEXU*/ wire MEXU_TIMA_LOADp = nand(MUZU_TIMA_LOADn, ALUR_RSTn, MEKE_INT_TIMERn);

    /*p03.REGA*/ REGA_TIMA_0.clk_n(SOGU_TIMA_CLK,   MEXU_TIMA_LOADp, PUXY_TIMA_LD_0);
    /*p03.POVY*/ POVY_TIMA_1.clk_n(REGA_TIMA_0.q(), MEXU_TIMA_LOADp, NERO_TIMA_LD_1);
    /*p03.PERU*/ PERU_TIMA_2.clk_n(POVY_TIMA_1.q(), MEXU_TIMA_LOADp, NADA_TIMA_LD_2);
    /*p03.RATE*/ RATE_TIMA_3.clk_n(PERU_TIMA_2.q(), MEXU_TIMA_LOADp, REPA_TIMA_LD_3);
    /*p03.RUBY*/ RUBY_TIMA_4.clk_n(RATE_TIMA_3.q(), MEXU_TIMA_LOADp, ROLU_TIMA_LD_4);
    /*p03.RAGE*/ RAGE_TIMA_5.clk_n(RUBY_TIMA_4.q(), MEXU_TIMA_LOADp, RUGY_TIMA_LD_5);
    /*p03.PEDA*/ PEDA_TIMA_6.clk_n(RAGE_TIMA_5.q(), MEXU_TIMA_LOADp, PYMA_TIMA_LD_6);
    /*p03.NUGA*/ NUGA_TIMA_7.clk_n(PEDA_TIMA_6.q(), MEXU_TIMA_LOADp, PAGU_TIMA_LD_7);

    /*p03.SOKU*/ top.CPU_TRI_D0.set_tribuf(TEDA_FF05_RD, REGA_TIMA_0.q());
    /*p03.RACY*/ top.CPU_TRI_D1.set_tribuf(TEDA_FF05_RD, POVY_TIMA_1.q());
    /*p03.RAVY*/ top.CPU_TRI_D2.set_tribuf(TEDA_FF05_RD, PERU_TIMA_2.q());
    /*p03.SOSY*/ top.CPU_TRI_D3.set_tribuf(TEDA_FF05_RD, RATE_TIMA_3.q());
    /*p03.SOMU*/ top.CPU_TRI_D4.set_tribuf(TEDA_FF05_RD, RUBY_TIMA_4.q());
    /*p03.SURO*/ top.CPU_TRI_D5.set_tribuf(TEDA_FF05_RD, RAGE_TIMA_5.q());
    /*p03.ROWU*/ top.CPU_TRI_D6.set_tribuf(TEDA_FF05_RD, PEDA_TIMA_6.q());
    /*p03.PUSO*/ top.CPU_TRI_D7.set_tribuf(TEDA_FF05_RD, NUGA_TIMA_7.q());

    /*p03.MUGY*/ wire MUGY_TIMA_MAX_RSTn = not(MEXU_TIMA_LOADp);

    /*p01.BALY*/ wire BALY_xBxxxxxx = not(top.BYJU_AxCDEFGH());
    /*p01.BOGA*/ wire BOGA_AxCDEFGH = not(BALY_xBxxxxxx);

    /*p03.NYDU*/ NYDU_TIMA_MAX.set(BOGA_AxCDEFGH, MUGY_TIMA_MAX_RSTn, NUGA_TIMA_7.q());
  }

  {
    /*p01.ALUR*/ wire ALUR_RSTn = not(top.AVOR_RSTp());
    /*p03.MERY*/ wire MERY_INT_TIMER_IN = nor(!NYDU_TIMA_MAX.q(), NUGA_TIMA_7.q());

    /*p01.BALY*/ wire BALY_xBxxxxxx = not(top.BYJU_AxCDEFGH());
    /*p01.BOGA*/ wire BOGA_AxCDEFGH = not(BALY_xBxxxxxx);
    /*p03.MOBA*/ MOBA_INT_TIMERp.set(BOGA_AxCDEFGH, ALUR_RSTn, MERY_INT_TIMER_IN);
  }

  // FF06 TMA
  {
    /*p01.ALUR*/ wire ALUR_RSTn = not(top.AVOR_RSTp());
    /*p03.SABU*/ SABU_TMA_0.set(TYJU_FF06_WR, ALUR_RSTn, top.CPU_TRI_D0);
    /*p03.NYKE*/ NYKE_TMA_1.set(TYJU_FF06_WR, ALUR_RSTn, top.CPU_TRI_D1);
    /*p03.MURU*/ MURU_TMA_2.set(TYJU_FF06_WR, ALUR_RSTn, top.CPU_TRI_D2);
    /*p03.TYVA*/ TYVA_TMA_3.set(TYJU_FF06_WR, ALUR_RSTn, top.CPU_TRI_D3);
    /*p03.TYRU*/ TYRU_TMA_4.set(TYJU_FF06_WR, ALUR_RSTn, top.CPU_TRI_D4);
    /*p03.SUFY*/ SUFY_TMA_5.set(TYJU_FF06_WR, ALUR_RSTn, top.CPU_TRI_D5);
    /*p03.PETO*/ PETO_TMA_6.set(TYJU_FF06_WR, ALUR_RSTn, top.CPU_TRI_D6);
    /*p03.SETA*/ SETA_TMA_7.set(TYJU_FF06_WR, ALUR_RSTn, top.CPU_TRI_D7);

    /*p03.SETE*/ top.CPU_TRI_D0.set_tribuf(TUBY_FF06_RD, SABU_TMA_0.q());
    /*p03.PYRE*/ top.CPU_TRI_D1.set_tribuf(TUBY_FF06_RD, NYKE_TMA_1.q());
    /*p03.NOLA*/ top.CPU_TRI_D2.set_tribuf(TUBY_FF06_RD, MURU_TMA_2.q());
    /*p03.SALU*/ top.CPU_TRI_D3.set_tribuf(TUBY_FF06_RD, TYVA_TMA_3.q());
    /*p03.SUPO*/ top.CPU_TRI_D4.set_tribuf(TUBY_FF06_RD, TYRU_TMA_4.q());
    /*p03.SOTU*/ top.CPU_TRI_D5.set_tribuf(TUBY_FF06_RD, SUFY_TMA_5.q());
    /*p03.REVA*/ top.CPU_TRI_D6.set_tribuf(TUBY_FF06_RD, PETO_TMA_6.q());
    /*p03.SAPU*/ top.CPU_TRI_D7.set_tribuf(TUBY_FF06_RD, SETA_TMA_7.q());
  }

  // FF07 TAC
  {
    /*p01.ALUR*/ wire ALUR_RSTn = not(top.AVOR_RSTp());
    /*p03.SOPU*/ SOPU_TAC_0.set(SARA_FF07_WR, ALUR_RSTn, top.CPU_TRI_D0);
    /*p03.SAMY*/ SAMY_TAC_1.set(SARA_FF07_WR, ALUR_RSTn, top.CPU_TRI_D1);
    /*p03.SABO*/ SABO_TAC_2.set(SARA_FF07_WR, ALUR_RSTn, top.CPU_TRI_D2);

    /*p03.RYLA*/ top.CPU_TRI_D0.set_tribuf(SORA_FF07_RD, SOPU_TAC_0.q());
    /*p03.ROTE*/ top.CPU_TRI_D1.set_tribuf(SORA_FF07_RD, SAMY_TAC_1.q());
    /*p03.SUPE*/ top.CPU_TRI_D2.set_tribuf(SORA_FF07_RD, SABO_TAC_2.q());
  }
}

//------------------------------------------------------------------------------

SignalHash TimerRegisters::commit() {
  SignalHash hash;
  /*p01.UKUP*/ hash << UKUP_DIV_00.commit_reg();
  /*p01.UFOR*/ hash << UFOR_DIV_01.commit_reg();
  /*p01.UNER*/ hash << UNER_DIV_02.commit_reg();
  /*p01.TERO*/ hash << TERO_DIV_03.commit_reg();
  /*p01.UNYK*/ hash << UNYK_DIV_04.commit_reg();
  /*p01.TAMA*/ hash << TAMA_DIV_05.commit_reg();
  /*p01.UGOT*/ hash << UGOT_DIV_06.commit_reg();
  /*p01.TULU*/ hash << TULU_DIV_07.commit_reg();
  /*p01.TUGO*/ hash << TUGO_DIV_08.commit_reg();
  /*p01.TOFE*/ hash << TOFE_DIV_09.commit_reg();
  /*p01.TERU*/ hash << TERU_DIV_10.commit_reg();
  /*p01.SOLA*/ hash << SOLA_DIV_11.commit_reg();
  /*p01.SUBU*/ hash << SUBU_DIV_12.commit_reg();
  /*p01.TEKA*/ hash << TEKA_DIV_13.commit_reg();
  /*p01.UKET*/ hash << UKET_DIV_14.commit_reg();
  /*p01.UPOF*/ hash << UPOF_DIV_15.commit_reg();
  /*p03.REGA*/ hash << REGA_TIMA_0.commit_counter();
  /*p03.POVY*/ hash << POVY_TIMA_1.commit_counter();
  /*p03.PERU*/ hash << PERU_TIMA_2.commit_counter();
  /*p03.RATE*/ hash << RATE_TIMA_3.commit_counter();
  /*p03.RUBY*/ hash << RUBY_TIMA_4.commit_counter();
  /*p03.RAGE*/ hash << RAGE_TIMA_5.commit_counter();
  /*p03.PEDA*/ hash << PEDA_TIMA_6.commit_counter();
  /*p03.NUGA*/ hash << NUGA_TIMA_7.commit_counter();
  /*p03.NYDU*/ hash << NYDU_TIMA_MAX.commit_reg();
  /*p03.MOBA*/ hash << MOBA_INT_TIMERp.commit_reg();
  /*p03.SABU*/ hash << SABU_TMA_0.commit_reg();
  /*p03.NYKE*/ hash << NYKE_TMA_1.commit_reg();
  /*p03.MURU*/ hash << MURU_TMA_2.commit_reg();
  /*p03.TYVA*/ hash << TYVA_TMA_3.commit_reg();
  /*p03.TYRU*/ hash << TYRU_TMA_4.commit_reg();
  /*p03.SUFY*/ hash << SUFY_TMA_5.commit_reg();
  /*p03.PETO*/ hash << PETO_TMA_6.commit_reg();
  /*p03.SETA*/ hash << SETA_TMA_7.commit_reg();
  /*p03.SOPU*/ hash << SOPU_TAC_0.commit_reg();
  /*p03.SAMY*/ hash << SAMY_TAC_1.commit_reg();
  /*p03.SABO*/ hash << SABO_TAC_2.commit_reg();
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
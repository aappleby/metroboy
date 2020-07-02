#include "Sch_Timer.h"

#include "Sch_Top.h"

using namespace Schematics;

//------------------------------------------------------------------------------

TimerSignals TimerRegisters::sig() const {
  TimerSignals sig;

  sig.UVYN_DIV_05n    = not(TAMA_DIV_05);
  sig.UMEK_DIV_06n    = not(UGOT_DIV_06);
  sig.UREK_DIV_07n    = not(TULU_DIV_07);
  sig.UPOF_DIV_15     = UPOF_DIV_15;
  sig.MOBA_INT_TIMERp = MOBA_INT_TIMERp;

  return sig;
}

//------------------------------------------------------------------------------

void TimerRegisters::tick(SchematicTop& gb){
  tick(
    gb.clk_reg.sig(gb),
    gb.rst_reg.sig(gb),
    gb.cpu_bus.sig(gb),
    gb.cpu_bus,
    gb.EXT_PIN_CLK_GOOD,
    gb.EXT_PIN_RST);

}

void TimerRegisters::tick(
  const ClockSignals& clk_sig,
  const ResetSignals& rst_sig,
  const CpuBusSignals& cpu_sig,
  CpuBus& cpu_bus,
  bool EXT_PIN_CLK_GOOD,
  bool EXT_PIN_RST) {
  auto tim_sig = sig();

  // FF04 DIV
  {
    /*p01.TAPE*/ wire FF04_WR = and(cpu_sig.TAPU_CPU_WR_xxxxxFGH, cpu_sig.RYFO_FF04_FF07p, cpu_sig.TOLA_A01n, cpu_sig.TOVY_A00n);
    /*p01.UCOB*/ wire UCOB_CLKBAD = not(EXT_PIN_CLK_GOOD);
    /*p01.UFOL*/ wire DIV_RSTn = nor(UCOB_CLKBAD, EXT_PIN_RST, FF04_WR);

    /*p01.UKUP*/ UKUP_DIV_00.set(clk_sig.BOGA_AxCDEFGH, DIV_RSTn, !UKUP_DIV_00.q());
    /*p01.UFOR*/ UFOR_DIV_01.set(!UKUP_DIV_00.q(), DIV_RSTn, !UFOR_DIV_01.q());
    /*p01.UNER*/ UNER_DIV_02.set(!UFOR_DIV_01.q(), DIV_RSTn, !UNER_DIV_02.q());
    
    // TERO second from bottom rung - feedback, so this is QN
    // TERO bottom rung             - UVYR, so this is Q
    
    /*p01.TERO*/ TERO_DIV_03.set(!UNER_DIV_02.q(), DIV_RSTn, !TERO_DIV_03.q());
    /*p01.UNYK*/ UNYK_DIV_04.set(!TERO_DIV_03.q(), DIV_RSTn, !UNYK_DIV_04.q());
    /*p01.TAMA*/ TAMA_DIV_05.set(!UNYK_DIV_04.q(), DIV_RSTn, !TAMA_DIV_05.q());

    ///*p01.ULUR*/ wire DIV_06_CLK = mux2(BOGA_AxCDEFGH, DIV_05, FF60_1);
    /*p01.ULUR*/ wire _DIV_06_CLK = TAMA_DIV_05.q();
    /*p01.UGOT*/ UGOT_DIV_06.set(!_DIV_06_CLK, DIV_RSTn, !UGOT_DIV_06.q());
    /*p01.TULU*/ TULU_DIV_07.set(!UGOT_DIV_06.q(), DIV_RSTn, !TULU_DIV_07.q());
    /*p01.TUGO*/ TUGO_DIV_08.set(!TULU_DIV_07.q(), DIV_RSTn, !TUGO_DIV_08.q());
    /*p01.TOFE*/ TOFE_DIV_09.set(!TUGO_DIV_08.q(), DIV_RSTn, !TOFE_DIV_09.q());
    /*p01.TERU*/ TERU_DIV_10.set(!TOFE_DIV_09.q(), DIV_RSTn, !TERU_DIV_10.q());
    /*p01.SOLA*/ SOLA_DIV_11.set(!TERU_DIV_10.q(), DIV_RSTn, !SOLA_DIV_11.q());
    /*p01.SUBU*/ SUBU_DIV_12.set(!SOLA_DIV_11.q(), DIV_RSTn, !SUBU_DIV_12.q());
    /*p01.TEKA*/ TEKA_DIV_13.set(!SUBU_DIV_12.q(), DIV_RSTn, !TEKA_DIV_13.q());
    /*p01.UKET*/ UKET_DIV_14.set(!TEKA_DIV_13.q(), DIV_RSTn, !UKET_DIV_14.q());
    /*p01.UPOF*/ UPOF_DIV_15.set(!UKET_DIV_14.q(), DIV_RSTn, !UPOF_DIV_15.q());

    /*p01.TAGY*/ wire FF04_RD = and (cpu_sig.TEDO_CPU_RD, cpu_sig.RYFO_FF04_FF07p, cpu_sig.TOLA_A01n, cpu_sig.TOVY_A00n);

    /*p01.UTOK*/ wire DIV_08n = not(TUGO_DIV_08.q());
    /*p01.SAPY*/ wire DIV_09n = not(TOFE_DIV_09.q());
    /*p01.UMER*/ wire DIV_10n = not(TERU_DIV_10.q());
    /*p01.RAVE*/ wire DIV_11n = not(SOLA_DIV_11.q());
    /*p01.RYSO*/ wire DIV_12n = not(SUBU_DIV_12.q());
    /*p01.UDOR*/ wire DIV_13n = not(TEKA_DIV_13.q());
    /*p01.TAWU*/ cpu_bus.TRI_D0.set_tribuf(FF04_RD, not(tim_sig.UMEK_DIV_06n));
    /*p01.TAKU*/ cpu_bus.TRI_D1.set_tribuf(FF04_RD, not(tim_sig.UREK_DIV_07n));
    /*p01.TEMU*/ cpu_bus.TRI_D2.set_tribuf(FF04_RD, not(DIV_08n));
    /*p01.TUSE*/ cpu_bus.TRI_D3.set_tribuf(FF04_RD, not(DIV_09n));
    /*p01.UPUG*/ cpu_bus.TRI_D4.set_tribuf(FF04_RD, not(DIV_10n));
    /*p01.SEPU*/ cpu_bus.TRI_D5.set_tribuf(FF04_RD, not(DIV_11n));
    /*p01.SAWA*/ cpu_bus.TRI_D6.set_tribuf(FF04_RD, not(DIV_12n));
    /*p01.TATU*/ cpu_bus.TRI_D7.set_tribuf(FF04_RD, not(DIV_13n));
  }

  /*p03.TOPE*/ wire TOPE_FF05_WRn = nand(cpu_sig.TAPU_CPU_WR_xxxxxFGH, cpu_sig.RYFO_FF04_FF07p, cpu_sig.TOLA_A01n, cpu_bus.PIN_A00);
  /*p03.MUZU*/ wire MUZU = or(cpu_bus.PIN_FROM_CPU5p.q(), TOPE_FF05_WRn);
  /*p03.MEKE*/ wire MEKE_INT_TIMERn = not(MOBA_INT_TIMERp.q());

  /*p01.ALUR*/ wire ALUR_RSTn = not(rst_sig.AVOR_RSTp);   // this goes all over the place
  /*p03.MEXU*/ wire MEXU_TIMA_LOAD = nand(MUZU, ALUR_RSTn, MEKE_INT_TIMERn);

  // FF05 TIMA
  {
    /*p03.UBOT*/ wire _CLK_256Kn = not(UFOR_DIV_01.q());
    /*p03.UVYR*/ wire _CLK_64Kn = not(TERO_DIV_03.q());

    /*p03.UKAP*/ wire _CLK_MUXa = mux2_n(tim_sig.UVYN_DIV_05n, _CLK_64Kn, TAC_0.q());
    /*p03.TEKO*/ wire _CLK_MUXb = mux2_n(_CLK_256Kn, tim_sig.UREK_DIV_07n, TAC_0.q());
    /*p03.TECY*/ wire _CLK_MUXc = mux2_n(_CLK_MUXa, _CLK_MUXb, TAC_1.q());

    /*p03.SOGU*/ wire _TIMA_CLK = nor(_CLK_MUXc, TAC_2.qn());
    /*p03.MULO*/ wire _TIMA_RST = not(ALUR_RSTn);
    /*p03.ROKE*/ wire _TIMA_MUX_0 = mux2_n(TMA_0.q(), cpu_bus.TRI_D0, TOPE_FF05_WRn);
    /*p03.PETU*/ wire _TIMA_MUX_1 = mux2_n(TMA_1.q(), cpu_bus.TRI_D1, TOPE_FF05_WRn);
    /*p03.NYKU*/ wire _TIMA_MUX_2 = mux2_n(TMA_2.q(), cpu_bus.TRI_D2, TOPE_FF05_WRn); // die annotation box wrong, this is a 5-rung
    /*p03.SOCE*/ wire _TIMA_MUX_3 = mux2_n(TMA_3.q(), cpu_bus.TRI_D3, TOPE_FF05_WRn);
    /*p03.SALA*/ wire _TIMA_MUX_4 = mux2_n(TMA_4.q(), cpu_bus.TRI_D4, TOPE_FF05_WRn);
    /*p03.SYRU*/ wire _TIMA_MUX_5 = mux2_n(TMA_5.q(), cpu_bus.TRI_D5, TOPE_FF05_WRn);
    /*p03.REFU*/ wire _TIMA_MUX_6 = mux2_n(TMA_6.q(), cpu_bus.TRI_D6, TOPE_FF05_WRn);
    /*p03.RATO*/ wire _TIMA_MUX_7 = mux2_n(TMA_7.q(), cpu_bus.TRI_D7, TOPE_FF05_WRn);
    /*p03.PUXY*/ wire _TIMA_LD_0 = nor(_TIMA_RST, _TIMA_MUX_0);
    /*p03.NERO*/ wire _TIMA_LD_1 = nor(_TIMA_RST, _TIMA_MUX_1);
    /*p03.NADA*/ wire _TIMA_LD_2 = nor(_TIMA_RST, _TIMA_MUX_2);
    /*p03.REPA*/ wire _TIMA_LD_3 = nor(_TIMA_RST, _TIMA_MUX_3);
    /*p03.ROLU*/ wire _TIMA_LD_4 = nor(_TIMA_RST, _TIMA_MUX_4);
    /*p03.RUGY*/ wire _TIMA_LD_5 = nor(_TIMA_RST, _TIMA_MUX_5);
    /*p03.PYMA*/ wire _TIMA_LD_6 = nor(_TIMA_RST, _TIMA_MUX_6);
    /*p03.PAGU*/ wire _TIMA_LD_7 = nor(_TIMA_RST, _TIMA_MUX_7);
    /*p03.REGA*/ TIMA_0.clk_n(_TIMA_CLK,  MEXU_TIMA_LOAD, _TIMA_LD_0);
    /*p03.POVY*/ TIMA_1.clk_n(TIMA_0.q(), MEXU_TIMA_LOAD, _TIMA_LD_1);
    /*p03.PERU*/ TIMA_2.clk_n(TIMA_1.q(), MEXU_TIMA_LOAD, _TIMA_LD_2);
    /*p03.RATE*/ TIMA_3.clk_n(TIMA_2.q(), MEXU_TIMA_LOAD, _TIMA_LD_3);
    /*p03.RUBY*/ TIMA_4.clk_n(TIMA_3.q(), MEXU_TIMA_LOAD, _TIMA_LD_4);
    /*p03.RAGE*/ TIMA_5.clk_n(TIMA_4.q(), MEXU_TIMA_LOAD, _TIMA_LD_5);
    /*p03.PEDA*/ TIMA_6.clk_n(TIMA_5.q(), MEXU_TIMA_LOAD, _TIMA_LD_6);
    /*p03.NUGA*/ TIMA_7.clk_n(TIMA_6.q(), MEXU_TIMA_LOAD, _TIMA_LD_7);

    /*p03.TEDA*/ wire _TEDA_FF05_RD = and (cpu_sig.TEDO_CPU_RD, cpu_sig.RYFO_FF04_FF07p, cpu_sig.TOLA_A01n, cpu_bus.PIN_A00);
    /*p03.SOKU*/ cpu_bus.TRI_D0.set_tribuf(_TEDA_FF05_RD, TIMA_0.q());
    /*p03.RACY*/ cpu_bus.TRI_D1.set_tribuf(_TEDA_FF05_RD, TIMA_1.q());
    /*p03.RAVY*/ cpu_bus.TRI_D2.set_tribuf(_TEDA_FF05_RD, TIMA_2.q());
    /*p03.SOSY*/ cpu_bus.TRI_D3.set_tribuf(_TEDA_FF05_RD, TIMA_3.q());
    /*p03.SOMU*/ cpu_bus.TRI_D4.set_tribuf(_TEDA_FF05_RD, TIMA_4.q());
    /*p03.SURO*/ cpu_bus.TRI_D5.set_tribuf(_TEDA_FF05_RD, TIMA_5.q());
    /*p03.ROWU*/ cpu_bus.TRI_D6.set_tribuf(_TEDA_FF05_RD, TIMA_6.q());
    /*p03.PUSO*/ cpu_bus.TRI_D7.set_tribuf(_TEDA_FF05_RD, TIMA_7.q());
  }

  {
    /*p03.MUGY*/ wire _TIMA_LOADn = not(MEXU_TIMA_LOAD);
    /*p03.NYDU*/ TIMA_MAX.set(clk_sig.BOGA_AxCDEFGH, _TIMA_LOADn, TIMA_7.q());
  }

  {
    /*p03.MERY*/ wire _MERY_INT_TIMER_IN = nor(!TIMA_MAX.q(), TIMA_7.q());
    /*p03.MOBA*/ MOBA_INT_TIMERp.set(clk_sig.BOGA_AxCDEFGH, ALUR_RSTn, _MERY_INT_TIMER_IN);
  }

  // FF06 TMA
  {
    /*p03.TYJU*/ wire _TYJU_FF06_WRn = nand(cpu_sig.TAPU_CPU_WR_xxxxxFGH, cpu_sig.RYFO_FF04_FF07p, cpu_sig.TOVY_A00n, cpu_bus.PIN_A01);
    /*p03.SABU*/ TMA_0.set(_TYJU_FF06_WRn, ALUR_RSTn, cpu_bus.TRI_D0);
    /*p03.NYKE*/ TMA_1.set(_TYJU_FF06_WRn, ALUR_RSTn, cpu_bus.TRI_D1);
    /*p03.MURU*/ TMA_2.set(_TYJU_FF06_WRn, ALUR_RSTn, cpu_bus.TRI_D2);
    /*p03.TYVA*/ TMA_3.set(_TYJU_FF06_WRn, ALUR_RSTn, cpu_bus.TRI_D3);
    /*p03.TYRU*/ TMA_4.set(_TYJU_FF06_WRn, ALUR_RSTn, cpu_bus.TRI_D4);
    /*p03.SUFY*/ TMA_5.set(_TYJU_FF06_WRn, ALUR_RSTn, cpu_bus.TRI_D5);
    /*p03.PETO*/ TMA_6.set(_TYJU_FF06_WRn, ALUR_RSTn, cpu_bus.TRI_D6);
    /*p03.SETA*/ TMA_7.set(_TYJU_FF06_WRn, ALUR_RSTn, cpu_bus.TRI_D7);
    /*p03.TUBY*/ wire FF06_RD = and (cpu_sig.TEDO_CPU_RD, cpu_sig.RYFO_FF04_FF07p, cpu_bus.PIN_A01, cpu_sig.TOVY_A00n);
    /*p03.SETE*/ cpu_bus.TRI_D0.set_tribuf(FF06_RD, TMA_0.q());
    /*p03.PYRE*/ cpu_bus.TRI_D1.set_tribuf(FF06_RD, TMA_1.q());
    /*p03.NOLA*/ cpu_bus.TRI_D2.set_tribuf(FF06_RD, TMA_2.q());
    /*p03.SALU*/ cpu_bus.TRI_D3.set_tribuf(FF06_RD, TMA_3.q());
    /*p03.SUPO*/ cpu_bus.TRI_D4.set_tribuf(FF06_RD, TMA_4.q());
    /*p03.SOTU*/ cpu_bus.TRI_D5.set_tribuf(FF06_RD, TMA_5.q());
    /*p03.REVA*/ cpu_bus.TRI_D6.set_tribuf(FF06_RD, TMA_6.q());
    /*p03.SAPU*/ cpu_bus.TRI_D7.set_tribuf(FF06_RD, TMA_7.q());
  }

  // FF07 TAC
  {
    /*p03.SARA*/ wire _SARA_FF07_WRn = nand(cpu_sig.TAPU_CPU_WR_xxxxxFGH, cpu_sig.RYFO_FF04_FF07p, cpu_bus.PIN_A00, cpu_bus.PIN_A01);
    /*p03.SOPU*/ TAC_0.set(_SARA_FF07_WRn, ALUR_RSTn, cpu_bus.TRI_D0);
    /*p03.SAMY*/ TAC_1.set(_SARA_FF07_WRn, ALUR_RSTn, cpu_bus.TRI_D1);
    /*p03.SABO*/ TAC_2.set(_SARA_FF07_WRn, ALUR_RSTn, cpu_bus.TRI_D2);
    /*p03.SORA*/ wire FF07_RD = and (cpu_sig.TEDO_CPU_RD, cpu_sig.RYFO_FF04_FF07p, cpu_bus.PIN_A00, cpu_bus.PIN_A01);
    /*p03.RYLA*/ cpu_bus.TRI_D0.set_tribuf(FF07_RD, TAC_0.q());
    /*p03.ROTE*/ cpu_bus.TRI_D1.set_tribuf(FF07_RD, TAC_1.q());
    /*p03.SUPE*/ cpu_bus.TRI_D2.set_tribuf(FF07_RD, TAC_2.q());
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
  /*p03.REGA*/ hash << TIMA_0.commit_counter();
  /*p03.POVY*/ hash << TIMA_1.commit_counter();
  /*p03.PERU*/ hash << TIMA_2.commit_counter();
  /*p03.RATE*/ hash << TIMA_3.commit_counter();
  /*p03.RUBY*/ hash << TIMA_4.commit_counter();
  /*p03.RAGE*/ hash << TIMA_5.commit_counter();
  /*p03.PEDA*/ hash << TIMA_6.commit_counter();
  /*p03.NUGA*/ hash << TIMA_7.commit_counter();
  /*p03.NYDU*/ hash << TIMA_MAX.commit_reg();
  /*p03.MOBA*/ hash << MOBA_INT_TIMERp.commit_reg();
  /*p03.SABU*/ hash << TMA_0.commit_reg();
  /*p03.NYKE*/ hash << TMA_1.commit_reg();
  /*p03.MURU*/ hash << TMA_2.commit_reg();
  /*p03.TYVA*/ hash << TMA_3.commit_reg();
  /*p03.TYRU*/ hash << TMA_4.commit_reg();
  /*p03.SUFY*/ hash << TMA_5.commit_reg();
  /*p03.PETO*/ hash << TMA_6.commit_reg();
  /*p03.SETA*/ hash << TMA_7.commit_reg();
  /*p03.SOPU*/ hash << TAC_0.commit_reg();
  /*p03.SAMY*/ hash << TAC_1.commit_reg();
  /*p03.SABO*/ hash << TAC_2.commit_reg();
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

  TIMA_0.dump(text_painter, "TIMA_0    ");
  TIMA_1.dump(text_painter, "TIMA_1    ");
  TIMA_2.dump(text_painter, "TIMA_2    ");
  TIMA_3.dump(text_painter, "TIMA_3    ");
  TIMA_4.dump(text_painter, "TIMA_4    ");
  TIMA_5.dump(text_painter, "TIMA_5    ");
  TIMA_6.dump(text_painter, "TIMA_6    ");
  TIMA_7.dump(text_painter, "TIMA_7    ");
  text_painter.newline();

  TMA_0.dump(text_painter, "TMA_0     ");
  TMA_1.dump(text_painter, "TMA_1     ");
  TMA_2.dump(text_painter, "TMA_2     ");
  TMA_3.dump(text_painter, "TMA_3     ");
  TMA_4.dump(text_painter, "TMA_4     ");
  TMA_5.dump(text_painter, "TMA_5     ");
  TMA_6.dump(text_painter, "TMA_6     ");
  TMA_7.dump(text_painter, "TMA_7     ");
  text_painter.newline();

  TAC_0.dump(text_painter, "TAC_0     ");
  TAC_1.dump(text_painter, "TAC_1     ");
  TAC_2.dump(text_painter, "TAC_2     ");
  text_painter.newline();

  TIMA_MAX.dump(text_painter, "TIMA_MAX  ");
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
  return pack(TIMA_0.q(), TIMA_1.q(), TIMA_2.q(), TIMA_3.q(), TIMA_4.q(), TIMA_5.q(), TIMA_6.q(), TIMA_7.q());
}

/*
void set_tima(uint8_t x) {
TIMA_0.set2(x & 0x01); TIMA_1.set2(x & 0x02); TIMA_2.set2(x & 0x04); TIMA_3.set2(x & 0x08);
TIMA_4.set2(x & 0x10); TIMA_5.set2(x & 0x20); TIMA_6.set2(x & 0x40); TIMA_7.set2(x & 0x80);
}
*/

wire get_tima_max() { return TIMA_MAX.q(); }
wire get_int_timer() { return MOBA_INT_TIMERp.q(); }

int get_tma() const {
  return pack(TMA_0.q(), TMA_1.q(), TMA_2.q(), TMA_3.q(), TMA_4.q(), TMA_5.q(), TMA_6.q(), TMA_7.q());
}

/*
void set_tma(uint8_t x) {
TMA_0.set2(x & 0x01); TMA_1.set2(x & 0x02); TMA_2.set2(x & 0x04); TMA_3.set2(x & 0x08);
TMA_4.set2(x & 0x10); TMA_5.set2(x & 0x20); TMA_6.set2(x & 0x40); TMA_7.set2(x & 0x80);
}
*/

int get_tac() const {
  return pack(TAC_0.q(), TAC_1.q(), TAC_2.q());
}

/*
void set_tac(uint8_t x) {
TAC_0.set2(x & 1);
TAC_1.set2(x & 2);
TAC_2.set2(x & 4);
}
*/

#endif
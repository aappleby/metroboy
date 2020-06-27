#include "Sch_Timer.h"

#include "Sch_Common.h"
#include "Sch_Pins.h"
#include "TestGB.h"
#include "Constants.h"

using namespace Schematics;

TimerSignals TimerRegisters::sig(const TestGB& /*gb*/) const {
  return {
    .UVYN_DIV_05n = not(TAMA_DIV_05),
    .UMEK_DIV_06n = not(UGOT_DIV_06),
    .UREK_DIV_07n = not(TULU_DIV_07),
    .MOBA_INT_TIMERp = MOBA_INT_TIMERp,
  };
}

void TimerRegisters::tick(TestGB& gb) {
  auto& cpu_bus = gb.cpu_bus;
  auto& ext_bus = gb.ext_bus;

  auto clk_sig = gb.clk_reg.sig(gb);
  auto adr_sig = gb.adr_reg.sig(cpu_bus);
  auto rst_sig = gb.rst_reg.sig(gb);
  auto cpu_sig = gb.cpu_reg.sig(gb);
  auto tim_sig = sig(gb);

  // div
  {
    /*p01.TAPE*/ wire _FF04_WR = and(cpu_sig.TAPU_CPU_WR_xxxxxFGH, adr_sig.RYFO_FF04_FF07p, adr_sig.TOLA_A01n, adr_sig.TOVY_A00n);
    /*p01.UFOL*/ wire _DIV_RSTn = nor(rst_sig.UCOB_CLKBAD, ext_bus.PIN_RST, _FF04_WR);

    /*p01.UKUP*/ UKUP_DIV_00.set(clk_sig.PIN_BOGA_AxCDEFGH, _DIV_RSTn, !UKUP_DIV_00.q());
    /*p01.UFOR*/ UFOR_DIV_01.set(!UKUP_DIV_00.q(), _DIV_RSTn, !UFOR_DIV_01.q());
    /*p01.UNER*/ UNER_DIV_02.set(!UFOR_DIV_01.q(), _DIV_RSTn, !UNER_DIV_02.q());
    
    // TERO second from bottom rung - feedback, so this is QN
    // TERO bottom rung             - UVYR, so this is Q
    
    /*p01.TERO*/ TERO_DIV_03.set(!UNER_DIV_02.q(), _DIV_RSTn, !TERO_DIV_03.q());
    /*p01.UNYK*/ UNYK_DIV_04.set(!TERO_DIV_03.q(), _DIV_RSTn, !UNYK_DIV_04.q());
    /*p01.TAMA*/ TAMA_DIV_05.set(!UNYK_DIV_04.q(), _DIV_RSTn, !TAMA_DIV_05.q());

    ///*p01.ULUR*/ wire DIV_06_CLK = mux2(BOGA_AxCDEFGH, DIV_05, FF60_1);
    /*p01.ULUR*/ wire _DIV_06_CLK = TAMA_DIV_05.q();
    /*p01.UGOT*/ UGOT_DIV_06.set(!_DIV_06_CLK, _DIV_RSTn, !UGOT_DIV_06.q());
    /*p01.TULU*/ TULU_DIV_07.set(!UGOT_DIV_06.q(), _DIV_RSTn, !TULU_DIV_07.q());
    /*p01.TUGO*/ TUGO_DIV_08.set(!TULU_DIV_07.q(), _DIV_RSTn, !TUGO_DIV_08.q());
    /*p01.TOFE*/ TOFE_DIV_09.set(!TUGO_DIV_08.q(), _DIV_RSTn, !TOFE_DIV_09.q());
    /*p01.TERU*/ TERU_DIV_10.set(!TOFE_DIV_09.q(), _DIV_RSTn, !TERU_DIV_10.q());
    /*p01.SOLA*/ SOLA_DIV_11.set(!TERU_DIV_10.q(), _DIV_RSTn, !SOLA_DIV_11.q());
    /*p01.SUBU*/ SUBU_DIV_12.set(!SOLA_DIV_11.q(), _DIV_RSTn, !SUBU_DIV_12.q());
    /*p01.TEKA*/ TEKA_DIV_13.set(!SUBU_DIV_12.q(), _DIV_RSTn, !TEKA_DIV_13.q());
    /*p01.UKET*/ UKET_DIV_14.set(!TEKA_DIV_13.q(), _DIV_RSTn, !UKET_DIV_14.q());
    /*p01.UPOF*/ UPOF_DIV_15.set(!UKET_DIV_14.q(), _DIV_RSTn, !UPOF_DIV_15.q());
  }

  /*p03.TOPE*/ wire _FF05_WRn = nand(cpu_sig.TAPU_CPU_WR_xxxxxFGH, adr_sig.RYFO_FF04_FF07p, adr_sig.TOLA_A01n, cpu_bus.PIN_A00);
  /*p03.MUZU*/ wire _MUZU = or(cpu_bus.PIN_FROM_CPU5p.q(), _FF05_WRn);
  /*p03.MEKE*/ wire _INT_TIMERn = not(MOBA_INT_TIMERp.q());
  /*p03.MEXU*/ wire _TIMA_LOAD = nand(_MUZU, rst_sig.ALUR_RSTn, _INT_TIMERn);

  // sch_timer.tick_tima
  {
    /*p03.UBOT*/ wire _CLK_256Kn = not(UFOR_DIV_01.q());
    /*p03.UVYR*/ wire _CLK_64Kn = not(TERO_DIV_03.q());

    /*p03.UKAP*/ wire _CLK_MUXa = mux2_n(tim_sig.UVYN_DIV_05n, _CLK_64Kn, TAC_0.q());
    /*p03.TEKO*/ wire _CLK_MUXb = mux2_n(_CLK_256Kn, tim_sig.UREK_DIV_07n, TAC_0.q());
    /*p03.TECY*/ wire _CLK_MUXc = mux2_n(_CLK_MUXa, _CLK_MUXb, TAC_1.q());

    /*p03.SOGU*/ wire _TIMA_CLK = nor(_CLK_MUXc, !TAC_2.q());
    /*p03.MULO*/ wire _TIMA_RST = not(rst_sig.ALUR_RSTn);
    /*p03.ROKE*/ wire _TIMA_MUX_0 = mux2_n(TMA_0.q(), cpu_bus.TS_D0, _FF05_WRn);
    /*p03.PETU*/ wire _TIMA_MUX_1 = mux2_n(TMA_1.q(), cpu_bus.TS_D1, _FF05_WRn);
    /*p03.NYKU*/ wire _TIMA_MUX_2 = mux2_n(TMA_2.q(), cpu_bus.TS_D2, _FF05_WRn); // die annotation box wrong, this is a 5-rung
    /*p03.SOCE*/ wire _TIMA_MUX_3 = mux2_n(TMA_3.q(), cpu_bus.TS_D3, _FF05_WRn);
    /*p03.SALA*/ wire _TIMA_MUX_4 = mux2_n(TMA_4.q(), cpu_bus.TS_D4, _FF05_WRn);
    /*p03.SYRU*/ wire _TIMA_MUX_5 = mux2_n(TMA_5.q(), cpu_bus.TS_D5, _FF05_WRn);
    /*p03.REFU*/ wire _TIMA_MUX_6 = mux2_n(TMA_6.q(), cpu_bus.TS_D6, _FF05_WRn);
    /*p03.RATO*/ wire _TIMA_MUX_7 = mux2_n(TMA_7.q(), cpu_bus.TS_D7, _FF05_WRn);
    /*p03.PUXY*/ wire _TIMA_LD_0 = nor(_TIMA_RST, _TIMA_MUX_0);
    /*p03.NERO*/ wire _TIMA_LD_1 = nor(_TIMA_RST, _TIMA_MUX_1);
    /*p03.NADA*/ wire _TIMA_LD_2 = nor(_TIMA_RST, _TIMA_MUX_2);
    /*p03.REPA*/ wire _TIMA_LD_3 = nor(_TIMA_RST, _TIMA_MUX_3);
    /*p03.ROLU*/ wire _TIMA_LD_4 = nor(_TIMA_RST, _TIMA_MUX_4);
    /*p03.RUGY*/ wire _TIMA_LD_5 = nor(_TIMA_RST, _TIMA_MUX_5);
    /*p03.PYMA*/ wire _TIMA_LD_6 = nor(_TIMA_RST, _TIMA_MUX_6);
    /*p03.PAGU*/ wire _TIMA_LD_7 = nor(_TIMA_RST, _TIMA_MUX_7);
    /*p03.REGA*/ TIMA_0.clk_n(_TIMA_CLK, _TIMA_LOAD, _TIMA_LD_0);
    /*p03.POVY*/ TIMA_1.clk_n(TIMA_0.q(), _TIMA_LOAD, _TIMA_LD_1);
    /*p03.PERU*/ TIMA_2.clk_n(TIMA_1.q(), _TIMA_LOAD, _TIMA_LD_2);
    /*p03.RATE*/ TIMA_3.clk_n(TIMA_2.q(), _TIMA_LOAD, _TIMA_LD_3);
    /*p03.RUBY*/ TIMA_4.clk_n(TIMA_3.q(), _TIMA_LOAD, _TIMA_LD_4);
    /*p03.RAGE*/ TIMA_5.clk_n(TIMA_4.q(), _TIMA_LOAD, _TIMA_LD_5);
    /*p03.PEDA*/ TIMA_6.clk_n(TIMA_5.q(), _TIMA_LOAD, _TIMA_LD_6);
    /*p03.NUGA*/ TIMA_7.clk_n(TIMA_6.q(), _TIMA_LOAD, _TIMA_LD_7);
  }

  {
    /*p03.MUGY*/ wire _TIMA_LOADn = not(_TIMA_LOAD);
    /*p03.NYDU*/ TIMA_MAX.set(clk_sig.PIN_BOGA_AxCDEFGH, _TIMA_LOADn, TIMA_7.q());
  }

  {
    /*p03.MERY*/ wire _INT_TIMER_IN = nor(!TIMA_MAX.q(), TIMA_7.q());
    /*p03.MOBA*/ MOBA_INT_TIMERp.set(clk_sig.PIN_BOGA_AxCDEFGH, rst_sig.ALUR_RSTn, _INT_TIMER_IN);
  }

  // FF06 TMA
  {
    /*p03.TYJU*/ wire _TYJU_FF06_WRn = nand(cpu_sig.TAPU_CPU_WR_xxxxxFGH, adr_sig.RYFO_FF04_FF07p, adr_sig.TOVY_A00n, cpu_bus.PIN_A01);
    /*p03.SABU*/ TMA_0.set(_TYJU_FF06_WRn, rst_sig.ALUR_RSTn, cpu_bus.TS_D0);
    /*p03.NYKE*/ TMA_1.set(_TYJU_FF06_WRn, rst_sig.ALUR_RSTn, cpu_bus.TS_D1);
    /*p03.MURU*/ TMA_2.set(_TYJU_FF06_WRn, rst_sig.ALUR_RSTn, cpu_bus.TS_D2);
    /*p03.TYVA*/ TMA_3.set(_TYJU_FF06_WRn, rst_sig.ALUR_RSTn, cpu_bus.TS_D3);
    /*p03.TYRU*/ TMA_4.set(_TYJU_FF06_WRn, rst_sig.ALUR_RSTn, cpu_bus.TS_D4);
    /*p03.SUFY*/ TMA_5.set(_TYJU_FF06_WRn, rst_sig.ALUR_RSTn, cpu_bus.TS_D5);
    /*p03.PETO*/ TMA_6.set(_TYJU_FF06_WRn, rst_sig.ALUR_RSTn, cpu_bus.TS_D6);
    /*p03.SETA*/ TMA_7.set(_TYJU_FF06_WRn, rst_sig.ALUR_RSTn, cpu_bus.TS_D7);
  }

  // FF07 TAC
  {
    /*p03.SARA*/ wire _SARA_FF07_WRn = nand(cpu_sig.TAPU_CPU_WR_xxxxxFGH, adr_sig.RYFO_FF04_FF07p, cpu_bus.PIN_A00, cpu_bus.PIN_A01);
    /*p03.SOPU*/ TAC_0.set(_SARA_FF07_WRn, rst_sig.ALUR_RSTn, cpu_bus.TS_D0);
    /*p03.SAMY*/ TAC_1.set(_SARA_FF07_WRn, rst_sig.ALUR_RSTn, cpu_bus.TS_D1);
    /*p03.SABO*/ TAC_2.set(_SARA_FF07_WRn, rst_sig.ALUR_RSTn, cpu_bus.TS_D2);
  }


  // FF04 DIV
  {
    /*p01.TAGY*/ wire FF04_RD = and (cpu_sig.TEDO_CPU_RD, adr_sig.RYFO_FF04_FF07p, adr_sig.TOLA_A01n, adr_sig.TOVY_A00n);

    /*p01.UTOK*/ wire DIV_08n = not(TUGO_DIV_08.q());
    /*p01.SAPY*/ wire DIV_09n = not(TOFE_DIV_09.q());
    /*p01.UMER*/ wire DIV_10n = not(TERU_DIV_10.q());
    /*p01.RAVE*/ wire DIV_11n = not(SOLA_DIV_11.q());
    /*p01.RYSO*/ wire DIV_12n = not(SUBU_DIV_12.q());
    /*p01.UDOR*/ wire DIV_13n = not(TEKA_DIV_13.q());
    /*p01.TAWU*/ cpu_bus.TS_D0.set_tribuf(FF04_RD, not(tim_sig.UMEK_DIV_06n));
    /*p01.TAKU*/ cpu_bus.TS_D1.set_tribuf(FF04_RD, not(tim_sig.UREK_DIV_07n));
    /*p01.TEMU*/ cpu_bus.TS_D2.set_tribuf(FF04_RD, not(DIV_08n));
    /*p01.TUSE*/ cpu_bus.TS_D3.set_tribuf(FF04_RD, not(DIV_09n));
    /*p01.UPUG*/ cpu_bus.TS_D4.set_tribuf(FF04_RD, not(DIV_10n));
    /*p01.SEPU*/ cpu_bus.TS_D5.set_tribuf(FF04_RD, not(DIV_11n));
    /*p01.SAWA*/ cpu_bus.TS_D6.set_tribuf(FF04_RD, not(DIV_12n));
    /*p01.TATU*/ cpu_bus.TS_D7.set_tribuf(FF04_RD, not(DIV_13n));
  }

  // FF05 TIMA
  {
    /*p03.TEDA*/ wire FF05_RD = and (cpu_sig.TEDO_CPU_RD, adr_sig.RYFO_FF04_FF07p, adr_sig.TOLA_A01n, cpu_bus.PIN_A00);
    /*p03.SOKU*/ cpu_bus.TS_D0.set_tribuf(FF05_RD, TIMA_0.q());
    /*p03.RACY*/ cpu_bus.TS_D1.set_tribuf(FF05_RD, TIMA_1.q());
    /*p03.RAVY*/ cpu_bus.TS_D2.set_tribuf(FF05_RD, TIMA_2.q());
    /*p03.SOSY*/ cpu_bus.TS_D3.set_tribuf(FF05_RD, TIMA_3.q());
    /*p03.SOMU*/ cpu_bus.TS_D4.set_tribuf(FF05_RD, TIMA_4.q());
    /*p03.SURO*/ cpu_bus.TS_D5.set_tribuf(FF05_RD, TIMA_5.q());
    /*p03.ROWU*/ cpu_bus.TS_D6.set_tribuf(FF05_RD, TIMA_6.q());
    /*p03.PUSO*/ cpu_bus.TS_D7.set_tribuf(FF05_RD, TIMA_7.q());
  }

  // FF06 TMA
  {
    /*p03.TUBY*/ wire FF06_RD = and (cpu_sig.TEDO_CPU_RD, adr_sig.RYFO_FF04_FF07p, cpu_bus.PIN_A01, adr_sig.TOVY_A00n);
    /*p03.SETE*/ cpu_bus.TS_D0.set_tribuf(FF06_RD, TMA_0.q());
    /*p03.PYRE*/ cpu_bus.TS_D1.set_tribuf(FF06_RD, TMA_1.q());
    /*p03.NOLA*/ cpu_bus.TS_D2.set_tribuf(FF06_RD, TMA_2.q());
    /*p03.SALU*/ cpu_bus.TS_D3.set_tribuf(FF06_RD, TMA_3.q());
    /*p03.SUPO*/ cpu_bus.TS_D4.set_tribuf(FF06_RD, TMA_4.q());
    /*p03.SOTU*/ cpu_bus.TS_D5.set_tribuf(FF06_RD, TMA_5.q());
    /*p03.REVA*/ cpu_bus.TS_D6.set_tribuf(FF06_RD, TMA_6.q());
    /*p03.SAPU*/ cpu_bus.TS_D7.set_tribuf(FF06_RD, TMA_7.q());
  }

  // FF07 TAC
  {
    /*p03.SORA*/ wire FF07_RD = and (cpu_sig.TEDO_CPU_RD, adr_sig.RYFO_FF04_FF07p, cpu_bus.PIN_A00, cpu_bus.PIN_A01);
    /*p03.RYLA*/ cpu_bus.TS_D0.set_tribuf(FF07_RD, TAC_0.q());
    /*p03.ROTE*/ cpu_bus.TS_D1.set_tribuf(FF07_RD, TAC_1.q());
    /*p03.SUPE*/ cpu_bus.TS_D2.set_tribuf(FF07_RD, TAC_2.q());
  }


}


bool TimerRegisters::commit() {
  bool changed = false;
  return changed;
}
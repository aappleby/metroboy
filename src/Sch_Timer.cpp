#include "Sch_Timer.h"

#include "Sch_Common.h"
#include "Sch_Pins.h"
#include "TestGB.h"
#include "Constants.h"

using namespace Schematics;

TimerSignals TimerSignals::get(TestGB& gb) {
  return {
    /*p01.UVYN*/ .UVYN_DIV_05n = not(gb.tim_reg.TAMA_DIV_05),
    /*p01.UMEK*/ .UMEK_DIV_06n = not(gb.tim_reg.UGOT_DIV_06),
    /*p01.UREK*/ .UREK_DIV_07n = not(gb.tim_reg.TULU_DIV_07),
  };
}

void TestGB::tick_timer() {
  auto clk_sig = clk_reg.sig(*this);
  auto tim_sig = TimerSignals::get(*this);
  auto adr_sig = adr_reg.sig(cpu_pins);
  auto rst_sig = ResetSignals::get(*this);
  auto cpu_sig = cpu_reg.sig(*this);

  /*p01.UCOB*/ wire UCOB_CLKBAD = not(sys_pins.CLK_GOOD);

  // div
  {
    /*p01.TAPE*/ wire _FF04_WR = and(cpu_sig.TAPU_CPU_WR_xxxxxFGH, adr_sig.RYFO_FF04_FF07p, adr_sig.TOLA_A01n, adr_sig.TOVY_A00n);
    /*p01.UFOL*/ wire _DIV_RSTn = nor(UCOB_CLKBAD, sys_pins.RST, _FF04_WR);

    /*p01.UKUP*/ tim_reg.UKUP_DIV_00.set(clk_sig.BOGA_AxCDEFGH, _DIV_RSTn, !tim_reg.UKUP_DIV_00.q());
    /*p01.UFOR*/ tim_reg.UFOR_DIV_01.set(!tim_reg.UKUP_DIV_00.q(), _DIV_RSTn, !tim_reg.UFOR_DIV_01.q());
    /*p01.UNER*/ tim_reg.UNER_DIV_02.set(!tim_reg.UFOR_DIV_01.q(), _DIV_RSTn, !tim_reg.UNER_DIV_02.q());
    
    // TERO second from bottom rung - feedback, so this is QN
    // TERO bottom rung             - UVYR, so this is Q
    
    /*p01.TERO*/ tim_reg.TERO_DIV_03.set(!tim_reg.UNER_DIV_02.q(), _DIV_RSTn, !tim_reg.TERO_DIV_03.q());
    /*p01.UNYK*/ tim_reg.UNYK_DIV_04.set(!tim_reg.TERO_DIV_03.q(), _DIV_RSTn, !tim_reg.UNYK_DIV_04.q());
    /*p01.TAMA*/ tim_reg.TAMA_DIV_05.set(!tim_reg.UNYK_DIV_04.q(), _DIV_RSTn, !tim_reg.TAMA_DIV_05.q());

    ///*p01.ULUR*/ wire DIV_06_CLK = mux2(BOGA_AxCDEFGH, DIV_05, FF60_1);
    /*p01.ULUR*/ wire _DIV_06_CLK = tim_reg.TAMA_DIV_05.q();
    /*p01.UGOT*/ tim_reg.UGOT_DIV_06.set(!_DIV_06_CLK, _DIV_RSTn, !tim_reg.UGOT_DIV_06.q());
    /*p01.TULU*/ tim_reg.TULU_DIV_07.set(!tim_reg.UGOT_DIV_06.q(), _DIV_RSTn, !tim_reg.TULU_DIV_07.q());
    /*p01.TUGO*/ tim_reg.TUGO_DIV_08.set(!tim_reg.TULU_DIV_07.q(), _DIV_RSTn, !tim_reg.TUGO_DIV_08.q());
    /*p01.TOFE*/ tim_reg.TOFE_DIV_09.set(!tim_reg.TUGO_DIV_08.q(), _DIV_RSTn, !tim_reg.TOFE_DIV_09.q());
    /*p01.TERU*/ tim_reg.TERU_DIV_10.set(!tim_reg.TOFE_DIV_09.q(), _DIV_RSTn, !tim_reg.TERU_DIV_10.q());
    /*p01.SOLA*/ tim_reg.SOLA_DIV_11.set(!tim_reg.TERU_DIV_10.q(), _DIV_RSTn, !tim_reg.SOLA_DIV_11.q());
    /*p01.SUBU*/ tim_reg.SUBU_DIV_12.set(!tim_reg.SOLA_DIV_11.q(), _DIV_RSTn, !tim_reg.SUBU_DIV_12.q());
    /*p01.TEKA*/ tim_reg.TEKA_DIV_13.set(!tim_reg.SUBU_DIV_12.q(), _DIV_RSTn, !tim_reg.TEKA_DIV_13.q());
    /*p01.UKET*/ tim_reg.UKET_DIV_14.set(!tim_reg.TEKA_DIV_13.q(), _DIV_RSTn, !tim_reg.UKET_DIV_14.q());
    /*p01.UPOF*/ tim_reg.UPOF_DIV_15.set(!tim_reg.UKET_DIV_14.q(), _DIV_RSTn, !tim_reg.UPOF_DIV_15.q());
  }

  /*p03.TOPE*/ wire _FF05_WRn = nand(cpu_sig.TAPU_CPU_WR_xxxxxFGH, adr_sig.RYFO_FF04_FF07p, adr_sig.TOLA_A01n, cpu_pins.A00);
  /*p03.MUZU*/ wire _MUZU = or (cpu_pins.FROM_CPU5.q(), _FF05_WRn);
  /*p03.MEKE*/ wire _INT_TIMERn = not(tim_reg.INT_TIMER.q());
  /*p03.MEXU*/ wire _TIMA_LOAD = nand(_MUZU, rst_sig.ALUR_RSTn, _INT_TIMERn);

  // sch_timer.tick_tima
  {
    /*p03.UBOT*/ wire _CLK_256Kn = not(tim_reg.UFOR_DIV_01.q());
    /*p03.UVYR*/ wire _CLK_64Kn = not(tim_reg.TERO_DIV_03.q());

    /*p03.UKAP*/ wire _CLK_MUXa = mux2_n(tim_sig.UVYN_DIV_05n, _CLK_64Kn, tim_reg.TAC_0.q());
    /*p03.TEKO*/ wire _CLK_MUXb = mux2_n(_CLK_256Kn, tim_sig.UREK_DIV_07n, tim_reg.TAC_0.q());
    /*p03.TECY*/ wire _CLK_MUXc = mux2_n(_CLK_MUXa, _CLK_MUXb, tim_reg.TAC_1.q());

    /*p03.SOGU*/ wire _TIMA_CLK = nor(_CLK_MUXc, !tim_reg.TAC_2.q());
    /*p03.MULO*/ wire _TIMA_RST = not(rst_sig.ALUR_RSTn);
    /*p03.ROKE*/ wire _TIMA_MUX_0 = mux2_n(tim_reg.TMA_0.q(), cpu_pins.D0, _FF05_WRn);
    /*p03.PETU*/ wire _TIMA_MUX_1 = mux2_n(tim_reg.TMA_1.q(), cpu_pins.D1, _FF05_WRn);
    /*p03.NYKU*/ wire _TIMA_MUX_2 = mux2_n(tim_reg.TMA_2.q(), cpu_pins.D2, _FF05_WRn); // die annotation box wrong, this is a 5-rung
    /*p03.SOCE*/ wire _TIMA_MUX_3 = mux2_n(tim_reg.TMA_3.q(), cpu_pins.D3, _FF05_WRn);
    /*p03.SALA*/ wire _TIMA_MUX_4 = mux2_n(tim_reg.TMA_4.q(), cpu_pins.D4, _FF05_WRn);
    /*p03.SYRU*/ wire _TIMA_MUX_5 = mux2_n(tim_reg.TMA_5.q(), cpu_pins.D5, _FF05_WRn);
    /*p03.REFU*/ wire _TIMA_MUX_6 = mux2_n(tim_reg.TMA_6.q(), cpu_pins.D6, _FF05_WRn);
    /*p03.RATO*/ wire _TIMA_MUX_7 = mux2_n(tim_reg.TMA_7.q(), cpu_pins.D7, _FF05_WRn);
    /*p03.PUXY*/ wire _TIMA_LD_0 = nor(_TIMA_RST, _TIMA_MUX_0);
    /*p03.NERO*/ wire _TIMA_LD_1 = nor(_TIMA_RST, _TIMA_MUX_1);
    /*p03.NADA*/ wire _TIMA_LD_2 = nor(_TIMA_RST, _TIMA_MUX_2);
    /*p03.REPA*/ wire _TIMA_LD_3 = nor(_TIMA_RST, _TIMA_MUX_3);
    /*p03.ROLU*/ wire _TIMA_LD_4 = nor(_TIMA_RST, _TIMA_MUX_4);
    /*p03.RUGY*/ wire _TIMA_LD_5 = nor(_TIMA_RST, _TIMA_MUX_5);
    /*p03.PYMA*/ wire _TIMA_LD_6 = nor(_TIMA_RST, _TIMA_MUX_6);
    /*p03.PAGU*/ wire _TIMA_LD_7 = nor(_TIMA_RST, _TIMA_MUX_7);
    /*p03.REGA*/ tim_reg.TIMA_0.clk_n(_TIMA_CLK, _TIMA_LOAD, _TIMA_LD_0);
    /*p03.POVY*/ tim_reg.TIMA_1.clk_n(tim_reg.TIMA_0.q(), _TIMA_LOAD, _TIMA_LD_1);
    /*p03.PERU*/ tim_reg.TIMA_2.clk_n(tim_reg.TIMA_1.q(), _TIMA_LOAD, _TIMA_LD_2);
    /*p03.RATE*/ tim_reg.TIMA_3.clk_n(tim_reg.TIMA_2.q(), _TIMA_LOAD, _TIMA_LD_3);
    /*p03.RUBY*/ tim_reg.TIMA_4.clk_n(tim_reg.TIMA_3.q(), _TIMA_LOAD, _TIMA_LD_4);
    /*p03.RAGE*/ tim_reg.TIMA_5.clk_n(tim_reg.TIMA_4.q(), _TIMA_LOAD, _TIMA_LD_5);
    /*p03.PEDA*/ tim_reg.TIMA_6.clk_n(tim_reg.TIMA_5.q(), _TIMA_LOAD, _TIMA_LD_6);
    /*p03.NUGA*/ tim_reg.TIMA_7.clk_n(tim_reg.TIMA_6.q(), _TIMA_LOAD, _TIMA_LD_7);
  }

  {
    /*p03.MUGY*/ wire _TIMA_LOADn = not(_TIMA_LOAD);
    /*p03.NYDU*/ tim_reg.TIMA_MAX.set(clk_sig.BOGA_AxCDEFGH, _TIMA_LOADn, tim_reg.TIMA_7.q());
  }

  {
    /*p03.MERY*/ wire _INT_TIMER_IN = nor(!tim_reg.TIMA_MAX.q(), tim_reg.TIMA_7.q());
    /*p03.MOBA*/ tim_reg.INT_TIMER.set(clk_sig.BOGA_AxCDEFGH, rst_sig.ALUR_RSTn, _INT_TIMER_IN);
  }

  // FF06 TMA
  {
    /*p03.TYJU*/ wire _TYJU_FF06_WRn = nand(cpu_sig.TAPU_CPU_WR_xxxxxFGH, adr_sig.RYFO_FF04_FF07p, adr_sig.TOVY_A00n, cpu_pins.A01);
    /*p03.SABU*/ tim_reg.TMA_0.set(_TYJU_FF06_WRn, rst_sig.ALUR_RSTn, cpu_pins.D0);
    /*p03.NYKE*/ tim_reg.TMA_1.set(_TYJU_FF06_WRn, rst_sig.ALUR_RSTn, cpu_pins.D1);
    /*p03.MURU*/ tim_reg.TMA_2.set(_TYJU_FF06_WRn, rst_sig.ALUR_RSTn, cpu_pins.D2);
    /*p03.TYVA*/ tim_reg.TMA_3.set(_TYJU_FF06_WRn, rst_sig.ALUR_RSTn, cpu_pins.D3);
    /*p03.TYRU*/ tim_reg.TMA_4.set(_TYJU_FF06_WRn, rst_sig.ALUR_RSTn, cpu_pins.D4);
    /*p03.SUFY*/ tim_reg.TMA_5.set(_TYJU_FF06_WRn, rst_sig.ALUR_RSTn, cpu_pins.D5);
    /*p03.PETO*/ tim_reg.TMA_6.set(_TYJU_FF06_WRn, rst_sig.ALUR_RSTn, cpu_pins.D6);
    /*p03.SETA*/ tim_reg.TMA_7.set(_TYJU_FF06_WRn, rst_sig.ALUR_RSTn, cpu_pins.D7);
  }

  // FF07 TAC
  {
    /*p03.SARA*/ wire _SARA_FF07_WRn = nand(cpu_sig.TAPU_CPU_WR_xxxxxFGH, adr_sig.RYFO_FF04_FF07p, cpu_pins.A00, cpu_pins.A01);
    /*p03.SOPU*/ tim_reg.TAC_0.set(_SARA_FF07_WRn, rst_sig.ALUR_RSTn, cpu_pins.D0);
    /*p03.SAMY*/ tim_reg.TAC_1.set(_SARA_FF07_WRn, rst_sig.ALUR_RSTn, cpu_pins.D1);
    /*p03.SABO*/ tim_reg.TAC_2.set(_SARA_FF07_WRn, rst_sig.ALUR_RSTn, cpu_pins.D2);
  }


}
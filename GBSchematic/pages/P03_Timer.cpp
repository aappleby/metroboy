#include "P03_Timer.h"

//-----------------------------------------------------------------------------
// This file should contain the schematics as directly translated to C,
// no modifications or simplifications.

const std::vector<SignalData> P03_Timer::signals() {
  return
  {
    SignalData("-----TIMA-----"),
    SignalData("REGA_0", offsetof(P03_Timer, REGA_0), 0, 1),
    SignalData("POVY_1", offsetof(P03_Timer, POVY_1), 0, 1),
    SignalData("PERU_2", offsetof(P03_Timer, PERU_2), 0, 1),
    SignalData("RATE_3", offsetof(P03_Timer, RATE_3), 0, 1),
    SignalData("RUBY_4", offsetof(P03_Timer, RUBY_4), 0, 1),
    SignalData("RAGE_5", offsetof(P03_Timer, RAGE_5), 0, 1),
    SignalData("PEDA_6", offsetof(P03_Timer, PEDA_6), 0, 1),
    SignalData("NUGA_7", offsetof(P03_Timer, NUGA_7), 0, 1),

    /*
    SignalData("-----TMA-----"),
    SignalData("SABU_0", offsetof(P03_Timer, SABU_0), 0, 1),
    SignalData("NYKE_1", offsetof(P03_Timer, NYKE_1), 0, 1),
    SignalData("MURU_2", offsetof(P03_Timer, MURU_2), 0, 1),
    SignalData("TYVA_3", offsetof(P03_Timer, TYVA_3), 0, 1),
    SignalData("TYRU_4", offsetof(P03_Timer, TYRU_4), 0, 1),
    SignalData("SUFY_5", offsetof(P03_Timer, SUFY_5), 0, 1),
    SignalData("PETO_6", offsetof(P03_Timer, PETO_6), 0, 1),
    SignalData("SETA_7", offsetof(P03_Timer, SETA_7), 0, 1),
    */

    SignalData("-----TAC-----"),
    SignalData("SOPU_0", offsetof(P03_Timer, SOPU_0), 0, 1),
    SignalData("SAMY_1", offsetof(P03_Timer, SAMY_1), 0, 1),
    SignalData("SABO_2", offsetof(P03_Timer, SABO_2), 0, 1),

    SignalData("-----Int-----"),
    SignalData("NYDU",   offsetof(P03_Timer, NYDU),   0, 1),
    SignalData("MOBA",   offsetof(P03_Timer, MOBA),   0, 1),

    SignalData("-----Clock mux-----"),
    SignalData("UVYR",   offsetof(P03_Timer, UVYR),   0, 1),
    SignalData("UKAP",   offsetof(P03_Timer, UKAP),   0, 1),
    SignalData("UBOT",   offsetof(P03_Timer, UBOT),   0, 1),
    SignalData("TEKO",   offsetof(P03_Timer, TEKO),   0, 1),
    SignalData("TECY",   offsetof(P03_Timer, TECY),   0, 1),
    SignalData("SOGU",   offsetof(P03_Timer, SOGU),   0, 1),
  };
}

//-----------------------------------------------------------------------------

void P03_Timer::tick(const P03_Timer& prev) {
  //----------
  // tac

  wire RYFO = and(in.A2, in.A00_07, in.FFXX);
  out.FF04_FF07 = RYFO;
  SARA = nand(in.CPU_WR, out.FF04_FF07, in.A0, in.A1);

  tock_neg(prev.SOPU_0, SOPU_0, prev.SARA, SARA, in.RESET2, in.D0);
  tock_neg(prev.SAMY_1, SAMY_1, prev.SARA, SARA, in.RESET2, in.D1);
  tock_neg(prev.SABO_2, SABO_2, prev.SARA, SARA, in.RESET2, in.D2);

  SORA = and(in.CPU_RD, out.FF04_FF07, in.A1, in.A0);
  RYLA = not(!SOPU_0);
  ROTE = not(!SAMY_1);
  SUPE = not(!SABO_2);

  if (SORA) {
    out.D2 = SUPE;
    out.D1 = ROTE;
    out.D0 = RYLA;
  }

  // clock mux
  UVYR = not(in.CLK_64K);
  UKAP = mux2(in.CLK_16K, UVYR, SOPU_0);
  UBOT = not(in.CLK_256K);
  TEKO = mux2(UBOT, in.FF04_D1n, SOPU_0);
  TECY = mux2(UKAP, TEKO, SAMY_1);
  SOGU = nor(TECY, !SABO_2);

  //----------
  // tma

  wire TOVY = not(in.A0);
  out.TOVY_A0n = TOVY;
  TYJU = nand(TOVY, in.A1, in.CPU_WR, out.FF04_FF07);

  tock_neg(prev.SABU_0, SABU_0, prev.TYJU, TYJU, in.RESET2, in.D0);
  tock_neg(prev.NYKE_1, NYKE_1, prev.TYJU, TYJU, in.RESET2, in.D1);
  tock_neg(prev.MURU_2, MURU_2, prev.TYJU, TYJU, in.RESET2, in.D2);
  tock_neg(prev.TYVA_3, TYVA_3, prev.TYJU, TYJU, in.RESET2, in.D3);
  tock_neg(prev.TYRU_4, TYRU_4, prev.TYJU, TYJU, in.RESET2, in.D4);
  tock_neg(prev.SUFY_5, SUFY_5, prev.TYJU, TYJU, in.RESET2, in.D5);
  tock_neg(prev.PETO_6, PETO_6, prev.TYJU, TYJU, in.RESET2, in.D6);
  tock_neg(prev.SETA_7, SETA_7, prev.TYJU, TYJU, in.RESET2, in.D7);

  wire SETE = not(!SABU_0);
  wire PYRE = not(!NYKE_1);
  wire NOLA = not(!MURU_2);
  wire SALU = not(!TYVA_3);
  wire SUPO = not(!TYRU_4);
  wire SOTU = not(!SUFY_5);
  wire REVA = not(!PETO_6);
  wire SAPU = not(!SETA_7);

  wire TUBY = and(out.FF04_FF07, in.CPU_RD, in.A1, TOVY);
  if (TUBY) {
    out.D0 = SETE;
    out.D1 = PYRE;
    out.D2 = NOLA;
    out.D3 = SALU;
    out.D4 = SUPO;
    out.D5 = SOTU;
    out.D6 = REVA;
    out.D7 = SAPU;
  }

  //----------
  // tima reload mux

  wire TOPE = nand(in.CPU_WR, out.FF04_FF07, in.A0, in.TOLA_A1n);
  wire ROKE = mux2(SABU_0, in.D0, TOPE);
  wire PETU = mux2(NYKE_1, in.D1, TOPE);
  wire NYKU = mux2(MURU_2, in.D2, TOPE);
  wire SOCE = mux2(TYVA_3, in.D3, TOPE);
  wire SALA = mux2(TYRU_4, in.D4, TOPE);
  wire SYRU = mux2(SUFY_5, in.D5, TOPE);
  wire REFU = mux2(PETO_6, in.D6, TOPE);
  wire RATO = mux2(SETA_7, in.D7, TOPE);

  // this nor doesn't make sense, it puts a negated value into TIMA
  /*
  wire MULO = not(in.RESET2);
  wire PUXY = nor(MULO, ROKE);
  wire NERO = nor(MULO, PETU);
  wire NADA = nor(MULO, NYKU);
  wire REPA = nor(MULO, SOCE);
  wire ROLU = nor(MULO, SALA);
  wire RUGY = nor(MULO, SYRU);
  wire PYMA = nor(MULO, REFU);
  wire PAGU = nor(MULO, RATO);
  */

  // this makes more sense but doesn't match the schematic
  wire MULO = in.RESET2;
  wire PUXY = and(MULO, ROKE);
  wire NERO = and(MULO, PETU);
  wire NADA = and(MULO, NYKU);
  wire REPA = and(MULO, SOCE);
  wire ROLU = and(MULO, SALA);
  wire RUGY = and(MULO, SYRU);
  wire PYMA = and(MULO, REFU);
  wire PAGU = and(MULO, RATO);

  //----------
  // tima

  wire MUZU = or(in.FROM_CPU5, TOPE);
  wire MEKE = not(in.INT_TIMER);
  wire MEXU = nand(MUZU, in.RESET2, MEKE);

  // daisy chain
  count_neg(prev.REGA_0, REGA_0, prev.SOGU,   SOGU,   MEXU, PUXY);
  count_neg(prev.POVY_1, POVY_1, prev.REGA_0, REGA_0, MEXU, NERO);
  count_neg(prev.PERU_2, PERU_2, prev.POVY_1, POVY_1, MEXU, NADA);
  count_neg(prev.RATE_3, RATE_3, prev.PERU_2, PERU_2, MEXU, REPA);
  count_neg(prev.RUBY_4, RUBY_4, prev.RATE_3, RATE_3, MEXU, ROLU);
  count_neg(prev.RAGE_5, RAGE_5, prev.RUBY_4, RUBY_4, MEXU, RUGY);
  count_neg(prev.PEDA_6, PEDA_6, prev.RAGE_5, RAGE_5, MEXU, PYMA);
  count_neg(prev.NUGA_7, NUGA_7, prev.PEDA_6, PEDA_6, MEXU, PAGU);

  bool carry = prev.NUGA_7 && !NUGA_7;

  wire SOKU = not(!REGA_0);
  wire RACY = not(!POVY_1);
  wire RAVY = not(!PERU_2);
  wire SOSY = not(!RATE_3);
  wire SOMU = not(!RUBY_4);
  wire SURO = not(!RAGE_5);
  wire ROWU = not(!PEDA_6);
  wire PUSO = not(!NUGA_7);

  wire TEDA = and(out.FF04_FF07, in.CPU_RD, in.TOLA_A1n, in.A0);
  if (TEDA) {
    out.OE = true;
    out.D0 = SOKU;
    out.D1 = RACY;
    out.D2 = RAVY;
    out.D3 = SOSY;
    out.D4 = SOMU;
    out.D5 = SURO;
    out.D6 = ROWU; // schematic missing annotation
    out.D7 = PUSO;
  }

  //----------
  // interrupt delay line

  wire MUGY = not(MEXU);
  tock_neg(prev.NYDU, NYDU, prev.in.BOGA_1M, in.BOGA_1M, MUGY, carry);
  wire MERY = nor(!prev.NYDU, NUGA_7);
  tock_neg(prev.MOBA, MOBA, prev.in.BOGA_1M, in.BOGA_1M, in.RESET2, MERY);
  out.INT_TIMER = MOBA;
}
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
    SignalData("NUGA_C", offsetof(P03_Timer, NUGA_C), 0, 1),

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

  RYFO = and(in.A2, in.A00_07, in.FFXX);
  out.FF04_FF07 = RYFO;
  SARA = nand(in.CPU_WR, out.FF04_FF07, in.A0, in.A1);

  SOPU_0 = prev.SOPU_0;
  SAMY_1 = prev.SAMY_1;
  SABO_2 = prev.SABO_2;

  if (prev.SARA && !SARA) {
    SOPU_0 = in.D0;
    SAMY_1 = in.D1;
    SABO_2 = in.D2;
  }

  if (!in.RESET2) {
    SOPU_0 = 0;
    SAMY_1 = 0;
    SABO_2 = 0;
  }


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

  TOVY = not(in.A0);
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

  SETE = not(!SABU_0);
  PYRE = not(!NYKE_1);
  NOLA = not(!MURU_2);
  SALU = not(!TYVA_3);
  SUPO = not(!TYRU_4);
  SOTU = not(!SUFY_5);
  REVA = not(!PETO_6);
  SAPU = not(!SETA_7);

  TUBY = and(out.FF04_FF07, in.CPU_RD, in.A1, TOVY);
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

  TOPE = nand(in.CPU_WR, out.FF04_FF07, in.A0, in.TOLA_A1n);
  ROKE = mux2(SABU_0, in.D0, TOPE);
  PETU = mux2(NYKE_1, in.D1, TOPE);
  NYKU = mux2(MURU_2, in.D2, TOPE);
  SOCE = mux2(TYVA_3, in.D3, TOPE);
  SALA = mux2(TYRU_4, in.D4, TOPE);
  SYRU = mux2(SUFY_5, in.D5, TOPE);
  REFU = mux2(PETO_6, in.D6, TOPE);
  RATO = mux2(SETA_7, in.D7, TOPE);

  // this nor doesn't make sense, it puts a negated value into TIMA
  /*
  MULO = not(in.RESET2);
  PUXY = nor(MULO, ROKE);
  NERO = nor(MULO, PETU);
  NADA = nor(MULO, NYKU);
  REPA = nor(MULO, SOCE);
  ROLU = nor(MULO, SALA);
  RUGY = nor(MULO, SYRU);
  PYMA = nor(MULO, REFU);
  PAGU = nor(MULO, RATO);
  */

  // this makes more sense but doesn't match the schematic
  MULO = in.RESET2;
  PUXY = and(MULO, ROKE);
  NERO = and(MULO, PETU);
  NADA = and(MULO, NYKU);
  REPA = and(MULO, SOCE);
  ROLU = and(MULO, SALA);
  RUGY = and(MULO, SYRU);
  PYMA = and(MULO, REFU);
  PAGU = and(MULO, RATO);

  //----------
  // tima

  // daisy chain

  REGA_0 = prev.REGA_0; REGA_C = prev.REGA_C; 
  POVY_1 = prev.POVY_1; POVY_C = prev.POVY_C;
  PERU_2 = prev.PERU_2; PERU_C = prev.PERU_C;
  RATE_3 = prev.RATE_3; RATE_C = prev.RATE_C;
  RUBY_4 = prev.RUBY_4; RUBY_C = prev.RUBY_C;
  RAGE_5 = prev.RAGE_5; RAGE_C = prev.RAGE_C;
  PEDA_6 = prev.PEDA_6; PEDA_C = prev.PEDA_C;
  NUGA_7 = prev.NUGA_7; NUGA_C = prev.NUGA_C;

  if (!prev.SOGU   && SOGU)   { REGA_0 = !prev.REGA_0; REGA_C = prev.REGA_0; }
  if (!prev.REGA_C && REGA_C) { POVY_1 = !prev.POVY_1; POVY_C = prev.POVY_1; }
  if (!prev.POVY_C && POVY_C) { PERU_2 = !prev.PERU_2; PERU_C = prev.PERU_2; }
  if (!prev.PERU_C && PERU_C) { RATE_3 = !prev.RATE_3; RATE_C = prev.RATE_3; }
  if (!prev.RATE_C && RATE_C) { RUBY_4 = !prev.RUBY_4; RUBY_C = prev.RUBY_4; }
  if (!prev.RUBY_C && RUBY_C) { RAGE_5 = !prev.RAGE_5; RAGE_C = prev.RAGE_5; }
  if (!prev.RAGE_C && RAGE_C) { PEDA_6 = !prev.PEDA_6; PEDA_C = prev.PEDA_6; }
  if (!prev.PEDA_C && PEDA_C) { NUGA_7 = !prev.NUGA_7; NUGA_C = prev.NUGA_7; }

  //----------
  // interrupt delay line

  wire MUZU = or(in.FROM_CPU5, TOPE);
  wire MEKE = not(out.INT_TIMER);
  wire MEXU = nand(MUZU, in.RESET2, MEKE);

  wire MUGY = not(MEXU);
  tock_neg(prev.NYDU, NYDU, prev.in.BOGA_1M, in.BOGA_1M, MUGY, NUGA_C);
  wire MERY = nor(!prev.NYDU, NUGA_7);
  tock_neg(prev.MOBA, MOBA, prev.in.BOGA_1M, in.BOGA_1M, in.RESET2, MERY);
  out.INT_TIMER = MOBA;

  if(MEXU) {
    REGA_0 = PUXY; REGA_C = 0;
    POVY_1 = NERO; POVY_C = 0;
    PERU_2 = NADA; PERU_C = 0;
    RATE_3 = REPA; RATE_C = 0;
    RUBY_4 = ROLU; RUBY_C = 0;
    RAGE_5 = RUGY; RAGE_C = 0;
    PEDA_6 = PYMA; PEDA_C = 0;
    NUGA_7 = PAGU; NUGA_C = 0;
  }

  //----------
  // tima read

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
}
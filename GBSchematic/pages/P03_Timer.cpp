#include "P03_Timer.h"

#include "Gameboy.h"

//-----------------------------------------------------------------------------
// This file should contain the schematics as directly translated to C,
// no modifications or simplifications.

const std::vector<SignalData> P03_Timer::signals() {
  return
  {
    SignalData("-----TAC-----"),
    SignalData("SOPU_0", offsetof(P03_Timer, SOPU_0)),
    SignalData("SAMY_1", offsetof(P03_Timer, SAMY_1)),
    SignalData("SABO_2", offsetof(P03_Timer, SABO_2)),

    SignalData("-----TMA-----"),
    SignalData("SABU_0", offsetof(P03_Timer, SABU_0)),
    SignalData("NYKE_1", offsetof(P03_Timer, NYKE_1)),
    SignalData("MURU_2", offsetof(P03_Timer, MURU_2)),
    SignalData("TYVA_3", offsetof(P03_Timer, TYVA_3)),
    SignalData("TYRU_4", offsetof(P03_Timer, TYRU_4)),
    SignalData("SUFY_5", offsetof(P03_Timer, SUFY_5)),
    SignalData("PETO_6", offsetof(P03_Timer, PETO_6)),
    SignalData("SETA_7", offsetof(P03_Timer, SETA_7)),

    SignalData("-----TIMA-----"),
    SignalData("REGA_0", offsetof(P03_Timer, REGA_0)),
    SignalData("POVY_1", offsetof(P03_Timer, POVY_1)),
    SignalData("PERU_2", offsetof(P03_Timer, PERU_2)),
    SignalData("RATE_3", offsetof(P03_Timer, RATE_3)),
    SignalData("RUBY_4", offsetof(P03_Timer, RUBY_4)),
    SignalData("RAGE_5", offsetof(P03_Timer, RAGE_5)),
    SignalData("PEDA_6", offsetof(P03_Timer, PEDA_6)),
    SignalData("NUGA_7", offsetof(P03_Timer, NUGA_7)),

    SignalData("-----Int-----"),
    SignalData("NYDU",   offsetof(P03_Timer, NYDU)),
    SignalData("MOBA",   offsetof(P03_Timer, MOBA)),

    SignalData("-----Clock mux-----"),
    SignalData("UVYR",   offsetof(P03_Timer, UVYR)),
    SignalData("UKAP",   offsetof(P03_Timer, UKAP)),
    SignalData("UBOT",   offsetof(P03_Timer, UBOT)),
    SignalData("TEKO",   offsetof(P03_Timer, TEKO)),
    SignalData("TECY",   offsetof(P03_Timer, TECY)),
    SignalData("SOGU",   offsetof(P03_Timer, SOGU)),
  };
}

//-----------------------------------------------------------------------------

void P03_Timer::tick(const Gameboy& a, const Gameboy& b, Gameboy& c) {

  //----------
  // random decoder

  c.p03.RYFO = and(b.A02, b.p06.ADDR_XX00_XX07, b.p07.ADDR_FFXX);

  //----------
  // TAC

  c.p03.SARA = nand(b.p07.CPU_WR, b.p03.FF04_FF07, b.A00, b.A01); // nand? guess it happens on the neg edge of CPU_WR?
  c.p03.SORA = and (b.p07.CPU_RD, b.p03.FF04_FF07, b.A00, b.A01);

  c.p03.SOPU_0 = tock_pos(a.p03.SARA, b.p03.SARA, b.p01.SYS_RESETn1, b.p03.SOPU_0, b.D0);
  c.p03.SAMY_1 = tock_pos(a.p03.SARA, b.p03.SARA, b.p01.SYS_RESETn1, b.p03.SAMY_1, b.D1);
  c.p03.SABO_2 = tock_pos(a.p03.SARA, b.p03.SARA, b.p01.SYS_RESETn1, b.p03.SABO_2, b.D2);

  c.p03.RYLA = not(!b.p03.SOPU_0);
  c.p03.ROTE = not(!b.p03.SAMY_1);
  c.p03.SUPE = not(!b.p03.SABO_2);

  if (b.p03.SORA) {
    c.D2 = b.p03.SUPE;
    c.D1 = b.p03.ROTE;
    c.D0 = b.p03.RYLA;
  }

  //----------
  // TMA

  c.p03.TOVY = not(b.A00);
  c.p03.TYJU = nand(b.p03.TOVY, b.A01, b.p07.CPU_WR, b.p03.FF04_FF07);

  c.p03.SABU_0 = tock_pos(a.p03.TYJU, b.p03.TYJU, b.p01.SYS_RESETn1, b.p03.SABU_0, b.D0);
  c.p03.NYKE_1 = tock_pos(a.p03.TYJU, b.p03.TYJU, b.p01.SYS_RESETn1, b.p03.NYKE_1, b.D1);
  c.p03.MURU_2 = tock_pos(a.p03.TYJU, b.p03.TYJU, b.p01.SYS_RESETn1, b.p03.MURU_2, b.D2);
  c.p03.TYVA_3 = tock_pos(a.p03.TYJU, b.p03.TYJU, b.p01.SYS_RESETn1, b.p03.TYVA_3, b.D3);
  c.p03.TYRU_4 = tock_pos(a.p03.TYJU, b.p03.TYJU, b.p01.SYS_RESETn1, b.p03.TYRU_4, b.D4);
  c.p03.SUFY_5 = tock_pos(a.p03.TYJU, b.p03.TYJU, b.p01.SYS_RESETn1, b.p03.SUFY_5, b.D5);
  c.p03.PETO_6 = tock_pos(a.p03.TYJU, b.p03.TYJU, b.p01.SYS_RESETn1, b.p03.PETO_6, b.D6);
  c.p03.SETA_7 = tock_pos(a.p03.TYJU, b.p03.TYJU, b.p01.SYS_RESETn1, b.p03.SETA_7, b.D7);

  c.p03.SETE = not(!b.p03.SABU_0);
  c.p03.PYRE = not(!b.p03.NYKE_1);
  c.p03.NOLA = not(!b.p03.MURU_2);
  c.p03.SALU = not(!b.p03.TYVA_3);
  c.p03.SUPO = not(!b.p03.TYRU_4);
  c.p03.SOTU = not(!b.p03.SUFY_5);
  c.p03.REVA = not(!b.p03.PETO_6);
  c.p03.SAPU = not(!b.p03.SETA_7);

  c.p03.TUBY = and(b.p03.FF04_FF07, b.p07.CPU_RD, b.A01, b.p03.TOVY);
  if (b.p03.TUBY) {
    c.D0 = b.p03.SETE;
    c.D1 = b.p03.PYRE;
    c.D2 = b.p03.NOLA;
    c.D3 = b.p03.SALU;
    c.D4 = b.p03.SUPO;
    c.D5 = b.p03.SOTU;
    c.D6 = b.p03.REVA;
    c.D7 = b.p03.SAPU;
  }

  //----------
  // TIMA clock mux

  c.p03.UVYR = not(b.p01.DIV_03);
  c.p03.UKAP = mux2(b.p01.CLK_16K, b.p03.UVYR, b.p03.SOPU_0);
  c.p03.UBOT = not(b.p01.DIV_01);
  c.p03.TEKO = mux2(b.p03.UBOT, b.p01.DIV_07n, b.p03.SOPU_0);
  c.p03.TECY = mux2(b.p03.UKAP, b.p03.TEKO, b.p03.SAMY_1);
  c.p03.SOGU = nor(b.p03.TECY, !b.p03.SABO_2);

  //----------
  // TIMA reload signal

  c.p03.MUZU = or(b.cpu.FROM_CPU5, b.p03.TOPE);
  c.p03.MEKE = not(b.p03.INT_TIMER);
  c.p03.MEXU = nand(b.p03.MUZU, b.p01.SYS_RESETn1, b.p03.MEKE);

  //----------
  // TIMA reload mux

  c.p03.TOPE = nand(b.p07.CPU_WR, b.p03.FF04_FF07, b.A00, b.p08.A1n);
  c.p03.ROKE = mux2n(b.p03.SABU_0, b.D0, b.p03.TOPE);
  c.p03.PETU = mux2n(b.p03.NYKE_1, b.D1, b.p03.TOPE);
  c.p03.NYKU = mux2n(b.p03.MURU_2, b.D2, b.p03.TOPE);
  c.p03.SOCE = mux2n(b.p03.TYVA_3, b.D3, b.p03.TOPE);
  c.p03.SALA = mux2n(b.p03.TYRU_4, b.D4, b.p03.TOPE);
  c.p03.SYRU = mux2n(b.p03.SUFY_5, b.D5, b.p03.TOPE);
  c.p03.REFU = mux2n(b.p03.PETO_6, b.D6, b.p03.TOPE);
  c.p03.RATO = mux2n(b.p03.SETA_7, b.D7, b.p03.TOPE);

  c.p03.MULO = not(b.p01.SYS_RESETn1);
  c.p03.PUXY = nor(b.p03.MULO, b.p03.ROKE);
  c.p03.NERO = nor(b.p03.MULO, b.p03.PETU);
  c.p03.NADA = nor(b.p03.MULO, b.p03.NYKU);
  c.p03.REPA = nor(b.p03.MULO, b.p03.SOCE);
  c.p03.ROLU = nor(b.p03.MULO, b.p03.SALA);
  c.p03.RUGY = nor(b.p03.MULO, b.p03.SYRU);
  c.p03.PYMA = nor(b.p03.MULO, b.p03.REFU);
  c.p03.PAGU = nor(b.p03.MULO, b.p03.RATO);

  //----------
  // TIMA

  c.p03.REGA_0 = count_pos(a.p03.SOGU,   b.p03.SOGU,   b.p03.MEXU, b.p03.REGA_0, b.p03.PUXY);
  c.p03.REGA_0 = count_pos(a.p03.REGA_0, b.p03.REGA_0, b.p03.MEXU, b.p03.POVY_1, b.p03.NERO);
  c.p03.REGA_0 = count_pos(a.p03.POVY_1, b.p03.POVY_1, b.p03.MEXU, b.p03.PERU_2, b.p03.NADA);
  c.p03.REGA_0 = count_pos(a.p03.PERU_2, b.p03.PERU_2, b.p03.MEXU, b.p03.RATE_3, b.p03.REPA);
  c.p03.REGA_0 = count_pos(a.p03.RATE_3, b.p03.RATE_3, b.p03.MEXU, b.p03.RUBY_4, b.p03.ROLU);
  c.p03.REGA_0 = count_pos(a.p03.RUBY_4, b.p03.RUBY_4, b.p03.MEXU, b.p03.RAGE_5, b.p03.RUGY);
  c.p03.REGA_0 = count_pos(a.p03.RAGE_5, b.p03.RAGE_5, b.p03.MEXU, b.p03.PEDA_6, b.p03.PYMA);
  c.p03.REGA_0 = count_pos(a.p03.PEDA_6, b.p03.PEDA_6, b.p03.MEXU, b.p03.NUGA_7, b.p03.PAGU);

  c.p03.SOKU_0 = not(!b.p03.REGA_0);
  c.p03.RACY_1 = not(!b.p03.POVY_1);
  c.p03.RAVY_2 = not(!b.p03.PERU_2);
  c.p03.SOSY_3 = not(!b.p03.RATE_3);
  c.p03.SOMU_4 = not(!b.p03.RUBY_4);
  c.p03.SURO_5 = not(!b.p03.RAGE_5);
  c.p03.ROWU_6 = not(!b.p03.PEDA_6);
  c.p03.PUSO_7 = not(!b.p03.NUGA_7);

  c.p03.TEDA = and(b.p03.FF04_FF07, b.p07.CPU_RD, b.p08.A1n, b.A00);
  if (b.p03.TEDA) {
    c.D0 = b.p03.SOKU_0;
    c.D1 = b.p03.RACY_1;
    c.D2 = b.p03.RAVY_2;
    c.D3 = b.p03.SOSY_3;
    c.D4 = b.p03.SOMU_4;
    c.D5 = b.p03.SURO_5;
    c.D6 = b.p03.ROWU_6;
    c.D7 = b.p03.PUSO_7;
  }

  //----------
  // INT_TIMER delay

  c.p03.MUGY = not(b.p03.MEXU);
  c.p03.NYDU = tock_pos(a.p01.DIV_CLK, b.p01.DIV_CLK, b.p03.MUGY, b.p03.NYDU, b.p03.NUGA_7);
  c.p03.MERY = nor(!b.p03.NYDU, b.p03.NUGA_7);
  c.p03.MOBA = tock_pos(a.p01.DIV_CLK, b.p01.DIV_CLK, b.p01.SYS_RESETn1, b.p03.MOBA, b.p03.MERY);
}
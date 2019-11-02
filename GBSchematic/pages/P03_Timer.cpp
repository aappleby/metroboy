#include "P03_Timer.h"

//-----------------------------------------------------------------------------
// This file should contain the schematics as directly translated to C,
// no modifications or simplifications.

const std::vector<SignalData> P03_Timer::signals() {
  return
  {
    SignalData("BOGA",   offsetof(P03_Timer, in.BOGA_1M)),
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

void P03_Timer::tick(const CpuSignals& cpu,
                     const ChipSignals& chip,
                     const P03_Timer& a, const TristateBus& busA,
                     const P03_Timer& b, const TristateBus& busB,
                     P03_Timer& c, TristateBus& busC)
{
  (void)chip;
  (void)busA;

  //----------
  // random decoder

  c.RYFO = and(busB.A2, b.in.A00_07, b.in.FFXX);
  c.out.FF04_FF07 = b.RYFO;

  //----------
  // TAC

  c.SARA = nand(cpu.CPU_WR, b.out.FF04_FF07, busB.A0, busB.A1);

  c.SOPU_0 = tock_pos(a.SARA, b.SARA, b.in.RESET2, b.SOPU_0, busB.D0);
  c.SAMY_1 = tock_pos(a.SARA, b.SARA, b.in.RESET2, b.SAMY_1, busB.D1);
  c.SABO_2 = tock_pos(a.SARA, b.SARA, b.in.RESET2, b.SABO_2, busB.D2);

  c.SORA = and(cpu.CPU_RD, b.out.FF04_FF07, busB.A1, busB.A0);
  c.RYLA = not(!b.SOPU_0);
  c.ROTE = not(!b.SAMY_1);
  c.SUPE = not(!b.SABO_2);

  if (b.SORA) {
    busC.D2 = b.SUPE;
    busC.D1 = b.ROTE;
    busC.D0 = b.RYLA;
  }

  //----------
  // TMA

  c.TOVY = not(busB.A0);
  c.out.TOVY_A0n = b.TOVY;
  c.TYJU = nand(b.TOVY, busB.A1, cpu.CPU_WR, b.out.FF04_FF07);

  c.SABU_0 = tock_pos(a.TYJU, b.TYJU, b.in.RESET2, b.SABU_0, busB.D0);
  c.NYKE_1 = tock_pos(a.TYJU, b.TYJU, b.in.RESET2, b.NYKE_1, busB.D1);
  c.MURU_2 = tock_pos(a.TYJU, b.TYJU, b.in.RESET2, b.MURU_2, busB.D2);
  c.TYVA_3 = tock_pos(a.TYJU, b.TYJU, b.in.RESET2, b.TYVA_3, busB.D3);
  c.TYRU_4 = tock_pos(a.TYJU, b.TYJU, b.in.RESET2, b.TYRU_4, busB.D4);
  c.SUFY_5 = tock_pos(a.TYJU, b.TYJU, b.in.RESET2, b.SUFY_5, busB.D5);
  c.PETO_6 = tock_pos(a.TYJU, b.TYJU, b.in.RESET2, b.PETO_6, busB.D6);
  c.SETA_7 = tock_pos(a.TYJU, b.TYJU, b.in.RESET2, b.SETA_7, busB.D7);

  c.SETE = not(!b.SABU_0);
  c.PYRE = not(!b.NYKE_1);
  c.NOLA = not(!b.MURU_2);
  c.SALU = not(!b.TYVA_3);
  c.SUPO = not(!b.TYRU_4);
  c.SOTU = not(!b.SUFY_5);
  c.REVA = not(!b.PETO_6);
  c.SAPU = not(!b.SETA_7);

  c.TUBY = and(b.out.FF04_FF07, cpu.CPU_RD, busB.A1, b.TOVY);
  if (b.TUBY) {
    busC.D0 = b.SETE;
    busC.D1 = b.PYRE;
    busC.D2 = b.NOLA;
    busC.D3 = b.SALU;
    busC.D4 = b.SUPO;
    busC.D5 = b.SOTU;
    busC.D6 = b.REVA;
    busC.D7 = b.SAPU;
  }

  //----------
  // TIMA clock mux

  c.UVYR = not(b.in.CLK_64K);
  c.UKAP = mux2(b.in.CLK_16K, b.UVYR, b.SOPU_0);
  c.UBOT = not(b.in.CLK_256K);
  c.TEKO = mux2(b.UBOT, b.in.FF04_D1n, b.SOPU_0);
  c.TECY = mux2(b.UKAP, b.TEKO, b.SAMY_1);
  c.SOGU = nor(b.TECY, !b.SABO_2);

  //----------
  // TIMA reload signal

  c.MUZU = or(cpu.FROM_CPU5, b.TOPE);
  c.MEKE = not(b.out.INT_TIMER);
  c.MEXU = nand(b.MUZU, b.in.RESET2, b.MEKE);

  //----------
  // TIMA reload mux

  c.TOPE = nand(cpu.CPU_WR, b.out.FF04_FF07, busB.A0, b.in.TOLA_A1n);
  c.ROKE = mux2n(b.SABU_0, busB.D0, b.TOPE);
  c.PETU = mux2n(b.NYKE_1, busB.D1, b.TOPE);
  c.NYKU = mux2n(b.MURU_2, busB.D2, b.TOPE);
  c.SOCE = mux2n(b.TYVA_3, busB.D3, b.TOPE);
  c.SALA = mux2n(b.TYRU_4, busB.D4, b.TOPE);
  c.SYRU = mux2n(b.SUFY_5, busB.D5, b.TOPE);
  c.REFU = mux2n(b.PETO_6, busB.D6, b.TOPE);
  c.RATO = mux2n(b.SETA_7, busB.D7, b.TOPE);

  c.MULO = not(b.in.RESET2);
  c.PUXY = nor(b.MULO, b.ROKE);
  c.NERO = nor(b.MULO, b.PETU);
  c.NADA = nor(b.MULO, b.NYKU);
  c.REPA = nor(b.MULO, b.SOCE);
  c.ROLU = nor(b.MULO, b.SALA);
  c.RUGY = nor(b.MULO, b.SYRU);
  c.PYMA = nor(b.MULO, b.REFU);
  c.PAGU = nor(b.MULO, b.RATO);

  //----------
  // TIMA

  c.REGA_0 = b.REGA_0;
  c.POVY_1 = b.POVY_1;
  c.PERU_2 = b.PERU_2;
  c.RATE_3 = b.RATE_3;
  c.RUBY_4 = b.RUBY_4;
  c.RAGE_5 = b.RAGE_5;
  c.PEDA_6 = b.PEDA_6;
  c.NUGA_7 = b.NUGA_7;

  if (a.SOGU   && !b.SOGU)   { c.REGA_0 = !b.REGA_0; }
  if (a.REGA_0 && !b.REGA_0) { c.POVY_1 = !b.POVY_1; }
  if (a.POVY_1 && !b.POVY_1) { c.PERU_2 = !b.PERU_2; }
  if (a.PERU_2 && !b.PERU_2) { c.RATE_3 = !b.RATE_3; }
  if (a.RATE_3 && !b.RATE_3) { c.RUBY_4 = !b.RUBY_4; }
  if (a.RUBY_4 && !b.RUBY_4) { c.RAGE_5 = !b.RAGE_5; }
  if (a.RAGE_5 && !b.RAGE_5) { c.PEDA_6 = !b.PEDA_6; }
  if (a.PEDA_6 && !b.PEDA_6) { c.NUGA_7 = !b.NUGA_7; }

  if(b.MEXU) {
    c.REGA_0 = b.PUXY;
    c.POVY_1 = b.NERO;
    c.PERU_2 = b.NADA;
    c.RATE_3 = b.REPA;
    c.RUBY_4 = b.ROLU;
    c.RAGE_5 = b.RUGY;
    c.PEDA_6 = b.PYMA;
    c.NUGA_7 = b.PAGU;
  }

  c.SOKU_0 = not(!b.REGA_0);
  c.RACY_1 = not(!b.POVY_1);
  c.RAVY_2 = not(!b.PERU_2);
  c.SOSY_3 = not(!b.RATE_3);
  c.SOMU_4 = not(!b.RUBY_4);
  c.SURO_5 = not(!b.RAGE_5);
  c.ROWU_6 = not(!b.PEDA_6);
  c.PUSO_7 = not(!b.NUGA_7);

  c.TEDA = and(b.out.FF04_FF07, cpu.CPU_RD, b.in.TOLA_A1n, busB.A0);
  if (b.TEDA) {
    busC.D0 = b.SOKU_0;
    busC.D1 = b.RACY_1;
    busC.D2 = b.RAVY_2;
    busC.D3 = b.SOSY_3;
    busC.D4 = b.SOMU_4;
    busC.D5 = b.SURO_5;
    busC.D6 = b.ROWU_6; // schematic missing annotation
    busC.D7 = b.PUSO_7;
  }

  //----------
  // INT_TIMER delay

  c.MUGY = not(b.MEXU);
  c.NYDU = tock_pos(a.in.BOGA_1M, b.in.BOGA_1M, b.MUGY, b.NYDU, b.NUGA_7);
  c.MERY = nor(!b.NYDU, b.NUGA_7);
  c.MOBA = tock_pos(a.in.BOGA_1M, b.in.BOGA_1M, b.in.RESET2, b.MOBA, b.MERY);
  c.out.INT_TIMER = b.MOBA;
}
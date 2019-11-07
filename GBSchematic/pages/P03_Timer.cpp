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

void P03_Timer::tick(const Gameboy& ga, const Gameboy& gb, Gameboy& gc) {
  const P03_Timer& pa = ga.p03;
  const P03_Timer& pb = gb.p03;
  P03_Timer& pc = gc.p03;

  //----------
  // random decoder

  pc.RYFO = and(gb.cpu.A2, gb.A00_07, gb.FFXX);
  gc.FF04_FF07 = pb.RYFO;

  //----------
  // TAC

  pc.SARA = nand(gb.cpu.CPU_WR, gb.FF04_FF07, gb.cpu.A0, gb.cpu.A1);

  pc.SOPU_0 = tock_pos(pa.SARA, pb.SARA, gb.RESET2, pb.SOPU_0, gb.cpu.D0);
  pc.SAMY_1 = tock_pos(pa.SARA, pb.SARA, gb.RESET2, pb.SAMY_1, gb.cpu.D1);
  pc.SABO_2 = tock_pos(pa.SARA, pb.SARA, gb.RESET2, pb.SABO_2, gb.cpu.D2);

  pc.SORA = and(gb.cpu.CPU_RD, gb.FF04_FF07, gb.cpu.A1, gb.cpu.A0);
  pc.RYLA = not(!pb.SOPU_0);
  pc.ROTE = not(!pb.SAMY_1);
  pc.SUPE = not(!pb.SABO_2);

  if (pb.SORA) {
    gc.cpu.D2 = pb.SUPE;
    gc.cpu.D1 = pb.ROTE;
    gc.cpu.D0 = pb.RYLA;
  }

  //----------
  // TMA

  pc.TOVY = not(gb.cpu.A0);
  gc.TOVY_A0n = pb.TOVY;
  pc.TYJU = nand(pb.TOVY, gb.cpu.A1, gb.cpu.CPU_WR, gb.FF04_FF07);

  pc.SABU_0 = tock_pos(pa.TYJU, pb.TYJU, gb.RESET2, pb.SABU_0, gb.cpu.D0);
  pc.NYKE_1 = tock_pos(pa.TYJU, pb.TYJU, gb.RESET2, pb.NYKE_1, gb.cpu.D1);
  pc.MURU_2 = tock_pos(pa.TYJU, pb.TYJU, gb.RESET2, pb.MURU_2, gb.cpu.D2);
  pc.TYVA_3 = tock_pos(pa.TYJU, pb.TYJU, gb.RESET2, pb.TYVA_3, gb.cpu.D3);
  pc.TYRU_4 = tock_pos(pa.TYJU, pb.TYJU, gb.RESET2, pb.TYRU_4, gb.cpu.D4);
  pc.SUFY_5 = tock_pos(pa.TYJU, pb.TYJU, gb.RESET2, pb.SUFY_5, gb.cpu.D5);
  pc.PETO_6 = tock_pos(pa.TYJU, pb.TYJU, gb.RESET2, pb.PETO_6, gb.cpu.D6);
  pc.SETA_7 = tock_pos(pa.TYJU, pb.TYJU, gb.RESET2, pb.SETA_7, gb.cpu.D7);

  pc.SETE = not(!pb.SABU_0);
  pc.PYRE = not(!pb.NYKE_1);
  pc.NOLA = not(!pb.MURU_2);
  pc.SALU = not(!pb.TYVA_3);
  pc.SUPO = not(!pb.TYRU_4);
  pc.SOTU = not(!pb.SUFY_5);
  pc.REVA = not(!pb.PETO_6);
  pc.SAPU = not(!pb.SETA_7);

  pc.TUBY = and(gb.FF04_FF07, gb.cpu.CPU_RD, gb.cpu.A1, pb.TOVY);
  if (pb.TUBY) {
    gc.cpu.D0 = pb.SETE;
    gc.cpu.D1 = pb.PYRE;
    gc.cpu.D2 = pb.NOLA;
    gc.cpu.D3 = pb.SALU;
    gc.cpu.D4 = pb.SUPO;
    gc.cpu.D5 = pb.SOTU;
    gc.cpu.D6 = pb.REVA;
    gc.cpu.D7 = pb.SAPU;
  }

  //----------
  // TIMA clock mux

  pc.UVYR = not(gb.CLK_64K);
  pc.UKAP = mux2(gb.CLK_16K, pb.UVYR, pb.SOPU_0);
  pc.UBOT = not(gb.CLK_256K);
  pc.TEKO = mux2(pb.UBOT, gb.FF04_D1n, pb.SOPU_0);
  pc.TECY = mux2(pb.UKAP, pb.TEKO, pb.SAMY_1);
  pc.SOGU = nor(pb.TECY, !pb.SABO_2);

  //----------
  // TIMA reload signal

  pc.MUZU = or(gb.cpu.FROM_CPU5, pb.TOPE);
  pc.MEKE = not(gb.INT_TIMER);
  pc.MEXU = nand(pb.MUZU, gb.RESET2, pb.MEKE);

  //----------
  // TIMA reload mux

  pc.TOPE = nand(gb.cpu.CPU_WR, gb.FF04_FF07, gb.cpu.A0, gb.TOLA_A1n);
  pc.ROKE = mux2n(pb.SABU_0, gb.cpu.D0, pb.TOPE);
  pc.PETU = mux2n(pb.NYKE_1, gb.cpu.D1, pb.TOPE);
  pc.NYKU = mux2n(pb.MURU_2, gb.cpu.D2, pb.TOPE);
  pc.SOCE = mux2n(pb.TYVA_3, gb.cpu.D3, pb.TOPE);
  pc.SALA = mux2n(pb.TYRU_4, gb.cpu.D4, pb.TOPE);
  pc.SYRU = mux2n(pb.SUFY_5, gb.cpu.D5, pb.TOPE);
  pc.REFU = mux2n(pb.PETO_6, gb.cpu.D6, pb.TOPE);
  pc.RATO = mux2n(pb.SETA_7, gb.cpu.D7, pb.TOPE);

  pc.MULO = not(gb.RESET2);
  pc.PUXY = nor(pb.MULO, pb.ROKE);
  pc.NERO = nor(pb.MULO, pb.PETU);
  pc.NADA = nor(pb.MULO, pb.NYKU);
  pc.REPA = nor(pb.MULO, pb.SOCE);
  pc.ROLU = nor(pb.MULO, pb.SALA);
  pc.RUGY = nor(pb.MULO, pb.SYRU);
  pc.PYMA = nor(pb.MULO, pb.REFU);
  pc.PAGU = nor(pb.MULO, pb.RATO);

  //----------
  // TIMA

  pc.REGA_0 = pb.REGA_0;
  pc.POVY_1 = pb.POVY_1;
  pc.PERU_2 = pb.PERU_2;
  pc.RATE_3 = pb.RATE_3;
  pc.RUBY_4 = pb.RUBY_4;
  pc.RAGE_5 = pb.RAGE_5;
  pc.PEDA_6 = pb.PEDA_6;
  pc.NUGA_7 = pb.NUGA_7;

  if (pa.SOGU   && !pb.SOGU)   { pc.REGA_0 = !pb.REGA_0; }
  if (pa.REGA_0 && !pb.REGA_0) { pc.POVY_1 = !pb.POVY_1; }
  if (pa.POVY_1 && !pb.POVY_1) { pc.PERU_2 = !pb.PERU_2; }
  if (pa.PERU_2 && !pb.PERU_2) { pc.RATE_3 = !pb.RATE_3; }
  if (pa.RATE_3 && !pb.RATE_3) { pc.RUBY_4 = !pb.RUBY_4; }
  if (pa.RUBY_4 && !pb.RUBY_4) { pc.RAGE_5 = !pb.RAGE_5; }
  if (pa.RAGE_5 && !pb.RAGE_5) { pc.PEDA_6 = !pb.PEDA_6; }
  if (pa.PEDA_6 && !pb.PEDA_6) { pc.NUGA_7 = !pb.NUGA_7; }

  if(pb.MEXU) {
    pc.REGA_0 = pb.PUXY;
    pc.POVY_1 = pb.NERO;
    pc.PERU_2 = pb.NADA;
    pc.RATE_3 = pb.REPA;
    pc.RUBY_4 = pb.ROLU;
    pc.RAGE_5 = pb.RUGY;
    pc.PEDA_6 = pb.PYMA;
    pc.NUGA_7 = pb.PAGU;
  }

  pc.SOKU_0 = not(!pb.REGA_0);
  pc.RACY_1 = not(!pb.POVY_1);
  pc.RAVY_2 = not(!pb.PERU_2);
  pc.SOSY_3 = not(!pb.RATE_3);
  pc.SOMU_4 = not(!pb.RUBY_4);
  pc.SURO_5 = not(!pb.RAGE_5);
  pc.ROWU_6 = not(!pb.PEDA_6);
  pc.PUSO_7 = not(!pb.NUGA_7);

  pc.TEDA = and(gb.FF04_FF07, gb.cpu.CPU_RD, gb.TOLA_A1n, gb.cpu.A0);
  if (pb.TEDA) {
    gc.cpu.D0 = pb.SOKU_0;
    gc.cpu.D1 = pb.RACY_1;
    gc.cpu.D2 = pb.RAVY_2;
    gc.cpu.D3 = pb.SOSY_3;
    gc.cpu.D4 = pb.SOMU_4;
    gc.cpu.D5 = pb.SURO_5;
    gc.cpu.D6 = pb.ROWU_6; // schematic missing annotation
    gc.cpu.D7 = pb.PUSO_7;
  }

  //----------
  // INT_TIMER delay

  pc.MUGY = not(pb.MEXU);
  pc.NYDU = tock_pos(ga.BOGA_1M, gb.BOGA_1M, pb.MUGY, pb.NYDU, pb.NUGA_7);
  pc.MERY = nor(!pb.NYDU, pb.NUGA_7);
  pc.MOBA = tock_pos(ga.BOGA_1M, gb.BOGA_1M, gb.RESET2, pb.MOBA, pb.MERY);
  gc.INT_TIMER = pb.MOBA;
}
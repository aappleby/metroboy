// This file should contain the schematics as directly translated to C, no modifications or simplifications

#include "Timer_sch.h"
#include "Schematics.h"
#include "AddressDecoder.h"
#include "MemBus.h"
#include "CpuBus.h"
#include "Clocks.h"

//-----------------------------------------------------------------------------

void Timer::tick(bool FF04_D1n) {
  bool RYFO = and(mem.A2, A00_07, FFXX);
  FF04_FF07 = RYFO;
  bool TOPE = nand(CPU_WR, FF04_FF07, mem.A0, TOLA_A1n);
  bool MUZU = or(cpu.FROM_CPU5, TOPE);
  bool MEKE = not(INT_TIMER);
  bool MEXU = nand(MUZU, RESET2, MEKE);

  //----------
  // tac

  bool SARA = nand(CPU_WR, FF04_FF07, mem.A0, mem.A1);
  bool SORA = and(CPU_RD, FF04_FF07, mem.A1, mem.A0);
  bool SABO_Q = SABO.tock(SARA, RESET2, mem.D2);
  bool SAMY_Q = SAMY.tock(SARA, RESET2, mem.D1);
  bool SOPU_Q = SOPU.tock(SARA, RESET2, mem.D0);

  bool SUPE = not(!SABO_Q);
  bool ROTE = not(!SAMY_Q);
  bool RYLA = not(!SOPU_Q);

  if (SORA) {
    mem.D2 = SUPE;
    mem.D1 = ROTE;
    mem.D0 = RYLA;
  }

  // clock mux
  bool UVYR = not(CLK_64k);
  bool UKAP = mux2(CLK_16k, UVYR, SOPU_Q);
  bool UBOT = not(CLK_256k);
  bool TEKO = mux2(UBOT, FF04_D1n, SOPU_Q);
  bool TECY = mux2(UKAP, TEKO, SAMY_Q);
  bool SOGU = nor(TECY, !SABO_Q);

  //----------
  // tma

  bool TOVY = not(mem.A0);
  TOVY_A0n = TOVY;
  bool TUBY = and(FF04_FF07, CPU_RD, mem.A1, TOVY);
  bool TYJU = nand(TOVY, mem.A1, CPU_WR, FF04_FF07);

  bool PETO_Q = PETO.tock(TYJU, RESET2, mem.D6);
  bool MURU_Q = MURU.tock(TYJU, RESET2, mem.D2);
  bool NYKE_Q = NYKE.tock(TYJU, RESET2, mem.D1);
  bool SETA_Q = SETA.tock(TYJU, RESET2, mem.D7);
  bool SABU_Q = SABU.tock(TYJU, RESET2, mem.D0);
  bool TYRU_Q = TYRU.tock(TYJU, RESET2, mem.D4);
  bool SUFY_Q = SUFY.tock(TYJU, RESET2, mem.D5);
  bool TYVA_Q = TYVA.tock(TYJU, RESET2, mem.D3);

  bool REVA = not(!PETO_Q);
  bool NOLA = not(!MURU_Q);
  bool PYRE = not(!NYKE_Q);
  bool SAPU = not(!SETA_Q);
  bool SETE = not(!SABU_Q);
  bool SUPO = not(!TYRU_Q);
  bool SOTU = not(!SUFY_Q);
  bool SALU = not(!TYVA_Q);

  if (TUBY) {
    mem.D6 = REVA;
    mem.D2 = NOLA;
    mem.D1 = PYRE;
    mem.D7 = SAPU;
    mem.D0 = SETE;
    mem.D4 = SUPO;
    mem.D5 = SOTU;
    mem.D3 = SALU;
  }

  bool REFU = mux2(PETO_Q, mem.D6, TOPE);
  bool NYKU = mux2(MURU_Q, mem.D2, TOPE);
  bool PETU = mux2(NYKE_Q, mem.D1, TOPE);
  bool RATO = mux2(SETA_Q, mem.D7, TOPE);
  bool ROKE = mux2(SABU_Q, mem.D0, TOPE);
  bool SALA = mux2(TYRU_Q, mem.D4, TOPE);
  bool SYRU = mux2(SUFY_Q, mem.D5, TOPE);
  bool SOCE = mux2(TYVA_Q, mem.D3, TOPE);

  bool MULO = not(RESET2);

  bool PYMA = nor(MULO, REFU);
  bool NADA = nor(MULO, NYKU);
  bool NERO = nor(MULO, PETU);
  bool PAGU = nor(MULO, RATO);
  bool PUXY = nor(MULO, ROKE);
  bool ROLU = nor(MULO, SALA);
  bool RUGY = nor(MULO, SYRU);
  bool REPA = nor(MULO, SOCE);

  //----------
  // tima

  bool TEDA = and(FF04_FF07, CPU_RD, TOLA_A1n, mem.A0);

  bool REGA_Q = REGA.count(SOGU,   MEXU, PUXY);
  bool POVY_Q = POVY.count(REGA_Q, MEXU, NERO);
  bool PERU_Q = PERU.count(POVY_Q, MEXU, NADA);
  bool RATE_Q = RATE.count(PERU_Q, MEXU, REPA);
  bool RUBY_Q = RUBY.count(RATE_Q, MEXU, ROLU);
  bool RAGE_Q = RAGE.count(RUBY_Q, MEXU, RUGY);
  bool PEDA_Q = PEDA.count(RAGE_Q, MEXU, PYMA);
  bool NUGA_Q = NUGA.count(PEDA_Q, MEXU, PAGU);

  bool SOKU = not(!REGA_Q);
  bool RACY = not(!POVY_Q);
  bool RAVY = not(!PERU_Q);
  bool SOSY = not(!RATE_Q);
  bool SOMU = not(!RUBY_Q);
  bool SURO = not(!RAGE_Q);
  bool ROWU = not(!PEDA_Q);
  bool PUSO = not(!NUGA_Q);

  if (TEDA) {
    mem.D0 = SOKU;
    mem.D1 = RACY;
    mem.D2 = RAVY;
    mem.D3 = SOSY;
    mem.D4 = SOMU;
    mem.D5 = SURO;
    mem.D6 = ROWU; // schematic missing annotation
    mem.D7 = PUSO;
  }

  bool MUGY = not(MEXU);
  bool NYDU_Q = NYDU.tock(BOGA1MHZ, MUGY, NUGA_Q);
  bool MERY = nor(!NYDU_Q, NUGA_Q);
  bool MOBA_Q = MOBA.tock(BOGA1MHZ, RESET2, MERY);
  INT_TIMER = MOBA_Q;
}
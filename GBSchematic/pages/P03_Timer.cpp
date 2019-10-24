#include "../Schematics.h"

//-----------------------------------------------------------------------------
// This file should contain the schematics as directly translated to C,
// no modifications or simplifications.

struct P3_Timer {

  struct Input {
    bool A00_07;
    bool FFXX;
    bool TOLA_A1n;
    bool CPU_WR;
    bool CPU_RD;
    bool FROM_CPU5;
    bool INT_TIMER;
    bool A0,A1,A2,A3,A4,A5,A6,A7;
    bool D0,D1,D2,D3,D4,D5,D6,D7;
    bool RESET2;
    bool CLK_64k;
    bool CLK_16k;
    bool CLK_256k;
    bool FF04_D1n;
    bool BOGA1MHZ;
  };

  struct Output {
    bool INT_TIMER;
    bool TOVY_A0n;
    bool FF04_FF07;
    bool OE;
    bool D0,D1,D2,D3,D4,D5,D6,D7;
  };

  // FF05 TIMA
  reg REGA,POVY,PERU,RATE,RUBY,RAGE,PEDA,NUGA;

  // FF06 TMA
  reg PETO,MURU,NYKE,SETA,SABU,TYRU,SUFY,TYVA;

  // FF07 TAC
  reg SABO,SAMY,SOPU;

  // INT_TIMER delay
  reg NYDU,MOBA;

  //----------------------------------------

  void tick(Input const& in, Output& out) {
    out = {};

    wire RYFO = and(in.A2, in.A00_07, in.FFXX);
    wire FF04_FF07 = RYFO;
    out.FF04_FF07 = RYFO;

    wire TOPE = nand(in.CPU_WR, FF04_FF07, in.A0, in.TOLA_A1n);
    wire MUZU = or(in.FROM_CPU5, TOPE);
    wire MEKE = not(in.INT_TIMER);
    wire MEXU = nand(MUZU, in.RESET2, MEKE);

    //----------
    // tac

    wire SARA = nand(in.CPU_WR, FF04_FF07, in.A0, in.A1);
    wire SABO_Q = SABO.tock(SARA, in.RESET2, in.D2);
    wire SAMY_Q = SAMY.tock(SARA, in.RESET2, in.D1);
    wire SOPU_Q = SOPU.tock(SARA, in.RESET2, in.D0);

    wire SUPE = not(!SABO_Q);
    wire ROTE = not(!SAMY_Q);
    wire RYLA = not(!SOPU_Q);

    // clock mux
    wire UVYR = not(in.CLK_64k);
    wire UKAP = mux2(in.CLK_16k, UVYR, SOPU_Q);
    wire UBOT = not(in.CLK_256k);
    wire TEKO = mux2(UBOT, in.FF04_D1n, SOPU_Q);
    wire TECY = mux2(UKAP, TEKO, SAMY_Q);
    wire SOGU = nor(TECY, !SABO_Q);

    wire SORA = and(in.CPU_RD, FF04_FF07, in.A1, in.A0);
    if (SORA) {
      out.OE = true;
      out.D2 = SUPE;
      out.D1 = ROTE;
      out.D0 = RYLA;
    }

    //----------
    // tma

    wire TOVY = not(in.A0);
    out.TOVY_A0n = TOVY;

    wire TUBY = and(FF04_FF07, in.CPU_RD, in.A1, TOVY);
    wire TYJU = nand(TOVY, in.A1, in.CPU_WR, FF04_FF07);

    wire PETO_Q = PETO.tock(TYJU, in.RESET2, in.D6);
    wire MURU_Q = MURU.tock(TYJU, in.RESET2, in.D2);
    wire NYKE_Q = NYKE.tock(TYJU, in.RESET2, in.D1);
    wire SETA_Q = SETA.tock(TYJU, in.RESET2, in.D7);
    wire SABU_Q = SABU.tock(TYJU, in.RESET2, in.D0);
    wire TYRU_Q = TYRU.tock(TYJU, in.RESET2, in.D4);
    wire SUFY_Q = SUFY.tock(TYJU, in.RESET2, in.D5);
    wire TYVA_Q = TYVA.tock(TYJU, in.RESET2, in.D3);

    wire REVA = not(!PETO_Q);
    wire NOLA = not(!MURU_Q);
    wire PYRE = not(!NYKE_Q);
    wire SAPU = not(!SETA_Q);
    wire SETE = not(!SABU_Q);
    wire SUPO = not(!TYRU_Q);
    wire SOTU = not(!SUFY_Q);
    wire SALU = not(!TYVA_Q);

    wire REFU = mux2(PETO_Q, in.D6, TOPE);
    wire NYKU = mux2(MURU_Q, in.D2, TOPE);
    wire PETU = mux2(NYKE_Q, in.D1, TOPE);
    wire RATO = mux2(SETA_Q, in.D7, TOPE);
    wire ROKE = mux2(SABU_Q, in.D0, TOPE);
    wire SALA = mux2(TYRU_Q, in.D4, TOPE);
    wire SYRU = mux2(SUFY_Q, in.D5, TOPE);
    wire SOCE = mux2(TYVA_Q, in.D3, TOPE);

    wire MULO = not(in.RESET2);

    wire PYMA = nor(MULO, REFU);
    wire NADA = nor(MULO, NYKU);
    wire NERO = nor(MULO, PETU);
    wire PAGU = nor(MULO, RATO);
    wire PUXY = nor(MULO, ROKE);
    wire ROLU = nor(MULO, SALA);
    wire RUGY = nor(MULO, SYRU);
    wire REPA = nor(MULO, SOCE);

    if (TUBY) {
      out.OE = true;
      out.D6 = REVA;
      out.D2 = NOLA;
      out.D1 = PYRE;
      out.D7 = SAPU;
      out.D0 = SETE;
      out.D4 = SUPO;
      out.D5 = SOTU;
      out.D3 = SALU;
    }

    //----------
    // tima

    // FIXME daisy-chained counter, count() won't work
    wire REGA_Q = REGA.count(SOGU,   MEXU, PUXY);
    wire POVY_Q = POVY.count(REGA_Q, MEXU, NERO);
    wire PERU_Q = PERU.count(POVY_Q, MEXU, NADA);
    wire RATE_Q = RATE.count(PERU_Q, MEXU, REPA);
    wire RUBY_Q = RUBY.count(RATE_Q, MEXU, ROLU);
    wire RAGE_Q = RAGE.count(RUBY_Q, MEXU, RUGY);
    wire PEDA_Q = PEDA.count(RAGE_Q, MEXU, PYMA);
    wire NUGA_Q = NUGA.count(PEDA_Q, MEXU, PAGU);

    wire SOKU = not(!REGA_Q);
    wire RACY = not(!POVY_Q);
    wire RAVY = not(!PERU_Q);
    wire SOSY = not(!RATE_Q);
    wire SOMU = not(!RUBY_Q);
    wire SURO = not(!RAGE_Q);
    wire ROWU = not(!PEDA_Q);
    wire PUSO = not(!NUGA_Q);

    wire TEDA = and(FF04_FF07, in.CPU_RD, in.TOLA_A1n, in.A0);
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
    // delay thing

    wire MUGY = not(MEXU);
    wire NYDU_Q = NYDU.tock(in.BOGA1MHZ, MUGY, NUGA_Q);
    wire MERY = nor(!NYDU_Q, NUGA_Q);
    wire MOBA_Q = MOBA.tock(in.BOGA1MHZ, in.RESET2, MERY);

    out.INT_TIMER = MOBA_Q;
  }
};


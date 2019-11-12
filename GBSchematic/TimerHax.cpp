#include "Schematics.h"

//-----------------------------------------------------------------------------
// This file should contain the schematics as directly translated to C,
// no modifications or simplifications.

struct P3_Timer {

  struct Input {
    bool CLK_64k;
    bool CLK_16k;
    bool CLK_256k;
    bool RESET2;

    bool CPU_RD;
    bool CPU_WR;

    bool FROM_CPU5;

    bool A00_07;
    bool FFXX;
    bool TOLA_A1n;

    bool INT_TIMER;

    uint16_t addr;
    uint8_t data;

    bool A0,A1,A2,A3,A4,A5,A6,A7;
    bool D0,D1,D2,D3,D4,D5,D6,D7;

    bool FF04_D1n;
    bool BOGA1MHZ;
  };

  // FF05 TIMA
  counter REGA,POVY,PERU,RATE,RUBY,RAGE,PEDA,NUGA;

  // FF06 TMA
  reg PETO,MURU,NYKE,SETA,SABU,TYRU,SUFY,TYVA;

  // FF07 TAC
  reg SABO,SAMY,SOPU;

  // INT_TIMER delay
  reg NYDU,MOBA;

  //----------------------------------------

  void tick(Input const& in) {
    bool SABO_Q = SABO.q();
    bool SAMY_Q = SAMY.q();
    bool SOPU_Q = SOPU.q();

    bool PETO_Q = PETO.q();
    bool MURU_Q = MURU.q();
    bool NYKE_Q = NYKE.q();
    bool SETA_Q = SETA.q();
    bool SABU_Q = SABU.q();
    bool TYRU_Q = TYRU.q();
    bool SUFY_Q = SUFY.q();
    bool TYVA_Q = TYVA.q();

    bool NUGA_CARRY = NUGA.carry();

    bool NYDU_Q = NYDU.q();
    bool MOBA_Q = MOBA.q();

    //----------
    // tac

    bool SARA = nand(in.CPU_WR, in.addr == 0xFF07);
    SABO.tock(SARA, in.RESET2, in.D2);
    SAMY.tock(SARA, in.RESET2, in.D1);
    SOPU.tock(SARA, in.RESET2, in.D0);

    //----------
    // tma

    bool TYJU = nand(in.CPU_WR, in.addr == 0xFF06);

    PETO.tock(TYJU, in.D6);
    MURU.tock(TYJU, in.D2);
    NYKE.tock(TYJU, in.D1);
    SETA.tock(TYJU, in.D7);
    SABU.tock(TYJU, in.D0);
    TYRU.tock(TYJU, in.D4);
    SUFY.tock(TYJU, in.D5);
    TYVA.tock(TYJU, in.D3);

    if (!in.RESET2) {
      PETO.reset();
      MURU.reset();
      NYKE.reset();
      SETA.reset();
      SABU.reset();
      TYRU.reset();
      SUFY.reset();
      TYVA.reset();
    }

    //----------
    // tima

    bool UKAP = mux2(in.CLK_16k, not(in.CLK_64k), SOPU_Q);
    bool TEKO = mux2(not(in.CLK_256k), in.FF04_D1n, SOPU_Q);
    bool TECY = mux2(UKAP, TEKO, SAMY_Q);
    bool SOGU = and(not(TECY), SABO_Q);

    REGA.count(SOGU);
    POVY.count(REGA.q());
    PERU.count(POVY.q());
    RATE.count(PERU.q());
    RUBY.count(RATE.q());
    RAGE.count(RUBY.q());
    PEDA.count(RAGE.q());
    NUGA.count(PEDA.q());

    bool MEXU = or(
      and(
        not(in.FROM_CPU5),
        in.CPU_WR,
        in.addr == 0xFF05
      ),
      not(in.RESET2),
      MOBA_Q
    );

    if (MEXU) {
      REGA.load(not(SABU_Q));
      POVY.load(not(NYKE_Q));
      PERU.load(not(MURU_Q));
      RATE.load(not(TYVA_Q));
      RUBY.load(not(TYRU_Q));
      RAGE.load(not(SUFY_Q));
      PEDA.load(not(PETO_Q));
      NUGA.load(not(SETA_Q));

      if (in.CPU_WR && in.addr == 0xFF05) {
        REGA.load(not(in.D0));
        POVY.load(not(in.D1));
        PERU.load(not(in.D2));
        RATE.load(not(in.D3));
        RUBY.load(not(in.D4));
        RAGE.load(not(in.D5));
        PEDA.load(not(in.D6));
        NUGA.load(not(in.D7));
      }

      if (!in.RESET2) {
        REGA.load(0);
        POVY.load(0);
        PERU.load(0);
        RATE.load(0);
        RUBY.load(0);
        RAGE.load(0);
        PEDA.load(0);
        NUGA.load(0);
      }
    }

    //----------
    // delay thing

    NYDU.tock(in.BOGA1MHZ, NUGA_CARRY);
    MOBA.tock(in.BOGA1MHZ, and(NYDU_Q, not(NUGA_CARRY)));

    bool MUGY = not(MEXU);
    if (!MUGY) {
      NYDU.reset();
    }

    if (!in.RESET2) {
      MOBA.reset();
    }

    //out.INT_TIMER = MOBA_Q;
  }
};


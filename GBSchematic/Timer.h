#pragma once
#include "Schematics.h"
#include "../src/TextPainter.h"

struct Timer {

  struct Input {
    bool RESET;       // active high!

    bool CLK_GOOD;    // {CLKIN_A}
    bool CLKIN_B;     // the master 4mhz clock

    bool INT_TIMER;

    bool CPU_RD;
    bool CPU_WR;

    bool CPU_RESET;   // {ABOL}
    bool AJER_2MHZ;   // on apu sheet

    bool FROM_CPU3;   // controls CPU_RD_SYNC?
    bool FROM_CPU4;   // only used by ABUZ
    bool FROM_CPU5;   // affects TIMA reload?

    uint16_t addr;
  
    bool T1T2n;       // true for debugging
    bool T1nT2;       // true for debugging
    bool T1nT2n;      // true for normal mode
    
    bool FF04_D1n;
    bool FF40_D7;     // lcd on
    bool FF60_D1;     // debugging

    bool A0,A1,A2,A3,A4,A5,A6,A7;
    bool D0,D1,D2,D3,D4,D5,D6,D7;
  };

  struct Output {
    bool INT_TIMER;

    bool TO_CPU;  // or(in.CPU_RESET, BOGA1MHZ)
    bool BUKE;    // nor(PHASE_B_Q, not(PHASE_C_Q), in.CPU_RESET)
    bool ABUZ;    // nor(in.T1nT2, unk3(CLK_ABCD_Q, not(PHASE_C_Q), in.FROM_CPU4))
    bool AFAS;    // and(PHASE_D_Q, CLK_ABCD_Q)

    bool D_OE;
    bool D0,D1,D2,D3,D4,D5,D6,D7;
};

  // 1mhz phase generator
  //reg AFUR,ALEF,APUK,ADYK;
  reg CLK_ABCD;
  reg CLK_BCDE;
  reg CLK_CDEF;
  reg CLK_DEFG;

  // register for the RESET2 signal, roughly
  reg RESET_REG;

  // div
  reg DIV_0,DIV_1,DIV_2,DIV_3,DIV_4,DIV_5;
  reg DIV_6,DIV_7,DIV_8,DIV_9,DIV_A,DIV_B,DIV_C,DIV_D,DIV_E,DIV_F;

  // FF05 TIMA
  //reg REGA,POVY,PERU,RATE,RUBY,RAGE,PEDA,NUGA;
  reg TIMA_0,TIMA_1,TIMA_2,TIMA_3,TIMA_4,TIMA_5,TIMA_6,TIMA_7;

  // FF06 TMA
  //reg SABU,NYKE,MURU,TYVA,TYRU,SUFY,PETO,SETA;
  reg TMA_0,TMA_1,TMA_2,TMA_3,TMA_4,TMA_5,TMA_6,TMA_7;

  // FF07 TAC
  //reg SOPU,SAMY,SABO;
  reg TAC_0,TAC_1,TAC_2;

  // INT_TIMER delay
  //reg NYDU,MOBA;
  reg INT_TIMER_DELAY_0;
  reg INT_TIMER_DELAY_1;

  Output tock1(const Input& in, TextPainter& tp);
};

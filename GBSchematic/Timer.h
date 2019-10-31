#pragma once
#include "Schematics.h"
#include "../src/TextPainter.h"
#include "SignalTree.h"

struct Timer {

  struct Input {
    bool RESET;       // active high!

    bool CLK_GOOD;    // {CLKIN_A}
    bool CLKIN_B;     // the master 4mhz clock

    bool INT_TIMER;

    bool CPU_RD;  // need to derive these from master clock
    bool CPU_WR;

    bool CPU_RESET;   // {ABOL}

    bool FROM_CPU3;   // controls CPU_RD_SYNC?
    bool FROM_CPU4;   // only used by ABUZ
    bool FROM_CPU5;   // affects TIMA reload?

    uint16_t addr;
    uint8_t  data;
  };

  struct Output {
    bool INT_TIMER;

    bool TO_CPU;  // or(in.CPU_RESET, BOGA1MHZ)
    bool BUKE;    // nor(PHASE_B_Q, not(PHASE_C_Q), in.CPU_RESET)
    bool ABUZ;    // nor(in.T1nT2, unk3(CLK_ABCD_Q, not(PHASE_C_Q), in.FROM_CPU4))
    bool AFAS;    // and(PHASE_D_Q, CLK_ABCD_Q)

    uint8_t data;
    bool    data_oe;
  };


  //----------

  Output tock1(const Input& in);
  void tock_phase_generator(bool CLKIN_B);
  bool tick_BOGA1MHZ(bool CPU_RESET, bool CLK_GOOD) const;
  bool tick_RESET2(bool CPU_RESET, bool RESET, bool CLK_GOOD);

  static std::vector<SignalData> signals1();
  static std::vector<SignalData> signals2();

  //----------

  // 1mhz phase generator
  reg CLK_ABCD;
  reg CLK_BCDE;
  reg CLK_CDEF;
  reg CLK_DEFG;

  // master reset
  reg   RESET_REG;

  // timer
  reg16  DIV;
  count8 TIMA;
  reg8   TMA;
  reg8   TAC;
  reg    INT_TIMER_DELAY_0;
  reg    INT_TIMER_DELAY_1;
};

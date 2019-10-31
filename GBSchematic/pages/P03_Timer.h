#pragma once
#include "../Schematics.h"
#include "../SignalTree.h"

//-----------------------------------------------------------------------------
// This file should contain the schematics as directly translated to C,
// no modifications or simplifications.

struct P03_Timer {

  struct Input {
    bool CLK_64K;
    bool CLK_16K;
    bool CLK_256K;
    bool RESET2;

    bool CPU_RD;
    bool CPU_WR;

    bool FROM_CPU5;  // what does this do?

    bool A00_07;
    bool FFXX;
    bool TOLA_A1n;

    bool INT_TIMER;

    bool A0,A1,A2,A3,A4,A5,A6,A7;
    bool D0,D1,D2,D3,D4,D5,D6,D7;

    bool FF04_D1n;
    bool BOGA_1M;
  };

  struct Output {
    bool INT_TIMER;
    bool TOVY_A0n;
    bool FF04_FF07;
    bool OE;
    bool D0,D1,D2,D3,D4,D5,D6,D7;
  };

  Input in;
  Output out;

  bool SOGU,TYJU;

  // Logic cells

  bool UVYR,UKAP,UBOT,TEKO,TECY;
  bool SARA,SORA,SUPE,ROTE,RYLA;

  // FF05 TIMA
  bool REGA_0,POVY_1,PERU_2,RATE_3,RUBY_4,RAGE_5,PEDA_6,NUGA_7;
  bool NUGA_C;

  // FF06 TMA
  bool SABU_0,NYKE_1,MURU_2,TYVA_3,TYRU_4,SUFY_5,PETO_6,SETA_7;

  // FF07 TAC
  bool SOPU_0,SAMY_1,SABO_2;

  // INT_TIMER delay
  bool NYDU,MOBA;

  static const std::vector<SignalData> signals();

  //----------------------------------------

  void tick(const P03_Timer& prev);
};


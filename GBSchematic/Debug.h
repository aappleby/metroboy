#pragma once
#include "Schematics.h"

struct DebugBus {
  bool T1nT2n;
  bool T1nT2;
  bool T1T2n;
  bool NET01; // not(T1nT2)

  void tick(bool RESET, bool T1, bool T2) {
    bool UBET = not(T1);
    bool UVAR = not(T2);
    bool UPOJ = nand(UBET, UVAR, RESET);
    bool UNOR = and(T2, UBET);
    bool UMUT = and(T1, UVAR);

    T1nT2n = UPOJ;
    T1nT2 = UNOR;
    T1T2n = UMUT;
  }
};

extern DebugBus dbg;
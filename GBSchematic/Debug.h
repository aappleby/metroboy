#pragma once
#include "Schematics.h"

struct DebugBus {
  bool T1nT2n;
  bool T1nT2;
  bool T1T2n;
  bool NET01;
  bool NET02;

  void tick(bool RESET, bool T1, bool T2) {
    T1nT2n = nand(not(T1), not(T2), RESET);
    T1nT2  = and(not(T1), T2);
    T1T2n  = and(T1, not(T2));
    NET01  = not(T1nT2);
    NET02  = T1nT2;
  }
};

extern DebugBus dbg;
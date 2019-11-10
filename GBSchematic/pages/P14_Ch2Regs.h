#pragma once

struct P14_Ch2Regs {
  bool BENY_OUT;
  bool FF19_D6;
  bool FF19_D7;

  union { bool HERO; bool CH2_FTICK; };
  union { bool AGYN; bool FF16_WR; };
};
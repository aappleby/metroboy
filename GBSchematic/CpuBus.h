#pragma once

struct CpuBus {
  bool FROM_CPU;
  // no FROM_CPU2 - maybe in APU?
  bool FROM_CPU3;
  bool FROM_CPU4;
  bool FROM_CPU5;
  bool FROM_CPU6;
  bool FROM_CPU7;
  bool FROM_CPU8;
  bool FROM_CPU9;
  bool FROM_CPU10;

  bool TO_CPU;
  bool TO_CPU2;
};

extern CpuBus cpu;

extern bool CPU_RD;
extern bool CPU_RDn;
extern bool CPU_RD2;
extern bool CPU_RAW_RD;
extern bool CPU_RD_SYNC; // not directly from cpu

extern bool CPU_WR;
extern bool CPU_WR2;
extern bool CPU_WRQ; // schematic has a question mark?
extern bool CPU_WR_RAW;

extern bool INT_VBL_BUF;
extern bool INT_STAT;
extern bool INT_TIMER;
extern bool INT_JP;
extern bool INT_SERIAL;

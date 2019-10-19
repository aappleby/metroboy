#pragma once

struct CpuBus {
  bool CPU_RD;
  bool CPU_RDn;
  bool CPU_RD2;
  bool CPU_RAW_RD;
  bool CPU_RD_SYNC; // not directly from cpu

  bool CPU_WR;
  bool CPU_WR2;
  bool CPU_WRQ; // schematic has a question mark?
  bool CPU_WR_RAW;

  bool INT_VBL_BUF;
  bool INT_STAT;
  bool INT_TIMER;
  bool INT_JP;
  bool INT_SERIAL;

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


#pragma once

extern bool CPU_RD;
extern bool CPU_RDn;
extern bool CPU_RD2;
extern bool CPU_RAW_RD;
extern bool CPU_RD_SYNC; // not directly from cpu

extern bool CPU_WR;
extern bool CPU_WR2;
extern bool CPU_WRQ; // schematic has a question mark?
extern bool CPU_WR_RAW;

extern bool FROM_CPU;
// no FROM_CPU2 - maybe in APU?
extern bool FROM_CPU3;
extern bool FROM_CPU4;
extern bool FROM_CPU5;
extern bool FROM_CPU6;
extern bool FROM_CPU7;
extern bool FROM_CPU8;
extern bool FROM_CPU9;
extern bool FROM_CPU10;

extern bool INT_VBL_BUF;
extern bool INT_STAT;
extern bool INT_TIMER;
extern bool INT_JP;
extern bool INT_SERIAL;

extern bool TO_CPU;
extern bool TO_CPU2;


#include "CpuBus.h"

CpuBus cpu;

bool CPU_RD;
bool CPU_RD2;
bool CPU_RDn;
bool CPU_RAW_RD;
bool CPU_RD_SYNC;

bool CPU_WR;
bool CPU_WR2;
bool CPU_WR_RAW;
bool CPU_WRQ; // schematic has a question mark

bool INT_VBL_BUF;
bool INT_STAT;
bool INT_TIMER;
bool INT_JP;
bool INT_SERIAL;

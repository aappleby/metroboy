#pragma once

#include "ChipSignals.h"
#include "CpuSignals.h"

#include "P01_ClocksReset.h"
#include "P02_Interrupts.h"
#include "P03_Timer.h"
#include "P04_DMA.h"
#include "P05_JoypadIO.h"
#include "P06_SerialLink.h"
#include "P07_SysDecode.h"
#include "P08_ExtCpuBuses.h"
#include "P09_ApuControl.h"
#include "P10_ApuDecode.h"
#include "P11_Ch1Regs.h"
#include "P12_Ch1Sweep.h"
#include "P13_Channel1.h"
#include "P14_Ch2Regs.h"
#include "P15_Channel2.h"
#include "P16_Ch3Regs.h"
#include "P17_WaveRam.h"
#include "P18_Channel3.h"
#include "P19_Ch4Regs.h"
#include "P20_Channel4.h"
#include "P21_VideoControl.h"
#include "P22_PpuDecode.h"
#include "P23_VideoRegs.h"
#include "P28_OAM.h"

struct Gameboy {
  int64_t timestamp;

  ChipSignals  chip;
  CpuSignals   cpu;

  P01_ClocksReset  p01;
  P02_Interrupts   p02;
  P03_Timer        p03;
  P04_DMA          p04;
  P05_JoypadIO     p05;
  P06_SerialLink   p06;
  P07_SysDecode    p07;
  P08_ExtCpuBuses  p08;
  P09_ApuControl   p09;
  P10_ApuDecode    p10;
  P11_Ch1Regs      p11;
  P12_Ch1Sweep     p12;
  P13_Channel1     p13;
  P14_Ch2Regs      p14;
  P15_Channel2     p15;
  P16_Ch3Regs      p16;
  P17_WaveRam      p17;
  P18_Channel3     p18;
  P19_Ch4Regs      p19;
  P20_Channel4     p20;
  P21_VideoControl p21;
  P22_PpuDecode    p22;
  P23_VideoRegs    p23;
  P28_OAM          p28;

  // this signal is weird
  bool ABOL;

  //----------
  // main bus

  bool A00,A01,A02,A03,A04,A05,A06,A07,A08,A09,A10,A11,A12,A13,A14,A15;
  bool D0,D1,D2,D3,D4,D5,D6,D7;
};
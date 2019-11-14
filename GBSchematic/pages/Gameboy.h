#pragma once

#include "../Schematics.h"
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
#include "P24_LcdControl.h"
#include "P25_VramInterface.h"
#include "P26_Background.h"
#include "P27_WindowMapLookup.h"
#include "P28_OAM.h"
#include "P29_SpriteControl.h"
#include "P30_SpriteStore.h"
#include "P31_SpriteXMatchers.h"
#include "P32_BgPixelShifter.h"
#include "P33_SpritePixelShifter.h"
#include "P34_SpritePaletteShifter.h"
#include "P35_PixelMux.h"
#include "P36_Palettes.h"

struct Gameboy {
  int64_t timestamp;

  ChipSignals  chip;
  CpuSignals   cpu;

  P01_ClocksReset          p01;
  P02_Interrupts           p02;
  P03_Timer                p03;
  P04_DMA                  p04;
  P05_JoypadIO             p05;
  P06_SerialLink           p06;
  P07_SysDecode            p07;
  P08_ExtCpuBuses          p08;
  P09_ApuControl           p09;
  P10_ApuDecode            p10;
  P11_Ch1Regs              p11;
  P12_Ch1Sweep             p12;
  P13_Channel1             p13;
  P14_Ch2Regs              p14;
  P15_Channel2             p15;
  P16_Ch3Regs              p16;
  P17_WaveRam              p17;
  P18_Channel3             p18;
  P19_Ch4Regs              p19;
  P20_Channel4             p20;
  P21_VideoControl         p21;
  P22_PpuDecode            p22;
  P23_VideoRegs            p23;
  P24_LcdControl           p24;
  P25_VramInterface        p25;
  P26_Background           p26;
  P27_WindowMapLookup      p27;
  P28_OAM                  p28;
  P29_SpriteControl        p29;
  P30_SpriteStore          p30;
  P31_SpriteXMatchers      p31;
  P32_BgPixelShifter       p32;
  P33_SpritePixelShifter   p33;
  P34_SpritePaletteShifter p34;
  P35_PixelMux             p35;
  P36_Palettes             p36;

  //----------
  // main bus

  union {
    uint16_t A;
    struct {
      bool A00 : 1;
      bool A01 : 1;
      bool A02 : 1;
      bool A03 : 1;
      bool A04 : 1;
      bool A05 : 1;
      bool A06 : 1;
      bool A07 : 1;
      bool A08 : 1;
      bool A09 : 1;
      bool A10 : 1;
      bool A11 : 1;
      bool A12 : 1;
      bool A13 : 1;
      bool A14 : 1;
      bool A15 : 1;
    };
  };

  union { 
    uint8_t D;
    struct {
      bool D0 : 1;
      bool D1 : 1;
      bool D2 : 1;
      bool D3 : 1;
      bool D4 : 1;
      bool D5 : 1;
      bool D6 : 1;
      bool D7 : 1;
    };
  };

  //----------
  // vram bus

  bool MA00,MA01,MA02,MA03,MA04,MA05,MA06,MA07,MA08,MA09,MA10,MA11,MA12;
  bool MD0,MD1,MD2,MD3,MD4,MD5,MD6,MD7;

  //----------
  // oam bus

  bool OAM_A_D0,OAM_A_D1,OAM_A_D2,OAM_A_D3,OAM_A_D4,OAM_A_D5,OAM_A_D6,OAM_A_D7;
  bool OAM_B_D0,OAM_B_D1,OAM_B_D2,OAM_B_D3,OAM_B_D4,OAM_B_D5,OAM_B_D6,OAM_B_D7;

  //----------
  // wave ram bus

  bool WAVE_RD_D0,WAVE_RD_D1,WAVE_RD_D2,WAVE_RD_D3,WAVE_RD_D4,WAVE_RD_D5,WAVE_RD_D6,WAVE_RD_D7;
};
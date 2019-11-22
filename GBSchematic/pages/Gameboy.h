#pragma once

#include "../Schematics.h"

#include "ChipSignals.h"
#include "CpuSignals.h"

#include "System.h"

#include "ApuControl.h"
#include "Channel1.h"
#include "Channel2.h"
#include "Channel3.h"
#include "Channel4.h"

#include "Sprites.h"
#include "PixelPipe.h"

#include "P21_VideoControl.h"
#include "P22_PpuDecode.h"
#include "P23_VideoRegs.h"
#include "P24_LcdControl.h"
#include "P25_VramInterface.h"
#include "P26_Background.h"
#include "P27_WindowMapLookup.h"


//-----------------------------------------------------------------------------

struct Gameboy {
  int64_t timestamp;

  ChipSignals  chip;
  CpuSignals   cpu;

  System     sys;
  ApuControl apu;
  Channel1   ch1;
  Channel2   ch2;
  Channel3   ch3;
  Channel4   ch4;

  Sprites    spr;

  P21_VideoControl         p21;
  P22_PpuDecode            p22;
  P23_VideoRegs            p23;
  P24_LcdControl           p24;
  P25_VramInterface        p25;
  P26_Background           p26;
  P27_WindowMapLookup      p27;
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

  /*p18.BARY*/ bool WAVE_DAC0;
  /*p18.BYKA*/ bool WAVE_DAC1;
  /*p18.BOPA*/ bool WAVE_DAC2;
  /*p18.BELY*/ bool WAVE_DAC3;

  //----------
  // vram bus

  bool MA00,MA01,MA02,MA03,MA04,MA05,MA06,MA07,MA08,MA09,MA10,MA11,MA12;
  bool MD0,MD1,MD2,MD3,MD4,MD5,MD6,MD7;

  //----------
  // oam bus

  bool OAM_CLK;

  bool TS_OAM_A_D0,TS_OAM_A_D1,TS_OAM_A_D2,TS_OAM_A_D3,TS_OAM_A_D4,TS_OAM_A_D5,TS_OAM_A_D6,TS_OAM_A_D7;
  bool TS_OAM_B_D0,TS_OAM_B_D1,TS_OAM_B_D2,TS_OAM_B_D3,TS_OAM_B_D4,TS_OAM_B_D5,TS_OAM_B_D6,TS_OAM_B_D7;

  //----------
  // wave ram bus

  bool WAVE_RAM_CTRL1;
  bool WAVE_RAM_CTRL3;
  bool WAVE_WRn;

  bool WAVE_D0;
  bool WAVE_D1;
  bool WAVE_D2;
  bool WAVE_D3;
  bool WAVE_D4;
  bool WAVE_D5;
  bool WAVE_D6;
  bool WAVE_D7;

  /*p18.BOLE*/ bool WAVE_A0;
  /*p18.AGYL*/ bool WAVE_A1;
  /*p18.AFUM*/ bool WAVE_A2;
  /*p18.AXOL*/ bool WAVE_A3;
};

//-----------------------------------------------------------------------------

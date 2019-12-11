#pragma once
#include "Schematics.h"


#include "Sch_CpuSignals.h"

#include "Sch_System.h"

#include "Sch_ApuControl.h"
#include "Sch_Channel1.h"
#include "Sch_Channel2.h"
#include "Sch_Channel3.h"
#include "Sch_Channel4.h"
#include "Sch_Sprites.h"
#include "Sch_PixelPipe.h"
#include "Sch_Video.h"

#include "Sch_Serial.h"
#include "Sch_Timer.h"
#include "Sch_DMA.h"
#include "Sch_Debug.h"
#include "Sch_Interrupts.h"
#include "Sch_Joypad.h"
#include "Sch_Pins.h"
#include "Sch_LCD.h"
#include "Sch_Bootrom.h"
#include "Sch_Hram.h"
#include "Sch_OAM.h"

namespace Schematics {

//-----------------------------------------------------------------------------

struct Gameboy {
  int64_t timestamp;

  CpuOut cpu_out;

  System     sys;
  ApuControl apu;
  Channel1   ch1;
  Channel2   ch2;
  Channel3   ch3;
  Channel4   ch4;

  Sprites    spr;
  PixelPipe  pix;
  Video      vid;
  Serial     ser;
  Timer      tim;
  DMA        dma;
  Debug      dbg;
  Interrupts irq;
  Joypad     joy;
  Pins       pin;
  LCD        lcd;
  Bootrom    rom;
  Hram       hrm;
  OAM        oam;

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
};

//-----------------------------------------------------------------------------

};
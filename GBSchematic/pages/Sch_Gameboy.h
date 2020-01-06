#pragma once
#include "Sch_Common.h"


#include "Sch_CpuSignals.h"

#include "Sch_ApuControl.h"
#include "Sch_Channel1.h"
#include "Sch_Channel2.h"
#include "Sch_Channel3.h"
#include "Sch_Channel4.h"
#include "Sch_Sprites.h"
#include "Sch_PixelPipe.h"
#include "Sch_Video.h"

#include "Sch_BusControl.h"
#include "Sch_Resets.h"
#include "Sch_Serial.h"
#include "Sch_Timer.h"
#include "Sch_DMA.h"
#include "Sch_Debug.h"
#include "Sch_Interrupts.h"
#include "Sch_Joypad.h"
#include "Sch_Pins.h"
#include "Sch_LCD.h"
#include "Sch_Bootrom.h"
#include "Sch_OAM.h"
#include "Sch_Vram.h"
#include "Sch_Registers.h"
#include "Sch_Clocks.h"
#include "Sch_Decoder.h"
#include "Sch_Joypad.h"

namespace Schematics {

//-----------------------------------------------------------------------------

struct Gameboy {
  int64_t timestamp;

  Cpu        cpu;
  ClkRegisters     sys_clk;
  VclkRegisters     vid_clk;
  RstRegisters rst_reg;
  RstSignals   rst_sig;
  BusTristates        bus;
  BusSignals ctl;
  //Decoder    dec;
  //Debug      dbg;

  //Sprites    spr;
  //PixelPipe  pix;
  //Video      vid;
  //Serial     ser;
  Timer      tim;
  //DMA        dma;
  //Interrupts irq;
  //Joypad     joy;
  //Pins       pin;

  //LcdRegisters  lcd;
  Bootrom    rom;
  //OAM        oam;
  //Vram       vrm;
  //LcdRegisterseg;

  ApuControl apu;
  Channel1   ch1;
  Channel2   ch2;
  Channel3   ch3;
  Channel4   ch4;
};

//-----------------------------------------------------------------------------

};
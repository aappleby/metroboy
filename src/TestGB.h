#pragma once

#include "Sch_Timer.h"
#include "Sch_LCD.h"
#include "Sch_DMA.h"
#include "Sch_SpriteStore.h"
#include "Sch_PPU.h"
#include "Sch_Window.h"
#include "Sch_Clocks.h"
#include "Sch_Resets.h"
#include "Sch_Debug.h"
#include "Sch_PixPipe.h"
#include "Sch_Joypad.h"
#include "Sch_Serial.h"

#include "Sch_VramPins.h"
#include "Sch_VramBus.h"
#include "Sch_ExtBus.h"
#include "Sch_CpuBus.h"
#include "Sch_OamBus.h"
#include "Sch_BusMux.h"
#include "Sch_Interrupts.h"
#include "Sch_Scroll.h"
#include "Sch_Bootrom.h"
#include "Sch_SpriteFetcher.h"
#include "Sch_TileFetcher.h"

namespace Schematics {

//-----------------------------------------------------------------------------

struct TestGB {

  void tick_everything();
  bool commit_everything();

  //-----------------------------------------------------------------------------

  int phase_counter = -32;

  /*
  uint8_t rom[65536];
  uint8_t ram[8192];
  uint8_t vram[8192];
  uint8_t hiram[128];
  */

  ClockRegisters clk_reg;
  DebugRegisters dbg_reg;
  DmaRegisters dma_reg;
  InterruptRegisters int_reg;
  JoypadRegisters joy_reg;

  LcdRegisters lcd_reg;
  PixelPipeRegisters pxp_reg;
  ResetRegisters rst_reg;
  SerialRegisters ser_reg;
  SpriteStoreRegisters sst_reg;
  TimerRegisters tim_reg;
  PpuRegisters ppu_reg;
  WindowRegisters win_reg;

  ScrollRegisters scr_reg;
  VramPins vram_pins;

  CpuBus  cpu_bus;
  CpuPinsOut cpu_pins_out;

  ExtPinsIn  ext_pins_in;
  ExtPinsOut ext_pins_out;

  OamBus oam_bus;
  VramBus vram_bus;

  TileFetcher tile_fetcher;
  SpriteFetcher sprite_fetcher;

  BusMux bus_mux;

  Bootrom bootrom;
};

//-----------------------------------------------------------------------------

};
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
#include "Sch_BusMux.h"
#include "Sch_Interrupts.h"
#include "Sch_Bootrom.h"
#include "Sch_SpriteFetcher.h"
#include "Sch_TileFetcher.h"
#include "Sch_SpriteScanner.h"
#include "Sch_PpuConfig.h"

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

  VramPins vram_pins;

  CpuBus  cpu_bus;
  CpuPinsOut cpu_pins_out;

  VramBus vram_bus;

  TileFetcher tile_fetcher;
  SpriteFetcher sprite_fetcher;
  SpriteScanner sprite_scanner;
  PpuConfig ppu_config;

  BusMux bus_mux;

  Bootrom bootrom;

  /* PIN_71 */ PinIn EXT_PIN_RST;
  /* PIN_74 */ PinIn EXT_PIN_CLK_GOOD;

  /* PIN_17 */ PinIn EXT_PIN_D0_C;    // -> P08.TOVO,SOMA
  /* PIN_18 */ PinIn EXT_PIN_D1_C;    // -> P08.RUZY,RONY
  /* PIN_19 */ PinIn EXT_PIN_D2_C;    // -> P08.ROME,RAXY
  /* PIN_20 */ PinIn EXT_PIN_D3_C;    // -> P08.SAZA,SELO
  /* PIN_21 */ PinIn EXT_PIN_D4_C;    // -> P08.TEHE,SODY
  /* PIN_22 */ PinIn EXT_PIN_D5_C;    // -> P08.RATU,SAGO
  /* PIN_23 */ PinIn EXT_PIN_D6_C;    // -> P08.SOCA,RUPA
  /* PIN_24 */ PinIn EXT_PIN_D7_C;    // -> P08.RYBA,SAZY
};

//-----------------------------------------------------------------------------

};
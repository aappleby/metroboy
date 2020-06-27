#include "Sch_Merged.h"
#include "Sch_Pins.h"

#include "Sch_Timer.h"
#include "Sch_LCD.h"
#include "Sch_CPU.h"
#include "Sch_DMA.h"
#include "Sch_SpriteStore.h"
#include "Sch_OAM.h"
#include "Sch_PPU.h"
#include "Sch_Window.h"
#include "Sch_Clocks.h"
#include "Sch_Resets.h"
#include "Sch_Debug.h"
#include "Sch_Address.h"
#include "Sch_PixPipe.h"
#include "Sch_Joypad.h"
#include "Sch_Serial.h"
#include "Sch_Cart.h"
#include "Sch_VRAM.h"
#include "Sch_VramBus.h"
#include "Sch_ExtBus.h"
#include "Sch_CpuBus.h"
#include "Sch_OamBus.h"
#include "Sch_BusMux.h"

namespace Schematics {

//-----------------------------------------------------------------------------

struct TestGB {

  void ext_preset();

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

  CpuRegisters cpu_reg;
  ClockRegisters clk_reg;
  BusRegisters bus_reg;
  ConfigRegisters cfg_reg;
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
  OamRegisters oam_reg;
  WindowRegisters win_reg;
  AddressRegisters adr_reg;
  CartRegisters cart_reg;
  
  VramPins vram_pins;

  CpuBus cpu_bus;
  ExtBus ext_bus;
  OamBus oam_bus;
  VramBus vram_bus;

  BusMux bus_mux;

  SysPins sys_pins;
  JoypadPins joy_pin;
  WavePins wave_pins;
};

//-----------------------------------------------------------------------------

};
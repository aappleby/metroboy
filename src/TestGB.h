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
  ClockRegisters clk_reg; // dumped
  BusRegisters bus_reg;// dumped
  ConfigRegisters cfg_reg;// dumped
  DebugRegisters dbg_reg;// dumped
  DmaRegisters dma_reg;// dumped
  InterruptRegisters int_reg;// dumped
  JoypadRegisters joy_reg;// dumped

  LcdRegisters lcd_reg;// dumped
  PixelPipeRegisters pxp_reg;// dumped
  ResetRegisters rst_reg;// dumped
  SerialRegisters ser_reg;// dumped
  SpriteStoreRegisters sst_reg;// dumped
  TimerRegisters tim_reg;// dumped
  PpuRegisters ppu_reg;// dumped
  OamRegisters oam_reg;// dumped
  WindowRegisters win_reg;
  AddressRegisters adr_reg;
  CartRegisters cart_reg;
  VramRegisters vram_reg;

  SysPins sys_pins; // dumped
  VramPins vram_pins; // dumped
  JoypadPins joy_pin; // dumped
  CpuPins cpu_pins; // dumped
  ExtPins ext_pins; // dumped
  OamPins oam_pins; // dumped
  WavePins wave_pins;
};

//-----------------------------------------------------------------------------

};
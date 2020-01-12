#include "../pages/Sch_Merged.h"
#include "../pages/Sch_Pins.h"

namespace Schematics {

//-----------------------------------------------------------------------------

struct TestGB {

  void tick_everything();
  bool commit_everything();

  Cart   cart;
  Ram8K  main_ram;
  Ram8K  video_ram;

  BusRegisters bus_reg;
  ClkRegisters clk_reg;
  ConfigRegisters cfg_reg;
  DebugRegisters dbg_reg;
  DmaRegisters dma_reg;
  InterruptRegisters int_reg;
  JoypadRegisters joy_reg;
  LcdRegisters lcd_reg;
  OamRegisters oam_reg;
  PixelPipeRegisters pxp_reg;
  RstRegisters rst_reg;
  SerialRegisters ser_reg;
  SpriteRegisters spr_reg;
  SpriteStoreRegisters sst_reg;
  TimerRegisters tim_reg;
  VclkRegisters vclk_reg;
  VidRegisters vid_reg;

  ExtPins ext_pins;
  VramPins vram_pins;
  SerialPins ser_pins;
  JoypadPins joy_pins;
  CpuPins cpu_pins;
  CartPins cart_pins;
  OamPins oam_pins;
  LcdPins lcd_pins;
  WavePins wave_pins;

  VramBus vrm_tri;
  BusTristates bus_tri;
  SpriteTristate sil_tri;

  uint8_t rom[65536];
};

//-----------------------------------------------------------------------------

};
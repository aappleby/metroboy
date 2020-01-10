#include "../pages/Sch_Merged.h"
#include "../pages/Sch_Pins.h"

namespace Schematics {

//-----------------------------------------------------------------------------

struct TestGB {

  VramPins vram_pins;
  SerialPins serial_pins;
  JoypadPins joypad_pins;
  CartPins cart_pins;
  OamPins oam_pins;

  VramBus vrm_tri;

  Cart    cart;
  Ram8K   vram;
  Ram8K   ram;
  Ram8K   hram;
  Bootrom bootrom;

  SysRegisters    sys_reg;
  ClkRegisters    clk_reg;
  RstRegisters    rst_reg;
  DebugRegisters  dbg_reg;
  VclkRegisters   vclk_reg;
  VidRegisters    vid_reg;
  ConfigRegisters       vid_cfg;
  LcdRegisters    lcd_reg;
  BusTristates    bus_tri;
  TimerRegisters  tim_reg;
  SerialRegisters ser_reg;

  SpriteStoreRegisters sst_reg;
  SpriteRegisters spr_reg;
  PixelPipeRegisters pxp_reg;
  OamRegisters oam_reg;
  JoypadRegisters joy_reg;
  DmaRegisters dma_reg;
  InterruptRegisters int_reg;

  uint8_t rom[65536];
};

//-----------------------------------------------------------------------------

};
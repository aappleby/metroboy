#include "../pages/Sch_Merged.h"
#include "../pages/Sch_Pins.h"

namespace Schematics {

//-----------------------------------------------------------------------------

struct TestGB {

  void pass_init(bool RST, bool CLKIN_A, bool CLKIN_B);
  void cpu_init(bool CLKREQ);
  void pin_init();
  void tick_ext();

  void tick_everything();
  bool commit_everything();

  //-----------------------------------------------------------------------------

  uint8_t rom[65536];
  uint8_t ram[8192];
  uint8_t vram[8192];
  uint8_t hiram[128];

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

  SysPins sys_pins;
  VramPins vram_pins;
  SerialPins ser_pins;
  JoypadPins joy_pins;
  CpuPins cpu_pins;
  ExtPins ext_pins;
  OamPins oam_pins;
  LcdPins lcd_pins;
  WavePins wave_pins;
};

//-----------------------------------------------------------------------------

};
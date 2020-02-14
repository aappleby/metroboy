#include "../pages/Sch_Merged.h"
#include "../pages/Sch_Pins.h"

namespace Schematics {

//-----------------------------------------------------------------------------

struct TestGB {

  void sys_preset(bool RST, bool CLKIN_A, bool CLKIN_B);
  void cpu_preset(bool CLKREQ, uint16_t addr, uint8_t data);
  void ext_preset();
  void ser_preset();
  void joy_preset();
  void vram_preset();

  void tick_everything();
  bool commit_everything();

  //-----------------------------------------------------------------------------

  /*
  uint8_t rom[65536];
  uint8_t ram[8192];
  uint8_t vram[8192];
  uint8_t hiram[128];
  */

  ClkRegisters clk_reg; // dumped
  BusRegisters bus_reg;// dumped
  ConfigRegisters cfg_reg;// dumped
  DebugRegisters dbg_reg;// dumped
  DmaRegisters dma_reg;// dumped
  InterruptRegisters int_reg;// dumped
  JoypadRegisters joy_reg;// dumped

  LcdRegisters lcd_reg;// dumped
  PixelPipeRegisters pxp_reg;// dumped
  RstRegisters rst_reg;// dumped
  SerialRegisters ser_reg;// dumped
  SpriteRegisters spr_reg;// dumped
  SpriteStoreRegisters sst_reg;// dumped
  TimerRegisters tim_reg;// dumped
  VclkRegisters vclk_reg;// dumped
  VidRegisters vid_reg;// dumped
  OamRegisters oam_reg;// dumped

  SysPins sys_pins; // dumped
  VramPins vram_pins; // dumped
  SerialPins ser_pins; // dumped
  JoypadPins joy_pins; // dumped
  CpuPins cpu_pins; // dumped
  ExtPins ext_pins; // dumped
  OamPins oam_pins; // dumped
  LcdPins lcd_pins; // dumped
  WavePins wave_pins;
};

//-----------------------------------------------------------------------------

};
#include "Sch_Merged.h"
#include "Sch_Pins.h"

namespace Schematics {

//-----------------------------------------------------------------------------

struct TestGB {

  void ext_preset();

  void tick_everything();
  bool commit_everything();

  void tick_dma(int phase, bool CUNU_RSTn, bool ABOL_CLKREQn, bool UNOR_MODE_DBG2);

  void tick_busmux(int phase);

  void tick_lcd(int phase, bool ALUR_RSTn, bool LCDC_EN);

  wire TEDO_BUS_RD() const;

  void tick_pipe      (bool ALUR_RSTn, bool UMUT_MODE_DBG1, bool UNOR_MODE_DBG2,
                       bool CLKPIPE_AxCxExGx, bool NYXU_BG_SEQ_RSTn, bool WUTY_SPRITE_DONE);
  void tick_vbus      (bool ALUR_RSTn, bool UMUT_MODE_DBG1, bool UNOR_MODE_DBG2);
  void tick_ebus      (bool ALUR_RSTn, bool UMUT_MODE_DBG1, bool UNOR_MODE_DBG2);
  void tick_oam       (bool ALUR_RSTn, bool UMUT_MODE_DBG1, bool UNOR_MODE_DBG2, bool ACYL_OAM_ADDR_PARSE);
  void tick_bootrom   (bool ALUR_RSTn, bool UMUT_MODE_DBG1, bool UNOR_MODE_DBG2);
  void tick_interrupts(bool ALUR_RSTn, bool UMUT_MODE_DBG1, bool UNOR_MODE_DBG2, bool WODU_RENDER_DONE);

  void tick_vram_lock(bool ALUR_RSTn, bool UMUT_MODE_DBG1, bool UNOR_MODE_DBG2);
  void tick_oam_lock(bool ALUR_RSTn, bool UMUT_MODE_DBG1, bool UNOR_MODE_DBG2);

  //-----------------------------------------------------------------------------


  int phase_counter = -32;

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
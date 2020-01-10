#include "../pages/Sch_Clocks.h"
#include "../pages/Sch_Resets.h"
#include "../pages/Sch_LCD.h"
#include "../pages/Sch_System.h"
#include "../pages/Sch_BusControl.h"
#include "../pages/Sch_Registers.h"
#include "../pages/Sch_Serial.h"
#include "../pages/Sch_Timer.h"
#include "../pages/Sch_Video.h"
#include "../pages/Sch_Debug.h"
#include "../pages/Sch_Pins.h"
#include "../pages/Sch_PixelPipe.h"
#include "../pages/Sch_Ram.h"
#include "../pages/Sch_OAM.h"
#include "../pages/Sch_Joypad.h"
#include "../pages/Sch_DMA.h"
#include "../pages/Sch_Cart.h"
#include "../pages/Sch_SpriteStore.h"
#include "../pages/Sch_Sprites.h"
#include "../pages/Sch_Bootrom.h"
#include "../pages/Sch_Interrupts.h"
#include "../pages/Sch_Vram.h"

namespace Schematics {

//-----------------------------------------------------------------------------

struct TestGB {

  TestGB();
  void pwron();
  void reset();
  void phase_begin();
  void phase_end();
  void pass_begin();
  bool pass_end();

  int sim_phase(int phase_count);
  int sim_pass();

  void boot();

  VramPins vram_pins;
  SerialPins serial_pins;
  LcdPins lcd_pins;
  JoypadPins joypad_pins;
  AudioPins audio_pins;
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
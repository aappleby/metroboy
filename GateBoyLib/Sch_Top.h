#pragma once

#include "GateBoyLib/Sch_Timer.h"
#include "GateBoyLib/Sch_LCD.h"
#include "GateBoyLib/Sch_DMA.h"
#include "GateBoyLib/Sch_SpriteStore.h"
#include "GateBoyLib/Sch_Clocks.h"
#include "GateBoyLib/Sch_PixPipe.h"
#include "GateBoyLib/Sch_Joypad.h"
#include "GateBoyLib/Sch_Serial.h"
#include "GateBoyLib/Sch_Interrupts.h"
#include "GateBoyLib/Sch_Bootrom.h"
#include "GateBoyLib/Sch_SpriteFetcher.h"
#include "GateBoyLib/Sch_TileFetcher.h"
#include "GateBoyLib/Sch_SpriteScanner.h"

#include "GateBoyLib/Sch_CpuBus.h"
#include "GateBoyLib/Sch_ExtBus.h"
#include "GateBoyLib/Sch_OamBus.h"
#include "GateBoyLib/Sch_VramBus.h"

namespace Schematics {

//-----------------------------------------------------------------------------

#pragma pack(push, 1)
struct SchematicTop {
  void reset() {}

  void reset_bootrom() {
    oam_bus.reset_bootrom();
    ext_bus.reset_bootrom();
    cpu_bus.reset_bootrom();
    vram_bus.reset_bootrom();

    clk_reg.reset_bootrom();
    dma_reg.reset_bootrom();
    int_reg.reset_bootrom();
    joypad.reset_bootrom();
    lcd_reg.reset_bootrom();
    pix_pipe.reset_bootrom();
    ser_reg.reset_bootrom();
    sprite_store.reset_bootrom();
    tim_reg.reset_bootrom();
    tile_fetcher.reset_bootrom();
    sprite_fetcher.reset_bootrom();
    sprite_scanner.reset_bootrom();
    bootrom.reset_bootrom();

    SOTO_DBG_VRAM.reset(REG_D0C0);

    PIN_LCD_DATA1.reset(TRI_HZNP);
    PIN_LCD_DATA0.reset(TRI_HZNP);
    PIN_LCD_CNTRL.reset(TRI_HZNP);
    PIN_LCD_CLOCK.reset(TRI_HZNP);
    PIN_LCD_HSYNC.reset(TRI_HZNP);
    PIN_LCD_LATCH.reset(TRI_HZNP);
    PIN_LCD_ALTSG.reset(TRI_HZNP);
    PIN_LCD_VSYNC.reset(TRI_HZNP);

    IE_D0.reset(REG_D0C0);
    IE_D1.reset(REG_D0C0);
    IE_D2.reset(REG_D0C0);
    IE_D3.reset(REG_D0C0);
    IE_D4.reset(REG_D0C0);
  }

  void tick_slow(wire RST, wire CLK, wire CLKGOOD, wire T1n, wire T2n, wire CPUREADY);
  void tock_slow(wire RST, wire CLK, wire CLKGOOD, wire T1n, wire T2n, wire CPUREADY);

  void tock_ext_bus (wire RST, uint8_t* cart_rom, uint8_t* cart_ram, uint8_t* ext_ram);
  void tock_vram_bus(wire RST, uint8_t* vid_ram);
  void tock_oam_bus (wire RST, uint8_t* oam_ram);
  void tock_zram_bus(wire RST, uint8_t* zero_ram);

  //-----------------------------------------------------------------------------

  /*p07.UMUT*/ Sig UMUT_MODE_DBG1p;
  /*p07.UNOR*/ Sig UNOR_MODE_DBG2p;
  /*p07.UPOJ*/ Sig UPOJ_MODE_PRODn;

  /*p25.TUTO*/ Sig TUTO_DBG_VRAMp;

  /*p01.ABUZ*/ Sig ABUZ_AVn;

  /*p07.TEDO*/ Sig TEDO_CPU_RDp;
  /*p28.LEKO*/ Sig LEKO_CPU_RDp;
  /*p07.TAPU*/ Sig TAPU_CPU_WRp_xxxxEFGx;
  /*p01.APOV*/ Sig APOV_CPU_WRp_xxxxEFGx;

  /*p27.NYXU*/ Sig NYXU_FETCH_TRIGn;

  /*p27.TEVO*/ Sig TEVO_FETCH_TRIGp;

  /*p27.TAVE*/ Sig TAVE_PRELOAD_DONE_TRIGp;

  /*p28.ACYL*/ Sig ACYL_SCANNINGp;

  /*#p29.AVAP*/ Sig AVAP_RENDER_START_TRIGp;

  /*p07.TUTU*/ Sig TUTU_ADDR_BOOTp;


  /*p04.DECY*/ Sig DECY_LATCH_EXTn;

  /*p28.AJON*/ Sig AJON_OAM_BUSY;
  /*p28.AJUJ*/ Sig AJUJ_OAM_BUSYn;

  //-----------------------------------------------------------------------------

  OamBus oam_bus;
  ExtBus ext_bus;
  CpuBus cpu_bus;
  VramBus vram_bus;

  ClockRegisters  clk_reg;

  DmaRegisters dma_reg;
  InterruptRegisters int_reg;
  Joypad joypad;
  LcdRegisters lcd_reg;
  PixelPipe pix_pipe;
  SerialRegisters ser_reg;
  SpriteStore sprite_store;
  Timer tim_reg;
  TileFetcher tile_fetcher;
  SpriteFetcher sprite_fetcher;
  SpriteScanner sprite_scanner;
  Bootrom bootrom;

  /*p25.SOTO*/ DFF17 SOTO_DBG_VRAM;

  Pin PIN_LCD_DATA1 = TRI_HZNP; // PIN_50
  Pin PIN_LCD_DATA0 = TRI_HZNP; // PIN_51
  Pin PIN_LCD_CNTRL = TRI_HZNP; // PIN_52
  Pin PIN_LCD_CLOCK = TRI_HZNP; // PIN_53
  Pin PIN_LCD_HSYNC = TRI_HZNP; // PIN_54
  Pin PIN_LCD_LATCH = TRI_HZNP; // PIN_55
  Pin PIN_LCD_ALTSG = TRI_HZNP; // PIN_56
  Pin PIN_LCD_VSYNC = TRI_HZNP; // PIN_57

  DFF IE_D0;
  DFF IE_D1;
  DFF IE_D2;
  DFF IE_D3;
  DFF IE_D4;
};
#pragma pack(pop)

//-----------------------------------------------------------------------------

};
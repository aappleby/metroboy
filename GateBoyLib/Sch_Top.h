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
  void reset_cart() {
    UMUT_MODE_DBG1p.reset(TRI_D0NP);
    UNOR_MODE_DBG2p.reset(TRI_D0NP);
    UPOJ_MODE_PRODn.reset(TRI_D1NP);
    TUTO_DBG_VRAMp.reset(TRI_D0NP);
    ABUZ_AVn.reset(TRI_D1NP);
    TEDO_CPU_RDp.reset(TRI_D0NP);
    LEKO_CPU_RDp.reset(TRI_D0NP);
    TAPU_CPU_WRp_xxxxEFGx.reset(TRI_D0NP);
    APOV_CPU_WRp_xxxxEFGx.reset(TRI_D0NP);
    NYXU_FETCH_TRIGn.reset(TRI_D1NP);
    TEVO_FETCH_TRIGp.reset(TRI_D0NP);
    TAVE_PRELOAD_DONE_TRIGp.reset(TRI_D0NP);
    AVAP_RENDER_START_TRIGp.reset(TRI_D0NP);
    TUTU_ADDR_BOOTp.reset(TRI_D0NP);

    oam_bus.reset_cart();
    ext_bus.reset_cart();
    cpu_bus.reset_cart();
    vram_bus.reset_cart();

    clk_reg.reset_cart();
    dma_reg.reset_cart();
    int_reg.reset_cart();
    joypad.reset_cart();
    lcd_reg.reset_cart();
    pix_pipe.reset_cart();
    ser_reg.reset_cart();
    sprite_store.reset_cart();
    tim_reg.reset_cart();
    tile_fetcher.reset_cart();
    sprite_fetcher.reset_cart();
    sprite_scanner.reset_cart();
    bootrom.reset_cart();

    SOTO_DBG_VRAM.reset(REG_D0C1);

    lcd_data0_delay.da.value = TRI_D1NP;
    lcd_data0_delay.db.value = TRI_D1NP;
    lcd_data0_delay.dc.value = TRI_D1NP;
    lcd_data0_delay.dd.value = TRI_D1NP;
    lcd_data0_delay.de.value = TRI_D1NP;
    lcd_data0_delay.df.value = TRI_D1NP;
    lcd_data0_delay.dg.value = TRI_D1NP;
    lcd_data0_delay.dh.value = TRI_D1NP;
    lcd_data0_delay.di.value = TRI_D1NP;

    lcd_data1_delay.da.value = TRI_D1NP;
    lcd_data1_delay.db.value = TRI_D1NP;
    lcd_data1_delay.dc.value = TRI_D1NP;
    lcd_data1_delay.dd.value = TRI_D1NP;
    lcd_data1_delay.de.value = TRI_D1NP;
    lcd_data1_delay.df.value = TRI_D1NP;
    lcd_data1_delay.dg.value = TRI_D1NP;
    lcd_data1_delay.dh.value = TRI_D1NP;
    lcd_data1_delay.di.value = TRI_D1NP;

    PIN_LCD_DATA1.reset(TRI_D0NP);
    PIN_LCD_DATA0.reset(TRI_D0NP);
    PIN_LCD_CNTRL.reset(TRI_D0NP);
    PIN_LCD_CLOCK.reset(TRI_D0NP);
    PIN_LCD_HSYNC.reset(TRI_D0NP);
    PIN_LCD_LATCH.reset(TRI_D0NP);
    PIN_LCD_FLIPS.reset(TRI_D0NP);
    PIN_LCD_VSYNC.reset(TRI_D0NP);

    IE_D0.reset(REG_D0C1);
    IE_D1.reset(REG_D0C1);
    IE_D2.reset(REG_D0C1);
    IE_D3.reset(REG_D0C1);
    IE_D4.reset(REG_D0C1);

    lcd_pix_lo.reset(TRI_D0NP);
    lcd_pix_hi.reset(TRI_D0NP);

    for (int i = 0; i < 160; i++) {
      lcd_pipe_lo[i].reset(REG_D0C0);
      lcd_pipe_hi[i].reset(REG_D0C0);
      lcd_line_lo[i].reset(REG_D0C0);
      lcd_line_hi[i].reset(REG_D0C0);
    }
  }

  void reset_boot() {
    UMUT_MODE_DBG1p.reset(TRI_HZNP);
    UNOR_MODE_DBG2p.reset(TRI_HZNP);
    UPOJ_MODE_PRODn.reset(TRI_HZNP);
    TUTO_DBG_VRAMp.reset(TRI_HZNP);
    ABUZ_AVn.reset(TRI_HZNP);
    TEDO_CPU_RDp.reset(TRI_HZNP);
    LEKO_CPU_RDp.reset(TRI_HZNP);
    TAPU_CPU_WRp_xxxxEFGx.reset(TRI_HZNP);
    APOV_CPU_WRp_xxxxEFGx.reset(TRI_HZNP);
    NYXU_FETCH_TRIGn.reset(TRI_HZNP);
    TEVO_FETCH_TRIGp.reset(TRI_HZNP);
    TAVE_PRELOAD_DONE_TRIGp.reset(TRI_HZNP);
    AVAP_RENDER_START_TRIGp.reset(TRI_HZNP);
    TUTU_ADDR_BOOTp.reset(TRI_HZNP);

    oam_bus.reset_boot();
    ext_bus.reset_boot();
    cpu_bus.reset_boot();
    vram_bus.reset_boot();

    clk_reg.reset_boot();
    dma_reg.reset_boot();
    int_reg.reset_boot();
    joypad.reset_boot();
    lcd_reg.reset_boot();
    pix_pipe.reset_boot();
    ser_reg.reset_boot();
    sprite_store.reset_boot();
    tim_reg.reset_boot();
    tile_fetcher.reset_boot();
    sprite_fetcher.reset_boot();
    sprite_scanner.reset_boot();
    bootrom.reset_boot();

    SOTO_DBG_VRAM.reset(REG_D0C0);

    PIN_LCD_DATA1.reset(TRI_HZNP);
    PIN_LCD_DATA0.reset(TRI_HZNP);
    PIN_LCD_CNTRL.reset(TRI_HZNP);
    PIN_LCD_CLOCK.reset(TRI_HZNP);
    PIN_LCD_HSYNC.reset(TRI_HZNP);
    PIN_LCD_LATCH.reset(TRI_HZNP);
    PIN_LCD_FLIPS.reset(TRI_HZNP);
    PIN_LCD_VSYNC.reset(TRI_HZNP);

    IE_D0.reset(REG_D0C0);
    IE_D1.reset(REG_D0C0);
    IE_D2.reset(REG_D0C0);
    IE_D3.reset(REG_D0C0);
    IE_D4.reset(REG_D0C0);

    lcd_data1_delay.reset();
    lcd_data0_delay.reset();

    lcd_pix_lo.reset(TRI_D0NP);
    lcd_pix_hi.reset(TRI_D0NP);

    for (int i = 0; i < 160; i++) {
      lcd_pipe_lo[i].reset(REG_D0C0);
      lcd_pipe_hi[i].reset(REG_D0C0);
      lcd_line_lo[i].reset(REG_D0C0);
      lcd_line_hi[i].reset(REG_D0C0);
    }
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
  /*p01.ABUZ*/ Sig ABUZ_xxCDEFGH;
  /*p07.TEDO*/ Sig TEDO_CPU_RDp;
  /*p28.LEKO*/ Sig LEKO_CPU_RDp;
  /*p07.TAPU*/ Sig TAPU_CPU_WRp_xxxxEFGx;
  /*p01.APOV*/ Sig APOV_CPU_WRp_xxxxEFGx;
  /*p27.NYXU*/ Sig NYXU_FETCH_TRIGn;
  /*p27.TEVO*/ Sig TEVO_FETCH_TRIGp;
  /*p27.TAVE*/ Sig TAVE_PRELOAD_DONE_TRIGp;
  /*p29.AVAP*/ Sig AVAP_RENDER_START_TRIGp;
  /*p07.TUTU*/ Sig TUTU_ADDR_BOOTp;

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

  DelayGlitch lcd_data1_delay;
  DelayGlitch lcd_data0_delay;

  /*PIN_50*/ Pin PIN_LCD_DATA1;
  /*PIN_51*/ Pin PIN_LCD_DATA0;
  /*PIN_52*/ Pin PIN_LCD_CNTRL;
  /*PIN_53*/ Pin PIN_LCD_CLOCK;
  /*PIN_54*/ Pin PIN_LCD_HSYNC;
  /*PIN_55*/ Pin PIN_LCD_LATCH;
  /*PIN_56*/ Pin PIN_LCD_FLIPS;
  /*PIN_57*/ Pin PIN_LCD_VSYNC;

  DFF IE_D0;
  DFF IE_D1;
  DFF IE_D2;
  DFF IE_D3;
  DFF IE_D4;

  NorLatch lcd_pix_lo;
  NorLatch lcd_pix_hi;

  DFF lcd_pipe_lo[160];
  DFF lcd_pipe_hi[160];
  DFF lcd_line_lo[160];
  DFF lcd_line_hi[160];
};
#pragma pack(pop)

//-----------------------------------------------------------------------------

};
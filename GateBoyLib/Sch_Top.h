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

  void tick_slow(wire RST, wire CLK, wire CLKGOOD, wire T1n, wire T2n);

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

  /*p25.SOTO*/ RegQN SOTO_DBG_VRAM  = REG_D0C0;

  Tri LCD_PIN_DATA1n = TRI_HZNP; // PIN_50 
  Tri LCD_PIN_DATA0n = TRI_HZNP; // PIN_51 
  Tri LCD_PIN_CNTRL = TRI_HZNP; // PIN_52 
  Tri LCD_PIN_CLOCK = TRI_HZNP; // PIN_53 
  Tri LCD_PIN_HSYNC = TRI_HZNP; // PIN_54 
  Tri LCD_PIN_LATCH = TRI_HZNP; // PIN_55 
  Tri LCD_PIN_ALTSG = TRI_HZNP; // PIN_56 
  Tri LCD_PIN_VSYNC = TRI_HZNP; // PIN_57 
};
#pragma pack(pop)

//-----------------------------------------------------------------------------

};
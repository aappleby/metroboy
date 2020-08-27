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

  SchematicTop();

  void tick_slow(wire RST, wire CLK, wire CLKGOOD, wire T1n, wire T2n);

  /*p04.DECY*/ wire DECY_LATCH_EXTn() {
    /*p04.DECY*/ wire DECY_LATCH_EXTn = not1(cpu_bus.CPU_PIN_LATCH_EXT.tp());
    return DECY_LATCH_EXTn;
  }

  /*p04.CATY*/ wire CATY_LATCH_EXTp() {
    /*p04.CATY*/ wire CATY_LATCH_EXTp = not1(DECY_LATCH_EXTn());
    return CATY_LATCH_EXTp;
  }

  // top.BETE, top.AJUJ
  /*p28.AJON*/ wire AJON_OAM_BUSY() const {
    /*p28.BOGE*/ wire BOGE_DMA_RUNNINGn = not1(dma_reg.MATU_DMA_RUNNINGp());
    /*p28.AJON*/ wire AJON_OAM_BUSY = and2(BOGE_DMA_RUNNINGn, pix_pipe.XYMU_RENDERINGp()); // def AND. ppu can read oam when there's rendering but no dma
    return AJON_OAM_BUSY;
  }

  // -> top.AMAB, top.APAG
  /*p28.AJUJ*/ wire AJUJ_OAM_BUSYn() const { 
     /*p28.AJUJ*/ wire AJUJ_OAM_BUSYn = nor3(dma_reg.MATU_DMA_RUNNINGp(), ACYL_SCANNINGp, AJON_OAM_BUSY()); // def nor4
     return AJUJ_OAM_BUSYn;
  }

  // -> oam.WYJA, oam.WUKU, oam.GUKO, top.APAG
  /*p28.AMAB*/ wire AMAB_CPU_READ_OAMp() const { 
      /*p28.AMAB*/ wire AMAB_CPU_READ_OAMp = and2(cpu_bus.SARO_FE00_FEFFp(), AJUJ_OAM_BUSYn()); // def and
      return AMAB_CPU_READ_OAMp;
  }

  //-----------------------------------------------------------------------------

  // boot.YULA, ext.SOBY, CPU_PIN_BOOTp
  /*p07.TUTU*/ wire TUTU_ADDR_BOOTp() const {
    /*p07.TERA*/ wire TERA_BOOT_BITp  = not1(bootrom.BOOT_BITn.qp());
    return and2(TERA_BOOT_BITp, cpu_bus.TULO_ADDR_00XXp());
  }

  //-----------------------------------------------------------------------------

  /*p07.UBET*/ Sig UBET_T1p;
  /*p07.UVAR*/ Sig UVAR_T2p;
  /*p07.UMUT*/ Sig UMUT_MODE_DBG1p;
  /*p07.UNOR*/ Sig UNOR_MODE_DBG2p;
  /*p07.UPOJ*/ Sig UPOJ_MODE_PRODn;
  /*p08.TOVA*/ Sig TOVA_MODE_DBG2n;

  /*p25.TUTO*/ Sig TUTO_DBG_VRAMp;
  /*p25.RACO*/ Sig RACO_DBG_VRAMn;

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
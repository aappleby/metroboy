#pragma once

#include "Sch_Timer.h"
#include "Sch_LCD.h"
#include "Sch_DMA.h"
#include "Sch_SpriteStore.h"
#include "Sch_Clocks.h"
#include "Sch_PixPipe.h"
#include "Sch_Joypad.h"
#include "Sch_Serial.h"
#include "Sch_Interrupts.h"
#include "Sch_Bootrom.h"
#include "Sch_SpriteFetcher.h"
#include "Sch_TileFetcher.h"
#include "Sch_SpriteScanner.h"

#include "Sch_CpuBus.h"
#include "Sch_ExtBus.h"
#include "Sch_OamBus.h"
#include "Sch_VramBus.h"

namespace Schematics {

//-----------------------------------------------------------------------------

struct SchematicTop {

  SchematicTop();

  void tick_slow(int phase);

  /*p29.AVAP*/ wire AVAP_RENDER_START_TRIGp() const {
    /*#p28.ANOM*/ wire ANOM_LINE_RSTn = nor2(lcd_reg.ATEJ_VID_LINE_END_TRIGp(), clk_reg.ATAR_VID_RSTp());
    /*#p29.BALU*/ wire BALU_LINE_RSTp = not1(ANOM_LINE_RSTn);
    /*#p29.BEBU*/ wire BEBU_SCAN_DONE_TRIGn = or3(sprite_scanner.DOBA_SCAN_DONE_B.qp(), BALU_LINE_RSTp, sprite_scanner.BYBA_SCAN_DONE_A.qn());
    /*#p29.AVAP*/ wire AVAP_RENDER_START_TRIGp = not1(BEBU_SCAN_DONE_TRIGn);
    return AVAP_RENDER_START_TRIGp;
  }

  // top.BETE, top.AJUJ
  /*p28.AJON*/ wire AJON_OAM_BUSY() const {
    /*p28.BOGE*/ wire BOGE_DMA_RUNNINGn = not1(dma_reg.MATU_DMA_RUNNINGp());
    /*p28.AJON*/ wire AJON_OAM_BUSY = and2(BOGE_DMA_RUNNINGn, pix_pipe.XYMU_RENDERINGp()); // def AND. ppu can read oam when there's rendering but no dma
    return AJON_OAM_BUSY;
  }

  // -> top.AMAB, top.APAG
  /*p28.AJUJ*/ wire AJUJ_OAM_BUSYn() const { 
     /*p28.AJUJ*/ wire AJUJ_OAM_BUSYn = nor3(dma_reg.MATU_DMA_RUNNINGp(), ACYL_SCANNINGp(), AJON_OAM_BUSY()); // def nor4
     return AJUJ_OAM_BUSYn;
  }

  // -> oam.WYJA, oam.WUKU, oam.GUKO, top.APAG
  /*p28.AMAB*/ wire AMAB_CPU_READ_OAMp() const { 
      /*p28.AMAB*/ wire AMAB_CPU_READ_OAMp = and2(cpu_bus.SARO_FE00_FEFFp(), AJUJ_OAM_BUSYn()); // def and
      return AMAB_CPU_READ_OAMp;
  }

  //-----------------------------------------------------------------------------

  // ext.TOZA, ext.SEPY, vram.TUCA
  /*p01.ABUZ*/ wire ABUZ_AVn() const {
    /*p??.APAP*/ wire APAP_AVn = not1(cpu_bus.CPU_PIN_ADDR_EXT.qp()); // Missing from schematic
    /*p01.AWOD*/ wire AWOD_AVp = nor2(clk_reg.UNOR_MODE_DBG2p(), APAP_AVn);
    /*p01.ABUZ*/ wire ABUZ_AVn = not1(AWOD_AVp);
    return ABUZ_AVn;
  }

  // boot.YULA, ext.SOBY, CPU_PIN_BOOTp
  /*p07.TUTU*/ wire TUTU_ADDR_BOOTp() const {
    /*p07.TERA*/ wire TERA_BOOT_BITp  = not1(bootrom.BOOT_BITn.qp());
    return and2(TERA_BOOT_BITp, cpu_bus.TULO_ADDR_00XXp());
  }

  // pxp.loze, pxp.luxa, tile.lony/lovy/laxu/mesu/nyva/moce

  // low on phase 0 of bg tile fetch

  wire NYXU_FETCH_TRIGn() const {
    /*p27.TEVO*/ wire TEVO_FETCH_TRIGp = or3(pix_pipe.SEKO_WIN_TILE_TRIG(),
                                             pix_pipe.SUZU_WIN_FIRST_TILEne(),
                                             TAVE_PRELOAD_DONE_TRIGp()); // Schematic wrong, this is OR
    /*p27.NYXU*/ wire NYXU_FETCH_TRIGn = nor3(AVAP_RENDER_START_TRIGp(),
                                              pix_pipe.MOSU_WIN_FETCH_TRIGp(),
                                              TEVO_FETCH_TRIGp);
    return NYXU_FETCH_TRIGn;
  }

  //-----------------------------------------------------------------------------

  // -> buncha stuff
  /*p07.TEDO*/ wire TEDO_CPU_RDp() const {
    /*p07.UJYV*/ wire UJYV_CPU_RDn = mux2_n(ext_bus.EXT_PIN_RD_C.qp(), cpu_bus.CPU_PIN_RDp.qp(), clk_reg.UNOR_MODE_DBG2p());
    /*p07.TEDO*/ wire TEDO_CPU_RDp = not1(UJYV_CPU_RDn);
    return TEDO_CPU_RDp;
  }

  // -> buncha stuff
  /*p07.ASOT*/ wire ASOT_CPU_RDp() const {
    /*p07.AJAS*/ wire _AJAS_CPU_RDn = not1(TEDO_CPU_RDp());
    /*p07.ASOT*/ wire ASOT_CPU_RDp = not1(_AJAS_CPU_RDn);
    return ASOT_CPU_RDp;
  }

  // vram.TUJA, top.UBAL/MEXO
  /*p01.APOV*/ wire APOV_CPU_WRp_xxxDxxxx() const {
    /*p01.AREV*/ wire AREV_CPU_WRn_ABCxEFGH = nand2(cpu_bus.CPU_PIN_WRp.qp(), clk_reg.AFAS_xxxDxxxx());
    /*p01.APOV*/ wire APOV_CPU_WRp_xxxDxxxx = not1(AREV_CPU_WRn_ABCxEFGH);
    return APOV_CPU_WRp_xxxDxxxx;
  }

  // boot.TUGE, int.REFA, joy.ATOZ, ser.URYS/UWAM, timer.TAPE/TOPE/TYJU/SARA, top.DYKY
  /*p07.TAPU*/ wire TAPU_CPU_WRp_xxxDxxxx() const {
    /*p01.AREV*/ wire AREV_CPU_WRn_ABCxEFGH = nand2(cpu_bus.CPU_PIN_WRp.qp(), clk_reg.AFAS_xxxDxxxx());
    /*p01.APOV*/ wire APOV_CPU_WRp_xxxDxxxx = not1(AREV_CPU_WRn_ABCxEFGH);
    /*p07.UBAL*/ wire UBAL_CPU_WRn_ABCxEFGH = mux2_n(ext_bus.EXT_PIN_WR_C.qp(), APOV_CPU_WRp_xxxDxxxx, clk_reg.UNOR_MODE_DBG2p());
    /*p07.TAPU*/ wire TAPU_CPU_WRp_xxxDxxxx = not1(UBAL_CPU_WRn_ABCxEFGH);
    return TAPU_CPU_WRp_xxxDxxxx;
  }

  // dma.lavy, lcd.xufa, oam.wyja, pxp.vely/xoma/myxe, ppu.waru/sepa, tile.bedy/arur, top.xuto, win.weko/wuza (most all the FF4X regs)
  /*p07.CUPA*/ wire CUPA_CPU_WRp_xxxDxxxx() const {
    /*p07.DYKY*/ wire DYKY_CPU_WRn_ABCxEFGH = not1(TAPU_CPU_WRp_xxxDxxxx());
    /*p07.CUPA*/ wire CUPA_CPU_WRp_xxxDxxxx = not1(DYKY_CPU_WRn_ABCxEFGH);
    return CUPA_CPU_WRp_xxxDxxxx;
  }

  // ext.NEVY
  /*p08.MEXO*/ wire MEXO_CPU_WRn_ABCxEFGH() const {
    /*p08.MEXO*/ wire MEXO_CPU_WRn_ABCxEFGH = not1(APOV_CPU_WRp_xxxDxxxx());
    return MEXO_CPU_WRn_ABCxEFGH;
  }

  //-----------------------------------------------------------------------------
  // PPU signals

  // int.asam, oam.aver/ajep, ppu.xaty, top.apar/.ajuj
  // so dma stops oam scan?
  /*p28.ACYL*/ wire ACYL_SCANNINGp() const {
    /*p28.ACYL*/ wire ACYL_SCANNINGp = and2(dma_reg.BOGE_DMA_RUNNINGn(), sprite_scanner.BESU_SCANNINGp.qp());
    return ACYL_SCANNINGp;
  }

  // -> sprite fetcher, top.TEVO
  /*p27.TAVE*/ wire TAVE_PRELOAD_DONE_TRIGp() const {
    /*p27.ROMO*/ wire _ROMO_PRELOAD_DONEn = not1(tile_fetcher._POKY_PRELOAD_LATCHp.qp());
    /*p27.SUVU*/ wire _SUVU_PRELOAD_DONE_TRIGn = nand4(pix_pipe.XYMU_RENDERINGp(), _ROMO_PRELOAD_DONEn, tile_fetcher._NYKA_FETCH_DONE_P11.qp(), tile_fetcher._PORY_FETCH_DONE_P12.qp());
    return  not1(_SUVU_PRELOAD_DONE_TRIGn);
  }

  // -> ppu.PASO, window.VETU, top.NYXU_TILE_FETCHER_RSTn
  /*p27.TEVO*/ wire TEVO_FETCH_TRIGp() const { 
    /*p27.TEVO*/ wire TEVO_FETCH_TRIGp = or3(pix_pipe.SEKO_WIN_TILE_TRIG(),
                                             pix_pipe.SUZU_WIN_FIRST_TILEne(),
                                             TAVE_PRELOAD_DONE_TRIGp()); // Schematic wrong, this is OR
    return TEVO_FETCH_TRIGp;
  }

  //-----------------------------------------------------------------------------
  // Sub-modules

  OamBus oam_bus;
  ExtBus ext_bus;
  CpuBus cpu_bus;
  VramBus vram_bus;

  ClockRegisters clk_reg;
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

  Tri LCD_PIN_DATA1 = TRI_HZNP; // PIN_50 
  Tri LCD_PIN_DATA0 = TRI_HZNP; // PIN_51 
  Tri LCD_PIN_CNTRL = TRI_HZNP; // PIN_52 
  Tri LCD_PIN_CLOCK = TRI_HZNP; // PIN_53 
  Tri LCD_PIN_HSYNC = TRI_HZNP; // PIN_54 
  Tri LCD_PIN_LATCH = TRI_HZNP; // PIN_55 
  Tri LCD_PIN_ALTSG = TRI_HZNP; // PIN_56 
  Tri LCD_PIN_VSYNC = TRI_HZNP; // PIN_57 
};

//-----------------------------------------------------------------------------

};
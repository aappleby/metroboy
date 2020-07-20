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

  SchematicTop& top = *this;

  void tick();
  uint64_t commit();

  // top.BETE, top.AJUJ
  /*p28.AJON*/ wire AJON_OAM_BUSY() const {
    /*p28.BOGE*/ wire BOGE_DMA_RUNNINGn = not(dma_reg.MATU_DMA_RUNNINGp());
    return and(BOGE_DMA_RUNNINGn, pix_pipe.XYMU_RENDERINGp()); // def AND. ppu can read oam when there's rendering but no dma
  }

  // -> top.AMAB, top.APAG
  /*p28.AJUJ*/ wire AJUJ_OAM_BUSYn() const { 
     return nor(top.dma_reg.MATU_DMA_RUNNINGp(), top.ACYL_SCANNINGp(), top.AJON_OAM_BUSY()); // def nor
  }

  // -> oam.WYJA, oam.WUKU, oam.GUKO, top.APAG
  /*p28.AMAB*/ wire AMAB_OAM_LOCKn() const { 
    return and (top.cpu_bus.SARO_FE00_FEFFp(), AJUJ_OAM_BUSYn()); // def and
  }

  //-----------------------------------------------------------------------------

  // ext.TOZA, ext.SEPY, vram.TUCA
  /*p01.ABUZ*/ wire ABUZ_CPU_ADDR_VALIDp() const {
    /*p01.AWOD*/ wire AWOD_CPU_ADDR_VALIDn = nor(top.clk_reg.UNOR_MODE_DBG2p(), top.cpu_bus.APAP_CPU_ADDR_VALIDp());
    return not(AWOD_CPU_ADDR_VALIDn);
  }

  // boot.YULA, ext.SOBY, CPU_PIN_BOOTp
  /*p07.TUTU*/ wire TUTU_ADDR_BOOTp() const {
    /*p07.TERA*/ wire TERA_BOOT_BITp  = not(bootrom.BOOT_BITn());
    return and(TERA_BOOT_BITp, top.cpu_bus.TULO_ADDR_00XXp());
  }

  // pxp.loze, pxp.luxa, tile.lony/lovy/laxu/mesu/nyva/moce
  /*p27.NYXU*/ wire NYXU_TILE_FETCHER_RSTn() const { return nor(sprite_scanner.AVAP_RENDER_START_TRIGp(), pix_pipe.MOSU_TILE_FETCHER_RSTp(), top.TEVO_FINE_RSTp()); }

  //-----------------------------------------------------------------------------

  // -> buncha stuff
  /*p07.TEDO*/ wire TEDO_CPU_RDp() const {
    /*p07.UJYV*/ wire UJYV_CPU_RDn = mux2_n(ext_bus.EXT_PIN_RDp_C(), cpu_bus.CPU_PIN_RDp(), top.clk_reg.UNOR_MODE_DBG2p());
    return not(UJYV_CPU_RDn);
  }

  // -> buncha stuff
  /*p07.ASOT*/ wire ASOT_CPU_RDp() const {
    /*p07.AJAS*/ wire _AJAS_CPU_RDn = not(TEDO_CPU_RDp());
    return not(_AJAS_CPU_RDn);
  }

  // oam.WUKU/GUKO, vram.TYVY
  /*p28.LEKO*/ wire LEKO_CPU_RDp() const {
    /*p04.CATY*/ wire CATY_FROM_CPU5p = not(top.cpu_bus.DECY_FROM_CPU5n());
    /*p28.MYNU*/ wire MYNU_CPU_RDn = nand(ASOT_CPU_RDp(), CATY_FROM_CPU5p);
    return not(MYNU_CPU_RDn);
  }

  // vram.TUJA, top.UBAL/MEXO
  /*p01.APOV*/ wire APOV_CPU_WRp_xxxxEFGx() const {
    /*p01.AREV*/ wire _AREV_CPU_WRn_ABCDxxxH = nand(cpu_bus.CPU_PIN_WRp(), clk_reg.AFAS_xxxxEFGx());
    return not(_AREV_CPU_WRn_ABCDxxxH);
  }

  // boot.TUGE, int.REFA, joy.ATOZ, ser.URYS/UWAM, timer.TAPE/TOPE/TYJU/SARA, top.DYKY
  /*p07.TAPU*/ wire TAPU_CPU_WRp_xxxxEFGx() const {
    /*p07.UBAL*/ wire _UBAL_CPU_WRn_ABCDxxxH = mux2_n(ext_bus.EXT_PIN_WRp_C(), APOV_CPU_WRp_xxxxEFGx(), top.clk_reg.UNOR_MODE_DBG2p());
    return not(_UBAL_CPU_WRn_ABCDxxxH);
  }

  // dma.lavy, lcd.xufa, oam.wyja, pxp.vely/xoma/myxe, ppu.waru/sepa, tile.bedy/arur, top.xuto, win.weko/wuza (most all the FF4X regs)
  /*p07.CUPA*/ wire CUPA_CPU_WRp_xxxxEFGx() const {
    /*p07.DYKY*/ wire _DYKY_CPU_WRn_ABCDxxxH = not(TAPU_CPU_WRp_xxxxEFGx());
    return not(_DYKY_CPU_WRn_ABCDxxxH);
  }

  // ext.NEVY
  /*p08.MEXO*/ wire MEXO_CPU_WRn_ABCDxxxH() const { return not(APOV_CPU_WRp_xxxxEFGx()); }

  //-----------------------------------------------------------------------------
  // PPU signals

  // int.asam, oam.aver/ajep, ppu.xaty, top.apar/.ajuj
  /*p28.ACYL*/ wire ACYL_SCANNINGp() const { return and(dma_reg.BOGE_DMA_RUNNINGn(), sprite_scanner.BESU_SCANNINGp()); } // so dma stops oam scan?


  // -> ppu.PASO, window.VETU, top.NYXU_TILE_FETCHER_RSTn
  /*p27.TEVO*/ wire TEVO_FINE_RSTp() const { 
    return nor(top.pix_pipe.SEKO_WX_MATCHne(), top.pix_pipe.SUZU_WIN_FIRST_TILEne(), tile_fetcher.TAVE_PORCH_DONE_TRIGp());
  }

  //-----------------------------------------------------------------------------
  // Internal state for debugging

  int phase_count = -17;
  int pass_count = 0;
  
  uint64_t phase_hash = 0;
  uint64_t combined_hash = 0;

  //-----------------------------------------------------------------------------
  // Sys pins

#if 0
  if (RST) {
    // This can't be run mode, TAPE doesn't affect UKUP
    ADYK_xxxDEFGx = ff9( ATAL_xBxDxFxH, !ATAL_xBxDxFxH, or(T1, T2), APUK_xxCDEFxx.q());
    UKUP_DIV_00 = ff9(BOGA_xBCDEFGH, 0, UKUP_DIV_00.qn());
  }
  else {
    if (CLKIN_A) {
      // So this must be run mode, and ADYK must have _negative_ reset
      // RST = 0
      // CLKIN_A = 1
      ADYK_xxxDEFGx = ff9( ATAL_xBxDxFxH, !ATAL_xBxDxFxH, 1, APUK_xxCDEFxx.q());
      UKUP_DIV_00 = ff9(BOGA_xBCDEFGH, !TAPE, UKUP_DIV_00.qn());  
    }
    else {
      // This also can't be run mode
      ADYK_xxxDEFGx = ff9( ATAL_xBxDxFxH, !ATAL_xBxDxFxH, 1, APUK_xxCDEFxx.q());
      UKUP_DIV_00 = ff9(BOGA_xBCDEFGH, 0, UKUP_DIV_00.qn());  
    }
  }
#endif

  //-----------------------------------------------------------------------------
  // Sub-modules

  //private:

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
};

//-----------------------------------------------------------------------------

};
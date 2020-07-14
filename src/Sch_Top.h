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

  SchematicTop& top = *this;

  SignalHash tick();
  void tock_buses();

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

  int phase_counter = -17;

  //-----------------------------------------------------------------------------
  // Sys pins

#if 0
  if (RST) {
    // This can't be run mode, TAPE doesn't affect UKUP
    ADYK_xxxDEFGx.set( ATAL_xBxDxFxH, !ATAL_xBxDxFxH, or(T1, T2), APUK_xxCDEFxx.q());
    UKUP_DIV_00.set(BOGA_xBCDEFGH, 0, UKUP_DIV_00.qn());
  }
  else {
    if (CLKIN_A) {
      // So this must be run mode, and ADYK must have _negative_ reset
      // RST = 0
      // CLKIN_A = 1
      ADYK_xxxDEFGx.set( ATAL_xBxDxFxH, !ATAL_xBxDxFxH, 1, APUK_xxCDEFxx.q());
      UKUP_DIV_00.set(BOGA_xBCDEFGH, !TAPE, UKUP_DIV_00.qn());  
    }
    else {
      // This also can't be run mode
      ADYK_xxxDEFGx.set( ATAL_xBxDxFxH, !ATAL_xBxDxFxH, 1, APUK_xxCDEFxx.q());
      UKUP_DIV_00.set(BOGA_xBCDEFGH, 0, UKUP_DIV_00.qn());  
    }
  }
#endif

  //-----------------------------------------------------------------------------
  // Top level registers

  /*p31.YLOR*/ Reg8 YLOR_OAM_DA0; // sprite x bit 0, 
  /*p31.ZYTY*/ Reg8 ZYTY_OAM_DA1; // sprite x bit 1, 
  /*p31.ZYVE*/ Reg8 ZYVE_OAM_DA2; // sprite x bit 2, 
  /*p31.ZEZY*/ Reg8 ZEZY_OAM_DA3; // sprite x bit 3, 
  /*p31.GOMO*/ Reg8 GOMO_OAM_DA4; // sprite x bit 4, sprite palette
  /*p31.BAXO*/ Reg8 BAXO_OAM_DA5; // sprite x bit 5, sprite x flip
  /*p31.YZOS*/ Reg8 YZOS_OAM_DA6; // sprite x bit 6, sprite y flip
  /*p31.DEPO*/ Reg8 DEPO_OAM_DA7; // sprite x bit 7, sprite priority

  /*p29.XUSO*/ Reg8 XUSO_OAM_DB0; // sprite y bit 0, sprite tile index bit 0
  /*p29.XEGU*/ Reg8 XEGU_OAM_DB1; // sprite y bit 1, sprite tile index bit 1
  /*p29.YJEX*/ Reg8 YJEX_OAM_DB2; // sprite y bit 2, sprite tile index bit 2
  /*p29.XYJU*/ Reg8 XYJU_OAM_DB3; // sprite y bit 3, sprite tile index bit 3
  /*p29.YBOG*/ Reg8 YBOG_OAM_DB4; // sprite y bit 4, sprite tile index bit 4
  /*p29.WYSO*/ Reg8 WYSO_OAM_DB5; // sprite y bit 5, sprite tile index bit 5
  /*p29.XOTE*/ Reg8 XOTE_OAM_DB6; // sprite y bit 6, sprite tile index bit 6
  /*p29.YZAB*/ Reg8 YZAB_OAM_DB7; // sprite y bit 7, sprite tile index bit 7

  /*p32.LEGU*/ Reg8 LEGU_TILE_DA0;
  /*p32.NUDU*/ Reg8 NUDU_TILE_DA1;
  /*p32.MUKU*/ Reg8 MUKU_TILE_DA2;
  /*p32.LUZO*/ Reg8 LUZO_TILE_DA3;
  /*p32.MEGU*/ Reg8 MEGU_TILE_DA4;
  /*p32.MYJY*/ Reg8 MYJY_TILE_DA5;
  /*p32.NASA*/ Reg8 NASA_TILE_DA6;
  /*p32.NEFO*/ Reg8 NEFO_TILE_DA7; // color wrong on die

  /*p32.RAWU*/ Reg11 RAWU_TILE_DB0; // also holds tile index during fetch
  /*p32.POZO*/ Reg11 POZO_TILE_DB1;
  /*p32.PYZO*/ Reg11 PYZO_TILE_DB2; 
  /*p32.POXA*/ Reg11 POXA_TILE_DB3; 
  /*p32.PULO*/ Reg11 PULO_TILE_DB4; 
  /*p32.POJU*/ Reg11 POJU_TILE_DB5; 
  /*p32.POWY*/ Reg11 POWY_TILE_DB6; 
  /*p32.PYJU*/ Reg11 PYJU_TILE_DB7;

  /*p33.PEFO*/ Reg8 PEFO_SPRITE_DA0;
  /*p33.ROKA*/ Reg8 ROKA_SPRITE_DA1;
  /*p33.MYTU*/ Reg8 MYTU_SPRITE_DA2;
  /*p33.RAMU*/ Reg8 RAMU_SPRITE_DA3;
  /*p33.SELE*/ Reg8 SELE_SPRITE_DA4;
  /*p33.SUTO*/ Reg8 SUTO_SPRITE_DA5;
  /*p33.RAMA*/ Reg8 RAMA_SPRITE_DA6;
  /*p33.RYDU*/ Reg8 RYDU_SPRITE_DA7;

  /*p33.REWO*/ Reg8 REWO_SPRITE_DB0;
  /*p33.PEBA*/ Reg8 PEBA_SPRITE_DB1;
  /*p33.MOFO*/ Reg8 MOFO_SPRITE_DB2;
  /*p33.PUDU*/ Reg8 PUDU_SPRITE_DB3;
  /*p33.SAJA*/ Reg8 SAJA_SPRITE_DB4;
  /*p33.SUNY*/ Reg8 SUNY_SPRITE_DB5;
  /*p33.SEMO*/ Reg8 SEMO_SPRITE_DB6;
  /*p33.SEGA*/ Reg8 SEGA_SPRITE_DB7;

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
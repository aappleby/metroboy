#include "Sch_Video.h"
#include "Sch_Gameboy.h"

#include "Sch_SpriteStore.h"

namespace Schematics {

//-----------------------------------------------------------------------------
// This file should contain the schematics as directly translated to C,
// no modifications or simplifications.

void P21_VideoControl_tick(const Bus& bus,
                           const BusControl& ctl,
                           const Pins& pins,
                           const LCDRegisters& lcd,
                           const Registers& regs,
                           const ClockSignals1& clk_sig1,
                           const ClockSignals2& clk_sig2,
                           const ResetSignals1& rst_sig1,
                           const ResetSignals2& rst_sig2,
                           const Decoder& dec,
                           const Sprites& spr,
                           const Video& vid,
                           const SpriteStore& sst,

                           Video& next,
                           Bus& bus_out) {

  /*p27.VYPO*/ wire VYPO = not(pins.P10_B);

  //----------

  {
    // We stop the pipe as soon as there's a store match or we switch to window mode
    {
      /*p27.PAHA*/ wire RENDERINGn = not(vid.RENDERING_LATCH);

      // weird latch
      /*p27.ROXY*/ if (RENDERINGn)          next.FINE_MATCH_DUMP = 1; 
      /*p27.ROXY*/ if (vid.FINE_MATCH_TRIG) next.FINE_MATCH_DUMP = 0;

      /*p24.SOCY*/ wire WIN_MODE2p = not(vid.WIN_MODE2b);
      /*p24.VYBO*/ wire VYBO = nor(sst.STORE_MATCH, vid.RENDER_DONE, clk_sig1.MYVO_xBxDxFxH);
      /*p24.TYFA*/ wire TYFA = and(WIN_MODE2p, vid.FRONT_PORCH_LATCHn, VYBO);

      /*p24.SEGU*/ next.SEGU_4M = not(TYFA);
      /*p24.ROXO*/ next.ROXO_4M = not(vid.SEGU_4M);
      /*p27.ROCO*/ next.ROCO_4M = not(vid.SEGU_4M);
      /*p24.SACU*/ next.CLKPIPE = nor(vid.SEGU_4M, vid.FINE_MATCH_DUMP);
    }

    // But we don't enter sprite mode until background sequencer hits BG_SEQ5
    {
      /*p27.SOWO*/ wire SPRITE_FETCH_LATCHn = not(vid.SPRITE_FETCH_LATCH);
      /*p27.TUKU*/ wire WIN_MODE2o = not(vid.WIN_MODE2b);
      /*p27.TEKY*/ wire SPRITE_FETCH = and(sst.STORE_MATCH, WIN_MODE2o, vid.BG_SEQ_5, SPRITE_FETCH_LATCHn);
      /*p27.SOBU*/ next.SPRITE_FETCH_SYNC1.tock(clk_sig1.TAVA_AxCxExGx, VYPO, SPRITE_FETCH);
      /*p27.SUDA*/ next.SPRITE_FETCH_SYNC2.tock(clk_sig1.LAPE_xBxDxFxH, VYPO,  vid.SPRITE_FETCH_SYNC1);
      /*p27.RYCE*/ next.SPRITE_FETCH_TRIG = and(vid.SPRITE_FETCH_SYNC1, !vid.SPRITE_FETCH_SYNC2);

      /*p27.SECA*/ wire SPRITE_FETCH_BEGINn = nor(vid.SPRITE_FETCH_TRIG, rst_sig2.VID_RESET5, lcd.VID_LINE_TRIG_d4n);
      /*p27.TAKA*/ if (!SPRITE_FETCH_BEGINn) next.SPRITE_FETCH_LATCH = 1; // weird latch

      // I guess this blocks sprite fetches for the front porch?
      /*p27.SUVU*/   wire SUVU = nand(vid.RENDERING_LATCH, vid.FRONT_PORCH, vid.BG_SEQ_6, vid.BG_SEQ_7);
      /*p27.TAVE*/ next.TAVE = not(SUVU);

      /*p27.VEKU*/ wire SPRITE_FETCH_ENDn = nor(spr.SPRITE_DONE, vid.TAVE);
      /*p27.TAKA*/ if (!SPRITE_FETCH_ENDn) next.SPRITE_FETCH_LATCH = 0; // weird latch

      /*p27.TUXY*/     wire TUXY = nand(vid.WIN_MODE_SYNC2, vid.WIN_MODE2n);
      /*p27.SUZU*/   wire SUZU = not(TUXY);
      /*p27.TEVO*/ next.MAP_X_CLK_STOPn = nor(vid.WIN_TRIGGER, SUZU, vid.TAVE);
    }
  }

  {
    /*p27.LAXE*/   wire BG_SEQ0n = not(vid.BG_SEQ_x1x3x5x7);
    /*p27.MYSO*/ wire BG_SEQ_TRIG_1357 = nor(vid.RENDERINGn, BG_SEQ0n, vid.BG_SEQ_x1x3x5x7_DELAY);

    /*p27.NOFU*/       wire BG_SEQ_0123xxxx  = not(vid.BG_SEQ_xxxx4567);
    /*p27.NYDY*/     wire BG_SEQ_TRIG_3n = nand(BG_SEQ_TRIG_1357, vid.BG_SEQ_xx23xx67, BG_SEQ_0123xxxx);
    /*p32.METE*/   wire BG_SEQ_TRIG_3 = not(BG_SEQ_TRIG_3n);
    /*p32.LOMA*/ next.BG_LATCH = not(BG_SEQ_TRIG_3);

    /*p27.NAKO*/         wire BG_SEQ_01xx45xx  = not(vid.BG_SEQ_xx23xx67);
    /*p27.MOFU*/       wire MOFU = and(BG_SEQ_TRIG_1357, BG_SEQ_01xx45xx);
    /*p32.LESO*/     wire LESO = not(MOFU);
    /*p32.AJAR*/   wire AJAR = not(LESO);
    /*p32.LABU*/ next.VRAM_TEMP_CLK = not(AJAR);
  }

  //----------
  // FF41 STAT

  
  //---

  {
    /*p21.WEGO*/ wire WEGO = or(rst_sig2.VID_RESET3, vid.RENDER_DONE_SYNC);

    // Weird latch? Weird latch!
    /*p21.XYMU*/ if (WEGO)               next.RENDERING_LATCH = 0;
    /*p21.XYMU*/ if (spr.SCAN_DONE_d0_TRIG) next.RENDERING_LATCH = 1;
    
    /*p24.LOBY*/ next.RENDERINGn = not(vid.RENDERING_LATCH);
    
    /*p21.XENA*/ wire STORE_MATCHn = not(sst.STORE_MATCH);
    /*p21.WODU*/ next.RENDER_DONE = and(STORE_MATCHn, vid.X_167);
    /*p21.VOGA*/ next.RENDER_DONE_SYNC.tock(clk_sig1.ALET_AxCxExGx, vid.X_RST, vid.RENDER_DONE);
  }


  //---


  {
    /*p27.NENY*/ wire BG_SEQ_01xxxxxx = not(vid.BG_SEQ_xx234567);

    /*p27.POTU*/ wire POTU = and(vid.BG_READ_VRAM, BG_SEQ_01xxxxxx);
    /*p25.XEZE*/ next.WIN_MAP_READ  = nand(POTU, vid.WIN_MODE_PORE);
    /*p26.ACEN*/ next.BG_MAP_READ   = and (POTU, vid.WIN_MODE_AXADn);


    /*p27.NOFU*/ wire BG_SEQ_0123xxxx = not(vid.BG_SEQ_xxxx4567);
    /*p27.XUHA*/ next.TILE_READ_AB  = not(BG_SEQ_0123xxxx);

    /*p27.NETA*/ next.TILE_READ     = and(vid.BG_READ_VRAM, vid.BG_SEQ_xx234567);

    /*p25.XUCY*/ next.WIN_TILE_READ = nand(vid.TILE_READ, vid.WIN_MODE_PORE);
    /*p26.ASUL*/ wire BG_TILE_READn = and (vid.TILE_READ, vid.WIN_MODE_AXADn);
    /*p26.BEJE*/ next.BG_TILE_READ  = not(BG_TILE_READn);

  }

  //----------

  //----------
  // Fine x matcher for x scroll

  {
    /*p27.SUHA*/ wire FINE_MATCH0n = xor(regs.SCX0, vid.FINE_CNT0);
    /*p27.SYBY*/ wire FINE_MATCH1n = xor(regs.SCX1, vid.FINE_CNT1);
    /*p27.SOZU*/ wire FINE_MATCH2n = xor(regs.SCX2, vid.FINE_CNT2);
    /*p27.RONE*/ wire FINE_MATCHn  = nand(vid.FINE_MATCH_DUMP, FINE_MATCH0n, FINE_MATCH1n, FINE_MATCH2n);
    /*p27.POHU*/ wire FINE_MATCH   = not(FINE_MATCHn);

    /*p27.PUXA*/ next.FINE_MATCH_SYNC1.tock(vid.ROXO_4M,          vid.RENDERING_LATCH, FINE_MATCH);
    /*p27.NYZE*/ next.FINE_MATCH_SYNC2.tock(clk_sig1.MOXE_xBxDxFxH, vid.RENDERING_LATCH, vid.FINE_MATCH_SYNC1);
    /*p27.POVA*/ next.FINE_MATCH_TRIG = and(vid.FINE_MATCH_SYNC1, !vid.FINE_MATCH_SYNC2);
  }

  {
    /*p25.ROPY*/ wire ROPY_RENDERINGn = not(vid.RENDERING_LATCH);
    /*p27.ROZE*/ wire FINE_COUNT_STOPn = nand(vid.FINE_CNT0, vid.FINE_CNT1, vid.FINE_CNT2);
    /*p27.PASO*/ wire FINE_RST = nor(vid.MAP_X_CLK_STOPn, ROPY_RENDERINGn);
    /*p27.PECU*/ wire FINE_CLK = nand(vid.ROXO_4M, FINE_COUNT_STOPn);
    /*p27.RYKU*/ next.FINE_CNT0.tock( FINE_CLK,      FINE_RST, !vid.FINE_CNT0);
    /*p27.ROGA*/ next.FINE_CNT1.tock(!vid.FINE_CNT0, FINE_RST, !vid.FINE_CNT1);
    /*p27.RUBU*/ next.FINE_CNT2.tock(!vid.FINE_CNT1, FINE_RST, !vid.FINE_CNT2);
  }

  {
    /*p27.ROZE*/ wire FINE_COUNT_STOPn = nand(vid.FINE_CNT0, vid.FINE_CNT1, vid.FINE_CNT2);
    /*p27.PANY*/ wire WIN_MATCH_ONSCREEN = nor(vid.WIN_MATCH, FINE_COUNT_STOPn);

    /*p27.RYFA*/ next.WIN_MATCH_ONSCREEN_SYNC1.tock(vid.SEGU_4M,          vid.RENDERING_LATCH, WIN_MATCH_ONSCREEN);
    /*p27.RENE*/ next.WIN_MATCH_ONSCREEN_SYNC2.tock(clk_sig1.ALET_AxCxExGx, vid.RENDERING_LATCH, vid.WIN_MATCH_ONSCREEN_SYNC1);
    /*p27.SEKO*/ next.WIN_TRIGGER = nor(vid.WIN_MATCH_ONSCREEN_SYNC2, !vid.WIN_MATCH_ONSCREEN_SYNC1);

    /*p27.PYCO*/ next.WIN_MATCH_SYNC1.tock(vid.ROCO_4M,            rst_sig2.VID_RESETn, vid.WIN_MATCH);
    /*p27.NUNU*/ next.WIN_MATCH_SYNC2.tock(clk_sig1.MEHE_xBxDxFxH, rst_sig2.VID_RESETn, vid.WIN_MATCH_SYNC1);

    /*p27.XOFO*/ wire LINE_RST = nand(regs.LCDC_WINEN, lcd.VID_LINE_TRIG_d4o, rst_sig2.VID_RESETn);

    // weird latch
    /*p27.PYNU*/ if (LINE_RST)            next.WIN_MODE_LATCH = 0;
    /*p27.PYNU*/ if (vid.WIN_MATCH_SYNC2) next.WIN_MODE_LATCH = 1;

    /*p27.NOCU*/ next.WIN_MODE_NOCUn = not(vid.WIN_MODE_LATCH);
    /*p27.PORE*/ next.WIN_MODE_PORE  = not(vid.WIN_MODE_NOCUn);
    /*p26.AXAD*/ next.WIN_MODE_AXADn = not(vid.WIN_MODE_PORE);

    /*p27.NOPA*/ next.WIN_MODE_SYNC.tock(clk_sig1.ALET_AxCxExGx, rst_sig2.VID_RESETn, vid.WIN_MODE_LATCH);
    /*p27.NUNY*/ next.WIN_MODE_TRIG = and(vid.WIN_MODE_LATCH, !vid.WIN_MODE_SYNC);

    // PUKU/RYDY form a NOR latch. WIN_MODE_TRIG is SET, (VID_RESET | BG_SEQ_7) is RESET.

    ///*p27.PUKU*/ next.PUKU = nor(vid.RYDY, vid.WIN_MODE_TRIG);
    ///*p27.RYDY*/ next.RYDY = nor(vid.PUKU, rst_reg.VID_RESET4, vid.BG_SEQ_7);

    if (vid.WIN_MODE_TRIG) {
      next.PUKU = 0;
      next.WIN_MODE_LATCH2 = 1;
    }
    if (rst_sig2.VID_RESET4 || vid.BG_SEQ_7) {
      next.PUKU = 1;
      next.WIN_MODE_LATCH2 = 0;
    }

    /*p27.SYLO*/ next.WIN_MODE2n = not(vid.WIN_MODE_LATCH2);
    /*p24.TOMU*/ next.WIN_MODE2b = not(vid.WIN_MODE2n);

    /*p27.SOVY*/ next.WIN_MODE_SYNC2.tock(clk_sig1.ALET_AxCxExGx, rst_sig2.VID_RESETn, vid.WIN_MODE_LATCH2);
  }

  //----------

  {
    /*p27.NYFO*/ wire WIN_MODE_TRIGn = not(vid.WIN_MODE_TRIG);
    /*p27.MOSU*/ wire WIN_MODE_TRIGb = not(WIN_MODE_TRIGn);

    /*p27.NYXU*/ wire BG_SEQ_RSTn = nor(spr.SCAN_DONE_d0_TRIG, WIN_MODE_TRIGb, vid.MAP_X_CLK_STOPn);

    // counts to 5? polarity?
    /*p27.MOCE*/ wire BG_SEQ_5n = nand(vid.BG_SEQ_x1x3x5x7, vid.BG_SEQ_xxxx4567, BG_SEQ_RSTn);
    /*p27.LYRY*/ next.BG_SEQ_5 = not(BG_SEQ_5n);

    /*p27.LEBO*/ wire BG_SEQ_CLK = nand(clk_sig1.ALET_AxCxExGx, BG_SEQ_5n);
    /*p27.LAXU*/ next.BG_SEQ_x1x3x5x7.tock( BG_SEQ_CLK,          BG_SEQ_RSTn, !vid.BG_SEQ_x1x3x5x7);
    /*p27.MESU*/ next.BG_SEQ_xx23xx67.tock(!vid.BG_SEQ_x1x3x5x7, BG_SEQ_RSTn, !vid.BG_SEQ_xx23xx67);
    /*p27.NYVA*/ next.BG_SEQ_xxxx4567.tock(!vid.BG_SEQ_xx23xx67, BG_SEQ_RSTn, !vid.BG_SEQ_xxxx4567);

    /*p27.NAKO*/ wire BG_SEQ_01xx45xx  = not(vid.BG_SEQ_xx23xx67);
    /*p27.NOFU*/ wire BG_SEQ_0123xxxx  = not(vid.BG_SEQ_xxxx4567);
    /*p27.NOGU*/ next.BG_SEQ_xx234567 = nand(BG_SEQ_01xx45xx, BG_SEQ_0123xxxx);



    /*p24.NAFY*/ wire RENDERING_AND_NOT_WIN_TRIG = nor(WIN_MODE_TRIGb, vid.RENDERINGn);
    /*p24.NYKA*/ next.BG_SEQ_6.tock(clk_sig1.ALET_AxCxExGx, RENDERING_AND_NOT_WIN_TRIG, vid.BG_SEQ_5);
    /*p24.PORY*/ next.BG_SEQ_7.tock(clk_sig1.MYVO_xBxDxFxH, RENDERING_AND_NOT_WIN_TRIG, vid.BG_SEQ_6);

    /*p27.LYZU*/ next.BG_SEQ_x1x3x5x7_DELAY.tock(clk_sig1.ALET_AxCxExGx, vid.RENDERING_LATCH, vid.BG_SEQ_x1x3x5x7);

    /*p27.LOVY*/ next.BG_SEQ5_SYNC.tock(clk_sig1.MYVO_xBxDxFxH, BG_SEQ_RSTn, vid.BG_SEQ_5);
    /*p27.LURY*/ wire LURY = and(!vid.BG_SEQ5_SYNC, vid.RENDERING_LATCH);
    
    // weird latch? polarity?
    /*p27.LONY*/ if (BG_SEQ_RSTn) next.BG_READ_VRAM_LATCH = 0;
    /*p27.LONY*/ if (LURY)        next.BG_READ_VRAM_LATCH = 1;

    /*p27.LUSU*/ wire BG_READ_VRAMn = not(vid.BG_READ_VRAM_LATCH);
    /*p27.LENA*/ next.BG_READ_VRAM  = not(BG_READ_VRAMn);
  }












































  //----------
  // The first tile generated is thrown away. I'm calling that section of rendering the front porch.

  {
    /*p24.PYGO*/ next.TILE_DONE.tock(clk_sig1.ALET_AxCxExGx, vid.RENDERING_LATCH, vid.BG_SEQ_7);
    
    // weird latch
    /*p24.POKY*/ if (vid.TILE_DONE)  next.FRONT_PORCH_LATCHn = 1; 
    /*p24.POKY*/ if (vid.RENDERINGn) next.FRONT_PORCH_LATCHn = 0;
    
    /*p27.ROMO*/ next.FRONT_PORCH = not(vid.FRONT_PORCH_LATCHn);
  }

  //----------
  // FF44 LY

  {
    /*p23.WAFU*/ wire FF44_RD = and(ctl.CPU_RD2, dec.FF44);
    /*p23.VARO*/ wire FF44_RDn = not(FF44_RD);

    /*p23.WURY*/ wire LY0n = not(lcd.Y0);
    /*p23.XEPO*/ wire LY1n = not(lcd.Y1);
    /*p23.MYFA*/ wire LY2n = not(lcd.Y2);
    /*p23.XUHY*/ wire LY3n = not(lcd.Y3);
    /*p23.WATA*/ wire LY4n = not(lcd.Y4);
    /*p23.XAGA*/ wire LY5n = not(lcd.Y5);
    /*p23.XUCE*/ wire LY6n = not(lcd.Y6);
    /*p23.XOWO*/ wire LY7n = not(lcd.Y7);

    /*p23.VEGA*/ if (FF44_RDn) bus_out.D0 = not(LY0n);
    /*p23.WUVA*/ if (FF44_RDn) bus_out.D1 = not(LY1n);
    /*p23.LYCO*/ if (FF44_RDn) bus_out.D2 = not(LY2n);
    /*p23.WOJY*/ if (FF44_RDn) bus_out.D3 = not(LY3n);
    /*p23.VYNE*/ if (FF44_RDn) bus_out.D4 = not(LY4n);
    /*p23.WAMA*/ if (FF44_RDn) bus_out.D5 = not(LY5n);
    /*p23.WAVO*/ if (FF44_RDn) bus_out.D6 = not(LY6n);
    /*p23.WEZE*/ if (FF44_RDn) bus_out.D7 = not(LY7n);
  }

  //----------
  // LY compare

  {
    /*p21.SYFU*/ wire LY_MATCH7 = xor(lcd.Y7, regs.LYC7);
    /*p21.TERY*/ wire LY_MATCH6 = xor(lcd.Y6, regs.LYC6);
    /*p21.TUCY*/ wire LY_MATCH5 = xor(lcd.Y5, regs.LYC5);
    /*p21.TYKU*/ wire LY_MATCH4 = xor(lcd.Y4, regs.LYC4);
    /*p21.RASY*/ wire LY_MATCH3 = xor(lcd.Y3, regs.LYC3);
    /*p21.REDA*/ wire LY_MATCH2 = xor(lcd.Y2, regs.LYC2);
    /*p21.TYDE*/ wire LY_MATCH1 = xor(lcd.Y1, regs.LYC1);
    /*p21.RYME*/ wire LY_MATCH0 = xor(lcd.Y0, regs.LYC0);

    /*p21.SOVU*/ wire LY_MATCHA = nor(LY_MATCH7, LY_MATCH6, LY_MATCH5, LY_MATCH4);
    /*p21.SUBO*/ wire LY_MATCHB = nor(LY_MATCH3, LY_MATCH2, LY_MATCH1, LY_MATCH0);

    /*p21.RAPE*/ wire LY_MATCHn = nand(LY_MATCHA, LY_MATCHB);
    /*p21.PALY*/ wire LY_MATCHa = not(LY_MATCHn);

    /*p21.ROPO*/ next.LYC_MATCH.tock(clk_sig2.TALU_xBCDExxx, rst_sig1.WESY_RESET, LY_MATCHa);
  }

  //----------
  // FF41 STAT stuff and interrupts

  {
    /*p21.SEPA*/ wire FF41_WR = and(ctl.CPU_WR2, dec.FF41);

    /*p21.RYVE*/ wire CLK_STAT= not(FF41_WR);
    /*p21.ROXE*/ next.INT_HBL_EN.tock(CLK_STAT, rst_sig1.WESY_RESET, bus.D3);
    /*p21.RUFO*/ next.INT_VBL_EN.tock(CLK_STAT, rst_sig1.WESY_RESET, bus.D4);
    /*p21.REFE*/ next.INT_OAM_EN.tock(CLK_STAT, rst_sig1.WESY_RESET, bus.D5);
    /*p21.RUGU*/ next.INT_LYC_EN.tock(CLK_STAT, rst_sig1.WESY_RESET, bus.D6);

    // 11: hblank   - rendering 0, vbl 0, oam 0
    // 10: vblank   - rendering 0, vbl 1, oam 0
    // 01: oam scan - rendering 0, vbl 0, oam 1
    // 00: render   - rendering 1, vbl 0, oam 0
    // so one of these has the wrong polarity

    /*p21.SADU*/ wire STAT_MODE0n = nor(vid.RENDERING_LATCH, lcd.VBLANK_d4b);
    /*p21.XATY*/ wire STAT_MODE1n = nor(vid.RENDERING_LATCH, spr.OAM_ADDR_PARSE);

    /*p21.RYJU*/ wire FF41_WRn = not(FF41_WR);

    /*p21.PAGO*/ wire STAT_LYC_MATCH1 = nor(rst_sig1.WESY_RESET, FF41_WRn);  // schematic wrong, this is NOR
    /*p21.RUPO*/ wire STAT_LYC_MATCH2 = or(vid.LYC_MATCH, STAT_LYC_MATCH1); // this is another of the weird or gates. could be nor?

    /*p21.TOBE*/ wire FF41_RDa = and(ctl.CPU_RD2, dec.FF41);
    /*p21.VAVE*/ wire FF41_RDb = FF41_RDa; // buffer, not inverter?
    /*p21.TEBY*/ if (FF41_RDa) bus_out.D0 = not(STAT_MODE0n);
    /*p21.WUGA*/ if (FF41_RDa) bus_out.D1 = not(STAT_MODE1n);
    /*p21.SEGO*/ if (FF41_RDa) bus_out.D2 = not(STAT_LYC_MATCH2);
    /*p21.PUZO*/ if (FF41_RDb) bus_out.D3 = vid.INT_HBL_EN;
    /*p21.POFO*/ if (FF41_RDb) bus_out.D4 = vid.INT_VBL_EN;
    /*p21.SASY*/ if (FF41_RDb) bus_out.D5 = vid.INT_OAM_EN;
    /*p21.POTE*/ if (FF41_RDb) bus_out.D6 = vid.INT_LYC_EN;

    /*p21.PURE*/ wire LINE_DONEa = not(lcd.NEW_LINE_d0a);
    /*p21.SELA*/ wire LINE_DONEo = not(LINE_DONEa);
    /*p21.TOLU*/ wire INT_VBLn = not(lcd.VBLANK_d4b);
    /*p21.TAPA*/ wire INT_OAM = and(INT_VBLn, LINE_DONEo);
    /*p21.TARU*/ wire INT_HBL = and(INT_VBLn, vid.RENDER_DONE);

    /*p21.SUKO*/ wire INT_STATb = amux4(vid.INT_LYC_EN, vid.LYC_MATCH,
                                        vid.INT_OAM_EN, INT_OAM,
                                        vid.INT_VBL_EN, lcd.VBLANK_d4b, // polarity?
                                        vid.INT_HBL_EN, INT_HBL);

    /*p21.TUVA*/ wire INT_STATn = not(INT_STATb);
    /*p21.VOTY*/ next.INT_STAT  = not(INT_STATn);
  }

  //----------
  // Window Y match


  {
    /*p27.NAZE*/ wire WY_MATCH0n = xor(lcd.Y0, regs.WY0);
    /*p27.PEBO*/ wire WY_MATCH1n = xor(lcd.Y1, regs.WY1);
    /*p27.POMO*/ wire WY_MATCH2n = xor(lcd.Y2, regs.WY2);
    /*p27.NEVU*/ wire WY_MATCH3n = xor(lcd.Y3, regs.WY3);
    /*p27.NOJO*/ wire WY_MATCH4n = xor(lcd.Y4, regs.WY4);
    /*p27.PAGA*/ wire WY_MATCH5n = xor(lcd.Y5, regs.WY5);
    /*p27.PEZO*/ wire WY_MATCH6n = xor(lcd.Y6, regs.WY6);
    /*p27.NUPA*/ wire WY_MATCH7n = xor(lcd.Y7, regs.WY7);

    // huh, the y matcher runs at 1 mhz but the x matcher runs at 4 mhz
    /*p27.PALO*/ wire WY_MATCH_HIn   = nand(regs.LCDC_WINEN, WY_MATCH4n, WY_MATCH5n, WY_MATCH6n, WY_MATCH7n);
    /*p27.NELE*/ wire WY_MATCH_HI    = not(WY_MATCH_HIn);
    /*p27.PAFU*/ wire WY_MATCHn      = nand(WY_MATCH_HI, WY_MATCH0n, WY_MATCH1n, WY_MATCH2n, WY_MATCH3n);
    /*p27.ROGE*/ wire WY_MATCH       = not(WY_MATCHn);
    /*p27.SARY*/ next.WY_MATCH_SYNC.tock(clk_sig2.TALU_xBCDExxx, rst_sig2.VID_RESETn, WY_MATCH);
  }


  //----------
  // Window X match

  {
    // polarity or gates wrong
    /*p27.REPU*/ wire IN_FRAME_Y  = nor(lcd.VBLANK_d4b, rst_sig2.VID_RESET4);   // schematic wrong, this is NOR
    /*p27.REJO*/ wire WIN_CHECK_X = or(vid.WY_MATCH_SYNC, IN_FRAME_Y); // another weird or gate. should be AND?

    /*p27.MYLO*/ wire WX_MATCH0n = xor(vid.X0, regs.WX0);
    /*p27.PUWU*/ wire WX_MATCH1n = xor(vid.X1, regs.WX1);
    /*p27.PUHO*/ wire WX_MATCH2n = xor(vid.X2, regs.WX2);
    /*p27.NYTU*/ wire WX_MATCH3n = xor(vid.X3, regs.WX3);
    /*p27.NEZO*/ wire WX_MATCH4n = xor(vid.X4, regs.WX4);
    /*p27.NORY*/ wire WX_MATCH5n = xor(vid.X5, regs.WX5);
    /*p27.NONO*/ wire WX_MATCH6n = xor(vid.X6, regs.WX6);
    /*p27.PASE*/ wire WX_MATCH7n = xor(vid.X7, regs.WX7);

    /*p27.PUKY*/ wire WX_MATCH_HIn = nand(WIN_CHECK_X, WX_MATCH4n, WX_MATCH5n, WX_MATCH6n, WX_MATCH7n);
    /*p27.NUFA*/ wire WX_MATCH_HI  = not (WX_MATCH_HIn);
    /*p27.NOGY*/ wire WIN_MATCHn   = nand(WX_MATCH_HI, WX_MATCH0n, WX_MATCH1n, WX_MATCH2n, WX_MATCH3n);
    /*p27.NUKO*/ next.WIN_MATCH    = not (WIN_MATCHn);
  }

  //----------
  // Map coord counters

  {

    /*p27.XOFO*/ wire X_RST = nand(regs.LCDC_WINEN, lcd.VID_LINE_TRIG_d4o, rst_sig2.VID_RESETn);
    /*p27.REPU*/ wire Y_RST  = nor(lcd.VBLANK_d4b, rst_sig2.VID_RESET4);   // schematic wrong, this is NOR

    /*p27.VETU*/ wire X_CLK = and(vid.MAP_X_CLK_STOPn, vid.WIN_MODE_PORE);
    /*p27.XACO*/ wire X_RSTn = not(X_RST);
    /*p27.WYKA*/ next.MAP_X0.tock(X_CLK,       X_RSTn, !vid.MAP_X0);
    /*p27.WODY*/ next.MAP_X1.tock(!vid.MAP_X0, X_RSTn, !vid.MAP_X1);
    /*p27.WOBO*/ next.MAP_X2.tock(!vid.MAP_X1, X_RSTn, !vid.MAP_X2);
    /*p27.WYKO*/ next.MAP_X3.tock(!vid.MAP_X2, X_RSTn, !vid.MAP_X3);
    /*p27.XOLO*/ next.MAP_X4.tock(!vid.MAP_X3, X_RSTn, !vid.MAP_X4);

    /*p27.WAZY*/ next.Y_CLK = not(vid.WIN_MODE_PORE);
    /*p27.SYNY*/ wire Y_RSTn = not(Y_RST);
    /*p27.VYNO*/ next.TILE_Y0.tock(vid.Y_CLK,    Y_RSTn, !vid.TILE_Y0);
    /*p27.VUJO*/ next.TILE_Y1.tock(!vid.TILE_Y0, Y_RSTn, !vid.TILE_Y1);
    /*p27.VYMU*/ next.TILE_Y2.tock(!vid.TILE_Y1, Y_RSTn, !vid.TILE_Y2);
    /*p27.TUFU*/ next.MAP_Y0.tock (!vid.TILE_Y2, Y_RSTn, !vid.MAP_Y0);
    /*p27.TAXA*/ next.MAP_Y1.tock (!vid.MAP_Y0,  Y_RSTn, !vid.MAP_Y1);
    /*p27.TOZO*/ next.MAP_Y2.tock (!vid.MAP_Y1,  Y_RSTn, !vid.MAP_Y2);
    /*p27.TATE*/ next.MAP_Y3.tock (!vid.MAP_Y2,  Y_RSTn, !vid.MAP_Y3);
    /*p27.TEKE*/ next.MAP_Y4.tock (!vid.MAP_Y3,  Y_RSTn, !vid.MAP_Y4);
  }

  //----------
  // Pixel counter. This is a little weird, presumably because it can tick at 4 mhz but not always?

  {
    /*p21.TADY*/ next.X_RST = nor(lcd.VID_LINE_TRIG_d4n, rst_sig2.VID_RESET3);

    /*p21.RYBO*/ wire RYBO = xor(vid.X0, vid.X1);
    /*p21.XUKE*/ wire XUKE = and(vid.X0, vid.X1);

    /*p21.XYLE*/ wire XYLE = and(vid.X2, XUKE);
    /*p21.XEGY*/ wire XEGY = xor(vid.X2, XUKE);
    /*p21.XORA*/ wire XORA = xor(vid.X3, XYLE);

    /*p21.SAKE*/ wire SAKE = xor(vid.X4, vid.X5);
    /*p21.TYBA*/ wire TYBA = and(vid.X4, vid.X5);
    /*p21.SURY*/ wire SURY = and(vid.X6, TYBA);
    /*p21.TYGE*/ wire TYGE = xor(vid.X6, TYBA);
    /*p21.ROKU*/ wire ROKU = xor(vid.X7, SURY);
    /*p21.TOCA*/ wire TOCA = not(vid.X3);

    /*p21.XEHO*/ next.X0.tock(vid.CLKPIPE, vid.X_RST, !vid.X0);
    /*p21.SAVY*/ next.X1.tock(vid.CLKPIPE, vid.X_RST, RYBO);
    /*p21.XODU*/ next.X2.tock(vid.CLKPIPE, vid.X_RST, XEGY);
    /*p21.XYDO*/ next.X3.tock(vid.CLKPIPE, vid.X_RST, XORA);
    /*p21.TUHU*/ next.X4.tock(TOCA,        vid.X_RST, !vid.X4);
    /*p21.TUKY*/ next.X5.tock(TOCA,        vid.X_RST, SAKE);
    /*p21.TAKO*/ next.X6.tock(TOCA,        vid.X_RST, TYGE);
    /*p21.SYBE*/ next.X7.tock(TOCA,        vid.X_RST, ROKU);

    /*p21.XUGU*/ next.X_167n = nand(vid.X0, vid.X1, vid.X2, vid.X5, vid.X7); // 128 + 32 + 4 + 2 + 1 = 167
    /*p21.XANO*/ next.X_167 = not(vid.X_167n);

    /*p21.ACAM*/ next.X0n = not(vid.X0);
    /*p21.AZUB*/ next.X1n = not(vid.X1);
    /*p21.AMEL*/ next.X2n = not(vid.X2);
    /*p21.AHAL*/ next.X3n = not(vid.X3);
    /*p21.APUX*/ next.X4n = not(vid.X4);
    /*p21.ABEF*/ next.X5n = not(vid.X5);
    /*p21.ADAZ*/ next.X6n = not(vid.X6);
    /*p21.ASAH*/ next.X7n = not(vid.X7);

  }
}

//-----------------------------------------------------------------------------

};
#include "Sch_Video.h"
#include "Sch_Gameboy.h"

#include "Sch_SpriteStore.h"

//#pragma warning(disable:4100)

namespace Schematics {

//-----------------------------------------------------------------------------

void VidRegisters::pwron() {}
void VidRegisters::reset() {}
void VidRegisters::phase_begin() {}
void VidRegisters::phase_end() {}
void VidRegisters::pass_begin() {}
bool VidRegisters::pass_end() { return false; }


VidSignals VidRegisters::tick(const ClkSignals& clk_sig,
                              const RstSignals& /*rst_sig*/,
                              const VclkSignals& /*vid_clk*/,
                              const JoypadPins& joy_pins,
                              const BusSignals& /*bus_sig*/,
                              const DecoderSignals& /*dec_sig*/,
                              const CartPins& /*cart_pins*/,
                              const LcdSignals& /*lcd_sig*/,
                              const SpriteStoreSignals& /*sst_sig*/,
                              BusTristates& /*bus_tri*/) {

  /*p27.VYPO*/ wire VYPO = not(joy_pins.P10_B);

  /*p01.LAPE*/ wire LAPE_xBxDxFxH = not(clk_sig.ALET_AxCxExGx);
  /*p27.MYVO*/ wire MYVO_xBxDxFxH = not(clk_sig.ALET_AxCxExGx);
  /*p27.MOXE*/ wire MOXE_xBxDxFxH = not(clk_sig.ALET_AxCxExGx);
  /*p27.MEHE*/ wire MEHE_xBxDxFxH = not(clk_sig.ALET_AxCxExGx);
  /*p27.TAVA*/ wire TAVA_AxCxExGx = not(LAPE_xBxDxFxH);

#if 0
  //----------

  {
    // We stop the pipe as soon as there's a store match or we switch to window mode
    {
      /*p27.PAHA*/ wire RENDERINGn = not(RENDERING_LATCH);

      // weird latch
      /*p27.ROXY*/ if (RENDERINGn)      FINE_MATCH_DUMP = 1; 
      /*p27.ROXY*/ if (FINE_MATCH_TRIG) FINE_MATCH_DUMP = 0;

      /*p24.SOCY*/ wire WIN_MODE2p = not(WIN_MODE2b);
      /*p24.VYBO*/ wire VYBO = nor(sst.STORE_MATCH, RENDER_DONE, MYVO_xBxDxFxH);
      /*p24.TYFA*/ wire TYFA = and(WIN_MODE2p, FRONT_PORCH_LATCHn, VYBO);

      /*p24.SEGU*/ wire SEGU_4M = not(TYFA);
      /*p24.ROXO*/ wire ROXO_4M = not(SEGU_4M);
      /*p27.ROCO*/ wire ROCO_4M = not(SEGU_4M);
      /*p24.SACU*/ wire CLKPIPE = nor(SEGU_4M, FINE_MATCH_DUMP);
    }

    // But we don't enter sprite mode until background sequencer hits BG_SEQ5
    {
      /*p27.SOWO*/ wire SPRITE_FETCH_LATCHn = not(SPRITE_FETCH_LATCH);
      /*p27.TUKU*/ wire WIN_MODE2o = not(WIN_MODE2b);
      /*p27.TEKY*/ wire SPRITE_FETCH = and(sst.STORE_MATCH, WIN_MODE2o, BG_SEQ_5, SPRITE_FETCH_LATCHn);
      /*p27.SOBU*/ SPRITE_FETCH_SYNC1.tock(TAVA_AxCxExGx, VYPO, SPRITE_FETCH);
      /*p27.SUDA*/ SPRITE_FETCH_SYNC2.tock(LAPE_xBxDxFxH, VYPO,  SPRITE_FETCH_SYNC1);
      /*p27.RYCE*/ SPRITE_FETCH_TRIG = and(SPRITE_FETCH_SYNC1, !SPRITE_FETCH_SYNC2);

      /*p27.SECA*/ wire SPRITE_FETCH_BEGINn = nor(SPRITE_FETCH_TRIG, vid_rst.VID_RESET5, lcd_sig.VID_LINE_TRIG_d4n);
      /*p27.TAKA*/ if (!SPRITE_FETCH_BEGINn) SPRITE_FETCH_LATCH = 1; // weird latch

      // I guess this blocks sprite fetches for the front porch?
      /*p27.SUVU*/   wire SUVU = nand(RENDERING_LATCH, FRONT_PORCH, BG_SEQ_6, BG_SEQ_7);
      /*p27.TAVE*/ TAVE = not(SUVU);

      /*p27.VEKU*/ wire SPRITE_FETCH_ENDn = nor(spr.SPRITE_DONE, TAVE);
      /*p27.TAKA*/ if (!SPRITE_FETCH_ENDn) SPRITE_FETCH_LATCH = 0; // weird latch

      /*p27.TUXY*/     wire TUXY = nand(WIN_MODE_SYNC2, WIN_MODE2n);
      /*p27.SUZU*/   wire SUZU = not(TUXY);
      /*p27.TEVO*/ MAP_X_CLK_STOPn = nor(WIN_TRIGGER, SUZU, TAVE);
    }
  }

  {
    /*p27.LAXE*/   wire BG_SEQ0n = not(BG_SEQ_x1x3x5x7);
    /*p27.MYSO*/ wire BG_SEQ_TRIG_1357 = nor(RENDERINGn, BG_SEQ0n, BG_SEQ_x1x3x5x7_DELAY);

    /*p27.NOFU*/       wire BG_SEQ_0123xxxx  = not(BG_SEQ_xxxx4567);
    /*p27.NYDY*/     wire BG_SEQ_TRIG_3n = nand(BG_SEQ_TRIG_1357, BG_SEQ_xx23xx67, BG_SEQ_0123xxxx);
    /*p32.METE*/   wire BG_SEQ_TRIG_3 = not(BG_SEQ_TRIG_3n);
    /*p32.LOMA*/ BG_LATCH = not(BG_SEQ_TRIG_3);

    /*p27.NAKO*/         wire BG_SEQ_01xx45xx  = not(BG_SEQ_xx23xx67);
    /*p27.MOFU*/       wire MOFU = and(BG_SEQ_TRIG_1357, BG_SEQ_01xx45xx);
    /*p32.LESO*/     wire LESO = not(MOFU);
    /*p32.AJAR*/   wire AJAR = not(LESO);
    /*p32.LABU*/ VRAM_TEMP_CLK = not(AJAR);
  }

  //----------
  // FF41 STAT

  
  //---

  {
    /*p21.WEGO*/ wire WEGO = or(vid_rst.VID_RESET3, RENDER_DONE_SYNC);

    // Weird latch? Weird latch!
    /*p21.XYMU*/ if (WEGO)               RENDERING_LATCH = 0;
    /*p21.XYMU*/ if (spr.SCAN_DONE_d0_TRIG) RENDERING_LATCH = 1;
    
    /*p24.LOBY*/ RENDERINGn = not(RENDERING_LATCH);
    
    /*p21.XENA*/ wire STORE_MATCHn = not(sst.STORE_MATCH);
    /*p21.WODU*/ RENDER_DONE = and(STORE_MATCHn, X_167);
    /*p21.VOGA*/ RENDER_DONE_SYNC.tock(clk_sig.ALET_AxCxExGx, X_RST, RENDER_DONE);
  }


  //---


  {
    /*p27.NENY*/ wire BG_SEQ_01xxxxxx = not(BG_SEQ_xx234567);

    /*p27.POTU*/ wire POTU = and(BG_READ_VRAM, BG_SEQ_01xxxxxx);
    /*p25.XEZE*/ WIN_MAP_READ  = nand(POTU, WIN_MODE_PORE);
    /*p26.ACEN*/ BG_MAP_READ   = and (POTU, WIN_MODE_AXADn);


    /*p27.NOFU*/ wire BG_SEQ_0123xxxx = not(BG_SEQ_xxxx4567);
    /*p27.XUHA*/ TILE_READ_AB  = not(BG_SEQ_0123xxxx);

    /*p27.NETA*/ TILE_READ     = and(BG_READ_VRAM, BG_SEQ_xx234567);

    /*p25.XUCY*/ WIN_TILE_READ = nand(TILE_READ, WIN_MODE_PORE);
    /*p26.ASUL*/ wire BG_TILE_READn = and (TILE_READ, WIN_MODE_AXADn);
    /*p26.BEJE*/ BG_TILE_READ  = not(BG_TILE_READn);

  }

  //----------

  //----------
  // Fine x matcher for x scroll

  {
    /*p27.SUHA*/ wire FINE_MATCH0n = xor(regs.SCX0, FINE_CNT0);
    /*p27.SYBY*/ wire FINE_MATCH1n = xor(regs.SCX1, FINE_CNT1);
    /*p27.SOZU*/ wire FINE_MATCH2n = xor(regs.SCX2, FINE_CNT2);
    /*p27.RONE*/ wire FINE_MATCHn  = nand(FINE_MATCH_DUMP, FINE_MATCH0n, FINE_MATCH1n, FINE_MATCH2n);
    /*p27.POHU*/ wire FINE_MATCH   = not(FINE_MATCHn);

    /*p27.PUXA*/ FINE_MATCH_SYNC1.tock(ROXO_4M,          RENDERING_LATCH, FINE_MATCH);
    /*p27.NYZE*/ FINE_MATCH_SYNC2.tock(MOXE_xBxDxFxH, RENDERING_LATCH, FINE_MATCH_SYNC1);
    /*p27.POVA*/ FINE_MATCH_TRIG = and(FINE_MATCH_SYNC1, !FINE_MATCH_SYNC2);
  }

  {
    /*p25.ROPY*/ wire ROPY_RENDERINGn = not(RENDERING_LATCH);
    /*p27.ROZE*/ wire FINE_COUNT_STOPn = nand(FINE_CNT0, FINE_CNT1, FINE_CNT2);
    /*p27.PASO*/ wire FINE_RST = nor(MAP_X_CLK_STOPn, ROPY_RENDERINGn);
    /*p27.PECU*/ wire FINE_CLK = nand(ROXO_4M, FINE_COUNT_STOPn);
    /*p27.RYKU*/ FINE_CNT0.tock( FINE_CLK,      FINE_RST, !FINE_CNT0);
    /*p27.ROGA*/ FINE_CNT1.tock(!FINE_CNT0, FINE_RST, !FINE_CNT1);
    /*p27.RUBU*/ FINE_CNT2.tock(!FINE_CNT1, FINE_RST, !FINE_CNT2);
  }

  {
    /*p27.ROZE*/ wire FINE_COUNT_STOPn = nand(FINE_CNT0, FINE_CNT1, FINE_CNT2);
    /*p27.PANY*/ wire WIN_MATCH_ONSCREEN = nor(WIN_MATCH, FINE_COUNT_STOPn);

    /*p27.RYFA*/ WIN_MATCH_ONSCREEN_SYNC1.tock(SEGU_4M,          RENDERING_LATCH, WIN_MATCH_ONSCREEN);
    /*p27.RENE*/ WIN_MATCH_ONSCREEN_SYNC2.tock(clk_sig.ALET_AxCxExGx, RENDERING_LATCH, WIN_MATCH_ONSCREEN_SYNC1);
    /*p27.SEKO*/ WIN_TRIGGER = nor(WIN_MATCH_ONSCREEN_SYNC2, !WIN_MATCH_ONSCREEN_SYNC1);

    /*p27.PYCO*/ WIN_MATCH_SYNC1.tock(ROCO_4M,            vid_rst.VID_RESETn, WIN_MATCH);
    /*p27.NUNU*/ WIN_MATCH_SYNC2.tock(MEHE_xBxDxFxH, vid_rst.VID_RESETn, WIN_MATCH_SYNC1);

    /*p27.XOFO*/ wire LINE_RST = nand(regs.LCDC_WINEN, lcd_sig.VID_LINE_TRIG_d4o, vid_rst.VID_RESETn);

    // weird latch
    /*p27.PYNU*/ if (LINE_RST)            WIN_MODE_LATCH = 0;
    /*p27.PYNU*/ if (WIN_MATCH_SYNC2) WIN_MODE_LATCH = 1;

    /*p27.NOCU*/ WIN_MODE_NOCUn = not(WIN_MODE_LATCH);
    /*p27.PORE*/ WIN_MODE_PORE  = not(WIN_MODE_NOCUn);
    /*p26.AXAD*/ WIN_MODE_AXADn = not(WIN_MODE_PORE);

    /*p27.NOPA*/ WIN_MODE_SYNC.tock(clk_sig.ALET_AxCxExGx, vid_rst.VID_RESETn, WIN_MODE_LATCH);
    /*p27.NUNY*/ WIN_MODE_TRIG = and(WIN_MODE_LATCH, !WIN_MODE_SYNC);

    // PUKU/RYDY form a NOR latch. WIN_MODE_TRIG is SET, (VID_RESET | BG_SEQ_7) is RESET.

    ///*p27.PUKU*/ PUKU = nor(RYDY, WIN_MODE_TRIG);
    ///*p27.RYDY*/ RYDY = nor(PUKU, rst_reg.VID_RESET4, BG_SEQ_7);

    if (WIN_MODE_TRIG) {
      PUKU = 0;
      WIN_MODE_LATCH2 = 1;
    }
    if (vid_rst.VID_RESET4 || BG_SEQ_7) {
      PUKU = 1;
      WIN_MODE_LATCH2 = 0;
    }

    /*p27.SYLO*/ WIN_MODE2n = not(WIN_MODE_LATCH2);
    /*p24.TOMU*/ WIN_MODE2b = not(WIN_MODE2n);

    /*p27.SOVY*/ WIN_MODE_SYNC2.tock(clk_sig.ALET_AxCxExGx, vid_rst.VID_RESETn, WIN_MODE_LATCH2);
  }

  //----------

  {
    /*p27.NYFO*/ wire WIN_MODE_TRIGn = not(WIN_MODE_TRIG);
    /*p27.MOSU*/ wire WIN_MODE_TRIGb = not(WIN_MODE_TRIGn);

    /*p27.NYXU*/ wire BG_SEQ_RSTn = nor(spr.SCAN_DONE_d0_TRIG, WIN_MODE_TRIGb, MAP_X_CLK_STOPn);

    // counts to 5? polarity?
    /*p27.MOCE*/ wire BG_SEQ_5n = nand(BG_SEQ_x1x3x5x7, BG_SEQ_xxxx4567, BG_SEQ_RSTn);
    /*p27.LYRY*/ BG_SEQ_5 = not(BG_SEQ_5n);

    /*p27.LEBO*/ wire BG_SEQ_CLK = nand(clk_sig.ALET_AxCxExGx, BG_SEQ_5n);
    /*p27.LAXU*/ BG_SEQ_x1x3x5x7.tock( BG_SEQ_CLK,          BG_SEQ_RSTn, !BG_SEQ_x1x3x5x7);
    /*p27.MESU*/ BG_SEQ_xx23xx67.tock(!BG_SEQ_x1x3x5x7, BG_SEQ_RSTn, !BG_SEQ_xx23xx67);
    /*p27.NYVA*/ BG_SEQ_xxxx4567.tock(!BG_SEQ_xx23xx67, BG_SEQ_RSTn, !BG_SEQ_xxxx4567);

    /*p27.NAKO*/ wire BG_SEQ_01xx45xx  = not(BG_SEQ_xx23xx67);
    /*p27.NOFU*/ wire BG_SEQ_0123xxxx  = not(BG_SEQ_xxxx4567);
    /*p27.NOGU*/ BG_SEQ_xx234567 = nand(BG_SEQ_01xx45xx, BG_SEQ_0123xxxx);



    /*p24.NAFY*/ wire RENDERING_AND_NOT_WIN_TRIG = nor(WIN_MODE_TRIGb, RENDERINGn);
    /*p24.NYKA*/ BG_SEQ_6.tock(clk_sig.ALET_AxCxExGx, RENDERING_AND_NOT_WIN_TRIG, BG_SEQ_5);
    /*p24.PORY*/ BG_SEQ_7.tock(MYVO_xBxDxFxH, RENDERING_AND_NOT_WIN_TRIG, BG_SEQ_6);

    /*p27.LYZU*/ BG_SEQ_x1x3x5x7_DELAY.tock(clk_sig.ALET_AxCxExGx, RENDERING_LATCH, BG_SEQ_x1x3x5x7);

    /*p27.LOVY*/ BG_SEQ5_SYNC.tock(MYVO_xBxDxFxH, BG_SEQ_RSTn, BG_SEQ_5);
    /*p27.LURY*/ wire LURY = and(!BG_SEQ5_SYNC, RENDERING_LATCH);
    
    // weird latch? polarity?
    /*p27.LONY*/ if (BG_SEQ_RSTn) BG_READ_VRAM_LATCH = 0;
    /*p27.LONY*/ if (LURY)        BG_READ_VRAM_LATCH = 1;

    /*p27.LUSU*/ wire BG_READ_VRAMn = not(BG_READ_VRAM_LATCH);
    /*p27.LENA*/ BG_READ_VRAM  = not(BG_READ_VRAMn);
  }

  //----------
  // The first tile generated is thrown away. I'm calling that section of rendering the front porch.

  {
    /*p24.PYGO*/ TILE_DONE.tock(clk_sig.ALET_AxCxExGx, RENDERING_LATCH, BG_SEQ_7);
    
    // weird latch
    /*p24.POKY*/ if (TILE_DONE)  FRONT_PORCH_LATCHn = 1; 
    /*p24.POKY*/ if (RENDERINGn) FRONT_PORCH_LATCHn = 0;
    
    /*p27.ROMO*/ FRONT_PORCH = not(FRONT_PORCH_LATCHn);
  }

  //----------
  // FF44 LY

  {
    /*p23.WAFU*/ wire FF44_RD = and(bus_sig.ASOT_CPURD, dec.FF44);
    /*p23.VARO*/ wire FF44_RDn = not(FF44_RD);

    /*p23.WURY*/ wire LY0n = not(lcd.Y0);
    /*p23.XEPO*/ wire LY1n = not(lcd.Y1);
    /*p23.MYFA*/ wire LY2n = not(lcd.Y2);
    /*p23.XUHY*/ wire LY3n = not(lcd.Y3);
    /*p23.WATA*/ wire LY4n = not(lcd.Y4);
    /*p23.XAGA*/ wire LY5n = not(lcd.Y5);
    /*p23.XUCE*/ wire LY6n = not(lcd.Y6);
    /*p23.XOWO*/ wire LY7n = not(lcd.Y7);

    if (!FF44_RDn) bus_out.set_data(
      /*p23.VEGA*/ not(LY0n),
      /*p23.WUVA*/ not(LY1n),
      /*p23.LYCO*/ not(LY2n),
      /*p23.WOJY*/ not(LY3n),
      /*p23.VYNE*/ not(LY4n),
      /*p23.WAMA*/ not(LY5n),
      /*p23.WAVO*/ not(LY6n),
      /*p23.WEZE*/ not(LY7n)
    );
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

    /*p21.ROPO*/ LYC_MATCH.tock(vid_clk.TALU_xBCDExxx, rst_sig1.WESY_RESET, LY_MATCHa);
  }

  //----------
  // FF41 STAT stuff and interrupts

  {
    /*p21.SEPA*/ wire FF41_WR = and(bus_sig.CUPA_CPUWR, dec.FF41);

    /*p21.RYVE*/ wire CLK_STAT= not(FF41_WR);
    /*p21.ROXE*/ INT_HBL_EN.tock(CLK_STAT, rst_sig1.WESY_RESET, bus.D0());
    /*p21.RUFO*/ INT_VBL_EN.tock(CLK_STAT, rst_sig1.WESY_RESET, bus.D1());
    /*p21.REFE*/ INT_OAM_EN.tock(CLK_STAT, rst_sig1.WESY_RESET, bus.D2());
    /*p21.RUGU*/ INT_LYC_EN.tock(CLK_STAT, rst_sig1.WESY_RESET, bus.D3());

    // 11: hblank   - rendering 0, vbl 0, oam 0
    // 10: vblank   - rendering 0, vbl 1, oam 0
    // 01: oam scan - rendering 0, vbl 0, oam 1
    // 00: render   - rendering 1, vbl 0, oam 0
    // so one of these has the wrong polarity

    /*p21.SADU*/ wire STAT_MODE0n = nor(RENDERING_LATCH, lcd_sig.VBLANK_d4b);
    /*p21.XATY*/ wire STAT_MODE1n = nor(RENDERING_LATCH, spr.OAM_ADDR_PARSE);

    /*p21.RYJU*/ wire FF41_WRn = not(FF41_WR);

    /*p21.PAGO*/ wire STAT_LYC_MATCH1 = nor(rst_sig1.WESY_RESET, FF41_WRn);  // schematic wrong, this is NOR
    /*p21.RUPO*/ wire STAT_LYC_MATCH2 = or(LYC_MATCH, STAT_LYC_MATCH1); // this is another of the weird or gates. could be nor?

    /*p21.TOBE*/ wire FF41_RDa = and(bus_sig.ASOT_CPURD, dec.FF41);
    /*p21.VAVE*/ wire FF41_RDb = FF41_RDa; // buffer, not inverter?

    if (FF41_RDa) bus_out.set_data(
      /*p21.TEBY*/ not(STAT_MODE0n),
      /*p21.WUGA*/ not(STAT_MODE1n),
      /*p21.SEGO*/ not(STAT_LYC_MATCH2),
      /*p21.PUZO*/ INT_HBL_EN,
      /*p21.POFO*/ INT_VBL_EN,
      /*p21.SASY*/ INT_OAM_EN,
      /*p21.POTE*/ INT_LYC_EN
    );

    /*p21.PURE*/ wire LINE_DONEa = not(lcd.NEW_LINE_d0a);
    /*p21.SELA*/ wire LINE_DONEo = not(LINE_DONEa);
    /*p21.TOLU*/ wire INT_VBLn = not(lcd_sig.VBLANK_d4b);
    /*p21.TAPA*/ wire INT_OAM = and(INT_VBLn, LINE_DONEo);
    /*p21.TARU*/ wire INT_HBL = and(INT_VBLn, RENDER_DONE);

    /*p21.SUKO*/ wire INT_STATb = amux4(INT_LYC_EN, LYC_MATCH,
                                        INT_OAM_EN, INT_OAM,
                                        INT_VBL_EN, lcd_sig.VBLANK_d4b, // polarity?
                                        INT_HBL_EN, INT_HBL);

    /*p21.TUVA*/ wire INT_STATn = not(INT_STATb);
    /*p21.VOTY*/ INT_STAT  = not(INT_STATn);
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
    /*p27.SARY*/ WY_MATCH_SYNC.tock(vid_clk.TALU_xBCDExxx, vid_rst.VID_RESETn, WY_MATCH);
  }


  //----------
  // Window X match

  {
    // polarity or gates wrong
    /*p27.REPU*/ wire IN_FRAME_Y  = nor(lcd_sig.VBLANK_d4b, vid_rst.VID_RESET4);   // schematic wrong, this is NOR
    /*p27.REJO*/ wire WIN_CHECK_X = or(WY_MATCH_SYNC, IN_FRAME_Y); // another weird or gate. should be AND?

    /*p27.MYLO*/ wire WX_MATCH0n = xor(X0, regs.WX0);
    /*p27.PUWU*/ wire WX_MATCH1n = xor(X1, regs.WX1);
    /*p27.PUHO*/ wire WX_MATCH2n = xor(X2, regs.WX2);
    /*p27.NYTU*/ wire WX_MATCH3n = xor(X3, regs.WX3);
    /*p27.NEZO*/ wire WX_MATCH4n = xor(X4, regs.WX4);
    /*p27.NORY*/ wire WX_MATCH5n = xor(X5, regs.WX5);
    /*p27.NONO*/ wire WX_MATCH6n = xor(X6, regs.WX6);
    /*p27.PASE*/ wire WX_MATCH7n = xor(X7, regs.WX7);

    /*p27.PUKY*/ wire WX_MATCH_HIn = nand(WIN_CHECK_X, WX_MATCH4n, WX_MATCH5n, WX_MATCH6n, WX_MATCH7n);
    /*p27.NUFA*/ wire WX_MATCH_HI  = not (WX_MATCH_HIn);
    /*p27.NOGY*/ wire WIN_MATCHn   = nand(WX_MATCH_HI, WX_MATCH0n, WX_MATCH1n, WX_MATCH2n, WX_MATCH3n);
    /*p27.NUKO*/ WIN_MATCH    = not (WIN_MATCHn);
  }

  //----------
  // Map coord counters

  {

    /*p27.XOFO*/ wire X_RST = nand(regs.LCDC_WINEN, lcd_sig.VID_LINE_TRIG_d4o, vid_rst.VID_RESETn);
    /*p27.REPU*/ wire Y_RST  = nor(lcd_sig.VBLANK_d4b, vid_rst.VID_RESET4);   // schematic wrong, this is NOR

    /*p27.VETU*/ wire X_CLK = and(MAP_X_CLK_STOPn, WIN_MODE_PORE);
    /*p27.XACO*/ wire X_RSTn = not(X_RST);
    /*p27.WYKA*/ MAP_X0.tock(X_CLK,       X_RSTn, !MAP_X0);
    /*p27.WODY*/ MAP_X1.tock(!MAP_X0, X_RSTn, !MAP_X1);
    /*p27.WOBO*/ MAP_X2.tock(!MAP_X1, X_RSTn, !MAP_X2);
    /*p27.WYKO*/ MAP_X3.tock(!MAP_X2, X_RSTn, !MAP_X3);
    /*p27.XOLO*/ MAP_X4.tock(!MAP_X3, X_RSTn, !MAP_X4);

    /*p27.WAZY*/ Y_CLK = not(WIN_MODE_PORE);
    /*p27.SYNY*/ wire Y_RSTn = not(Y_RST);
    /*p27.VYNO*/ TILE_Y0.tock(Y_CLK,    Y_RSTn, !TILE_Y0);
    /*p27.VUJO*/ TILE_Y1.tock(!TILE_Y0, Y_RSTn, !TILE_Y1);
    /*p27.VYMU*/ TILE_Y2.tock(!TILE_Y1, Y_RSTn, !TILE_Y2);
    /*p27.TUFU*/ MAP_Y0.tock (!TILE_Y2, Y_RSTn, !MAP_Y0);
    /*p27.TAXA*/ MAP_Y1.tock (!MAP_Y0,  Y_RSTn, !MAP_Y1);
    /*p27.TOZO*/ MAP_Y2.tock (!MAP_Y1,  Y_RSTn, !MAP_Y2);
    /*p27.TATE*/ MAP_Y3.tock (!MAP_Y2,  Y_RSTn, !MAP_Y3);
    /*p27.TEKE*/ MAP_Y4.tock (!MAP_Y3,  Y_RSTn, !MAP_Y4);
  }

  //----------
  // Pixel counter. This is a little weird, presumably because it can tick at 4 mhz but not always?

  {
    /*p21.TADY*/ X_RST = nor(lcd_sig.VID_LINE_TRIG_d4n, vid_rst.VID_RESET3);

    /*p21.RYBO*/ wire RYBO = xor(X0, X1);
    /*p21.XUKE*/ wire XUKE = and(X0, X1);

    /*p21.XYLE*/ wire XYLE = and(X2, XUKE);
    /*p21.XEGY*/ wire XEGY = xor(X2, XUKE);
    /*p21.XORA*/ wire XORA = xor(X3, XYLE);

    /*p21.SAKE*/ wire SAKE = xor(X4, X5);
    /*p21.TYBA*/ wire TYBA = and(X4, X5);
    /*p21.SURY*/ wire SURY = and(X6, TYBA);
    /*p21.TYGE*/ wire TYGE = xor(X6, TYBA);
    /*p21.ROKU*/ wire ROKU = xor(X7, SURY);
    /*p21.TOCA*/ wire TOCA = not(X3);

    /*p21.XEHO*/ X0.tock(CLKPIPE, X_RST, !X0);
    /*p21.SAVY*/ X1.tock(CLKPIPE, X_RST, RYBO);
    /*p21.XODU*/ X2.tock(CLKPIPE, X_RST, XEGY);
    /*p21.XYDO*/ X3.tock(CLKPIPE, X_RST, XORA);
    /*p21.TUHU*/ X4.tock(TOCA,        X_RST, !X4);
    /*p21.TUKY*/ X5.tock(TOCA,        X_RST, SAKE);
    /*p21.TAKO*/ X6.tock(TOCA,        X_RST, TYGE);
    /*p21.SYBE*/ X7.tock(TOCA,        X_RST, ROKU);

    /*p21.XUGU*/ X_167n = nand(X0, X1, X2, X5, X7); // 128 + 32 + 4 + 2 + 1 = 167
    /*p21.XANO*/ X_167 = not(X_167n);

    /*p21.ACAM*/ X0n = not(X0);
    /*p21.AZUB*/ X1n = not(X1);
    /*p21.AMEL*/ X2n = not(X2);
    /*p21.AHAL*/ X3n = not(X3);
    /*p21.APUX*/ X4n = not(X4);
    /*p21.ABEF*/ X5n = not(X5);
    /*p21.ADAZ*/ X6n = not(X6);
    /*p21.ASAH*/ X7n = not(X7);
  }

#endif

  return {
  };
}

//-----------------------------------------------------------------------------

};
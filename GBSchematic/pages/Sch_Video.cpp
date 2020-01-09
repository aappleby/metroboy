#include "Sch_Video.h"

#include "Sch_Clocks.h"
#include "Sch_System.h"
#include "Sch_SpriteStore.h"
#include "Sch_LCD.h"
#include "Sch_Registers.h"
#include "Sch_Resets.h"
#include "Sch_BusControl.h"
#include "Sch_Decoder.h"
#include "Sch_Sprites.h"

//#pragma warning(disable:4100)

namespace Schematics {

//-----------------------------------------------------------------------------

void VidRegisters::pwron() {}
void VidRegisters::reset() {}
void VidRegisters::phase_begin() {}
void VidRegisters::phase_end() {}
void VidRegisters::pass_begin() {}
bool VidRegisters::pass_end() { return false; }


VidSignals VidRegisters::tick(const SysSignals& sys_sig,
                              const ClkSignals& clk_sig,
                              const RstSignals& rst_sig,
                              const VclkSignals& /*vid_clk*/,
                              const BusSignals& bus_sig,
                              const DecoderSignals& dec_sig,
                              const LcdSignals& lcd_sig,
                              const LcdRegisters& lcd_reg,
                              const SpriteSignals& spr_sig,
                              const SpriteStoreSignals& /*sst_sig*/,
                              BusTristates& bus_tri,
                              bool STORE_MATCH,
                              bool AVAP_SCAN_DONE_d0_TRIG,
                              bool WUTY_SPRITE_DONE) {

  /*p27.VYPO*/ wire VYPO = not(sys_sig.PIN_P10_B);

  /*p01.LAPE*/ wire LAPE_xBxDxFxH = not(clk_sig.ALET_AxCxExGx);
  /*p27.MYVO*/ wire MYVO_xBxDxFxH = not(clk_sig.ALET_AxCxExGx);
  /*p27.MOXE*/ wire MOXE_xBxDxFxH = not(clk_sig.ALET_AxCxExGx);
  /*p27.MEHE*/ wire MEHE_xBxDxFxH = not(clk_sig.ALET_AxCxExGx);
  /*p27.TAVA*/ wire TAVA_AxCxExGx = not(LAPE_xBxDxFxH);

  /*p24.LOBY*/ wire LOBY_RENDERINGn = not(XYMU_RENDERING_LATCH);
  /*p21.XUGU*/ wire X_167n = nand(X0, X1, X2, X5, X7); // 128 + 32 + 4 + 2 + 1 = 167
  /*p21.XANO*/ wire X_167 = not(X_167n);
  /*p21.XENA*/ wire STORE_MATCHn = not(STORE_MATCH);
  /*p21.WODU*/ wire WODU_RENDER_DONE = and(STORE_MATCHn, X_167);

  /*p01.CUNU*/ bool CUNU_RESETn = not(rst_sig.SYS_RESET);
  /*p01.XORE*/ bool XORE_RESET  = not(CUNU_RESETn);
  /*p01.WESY*/ bool WESY_RESET  = not(XORE_RESET);
  
  //----------

  bool TAVE;
  bool TEVO_CLK_STOPn;

  {
    /*p27.SEKO*/ wire SEKO_WIN_TRIGGER = nor(WIN_MATCH_ONSCREEN_SYNC2, !WIN_MATCH_ONSCREEN_SYNC1);
    /*p27.SYLO*/ wire SYLO_WIN_MODEn = not(RYDY_WIN_MODE_LATCH);
    /*p27.TUXY*/ wire TUXY = nand(SOVY_WIN_MODE_SYNC, SYLO_WIN_MODEn);
    /*p27.SUZU*/ wire SUZU = not(TUXY);
    /*p27.ROMO*/ wire FRONT_PORCH = not(POKY_FRONT_PORCH_LATCHn);
    /*p27.SUVU*/ wire SUVU = nand(XYMU_RENDERING_LATCH, FRONT_PORCH, BG_SEQ_6, BG_SEQ_7);
    /*p27.TAVE*/ TAVE = not(SUVU);
    /*p27.TEVO*/ TEVO_CLK_STOPn = nor(SEKO_WIN_TRIGGER, SUZU, TAVE);
  }


  {
    /*p27.SYLO*/ wire SYLO_WIN_MODEn = not(RYDY_WIN_MODE_LATCH);
    /*p24.TOMU*/ wire TOMU_WIN_MODE  = not(SYLO_WIN_MODEn);
    /*p27.TUKU*/ wire TUKU_WIN_MODEn = not(TOMU_WIN_MODE);
    /*p27.SOWO*/ wire SOWO_SPRITE_FETCH_LATCHn = not(_SPRITE_FETCH_LATCH);

    /*p27.NUNY*/ wire NUNY_WIN_MODE_TRIG  = and(WIN_MODE_LATCH1, !NOPA_WIN_MODE_SYNC);
    /*p27.NYFO*/ wire NYFO_WIN_MODE_TRIGn = not(NUNY_WIN_MODE_TRIG);
    /*p27.MOSU*/ wire MOSU_WIN_MODE_TRIG  = not(NYFO_WIN_MODE_TRIGn);

    /*p27.NYXU*/ wire NYXU_BG_SEQ_RSTn = nor(AVAP_SCAN_DONE_d0_TRIG, MOSU_WIN_MODE_TRIG, TEVO_CLK_STOPn);
    /*p27.MOCE*/ wire MOCE_BG_SEQ_5n = nand(BG_SEQ_x1x3x5x7, BG_SEQ_xxxx4567, NYXU_BG_SEQ_RSTn);
    /*p27.LYRY*/ wire LYRY_BG_SEQ_5 = not(MOCE_BG_SEQ_5n);
    /*p27.TEKY*/ wire SPRITE_FETCH = and(STORE_MATCH, TUKU_WIN_MODEn, LYRY_BG_SEQ_5, SOWO_SPRITE_FETCH_LATCHn);
    /*p27.SOBU*/ _SPRITE_FETCH_SYNC1.set(TAVA_AxCxExGx, sys_sig.P10_Bn, SPRITE_FETCH);
    /*p27.SUDA*/ _SPRITE_FETCH_SYNC2.set(LAPE_xBxDxFxH, sys_sig.P10_Bn, _SPRITE_FETCH_SYNC1);
    /*p27.RYCE*/ wire RYCE_SPRITE_FETCH_TRIG = and(_SPRITE_FETCH_SYNC1, !_SPRITE_FETCH_SYNC2);
    /*p01.ROSY*/ wire ROSY_VID_RESET = not(rst_sig.VID_RESETn);
    /*p27.SECA*/ wire SPRITE_FETCH_BEGINn = nor(RYCE_SPRITE_FETCH_TRIG, ROSY_VID_RESET, lcd_sig.BYHA_VID_LINE_TRIG_d4n);
    /*p27.VEKU*/ wire SPRITE_FETCH_ENDn = nor(WUTY_SPRITE_DONE, TAVE);
    /*p27.TAKA*/ _SPRITE_FETCH_LATCH.latch(!SPRITE_FETCH_BEGINn, !SPRITE_FETCH_ENDn); // polarity?
  }


  {
    /*p01.TOFU*/ wire VID_RESET3  = not(rst_sig.VID_RESETn);
    /*p21.WEGO*/ wire WEGO = or(VID_RESET3, RENDER_DONE_SYNC);
    /*p21.XYMU*/ XYMU_RENDERING_LATCH.latch(AVAP_SCAN_DONE_d0_TRIG, WEGO);

    /*p21.TADY*/ wire TADY_X_RST = nor(lcd_sig.BYHA_VID_LINE_TRIG_d4n, VID_RESET3);
    /*p21.VOGA*/ RENDER_DONE_SYNC.set(clk_sig.ALET_AxCxExGx, TADY_X_RST, WODU_RENDER_DONE);
  }


#if 0
  //---


  {
    /*p27.NENY*/ wire BG_SEQ_01xxxxxx = not(BG_SEQ_xx234567);

    /*p27.LUSU*/ wire BG_READ_VRAMn = not(LONY_LATCH);
    /*p27.LENA*/ wire BG_READ_VRAM  = not(BG_READ_VRAMn);
    /*p27.POTU*/ wire POTU          = and(BG_READ_VRAM, BG_SEQ_01xxxxxx);
    /*p25.XEZE*/ wire XEZE_WIN_MAP_READ  = nand(POTU, WIN_MODE_PORE);
    /*p26.ACEN*/ sig.ACEN_BG_MAP_READ   = and (POTU, WIN_MODE_AXADn);


    /*p27.NOFU*/ wire BG_SEQ_0123xxxx = not(BG_SEQ_xxxx4567);
    /*p27.XUHA*/ wire XUHA_TILE_READ_AB = not(BG_SEQ_0123xxxx);

    /*p27.NETA*/ wire NETA_TILE_READ     = and(BG_READ_VRAM, BG_SEQ_xx234567);

    /*p25.XUCY*/ wire XUCY_WIN_TILE_READ = nand(TILE_READ, WIN_MODE_PORE);
    /*p26.ASUL*/ wire BG_TILE_READn = and (TILE_READ, WIN_MODE_AXADn);
    /*p26.BEJE*/ sig.BEJE_BG_TILE_READ  = not(BG_TILE_READn);

  }

  //----------
#endif

  //----------

  {
    /*p27.NUNY*/ wire NUNY_WIN_MODE_TRIG  = and(WIN_MODE_LATCH1, !NOPA_WIN_MODE_SYNC);
    /*p27.NYFO*/ wire NYFO_WIN_MODE_TRIGn = not(NUNY_WIN_MODE_TRIG);
    /*p27.MOSU*/ wire MOSU_WIN_MODE_TRIG  = not(NYFO_WIN_MODE_TRIGn);

    /*p27.NYXU*/ wire NYXU_BG_SEQ_RSTn = nor(AVAP_SCAN_DONE_d0_TRIG, MOSU_WIN_MODE_TRIG, TEVO_CLK_STOPn);

    // counts to 5? polarity?
    /*p27.MOCE*/ wire BG_SEQ_5n = nand(BG_SEQ_x1x3x5x7, BG_SEQ_xxxx4567, NYXU_BG_SEQ_RSTn);
    /*p27.LYRY*/ wire BG_SEQ_5 = not(BG_SEQ_5n);

    /*p27.LEBO*/ wire BG_SEQ_CLK = nand(clk_sig.ALET_AxCxExGx, BG_SEQ_5n);
    /*p27.NYXU*/ wire BG_SEQ_RSTn = nor(AVAP_SCAN_DONE_d0_TRIG, MOSU_WIN_MODE_TRIG, TEVO_CLK_STOPn);
    /*p27.LAXU*/ BG_SEQ_x1x3x5x7.set( BG_SEQ_CLK,      BG_SEQ_RSTn, !BG_SEQ_x1x3x5x7);
    /*p27.MESU*/ BG_SEQ_xx23xx67.set(!BG_SEQ_x1x3x5x7, BG_SEQ_RSTn, !BG_SEQ_xx23xx67);
    /*p27.NYVA*/ BG_SEQ_xxxx4567.set(!BG_SEQ_xx23xx67, BG_SEQ_RSTn, !BG_SEQ_xxxx4567);

    /*p27.NAKO*/ wire BG_SEQ_01xx45xx = not(BG_SEQ_xx23xx67);
    /*p27.NOFU*/ wire BG_SEQ_0123xxxx = not(BG_SEQ_xxxx4567);
    /*p27.NOGU*/ wire BG_SEQ_xx234567 = nand(BG_SEQ_01xx45xx, BG_SEQ_0123xxxx);

    /*p24.NAFY*/ wire RENDERING_AND_NOT_WIN_TRIG = nor(MOSU_WIN_MODE_TRIG, LOBY_RENDERINGn);
    /*p24.NYKA*/ BG_SEQ_6.set(clk_sig.ALET_AxCxExGx, RENDERING_AND_NOT_WIN_TRIG, BG_SEQ_5);
    /*p24.PORY*/ BG_SEQ_7.set(MYVO_xBxDxFxH, RENDERING_AND_NOT_WIN_TRIG, BG_SEQ_6);

    /*p27.LYZU*/ BG_SEQ_x1x3x5x7_DELAY.set(clk_sig.ALET_AxCxExGx, XYMU_RENDERING_LATCH, BG_SEQ_x1x3x5x7);

    /*p27.LOVY*/ BG_SEQ5_SYNC.set(MYVO_xBxDxFxH, BG_SEQ_RSTn, BG_SEQ_5);
    /*p27.LURY*/ wire LURY = and(!BG_SEQ5_SYNC, XYMU_RENDERING_LATCH);
    
    /*p27.LONY*/ LONY_LATCH.latch(LURY, NYXU_BG_SEQ_RSTn); // polarity?
    
    // The first tile generated is thrown away. I'm calling that section of rendering the front porch.
    /*p24.PYGO*/ PYGO_TILE_DONE.set(clk_sig.ALET_AxCxExGx, XYMU_RENDERING_LATCH, BG_SEQ_7);
    /*p24.POKY*/ POKY_FRONT_PORCH_LATCHn.latch(PYGO_TILE_DONE, LOBY_RENDERINGn);

    /*p27.LAXE*/ wire BG_SEQ0n = not(BG_SEQ_x1x3x5x7);
    /*p27.MYSO*/ wire BG_SEQ_TRIG_1357 = nor(LOBY_RENDERINGn, BG_SEQ0n, BG_SEQ_x1x3x5x7_DELAY);

    /*p27.NYDY*/ wire BG_SEQ_TRIG_3n = nand(BG_SEQ_TRIG_1357, BG_SEQ_xx23xx67, BG_SEQ_0123xxxx);
    /*p32.METE*/ wire BG_SEQ_TRIG_3 = not(BG_SEQ_TRIG_3n);
    /*p32.LOMA*/ wire BG_LATCH = not(BG_SEQ_TRIG_3);

    /*p27.MOFU*/ wire MOFU = and(BG_SEQ_TRIG_1357, BG_SEQ_01xx45xx);
    /*p32.LESO*/ wire LESO = not(MOFU);
    /*p32.AJAR*/ wire AJAR = not(LESO);
    /*p32.LABU*/ wire VRAM_TEMP_CLK = not(AJAR);
  }

  //----------
  // FF44 LY

  {
    /*p23.WAFU*/ wire FF44_RD = and(bus_sig.ASOT_CPURD, dec_sig.FF44);
    /*p23.VARO*/ wire FF44_RDn = not(FF44_RD);

    /*p23.WURY*/ wire LY0n = not(lcd_reg.Y0);
    /*p23.XEPO*/ wire LY1n = not(lcd_reg.Y1);
    /*p23.MYFA*/ wire LY2n = not(lcd_reg.Y2);
    /*p23.XUHY*/ wire LY3n = not(lcd_reg.Y3);
    /*p23.WATA*/ wire LY4n = not(lcd_reg.Y4);
    /*p23.XAGA*/ wire LY5n = not(lcd_reg.Y5);
    /*p23.XUCE*/ wire LY6n = not(lcd_reg.Y6);
    /*p23.XOWO*/ wire LY7n = not(lcd_reg.Y7);

    if (!FF44_RDn) bus_tri.set_data(
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
  // FF41 STAT stuff and interrupts

  {
    /*p21.SEPA*/ wire FF41_WR = and(bus_sig.CUPA_CPUWR, dec_sig.FF41);
    /*p21.RYVE*/ wire RYVE_FF41_WRn = not(FF41_WR);

    /*p21.ROXE*/ INT_HBL_EN.set(RYVE_FF41_WRn, WESY_RESET, bus_tri.D0());
    /*p21.RUFO*/ INT_VBL_EN.set(RYVE_FF41_WRn, WESY_RESET, bus_tri.D1());
    /*p21.REFE*/ INT_OAM_EN.set(RYVE_FF41_WRn, WESY_RESET, bus_tri.D2());
    /*p21.RUGU*/ INT_LYC_EN.set(RYVE_FF41_WRn, WESY_RESET, bus_tri.D3());

    // 11: hblank   - rendering 0, vbl 0, oam 0
    // 10: vblank   - rendering 0, vbl 1, oam 0
    // 01: oam scan - rendering 0, vbl 0, oam 1
    // 00: render   - rendering 1, vbl 0, oam 0
    // so one of these has the wrong polarity

    /*p21.SADU*/ wire STAT_MODE0n = nor(XYMU_RENDERING_LATCH, lcd_sig.PARU_VBLANK_d4);
    /*p21.XATY*/ wire STAT_MODE1n = nor(XYMU_RENDERING_LATCH, spr_sig.ACYL_OAM_ADDR_PARSE);

    /*p21.TOBE*/ wire FF41_RDa = and(bus_sig.ASOT_CPURD, dec_sig.FF41);
    /*p21.VAVE*/ wire FF41_RDb = FF41_RDa; // buffer, not inverter?

    if (FF41_RDa) bus_tri.set_data(
      /*p21.TEBY*/ not(STAT_MODE0n),
      /*p21.WUGA*/ not(STAT_MODE1n),
      /*p21.SEGO*/ not(RUPO_LATCH_LYC_MATCH),
      /*p21.PUZO*/ INT_HBL_EN,
      /*p21.POFO*/ INT_VBL_EN,
      /*p21.SASY*/ INT_OAM_EN,
      /*p21.POTE*/ INT_LYC_EN
    );

    /*p21.PURE*/ wire LINE_DONEa = not(lcd_reg.NEW_LINE_d0a);
    /*p21.SELA*/ wire LINE_DONEo = not(LINE_DONEa);
    /*p21.TOLU*/ wire INT_VBLn = not(lcd_sig.PARU_VBLANK_d4);
    /*p21.TAPA*/ wire INT_OAM = and(INT_VBLn, LINE_DONEo);
    /*p21.TARU*/ wire INT_HBL = and(INT_VBLn, WODU_RENDER_DONE);

    /*p21.SUKO*/ wire INT_STATb = amux4(INT_LYC_EN, ROPO_LYC_MATCH,
                                        INT_OAM_EN, INT_OAM,
                                        INT_VBL_EN, lcd_sig.PARU_VBLANK_d4, // polarity?
                                        INT_HBL_EN, INT_HBL);

    /*p21.TUVA*/ wire INT_STATn = not(INT_STATb);
    /*p21.VOTY*/ wire INT_STAT  = not(INT_STATn);
  }

#if 0
  return {
    /*p21.VOTY*/ .INT_STAT = INT_STAT,
    /*p26.ACEN*/ .ACEN_BG_MAP_READ = ,
    /*p25.XEZE*/ .XEZE_WIN_MAP_READ = ,
    /*p26.BEJE*/ .BEJE_BG_TILE_READ = ,
    /*p27.XUHA*/ .XUHA_TILE_READ_AB = ,
    /*p25.XUCY*/ .XUCY_WIN_TILE_READ = ,
    /*p27.NETA*/ .NETA_TILE_READ = ,
    /*p27.RYCE*/ .RYCE_SPRITE_FETCH_TRIG = ,
  };
#endif

  return {};
}

//-----------------------------------------------------------------------------
// Fine x matcher for x scroll
// We stop the pipe as soon as there's a store match or we switch to window mode

void VidRegisters::fineMatch(const ClkSignals& clk_sig,
                             const VidConfig& vid_reg2,
                             bool TEVO_CLK_STOPn,
                             bool STORE_MATCH) {

  /*p21.XUGU*/ wire XUGU_X_167n = nand(X0, X1, X2, X5, X7); // 128 + 32 + 4 + 2 + 1 = 167
  /*p21.XANO*/ wire XANO_X_167 = not(XUGU_X_167n);
  /*p21.XENA*/ wire XENA_STORE_MATCHn = not(STORE_MATCH);
  /*p21.WODU*/ wire WODU_RENDER_DONE = and(XENA_STORE_MATCHn, XANO_X_167);

  /*p27.MYVO*/ wire MYVO_xBxDxFxH = not(clk_sig.ALET_AxCxExGx);
  /*p27.MOXE*/ wire MOXE_xBxDxFxH = not(clk_sig.ALET_AxCxExGx);

  /*p24.VYBO*/ wire VYBO_AxCxExGx = nor(STORE_MATCH, WODU_RENDER_DONE, MYVO_xBxDxFxH);

  /*p27.SYLO*/ wire SYLO_WIN_MODEn = not(RYDY_WIN_MODE_LATCH);
  /*p24.TOMU*/ wire TOMU_WIN_MODE  = not(SYLO_WIN_MODEn);
  /*p24.SOCY*/ wire SOCY_WIN_MODEn = not(TOMU_WIN_MODE);
  /*p24.TYFA*/ wire TYFA_AxCxExGx  = and(SOCY_WIN_MODEn, POKY_FRONT_PORCH_LATCHn, VYBO_AxCxExGx);

  /*p24.SEGU*/ wire SEGU_xBxDxFxH = not(TYFA_AxCxExGx);
  /*p24.ROXO*/ wire ROXO_AxCxExGx = not(SEGU_xBxDxFxH);
  /*p27.ROCO*/ wire ROCO_AxCxExGx = not(SEGU_xBxDxFxH);
  /*p24.SACU*/ wire CLKPIPE_AxCxExGx = nor(SEGU_xBxDxFxH, FINE_MATCH_DUMP);

  /*p27.SUHA*/ wire FINE_MATCH0n = xor(vid_reg2.SCX0, FINE_CNT0);
  /*p27.SYBY*/ wire FINE_MATCH1n = xor(vid_reg2.SCX1, FINE_CNT1);
  /*p27.SOZU*/ wire FINE_MATCH2n = xor(vid_reg2.SCX2, FINE_CNT2);
  /*p27.RONE*/ wire FINE_MATCHn  = nand(FINE_MATCH_DUMP, FINE_MATCH0n, FINE_MATCH1n, FINE_MATCH2n);
  /*p27.POHU*/ wire FINE_MATCH   = not(FINE_MATCHn);

  /*p27.PUXA*/ FINE_MATCH_SYNC1.set(ROXO_AxCxExGx, XYMU_RENDERING_LATCH, FINE_MATCH);
  /*p27.NYZE*/ FINE_MATCH_SYNC2.set(MOXE_xBxDxFxH, XYMU_RENDERING_LATCH, FINE_MATCH_SYNC1);

  /*p27.PAHA*/ wire RENDERINGn = not(XYMU_RENDERING_LATCH);
  /*p27.POVA*/ wire FINE_MATCH_TRIG = and(FINE_MATCH_SYNC1, !FINE_MATCH_SYNC2);
  /*p27.ROXY*/ FINE_MATCH_DUMP.latch(RENDERINGn, FINE_MATCH_TRIG);

  /*p25.ROPY*/ wire ROPY_RENDERINGn = not(XYMU_RENDERING_LATCH);
  /*p27.ROZE*/ wire FINE_COUNT_STOPn = nand(FINE_CNT0, FINE_CNT1, FINE_CNT2);
  /*p27.PASO*/ wire FINE_RST = nor(TEVO_CLK_STOPn, ROPY_RENDERINGn);
  /*p27.PECU*/ wire FINE_CLK = nand(ROXO_AxCxExGx, FINE_COUNT_STOPn);
  /*p27.RYKU*/ FINE_CNT0.set( FINE_CLK,  FINE_RST, !FINE_CNT0);
  /*p27.ROGA*/ FINE_CNT1.set(!FINE_CNT0, FINE_RST, !FINE_CNT1);
  /*p27.RUBU*/ FINE_CNT2.set(!FINE_CNT1, FINE_RST, !FINE_CNT2);
}

//-----------------------------------------------------------------------------
// Map coord counters for window mode

void VidRegisters::mapCounter(const RstSignals& rst_sig,
                              const LcdSignals& lcd_sig,
                              const VidConfig& vid_reg2,
                              bool TEVO_CLK_STOPn) {
  /*p01.PYRY*/ wire PYRY_VID_RST = not(rst_sig.VID_RESETn);
  /*p27.XOFO*/ wire XOFO_WIN_RST = nand(vid_reg2.LCDC_WINEN, lcd_sig.XAHY_VID_LINE_TRIG_d4n, rst_sig.VID_RESETn);

  /*p27.NOCU*/ wire NOCU_WIN_MODEn = not(WIN_MODE_LATCH1);
  /*p27.PORE*/ wire PORE_WIN_MODE  = not(NOCU_WIN_MODEn);

  // something weird here, this doesn't look like a clock
  /*p27.VETU*/ wire VETU_X_CLK = and(TEVO_CLK_STOPn, PORE_WIN_MODE);
  /*p27.XACO*/ wire XACO_X_RSTn = not(XOFO_WIN_RST);

  /*p27.WYKA*/ MAP_X0.set(VETU_X_CLK, XACO_X_RSTn, !MAP_X0);
  /*p27.WODY*/ MAP_X1.set(!MAP_X0,    XACO_X_RSTn, !MAP_X1);
  /*p27.WOBO*/ MAP_X2.set(!MAP_X1,    XACO_X_RSTn, !MAP_X2);
  /*p27.WYKO*/ MAP_X3.set(!MAP_X2,    XACO_X_RSTn, !MAP_X3);
  /*p27.XOLO*/ MAP_X4.set(!MAP_X3,    XACO_X_RSTn, !MAP_X4);

  /*p27.WAZY*/ wire WAZY_Y_CLK = not(PORE_WIN_MODE);
  /*p27.REPU*/ wire REPU_RST  = nor(lcd_sig.PARU_VBLANK_d4, PYRY_VID_RST);   // schematic wrong, this is NOR
  /*p27.SYNY*/ wire SYNY_RSTn = not(REPU_RST);

  /*p27.VYNO*/ TILE_Y0.set(WAZY_Y_CLK, SYNY_RSTn, !TILE_Y0);
  /*p27.VUJO*/ TILE_Y1.set(!TILE_Y0,   SYNY_RSTn, !TILE_Y1);
  /*p27.VYMU*/ TILE_Y2.set(!TILE_Y1,   SYNY_RSTn, !TILE_Y2);

  /*p27.TUFU*/ MAP_Y0.set(!TILE_Y2,    SYNY_RSTn, !MAP_Y0);
  /*p27.TAXA*/ MAP_Y1.set(!MAP_Y0,     SYNY_RSTn, !MAP_Y1);
  /*p27.TOZO*/ MAP_Y2.set(!MAP_Y1,     SYNY_RSTn, !MAP_Y2);
  /*p27.TATE*/ MAP_Y3.set(!MAP_Y2,     SYNY_RSTn, !MAP_Y3);
  /*p27.TEKE*/ MAP_Y4.set(!MAP_Y3,     SYNY_RSTn, !MAP_Y4);
}

//-----------------------------------------------------------------------------
// Pixel counter. This is a little weird, presumably because it can tick at 4 mhz but not always?

void VidRegisters::pixelCounter(const RstSignals& rst_sig, const LcdSignals& lcd_sig, bool CLKPIPE) {

  /*p01.TOFU*/ wire VID_RESET3  = not(rst_sig.VID_RESETn);
  /*p21.TADY*/ wire TADY_X_RST = nor(lcd_sig.BYHA_VID_LINE_TRIG_d4n, VID_RESET3);

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

  /*p21.XEHO*/ X0.set(CLKPIPE, TADY_X_RST, !X0);
  /*p21.SAVY*/ X1.set(CLKPIPE, TADY_X_RST, RYBO);
  /*p21.XODU*/ X2.set(CLKPIPE, TADY_X_RST, XEGY);
  /*p21.XYDO*/ X3.set(CLKPIPE, TADY_X_RST, XORA);
  /*p21.TUHU*/ X4.set(TOCA,    TADY_X_RST, !X4);
  /*p21.TUKY*/ X5.set(TOCA,    TADY_X_RST, SAKE);
  /*p21.TAKO*/ X6.set(TOCA,    TADY_X_RST, TYGE);
  /*p21.SYBE*/ X7.set(TOCA,    TADY_X_RST, ROKU);

  /*p21.XUGU*/ wire X_167n = nand(X0, X1, X2, X5, X7); // 128 + 32 + 4 + 2 + 1 = 167
  /*p21.XANO*/ wire X_167 = not(X_167n);
}

//-----------------------------------------------------------------------------
// LY compare

void VidRegisters::lyMatch(const BusSignals& bus_sig,
                           const DecoderSignals& dec_sig,
                           const VclkSignals& vid_clk,
                           const RstSignals& rst_sig,
                           const LcdRegisters& lcd_reg,
                           const VidConfig& vid_reg2) {
  /*p21.SYFU*/ wire LY_MATCH7 = xor(lcd_reg.Y7, vid_reg2.LYC7);
  /*p21.TERY*/ wire LY_MATCH6 = xor(lcd_reg.Y6, vid_reg2.LYC6);
  /*p21.TUCY*/ wire LY_MATCH5 = xor(lcd_reg.Y5, vid_reg2.LYC5);
  /*p21.TYKU*/ wire LY_MATCH4 = xor(lcd_reg.Y4, vid_reg2.LYC4);
  /*p21.RASY*/ wire LY_MATCH3 = xor(lcd_reg.Y3, vid_reg2.LYC3);
  /*p21.REDA*/ wire LY_MATCH2 = xor(lcd_reg.Y2, vid_reg2.LYC2);
  /*p21.TYDE*/ wire LY_MATCH1 = xor(lcd_reg.Y1, vid_reg2.LYC1);
  /*p21.RYME*/ wire LY_MATCH0 = xor(lcd_reg.Y0, vid_reg2.LYC0);

  /*p21.SOVU*/ wire LY_MATCHA = nor(LY_MATCH7, LY_MATCH6, LY_MATCH5, LY_MATCH4);
  /*p21.SUBO*/ wire LY_MATCHB = nor(LY_MATCH3, LY_MATCH2, LY_MATCH1, LY_MATCH0);

  /*p21.RAPE*/ wire LY_MATCHn = nand(LY_MATCHA, LY_MATCHB);
  /*p21.PALY*/ wire LY_MATCHa = not(LY_MATCHn);

  /*p01.CUNU*/ bool CUNU_RESETn = not(rst_sig.SYS_RESET);
  /*p01.XORE*/ bool XORE_RESET  = not(CUNU_RESETn);
  /*p01.WESY*/ bool WESY_RESET  = not(XORE_RESET);

  /*p21.SEPA*/ wire FF41_WR = and(bus_sig.CUPA_CPUWR, dec_sig.FF41);
  /*p21.RYJU*/ wire FF41_WRn = not(FF41_WR);

  /*p21.ROPO*/ ROPO_LYC_MATCH.set(vid_clk.TALU_xBCDExxx, WESY_RESET, LY_MATCHa);
  /*p21.PAGO*/ wire PAGO_LYC_MATCH_RST = nor(WESY_RESET, FF41_WRn);  // schematic wrong, this is NOR
  /*p21.RUPO*/ RUPO_LATCH_LYC_MATCH.latch(ROPO_LYC_MATCH, PAGO_LYC_MATCH_RST); // this is another of the weird or gates. could be nor?
}

//-----------------------------------------------------------------------------

bool VidRegisters::winMatch(const ClkSignals& clk_sig,
                            const VclkSignals& vid_clk,
                            const RstSignals& rst_sig,
                            const LcdSignals& lcd_sig,
                            const LcdRegisters& lcd_reg,
                            const VidConfig& vid_reg2,
                            bool SEGU_4M,
                            bool ROCO_4M) {
  
  //----------
  // Window Y match


  /*p27.NAZE*/ wire WY_MATCH0n = xor(lcd_reg.Y0, vid_reg2.WY0);
  /*p27.PEBO*/ wire WY_MATCH1n = xor(lcd_reg.Y1, vid_reg2.WY1);
  /*p27.POMO*/ wire WY_MATCH2n = xor(lcd_reg.Y2, vid_reg2.WY2);
  /*p27.NEVU*/ wire WY_MATCH3n = xor(lcd_reg.Y3, vid_reg2.WY3);
  /*p27.NOJO*/ wire WY_MATCH4n = xor(lcd_reg.Y4, vid_reg2.WY4);
  /*p27.PAGA*/ wire WY_MATCH5n = xor(lcd_reg.Y5, vid_reg2.WY5);
  /*p27.PEZO*/ wire WY_MATCH6n = xor(lcd_reg.Y6, vid_reg2.WY6);
  /*p27.NUPA*/ wire WY_MATCH7n = xor(lcd_reg.Y7, vid_reg2.WY7);

  // huh, the y matcher runs at 1 mhz but the x matcher runs at 4 mhz
  /*p27.PALO*/ wire WY_MATCH_HIn   = nand(vid_reg2.LCDC_WINEN, WY_MATCH4n, WY_MATCH5n, WY_MATCH6n, WY_MATCH7n);
  /*p27.NELE*/ wire WY_MATCH_HI    = not(WY_MATCH_HIn);
  /*p27.PAFU*/ wire WY_MATCHn      = nand(WY_MATCH_HI, WY_MATCH0n, WY_MATCH1n, WY_MATCH2n, WY_MATCH3n);
  /*p27.ROGE*/ wire WY_MATCH       = not(WY_MATCHn);
  /*p27.SARY*/ WY_MATCH_SYNC.set(vid_clk.TALU_xBCDExxx, rst_sig.VID_RESETn, WY_MATCH);

  //----------
  // Window X match

  // polarity or gates wrong
  /*p01.PYRY*/ wire VID_RESET4  = not(rst_sig.VID_RESETn);
  /*p27.REPU*/ wire IN_FRAME_Y  = nor(lcd_sig.PARU_VBLANK_d4, VID_RESET4);   // schematic wrong, this is NOR
  /*p27.REJO*/ wire WIN_CHECK_X = or(WY_MATCH_SYNC, IN_FRAME_Y); // another weird or gate. should be AND?

  /*p27.MYLO*/ wire WX_MATCH0n = xor(X0, vid_reg2.WX0);
  /*p27.PUWU*/ wire WX_MATCH1n = xor(X1, vid_reg2.WX1);
  /*p27.PUHO*/ wire WX_MATCH2n = xor(X2, vid_reg2.WX2);
  /*p27.NYTU*/ wire WX_MATCH3n = xor(X3, vid_reg2.WX3);
  /*p27.NEZO*/ wire WX_MATCH4n = xor(X4, vid_reg2.WX4);
  /*p27.NORY*/ wire WX_MATCH5n = xor(X5, vid_reg2.WX5);
  /*p27.NONO*/ wire WX_MATCH6n = xor(X6, vid_reg2.WX6);
  /*p27.PASE*/ wire WX_MATCH7n = xor(X7, vid_reg2.WX7);

  /*p27.PUKY*/ wire WX_MATCH_HIn = nand(WIN_CHECK_X, WX_MATCH4n, WX_MATCH5n, WX_MATCH6n, WX_MATCH7n);
  /*p27.NUFA*/ wire WX_MATCH_HI  = not (WX_MATCH_HIn);
  /*p27.NOGY*/ wire WIN_MATCHn   = nand(WX_MATCH_HI, WX_MATCH0n, WX_MATCH1n, WX_MATCH2n, WX_MATCH3n);
  /*p27.NUKO*/ wire WIN_MATCH    = not (WIN_MATCHn);

  /*p27.ROZE*/ wire FINE_COUNT_STOPn = nand(FINE_CNT0, FINE_CNT1, FINE_CNT2);
  /*p27.PANY*/ wire WIN_MATCH_ONSCREEN = nor(WIN_MATCH, FINE_COUNT_STOPn);

  /*p27.RYFA*/ WIN_MATCH_ONSCREEN_SYNC1.set(SEGU_4M,               XYMU_RENDERING_LATCH, WIN_MATCH_ONSCREEN);
  /*p27.RENE*/ WIN_MATCH_ONSCREEN_SYNC2.set(clk_sig.ALET_AxCxExGx, XYMU_RENDERING_LATCH, WIN_MATCH_ONSCREEN_SYNC1);

  /*p27.MEHE*/ wire MEHE_xBxDxFxH = not(clk_sig.ALET_AxCxExGx);

  /*p27.PYCO*/ WIN_MATCH_SYNC1.set(ROCO_4M,       rst_sig.VID_RESETn, WIN_MATCH);
  /*p27.NUNU*/ WIN_MATCH_SYNC2.set(MEHE_xBxDxFxH, rst_sig.VID_RESETn, WIN_MATCH_SYNC1);

  /*p27.XOFO*/ wire WIN_MODE_RST = nand(vid_reg2.LCDC_WINEN, lcd_sig.XAHY_VID_LINE_TRIG_d4n, rst_sig.VID_RESETn);
  /*p27.PYNU*/ WIN_MODE_LATCH1.latch(WIN_MATCH_SYNC2, WIN_MODE_RST);

  /*p27.NOCU*/ wire WIN_MODE_NOCUn = not(WIN_MODE_LATCH1);
  /*p27.PORE*/ wire WIN_MODE_PORE  = not(WIN_MODE_NOCUn);
  /*p26.AXAD*/ wire WIN_MODE_AXADn = not(WIN_MODE_PORE);

  /*p27.NOPA*/ NOPA_WIN_MODE_SYNC.set(clk_sig.ALET_AxCxExGx, rst_sig.VID_RESETn, WIN_MODE_LATCH1);
  /*p27.NUNY*/ wire NUNY_WIN_MODE_TRIG = and(WIN_MODE_LATCH1, !NOPA_WIN_MODE_SYNC);

  // PUKU/RYDY form a NOR latch. WIN_MODE_TRIG is SET, (VID_RESET | BG_SEQ_7) is RESET.
  ///*p27.PUKU*/ PUKU = nor(RYDY, WIN_MODE_TRIG);
  ///*p27.RYDY*/ RYDY = nor(PUKU, rst_reg.VID_RESET4, BG_SEQ_7);

  /*p27.RYDY*/ RYDY_WIN_MODE_LATCH.latch(NUNY_WIN_MODE_TRIG, VID_RESET4 || BG_SEQ_7);

  /*p27.SOVY*/ SOVY_WIN_MODE_SYNC.set(clk_sig.ALET_AxCxExGx, rst_sig.VID_RESETn, RYDY_WIN_MODE_LATCH);

  return WIN_MATCH;
}

//-----------------------------------------------------------------------------

};
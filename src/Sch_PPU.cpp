#include "Sch_PPU.h"

#include "TestGB.h"

using namespace Schematics;

// Die trace
// NUNY04 = and(PYNU04, NOPA16)
// NYFO02 = not(NUNY04)
// MOSU03 = not(NYFO02) // 3 rung inverter
// ABAF02 = not(CATU17)

// BYHA = weirdgate
// BYHA01 << ANEL17
// BYHA02 << ABAF02
// BYHA03 xx CATU17 (crossover?)
// BYHA04 << ABEZ02
// BYHA05 >> ATEJ01

// ATEJ03 = not(BYHA05)
// ANOM03 = nor(ATEJ03, ATAR02)
// BALU02 = not(ANOM03)
// BEBU05 = or(DOBA17, BALU02, BYBA16)
// AVAP03 = not(BEBU05)
// NYXU04 = nor(AVAP03, MOSU03, TEVO05)

// Die trace:
// SAKY = nor(TULY17, VONU17)
// TEPA = not(XYMU)
// TYSO = or(SAKY, TEPA)
// TEXY = not(TYSO)

// WUSA arms on the ground side, nor latch
// WUSA00 << XAJO03
// WUSA01 nc
// WUSA02 >> nc
// WUSA03 >> TOBA00
// WUSA04 nc 
// WUSA05 << WEGO03

// When XAJO03 goes high, WUSA03 goes high.
// When WEGO03 goes high, WUSA03 goes low.

// XEHO01 <> XEHO12
// XEHO02 << SACU04
// XEHO03 <> XEHO09
// XEHO04 nc
// XEHO05 nc
// XEHO06 << XYMU03
// XEHO07 << XEHO16 // so it's a counter
// XEHO08 nc
// XEHO09 <> XEHO03
// XEHO10 nc
// XEHO11 ?? RUMA01
// XEHO12 <> XEHO01
// XEHO13 << XYMU03
// XEHO14 nc
// XEHO15 nc
// XEHO16
// XEHO17 >> XAJO01

// ROXY = NOR latch
// ROXY00 << PAHA
// ROXY01 nc
// ROXY02 >> nc
// ROXY03 >> RONE00
// ROXY04 nc
// ROXY05 << POVA

// If PAHA goes high, ROXY03 goes high.
// If POVA goes high, ROXY03 goes low

// LURY01 << LOVY15 (next to bottom rung)
// LURY02 << XYMU02
// LURY03 nc
// LURY04 >> LONY01

// LONY has "arms" on the VCC side - different from the other latches?
// Probably a NAND latch instead of NOR
// LONY01 << LURY03
// LONY02 nc
// LONY03 == nc
// LONY04 >> LUSU01, MYMA01
// LONY05 nc
// LONY06 << NYXU03

// if LURY goes low, LONY goes low
// if NYXU goes low, LONY goes high

// NOR latch
// POKY00 << PYGO
// POKY01 nc
// POKY02 >> nc
// POKY03 >> ROMO00, others
// POKY04 nc
// POKY05 << LOBY01

// If PYGO goes high, POKY03 goes high
// if LOBY goes high, POKY03 goes low.

// XYMU has "arms" on the ground side
// XYMU00 << WEGO03
// XYMU01 nc
// XYMU02 >> bunch of stuff
// XYMU03 >> nc
// XYMU04 nc
// XYMU05 << AVAP02

// if AVAP02 goes high, XYMU02 goes high.
// if WEGO03 goes high, XYMU02 goes low.

// TAKA has arms on the VCC side - nand latch
// TAKA01 << VEKU02
// TAKA02 nc
// TAKA03 >> nc
// TAKA04 >> SOWO00
// TAKA05 nc
// TAKA06 << SECA03
// if SECA03 goes low, TAKA04 goes high
// if VEKU02 goes low, TAKA04 goes low

//---------------------------------------------------------------------------

struct ScrollSignals {
  /*p26.FAFO*/ wire _TILE_Y0S;
  /*p26.EMUX*/ wire _TILE_Y1S;
  /*p26.ECAB*/ wire _TILE_Y2S;
  /*p26.ETAM*/ wire _MAP_Y0S;
  /*p26.DOTO*/ wire _MAP_Y1S;
  /*p26.DABA*/ wire _MAP_Y2S;
  /*p26.EFYK*/ wire _MAP_Y3S;
  /*p26.EJOK*/ wire _MAP_Y4S;

  /*p26.BABE*/ wire _MAP_X0S;
  /*p26.ABOD*/ wire _MAP_X1S;
  /*p26.BEWY*/ wire _MAP_X2S;
  /*p26.BYCA*/ wire _MAP_X3S;
  /*p26.ACUL*/ wire _MAP_X4S;

  static ScrollSignals get(const LcdRegisters& lcd_reg, const PpuRegisters& ppu_reg, const ConfigRegisters& cfg_reg) {
    /*p26.FAFO*/ wire _TILE_Y0S = add_s(lcd_reg.MUWY_Y0.q(), cfg_reg.SCY0.q(), 0);
    /*p26.FAFO*/ wire _TILE_Y0C = add_c(lcd_reg.MUWY_Y0.q(), cfg_reg.SCY0.q(), 0);
    /*p26.EMUX*/ wire _TILE_Y1S = add_s(lcd_reg.MYRO_Y1.q(), cfg_reg.SCY1.q(), _TILE_Y0C);
    /*p26.EMUX*/ wire _TILE_Y1C = add_c(lcd_reg.MYRO_Y1.q(), cfg_reg.SCY1.q(), _TILE_Y0C);
    /*p26.ECAB*/ wire _TILE_Y2S = add_s(lcd_reg.LEXA_Y2.q(), cfg_reg.SCY2.q(), _TILE_Y1C);
    /*p26.ECAB*/ wire _TILE_Y2C = add_c(lcd_reg.LEXA_Y2.q(), cfg_reg.SCY2.q(), _TILE_Y1C);
    /*p26.ETAM*/ wire _MAP_Y0S = add_s(lcd_reg.LYDO_Y3.q(), cfg_reg.SCY3.q(), _TILE_Y2C);
    /*p26.ETAM*/ wire _MAP_Y0C = add_c(lcd_reg.LYDO_Y3.q(), cfg_reg.SCY3.q(), _TILE_Y2C);
    /*p26.DOTO*/ wire _MAP_Y1S = add_s(lcd_reg.LOVU_Y4.q(), cfg_reg.SCY4.q(), _MAP_Y0C);
    /*p26.DOTO*/ wire _MAP_Y1C = add_c(lcd_reg.LOVU_Y4.q(), cfg_reg.SCY4.q(), _MAP_Y0C);
    /*p26.DABA*/ wire _MAP_Y2S = add_s(lcd_reg.LEMA_Y5.q(), cfg_reg.SCY5.q(), _MAP_Y1C);
    /*p26.DABA*/ wire _MAP_Y2C = add_c(lcd_reg.LEMA_Y5.q(), cfg_reg.SCY5.q(), _MAP_Y1C);
    /*p26.EFYK*/ wire _MAP_Y3S = add_s(lcd_reg.MATO_Y6.q(), cfg_reg.SCY6.q(), _MAP_Y2C);
    /*p26.EFYK*/ wire _MAP_Y3C = add_c(lcd_reg.MATO_Y6.q(), cfg_reg.SCY6.q(), _MAP_Y2C);
    /*p26.EJOK*/ wire _MAP_Y4S = add_s(lcd_reg.LAFO_Y7.q(), cfg_reg.SCY7.q(), _MAP_Y3C);
    /*p26.EJOK*/ wire _MAP_Y4C = add_c(lcd_reg.LAFO_Y7.q(), cfg_reg.SCY7.q(), _MAP_Y3C);

    (void)_MAP_Y4C;

    /*p26.ATAD*/ wire _TILE_X0S = add_s(ppu_reg.XEHO_X0.q(), cfg_reg.SCX0.q(), 0); // unused
    /*p26.ATAD*/ wire _TILE_X0C = add_c(ppu_reg.XEHO_X0.q(), cfg_reg.SCX0.q(), 0);
    /*p26.BEHU*/ wire _TILE_X1S = add_s(ppu_reg.SAVY_X1.q(), cfg_reg.SCX1.q(), _TILE_X0C); // unused
    /*p26.BEHU*/ wire _TILE_X1C = add_c(ppu_reg.SAVY_X1.q(), cfg_reg.SCX1.q(), _TILE_X0C);
    /*p26.APYH*/ wire _TILE_X2S = add_s(ppu_reg.XODU_X2.q(), cfg_reg.SCX2.q(), _TILE_X1C); // unused
    /*p26.APYH*/ wire _TILE_X2C = add_c(ppu_reg.XODU_X2.q(), cfg_reg.SCX2.q(), _TILE_X1C);
    /*p26.BABE*/ wire _MAP_X0S = add_s(ppu_reg.XYDO_X3.q(), cfg_reg.SCX3.q(), _TILE_X2C);
    /*p26.BABE*/ wire _MAP_X0C = add_c(ppu_reg.XYDO_X3.q(), cfg_reg.SCX3.q(), _TILE_X2C);
    /*p26.ABOD*/ wire _MAP_X1S = add_s(ppu_reg.TUHU_X4.q(), cfg_reg.SCX4.q(), _MAP_X0C);
    /*p26.ABOD*/ wire _MAP_X1C = add_c(ppu_reg.TUHU_X4.q(), cfg_reg.SCX4.q(), _MAP_X0C);
    /*p26.BEWY*/ wire _MAP_X2S = add_s(ppu_reg.TUKY_X5.q(), cfg_reg.SCX5.q(), _MAP_X1C);
    /*p26.BEWY*/ wire _MAP_X2C = add_c(ppu_reg.TUKY_X5.q(), cfg_reg.SCX5.q(), _MAP_X1C);
    /*p26.BYCA*/ wire _MAP_X3S = add_s(ppu_reg.TAKO_X6.q(), cfg_reg.SCX6.q(), _MAP_X2C);
    /*p26.BYCA*/ wire _MAP_X3C = add_c(ppu_reg.TAKO_X6.q(), cfg_reg.SCX6.q(), _MAP_X2C);
    /*p26.ACUL*/ wire _MAP_X4S = add_s(ppu_reg.SYBE_X7.q(), cfg_reg.SCX7.q(), _MAP_X3C);
    /*p26.ACUL*/ wire _MAP_X4C = add_c(ppu_reg.SYBE_X7.q(), cfg_reg.SCX7.q(), _MAP_X3C);

    (void)_TILE_X0S;
    (void)_TILE_X1S;
    (void)_TILE_X2S;
    (void)_MAP_X4C;

    return {
      ._TILE_Y0S = _TILE_Y0S,
      ._TILE_Y1S = _TILE_Y1S,
      ._TILE_Y2S = _TILE_Y2S,
      ._MAP_Y0S = _MAP_Y0S,
      ._MAP_Y1S = _MAP_Y1S,
      ._MAP_Y2S = _MAP_Y2S,
      ._MAP_Y3S = _MAP_Y3S,
      ._MAP_Y4S = _MAP_Y4S,
      ._MAP_X0S = _MAP_X0S,
      ._MAP_X1S = _MAP_X1S,
      ._MAP_X2S = _MAP_X2S,
      ._MAP_X3S = _MAP_X3S,
      ._MAP_X4S = _MAP_X4S,
    };
  }
};

//------------------------------------------------------------------------------

PpuSignals PpuRegisters::sig(const TestGB& gb) const {
  auto win_sig = gb.win_reg.sig(gb);

  auto& sst_reg = gb.sst_reg;
  auto sst_sig = sst_reg.sig(gb);
  auto clk_sig = gb.clk_reg.sig(gb);
  auto rst_sig = gb.rst_reg.sig(gb);
  auto dma_sig = gb.dma_reg.sig(gb);
  auto adr_sig = gb.adr_reg.sig(gb.cpu_bus);

  auto& vram_bus = gb.vram_bus;
  auto dbg_sig = gb.dbg_reg.sig(gb);

  //----------

  /*p21.XUGU*/ wire _XUGU_X_167n = nand(XEHO_X0.q(), SAVY_X1.q(), XODU_X2.q(), TUKY_X5.q(), SYBE_X7.q()); // 128 + 32 + 4 + 2 + 1 = 167
  /*p21.XANO*/ wire _XANO_X_167 = not(_XUGU_X_167n);
  /*p21.XENA*/ wire _XENA_STORE_MATCHn = not(sst_sig.FEPO_STORE_MATCHp);
  /*p21.WODU*/ wire _WODU_RENDER_DONE = and (_XENA_STORE_MATCHn, _XANO_X_167);

  /*p27.ROMO*/ wire _ROMO_FRONT_PORCH = not(POKY_FRONT_PORCH_LATCH);
  /*p27.SUVU*/ wire _SUVU = nand(XYMU_RENDERINGp, _ROMO_FRONT_PORCH, NYKA_BFETCH_DONE_SYNC, PORY_BFETCH_DONE_SYNC_DELAY);
  /*p27.TAVE*/ wire _TAVE = not(_SUVU);

  /*p27.TEVO*/ wire _TEVO_CLK_STOPn = nor(win_sig.SEKO_WIN_TRIGGER, win_sig.SUZU, _TAVE);

  /*p27.NYXU*/ wire _NYXU_BFETCH_RSTn = nor(sst_sig.AVAP_SCAN_DONE_d0_TRIGp, win_sig.MOSU_WIN_MODE_TRIGp, _TEVO_CLK_STOPn);

  /*p28.ACYL*/ wire _ACYL_PPU_USE_OAM1p = and (dma_sig.BOGE_DMA_RUNNINGn, sst_sig.BESU_SCANNINGp);

  // So we only read a sprite if both those regs are... low? what is rung 17's polarity?

  /*p29.SAKY*/ wire _SAKY = nor(TULY_SFETCH_S1.q(), VONU_SFETCH_S1_D4.q());
  /*p29.TEPA*/ wire _TEPA_RENDERINGn = not(XYMU_RENDERINGp);
  /*p29.TYSO*/ wire _TYSO_SPRITE_READn = or(_SAKY, _TEPA_RENDERINGn);
  /*p29.TEXY*/ wire _TEXY_SPRITE_READ = not(_TYSO_SPRITE_READn);

  /*p29.TYNO*/ wire _TYNO = nand(TOXE_SFETCH_S0_D0.q(), SEBA_SFETCH_S1_D5.q(), VONU_SFETCH_S1_D4.q());
  /*p29.VUSA*/ wire _VUSA = or(!TYFO_SFETCH_S0_D1.q(), _TYNO);
  /*p29.WUTY*/ wire _WUTY_SPRITE_DONE = not(_VUSA);

  /*p24.VYBO*/ wire _VYBO_PIX_CLK_AxCxExGx = nor(sst_sig.FEPO_STORE_MATCHp, _WODU_RENDER_DONE, clk_sig.MYVO_AxCxExGx);
  /*p24.TYFA*/ wire _TYFA_AxCxExGx = and (win_sig.SOCY_WIN_HITn, POKY_FRONT_PORCH_LATCH, _VYBO_PIX_CLK_AxCxExGx);
  /*p24.SEGU*/ wire _SEGU_xBxDxFxH = not(_TYFA_AxCxExGx);
  /*p24.ROXO*/ wire _ROXO_AxCxExGx = not(_SEGU_xBxDxFxH);
  /*p27.ROCO*/ wire _ROCO_AxCxExGx = not(_SEGU_xBxDxFxH);
  /*p24.SACU*/ wire _SACU_CLKPIPE_AxCxExGx = nor(_SEGU_xBxDxFxH, ROXY_FINE_MATCH_LATCHn);

  /*p24.LOBY*/ wire _LOBY_RENDERINGn = not(XYMU_RENDERINGp.q());
  /*p25.ROPY*/ wire _ROPY_RENDERINGn = not(XYMU_RENDERINGp.q());

  /*p27.ROZE*/ wire _ROZE_FINE_COUNT_STOPn = nand(RYKU_FINE_CNT0, ROGA_FINE_CNT1, RUBU_FINE_CNT2);

  /*p25.TUCA*/ wire _TUCA_CPU_VRAM_RD = and (adr_sig.SOSE_8000_9FFFp, dbg_sig.ABUZ);
  /*p25.TEFY*/ wire _TEFY_MCS_Cn = not(vram_bus.PIN_MCSn_C);
  /*p25.TOLE*/ wire _TOLE_VRAM_RD = mux2_p(_TEFY_MCS_Cn, _TUCA_CPU_VRAM_RD, dbg_sig.TUTO_DBG_VRAM);
  /*p25.SERE*/ wire _SERE_VRAM_RD = and (_TOLE_VRAM_RD, _ROPY_RENDERINGn);
  /*p29.TYTU*/ wire _TYTU_SFETCH_S0_D0n = not(TOXE_SFETCH_S0_D0.q());
  /*p29.TACU*/ wire _TACU_SPR_SEQ_5_TRIG = nand(TYFO_SFETCH_S0_D1.q(), _TYTU_SFETCH_S0_D0n);
  /*p27.NAKO*/ wire _NAKO_FETCH_S1n = not(MESU_BFETCH_S1.q());
  /*p27.NOFU*/ wire _NOFU_FETCH_S2n = not(NYVA_BFETCH_S2.q());


  return {
    .TEVO_CLK_STOPn = _TEVO_CLK_STOPn,
    .WODU_RENDER_DONE = _WODU_RENDER_DONE,
    .NYXU_BFETCH_RSTn = _NYXU_BFETCH_RSTn,
    .TEXY_SPRITE_READ = _TEXY_SPRITE_READ,
    .WUTY_SPRITE_DONE = _WUTY_SPRITE_DONE,
    .ACYL_PPU_USE_OAM1p = _ACYL_PPU_USE_OAM1p,
    .POKY_FRONT_PORCH_LATCH = POKY_FRONT_PORCH_LATCH,
    .TAVE = _TAVE,
    .XYMU_RENDERINGp = XYMU_RENDERINGp,
    .TEPA_RENDERINGn = _TEPA_RENDERINGn,
    .LOBY_RENDERINGn = _LOBY_RENDERINGn,
    .ROPY_RENDERINGn = _ROPY_RENDERINGn,
    .SEGU_xBxDxFxH = _SEGU_xBxDxFxH,
    .ROXO_AxCxExGx = _ROXO_AxCxExGx,
    .ROCO_AxCxExGx = _ROCO_AxCxExGx,
    .SERE_VRAM_RD = _SERE_VRAM_RD,
    .SACU_CLKPIPE_AxCxExGx = _SACU_CLKPIPE_AxCxExGx,
    .ROZE_FINE_COUNT_STOPn = _ROZE_FINE_COUNT_STOPn,
    .TYTU_SFETCH_S0_D0n = _TYTU_SFETCH_S0_D0n,
    .TACU_SPR_SEQ_5_TRIG = _TACU_SPR_SEQ_5_TRIG,
    .NAKO_FETCH_S1n = _NAKO_FETCH_S1n,
    .NOFU_FETCH_S2n = _NOFU_FETCH_S2n,
  };
}

//------------------------------------------------------------------------------

void PpuRegisters::tick(TestGB& gb) {
  auto& ppu_reg = *this;
  auto& lcd_reg = gb.lcd_reg;
  auto& cfg_reg = gb.cfg_reg;
  auto& win_reg = gb.win_reg;
  auto& pxp_reg = gb.pxp_reg;
  auto& oam_reg = gb.oam_reg;
  auto& sst_reg = gb.sst_reg;

  auto clk_sig = gb.clk_reg.sig(gb);
  auto rst_sig = gb.rst_reg.sig(gb);
  auto win_sig = gb.win_reg.sig(gb);
  auto lcd_sig = gb.lcd_reg.sig(gb);
  auto dbg_sig = gb.dbg_reg.sig(gb);
  auto sst_sig = gb.sst_reg.sig(gb);
  auto ppu_sig = sig(gb);
  auto scr_sig = ScrollSignals::get(lcd_reg, ppu_reg, cfg_reg);

  auto& vram_bus = gb.vram_bus;

  //----------

  // The first tile generated is thrown away. I'm calling that section of rendering the front porch.

  {
    /*p24.PYGO*/ PYGO_TILE_DONE.set(clk_sig.ALET_xBxDxFxH, XYMU_RENDERINGp, PORY_BFETCH_DONE_SYNC_DELAY);
  }

  {
    /*p24.POKY*/ POKY_FRONT_PORCH_LATCH.nor_latch(PYGO_TILE_DONE, ppu_sig.LOBY_RENDERINGn);
  }


  {
    /*p21.WEGO*/ wire _WEGO_RST_LATCH = or(rst_sig.TOFU_VID_RSTp, VOGA_RENDER_DONE_SYNC);
    /*p27.POVA*/ wire _POVA_FINE_MATCH_SETp = and (PUXA_FINE_MATCH_SYNC1, !NYZE_FINE_MATCH_SYNC2);

    /*p21.XYMU*/ XYMU_RENDERINGp.nor_latch(sst_sig.AVAP_SCAN_DONE_d0_TRIGp, _WEGO_RST_LATCH);

    {
      // XAJO04 = and(XEHO17, XYDO17);
      /*p21.XAJO*/ wire _XAJO_X_009 = and (XEHO_X0.q(), XYDO_X3.q());
      /*p21.WUSA*/ WUSA_CPEN_LATCH.nor_latch(_XAJO_X_009, _WEGO_RST_LATCH);
      /*p21.TOBA*/ wire _TOBA = and (ppu_sig.SACU_CLKPIPE_AxCxExGx, WUSA_CPEN_LATCH);
      /*p21.SEMU*/ wire _SEMU_LCD_CPn = or(_TOBA, _POVA_FINE_MATCH_SETp);
      /*p21.RYPO*/ wire _RYPO_LCD_CP = not(_SEMU_LCD_CPn);
      CP.set(_RYPO_LCD_CP);
    }

    {
      /*p27.PAHA*/ wire _PAHA_FINE_MATCH_RSTp = not(XYMU_RENDERINGp);
      /*p27.ROXY*/ ROXY_FINE_MATCH_LATCHn.nor_latch(_PAHA_FINE_MATCH_RSTp, _POVA_FINE_MATCH_SETp);
    }

    {
      /*p27.PECU*/ wire _PECU_FINE_CLK = nand(ppu_sig.ROXO_AxCxExGx, ppu_sig.ROZE_FINE_COUNT_STOPn);
      /*p27.PASO*/ wire _PASO_FINE_RST = nor(ppu_sig.TEVO_CLK_STOPn, ppu_sig.ROPY_RENDERINGn);
      /*p27.RYKU*/ RYKU_FINE_CNT0.set(_PECU_FINE_CLK, _PASO_FINE_RST, !RYKU_FINE_CNT0);
      /*p27.ROGA*/ ROGA_FINE_CNT1.set(!RYKU_FINE_CNT0, _PASO_FINE_RST, !ROGA_FINE_CNT1);
      /*p27.RUBU*/ RUBU_FINE_CNT2.set(!ROGA_FINE_CNT1, _PASO_FINE_RST, !RUBU_FINE_CNT2);
    }

    {
      /*p27.SUHA*/ wire _SUHA_FINE_MATCH0 = xnor(cfg_reg.SCX0, RYKU_FINE_CNT0); // Arms on the ground side, XNOR
      /*p27.SYBY*/ wire _SYBY_FINE_MATCH1 = xnor(cfg_reg.SCX1, ROGA_FINE_CNT1);
      /*p27.SOZU*/ wire _SOZU_FINE_MATCH2 = xnor(cfg_reg.SCX2, RUBU_FINE_CNT2);

      /*p27.RONE*/ wire _RONE_FINE_MATCHn = nand(ROXY_FINE_MATCH_LATCHn, _SUHA_FINE_MATCH0, _SYBY_FINE_MATCH1, _SOZU_FINE_MATCH2);
      /*p27.POHU*/ wire _POHU_FINE_MATCH = not(_RONE_FINE_MATCHn);
      /*p27.MOXE*/ wire _MOXE_AxCxExGx = not(clk_sig.ALET_xBxDxFxH);
      /*p27.PUXA*/ PUXA_FINE_MATCH_SYNC1.set(ppu_sig.ROXO_AxCxExGx, XYMU_RENDERINGp, _POHU_FINE_MATCH);
      /*p27.NYZE*/ NYZE_FINE_MATCH_SYNC2.set(_MOXE_AxCxExGx, XYMU_RENDERINGp, PUXA_FINE_MATCH_SYNC1);
    }
  }

  //----------

  /*p27.MOCE*/ wire _MOCE_BFETCH_DONEn = nand(LAXU_BFETCH_S0, NYVA_BFETCH_S2, ppu_sig.NYXU_BFETCH_RSTn);

  {
    /*p27.LEBO*/ wire _LEBO_BFETCH_CLKp = nand(clk_sig.ALET_xBxDxFxH, _MOCE_BFETCH_DONEn);

    /*p27.LAXU*/ LAXU_BFETCH_S0.set(_LEBO_BFETCH_CLKp, ppu_sig.NYXU_BFETCH_RSTn, !LAXU_BFETCH_S0);
    /*p27.MESU*/ MESU_BFETCH_S1.set(!LAXU_BFETCH_S0, ppu_sig.NYXU_BFETCH_RSTn, !MESU_BFETCH_S1);
    /*p27.NYVA*/ NYVA_BFETCH_S2.set(!MESU_BFETCH_S1, ppu_sig.NYXU_BFETCH_RSTn, !NYVA_BFETCH_S2);
  }

  {
    // this seems reasonable. BG_READ_VRAM_LATCHp is bracketed by BFETCH_RSTn (start) and
    // vid_reg.LOVY_BG_SEQ5_SYNC (stop).

    /*p27.LURY*/ wire _LURY_BG_READ_VRAM_LATCH_RSTn = and(!LOVY_BG_SEQ5_SYNC, XYMU_RENDERINGp);
    /*p27.LONY*/ LONY_BG_READ_VRAM_LATCHp.nand_latch(ppu_sig.NYXU_BFETCH_RSTn, _LURY_BG_READ_VRAM_LATCH_RSTn);
  }

  /*p27.LYRY*/ wire _LYRY_BFETCH_DONEp = not(_MOCE_BFETCH_DONEn);

  {
    /*p27.LOVY*/ LOVY_BG_SEQ5_SYNC.set(clk_sig.MYVO_AxCxExGx, ppu_sig.NYXU_BFETCH_RSTn, _LYRY_BFETCH_DONEp);

    /*p24.NAFY*/ wire _NAFY_RENDERING_AND_NOT_WIN_TRIG = nor(win_sig.MOSU_WIN_MODE_TRIGp, ppu_sig.LOBY_RENDERINGn);

    /*p27.LYZU*/ LYZU_BFETCH_S0_DELAY.set       (clk_sig.ALET_xBxDxFxH, ppu_sig.XYMU_RENDERINGp,          LAXU_BFETCH_S0);
    /*p24.NYKA*/ NYKA_BFETCH_DONE_SYNC.set      (clk_sig.ALET_xBxDxFxH, _NAFY_RENDERING_AND_NOT_WIN_TRIG, _LYRY_BFETCH_DONEp);
    /*p24.PORY*/ PORY_BFETCH_DONE_SYNC_DELAY.set(clk_sig.MYVO_AxCxExGx, _NAFY_RENDERING_AND_NOT_WIN_TRIG, NYKA_BFETCH_DONE_SYNC);
  }


  //----------------------------------------
  // So this is def the chunk that watches FEPO_STORE_MATCHp and triggers a sprite fetch...

  // Maybe we should annotate phase starting with the phase 0 = FEPO_MATCH_SYNC goes high?

  {
    /*p27.SOWO*/ wire _SOWO_SPRITE_FETCH_LATCHn = not(TAKA_SFETCH_RUN_LATCH);
    /*p27.TEKY*/ wire _TEKY_SPRITE_FETCH = and (sst_sig.FEPO_STORE_MATCHp, win_sig.TUKU_WIN_HITn, _LYRY_BFETCH_DONEp, _SOWO_SPRITE_FETCH_LATCHn);
    /*p27.SOBU*/ SOBU_SPRITE_FETCH_SYNC1.set(clk_sig.TAVA_xBxDxFxH, dbg_sig.VYPO_P10_Bn, _TEKY_SPRITE_FETCH);
    /*p27.SUDA*/ SUDA_SPRITE_FETCH_SYNC2.set(clk_sig.LAPE_AxCxExGx, dbg_sig.VYPO_P10_Bn, SOBU_SPRITE_FETCH_SYNC1);

    /*p27.RYCE*/ wire _RYCE_SPRITE_FETCH_TRIG = and (SOBU_SPRITE_FETCH_SYNC1, !SUDA_SPRITE_FETCH_SYNC2);
    /*p27.SECA*/ wire _SECA_SFETCH_SETn = nor(_RYCE_SPRITE_FETCH_TRIG, rst_sig.ROSY_VID_RSTp, lcd_sig.BYHA_VID_LINE_TRIG_d4n); // def nor

    /*p27.VEKU*/ wire _VEKU_SFETCH_RSTn = nor(ppu_sig.WUTY_SPRITE_DONE, ppu_sig.TAVE); // def nor
    /*p27.TAKA*/ TAKA_SFETCH_RUN_LATCH.nand_latch(_SECA_SFETCH_SETn, _VEKU_SFETCH_RSTn);

    /*p29.TAME*/ wire _TAME_SFETCH_101n = nand(TESE_SFETCH_S2, TOXE_SFETCH_S0_D0);
    /*p29.TOMA*/ wire _TOMA_xBxDxFxH = nand(clk_sig.LAPE_AxCxExGx, _TAME_SFETCH_101n);

    /*p29.TOXE*/ TOXE_SFETCH_S0_D0.set(_TOMA_xBxDxFxH,  _SECA_SFETCH_SETn, !TOXE_SFETCH_S0_D0);
    /*p29.TULY*/ TULY_SFETCH_S1.set(!TOXE_SFETCH_S0_D0, _SECA_SFETCH_SETn, !TULY_SFETCH_S1);
    /*p29.TESE*/ TESE_SFETCH_S2.set(!TULY_SFETCH_S1,    _SECA_SFETCH_SETn, !TESE_SFETCH_S2);

    /*p29.TYFO*/ TYFO_SFETCH_S0_D1.set(clk_sig.LAPE_AxCxExGx, dbg_sig.VYPO_P10_Bn, TOXE_SFETCH_S0_D0);
    /*p29.TOBU*/ TOBU_SFETCH_S1_D2.set(clk_sig.TAVA_xBxDxFxH, XYMU_RENDERINGp, TULY_SFETCH_S1);    // note input is seq 1 not 2

    /*p29.VONU*/ VONU_SFETCH_S1_D4.set(clk_sig.TAVA_xBxDxFxH, XYMU_RENDERINGp, TOBU_SFETCH_S1_D2);
    /*p29.SEBA*/ SEBA_SFETCH_S1_D5.set(clk_sig.LAPE_AxCxExGx, XYMU_RENDERINGp, VONU_SFETCH_S1_D4); // is this clock wrong?
  }


  //----------

  {
    // vid x position, has carry lookahead because this increments every tcycle
    /*p21.RYBO*/ wire RYBO = xor(XEHO_X0, SAVY_X1);
    /*p21.XUKE*/ wire XUKE = and(XEHO_X0, SAVY_X1);

    /*p21.XYLE*/ wire XYLE = and(XODU_X2, XUKE);
    /*p21.XEGY*/ wire XEGY = xor(XODU_X2, XUKE);
    /*p21.XORA*/ wire XORA = xor(XYDO_X3, XYLE);

    /*p21.SAKE*/ wire SAKE = xor(TUHU_X4, TUKY_X5);
    /*p21.TYBA*/ wire TYBA = and(TUHU_X4, TUKY_X5);
    /*p21.SURY*/ wire SURY = and(TAKO_X6, TYBA);
    /*p21.TYGE*/ wire TYGE = xor(TAKO_X6, TYBA);
    /*p21.ROKU*/ wire ROKU = xor(SYBE_X7, SURY);

    /*p21.TADY*/ wire TADY_X_RSTn = nor(lcd_sig.BYHA_VID_LINE_TRIG_d4n, rst_sig.TOFU_VID_RSTp);

    /*p24.TOCA*/ wire TOCA_CLKPIPE_HI = not(XYDO_X3);

    // My labels are all wrong here, wtf?

    /*p21.XEHO*/ XEHO_X0.set(ppu_sig.SACU_CLKPIPE_AxCxExGx, TADY_X_RSTn, !XEHO_X0);
    /*p21.SAVY*/ SAVY_X1.set(ppu_sig.SACU_CLKPIPE_AxCxExGx, TADY_X_RSTn, RYBO);
    /*p21.XODU*/ XODU_X2.set(ppu_sig.SACU_CLKPIPE_AxCxExGx, TADY_X_RSTn, XEGY);
    /*p21.XYDO*/ XYDO_X3.set(ppu_sig.SACU_CLKPIPE_AxCxExGx, TADY_X_RSTn, XORA);
    /*p21.TUHU*/ TUHU_X4.set(TOCA_CLKPIPE_HI, TADY_X_RSTn, !TUHU_X4);
    /*p21.TUKY*/ TUKY_X5.set(TOCA_CLKPIPE_HI, TADY_X_RSTn, SAKE);
    /*p21.TAKO*/ TAKO_X6.set(TOCA_CLKPIPE_HI, TADY_X_RSTn, TYGE);
    /*p21.SYBE*/ SYBE_X7.set(TOCA_CLKPIPE_HI, TADY_X_RSTn, ROKU);
  }

  {
    /*p21.TADY*/ wire TADY_X_RST = nor(lcd_sig.BYHA_VID_LINE_TRIG_d4n, rst_sig.TOFU_VID_RSTp);
    // having this reset connected to both RENDER_DONE_SYNC and x seems odd
    /*p21.VOGA*/ VOGA_RENDER_DONE_SYNC.set(clk_sig.ALET_xBxDxFxH, TADY_X_RST, ppu_sig.WODU_RENDER_DONE);
  }

  {
    // LCD horizontal sync pin latch
    // if AVAP goes high, POFY goes high.
    // if PAHO or TOFU go high, POFY goes low.

    /*p24.PAHO*/ PAHO_X_8_SYNC.set(ppu_sig.ROXO_AxCxExGx, ppu_sig.XYMU_RENDERINGp, XYDO_X3);
    /*p24.RUJU*/ POFY_ST_LATCH.nor_latch(sst_sig.AVAP_SCAN_DONE_d0_TRIGp, PAHO_X_8_SYNC || rst_sig.TOFU_VID_RSTp);
    /*p24.RUZE*/ wire _RUZE_PIN_ST = not(POFY_ST_LATCH);
    ST.set(_RUZE_PIN_ST);
  }

  /*p27.LUSU*/ wire LUSU_BGW_VRAM_RDn = not(LONY_BG_READ_VRAM_LATCHp.q());
  /*p27.LENA*/ wire LENA_BGW_VRAM_RD = not(LUSU_BGW_VRAM_RDn);

  {
    /*p27.NOGU*/ wire _NOGU_FETCH_01p = nand(ppu_sig.NAKO_FETCH_S1n, ppu_sig.NOFU_FETCH_S2n);
    /*p27.NENY*/ wire _NENY_FETCH_01n = not(_NOGU_FETCH_01p);
    /*p26.AXAD*/ wire _AXAD_WIN_MODEn = not(win_sig.PORE_WIN_MODE);

    {
      /*p27.POTU*/ wire _POTU_BGW_FETCH_01 = and (LENA_BGW_VRAM_RD, _NENY_FETCH_01n);

      // Background map read
      /*p26.ACEN*/ wire _ACEN_BG_MAP_READp = and (_POTU_BGW_FETCH_01, _AXAD_WIN_MODEn);
      /*p26.BAFY*/ wire _BAFY_BG_MAP_READn = not(_ACEN_BG_MAP_READp);
      /*p26.AXEP*/ vram_bus.TS_MA00.set_tribuf(_BAFY_BG_MAP_READn, scr_sig._MAP_X0S);
      /*p26.AFEB*/ vram_bus.TS_MA01.set_tribuf(_BAFY_BG_MAP_READn, scr_sig._MAP_X1S);
      /*p26.ALEL*/ vram_bus.TS_MA02.set_tribuf(_BAFY_BG_MAP_READn, scr_sig._MAP_X2S);
      /*p26.COLY*/ vram_bus.TS_MA03.set_tribuf(_BAFY_BG_MAP_READn, scr_sig._MAP_X3S);
      /*p26.AJAN*/ vram_bus.TS_MA04.set_tribuf(_BAFY_BG_MAP_READn, scr_sig._MAP_X4S);
      /*p26.DUHO*/ vram_bus.TS_MA05.set_tribuf(_BAFY_BG_MAP_READn, scr_sig._MAP_Y0S);
      /*p26.CASE*/ vram_bus.TS_MA06.set_tribuf(_BAFY_BG_MAP_READn, scr_sig._MAP_Y1S);
      /*p26.CYPO*/ vram_bus.TS_MA07.set_tribuf(_BAFY_BG_MAP_READn, scr_sig._MAP_Y2S);
      /*p26.CETA*/ vram_bus.TS_MA08.set_tribuf(_BAFY_BG_MAP_READn, scr_sig._MAP_Y3S);
      /*p26.DAFE*/ vram_bus.TS_MA09.set_tribuf(_BAFY_BG_MAP_READn, scr_sig._MAP_Y4S);
      /*p26.AMUV*/ vram_bus.TS_MA10.set_tribuf(_BAFY_BG_MAP_READn, cfg_reg.LCDC_BGMAP.q());
      /*p26.COVE*/ vram_bus.TS_MA11.set_tribuf(_BAFY_BG_MAP_READn, dbg_sig.VYPO_P10_Bn);
      /*p26.COXO*/ vram_bus.TS_MA12.set_tribuf(_BAFY_BG_MAP_READn, dbg_sig.VYPO_P10_Bn);

      // Window map read
      /*p25.XEZE*/ wire _XEZE_WIN_MAP_READp = and (_POTU_BGW_FETCH_01, win_sig.PORE_WIN_MODE);
      /*p25.WUKO*/ wire _WUKO_WIN_MAP_READn = not(_XEZE_WIN_MAP_READp);
      /*p27.XEJA*/ vram_bus.TS_MA00.set_tribuf(_WUKO_WIN_MAP_READn, win_reg.WIN_X3.q());
      /*p27.XAMO*/ vram_bus.TS_MA01.set_tribuf(_WUKO_WIN_MAP_READn, win_reg.WIN_X4.q());
      /*p27.XAHE*/ vram_bus.TS_MA02.set_tribuf(_WUKO_WIN_MAP_READn, win_reg.WIN_X5.q());
      /*p27.XULO*/ vram_bus.TS_MA03.set_tribuf(_WUKO_WIN_MAP_READn, win_reg.WIN_X6.q());
      /*p27.WUJU*/ vram_bus.TS_MA04.set_tribuf(_WUKO_WIN_MAP_READn, win_reg.WIN_X7.q());
      /*p27.VYTO*/ vram_bus.TS_MA05.set_tribuf(_WUKO_WIN_MAP_READn, win_reg.WIN_Y3.q());
      /*p27.VEHA*/ vram_bus.TS_MA06.set_tribuf(_WUKO_WIN_MAP_READn, win_reg.WIN_Y4.q());
      /*p27.VACE*/ vram_bus.TS_MA07.set_tribuf(_WUKO_WIN_MAP_READn, win_reg.WIN_Y5.q());
      /*p27.VOVO*/ vram_bus.TS_MA08.set_tribuf(_WUKO_WIN_MAP_READn, win_reg.WIN_Y6.q());
      /*p27.VULO*/ vram_bus.TS_MA09.set_tribuf(_WUKO_WIN_MAP_READn, win_reg.WIN_Y7.q());
      /*p27.VEVY*/ vram_bus.TS_MA10.set_tribuf(_WUKO_WIN_MAP_READn, cfg_reg.LCDC_WINMAP.q());
      /*p27.VEZA*/ vram_bus.TS_MA11.set_tribuf(_WUKO_WIN_MAP_READn, dbg_sig.VYPO_P10_Bn);
      /*p27.VOGU*/ vram_bus.TS_MA12.set_tribuf(_WUKO_WIN_MAP_READn, dbg_sig.VYPO_P10_Bn);
    }

    {
      // Background/window tile read
      /*p27.XUHA*/ wire _XUHA_FETCH_S2p = not(ppu_sig.NOFU_FETCH_S2n);
      /*p27.NETA*/ wire _NETA_TILE_READn = and (LENA_BGW_VRAM_RD, _NENY_FETCH_01n);
      /*p26.ASUL*/ wire _ASUL_TILE_READp = and (_NETA_TILE_READn, _AXAD_WIN_MODEn);
      /*p26.BEJE*/ wire _BEJE_TILE_READn = not(_ASUL_TILE_READp);
      /*p25.XUCY*/ wire _XUCY_TILE_READn = nand(_NETA_TILE_READn, win_sig.PORE_WIN_MODE);
      /*p25.VUZA*/ wire _VUZA_TILE_BANKp = nor(cfg_reg.LCDC_BGTILE.q(), pxp_reg.BG_PIX_B7.q()); // register reused

      /*p26.ASUM*/ vram_bus.TS_MA00.set_tribuf(_BEJE_TILE_READn, _XUHA_FETCH_S2p);
      /*p26.EVAD*/ vram_bus.TS_MA01.set_tribuf(_BEJE_TILE_READn, scr_sig._TILE_Y0S);
      /*p26.DAHU*/ vram_bus.TS_MA02.set_tribuf(_BEJE_TILE_READn, scr_sig._TILE_Y1S);
      /*p26.DODE*/ vram_bus.TS_MA03.set_tribuf(_BEJE_TILE_READn, scr_sig._TILE_Y2S);

      /*p25.XONU*/ vram_bus.TS_MA00.set_tribuf(_XUCY_TILE_READn, _XUHA_FETCH_S2p);
      /*p25.WUDO*/ vram_bus.TS_MA01.set_tribuf(_XUCY_TILE_READn, win_reg.WIN_Y0.q());
      /*p25.WAWE*/ vram_bus.TS_MA02.set_tribuf(_XUCY_TILE_READn, win_reg.WIN_Y1.q());
      /*p25.WOLU*/ vram_bus.TS_MA03.set_tribuf(_XUCY_TILE_READn, win_reg.WIN_Y2.q());

      /*p25.VAPY*/ vram_bus.TS_MA04.set_tribuf(_NETA_TILE_READn, pxp_reg.BG_PIX_B0.q()); // register reused
      /*p25.SEZU*/ vram_bus.TS_MA05.set_tribuf(_NETA_TILE_READn, pxp_reg.BG_PIX_B1.q());
      /*p25.VEJY*/ vram_bus.TS_MA06.set_tribuf(_NETA_TILE_READn, pxp_reg.BG_PIX_B2.q());
      /*p25.RUSA*/ vram_bus.TS_MA07.set_tribuf(_NETA_TILE_READn, pxp_reg.BG_PIX_B3.q());
      /*p25.ROHA*/ vram_bus.TS_MA08.set_tribuf(_NETA_TILE_READn, pxp_reg.BG_PIX_B4.q());
      /*p25.RESO*/ vram_bus.TS_MA09.set_tribuf(_NETA_TILE_READn, pxp_reg.BG_PIX_B5.q());
      /*p25.SUVO*/ vram_bus.TS_MA10.set_tribuf(_NETA_TILE_READn, pxp_reg.BG_PIX_B6.q());
      /*p25.TOBO*/ vram_bus.TS_MA11.set_tribuf(_NETA_TILE_READn, pxp_reg.BG_PIX_B7.q());
      /*p25.VURY*/ vram_bus.TS_MA12.set_tribuf(_NETA_TILE_READn, _VUZA_TILE_BANKp);
    }
  }

  /*p29.ABON*/ wire ABON_SPR_VRAM_RDp1 = not(ppu_sig.TEXY_SPRITE_READ);

  {
    // TS_LINE_* out of order
    // why is there a ! on REG_OAM_B0?

    /*p29.FUFO*/ wire _FUFO_LCDC_SPSIZEn = not(cfg_reg.LCDC_SPSIZE.q());
    /*p29.WUKY*/ wire _WUKY_FLIP_Y = not(oam_reg.YZOS_SPRITE_X6.q());

    /*p29.WAGO*/ wire _WAGO = xor (_WUKY_FLIP_Y, sst_reg.WENU_TS_LINE_0);
    /*p29.GEJY*/ wire _GEJY_SPRITE_Y3 = amux2(_FUFO_LCDC_SPSIZEn, !oam_reg.XUSO_SPRITE_Y0.q(), cfg_reg.LCDC_SPSIZE.q(), _WAGO);
    /*p29.XUQU*/ wire _XUQU_SPRITE_AB = not(!ppu_reg.VONU_SFETCH_S1_D4.q());
    /*p29.CYVU*/ wire _CYVU_SPRITE_Y0 = xor (_WUKY_FLIP_Y, sst_reg.CUCU_TS_LINE_1);
    /*p29.BORE*/ wire _BORE_SPRITE_Y1 = xor (_WUKY_FLIP_Y, sst_reg.CUCA_TS_LINE_2);
    /*p29.BUVY*/ wire _BUVY_SPRITE_Y2 = xor (_WUKY_FLIP_Y, sst_reg.CEGA_TS_LINE_3);


    /*p29.ABEM*/ vram_bus.TS_MA00.set_tribuf(!ABON_SPR_VRAM_RDp1, _XUQU_SPRITE_AB);
    /*p29.BAXE*/ vram_bus.TS_MA01.set_tribuf(!ABON_SPR_VRAM_RDp1, _CYVU_SPRITE_Y0);
    /*p29.ARAS*/ vram_bus.TS_MA02.set_tribuf(!ABON_SPR_VRAM_RDp1, _BORE_SPRITE_Y1);
    /*p29.AGAG*/ vram_bus.TS_MA03.set_tribuf(!ABON_SPR_VRAM_RDp1, _BUVY_SPRITE_Y2);
    /*p29.FAMU*/ vram_bus.TS_MA04.set_tribuf(!ABON_SPR_VRAM_RDp1, _GEJY_SPRITE_Y3);
    /*p29.FUGY*/ vram_bus.TS_MA05.set_tribuf(!ABON_SPR_VRAM_RDp1, oam_reg.XEGU_SPRITE_Y1.q());
    /*p29.GAVO*/ vram_bus.TS_MA06.set_tribuf(!ABON_SPR_VRAM_RDp1, oam_reg.YJEX_SPRITE_Y2.q());
    /*p29.WYGA*/ vram_bus.TS_MA07.set_tribuf(!ABON_SPR_VRAM_RDp1, oam_reg.XYJU_SPRITE_Y3.q());
    /*p29.WUNE*/ vram_bus.TS_MA08.set_tribuf(!ABON_SPR_VRAM_RDp1, oam_reg.YBOG_SPRITE_Y4.q());
    /*p29.GOTU*/ vram_bus.TS_MA09.set_tribuf(!ABON_SPR_VRAM_RDp1, oam_reg.WYSO_SPRITE_Y5.q());
    /*p29.GEGU*/ vram_bus.TS_MA10.set_tribuf(!ABON_SPR_VRAM_RDp1, oam_reg.XOTE_SPRITE_Y6.q());
    /*p29.XEHE*/ vram_bus.TS_MA11.set_tribuf(!ABON_SPR_VRAM_RDp1, oam_reg.YZAB_SPRITE_Y7.q());
    /*p29.DYSO*/ vram_bus.TS_MA12.set_tribuf(!ABON_SPR_VRAM_RDp1, joy_pin.P10_B);   // sprites always in low half of tile store
  }


}

//------------------------------------------------------------------------------

bool PpuRegisters::commit() {
  bool changed = false;
  /*p??.ROXY*/ changed |= ROXY_FINE_MATCH_LATCHn.commit_latch();
  /*p??.PUXA*/ changed |= PUXA_FINE_MATCH_SYNC1.commit_reg();
  /*p27.NYZE*/ changed |= NYZE_FINE_MATCH_SYNC2.commit_reg();
  /*p27.RYKU*/ changed |= RYKU_FINE_CNT0.commit_reg();
  /*p27.ROGA*/ changed |= ROGA_FINE_CNT1.commit_reg();
  /*p27.RUBU*/ changed |= RUBU_FINE_CNT2.commit_reg();
  /*p21.XEHO*/ changed |= XEHO_X0.commit_reg();
  /*p21.SAVY*/ changed |= SAVY_X1.commit_reg();
  /*p21.XODU*/ changed |= XODU_X2.commit_reg();
  /*p21.XYDO*/ changed |= XYDO_X3.commit_reg();
  /*p21.TUHU*/ changed |= TUHU_X4.commit_reg();
  /*p21.TUKY*/ changed |= TUKY_X5.commit_reg();
  /*p21.TAKO*/ changed |= TAKO_X6.commit_reg();
  /*p21.SYBE*/ changed |= SYBE_X7.commit_reg();
  /*p21.XYMU*/ changed |= XYMU_RENDERINGp.commit_latch();
  /*p21.VOGA*/ changed |= VOGA_RENDER_DONE_SYNC.commit_reg();
  /*p21.ROXE*/ changed |= ROXE_INT_HBL_EN.commit_reg();
  /*p21.RUFO*/ changed |= RUFO_INT_VBL_EN.commit_reg();
  /*p21.REFE*/ changed |= REFE_INT_OAM_EN.commit_reg();
  /*p21.RUGU*/ changed |= RUGU_INT_LYC_EN.commit_reg();
  /*p27.LONY*/ changed |= LONY_BG_READ_VRAM_LATCHp.commit_latch();
  /*p27.LAXU*/ changed |= LAXU_BFETCH_S0.commit_reg();
  /*p27.MESU*/ changed |= MESU_BFETCH_S1.commit_reg();
  /*p27.NYVA*/ changed |= NYVA_BFETCH_S2.commit_reg();
  /*p27.LOVY*/ changed |= LOVY_BG_SEQ5_SYNC.commit_reg();
  /*p24.NYKA*/ changed |= NYKA_BFETCH_DONE_SYNC.commit_reg();
  /*p24.PORY*/ changed |= PORY_BFETCH_DONE_SYNC_DELAY.commit_reg();
  /*p27.LYZU*/ changed |= LYZU_BFETCH_S0_DELAY.commit_reg();
  /*p24.PYGO*/ changed |= PYGO_TILE_DONE.commit_reg();
  /*p24.POKY*/ changed |= POKY_FRONT_PORCH_LATCH.commit_latch();

  /*p27.TAKA*/ changed |= TAKA_SFETCH_RUN_LATCH.commit_latch();
  /*p27.SOBU*/ changed |= SOBU_SPRITE_FETCH_SYNC1.commit_reg();
  /*p27.SUDA*/ changed |= SUDA_SPRITE_FETCH_SYNC2.commit_reg();


  /*p24.PAHO*/ changed |= PAHO_X_8_SYNC.commit_reg();
  /*p24.RUJU*/ changed |= POFY_ST_LATCH.commit_latch(); // nor latch with p24.RUJU, p24.POME
  /*p21.WUSA*/ changed |= WUSA_CPEN_LATCH.commit_latch();
  /* PIN_53 */ changed |= CP.commit_pinout();
  /* PIN_54 */ changed |= ST.commit_pinout();
  return changed;
}

//------------------------------------------------------------------------------

//dump_long(text_painter, "PAHO_X_8_SYNC ", PAHO_X_8_SYNC.a);
//dump_long(text_painter, "WUSA_CPEN_LATCH    ", WUSA_CPEN_LATCH.a);
//dump_long(text_painter, "POFY_ST_LATCH ", POFY_ST_LATCH.a);
    //CP.dump(text_painter, "CP  ");
    //ST.dump(text_painter, "ST  ");

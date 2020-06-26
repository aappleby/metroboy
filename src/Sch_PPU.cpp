#include "Sch_Merged.h"

#include "Sch_Common.h"
#include "Sch_Pins.h"
#include "TestGB.h"
#include "Constants.h"

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

//------------------------------------------------------------------------------

PpuSignals PpuRegisters::sig(const TestGB& gb) const {
  auto win_sig = gb.win_reg.sig(gb);

  auto& sst_reg = gb.sst_reg;
  auto sst_sig = sst_reg.sig(gb);
  auto clk_sig = gb.clk_reg.sig(gb);
  auto rst_sig = gb.rst_reg.sig(gb);
  auto dma_sig = gb.dma_reg.sig(gb);
  auto adr_sig = gb.adr_reg.sig(gb.cpu_pins);

  auto& vram_pins = gb.vram_pins;
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

  /*p28.ACYL*/ wire _ACYL_PPU_USE_OAM1p = and (dma_sig.BOGE_DMA_RUNNINGn, sst_reg.BESU_SCANNINGp);

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
  /*p25.TEFY*/ wire _TEFY_MCS_Cn = not(vram_pins.MCS_C);
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

void PpuRegisters::tick(const TestGB& gb) {
  auto clk_sig = gb.clk_reg.sig(gb);
  auto rst_sig = gb.rst_reg.sig(gb);
  auto win_sig = gb.win_reg.sig(gb);
  auto lcd_sig = gb.lcd_reg.sig(gb);
  auto dbg_sig = gb.dbg_reg.sig(gb);
  auto sst_sig = gb.sst_reg.sig(gb);
  auto ppu_sig = sig(gb);

  auto& cfg_reg = gb.cfg_reg;

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

#include "Sch_Merged.h"

#include "Sch_Common.h"
#include "Sch_Pins.h"
#include "TestGB.h"
#include "Constants.h"

#pragma warning(disable:4458)

#define HAX 1
#undef HAX

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

//------------------------------------------------------------------------------

PpuSignals PpuRegisters::sig(const TestGB& gb) const {
  auto win_sig = gb.win_reg.sig(gb);

  auto& sst_reg = gb.sst_reg;
  auto sst_sig = sst_reg.sig(gb);
  auto& dma_reg = gb.dma_reg;
  auto clk_sig = gb.clk_reg.sig(gb);
  auto rst_sig = gb.rst_reg.sig(gb);

  /*p21.XUGU*/ wire XUGU_X_167n = nand(SAXO_X0.q(), TYPO_X1.q(), VYZO_X2.q(), TAHA_X5.q(), SYBE_X7.q()); // 128 + 32 + 4 + 2 + 1 = 167
  /*p21.XANO*/ wire XANO_X_167 = not(XUGU_X_167n);
  /*p21.XENA*/ wire XENA_STORE_MATCHn = not(sst_sig.FEPO_STORE_MATCHp);
  /*p21.WODU*/ wire WODU_RENDER_DONE = and (XENA_STORE_MATCHn, XANO_X_167);

  /*p27.ROMO*/ wire FRONT_PORCH = not(POKY_FRONT_PORCH_LATCH);
  /*p27.SUVU*/ wire SUVU = nand(XYMU_RENDERINGp, FRONT_PORCH, NYKA_BFETCH_DONE_SYNC, PORY_BFETCH_DONE_SYNC_DELAY);
  /*p27.TAVE*/ wire TAVE = not(SUVU);

  /*p27.TEVO*/ wire TEVO_CLK_STOPn = nor(win_sig.SEKO_WIN_TRIGGER, win_sig.SUZU, TAVE);

  /*p27.NYXU*/ wire NYXU_BFETCH_RSTn = nor(sst_sig.AVAP_SCAN_DONE_d0_TRIGp, win_sig.MOSU_WIN_MODE_TRIGp, TEVO_CLK_STOPn);

  /*p28.BOGE*/ wire DMA_RUNNINGn = not(dma_reg.MATU_DMA_OAM_WRp.q());
  /*p28.ACYL*/ wire ACYL_PPU_USE_OAM1p = and (DMA_RUNNINGn, sst_reg.BESU_SCANNINGp);

  // So we only read a sprite if both those regs are... low? what is rung 17's polarity?

  /*p29.SAKY*/ wire SAKY = nor(TULY_SFETCH_S1.q(), VONU_SFETCH_S1_D4.q());
  /*p29.TEPA*/ wire TEPA_RENDERINGn = not(XYMU_RENDERINGp);
  /*p29.TYSO*/ wire TYSO_SPRITE_READn = or (SAKY, TEPA_RENDERINGn);
  /*p29.TEXY*/ wire TEXY_SPRITE_READ = not(TYSO_SPRITE_READn);

  /*p29.TYNO*/ wire TYNO = nand(TOXE_SFETCH_S0_D0.q(), SEBA_SFETCH_S1_D5.q(), VONU_SFETCH_S1_D4.q());
  /*p29.VUSA*/ wire VUSA = or (!TYFO_SFETCH_S0_D1.q(), TYNO);
  /*p29.WUTY*/ wire WUTY_SPRITE_DONE = not(VUSA);

  /*p24.VYBO*/ wire VYBO_PIX_CLK_AxCxExGx = nor(sst_sig.FEPO_STORE_MATCHp, WODU_RENDER_DONE, clk_sig.MYVO_AxCxExGx);
  /*p24.TYFA*/ wire TYFA_AxCxExGx = and (win_sig.SOCY_WIN_HITn, POKY_FRONT_PORCH_LATCH, VYBO_PIX_CLK_AxCxExGx);
  /*p24.SEGU*/ wire SEGU_xBxDxFxH = not(TYFA_AxCxExGx);
  /*p24.ROXO*/ wire ROXO_AxCxExGx = not(SEGU_xBxDxFxH);
  /*p27.ROCO*/ wire ROCO_AxCxExGx = not(SEGU_xBxDxFxH);
  /*p24.SACU*/ wire SACU_CLKPIPE_AxCxExGx = nor(SEGU_xBxDxFxH, ROXY_FINE_MATCH_LATCHn);

  return {
    .TEVO_CLK_STOPn = TEVO_CLK_STOPn,
    .WODU_RENDER_DONE = WODU_RENDER_DONE,
    .NYXU_BFETCH_RSTn = NYXU_BFETCH_RSTn,
    .TEXY_SPRITE_READ = TEXY_SPRITE_READ,
    .WUTY_SPRITE_DONE = WUTY_SPRITE_DONE,
    .ACYL_PPU_USE_OAM1p = ACYL_PPU_USE_OAM1p,
    .POKY_FRONT_PORCH_LATCH = POKY_FRONT_PORCH_LATCH,
    .XYMU_RENDERINGp = XYMU_RENDERINGp,
    .SEGU_xBxDxFxH = SEGU_xBxDxFxH,
    .ROXO_AxCxExGx = ROXO_AxCxExGx,
    .ROCO_AxCxExGx = ROCO_AxCxExGx,
    .SACU_CLKPIPE_AxCxExGx = SACU_CLKPIPE_AxCxExGx,
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
  auto& sst_reg = gb.sst_reg;

  /*p24.LOBY*/ wire LOBY_RENDERINGn = not(XYMU_RENDERINGp);

  /*p28.ANOM*/ wire ANOM_SCAN_RSTn = nor(lcd_sig.ATEJ_VID_LINE_TRIG_d4p, rst_sig.ATAR_VID_RSTp);
  /*p29.BALU*/ wire BALU_SCAN_RST = not(ANOM_SCAN_RSTn);
  /*p29.BEBU*/ wire SCAN_DONE_d0_TRIGn = or (BALU_SCAN_RST, sst_reg.SCAN_DONE_d5.q(), !sst_reg.SCAN_DONE_d4.q());
  /*p29.AVAP*/ wire AVAP_SCAN_DONE_d0_TRIGp = not(SCAN_DONE_d0_TRIGn);

  /*p25.ROPY*/ wire ROPY_RENDERINGn = not(XYMU_RENDERINGp);
  /*p27.MOXE*/ wire _MOXE_AxCxExGx = not(clk_sig.ALET_xBxDxFxH);

  /*p27.MOCE*/ wire MOCE_BFETCH_DONEn = nand(LAXU_BFETCH_S0, NYVA_BFETCH_S2, ppu_sig.NYXU_BFETCH_RSTn);
  /*p27.LYRY*/ wire LYRY_BFETCH_DONEp = not(MOCE_BFETCH_DONEn);

  // The first tile generated is thrown away. I'm calling that section of rendering the front porch.

  {
    // NOR latch
    // POKY00 << PYGO
    // POKY01 nc
    // POKY02 >> nc
    // POKY03 >> ROMO00, others
    // POKY04 nc
    // POKY05 << LOBY01

    // If PYGO goes high, POKY03 goes high
    // if LOBY goes high, POKY03 goes low.

    /*p24.PYGO*/ PYGO_TILE_DONE.set(clk_sig.ALET_xBxDxFxH, XYMU_RENDERINGp, PORY_BFETCH_DONE_SYNC_DELAY);
    /*p24.POKY*/ POKY_FRONT_PORCH_LATCH.nor_latch(PYGO_TILE_DONE, LOBY_RENDERINGn);
  }


  {
    /*p27.ROZE*/ wire FINE_COUNT_STOPn = nand(RYKU_FINE_CNT0, ROGA_FINE_CNT1, RUBU_FINE_CNT2);
    /*p27.PECU*/ wire FINE_CLK = nand(ppu_sig.ROXO_AxCxExGx, FINE_COUNT_STOPn);
    /*p27.PASO*/ wire FINE_RST = nor(ppu_sig.TEVO_CLK_STOPn, ROPY_RENDERINGn);
    /*p27.RYKU*/ RYKU_FINE_CNT0.set(FINE_CLK, FINE_RST, !RYKU_FINE_CNT0);
    /*p27.ROGA*/ ROGA_FINE_CNT1.set(!RYKU_FINE_CNT0, FINE_RST, !ROGA_FINE_CNT1);
    /*p27.RUBU*/ RUBU_FINE_CNT2.set(!ROGA_FINE_CNT1, FINE_RST, !RUBU_FINE_CNT2);

    // Arms on the ground side, XNOR
    /*p27.SUHA*/ wire SUHA_FINE_MATCH0 = xnor(cfg_reg.SCX0, RYKU_FINE_CNT0);
    /*p27.SYBY*/ wire SYBY_FINE_MATCH1 = xnor(cfg_reg.SCX1, ROGA_FINE_CNT1);
    /*p27.SOZU*/ wire SOZU_FINE_MATCH2 = xnor(cfg_reg.SCX2, RUBU_FINE_CNT2);

    /*p27.RONE*/ wire RONE_FINE_MATCHn = nand(ROXY_FINE_MATCH_LATCHn, SUHA_FINE_MATCH0, SYBY_FINE_MATCH1, SOZU_FINE_MATCH2);
    /*p27.POHU*/ wire POHU_FINE_MATCH = not(RONE_FINE_MATCHn);

    /*p27.PUXA*/ PUXA_FINE_MATCH_SYNC1.set(ppu_sig.ROXO_AxCxExGx, XYMU_RENDERINGp, POHU_FINE_MATCH);

    /*p27.NYZE*/ NYZE_FINE_MATCH_SYNC2.set(_MOXE_AxCxExGx, XYMU_RENDERINGp, PUXA_FINE_MATCH_SYNC1);

    /*p27.POVA*/ wire POVA_FINE_MATCH_SETp = and (PUXA_FINE_MATCH_SYNC1, !NYZE_FINE_MATCH_SYNC2);
    /*p27.PAHA*/ wire PAHA_FINE_MATCH_RSTp = not(XYMU_RENDERINGp);
    /*p27.ROXY*/ ROXY_FINE_MATCH_LATCHn.nor_latch(PAHA_FINE_MATCH_RSTp, POVA_FINE_MATCH_SETp);
  }


  //----------

  {
    // Original

    {
      /*p27.LEBO*/ wire LEBO_BFETCH_CLKp  = nand(clk_sig.ALET_xBxDxFxH, MOCE_BFETCH_DONEn);

      /*p27.LAXU*/ LAXU_BFETCH_S0.set(LEBO_BFETCH_CLKp,     ppu_sig.NYXU_BFETCH_RSTn, !LAXU_BFETCH_S0);
      /*p27.MESU*/ MESU_BFETCH_S1.set(!LAXU_BFETCH_S0,      ppu_sig.NYXU_BFETCH_RSTn, !MESU_BFETCH_S1);
      /*p27.NYVA*/ NYVA_BFETCH_S2.set(!MESU_BFETCH_S1,      ppu_sig.NYXU_BFETCH_RSTn, !NYVA_BFETCH_S2);

      /*p27.LOVY*/ LOVY_BG_SEQ5_SYNC.set(clk_sig.MYVO_AxCxExGx, ppu_sig.NYXU_BFETCH_RSTn, LYRY_BFETCH_DONEp);
    }

    {
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

      // schematic - lury = and(lovy.qn, xymu)

      // this seems reasonable. BG_READ_VRAM_LATCHp is bracketed by BFETCH_RSTn (start) and
      // vid_reg.LOVY_BG_SEQ5_SYNC (stop).

      /*p27.LURY*/ wire LURY = and(!LOVY_BG_SEQ5_SYNC, XYMU_RENDERINGp);
      /*p27.LONY*/ LONY_BG_READ_VRAM_LATCHp.nand_latch(ppu_sig.NYXU_BFETCH_RSTn, LURY);
    }

    {
      /*p24.NAFY*/ wire NAFY_RENDERING_AND_NOT_WIN_TRIG = nor(win_sig.MOSU_WIN_MODE_TRIGp, LOBY_RENDERINGn);

      /*p27.LYZU*/ LYZU_BFETCH_S0_DELAY.set       (clk_sig.ALET_xBxDxFxH, XYMU_RENDERINGp,         LAXU_BFETCH_S0);
      /*p24.NYKA*/ NYKA_BFETCH_DONE_SYNC.set      (clk_sig.ALET_xBxDxFxH, NAFY_RENDERING_AND_NOT_WIN_TRIG, LYRY_BFETCH_DONEp);
      /*p24.PORY*/ PORY_BFETCH_DONE_SYNC_DELAY.set(clk_sig.MYVO_AxCxExGx, NAFY_RENDERING_AND_NOT_WIN_TRIG, NYKA_BFETCH_DONE_SYNC);
    }

    //--------------------------------------------------------------------------------
    // So this is def the chunk that watches FEPO_STORE_MATCHp and triggers a sprite fetch...
    // TAKA has arms on the VCC side - nand latch
    // TAKA01 << VEKU02
    // TAKA02 nc
    // TAKA03 >> nc
    // TAKA04 >> SOWO00
    // TAKA05 nc
    // TAKA06 << SECA03
    // if SECA03 goes low, TAKA04 goes high
    // if VEKU02 goes low, TAKA04 goes low

    // Maybe we should annotate phase starting with the phase 0 = FEPO_MATCH_SYNC goes high?

    {
      /*p27.ROMO*/ wire FRONT_PORCH = not(POKY_FRONT_PORCH_LATCH);
      /*p27.SUVU*/ wire SUVU = nand(XYMU_RENDERINGp, FRONT_PORCH, NYKA_BFETCH_DONE_SYNC, PORY_BFETCH_DONE_SYNC_DELAY);
      /*p27.TAVE*/ wire TAVE = not(SUVU);
      /*p27.RYCE*/ wire RYCE_SPRITE_FETCH_TRIG = and(SOBU_SPRITE_FETCH_SYNC1, !SUDA_SPRITE_FETCH_SYNC2);

      /*p27.SECA*/ wire SECA_SFETCH_SETn = nor(RYCE_SPRITE_FETCH_TRIG, rst_sig.ROSY_VID_RSTp, lcd_sig.BYHA_VID_LINE_TRIG_d4n); // def nor
      /*p27.VEKU*/ wire VEKU_SFETCH_RSTn = nor(ppu_sig.WUTY_SPRITE_DONE, TAVE); // def nor

      /*p27.TAKA*/ TAKA_SFETCH_RUN_LATCH.nand_latch(SECA_SFETCH_SETn, VEKU_SFETCH_RSTn);
      /*p27.SOWO*/ wire SOWO_SPRITE_FETCH_LATCHn = not(TAKA_SFETCH_RUN_LATCH);

      /*p27.TEKY*/ wire TEKY_SPRITE_FETCH = and(sst_sig.FEPO_STORE_MATCHp, win_sig.TUKU_WIN_HITn, LYRY_BFETCH_DONEp, SOWO_SPRITE_FETCH_LATCHn);

      /*p27.SOBU*/ SOBU_SPRITE_FETCH_SYNC1.set(clk_sig.TAVA_xBxDxFxH, dbg_sig.VYPO_P10_Bn, TEKY_SPRITE_FETCH);
      /*p27.SUDA*/ SUDA_SPRITE_FETCH_SYNC2.set(clk_sig.LAPE_AxCxExGx, dbg_sig.VYPO_P10_Bn, SOBU_SPRITE_FETCH_SYNC1);
    }
    {
      /*p27.RYCE*/ wire RYCE_SPRITE_FETCH_TRIG = and(SOBU_SPRITE_FETCH_SYNC1, !SUDA_SPRITE_FETCH_SYNC2);
      /*p27.SECA*/ wire SECA_SFETCH_RSTn = nor(RYCE_SPRITE_FETCH_TRIG, rst_sig.ROSY_VID_RSTp, lcd_sig.BYHA_VID_LINE_TRIG_d4n);
      /*p29.TAME*/ wire TAME_SFETCH_101n = nand(TESE_SFETCH_S2, TOXE_SFETCH_S0_D0);

      /*p29.TOMA*/ wire TOMA_xBxDxFxH = nand(clk_sig.LAPE_AxCxExGx, TAME_SFETCH_101n);

      /*p29.TOXE*/ TOXE_SFETCH_S0_D0.set(TOMA_xBxDxFxH,           SECA_SFETCH_RSTn, !TOXE_SFETCH_S0_D0);
      /*p29.TULY*/ TULY_SFETCH_S1.set(!TOXE_SFETCH_S0_D0, SECA_SFETCH_RSTn, !TULY_SFETCH_S1);
      /*p29.TESE*/ TESE_SFETCH_S2.set(!TULY_SFETCH_S1, SECA_SFETCH_RSTn, !TESE_SFETCH_S2);

      /*p29.TYFO*/ TYFO_SFETCH_S0_D1.set(clk_sig.LAPE_AxCxExGx, dbg_sig.VYPO_P10_Bn, TOXE_SFETCH_S0_D0);
      /*p29.TOBU*/ TOBU_SFETCH_S1_D2.set(clk_sig.TAVA_xBxDxFxH, XYMU_RENDERINGp, TULY_SFETCH_S1);    // note input is seq 1 not 2

      /*p29.VONU*/ VONU_SFETCH_S1_D4.set(clk_sig.TAVA_xBxDxFxH, XYMU_RENDERINGp, TOBU_SFETCH_S1_D2);
      /*p29.SEBA*/ SEBA_SFETCH_S1_D5.set(clk_sig.LAPE_AxCxExGx, XYMU_RENDERINGp, VONU_SFETCH_S1_D4); // is this clock wrong?
    }
  }


  {
    // XYMU has "arms" on the ground side
    // XYMU00 << WEGO03
    // XYMU01 nc
    // XYMU02 >> bunch of stuff
    // XYMU03 >> nc
    // XYMU04 nc
    // XYMU05 << AVAP02

    // if AVAP02 goes high, XYMU02 goes high.
    // if WEGO03 goes high, XYMU02 goes low.

    /*p21.WEGO*/ wire WEGO_RST_LATCH = or (rst_sig.TOFU_VID_RSTp, VOGA_RENDER_DONE_SYNC);
    /*p21.XYMU*/ XYMU_RENDERINGp.nor_latch(AVAP_SCAN_DONE_d0_TRIGp, WEGO_RST_LATCH);
  }

  //----------



  {
    /*p21.TADY*/ wire TADY_X_RST = nor(lcd_sig.BYHA_VID_LINE_TRIG_d4n, rst_sig.TOFU_VID_RSTp);
    // having this reset connected to both RENDER_DONE_SYNC and x seems odd
    /*p21.VOGA*/ VOGA_RENDER_DONE_SYNC.set(clk_sig.ALET_xBxDxFxH, TADY_X_RST, ppu_sig.WODU_RENDER_DONE);
  }

  // vid x position, has carry lookahead because this increments every tcycle

  /*p21.RYBO*/ wire RYBO = xor(SAXO_X0, TYPO_X1);
  /*p21.XUKE*/ wire XUKE = and(SAXO_X0, TYPO_X1);

  /*p21.XYLE*/ wire XYLE = and(VYZO_X2, XUKE);
  /*p21.XEGY*/ wire XEGY = xor(VYZO_X2, XUKE);
  /*p21.XORA*/ wire XORA = xor(TELU_X3, XYLE);

  /*p21.SAKE*/ wire SAKE = xor(SUDE_X4, TAHA_X5);
  /*p21.TYBA*/ wire TYBA = and(SUDE_X4, TAHA_X5);
  /*p21.SURY*/ wire SURY = and(TYRY_X6, TYBA);
  /*p21.TYGE*/ wire TYGE = xor(TYRY_X6, TYBA);
  /*p21.ROKU*/ wire ROKU = xor(SYBE_X7, SURY);

  /*p21.TADY*/ wire TADY_X_RSTn = nor(lcd_sig.BYHA_VID_LINE_TRIG_d4n, rst_sig.TOFU_VID_RSTp);

  /*p24.TOCA*/ wire TOCA_CLKPIPE_HI = not(TELU_X3);

  // My labels are all wrong here, wtf?

  /*p21.XEHO*/ SAXO_X0.set(ppu_sig.SACU_CLKPIPE_AxCxExGx, TADY_X_RSTn, !SAXO_X0);
  /*p21.SAVY*/ TYPO_X1.set(ppu_sig.SACU_CLKPIPE_AxCxExGx, TADY_X_RSTn, RYBO);
  /*p21.XODU*/ VYZO_X2.set(ppu_sig.SACU_CLKPIPE_AxCxExGx, TADY_X_RSTn, XEGY);
  /*p21.XYDO*/ TELU_X3.set(ppu_sig.SACU_CLKPIPE_AxCxExGx, TADY_X_RSTn, XORA);
  /*p21.TUHU*/ SUDE_X4.set(TOCA_CLKPIPE_HI, TADY_X_RSTn, !SUDE_X4);
  /*p21.TUKY*/ TAHA_X5.set(TOCA_CLKPIPE_HI, TADY_X_RSTn, SAKE);
  /*p21.TAKO*/ TYRY_X6.set(TOCA_CLKPIPE_HI, TADY_X_RSTn, TYGE);
  /*p21.SYBE*/ SYBE_X7.set(TOCA_CLKPIPE_HI, TADY_X_RSTn, ROKU);



  wire XYDO_X3 = TELU_X3;

  // LCD horizontal sync pin latch


  /*p24.PAHO*/ PAHO_X_8_SYNC.set(ppu_sig.ROXO_AxCxExGx, ppu_sig.XYMU_RENDERINGp, XYDO_X3);

  // if AVAP goes high, POFY goes high.
  // if PAHO or TOFU go high, POFY goes low.
  /*p24.RUJU*/ POFY_ST_LATCH.nor_latch(sst_sig.AVAP_SCAN_DONE_d0_TRIGp, ppu_reg.PAHO_X_8_SYNC || rst_sig.TOFU_VID_RSTp);

  /*p24.RUZE*/ wire RUZE_PIN_ST = not(POFY_ST_LATCH);
  ST.set(RUZE_PIN_ST);

  // FIXME
#if 0
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


  wire XEHO_X0 = gb.ppu_reg.SAXO_X0;
  wire XYDO_X3 = gb.ppu_reg.TELU_X3;
  /*p21.WEGO*/ wire WEGO_RST_LATCH = or (rst_sig.TOFU_VID_RSTp, VOGA_RENDER_DONE_SYNC);

  // XAJO04 = and(XEHO17, XYDO17);

  /*p21.XAJO*/ wire XAJO_X_009 = and (XEHO.q(), XYDO.q());
  /*p21.WUSA*/ WUSA_CPEN_LATCH.nor_latch(XAJO_X_009, WEGO_RST_LATCH);


  /*p21.TOBA*/ wire TOBA = and (ppu_sig.SACU_CLKPIPE_AxCxExGx, gb.lcd_reg.WUSA_CPEN_LATCH);
  /*p27.POVA*/ wire POVA_FINE_MATCH_TRIG = and (PUXA_FINE_MATCH_SYNC1, !NYZE_FINE_MATCH_SYNC2);
  /*p21.SEMU*/ wire SEMU_LCD_CPn = or (TOBA, POVA_FINE_MATCH_TRIG);
  /*p21.RYPO_LCD_CP*/ wire RYPO_LCD_CP = not(SEMU_LCD_CPn);

  CP.set(RYPO_LCD_CP);
#endif
}
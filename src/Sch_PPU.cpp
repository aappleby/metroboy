#include "Sch_Merged.h"

#include "Sch_Common.h"
#include "Sch_Pins.h"
#include "TestGB.h"
#include "Constants.h"

#pragma warning(disable:4458)

#define HAX 1
#undef HAX

using namespace Schematics;

void TestGB::tick_ppu() {
  /*p27.VYPO*/ wire VYPO_P10_Bn = not(joy_pin.P10_B);

  auto clk_sig = clk_reg.sig(*this);
  auto rst_sig = ResetSignals::get(*this);

  /*p27.NUNY*/ wire NUNY_WIN_MODE_TRIGp = and(win_reg.PYNU_WIN_MODE_LATCH, !win_reg.NOPA_WIN_MODE_SYNC);
  /*p27.NYFO*/ wire NYFO_WIN_MODE_TRIGn = not(NUNY_WIN_MODE_TRIGp);
  /*p27.MOSU*/ wire MOSU_WIN_MODE_TRIGp = not(NYFO_WIN_MODE_TRIGn);

  /*p24.LOBY*/ wire LOBY_RENDERINGn = not(ppu_reg.XYMU_RENDERINGp);

  /*p28.ABAF*/ wire ABAF_VID_LINE_d4n      = not(lcd_reg.CATU_VID_LINE_d4.q());
  /*p28.BYHA*/ wire BYHA_VID_LINE_TRIG_d4n = and (or (lcd_reg.ANEL_VID_LINE_d6.q(), ABAF_VID_LINE_d4n), rst_sig.ABEZ_VID_RSTn);
  /*p28.ATEJ*/ wire ATEJ_VID_LINE_TRIG_d4p = not(BYHA_VID_LINE_TRIG_d4n);

  /*p28.ANOM*/ wire ANOM_SCAN_RSTn = nor(ATEJ_VID_LINE_TRIG_d4p, rst_sig.ATAR_VID_RSTp);
  /*p29.BALU*/ wire BALU_SCAN_RST = not(ANOM_SCAN_RSTn);
  /*p29.BEBU*/ wire SCAN_DONE_d0_TRIGn = or (BALU_SCAN_RST, sst_reg.SCAN_DONE_d5.q(), !sst_reg.SCAN_DONE_d4.q());
  /*p29.AVAP*/ wire AVAP_SCAN_DONE_d0_TRIGp = not(SCAN_DONE_d0_TRIGn);

  /*p25.ROPY*/ wire ROPY_RENDERINGn = not(ppu_reg.XYMU_RENDERINGp);
  /*p27.MOXE*/ wire _MOXE_AxCxExGx = not(clk_sig.ALET_xBxDxFxH);

  /*p24.VYBO*/ wire VYBO_PIX_CLK_AxCxExGx = nor(sst_reg.FEPO_STORE_MATCHp, WODU_RENDER_DONE(), clk_sig.MYVO_AxCxExGx);

  /*p24.TYFA*/ wire TYFA_AxCxExGx = and (SOCY_WIN_HITn(), ppu_reg.POKY_FRONT_PORCH_LATCH, VYBO_PIX_CLK_AxCxExGx);
  /*p24.SEGU*/ wire SEGU_xBxDxFxH = not(TYFA_AxCxExGx);
  /*p24.ROXO*/ wire ROXO_AxCxExGx = not(SEGU_xBxDxFxH);

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

    /*p24.PYGO*/ ppu_reg.PYGO_TILE_DONE.set(clk_sig.ALET_xBxDxFxH, ppu_reg.XYMU_RENDERINGp, ppu_reg.PORY_BFETCH_DONE_SYNC_DELAY);
    /*p24.POKY*/ ppu_reg.POKY_FRONT_PORCH_LATCH.nor_latch(ppu_reg.PYGO_TILE_DONE, LOBY_RENDERINGn);
  }

  {
    /*p27.TUXY*/ wire TUXY = nand(win_reg.SOVY_WIN_HIT_SYNC, SYLO_WIN_HITn());
    /*p27.SUZU*/ wire SUZU = not(TUXY);
    /*p27.SEKO*/ wire SEKO_WIN_TRIGGER = nor(win_reg.RENE_WIN_MATCH_ONSCREEN_SYNC2, !win_reg.RYFA_WIN_MATCH_ONSCREEN_SYNC1);
    /*p27.ROMO*/ wire FRONT_PORCH = not(ppu_reg.POKY_FRONT_PORCH_LATCH);
    /*p27.SUVU*/ wire SUVU = nand(ppu_reg.XYMU_RENDERINGp, FRONT_PORCH, ppu_reg.NYKA_BFETCH_DONE_SYNC, ppu_reg.PORY_BFETCH_DONE_SYNC_DELAY);
    /*p27.TAVE*/ wire TAVE = not(SUVU);
    /*p27.TEVO*/ TEVO_CLK_STOPn = nor(SEKO_WIN_TRIGGER, SUZU, TAVE);
  }

  {
    /*p27.ROZE*/ wire FINE_COUNT_STOPn = nand(ppu_reg.RYKU_FINE_CNT0, ppu_reg.ROGA_FINE_CNT1, ppu_reg.RUBU_FINE_CNT2);
    /*p27.PECU*/ wire FINE_CLK = nand(ROXO_AxCxExGx, FINE_COUNT_STOPn);
    /*p27.PASO*/ wire FINE_RST = nor(TEVO_CLK_STOPn, ROPY_RENDERINGn);
    /*p27.RYKU*/ ppu_reg.RYKU_FINE_CNT0.set(FINE_CLK, FINE_RST, !ppu_reg.RYKU_FINE_CNT0);
    /*p27.ROGA*/ ppu_reg.ROGA_FINE_CNT1.set(!ppu_reg.RYKU_FINE_CNT0, FINE_RST, !ppu_reg.ROGA_FINE_CNT1);
    /*p27.RUBU*/ ppu_reg.RUBU_FINE_CNT2.set(!ppu_reg.ROGA_FINE_CNT1, FINE_RST, !ppu_reg.RUBU_FINE_CNT2);

    // Arms on the ground side, XNOR
    /*p27.SUHA*/ wire SUHA_FINE_MATCH0 = xnor(cfg_reg.SCX0, ppu_reg.RYKU_FINE_CNT0);
    /*p27.SYBY*/ wire SYBY_FINE_MATCH1 = xnor(cfg_reg.SCX1, ppu_reg.ROGA_FINE_CNT1);
    /*p27.SOZU*/ wire SOZU_FINE_MATCH2 = xnor(cfg_reg.SCX2, ppu_reg.RUBU_FINE_CNT2);

    /*p27.RONE*/ wire RONE_FINE_MATCHn = nand(ppu_reg.ROXY_FINE_MATCH_LATCHn, SUHA_FINE_MATCH0, SYBY_FINE_MATCH1, SOZU_FINE_MATCH2);
    /*p27.POHU*/ wire POHU_FINE_MATCH = not(RONE_FINE_MATCHn);

    /*p27.PUXA*/ ppu_reg.PUXA_FINE_MATCH_SYNC1.set(ROXO_AxCxExGx, ppu_reg.XYMU_RENDERINGp, POHU_FINE_MATCH);

    /*p27.NYZE*/ ppu_reg.NYZE_FINE_MATCH_SYNC2.set(_MOXE_AxCxExGx, ppu_reg.XYMU_RENDERINGp, ppu_reg.PUXA_FINE_MATCH_SYNC1);

    /*p27.POVA*/ wire POVA_FINE_MATCH_SETp = and (ppu_reg.PUXA_FINE_MATCH_SYNC1, !ppu_reg.NYZE_FINE_MATCH_SYNC2);
    /*p27.PAHA*/ wire PAHA_FINE_MATCH_RSTp = not(ppu_reg.XYMU_RENDERINGp);
    /*p27.ROXY*/ ppu_reg.ROXY_FINE_MATCH_LATCHn.nor_latch(PAHA_FINE_MATCH_RSTp, POVA_FINE_MATCH_SETp);
  }


  //----------

  {
    Signal NYXU_BFETCH_RSTn;

    {
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

      // Hax

#if HAX
      bool WIN_MODE_TRIG  = ppu_reg.PYNU_WIN_MODE_LATCH && !ppu_reg.NOPA_WIN_MODE_SYNC;
      bool SCAN_DONE_TRIG = ppu_reg.SCAN_DONE_d4.q() && !ppu_reg.SCAN_DONE_d5.q();
      bool NEW_LINE_TRIG  = lcd_reg.CATU_VID_LINE_d4.q() && !lcd_reg.ANEL_VID_LINE_d6.q();

      if (WIN_MODE_TRIG) {
        /*p27.NYXU*/ NYXU_BFETCH_RSTn = 0;
      }
      else if (TEVO_CLK_STOPn) {
        /*p27.NYXU*/ NYXU_BFETCH_RSTn = 0;
      }
      else if (!SCAN_DONE_TRIG) {
        /*p27.NYXU*/ NYXU_BFETCH_RSTn = 1;
      }
      else if (NEW_LINE_TRIG) {
        /*p27.NYXU*/ NYXU_BFETCH_RSTn = 1;
      }
      else {
        /*p27.NYXU*/ NYXU_BFETCH_RSTn = nand(rst_sig.ABEZ_VID_RSTn(), !ATAR_VID_RSTp());
      }
#else
      // Original
      // wait why is TEVO_CLK_STOPn here? Polarity?
      /*p27.NYXU*/ NYXU_BFETCH_RSTn             = nor(AVAP_SCAN_DONE_d0_TRIGp, MOSU_WIN_MODE_TRIGp, TEVO_CLK_STOPn);
#endif
    }

    /*p27.MOCE*/ wire MOCE_BFETCH_DONEn = nand(ppu_reg.LAXU_BFETCH_S0, ppu_reg.NYVA_BFETCH_S2, NYXU_BFETCH_RSTn);
    /*p27.LYRY*/ wire LYRY_BFETCH_DONEp = not(MOCE_BFETCH_DONEn);

    // Original

    {
      /*p27.LEBO*/ wire LEBO_BFETCH_CLKp  = nand(clk_sig.ALET_xBxDxFxH, MOCE_BFETCH_DONEn);

      /*p27.LAXU*/ ppu_reg.LAXU_BFETCH_S0.set(LEBO_BFETCH_CLKp,        NYXU_BFETCH_RSTn, !ppu_reg.LAXU_BFETCH_S0);
      /*p27.MESU*/ ppu_reg.MESU_BFETCH_S1.set(!ppu_reg.LAXU_BFETCH_S0,      NYXU_BFETCH_RSTn, !ppu_reg.MESU_BFETCH_S1);
      /*p27.NYVA*/ ppu_reg.NYVA_BFETCH_S2.set(!ppu_reg.MESU_BFETCH_S1,      NYXU_BFETCH_RSTn, !ppu_reg.NYVA_BFETCH_S2);
      /*p27.LOVY*/ ppu_reg.LOVY_BG_SEQ5_SYNC.set(clk_sig.MYVO_AxCxExGx, NYXU_BFETCH_RSTn, LYRY_BFETCH_DONEp);
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
#if HAX
      if (XYMU_RENDERINGp) {
        /*p27.LONY*/ ppu_reg.LONY_BG_READ_VRAM_LATCHp.nand_latch(NYXU_BFETCH_RSTn, !ppu_reg.LOVY_BG_SEQ5_SYNC);
      }
      else {
        /*p27.LONY*/ ppu_reg.LONY_BG_READ_VRAM_LATCHp.nand_latch(NYXU_BFETCH_RSTn, 0);
      }
#else
      /*p27.LURY*/ wire LURY = and(!ppu_reg.LOVY_BG_SEQ5_SYNC, ppu_reg.XYMU_RENDERINGp);
      /*p27.LONY*/ ppu_reg.LONY_BG_READ_VRAM_LATCHp.nand_latch(NYXU_BG_SEQ_RSTn, LURY);
#endif
    }

    {
      /*p24.NAFY*/ wire NAFY_RENDERING_AND_NOT_WIN_TRIG = nor(MOSU_WIN_MODE_TRIGp, LOBY_RENDERINGn);

      /*p27.LYZU*/ ppu_reg.LYZU_BFETCH_S0_DELAY.set       (clk_sig.ALET_xBxDxFxH, ppu_reg.XYMU_RENDERINGp,         ppu_reg.LAXU_BFETCH_S0);
      /*p24.NYKA*/ ppu_reg.NYKA_BFETCH_DONE_SYNC.set      (clk_sig.ALET_xBxDxFxH, NAFY_RENDERING_AND_NOT_WIN_TRIG, LYRY_BFETCH_DONEp);
      /*p24.PORY*/ ppu_reg.PORY_BFETCH_DONE_SYNC_DELAY.set(clk_sig.MYVO_AxCxExGx, NAFY_RENDERING_AND_NOT_WIN_TRIG, ppu_reg.NYKA_BFETCH_DONE_SYNC);
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
#if HAX

      wire TUKU_WIN_HITn = !ppu_reg.RYDY_WIN_HIT_LATCH.q();
      wire BFETCH_STOP = and(ppu_reg.LAXU_BFETCH_S0, ppu_reg.NYVA_BFETCH_S2, NYXU_BFETCH_RSTn);


      wire TEKY_SPRITE_FETCH = and(sst_reg.FEPO_STORE_MATCHp,
                                    TUKU_WIN_HITn,
                                    BFETCH_STOP,
                                    !ppu_reg.TAKA_SFETCH_RUN_LATCH);

      ppu_reg.SOBU_SPRITE_FETCH_SYNC1.set(TAVA_xBxDxFxH, VYPO_P10_Bn, TEKY_SPRITE_FETCH);
      ppu_reg.SUDA_SPRITE_FETCH_SYNC2.set(LAPE_AxCxExGx, VYPO_P10_Bn, ppu_reg.SOBU_SPRITE_FETCH_SYNC1);

      wire RYCE_SPRITE_FETCH_TRIG = and(ppu_reg.SOBU_SPRITE_FETCH_SYNC1, !ppu_reg.SUDA_SPRITE_FETCH_SYNC2);
      wire SECA_SFETCH_SETn = nor(RYCE_SPRITE_FETCH_TRIG, not(XAPO_VID_RSTn()), BYHA_VID_LINE_TRIG_d4n()); // def nor

      wire FRONT_PORCH = not(ppu_reg.POKY_FRONT_PORCH_LATCH);
      wire SUVU = nand(ppu_reg.XYMU_RENDERINGp, FRONT_PORCH, ppu_reg.NYKA_BFETCH_DONE_SYNC, ppu_reg.PORY_BFETCH_DONE_SYNC_DELAY);
      wire TYNO = nand(ppu_reg.TOXE_SFETCH_S0_D0.q(), ppu_reg.SEBA_SFETCH_S1_D5.q(), ppu_reg.VONU_SFETCH_S1_D4.q());
      wire VUSA = or (!ppu_reg.TYFO_SFETCH_S0_D1.q(), TYNO);
      wire WUTY_SPRITE_DONE = not(VUSA);

      wire VEKU_SFETCH_RSTn = nor(WUTY_SPRITE_DONE, not(SUVU)); // def nor

      ppu_reg.TAKA_SFETCH_RUN_LATCH.nand_latch(SECA_SFETCH_SETn, VEKU_SFETCH_RSTn);

#else
      /*p27.ROMO*/ wire FRONT_PORCH = not(ppu_reg.POKY_FRONT_PORCH_LATCH);
      /*p27.SUVU*/ wire SUVU = nand(ppu_reg.XYMU_RENDERINGp, FRONT_PORCH, ppu_reg.NYKA_BFETCH_DONE_SYNC, ppu_reg.PORY_BFETCH_DONE_SYNC_DELAY);
      /*p27.TAVE*/ wire TAVE = not(SUVU);
      /*p27.RYCE*/ wire RYCE_SPRITE_FETCH_TRIG = and(ppu_reg.SOBU_SPRITE_FETCH_SYNC1, !ppu_reg.SUDA_SPRITE_FETCH_SYNC2);

      /*p27.SECA*/ wire SECA_SFETCH_SETn = nor(RYCE_SPRITE_FETCH_TRIG, rst_sig.ROSY_VID_RSTp, BYHA_VID_LINE_TRIG_d4n); // def nor
      /*p27.VEKU*/ wire VEKU_SFETCH_RSTn = nor(WUTY_SPRITE_DONE(), TAVE); // def nor

      /*p27.TAKA*/ ppu_reg.TAKA_SFETCH_RUN_LATCH.nand_latch(SECA_SFETCH_SETn, VEKU_SFETCH_RSTn);
      /*p27.SOWO*/ wire SOWO_SPRITE_FETCH_LATCHn = not(ppu_reg.TAKA_SFETCH_RUN_LATCH);

      /*p27.TEKY*/ wire TEKY_SPRITE_FETCH = and(sst_reg.FEPO_STORE_MATCHp, TUKU_WIN_HITn(), LYRY_BFETCH_DONEp, SOWO_SPRITE_FETCH_LATCHn);

      /*p27.SOBU*/ ppu_reg.SOBU_SPRITE_FETCH_SYNC1.set(clk_sig.TAVA_xBxDxFxH, VYPO_P10_Bn, TEKY_SPRITE_FETCH);
      /*p27.SUDA*/ ppu_reg.SUDA_SPRITE_FETCH_SYNC2.set(clk_sig.LAPE_AxCxExGx, VYPO_P10_Bn, ppu_reg.SOBU_SPRITE_FETCH_SYNC1);
#endif
    }
    {
      /*p27.RYCE*/ wire RYCE_SPRITE_FETCH_TRIG = and(ppu_reg.SOBU_SPRITE_FETCH_SYNC1, !ppu_reg.SUDA_SPRITE_FETCH_SYNC2);
      /*p27.SECA*/ wire SECA_SFETCH_RSTn = nor(RYCE_SPRITE_FETCH_TRIG, rst_sig.ROSY_VID_RSTp, BYHA_VID_LINE_TRIG_d4n);
      /*p29.TAME*/ wire TAME_SFETCH_101n = nand(ppu_reg.TESE_SFETCH_S2, ppu_reg.TOXE_SFETCH_S0_D0);

      /*p29.TOMA*/ wire TOMA_xBxDxFxH = nand(clk_sig.LAPE_AxCxExGx, TAME_SFETCH_101n);

      /*p29.TOXE*/ ppu_reg.TOXE_SFETCH_S0_D0.set(TOMA_xBxDxFxH,           SECA_SFETCH_RSTn, !ppu_reg.TOXE_SFETCH_S0_D0);
      /*p29.TULY*/ ppu_reg.TULY_SFETCH_S1.set(!ppu_reg.TOXE_SFETCH_S0_D0, SECA_SFETCH_RSTn, !ppu_reg.TULY_SFETCH_S1);
      /*p29.TESE*/ ppu_reg.TESE_SFETCH_S2.set(!ppu_reg.TULY_SFETCH_S1, SECA_SFETCH_RSTn, !ppu_reg.TESE_SFETCH_S2);

      /*p29.TYFO*/ ppu_reg.TYFO_SFETCH_S0_D1.set(clk_sig.LAPE_AxCxExGx, VYPO_P10_Bn, ppu_reg.TOXE_SFETCH_S0_D0);
      /*p29.TOBU*/ ppu_reg.TOBU_SFETCH_S1_D2.set(clk_sig.TAVA_xBxDxFxH, ppu_reg.XYMU_RENDERINGp, ppu_reg.TULY_SFETCH_S1);    // note input is seq 1 not 2

      /*p29.VONU*/ ppu_reg.VONU_SFETCH_S1_D4.set(clk_sig.TAVA_xBxDxFxH, ppu_reg.XYMU_RENDERINGp, ppu_reg.TOBU_SFETCH_S1_D2);
      /*p29.SEBA*/ ppu_reg.SEBA_SFETCH_S1_D5.set(clk_sig.LAPE_AxCxExGx, ppu_reg.XYMU_RENDERINGp, ppu_reg.VONU_SFETCH_S1_D4); // is this clock wrong?
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

    /*p21.WEGO*/ wire WEGO_RST_LATCH = or (rst_sig.TOFU_VID_RSTp, ppu_reg.VOGA_RENDER_DONE_SYNC);
    /*p21.XYMU*/ ppu_reg.XYMU_RENDERINGp.nor_latch(AVAP_SCAN_DONE_d0_TRIGp, WEGO_RST_LATCH);
  }

  //----------



  {
    /*p21.TADY*/ wire TADY_X_RST = nor(BYHA_VID_LINE_TRIG_d4n, rst_sig.TOFU_VID_RSTp);
    // having this reset connected to both RENDER_DONE_SYNC and x seems odd
    /*p21.VOGA*/ ppu_reg.VOGA_RENDER_DONE_SYNC.set(clk_sig.ALET_xBxDxFxH, TADY_X_RST, WODU_RENDER_DONE());
  }

  // vid x position, has carry lookahead because this increments every tcycle

#if HAX
  wire X_167 = and(ppu_reg.SAXO_X0.q(), ppu_reg.TYPO_X1.q(), ppu_reg.VYZO_X2.q(), ppu_reg.TAHA_X5.q(), ppu_reg.SYBE_X7.q()); // 128 + 32 + 4 + 2 + 1 = 167

  wire TYFA_AxCxExGx = nand(!ppu_reg.RYDY_WIN_HIT_LATCH.q(),
                              ppu_reg.POKY_FRONT_PORCH_LATCH,
                              !sst_reg.FEPO_STORE_MATCHp,
                              !and(X_167, !sst_reg.FEPO_STORE_MATCHp),
                              !MYVO_AxCxExGx());
  wire SACU_CLKPIPE_AxCxExGx = nor(TYFA_AxCxExGx, ppu_reg.ROXY_FINE_MATCH_LATCHn);
  wire TOCA_CLKPIPE_HI = not(ppu_reg.TELU_X3);

  if (BYHA_VID_LINE_TRIG_d4n() || !XAPO_VID_RSTn()) {
    ppu_reg.SAXO_X0.rst_sync(SACU_CLKPIPE_AxCxExGx);
    ppu_reg.TYPO_X1.rst_sync(SACU_CLKPIPE_AxCxExGx);
    ppu_reg.VYZO_X2.rst_sync(SACU_CLKPIPE_AxCxExGx);
    ppu_reg.TELU_X3.rst_sync(SACU_CLKPIPE_AxCxExGx);
    ppu_reg.SUDE_X4.rst_sync(TOCA_CLKPIPE_HI);
    ppu_reg.TAHA_X5.rst_sync(TOCA_CLKPIPE_HI);
    ppu_reg.TYRY_X6.rst_sync(TOCA_CLKPIPE_HI);
    ppu_reg.SYBE_X7.rst_sync(TOCA_CLKPIPE_HI);
  }
  else {
    ppu_reg.SAXO_X0.set(SACU_CLKPIPE_AxCxExGx, !ppu_reg.SAXO_X0);
    ppu_reg.TYPO_X1.set(SACU_CLKPIPE_AxCxExGx, xor(ppu_reg.TYPO_X1, and(ppu_reg.SAXO_X0)));
    ppu_reg.VYZO_X2.set(SACU_CLKPIPE_AxCxExGx, xor(ppu_reg.VYZO_X2, and(ppu_reg.SAXO_X0, ppu_reg.TYPO_X1)));
    ppu_reg.TELU_X3.set(SACU_CLKPIPE_AxCxExGx, xor(ppu_reg.TELU_X3, and(ppu_reg.SAXO_X0, ppu_reg.TYPO_X1, ppu_reg.VYZO_X2)));
    ppu_reg.SUDE_X4.set(TOCA_CLKPIPE_HI, !ppu_reg.SUDE_X4);
    ppu_reg.TAHA_X5.set(TOCA_CLKPIPE_HI, xor(ppu_reg.SUDE_X4, and(ppu_reg.TAHA_X5)));
    ppu_reg.TYRY_X6.set(TOCA_CLKPIPE_HI, xor(ppu_reg.TYRY_X6, and(ppu_reg.SUDE_X4, ppu_reg.TAHA_X5)));
    ppu_reg.SYBE_X7.set(TOCA_CLKPIPE_HI, xor(ppu_reg.SYBE_X7, and(ppu_reg.SUDE_X4, ppu_reg.TAHA_X5, ppu_reg.TYRY_X6)));
  }
#else

  /*p21.RYBO*/ wire RYBO = xor(ppu_reg.SAXO_X0, ppu_reg.TYPO_X1);
  /*p21.XUKE*/ wire XUKE = and(ppu_reg.SAXO_X0, ppu_reg.TYPO_X1);

  /*p21.XYLE*/ wire XYLE = and(ppu_reg.VYZO_X2, XUKE);
  /*p21.XEGY*/ wire XEGY = xor(ppu_reg.VYZO_X2, XUKE);
  /*p21.XORA*/ wire XORA = xor(ppu_reg.TELU_X3, XYLE);

  /*p21.SAKE*/ wire SAKE = xor(ppu_reg.SUDE_X4, ppu_reg.TAHA_X5);
  /*p21.TYBA*/ wire TYBA = and(ppu_reg.SUDE_X4, ppu_reg.TAHA_X5);
  /*p21.SURY*/ wire SURY = and(ppu_reg.TYRY_X6, TYBA);
  /*p21.TYGE*/ wire TYGE = xor(ppu_reg.TYRY_X6, TYBA);
  /*p21.ROKU*/ wire ROKU = xor(ppu_reg.SYBE_X7, SURY);

  /*p21.TADY*/ wire TADY_X_RSTn = nor(BYHA_VID_LINE_TRIG_d4n, rst_sig.TOFU_VID_RSTp);

  /*p24.SACU*/ wire SACU_CLKPIPE_AxCxExGx = nor(SEGU_xBxDxFxH, ppu_reg.ROXY_FINE_MATCH_LATCHn);
  /*p24.TOCA*/ wire TOCA_CLKPIPE_HI = not(ppu_reg.TELU_X3);

  /*p21.XEHO*/ ppu_reg.SAXO_X0.set(SACU_CLKPIPE_AxCxExGx, TADY_X_RSTn, !ppu_reg.SAXO_X0);
  /*p21.SAVY*/ ppu_reg.TYPO_X1.set(SACU_CLKPIPE_AxCxExGx, TADY_X_RSTn, RYBO);
  /*p21.XODU*/ ppu_reg.VYZO_X2.set(SACU_CLKPIPE_AxCxExGx, TADY_X_RSTn, XEGY);
  /*p21.XYDO*/ ppu_reg.TELU_X3.set(SACU_CLKPIPE_AxCxExGx, TADY_X_RSTn, XORA);
  /*p21.TUHU*/ ppu_reg.SUDE_X4.set(TOCA_CLKPIPE_HI, TADY_X_RSTn, !ppu_reg.SUDE_X4);
  /*p21.TUKY*/ ppu_reg.TAHA_X5.set(TOCA_CLKPIPE_HI, TADY_X_RSTn, SAKE);
  /*p21.TAKO*/ ppu_reg.TYRY_X6.set(TOCA_CLKPIPE_HI, TADY_X_RSTn, TYGE);
  /*p21.SYBE*/ ppu_reg.SYBE_X7.set(TOCA_CLKPIPE_HI, TADY_X_RSTn, ROKU);
#endif
}
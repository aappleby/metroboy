#include "Sch_SpriteFetcher.h"
#include "TestGB.h"

using namespace Schematics;

// VAPE04 >> XUJY01
// SAKY = nor(TULY17, VONU17)
// TEPA = not(XYMU)
// TYSO = or(SAKY, TEPA)
// TEXY = not(TYSO)

//------------------------------------------------------------------------------

SpriteFetcherSignals SpriteFetcher::sig(const TestGB& gb) const {
  SpriteFetcherSignals sig;

  auto ppu_sig = gb.ppu_reg.sig(gb);

  /*p33.PEFO*/ sig.SPR_PIX_A0 = SPR_PIX_A0;
  /*p33.ROKA*/ sig.SPR_PIX_A1 = SPR_PIX_A1;
  /*p33.MYTU*/ sig.SPR_PIX_A2 = SPR_PIX_A2;
  /*p33.RAMU*/ sig.SPR_PIX_A3 = SPR_PIX_A3;
  /*p33.SELE*/ sig.SPR_PIX_A4 = SPR_PIX_A4;
  /*p33.SUTO*/ sig.SPR_PIX_A5 = SPR_PIX_A5;
  /*p33.RAMA*/ sig.SPR_PIX_A6 = SPR_PIX_A6;
  /*p33.RYDU*/ sig.SPR_PIX_A7 = SPR_PIX_A7;

  /*p33.REWO*/ sig.SPR_PIX_B0 = SPR_PIX_B0;
  /*p33.PEBA*/ sig.SPR_PIX_B1 = SPR_PIX_B1;
  /*p33.MOFO*/ sig.SPR_PIX_B2 = SPR_PIX_B2;
  /*p33.PUDU*/ sig.SPR_PIX_B3 = SPR_PIX_B3;
  /*p33.SAJA*/ sig.SPR_PIX_B4 = SPR_PIX_B4;
  /*p33.SUNY*/ sig.SPR_PIX_B5 = SPR_PIX_B5;
  /*p33.SEMO*/ sig.SPR_PIX_B6 = SPR_PIX_B6;
  /*p33.SEGA*/ sig.SPR_PIX_B7 = SPR_PIX_B7;

  {
    /*p29.TEPA*/ wire TEPA_RENDERINGn = not(ppu_sig.XYMU_RENDERINGp);
    /*p29.TUVO*/ wire TUVO_PPU_OAM_RDp = nor(TEPA_RENDERINGn, TULY_SFETCH_S1, TESE_SFETCH_S2);
    /*p28.WEFY*/ sig.WEFY_SPR_READp = and(TUVO_PPU_OAM_RDp, TYFO_SFETCH_S0_D1);
  }

  {
#if 0
    /*p29.TACU*/ wire TACU_SPR_SEQ_5_TRIG = nand(TYFO_SFETCH_S0_D1, !TOXE_SFETCH_S0_D0);
    /*p25.VAPE*/ wire VAPE_FETCH_OAM_CLK = and (RENDERING, !TULY_SFETCH_S1, !TESE_SFETCH_S2, TACU_SPR_SEQ_5_TRIG);
#endif
    /*p29.TEPA*/ wire TEPA_RENDERINGn = not(ppu_sig.XYMU_RENDERINGp);
    /*p29.TUVO*/ wire TUVO_PPU_OAM_RDp = nor(TEPA_RENDERINGn, TULY_SFETCH_S1, TESE_SFETCH_S2);
    /*p29.TYTU*/ wire TYTU_SFETCH_S0_D0n = not(TOXE_SFETCH_S0_D0);
    /*p29.TACU*/ wire TACU_SPR_SEQ_5_TRIG = nand(TYFO_SFETCH_S0_D1, TYTU_SFETCH_S0_D0n);
    /*p25.VAPE*/ sig.VAPE_FETCH_OAM_CLK = and (TUVO_PPU_OAM_RDp, TACU_SPR_SEQ_5_TRIG);
  }

  {
    // TYNO_01 << TOXE_17
    // TYNO_02 << SEBA_17
    // TYNO_03 << VONU_17
    // TYNO_04 >> VUSA_02

    // VUSA_01 << TYFO_16
    // VUSA_02 << TYNO_04
    // VUSA_03 nc
    // VUSA_04 >>

#if 0
    PIPE_LOAD_SPRITE = and(TOXE_SFETCH_S0_D0,
                           TYFO_SFETCH_S0_D1,
                           VONU_SFETCH_S1_D4,
                           SEBA_SFETCH_S1_D5);
#endif

    /*p29.TYNO*/ wire TYNO = nand(TOXE_SFETCH_S0_D0.q(), SEBA_SFETCH_S1_D5.q(), VONU_SFETCH_S1_D4.q());
    /*p29.VUSA*/ wire VUSA_PIPE_LOAD_SPRITEn = or(TYFO_SFETCH_S0_D1.qn(), TYNO);
    /*p29.WUTY*/ sig.WUTY_PIPE_LOAD_SPRITEp = not(VUSA_PIPE_LOAD_SPRITEn);
    /*p29.XEFY*/ sig.XEPY_PIPE_LOAD_SPRITEn = not(sig.WUTY_PIPE_LOAD_SPRITEp);

  }

  /*p27.SOWO*/ sig.SOWO_SFETCH_RUNNINGn = not(TAKA_SFETCH_RUNNINGp);

  {
    /*p29.TEPA*/ wire TEPA_RENDERINGn = not(ppu_sig.XYMU_RENDERINGp);
    /*p29.SAKY*/ wire SAKY = nor(TULY_SFETCH_S1.q(), VONU_SFETCH_S1_D4.q());
    /*p29.TYSO*/ wire _TYSO_SPRITE_READn = or(SAKY, TEPA_RENDERINGn);
    /*p29.TEXY*/ sig.TEXY_SPRITE_READp = not(_TYSO_SPRITE_READn);
  }

  {
    /*p29.TYTU*/ wire TYTU_SFETCH_S0_D0n = not(TOXE_SFETCH_S0_D0.q());
    /*p29.TACU*/ wire TACU_SPR_SEQ_5_TRIG = nand(TYFO_SFETCH_S0_D1.q(), TYTU_SFETCH_S0_D0n);
    /*p29.ABON*/ wire ABON_SPR_VRAM_RDp1 = not(sig.TEXY_SPRITE_READp);
    /*p25.SOHO*/ sig.SOHO_SPR_VRAM_RDp = and (TACU_SPR_SEQ_5_TRIG, ABON_SPR_VRAM_RDp1);
  }

  return sig;
}

//------------------------------------------------------------------------------

void SpriteFetcher::tick(TestGB& gb) {
  auto& vram_bus = gb.vram_bus;
  auto ppu_sig = gb.ppu_reg.sig(gb);
  auto dbg_sig = gb.dbg_reg.sig(gb);
  auto sst_sig = gb.sst_reg.sig(gb);
  auto clk_sig = gb.clk_reg.sig(gb);
  auto rst_sig = gb.rst_reg.sig(gb);
  auto lcd_sig = gb.lcd_reg.sig(gb);

  auto& ppu_config = gb.ppu_config;

  auto sprite_fetcher_sig = gb.sprite_fetcher.sig(gb);

  wire P10_B = 0;

  //----------------------------------------
  // So this is def the chunk that watches FEPO_STORE_MATCHp and triggers a sprite fetch...

  // Maybe we should annotate phase starting with the phase 0 = FEPO_MATCH_SYNC goes high?

  {
    /*p27.SOBU*/ SOBU_SPRITE_FETCH_TRIG_A.set(clk_sig.TAVA_xBxDxFxH, dbg_sig.VYPO_P10_Bn, ppu_sig.TEKY_SPRITE_FETCH);
    /*p27.SUDA*/ SUDA_SPRITE_FETCH_TRIG_B.set(clk_sig.LAPE_AxCxExGx, dbg_sig.VYPO_P10_Bn, SOBU_SPRITE_FETCH_TRIG_A);
    /*p27.RYCE*/ wire RYCE_SPRITE_FETCH_TRIG = and (SOBU_SPRITE_FETCH_TRIG_A, !SUDA_SPRITE_FETCH_TRIG_B);

    /*p27.SECA*/ wire SECA_SFETCH_RUNNING_SETn = nor(RYCE_SPRITE_FETCH_TRIG, rst_sig.ROSY_VID_RSTp, lcd_sig.BYHA_VID_LINE_TRIG_d4n); // def nor
    /*p27.TAKA*/ TAKA_SFETCH_RUNNINGp.nand_latch(SECA_SFETCH_RUNNING_SETn, ppu_sig.VEKU_SFETCH_RUNNING_RSTn);

    /*p29.TAME*/ wire TAME_SFETCH_CLK_GATE = nand(TESE_SFETCH_S2, TOXE_SFETCH_S0_D0);
    /*p29.TOMA*/ wire TOMA_SFETCH_CLK = nand(clk_sig.LAPE_AxCxExGx, TAME_SFETCH_CLK_GATE);

    /*p29.TOXE*/ TOXE_SFETCH_S0_D0.set(TOMA_SFETCH_CLK,         SECA_SFETCH_RUNNING_SETn,        !TOXE_SFETCH_S0_D0);
    /*p29.TYFO*/ TYFO_SFETCH_S0_D1.set(clk_sig.LAPE_AxCxExGx, dbg_sig.VYPO_P10_Bn,     TOXE_SFETCH_S0_D0);
    /*p29.TULY*/ TULY_SFETCH_S1.set(!TOXE_SFETCH_S0_D0,       SECA_SFETCH_RUNNING_SETn,        !TULY_SFETCH_S1);
    /*p29.TESE*/ TESE_SFETCH_S2.set(!TULY_SFETCH_S1,          SECA_SFETCH_RUNNING_SETn,        !TESE_SFETCH_S2);

    /*p29.TOBU*/ TOBU_SFETCH_S1_D2.set(clk_sig.TAVA_xBxDxFxH, ppu_sig.XYMU_RENDERINGp, TULY_SFETCH_S1);    // note input is seq 1 not 2
    /*p29.VONU*/ VONU_SFETCH_S1_D4.set(clk_sig.TAVA_xBxDxFxH, ppu_sig.XYMU_RENDERINGp, TOBU_SFETCH_S1_D2);
    /*p29.SEBA*/ SEBA_SFETCH_S1_D5.set(clk_sig.LAPE_AxCxExGx, ppu_sig.XYMU_RENDERINGp, VONU_SFETCH_S1_D4); // is this clock wrong?
  }

  {
    auto bus_sig = gb.bus_mux.sig(gb);

    // SFETCH_000 - TOPU_SPRITE_PIX_LATCH_A = 0, 
    // SFETCH_001
    // SFETCH_010
    // SFETCH_011
    // SFETCH_100
    // SFETCH_101
    // SFETCH_110
    // SFETCH_111

    /*p29.XONO*/ wire XONO_FLIP_X = and (bus_sig.BAXO_SPRITE_X5, sprite_fetcher_sig.TEXY_SPRITE_READp);
    /*p33.POBE*/ wire SPR_PIX_FLIP0 = mux2_p(vram_bus.TRI_D7, vram_bus.TRI_D0, XONO_FLIP_X);
    /*p33.PACY*/ wire SPR_PIX_FLIP1 = mux2_p(vram_bus.TRI_D6, vram_bus.TRI_D1, XONO_FLIP_X);
    /*p33.PONO*/ wire SPR_PIX_FLIP2 = mux2_p(vram_bus.TRI_D5, vram_bus.TRI_D2, XONO_FLIP_X);
    /*p33.PUGU*/ wire SPR_PIX_FLIP3 = mux2_p(vram_bus.TRI_D4, vram_bus.TRI_D3, XONO_FLIP_X);
    /*p33.PUTE*/ wire SPR_PIX_FLIP4 = mux2_p(vram_bus.TRI_D3, vram_bus.TRI_D4, XONO_FLIP_X);
    /*p33.PULY*/ wire SPR_PIX_FLIP5 = mux2_p(vram_bus.TRI_D2, vram_bus.TRI_D5, XONO_FLIP_X);
    /*p33.PELO*/ wire SPR_PIX_FLIP6 = mux2_p(vram_bus.TRI_D1, vram_bus.TRI_D6, XONO_FLIP_X);
    /*p33.PAWE*/ wire SPR_PIX_FLIP7 = mux2_p(vram_bus.TRI_D0, vram_bus.TRI_D7, XONO_FLIP_X);

#if 0
    wire LATCH_SPPIXA = and (RENDERING, TOXE_SFETCH_S0_D0, !TYFO_SFETCH_S0_D1, TULY_SFETCH_S1);
    wire LATCH_SPPIXB = and (RENDERING, TOXE_SFETCH_S0_D0, !TYFO_SFETCH_S0_D1, VONU_SFETCH_S1_D4);
#endif

    /*p29.TYTU*/ wire TYTU_SFETCH_S0n = not(TOXE_SFETCH_S0_D0.q());
    /*p24.LOBY*/ wire LOBY_RENDERINGn = not(ppu_sig.XYMU_RENDERINGp);
    /*p29.SYCU*/ wire SYCU = nor(TYTU_SFETCH_S0n, LOBY_RENDERINGn, TYFO_SFETCH_S0_D1);

    /*p29.TOPU*/ wire TOPU_LATCH_SPPIXA = and (TULY_SFETCH_S1, SYCU);
    /*p29.VYWA*/ wire VYWA_CLKp = not(TOPU_LATCH_SPPIXA);
    /*p29.WENY*/ wire WENY_CLKn = not(VYWA_CLKp);
    /*p29.XADO*/ wire XADO_CLKp = not(WENY_CLKn);
    /*p33.PEFO*/ SPR_PIX_A0.set(XADO_CLKp, SPR_PIX_FLIP0);
    /*p33.ROKA*/ SPR_PIX_A1.set(XADO_CLKp, SPR_PIX_FLIP1);
    /*p33.MYTU*/ SPR_PIX_A2.set(XADO_CLKp, SPR_PIX_FLIP2);
    /*p33.RAMU*/ SPR_PIX_A3.set(XADO_CLKp, SPR_PIX_FLIP3);
    /*p33.SELE*/ SPR_PIX_A4.set(XADO_CLKp, SPR_PIX_FLIP4);
    /*p33.SUTO*/ SPR_PIX_A5.set(XADO_CLKp, SPR_PIX_FLIP5);
    /*p33.RAMA*/ SPR_PIX_A6.set(XADO_CLKp, SPR_PIX_FLIP6);
    /*p33.RYDU*/ SPR_PIX_A7.set(XADO_CLKp, SPR_PIX_FLIP7);

    /*p29.RACA*/ wire RACA_LATCH_SPPIXB = and (VONU_SFETCH_S1_D4, SYCU);
    /*p29.PEBY*/ wire PEBY_CLKp = not(RACA_LATCH_SPPIXB);
    /*p29.NYBE*/ wire NYBE_CLKn = not(PEBY_CLKp);
    /*p29.PUCO*/ wire PUCO_CLKb = not(NYBE_CLKn);
    /*p33.REWO*/ SPR_PIX_B0.set(PUCO_CLKb, SPR_PIX_FLIP0);
    /*p33.PEBA*/ SPR_PIX_B1.set(PUCO_CLKb, SPR_PIX_FLIP1);
    /*p33.MOFO*/ SPR_PIX_B2.set(PUCO_CLKb, SPR_PIX_FLIP2);
    /*p33.PUDU*/ SPR_PIX_B3.set(PUCO_CLKb, SPR_PIX_FLIP3);
    /*p33.SAJA*/ SPR_PIX_B4.set(PUCO_CLKb, SPR_PIX_FLIP4);
    /*p33.SUNY*/ SPR_PIX_B5.set(PUCO_CLKb, SPR_PIX_FLIP5);
    /*p33.SEMO*/ SPR_PIX_B6.set(PUCO_CLKb, SPR_PIX_FLIP6);
    /*p33.SEGA*/ SPR_PIX_B7.set(PUCO_CLKb, SPR_PIX_FLIP7);
  }

  {
    auto bus_sig = gb.bus_mux.sig(gb);

#if 0
    wire SPRITE_READ = RENDERING && (TULY_SFETCH_S1 || VONU_SFETCH_S1_D4);
#endif

    /*p29.FUFO*/ wire _FUFO_LCDC_SPSIZEn = not(ppu_config.XYMO_LCDC_SPSIZE);
    /*p29.WUKY*/ wire _WUKY_FLIP_Y = not(bus_sig.YZOS_SPRITE_X6);

    /*p29.XUQU*/ wire _XUQU_SPRITE_AB = not(!VONU_SFETCH_S1_D4.q());
    /*p29.CYVU*/ wire _CYVU_SPRITE_Y0 = xor (_WUKY_FLIP_Y, sst_sig.CUCU_TS_LINE_1);
    /*p29.BORE*/ wire _BORE_SPRITE_Y1 = xor (_WUKY_FLIP_Y, sst_sig.CUCA_TS_LINE_2);
    /*p29.BUVY*/ wire _BUVY_SPRITE_Y2 = xor (_WUKY_FLIP_Y, sst_sig.CEGA_TS_LINE_3);

    /*p29.WAGO*/ wire _WAGO = xor (_WUKY_FLIP_Y, sst_sig.WENU_TS_LINE_0);
    /*p29.GEJY*/ wire _GEJY_SPRITE_Y3 = amux2(_FUFO_LCDC_SPSIZEn, !bus_sig.XUSO_SPRITE_Y0, ppu_config.XYMO_LCDC_SPSIZE, _WAGO);

    /*p29.ABON*/ wire ABON_SPRITE_READn = not(sprite_fetcher_sig.TEXY_SPRITE_READp);

    /*p29.ABEM*/ vram_bus.TRI_A00.set_tribuf(ABON_SPRITE_READn, _XUQU_SPRITE_AB);
    /*p29.BAXE*/ vram_bus.TRI_A01.set_tribuf(ABON_SPRITE_READn, _CYVU_SPRITE_Y0);
    /*p29.ARAS*/ vram_bus.TRI_A02.set_tribuf(ABON_SPRITE_READn, _BORE_SPRITE_Y1);
    /*p29.AGAG*/ vram_bus.TRI_A03.set_tribuf(ABON_SPRITE_READn, _BUVY_SPRITE_Y2);
    /*p29.FAMU*/ vram_bus.TRI_A04.set_tribuf(ABON_SPRITE_READn, _GEJY_SPRITE_Y3);
    /*p29.FUGY*/ vram_bus.TRI_A05.set_tribuf(ABON_SPRITE_READn, bus_sig.XEGU_SPRITE_Y1);
    /*p29.GAVO*/ vram_bus.TRI_A06.set_tribuf(ABON_SPRITE_READn, bus_sig.YJEX_SPRITE_Y2);
    /*p29.WYGA*/ vram_bus.TRI_A07.set_tribuf(ABON_SPRITE_READn, bus_sig.XYJU_SPRITE_Y3);
    /*p29.WUNE*/ vram_bus.TRI_A08.set_tribuf(ABON_SPRITE_READn, bus_sig.YBOG_SPRITE_Y4);
    /*p29.GOTU*/ vram_bus.TRI_A09.set_tribuf(ABON_SPRITE_READn, bus_sig.WYSO_SPRITE_Y5);
    /*p29.GEGU*/ vram_bus.TRI_A10.set_tribuf(ABON_SPRITE_READn, bus_sig.XOTE_SPRITE_Y6);
    /*p29.XEHE*/ vram_bus.TRI_A11.set_tribuf(ABON_SPRITE_READn, bus_sig.YZAB_SPRITE_Y7);
    /*p29.DYSO*/ vram_bus.TRI_A12.set_tribuf(ABON_SPRITE_READn, P10_B);   // sprites always in low half of tile store
  }
}

//------------------------------------------------------------------------------

bool SpriteFetcher::commit() {
  bool changed = false;

  /*p27.TAKA*/ changed |= TAKA_SFETCH_RUNNINGp.commit_latch();
  /*p27.SOBU*/ changed |= SOBU_SPRITE_FETCH_TRIG_A.commit_reg();
  /*p27.SUDA*/ changed |= SUDA_SPRITE_FETCH_TRIG_B.commit_reg();

  /*p29.TOXE*/ changed |= TOXE_SFETCH_S0_D0.commit_reg();
  /*p29.TULY*/ changed |= TULY_SFETCH_S1.commit_reg();
  /*p29.TESE*/ changed |= TESE_SFETCH_S2.commit_reg();
  /*p29.TOBU*/ changed |= TOBU_SFETCH_S1_D2.commit_reg();
  /*p29.VONU*/ changed |= VONU_SFETCH_S1_D4.commit_reg();
  /*p29.SEBA*/ changed |= SEBA_SFETCH_S1_D5.commit_reg();
  /*p29.TYFO*/ changed |= TYFO_SFETCH_S0_D1.commit_reg();

  /*p33.PEFO*/ changed |= SPR_PIX_A0.commit_reg();
  /*p33.ROKA*/ changed |= SPR_PIX_A1.commit_reg();
  /*p33.MYTU*/ changed |= SPR_PIX_A2.commit_reg();
  /*p33.RAMU*/ changed |= SPR_PIX_A3.commit_reg();
  /*p33.SELE*/ changed |= SPR_PIX_A4.commit_reg();
  /*p33.SUTO*/ changed |= SPR_PIX_A5.commit_reg();
  /*p33.RAMA*/ changed |= SPR_PIX_A6.commit_reg();
  /*p33.RYDU*/ changed |= SPR_PIX_A7.commit_reg();
  /*p33.REWO*/ changed |= SPR_PIX_B0.commit_reg();
  /*p33.PEBA*/ changed |= SPR_PIX_B1.commit_reg();
  /*p33.MOFO*/ changed |= SPR_PIX_B2.commit_reg();
  /*p33.PUDU*/ changed |= SPR_PIX_B3.commit_reg();
  /*p33.SAJA*/ changed |= SPR_PIX_B4.commit_reg();
  /*p33.SUNY*/ changed |= SPR_PIX_B5.commit_reg();
  /*p33.SEMO*/ changed |= SPR_PIX_B6.commit_reg();
  /*p33.SEGA*/ changed |= SPR_PIX_B7.commit_reg();

  return changed;
}


//------------------------------------------------------------------------------


#if 0

void dump_regs(TextPainter& text_painter) {
  text_painter.dprintf("----- SPR_REG -----\n");

  TOXE_SFETCH_S0_D0.dump(text_painter, "TOXE_SFETCH_S0_D0    ");
  TULY_SFETCH_S1.dump(text_painter, "TULY_SFETCH_S1    ");
  TESE_SFETCH_S2.dump(text_painter, "TESE_SFETCH_S2    ");
  TOBU_SFETCH_S1_D2.dump(text_painter, "TOBU_SFETCH_S1_D2  ");
  VONU_SFETCH_S1_D4.dump(text_painter, "VONU_SFETCH_S1_D4 ");
  SEBA_SFETCH_S1_D5.dump(text_painter, "SEBA_SFETCH_S1_D5 ");
  TYFO_SFETCH_S0_D1.dump(text_painter, "TYFO_SFETCH_S0_D1     ");
  //CENO_SCANNINGp.dump(text_painter, "CENO_SCANNINGp");

  //text_painter.dprintf("SCAN    %d\n", scan());
  //SCAN_DONE_d4.dump(text_painter, "SCAN_DONE_d4 ");
  //SCAN_DONE_d5.dump(text_painter, "SCAN_DONE_d5 ");

  //text_painter.dprintf("SPR_IDX %d\n", spr_idx());
  //text_painter.dprintf("TS_IDX  %d\n", ts_idx());
  //text_painter.dprintf("TS_LINE %d\n", ts_line());
  text_painter.newline();
}



#endif
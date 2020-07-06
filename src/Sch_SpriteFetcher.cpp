#include "Sch_SpriteFetcher.h"
#include "Sch_Top.h"

using namespace Schematics;

// VAPE04 >> XUJY01
// SAKY = nor(TULY17, VONU17)
// TEPA = not(XYMU)
// TYSO = or(SAKY, TEPA)
// TEXY = not(TYSO)

//------------------------------------------------------------------------------

void SpriteFetcher::tick(SchematicTop& top) {

  /*p01.ANOS*/ wire ANOS_AxCxExGx = not(top.SYS_PIN_CLK_B);
  /*p01.ATAL*/ wire ATAL_xBxDxFxH = not(ANOS_AxCxExGx);
  /*p01.AZOF*/ wire AZOF_AxCxExGx = not(ATAL_xBxDxFxH);
  /*p01.ZAXY*/ wire ZAXY_xBxDxFxH = not(AZOF_AxCxExGx);
  /*p01.ZEME*/ wire ZEME_AxCxExGx = not(ZAXY_xBxDxFxH);
  /*p01.ALET*/ wire ALET_xBxDxFxH = not(ZEME_AxCxExGx);
  /*p01.LAPE*/ wire LAPE_AxCxExGx = not(ALET_xBxDxFxH);
  /*p27.TAVA*/ wire TAVA_xBxDxFxH = not(LAPE_AxCxExGx);

  /*p01.ROSY*/ wire ROSY_VID_RSTp = not(top.XAPO_VID_RSTn());

  /*p24.LOBY*/ wire LOBY_RENDERINGn = not(top.XYMU_RENDERINGp());

  //----------------------------------------
  // So this is def the chunk that watches FEPO_STORE_MATCHp and triggers a sprite fetch...

  // Maybe we should annotate phase starting with the phase 0 = FEPO_MATCH_SYNC goes high?

  {

    // SOBU_01 SC
    // SOBU_02 << TAVA_02 CLKp
    // SOBU_03 SC
    // SOBU_04 NC
    // SOBU_05 NC
    // SOBU_06 << VYPO    RSTn 
    // SOBU_07 << TEKY_06 D
    // SOBU_08 NC
    // SOBU_09 SC
    // SOBU_10 NC
    // SOBU_11 NC
    // SOBU_12 SC
    // SOBU_13 << VYPO    RSTn
    // SOBU_14 NC
    // SOBU_15 NC
    // SOBU_16 >> NC      QN
    // SOBU_17 >> SUDA_07 Q

    // TAKA has arms on the VCC side - nand latch
    // TAKA01 << VEKU02
    // TAKA02 nc
    // TAKA03 >> nc
    // TAKA04 >> SOWO00
    // TAKA05 nc
    // TAKA06 << SECA03
    // if SECA03 goes low, TAKA04 goes high
    // if VEKU02 goes low, TAKA04 goes low

#if 0

    // simple case

    /*p27.TEKY*/ wire TEKY_SPRITE_FETCH = and (top.FEPO_STORE_MATCHp(), not(TAKA_SFETCH_RUNNINGp));

    /*p27.SOBU*/ SOBU_SPRITE_FETCH_A.set(TAVA_xBxDxFxH, top.VYPO_GND, TEKY_SPRITE_FETCH);
    /*p27.SUDA*/ SUDA_SPRITE_FETCH_B.set(LAPE_AxCxExGx, top.VYPO_GND, SOBU_SPRITE_FETCH_A);
    /*p27.RYCE*/ wire RYCE_SPRITE_FETCHpe = and (SOBU_SPRITE_FETCH_A, !SUDA_SPRITE_FETCH_B);
    /*p27.SECA*/ wire SECA_SFETCH_FETCHne = not(RYCE_SPRITE_FETCHpe);

    /*p27.TAKA*/ TAKA_SFETCH_RUNNINGp.nand_latch(SECA_SFETCH_FETCHne, top.VEKU_SFETCH_RUNNING_RSTn());

    /*p29.TAME*/ wire TAME_SFETCH_CLK_GATE = nand(TESE_SFETCH_S2, TOXE_SFETCH_S0);
    /*p29.TOMA*/ wire TOMA_SFETCH_CLK_xBxDxFxH = nand(LAPE_AxCxExGx, TAME_SFETCH_CLK_GATE);

    /*p29.TOXE*/ TOXE_SFETCH_S0.set(TOMA_SFETCH_CLK_xBxDxFxH, SECA_SFETCH_FETCHne, !TOXE_SFETCH_S0);
    /*p29.TULY*/ TULY_SFETCH_S1.set(!TOXE_SFETCH_S0,          SECA_SFETCH_FETCHne, !TULY_SFETCH_S1);
    /*p29.TESE*/ TESE_SFETCH_S2.set(!TULY_SFETCH_S1,          SECA_SFETCH_FETCHne, !TESE_SFETCH_S2);

    /*p29.TOBU*/ TOBU_SFETCH_S1_D2.set(TAVA_xBxDxFxH, 1, TULY_SFETCH_S1);    // note input is seq 1 not 2
    /*p29.VONU*/ VONU_SFETCH_S1_D4.set(TAVA_xBxDxFxH, 1, TOBU_SFETCH_S1_D2);
    /*p29.SEBA*/ SEBA_SFETCH_S1_D5.set(LAPE_AxCxExGx, 1, VONU_SFETCH_S1_D4);

  /*p29.VUSA*/ wire VUSA_SPRITE_DONEn = or(!TYFO_SFETCH_S0_D1, !TOXE_SFETCH_S0, !SEBA_SFETCH_S1_D5, !VONU_SFETCH_S1_D4);

#endif

    /*p27.SYLO*/ wire SYLO_WIN_HITn = not(top.RYDY_WIN_FIRST_TILE_A());
    /*p24.TOMU*/ wire TOMU_WIN_HITp = not(SYLO_WIN_HITn);
    /*p27.TUKU*/ wire TUKU_WIN_HITn = not(TOMU_WIN_HITp);
    /*p27.SOWO*/ wire SOWO_SFETCH_RUNNINGn = not(TAKA_SFETCH_RUNNINGp);
    /*p27.TEKY*/ wire TEKY_SPRITE_FETCH = and (top.FEPO_STORE_MATCHp(), TUKU_WIN_HITn, top.LYRY_BFETCH_DONEp(), SOWO_SFETCH_RUNNINGn);

    /*p27.SOBU*/ SOBU_SPRITE_FETCH_A.set(TAVA_xBxDxFxH, top.VYPO_GND, TEKY_SPRITE_FETCH);
    /*p27.SUDA*/ SUDA_SPRITE_FETCH_B.set(LAPE_AxCxExGx, top.VYPO_GND, SOBU_SPRITE_FETCH_A);
    /*p27.RYCE*/ wire RYCE_SPRITE_FETCHpe = and (SOBU_SPRITE_FETCH_A, !SUDA_SPRITE_FETCH_B);
    /*p27.SECA*/ wire SECA_SPRITE_FETCHne = nor(RYCE_SPRITE_FETCHpe, ROSY_VID_RSTp, top.BYHA_VID_LINE_TRIG_d4()); // def nor

    /*p27.TAKA*/ TAKA_SFETCH_RUNNINGp.nand_latch(SECA_SPRITE_FETCHne, top.VEKU_SFETCH_RUNNING_RSTn());

    /*p29.TAME*/ wire TAME_SFETCH_CLK_GATE = nand(TESE_SFETCH_S2, TOXE_SFETCH_S0);
    /*p29.TOMA*/ wire TOMA_SFETCH_CLK_xBxDxFxH = nand(LAPE_AxCxExGx, TAME_SFETCH_CLK_GATE);

    /*p29.TOXE*/ TOXE_SFETCH_S0.set(TOMA_SFETCH_CLK_xBxDxFxH, SECA_SPRITE_FETCHne, !TOXE_SFETCH_S0);
    /*p29.TULY*/ TULY_SFETCH_S1.set(!TOXE_SFETCH_S0,          SECA_SPRITE_FETCHne, !TULY_SFETCH_S1);
    /*p29.TESE*/ TESE_SFETCH_S2.set(!TULY_SFETCH_S1,          SECA_SPRITE_FETCHne, !TESE_SFETCH_S2);

    /*p29.TOBU*/ TOBU_SFETCH_S1_D2.set(TAVA_xBxDxFxH, top.XYMU_RENDERINGp().as_nwire(), TULY_SFETCH_S1);    // note input is seq 1 not 2
    /*p29.VONU*/ VONU_SFETCH_S1_D4.set(TAVA_xBxDxFxH, top.XYMU_RENDERINGp().as_nwire(), TOBU_SFETCH_S1_D2);
    /*p29.SEBA*/ SEBA_SFETCH_S1_D5.set(LAPE_AxCxExGx, top.XYMU_RENDERINGp().as_nwire(), VONU_SFETCH_S1_D4);
  }

  {
    /*p29.XONO*/ wire XONO_FLIP_X = and (top.BAXO_SPRITE_X5(), top.TEXY_SPR_READ_VRAMp());
    /*p33.POBE*/ wire SPR_PIX_FLIP0 = mux2_p(top.VRM_TRI_D7, top.VRM_TRI_D0, XONO_FLIP_X);
    /*p33.PACY*/ wire SPR_PIX_FLIP1 = mux2_p(top.VRM_TRI_D6, top.VRM_TRI_D1, XONO_FLIP_X);
    /*p33.PONO*/ wire SPR_PIX_FLIP2 = mux2_p(top.VRM_TRI_D5, top.VRM_TRI_D2, XONO_FLIP_X);
    /*p33.PUGU*/ wire SPR_PIX_FLIP3 = mux2_p(top.VRM_TRI_D4, top.VRM_TRI_D3, XONO_FLIP_X);
    /*p33.PUTE*/ wire SPR_PIX_FLIP4 = mux2_p(top.VRM_TRI_D3, top.VRM_TRI_D4, XONO_FLIP_X);
    /*p33.PULY*/ wire SPR_PIX_FLIP5 = mux2_p(top.VRM_TRI_D2, top.VRM_TRI_D5, XONO_FLIP_X);
    /*p33.PELO*/ wire SPR_PIX_FLIP6 = mux2_p(top.VRM_TRI_D1, top.VRM_TRI_D6, XONO_FLIP_X);
    /*p33.PAWE*/ wire SPR_PIX_FLIP7 = mux2_p(top.VRM_TRI_D0, top.VRM_TRI_D7, XONO_FLIP_X);


    /*p29.TYFO*/ TYFO_SFETCH_S0_D1.set(LAPE_AxCxExGx, top.VYPO_GND, TOXE_SFETCH_S0);
    /*p29.TYTU*/ wire TYTU_SFETCH_S0n = not(TOXE_SFETCH_S0.q());
    /*p29.SYCU*/ wire SYCU_SFETCH_S0pe = nor(TYTU_SFETCH_S0n, LOBY_RENDERINGn, TYFO_SFETCH_S0_D1);

    /*p29.RACA*/ wire RACA_LATCH_SPPIXB = and (VONU_SFETCH_S1_D4, SYCU_SFETCH_S0pe);
    /*p29.PEBY*/ wire PEBY_CLKp = not(RACA_LATCH_SPPIXB);
    /*p29.NYBE*/ wire NYBE_CLKn = not(PEBY_CLKp);
    /*p29.PUCO*/ wire PUCO_CLKb = not(NYBE_CLKn);

    /*p33.PEFO*/ SPR_PIX_A0.set(PUCO_CLKb, !PUCO_CLKb, SPR_PIX_FLIP0);
    /*p33.ROKA*/ SPR_PIX_A1.set(PUCO_CLKb, !PUCO_CLKb, SPR_PIX_FLIP1);
    /*p33.MYTU*/ SPR_PIX_A2.set(PUCO_CLKb, !PUCO_CLKb, SPR_PIX_FLIP2);
    /*p33.RAMU*/ SPR_PIX_A3.set(PUCO_CLKb, !PUCO_CLKb, SPR_PIX_FLIP3);
    /*p33.SELE*/ SPR_PIX_A4.set(PUCO_CLKb, !PUCO_CLKb, SPR_PIX_FLIP4);
    /*p33.SUTO*/ SPR_PIX_A5.set(PUCO_CLKb, !PUCO_CLKb, SPR_PIX_FLIP5);
    /*p33.RAMA*/ SPR_PIX_A6.set(PUCO_CLKb, !PUCO_CLKb, SPR_PIX_FLIP6);
    /*p33.RYDU*/ SPR_PIX_A7.set(PUCO_CLKb, !PUCO_CLKb, SPR_PIX_FLIP7);

    /*p29.TOPU*/ wire TOPU_LATCH_SPPIXA = and (TULY_SFETCH_S1, SYCU_SFETCH_S0pe);
    /*p29.VYWA*/ wire VYWA_CLKp = not(TOPU_LATCH_SPPIXA);
    /*p29.WENY*/ wire WENY_CLKn = not(VYWA_CLKp);
    /*p29.XADO*/ wire XADO_CLKp = not(WENY_CLKn);

    /*p33.REWO*/ SPR_PIX_B0.set(XADO_CLKp, !XADO_CLKp, SPR_PIX_FLIP0);
    /*p33.PEBA*/ SPR_PIX_B1.set(XADO_CLKp, !XADO_CLKp, SPR_PIX_FLIP1);
    /*p33.MOFO*/ SPR_PIX_B2.set(XADO_CLKp, !XADO_CLKp, SPR_PIX_FLIP2);
    /*p33.PUDU*/ SPR_PIX_B3.set(XADO_CLKp, !XADO_CLKp, SPR_PIX_FLIP3);
    /*p33.SAJA*/ SPR_PIX_B4.set(XADO_CLKp, !XADO_CLKp, SPR_PIX_FLIP4);
    /*p33.SUNY*/ SPR_PIX_B5.set(XADO_CLKp, !XADO_CLKp, SPR_PIX_FLIP5);
    /*p33.SEMO*/ SPR_PIX_B6.set(XADO_CLKp, !XADO_CLKp, SPR_PIX_FLIP6);
    /*p33.SEGA*/ SPR_PIX_B7.set(XADO_CLKp, !XADO_CLKp, SPR_PIX_FLIP7);
  }

  {
    /*p29.FUFO*/ wire _FUFO_LCDC_SPSIZEn = not(top.XYMO_LCDC_SPSIZE);
    /*p29.WUKY*/ wire _WUKY_FLIP_Y = not(top.YZOS_SPRITE_X6());

    /*p29.XUQU*/ wire _XUQU_SPRITE_AB = not(!VONU_SFETCH_S1_D4.q());
    /*p29.CYVU*/ wire _CYVU_SPRITE_Y0 = xor (_WUKY_FLIP_Y, top.SPR_TRI_LINE_1);
    /*p29.BORE*/ wire _BORE_SPRITE_Y1 = xor (_WUKY_FLIP_Y, top.SPR_TRI_LINE_2);
    /*p29.BUVY*/ wire _BUVY_SPRITE_Y2 = xor (_WUKY_FLIP_Y, top.SPR_TRI_LINE_3);

    /*p29.WAGO*/ wire _WAGO = xor (_WUKY_FLIP_Y, top.SPR_TRI_LINE_0);
    /*p29.GEJY*/ wire _GEJY_SPRITE_Y3 = amux2(_FUFO_LCDC_SPSIZEn, !top.XUSO_SPRITE_Y0(), top.XYMO_LCDC_SPSIZE, _WAGO);

    /*p29.ABON*/ wire ABON_SPRITE_READn = not(top.TEXY_SPR_READ_VRAMp());

    /*p29.ABEM*/ top.VRM_TRI_A00.set_tribuf(ABON_SPRITE_READn, _XUQU_SPRITE_AB);
    /*p29.BAXE*/ top.VRM_TRI_A01.set_tribuf(ABON_SPRITE_READn, _CYVU_SPRITE_Y0);
    /*p29.ARAS*/ top.VRM_TRI_A02.set_tribuf(ABON_SPRITE_READn, _BORE_SPRITE_Y1);
    /*p29.AGAG*/ top.VRM_TRI_A03.set_tribuf(ABON_SPRITE_READn, _BUVY_SPRITE_Y2);
    /*p29.FAMU*/ top.VRM_TRI_A04.set_tribuf(ABON_SPRITE_READn, _GEJY_SPRITE_Y3);
    /*p29.FUGY*/ top.VRM_TRI_A05.set_tribuf(ABON_SPRITE_READn, top.XEGU_SPRITE_Y1());
    /*p29.GAVO*/ top.VRM_TRI_A06.set_tribuf(ABON_SPRITE_READn, top.YJEX_SPRITE_Y2());
    /*p29.WYGA*/ top.VRM_TRI_A07.set_tribuf(ABON_SPRITE_READn, top.XYJU_SPRITE_Y3());
    /*p29.WUNE*/ top.VRM_TRI_A08.set_tribuf(ABON_SPRITE_READn, top.YBOG_SPRITE_Y4());
    /*p29.GOTU*/ top.VRM_TRI_A09.set_tribuf(ABON_SPRITE_READn, top.WYSO_SPRITE_Y5());
    /*p29.GEGU*/ top.VRM_TRI_A10.set_tribuf(ABON_SPRITE_READn, top.XOTE_SPRITE_Y6());
    /*p29.XEHE*/ top.VRM_TRI_A11.set_tribuf(ABON_SPRITE_READn, top.YZAB_SPRITE_Y7());
    /*p29.DYSO*/ top.VRM_TRI_A12.set_tribuf(ABON_SPRITE_READn, top.GND);   // sprites always in low half of tile store
  }
}

//------------------------------------------------------------------------------

SignalHash SpriteFetcher::commit() {
  SignalHash hash;

  /*p27.TAKA*/ hash << TAKA_SFETCH_RUNNINGp.commit_latch();
  /*p27.SOBU*/ hash << SOBU_SPRITE_FETCH_A.commit_reg();
  /*p27.SUDA*/ hash << SUDA_SPRITE_FETCH_B.commit_reg();

  /*p29.TOXE*/ hash << TOXE_SFETCH_S0.commit_reg();
  /*p29.TULY*/ hash << TULY_SFETCH_S1.commit_reg();
  /*p29.TESE*/ hash << TESE_SFETCH_S2.commit_reg();
  /*p29.TOBU*/ hash << TOBU_SFETCH_S1_D2.commit_reg();
  /*p29.VONU*/ hash << VONU_SFETCH_S1_D4.commit_reg();
  /*p29.SEBA*/ hash << SEBA_SFETCH_S1_D5.commit_reg();
  /*p29.TYFO*/ hash << TYFO_SFETCH_S0_D1.commit_reg();

  /*p33.PEFO*/ hash << SPR_PIX_A0.commit_reg();
  /*p33.ROKA*/ hash << SPR_PIX_A1.commit_reg();
  /*p33.MYTU*/ hash << SPR_PIX_A2.commit_reg();
  /*p33.RAMU*/ hash << SPR_PIX_A3.commit_reg();
  /*p33.SELE*/ hash << SPR_PIX_A4.commit_reg();
  /*p33.SUTO*/ hash << SPR_PIX_A5.commit_reg();
  /*p33.RAMA*/ hash << SPR_PIX_A6.commit_reg();
  /*p33.RYDU*/ hash << SPR_PIX_A7.commit_reg();
  /*p33.REWO*/ hash << SPR_PIX_B0.commit_reg();
  /*p33.PEBA*/ hash << SPR_PIX_B1.commit_reg();
  /*p33.MOFO*/ hash << SPR_PIX_B2.commit_reg();
  /*p33.PUDU*/ hash << SPR_PIX_B3.commit_reg();
  /*p33.SAJA*/ hash << SPR_PIX_B4.commit_reg();
  /*p33.SUNY*/ hash << SPR_PIX_B5.commit_reg();
  /*p33.SEMO*/ hash << SPR_PIX_B6.commit_reg();
  /*p33.SEGA*/ hash << SPR_PIX_B7.commit_reg();

  return hash;
}


//------------------------------------------------------------------------------


#if 0

void dump_regs(TextPainter& text_painter) {
  text_painter.dprintf("----- SPR_REG -----\n");

  TOXE_SFETCH_S0.dump(text_painter, "TOXE_SFETCH_S0    ");
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
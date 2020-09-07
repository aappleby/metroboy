#include "GateBoyLib/Sch_SpriteFetcher.h"
#include "GateBoyLib/Sch_Top.h"

using namespace Schematics;

#pragma warning(disable:4189)

//------------------------------------------------------------------------------

void SpriteFetcher::dump(Dumper& d) const {
  d("----------SpriteFetch---------\n");
  d("SOBU_SFETCH_REQp     %c\n", SOBU_SFETCH_REQp    .c());
  d("SUDA_SFETCH_REQp     %c\n", SUDA_SFETCH_REQp    .c());
  d("\n");
  d("TAKA_SFETCH_RUNNINGp %c\n", TAKA_SFETCH_RUNNINGp.c());
  d("\n");
  d("TOXE_SFETCH_S0       %c\n", TOXE_SFETCH_S0      .c());
  d("TYFO_SFETCH_S0_D1    %c\n", TYFO_SFETCH_S0_D1   .c());
  d("\n");
  d("TULY_SFETCH_S1       %c\n", TULY_SFETCH_S1      .c());
  d("TOBU_SFETCH_S1_D2    %c\n", TOBU_SFETCH_S1_D2   .c());
  d("VONU_SFETCH_S1_D4    %c\n", VONU_SFETCH_S1_D4   .c());
  d("SEBA_SFETCH_S1_D5    %c\n", SEBA_SFETCH_S1_D5   .c());
  d("\n");
  d("TESE_SFETCH_S2       %c\n", TESE_SFETCH_S2      .c());
  d("\n");
}

//------------------------------------------------------------------------------

void SpriteFetcher::tick(const SchematicTop& top) {
  /* p29.TEPA*/ wire TEPA_RENDERINGn = not1(top.pix_pipe.XYMU_RENDERINGp.tp());
  /* p24.LOBY*/ wire LOBY_RENDERINGn = not1(top.pix_pipe.XYMU_RENDERINGp.tp());

  // FIXME this is kind of a mess
  /* p29.TYNO*/ wire TYNO = nand3(TOXE_SFETCH_S0.qp(), SEBA_SFETCH_S1_D5.qp(), VONU_SFETCH_S1_D4.qp());
  /* p29.VUSA*/ wire VUSA_SPRITE_DONEn  = or2(TYFO_SFETCH_S0_D1.qn(), TYNO);
  /* p29.WUTY*/ WUTY_SPRITE_DONEp = not1(VUSA_SPRITE_DONEn);
  /* p29.TUVO*/ wire TUVO_PPU_OAM_RDp = nor3(TEPA_RENDERINGn, TULY_SFETCH_S1.qp(), TESE_SFETCH_S2.qp());
  /* p28.WEFY*/ WEFY_SPR_READp = and2(TUVO_PPU_OAM_RDp, TYFO_SFETCH_S0_D1.qp());
  /* p29.TYTU*/ wire TYTU_SFETCH_S0n = not1(TOXE_SFETCH_S0.qp());
  /* p29.TACU*/ wire TACU_SPR_SEQ_5_TRIG = nand2(TYFO_SFETCH_S0_D1.qp(), TYTU_SFETCH_S0n);
  /* p29.SAKY*/ wire SAKY_SFETCH_MYSTERY = nor2(TULY_SFETCH_S1.qp(), VONU_SFETCH_S1_D4.qp());
  /* p29.TYSO*/ wire TYSO_SPRITE_READn = or2(SAKY_SFETCH_MYSTERY, TEPA_RENDERINGn); // def or
  /* p29.TEXY*/ TEXY_SPR_READ_VRAMp = not1(TYSO_SPRITE_READn);
  /* p29.ABON*/ ABON_SPR_VRM_RDn = not1(TEXY_SPR_READ_VRAMp);
  /* p25.SOHO*/ SOHO_SPR_VRAM_RDp = and2(TACU_SPR_SEQ_5_TRIG, TEXY_SPR_READ_VRAMp);
  /* p25.VAPE*/ VAPE_FETCH_OAM_CLK = and2(TUVO_PPU_OAM_RDp, TACU_SPR_SEQ_5_TRIG);
  /*#p29.XUQU*/ XUQU_SPRITE_AB = not1(top.sprite_fetcher.VONU_SFETCH_S1_D4.qn());
  /*p29.SYCU*/  wire SYCU_SFETCH_S0pe = nor3(TYTU_SFETCH_S0n, LOBY_RENDERINGn, top.sprite_fetcher.TYFO_SFETCH_S0_D1.qp());
  /*#p29.RACA*/ RACA_LATCH_SPPIXB = and2(top.sprite_fetcher.VONU_SFETCH_S1_D4.qp(), SYCU_SFETCH_S0pe);
  /*#p29.TOPU*/ TOPU_LATCH_SPPIXA = and2(top.sprite_fetcher.TULY_SFETCH_S1.qp(), SYCU_SFETCH_S0pe);
}

//------------------------------------------------------------------------------

void SpriteFetcher::tock(SchematicTop& top) {
  wire VYPO = 1;
  wire XYMU_RENDERINGp = top.pix_pipe.XYMU_RENDERINGp.tp();
  wire ATEJ_VID_LINE_END_TRIGp = top.lcd_reg.ATEJ_VID_LINE_END_TRIGp;

  /* p01.ROSY*/ wire ROSY_VID_RSTp = not1(top.clk_reg.XAPO_VID_RSTn);
  /* p01.ZEME*/ wire ZEME_AxCxExGx = not1(top.clk_reg.ZAXY_xBxDxFxH);
  /* p01.ALET*/ wire ALET_xBxDxFxH = not1(ZEME_AxCxExGx);
  /* p01.LAPE*/ wire LAPE_AxCxExGx = not1(ALET_xBxDxFxH);
  /* p27.TAVA*/ wire TAVA_xBxDxFxH = not1(LAPE_AxCxExGx);

  //----------------------------------------

  /* p27.TUKU*/ wire TUKU_WIN_HITn = not1(top.pix_pipe.TOMU_WIN_HITp);
  /* p27.LYRY*/ wire LYRY_BFETCH_DONEp = not1(top.tile_fetcher.MOCE_BFETCH_DONEn);
  /* p27.SOWO*/ wire SOWO_SFETCH_RUNNINGn = not1(TAKA_SFETCH_RUNNINGp.tp());

  /* p27.TEKY*/ wire TEKY_SFETCH_REQp = and4(top.sprite_store.FEPO_STORE_MATCHp, TUKU_WIN_HITn, LYRY_BFETCH_DONEp, SOWO_SFETCH_RUNNINGn);

  /* p27.SOBU*/ SOBU_SFETCH_REQp.tock(TAVA_xBxDxFxH, VYPO, TEKY_SFETCH_REQp);
  /* p27.SUDA*/ SUDA_SFETCH_REQp.tock(LAPE_AxCxExGx, VYPO, SOBU_SFETCH_REQp.qp());

  /* p27.RYCE*/ wire RYCE_SFETCH_TRIGp = and2(SOBU_SFETCH_REQp.qp(),  SUDA_SFETCH_REQp.qn());

  /*#p27.SECA*/ wire SECA_SFETCH_RUNNING_SETn = nor3(RYCE_SFETCH_TRIGp, ROSY_VID_RSTp, ATEJ_VID_LINE_END_TRIGp);
  /* p27.VEKU*/ wire VEKU_SFETCH_RUNNING_RSTn = nor2(WUTY_SPRITE_DONEp, top.TAVE_PRELOAD_DONE_TRIGp); // def nor
  /* p27.TAKA*/ TAKA_SFETCH_RUNNINGp = nand_latch(SECA_SFETCH_RUNNING_SETn, VEKU_SFETCH_RUNNING_RSTn);
   
  /*#p29.TAME*/ wire TAME_SFETCH_CLK_GATE = nand2(TESE_SFETCH_S2.qp(), TOXE_SFETCH_S0.qp());
  /*#p29.TOMA*/ wire TOMA_SFETCH_CLK_xBxDxFxH = nand2(LAPE_AxCxExGx, TAME_SFETCH_CLK_GATE);

  /*#p29.TOXE*/ TOXE_SFETCH_S0   .tock(TOMA_SFETCH_CLK_xBxDxFxH, SECA_SFETCH_RUNNING_SETn, TOXE_SFETCH_S0.qn());
  /*#p29.TYFO*/ TYFO_SFETCH_S0_D1.tock(LAPE_AxCxExGx,            VYPO,                     TOXE_SFETCH_S0.qp());
  /*#p29.TULY*/ TULY_SFETCH_S1   .tock(TOXE_SFETCH_S0.qn(),      SECA_SFETCH_RUNNING_SETn, TULY_SFETCH_S1.qn());
  /*#p29.TOBU*/ TOBU_SFETCH_S1_D2.tock(TAVA_xBxDxFxH,            XYMU_RENDERINGp,          TULY_SFETCH_S1.qp());
  /*#p29.VONU*/ VONU_SFETCH_S1_D4.tock(TAVA_xBxDxFxH,            XYMU_RENDERINGp,          TOBU_SFETCH_S1_D2.qp());
  /*#p29.SEBA*/ SEBA_SFETCH_S1_D5.tock(LAPE_AxCxExGx,            XYMU_RENDERINGp,          VONU_SFETCH_S1_D4.qp());
  /*#p29.TESE*/ TESE_SFETCH_S2   .tock(TULY_SFETCH_S1.qn(),      SECA_SFETCH_RUNNING_SETn, TESE_SFETCH_S2.qn());
}

//------------------------------------------------------------------------------

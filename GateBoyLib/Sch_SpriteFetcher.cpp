#include "GateBoyLib/Sch_SpriteFetcher.h"
#include "GateBoyLib/Sch_Top.h"

using namespace Schematics;

#pragma warning(disable:4189)

//------------------------------------------------------------------------------

void SpriteFetcher::dump(Dumper& d) const {
  d("\002===== SpriteFetch =====\001\n");
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
  /* p29.TEPA*/ wire TEPA_RENDERINGn = not1(top.pix_pipe.XYMU_RENDERINGn.qn03());
  /* p24.LOBY*/ wire LOBY_RENDERINGn = not1(top.pix_pipe.XYMU_RENDERINGn.qn03());

  // FIXME this is kind of a mess
  /* p29.TYNO*/ wire TYNO = nand3(TOXE_SFETCH_S0.qp17(), SEBA_SFETCH_S1_D5.qp17(), VONU_SFETCH_S1_D4.qp17());
  /* p29.VUSA*/ wire VUSA_SPRITE_DONEn  = or2(TYFO_SFETCH_S0_D1.qn16(), TYNO);
  /* p29.WUTY*/ WUTY_SPRITE_DONEp = not1(VUSA_SPRITE_DONEn);
  /* p29.TUVO*/ wire TUVO_PPU_OAM_RDp = nor3(TEPA_RENDERINGn, TULY_SFETCH_S1.qp17(), TESE_SFETCH_S2.qp17());
  /* p28.WEFY*/ WEFY_SPR_READp = and2(TUVO_PPU_OAM_RDp, TYFO_SFETCH_S0_D1.qp17());
  /* p29.TYTU*/ wire TYTU_SFETCH_S0n = not1(TOXE_SFETCH_S0.qp17());
  /* p29.TACU*/ wire TACU_SPR_SEQ_5_TRIG = nand2(TYFO_SFETCH_S0_D1.qp17(), TYTU_SFETCH_S0n);
  /* p29.SAKY*/ wire SAKY_SFETCH_MYSTERY = nor2(TULY_SFETCH_S1.qp17(), VONU_SFETCH_S1_D4.qp17());
  /* p29.TYSO*/ wire TYSO_SPRITE_READn = or2(SAKY_SFETCH_MYSTERY, TEPA_RENDERINGn); // def or
  /* p29.TEXY*/ TEXY_SPR_READ_VRAMp = not1(TYSO_SPRITE_READn);
  /* p29.ABON*/ ABON_SPR_VRM_RDn = not1(TEXY_SPR_READ_VRAMp);
  /* p25.SOHO*/ SOHO_SPR_VRAM_RDp = and2(TACU_SPR_SEQ_5_TRIG, TEXY_SPR_READ_VRAMp);
  /* p25.VAPE*/ VAPE_FETCH_OAM_CLK = and2(TUVO_PPU_OAM_RDp, TACU_SPR_SEQ_5_TRIG);
  /*#p29.XUQU*/ XUQU_SPRITE_AB = not1(top.sprite_fetcher.VONU_SFETCH_S1_D4.qn16());
  /*p29.SYCU*/  wire SYCU_SFETCH_S0pe = nor3(TYTU_SFETCH_S0n, LOBY_RENDERINGn, top.sprite_fetcher.TYFO_SFETCH_S0_D1.qp17());
  /*#p29.RACA*/ RACA_LATCH_SPPIXB = and2(top.sprite_fetcher.VONU_SFETCH_S1_D4.qp17(), SYCU_SFETCH_S0pe);
  /*#p29.TOPU*/ TOPU_LATCH_SPPIXA = and2(top.sprite_fetcher.TULY_SFETCH_S1.qp17(), SYCU_SFETCH_S0pe);
}

//------------------------------------------------------------------------------

void SpriteFetcher::tock(SchematicTop& top) {
  wire VYPO = 1;
  wire XYMU_RENDERINGp = top.pix_pipe.XYMU_RENDERINGn.qn03();
  wire ATEJ_VID_LINE_END_TRIGp = top.lcd_reg.ATEJ_LINE_TRIGp;

  /* p01.ROSY*/ wire _ROSY_VID_RSTp = not1(top.clk_reg.XAPO_VID_RSTn);
  /* p01.LAPE*/ wire _LAPE_AxCxExGx = not1(top.clk_reg.ALET_xBxDxFxH);
  /* p27.TAVA*/ wire _TAVA_xBxDxFxH = not1(_LAPE_AxCxExGx);

  //----------------------------------------

  /* p27.TUKU*/ wire _TUKU_WIN_HITn = not1(top.pix_pipe.TOMU_WIN_HITp);

  /* p27.SOWO*/ wire _SOWO_SFETCH_RUNNINGn = not1(TAKA_SFETCH_RUNNINGp.qp03());

  /* p27.TEKY*/ wire _TEKY_SFETCH_REQp = and4(top.sprite_store.FEPO_STORE_MATCHp, _TUKU_WIN_HITn, top.tile_fetcher.LYRY_BFETCH_DONEp, _SOWO_SFETCH_RUNNINGn);

  /* p27.SOBU*/ SOBU_SFETCH_REQp.dff17(_TAVA_xBxDxFxH, VYPO, _TEKY_SFETCH_REQp);
  /* p27.SUDA*/ SUDA_SFETCH_REQp.dff17(_LAPE_AxCxExGx, VYPO, SOBU_SFETCH_REQp.qp17());

  /* p27.RYCE*/ wire _RYCE_SFETCH_TRIGp = and2(SOBU_SFETCH_REQp.qp17(),  SUDA_SFETCH_REQp.qn16());

  /*#p27.SECA*/ wire _SECA_SFETCH_RUNNING_SETn = nor3(_RYCE_SFETCH_TRIGp, _ROSY_VID_RSTp, ATEJ_VID_LINE_END_TRIGp);
  /* p27.VEKU*/ wire _VEKU_SFETCH_RUNNING_RSTn = nor2(WUTY_SPRITE_DONEp, top.TAVE_PRELOAD_DONE_TRIGp); // def nor
  /* p27.TAKA*/ TAKA_SFETCH_RUNNINGp.nand_latch(_SECA_SFETCH_RUNNING_SETn, _VEKU_SFETCH_RUNNING_RSTn);

  /*#p29.TAME*/ wire _TAME_SFETCH_CLK_GATE = nand2(TESE_SFETCH_S2.qp17(), TOXE_SFETCH_S0.qp17());
  /*#p29.TOMA*/ wire _TOMA_SFETCH_CLK_xBxDxFxH = nand2(_LAPE_AxCxExGx, _TAME_SFETCH_CLK_GATE);

  /*#p29.TOXE*/ TOXE_SFETCH_S0   .dff17(_TOMA_SFETCH_CLK_xBxDxFxH, _SECA_SFETCH_RUNNING_SETn, TOXE_SFETCH_S0.qn16());
  /*#p29.TYFO*/ TYFO_SFETCH_S0_D1.dff17(_LAPE_AxCxExGx,            VYPO,                     TOXE_SFETCH_S0.qp17());
  /*#p29.TULY*/ TULY_SFETCH_S1   .dff17(TOXE_SFETCH_S0.qn16(),    _SECA_SFETCH_RUNNING_SETn, TULY_SFETCH_S1.qn16());
  /*#p29.TOBU*/ TOBU_SFETCH_S1_D2.dff17(_TAVA_xBxDxFxH,            XYMU_RENDERINGp,          TULY_SFETCH_S1.qp17());
  /*#p29.VONU*/ VONU_SFETCH_S1_D4.dff17(_TAVA_xBxDxFxH,            XYMU_RENDERINGp,          TOBU_SFETCH_S1_D2.qp17());
  /*#p29.SEBA*/ SEBA_SFETCH_S1_D5.dff17(_LAPE_AxCxExGx,            XYMU_RENDERINGp,          VONU_SFETCH_S1_D4.qp17());
  /*#p29.TESE*/ TESE_SFETCH_S2   .dff17(TULY_SFETCH_S1.qn16(),    _SECA_SFETCH_RUNNING_SETn, TESE_SFETCH_S2.qn16());
}

//------------------------------------------------------------------------------

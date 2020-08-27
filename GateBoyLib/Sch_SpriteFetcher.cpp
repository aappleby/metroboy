#include "GateBoyLib/Sch_SpriteFetcher.h"
#include "GateBoyLib/Sch_Top.h"

using namespace Schematics;

void SpriteFetcher::dump(Dumper& d) const {
  d("----------SpriteFetch---------\n");
  d("_SOBU_SFETCH_REQp     %c\n", _SOBU_SFETCH_REQp    .c());
  d("_SUDA_SFETCH_REQp     %c\n", _SUDA_SFETCH_REQp    .c());
  d("\n");
  d("_TAKA_SFETCH_RUNNINGp %c\n", _TAKA_SFETCH_RUNNINGp.c());
  d("\n");
  d("_TOXE_SFETCH_S0       %c\n", _TOXE_SFETCH_S0      .c());
  d("_TYFO_SFETCH_S0_D1    %c\n", _TYFO_SFETCH_S0_D1   .c());
  d("\n");
  d("_TULY_SFETCH_S1       %c\n", _TULY_SFETCH_S1      .c());
  d("_TOBU_SFETCH_S1_D2    %c\n", _TOBU_SFETCH_S1_D2   .c());
  d("_VONU_SFETCH_S1_D4    %c\n", _VONU_SFETCH_S1_D4   .c());
  d("_SEBA_SFETCH_S1_D5    %c\n", _SEBA_SFETCH_S1_D5   .c());
  d("\n");
  d("_TESE_SFETCH_S2       %c\n", _TESE_SFETCH_S2      .c());
  d("\n");
}


//------------------------------------------------------------------------------

void SpriteFetcher::tick(const SchematicTop& /*top*/) {
}

//------------------------------------------------------------------------------

void SpriteFetcher::tock(SchematicTop& top) {
  /*p01.ROSY*/ wire ROSY_VID_RSTp = not1(top.clk_reg.XAPO_VID_RSTn);

  /*p01.ZEME*/ wire ZEME_AxCxExGx = not1(top.clk_reg.ZAXY_xBxDxFxH);
  /*p01.ALET*/ wire ALET_xBxDxFxH = not1(ZEME_AxCxExGx);
  /*p01.LAPE*/ wire _LAPE_AxCxExGx = not1(ALET_xBxDxFxH);
  /*p27.TAVA*/ wire _TAVA_xBxDxFxH = not1(_LAPE_AxCxExGx);
  wire VYPO = 1;

  //----------------------------------------

  /*p27.TUKU*/ wire _TUKU_WIN_HITn = not1(top.pix_pipe.TOMU_WIN_HITp());
  /*p27.SOWO*/ wire _SOWO_SFETCH_RUNNINGn = not1(_TAKA_SFETCH_RUNNINGp.tp());
  /*p27.MOCE*/ wire MOCE_BFETCH_DONEn = nand3(top.tile_fetcher._LAXU_BFETCH_S0.qp(), top.tile_fetcher._NYVA_BFETCH_S2.qp(), top.NYXU_FETCH_TRIGn);
  /*p27.LYRY*/ wire LYRY_BFETCH_DONEp = not1(MOCE_BFETCH_DONEn);

  /*p27.TEKY*/ wire _TEKY_SFETCH_REQp = and4(top.sprite_store.FEPO_STORE_MATCHp, _TUKU_WIN_HITn, LYRY_BFETCH_DONEp, _SOWO_SFETCH_RUNNINGn);

  /*p27.SOBU*/ _SOBU_SFETCH_REQp = dff17_B(_TAVA_xBxDxFxH, VYPO, _TEKY_SFETCH_REQp);
  /*p27.SUDA*/ _SUDA_SFETCH_REQp = dff17_A(_LAPE_AxCxExGx, VYPO, _SOBU_SFETCH_REQp.qp());

  /*p27.RYCE*/ wire _RYCE_SFETCH_TRIGp = and2(_SOBU_SFETCH_REQp.qp(),  _SUDA_SFETCH_REQp.qn());

  wire ATEJ_VID_LINE_END_TRIGp = top.lcd_reg.ATEJ_VID_LINE_END_TRIGp();

  // SECA := nor3(RYCE, ROSY, ATEJ) // atej not byha?

  /*p29.TYNO*/ wire TYNO = nand3(_TOXE_SFETCH_S0.qp(), _SEBA_SFETCH_S1_D5.qp(), _VONU_SFETCH_S1_D4.qp());
  /*p29.VUSA*/ wire VUSA_SPRITE_DONEn = or2(_TYFO_SFETCH_S0_D1.qn(), TYNO);
  /*p29.WUTY*/ wire WUTY_SPRITE_DONEp = not1(VUSA_SPRITE_DONEn);

  /*p27.SECA*/ wire _SECA_SFETCH_RUNNING_SETn = nor3(_RYCE_SFETCH_TRIGp, ROSY_VID_RSTp, ATEJ_VID_LINE_END_TRIGp); // def nor
  /*p27.VEKU*/ wire _VEKU_SFETCH_RUNNING_RSTn = nor2(WUTY_SPRITE_DONEp, top.TAVE_PRELOAD_DONE_TRIGp); // def nor
  /*p27.TAKA*/ _TAKA_SFETCH_RUNNINGp = nand_latch(_SECA_SFETCH_RUNNING_SETn, _VEKU_SFETCH_RUNNING_RSTn);

  // TAME := nand(TESE_Q, TOXE_Q);
  // TOMA := nand(LAPE, TAME);
    
  // TOXE := dff17(TOMA,    SECA, TOXE_QN);
  // TULY := dff17(TOXE_QN, SECA, TULY_QN);
  // TESE := dff17(TULY_QN, SECA, TESE_QN);
  // TYFO := dff17(LAPE,    VYPO, TOXE_Q);
  // TOBU := dff17(TAVA,    XYMU, TULY_Q);
  // VONU := dff17(TAVA,    XYMU, TOBU_Q);
  // SEBA := dff17(LAPE,    XYMU, VONU_Q);

  /*p29.TAME*/ wire _TAME_SFETCH_CLK_GATE = nand2(_TESE_SFETCH_S2.qp(), _TOXE_SFETCH_S0.qp());
  /*p29.TOMA*/ wire _TOMA_SFETCH_CLK_xBxDxFxH = nand2(_LAPE_AxCxExGx, _TAME_SFETCH_CLK_GATE);

  /*p29.TOXE*/ _TOXE_SFETCH_S0    = dff17_AB(_TOMA_SFETCH_CLK_xBxDxFxH, _SECA_SFETCH_RUNNING_SETn,      _TOXE_SFETCH_S0.qn());
  /*p29.TYFO*/ _TYFO_SFETCH_S0_D1 = dff17_AB(_LAPE_AxCxExGx,            VYPO,                           _TOXE_SFETCH_S0.qp());

  /*p29.TULY*/ _TULY_SFETCH_S1    = dff17_AB(_TOXE_SFETCH_S0.qn(),      _SECA_SFETCH_RUNNING_SETn,      _TULY_SFETCH_S1.qn());
  /*p29.TOBU*/ _TOBU_SFETCH_S1_D2 = dff17_B (_TAVA_xBxDxFxH,            top.pix_pipe.XYMU_RENDERINGp(), _TULY_SFETCH_S1.qp());
  /*p29.VONU*/ _VONU_SFETCH_S1_D4 = dff17_AB(_TAVA_xBxDxFxH,            top.pix_pipe.XYMU_RENDERINGp(), _TOBU_SFETCH_S1_D2.qp());
  /*p29.SEBA*/ _SEBA_SFETCH_S1_D5 = dff17_B (_LAPE_AxCxExGx,            top.pix_pipe.XYMU_RENDERINGp(), _VONU_SFETCH_S1_D4.qp());

  /*p29.TESE*/ _TESE_SFETCH_S2    = dff17_AB(_TULY_SFETCH_S1.qn(),      _SECA_SFETCH_RUNNING_SETn,      _TESE_SFETCH_S2.qn());
}

//------------------------------------------------------------------------------

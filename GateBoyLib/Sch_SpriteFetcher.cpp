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

void SpriteFetcher::tock(SchematicTop& top) {
  wire VYPO = 1;

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

  /*#p27.SECA*/ wire _SECA_SFETCH_RUNNING_SETn = nor3(_RYCE_SFETCH_TRIGp, _ROSY_VID_RSTp, top.lcd_reg.ATEJ_LINE_TRIGp);
  /* p27.VEKU*/ wire _VEKU_SFETCH_RUNNING_RSTn = nor2(WUTY_SPRITE_DONEp, top.pix_pipe.TAVE_PRELOAD_DONE_TRIGp); // def nor
  /* p27.TAKA*/ TAKA_SFETCH_RUNNINGp.nand_latch(_SECA_SFETCH_RUNNING_SETn, _VEKU_SFETCH_RUNNING_RSTn);

  /*#p29.TAME*/ wire _TAME_SFETCH_CLK_GATE = nand2(TESE_SFETCH_S2.qp17(), TOXE_SFETCH_S0.qp17());
  /*#p29.TOMA*/ wire _TOMA_SFETCH_CLK_xBxDxFxH = nand2(_LAPE_AxCxExGx, _TAME_SFETCH_CLK_GATE);

  /*#p29.TOXE*/ TOXE_SFETCH_S0   .dff17(_TOMA_SFETCH_CLK_xBxDxFxH, _SECA_SFETCH_RUNNING_SETn, TOXE_SFETCH_S0.qn16());
  /*#p29.TYFO*/ TYFO_SFETCH_S0_D1.dff17(_LAPE_AxCxExGx,            VYPO,                     TOXE_SFETCH_S0.qp17());
  /*#p29.TULY*/ TULY_SFETCH_S1   .dff17(TOXE_SFETCH_S0.qn16(),    _SECA_SFETCH_RUNNING_SETn, TULY_SFETCH_S1.qn16());
  /*#p29.TOBU*/ TOBU_SFETCH_S1_D2.dff17(_TAVA_xBxDxFxH,            top.pix_pipe.XYMU_RENDERINGn.qn03(),          TULY_SFETCH_S1.qp17());
  /*#p29.VONU*/ VONU_SFETCH_S1_D4.dff17(_TAVA_xBxDxFxH,            top.pix_pipe.XYMU_RENDERINGn.qn03(),          TOBU_SFETCH_S1_D2.qp17());
  /*#p29.SEBA*/ SEBA_SFETCH_S1_D5.dff17(_LAPE_AxCxExGx,            top.pix_pipe.XYMU_RENDERINGn.qn03(),          VONU_SFETCH_S1_D4.qp17());
  /*#p29.TESE*/ TESE_SFETCH_S2   .dff17(TULY_SFETCH_S1.qn16(),    _SECA_SFETCH_RUNNING_SETn, TESE_SFETCH_S2.qn16());
}

//------------------------------------------------------------------------------

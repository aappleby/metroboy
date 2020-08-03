#include "Sch_SpriteFetcher.h"
#include "Sch_Top.h"

using namespace Schematics;

void SpriteFetcher::dump(Dumper& d) const {
  d("----------SpriteFetch---------\n");
  d("_TAKA_SFETCH_RUNNINGp %c\n", _TAKA_SFETCH_RUNNINGp.c());
  d("_SOBU_SFETCH_REQp     %c\n", _SOBU_SFETCH_REQp    .c());
  d("_SUDA_SFETCH_REQp     %c\n", _SUDA_SFETCH_REQp    .c());
  d("_TOXE_SFETCH_S0       %c\n", _TOXE_SFETCH_S0      .c());
  d("_TYFO_SFETCH_S0_D1    %c\n", _TYFO_SFETCH_S0_D1   .c());
  d("_TULY_SFETCH_S1       %c\n", _TULY_SFETCH_S1      .c());
  d("_TESE_SFETCH_S2       %c\n", _TESE_SFETCH_S2      .c());
  d("_TOBU_SFETCH_S1_D2    %c\n", _TOBU_SFETCH_S1_D2   .c());
  d("_VONU_SFETCH_S1_D4    %c\n", _VONU_SFETCH_S1_D4   .c());
  d("_SEBA_SFETCH_S1_D5    %c\n", _SEBA_SFETCH_S1_D5   .c());
  d("\n");
}


//------------------------------------------------------------------------------

void SpriteFetcher::tick(const SchematicTop& top) {
  _TEPA_RENDERINGn = top.pix_pipe.TEPA_RENDERINGn();
  _LOBY_RENDERINGn = top.pix_pipe.LOBY_RENDERINGn();
}

//------------------------------------------------------------------------------

void SpriteFetcher::tock(SchematicTop& top) {

  /*p01.LAPE*/ wire _LAPE_AxCxExGx = not(top.clk_reg.ALET_xBxDxFxH());
  /*p27.TAVA*/ wire _TAVA_xBxDxFxH = not(_LAPE_AxCxExGx);
  wire VYPO_GND = 0;

  //----------------------------------------
  // So this is def the chunk that watches FEPO_STORE_MATCHp and triggers a sprite fetch...
  // Maybe we should annotate phase starting with the phase 0 = FEPO_MATCH_SYNC goes high?

  {

    /*p27.TUKU*/ wire _TUKU_WIN_HITn = not(top.pix_pipe.TOMU_WIN_HITp());
    /*p27.SOWO*/ wire _SOWO_SFETCH_RUNNINGn = not(_TAKA_SFETCH_RUNNINGp);
    
    /*p27.TEKY*/ wire _TEKY_SFETCH_REQp = and (top.sprite_store.FEPO_STORE_MATCHp, _TUKU_WIN_HITn, top.tile_fetcher.LYRY_BFETCH_DONEp(), _SOWO_SFETCH_RUNNINGn);
    /*p27.SOBU*/ _SOBU_SFETCH_REQp = dff17(_TAVA_xBxDxFxH, VYPO_GND, _TEKY_SFETCH_REQp);
    /*p27.SUDA*/ _SUDA_SFETCH_REQp = dff17(_LAPE_AxCxExGx, VYPO_GND, _SOBU_SFETCH_REQp.q());
    /*p27.RYCE*/ wire _RYCE_SFETCH_TRIGp = and (_SOBU_SFETCH_REQp.q(),  _SUDA_SFETCH_REQp.qn());

    /*p27.SECA*/ wire _SECA_SFETCH_RUNNING_SETn = nor(_RYCE_SFETCH_TRIGp, top.clk_reg.ROSY_VID_RSTp(), top.lcd_reg.BYHA_VID_LINE_END_TRIGn()); // def nor
    /*p27.VEKU*/ wire _VEKU_SFETCH_RUNNING_RSTn = nor(WUTY_SPRITE_DONEp(), top.tile_fetcher.TAVE_PORCH_DONE_TRIGp()); // def nor
    /*p27.TAKA*/ _TAKA_SFETCH_RUNNINGp = nand_latch(_SECA_SFETCH_RUNNING_SETn, _VEKU_SFETCH_RUNNING_RSTn);

    /*p29.TAME*/ wire _TAME_SFETCH_CLK_GATE = nand(_TESE_SFETCH_S2.q(), _TOXE_SFETCH_S0.q());
    /*p29.TOMA*/ wire _TOMA_SFETCH_CLK_xBxDxFxH = nand(_LAPE_AxCxExGx, _TAME_SFETCH_CLK_GATE);
    /*p29.TOXE*/ _TOXE_SFETCH_S0 = dff17(_TOMA_SFETCH_CLK_xBxDxFxH, _SECA_SFETCH_RUNNING_SETn, _TOXE_SFETCH_S0.qn());
    /*p29.TULY*/ _TULY_SFETCH_S1 = dff17(_TOXE_SFETCH_S0.qn(),      _SECA_SFETCH_RUNNING_SETn, _TULY_SFETCH_S1.qn());
    /*p29.TESE*/ _TESE_SFETCH_S2 = dff17(_TULY_SFETCH_S1.qn(),      _SECA_SFETCH_RUNNING_SETn, _TESE_SFETCH_S2.qn());

    /*p29.TYFO*/ _TYFO_SFETCH_S0_D1 = dff17(_LAPE_AxCxExGx, VYPO_GND, _TOXE_SFETCH_S0.q());

    /*p29.TOBU*/ _TOBU_SFETCH_S1_D2 = dff17(_TAVA_xBxDxFxH, top.pix_pipe.XYMU_RENDERINGp(), _TULY_SFETCH_S1.q());    // note input is seq 1 not 2
    /*p29.VONU*/ _VONU_SFETCH_S1_D4 = dff17(_TAVA_xBxDxFxH, top.pix_pipe.XYMU_RENDERINGp(), _TOBU_SFETCH_S1_D2.q());
    /*p29.SEBA*/ _SEBA_SFETCH_S1_D5 = dff17(_LAPE_AxCxExGx, top.pix_pipe.XYMU_RENDERINGp(), _VONU_SFETCH_S1_D4.q());
  }
}

//------------------------------------------------------------------------------

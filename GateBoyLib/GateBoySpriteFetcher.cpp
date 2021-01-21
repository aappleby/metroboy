#include "GateBoyLib/GateBoySpriteFetcher.h"

#include "GateBoyLib/GateBoy.h"

void SpriteFetcher::reset_to_cart() {
  TAKA_SFETCH_RUNNINGp.state = 0b00011000;
  SOBU_SFETCH_REQp.state = 0b00011000;
  SUDA_SFETCH_REQp.state = 0b00011010;
  TOXE_SFETCH_S0p.state = 0b00011011;
  TULY_SFETCH_S1p.state = 0b00011000;
  TESE_SFETCH_S2p.state = 0b00011011;
  TYFO_SFETCH_S0p_D1.state = 0b00011011;
  TOBU_SFETCH_S1p_D2.state = 0b00011000;
  VONU_SFETCH_S1p_D4.state = 0b00011000;
  SEBA_SFETCH_S1p_D5.state = 0b00011010;
}

void GateBoy::tock_sprite_fetcher(wire TEKY_SFETCH_REQp_old) {
  SigIn SIG_VCC = 1;

  /* p27.SUDA*/ sprite_fetcher.SUDA_SFETCH_REQp.dff17(LAPE_AxCxExGx(), SIG_VCC, sprite_fetcher.SOBU_SFETCH_REQp.qp_old());
  /* p27.SOBU*/ sprite_fetcher.SOBU_SFETCH_REQp.dff17(TAVA_xBxDxFxH(), SIG_VCC, TEKY_SFETCH_REQp_old);

  /*#p29.TYFO*/ sprite_fetcher.TYFO_SFETCH_S0p_D1.dff17(LAPE_AxCxExGx(), SIG_VCC,                  sprite_fetcher.TOXE_SFETCH_S0p.qp_old());
  /*#p29.SEBA*/ sprite_fetcher.SEBA_SFETCH_S1p_D5.dff17(LAPE_AxCxExGx(), XYMU_RENDERINGn.qn_new(), sprite_fetcher.VONU_SFETCH_S1p_D4.qp_old());
  /*#p29.VONU*/ sprite_fetcher.VONU_SFETCH_S1p_D4.dff17(TAVA_xBxDxFxH(), XYMU_RENDERINGn.qn_new(), sprite_fetcher.TOBU_SFETCH_S1p_D2.qp_old());
  /*#p29.TOBU*/ sprite_fetcher.TOBU_SFETCH_S1p_D2.dff17(TAVA_xBxDxFxH(), XYMU_RENDERINGn.qn_new(), sprite_fetcher.TULY_SFETCH_S1p.qp_old());

  /* p27.RYCE*/ wire _RYCE_SFETCH_TRIGp = and2(sprite_fetcher.SOBU_SFETCH_REQp.qp_new(), sprite_fetcher.SUDA_SFETCH_REQp.qn_new());
  /*#p27.SECA*/ wire _SECA_SFETCH_RSTn = nor3(_RYCE_SFETCH_TRIGp, ROSY_VID_RSTp(), ATEJ_LINE_RSTp_new());

  // Feedback loop
  for (int feedback = 0; feedback < 2; feedback++) {
    /*#p29.TAME*/ wire _TAME_SFETCH_CLK_GATE = nand2(sprite_fetcher.TESE_SFETCH_S2p.qp_any(), sprite_fetcher.TOXE_SFETCH_S0p.qp_any());
    /*#p29.TOMA*/ wire _TOMA_SFETCH_xBxDxFxH_= nand2(LAPE_AxCxExGx(), _TAME_SFETCH_CLK_GATE);
    /*#p29.TOXE*/ sprite_fetcher.TOXE_SFETCH_S0p.dff17_any(_TOMA_SFETCH_xBxDxFxH_,                  _SECA_SFETCH_RSTn, sprite_fetcher.TOXE_SFETCH_S0p.qn_any());
    /*#p29.TULY*/ sprite_fetcher.TULY_SFETCH_S1p.dff17_any(sprite_fetcher.TOXE_SFETCH_S0p.qn_any(), _SECA_SFETCH_RSTn, sprite_fetcher.TULY_SFETCH_S1p.qn_any());
    /*#p29.TESE*/ sprite_fetcher.TESE_SFETCH_S2p.dff17_any(sprite_fetcher.TULY_SFETCH_S1p.qn_any(), _SECA_SFETCH_RSTn, sprite_fetcher.TESE_SFETCH_S2p.qn_any());
  }

  /* p27.VEKU*/ wire _VEKU_SFETCH_RUNNING_RSTn = nor2(WUTY_SFETCH_DONE_TRIGp(), TAVE_PRELOAD_DONE_TRIGp_new()); // def nor
  /* p27.TAKA*/ sprite_fetcher.TAKA_SFETCH_RUNNINGp.nand_latch(_SECA_SFETCH_RSTn, _VEKU_SFETCH_RUNNING_RSTn);
}
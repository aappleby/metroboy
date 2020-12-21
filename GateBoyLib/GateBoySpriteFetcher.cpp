#include "GateBoyLib/GateBoySpriteFetcher.h"

#include "GateBoyLib/GateBoyResetDebug.h"
#include "GateBoyLib/GateBoyClocks.h"

void SpriteFetcher::reset_to_cart() {
  TAKA_SFETCH_RUNNINGp.state = REG_D0C0;
  SOBU_SFETCH_REQp  .state = REG_D0C0;
  SUDA_SFETCH_REQp  .state = REG_D0C1;
  TOXE_SFETCH_S0p   .state = REG_D1C1;
  TULY_SFETCH_S1p   .state = REG_D0C0;
  TESE_SFETCH_S2p   .state = REG_D1C1;
  TYFO_SFETCH_S0p_D1.state = REG_D1C1;
  TOBU_SFETCH_S1p_D2.state = REG_D0C0;
  VONU_SFETCH_S1p_D4.state = REG_D0C0;
  SEBA_SFETCH_S1p_D5.state = REG_D0C1;
}

void SpriteFetcher::tock(
  GateBoyResetDebug& rst,
  GateBoyClock& clk,
  wire XYMU_RENDERINGp,
  wire ATEJ_LINE_RSTp,
  wire TAVE_PRELOAD_DONE_TRIGp,
  wire TEKY_SFETCH_REQp_old)
{
  wire _VYPO_VCC = 1;

  _XYMU_RENDERINGp.set_new(XYMU_RENDERINGp);

  /* p27.SUDA*/ SUDA_SFETCH_REQp.dff17(clk.LAPE_AxCxExGx(), _VYPO_VCC, SOBU_SFETCH_REQp.qp_old());
  /* p27.SOBU*/ SOBU_SFETCH_REQp.dff17(clk.TAVA_xBxDxFxH(), _VYPO_VCC, TEKY_SFETCH_REQp_old);

  /*#p29.TYFO*/ TYFO_SFETCH_S0p_D1.dff17(clk.LAPE_AxCxExGx(), _VYPO_VCC,       TOXE_SFETCH_S0p.qp_old());
  /*#p29.SEBA*/ SEBA_SFETCH_S1p_D5.dff17(clk.LAPE_AxCxExGx(), XYMU_RENDERINGp, VONU_SFETCH_S1p_D4.qp_old());
  /*#p29.VONU*/ VONU_SFETCH_S1p_D4.dff17(clk.TAVA_xBxDxFxH(), XYMU_RENDERINGp, TOBU_SFETCH_S1p_D2.qp_old());
  /*#p29.TOBU*/ TOBU_SFETCH_S1p_D2.dff17(clk.TAVA_xBxDxFxH(), XYMU_RENDERINGp, TULY_SFETCH_S1p.qp_old());

  /* p27.RYCE*/ wire _RYCE_SFETCH_TRIGp = and2(SOBU_SFETCH_REQp.qp_new(), SUDA_SFETCH_REQp.qn_new());
  /*#p27.SECA*/ wire _SECA_SFETCH_RSTn = nor3(_RYCE_SFETCH_TRIGp, rst.ROSY_VID_RSTp(), ATEJ_LINE_RSTp);

  // Feedback loop
  for (int feedback = 0; feedback < 2; feedback++) {
    /*#p29.TAME*/ wire _TAME_SFETCH_CLK_GATE = nand2(TESE_SFETCH_S2p.qp_any(), TOXE_SFETCH_S0p.qp_any());
    /*#p29.TOMA*/ wire _TOMA_SFETCH_xBxDxFxH_= nand2(clk.LAPE_AxCxExGx(), _TAME_SFETCH_CLK_GATE);
    /*#p29.TOXE*/ TOXE_SFETCH_S0p.dff17_any(_TOMA_SFETCH_xBxDxFxH_,   _SECA_SFETCH_RSTn, TOXE_SFETCH_S0p.qn_any());
    /*#p29.TULY*/ TULY_SFETCH_S1p.dff17_any(TOXE_SFETCH_S0p.qn_any(), _SECA_SFETCH_RSTn, TULY_SFETCH_S1p.qn_any());
    /*#p29.TESE*/ TESE_SFETCH_S2p.dff17_any(TULY_SFETCH_S1p.qn_any(), _SECA_SFETCH_RSTn, TESE_SFETCH_S2p.qn_any());
  }

  /* p27.VEKU*/ wire _VEKU_SFETCH_RUNNING_RSTn = nor2(WUTY_SFETCH_DONE_TRIGp(), TAVE_PRELOAD_DONE_TRIGp); // def nor
  /* p27.TAKA*/ TAKA_SFETCH_RUNNINGp.nand_latch(_SECA_SFETCH_RSTn, _VEKU_SFETCH_RUNNING_RSTn);
}
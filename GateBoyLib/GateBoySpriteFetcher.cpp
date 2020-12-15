#include "GateBoyLib/GateBoySpriteFetcher.h"

#include "GateBoyLib/GateBoyResetDebug.h"
#include "GateBoyLib/GateBoyClocks.h"

void SpriteFetcher::tock(GateBoyResetDebug& rst, GateBoyClock& clk, wire XYMU_RENDERINGp, wire ATEJ_LINE_RSTp) {
  wire _VYPO_VCC = 1;
  /*#p29.TYFO*/ TYFO_SFETCH_S0p_D1.dff17(clk.LAPE_AxCxExGx(), _VYPO_VCC,       TOXE_SFETCH_S0p.qp_old());
  /*#p29.SEBA*/ SEBA_SFETCH_S1p_D5.dff17(clk.LAPE_AxCxExGx(), XYMU_RENDERINGp, VONU_SFETCH_S1p_D4.qp_old());
  /*#p29.VONU*/ VONU_SFETCH_S1p_D4.dff17(clk.TAVA_xBxDxFxH(), XYMU_RENDERINGp, TOBU_SFETCH_S1p_D2.qp_old());
  /*#p29.TOBU*/ TOBU_SFETCH_S1p_D2.dff17(clk.TAVA_xBxDxFxH(), XYMU_RENDERINGp, TULY_SFETCH_S1p.qp_old());

  wire _SECA_SFETCH_RSTn = SECA_SFETCH_RSTn(rst.ROSY_VID_RSTp(), ATEJ_LINE_RSTp);

  /*#p29.TOXE*/ TOXE_SFETCH_S0p.RSTn(_SECA_SFETCH_RSTn);
  /*#p29.TULY*/ TULY_SFETCH_S1p.RSTn(_SECA_SFETCH_RSTn);
  /*#p29.TESE*/ TESE_SFETCH_S2p.RSTn(_SECA_SFETCH_RSTn);

  /*#p29.TAME*/ wire _TAME_SFETCH_CLK_GATE = nand2(TESE_SFETCH_S2p.qp_new(), TOXE_SFETCH_S0p.qp_new());
  /*#p29.TOMA*/ wire _TOMA_SFETCH_xBxDxFxH_= nand2(clk.LAPE_AxCxExGx(), _TAME_SFETCH_CLK_GATE);
  /*#p29.TOXE*/ TOXE_SFETCH_S0p.dff17(_TOMA_SFETCH_xBxDxFxH_,   _SECA_SFETCH_RSTn, TOXE_SFETCH_S0p.qn_new());
  /*#p29.TULY*/ TULY_SFETCH_S1p.dff17(TOXE_SFETCH_S0p.qn_new(), _SECA_SFETCH_RSTn, TULY_SFETCH_S1p.qn_new());
  /*#p29.TESE*/ TESE_SFETCH_S2p.dff17(TULY_SFETCH_S1p.qn_new(), _SECA_SFETCH_RSTn, TESE_SFETCH_S2p.qn_new());

}
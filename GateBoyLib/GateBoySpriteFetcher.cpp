#include "GateBoyLib/GateBoySpriteFetcher.h"

#include "GateBoyLib/GateBoyResetDebug.h"
#include "GateBoyLib/GateBoyClocks.h"

void SpriteFetcher::dump(Dumper& d) {
  d.dump_bitp("TAKA_SFETCH_RUNNINGp", TAKA_SFETCH_RUNNINGp.state);
  d.dump_bitp("SOBU_SFETCH_REQp    ", SOBU_SFETCH_REQp    .state);
  d.dump_bitp("SUDA_SFETCH_REQp    ", SUDA_SFETCH_REQp    .state);
  d.dump_bitp("TOXE_SFETCH_S0      ", TOXE_SFETCH_S0p     .state);
  d.dump_bitp("TULY_SFETCH_S1      ", TULY_SFETCH_S1p     .state);
  d.dump_bitp("TESE_SFETCH_S2      ", TESE_SFETCH_S2p     .state);
  d.dump_bitp("TYFO_SFETCH_S0_D1   ", TYFO_SFETCH_S0p_D1  .state);
  d.dump_bitp("TOBU_SFETCH_S1_D2   ", TOBU_SFETCH_S1p_D2  .state);
  d.dump_bitp("VONU_SFETCH_S1_D4   ", VONU_SFETCH_S1p_D4  .state);
  d.dump_bitp("SEBA_SFETCH_S1_D5   ", SEBA_SFETCH_S1p_D5  .state);
}

void SpriteFetcher::reset_cart() {
  TAKA_SFETCH_RUNNINGp.reset(REG_D0C0);
  SOBU_SFETCH_REQp.reset(REG_D0C0);
  SUDA_SFETCH_REQp.reset(REG_D0C1);
  TOXE_SFETCH_S0p.reset(REG_D1C1);
  TULY_SFETCH_S1p.reset(REG_D0C0);
  TESE_SFETCH_S2p.reset(REG_D1C1);
  TYFO_SFETCH_S0p_D1.reset(REG_D1C1);
  TOBU_SFETCH_S1p_D2.reset(REG_D0C0);
  VONU_SFETCH_S1p_D4.reset(REG_D0C0);
  SEBA_SFETCH_S1p_D5.reset(REG_D0C1);
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

  /*#p29.TOXE*/ TOXE_SFETCH_S0p.RSTn(_SECA_SFETCH_RSTn);
  /*#p29.TULY*/ TULY_SFETCH_S1p.RSTn(_SECA_SFETCH_RSTn);
  /*#p29.TESE*/ TESE_SFETCH_S2p.RSTn(_SECA_SFETCH_RSTn);

  /*#p29.TAME*/ wire _TAME_SFETCH_CLK_GATE = nand2(TESE_SFETCH_S2p.qp_new(), TOXE_SFETCH_S0p.qp_new());
  /*#p29.TOMA*/ wire _TOMA_SFETCH_xBxDxFxH_= nand2(clk.LAPE_AxCxExGx(), _TAME_SFETCH_CLK_GATE);
  /*#p29.TOXE*/ TOXE_SFETCH_S0p.dff17(_TOMA_SFETCH_xBxDxFxH_,   _SECA_SFETCH_RSTn, TOXE_SFETCH_S0p.qn_new());
  /*#p29.TULY*/ TULY_SFETCH_S1p.dff17(TOXE_SFETCH_S0p.qn_new(), _SECA_SFETCH_RSTn, TULY_SFETCH_S1p.qn_new());
  /*#p29.TESE*/ TESE_SFETCH_S2p.dff17(TULY_SFETCH_S1p.qn_new(), _SECA_SFETCH_RSTn, TESE_SFETCH_S2p.qn_new());

  /* p27.VEKU*/ wire _VEKU_SFETCH_RUNNING_RSTn = nor2(WUTY_SFETCH_DONE_TRIGp(), TAVE_PRELOAD_DONE_TRIGp); // def nor
  /* p27.TAKA*/ TAKA_SFETCH_RUNNINGp.nand_latch(_SECA_SFETCH_RSTn, _VEKU_SFETCH_RUNNING_RSTn);
}
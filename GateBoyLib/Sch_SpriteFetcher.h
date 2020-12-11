#pragma once
#include "GateBoyLib/Gates.h"

//-----------------------------------------------------------------------------

struct SpriteFetcher {
  void reset_cart() {
    TAKA_SFETCH_RUNNINGp_xxx.reset(0);
    SOBU_SFETCH_REQp_odd.reset(REG_D0C0);
    SUDA_SFETCH_REQp_evn.reset(REG_D0C1);

    TOXE_SFETCH_S0p_odd.reset(REG_D1C1);
    TYFO_SFETCH_S0p_D1_evn.reset(REG_D1C1);
    TULY_SFETCH_S1p_odd.reset(REG_D0C0);
    TESE_SFETCH_S2p_odd.reset(REG_D1C1);
    TOBU_SFETCH_S1p_D2_odd.reset(REG_D0C0);
    VONU_SFETCH_S1p_D4_odd.reset(REG_D0C0);
    SEBA_SFETCH_S1p_D5_evn.reset(REG_D0C1);
  }

  /*p27.TAKA*/ NandLatch TAKA_SFETCH_RUNNINGp_xxx; // ABCDEFGH Set on odd, cleared on even
  /*p27.SOBU*/ DFF17 SOBU_SFETCH_REQp_odd;         // xBxDxFxH
  /*p27.SUDA*/ DFF17 SUDA_SFETCH_REQp_evn;         // AxBxExGx

  /*p29.TOXE*/ DFF17 TOXE_SFETCH_S0p_odd;          // ABxDxFxH Beginning of line, reset on A. During line, ticks and resets on odd.
  /*p29.TULY*/ DFF17 TULY_SFETCH_S1p_odd;          // ABxDxFxH Beginning of line, reset on A. During line, ticks and resets on odd.
  /*p29.TESE*/ DFF17 TESE_SFETCH_S2p_odd;          // ABxDxFxH Beginning of line, reset on A. During line, ticks and resets on odd.

  /*p29.TYFO*/ DFF17 TYFO_SFETCH_S0p_D1_evn;       // AxCxExGx
  /*p29.TOBU*/ DFF17 TOBU_SFETCH_S1p_D2_odd;       // xBxDxFxH
  /*p29.VONU*/ DFF17 VONU_SFETCH_S1p_D4_odd;       // xBxDxFxH
  /*p29.SEBA*/ DFF17 SEBA_SFETCH_S1p_D5_evn;       // AxCxExGx
};

//-----------------------------------------------------------------------------

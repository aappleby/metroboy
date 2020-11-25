#pragma once
#include "GateBoyLib/Gates.h"

//-----------------------------------------------------------------------------

struct SpriteFetcher {
  void reset_cart() {
    TAKA_SFETCH_RUNNINGp.reset(0);
    SOBU_SFETCH_REQp.reset(REG_D0C0);
    SUDA_SFETCH_REQp.reset(REG_D0C1);

    TOXE_SFETCH_S0p.reset(REG_D1C1);
    TYFO_SFETCH_S0p_D1.reset(REG_D1C1);
    TULY_SFETCH_S1p.reset(REG_D0C0);
    TESE_SFETCH_S2p.reset(REG_D1C1);
    TOBU_SFETCH_S1p_D2.reset(REG_D0C0);
    VONU_SFETCH_S1p_D4.reset(REG_D0C0);
    SEBA_SFETCH_S1p_D5.reset(REG_D0C1);
  }

  void reset_boot() {
    TAKA_SFETCH_RUNNINGp.reset(0);
    SOBU_SFETCH_REQp.reset(REG_D0C0);
    SUDA_SFETCH_REQp.reset(REG_D0C0);

    TOXE_SFETCH_S0p.reset(REG_D0C0);
    TYFO_SFETCH_S0p_D1.reset(REG_D0C0);
    TULY_SFETCH_S1p.reset(REG_D0C0);
    TESE_SFETCH_S2p.reset(REG_D0C0);
    TOBU_SFETCH_S1p_D2.reset(REG_D0C0);
    VONU_SFETCH_S1p_D4.reset(REG_D0C0);
    SEBA_SFETCH_S1p_D5.reset(REG_D0C0);
  }

  // When TOXE & TYFO & VONU & SEBA, sprites are loaded into the pipe.

  /*p27.TAKA*/ NandLatch TAKA_SFETCH_RUNNINGp;
  /*p27.SOBU*/ DFF17 SOBU_SFETCH_REQp;
  /*p27.SUDA*/ DFF17 SUDA_SFETCH_REQp;

  /*p29.TOXE*/ DFF17 TOXE_SFETCH_S0p;
  /*p29.TULY*/ DFF17 TULY_SFETCH_S1p;
  /*p29.TESE*/ DFF17 TESE_SFETCH_S2p;
  /*p29.TYFO*/ DFF17 TYFO_SFETCH_S0p_D1;
  /*p29.TOBU*/ DFF17 TOBU_SFETCH_S1p_D2;
  /*p29.VONU*/ DFF17 VONU_SFETCH_S1p_D4;
  /*p29.SEBA*/ DFF17 SEBA_SFETCH_S1p_D5;
};

//-----------------------------------------------------------------------------

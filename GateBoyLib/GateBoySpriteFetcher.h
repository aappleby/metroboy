#pragma once
#include "GateBoyLib/Gates.h"

#include "GateBoyLib/GateBoyRegisters.h"

//-----------------------------------------------------------------------------

struct SpriteFetcher {
  void reset_to_cart() {
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

  /*p27.TAKA*/ NandLatch TAKA_SFETCH_RUNNINGp; // ABCDEFGH Set on odd, cleared on even
  /*p27.SOBU*/ DFF17 SOBU_SFETCH_REQp;         // xBxDxFxH
  /*p27.SUDA*/ DFF17 SUDA_SFETCH_REQp;         // AxBxExGx

  /*p29.TOXE*/ DFF17 TOXE_SFETCH_S0p;          // ABxDxFxH Beginning of line, reset on A. During line, ticks and resets on odd.
  /*p29.TULY*/ DFF17 TULY_SFETCH_S1p;          // ABxDxFxH Beginning of line, reset on A. During line, ticks and resets on odd.
  /*p29.TESE*/ DFF17 TESE_SFETCH_S2p;          // ABxDxFxH Beginning of line, reset on A. During line, ticks and resets on odd.

  /*p29.TYFO*/ DFF17 TYFO_SFETCH_S0p_D1;       // AxCxExGx
  /*p29.TOBU*/ DFF17 TOBU_SFETCH_S1p_D2;       // xBxDxFxH
  /*p29.VONU*/ DFF17 VONU_SFETCH_S1p_D4;       // xBxDxFxH
  /*p29.SEBA*/ DFF17 SEBA_SFETCH_S1p_D5;       // AxCxExGx
};

//-----------------------------------------------------------------------------

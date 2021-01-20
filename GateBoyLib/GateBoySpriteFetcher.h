#pragma once
#include "GateBoyLib/Gates.h"

#include "GateBoyLib/GateBoyRegisters.h"

//-----------------------------------------------------------------------------

struct SpriteFetcher {
  void reset_to_cart();

  void dump(Dumper& d) {
    d.dump_bitp   ("TAKA_SFETCH_RUNNINGp : ", TAKA_SFETCH_RUNNINGp.state);
    d.dump_bitp   ("SOBU_SFETCH_REQp     : ", SOBU_SFETCH_REQp    .state);
    d.dump_bitp   ("SUDA_SFETCH_REQp     : ", SUDA_SFETCH_REQp    .state);
    d.dump_bitp   ("TOXE_SFETCH_S0       : ", TOXE_SFETCH_S0p     .state);
    d.dump_bitp   ("TULY_SFETCH_S1       : ", TULY_SFETCH_S1p     .state);
    d.dump_bitp   ("TESE_SFETCH_S2       : ", TESE_SFETCH_S2p     .state);
    d.dump_bitp   ("TYFO_SFETCH_S0_D1    : ", TYFO_SFETCH_S0p_D1  .state);
    d.dump_bitp   ("TOBU_SFETCH_S1_D2    : ", TOBU_SFETCH_S1p_D2  .state);
    d.dump_bitp   ("VONU_SFETCH_S1_D4    : ", VONU_SFETCH_S1p_D4  .state);
    d.dump_bitp   ("SEBA_SFETCH_S1_D5    : ", SEBA_SFETCH_S1p_D5  .state);
    d.dump_slice2n("Temp A : ", &sprite_pix_a.REWO_SPRITE_DA0n, 8);
    d.dump_slice2n("Temp B : ", &sprite_pix_b.PEFO_SPRITE_DB0n, 8);
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

  SpritePixA sprite_pix_a;
  SpritePixB sprite_pix_b;
};

//-----------------------------------------------------------------------------

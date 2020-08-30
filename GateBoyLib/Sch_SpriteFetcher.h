#pragma once
#include "GateBoyLib/Cells.h"

namespace Schematics {

struct SchematicTop;
struct VramBus;

//-----------------------------------------------------------------------------

struct SpriteFetcher {
  void tick(const SchematicTop& gb);
  void tock(SchematicTop& gb);
  void dump(Dumper& d) const;

  /* p29.WUTY*/ Sig WUTY_SPRITE_DONEp;   // -> pix pipe, sprite store
  /* p28.WEFY*/ Sig WEFY_SPR_READp;      // -> oam bus
  /* p25.VAPE*/ Sig VAPE_FETCH_OAM_CLK;  // -> oam bus
  /* p25.SOHO*/ Sig SOHO_SPR_VRAM_RDp;   // -> vram bus
  /* p29.TEXY*/ Sig TEXY_SPR_READ_VRAMp; // -> vram bus
  /* p29.ABON*/ Sig ABON_SPR_VRM_RDn;    // -> vram bus
  /*#p29.XUQU*/ Sig XUQU_SPRITE_AB;      // -> vram bus
  /*#p29.RACA*/ Sig RACA_LATCH_SPPIXB;   // -> vram bus
  /*#p29.TOPU*/ Sig TOPU_LATCH_SPPIXA;   // -> vram bus

private:

  // When TOXE & TYFO & VONU & SEBA, sprites are loaded into the pipe.

  /*p27.TAKA*/ Tri    TAKA_SFETCH_RUNNINGp = TRI_D0NP;
  /*p27.SOBU*/ RegQP  SOBU_SFETCH_REQp = REG_D0C0;
  /*p27.SUDA*/ RegQN  SUDA_SFETCH_REQp = REG_D0C0;

  /*p29.TOXE*/ RegQPN TOXE_SFETCH_S0 = REG_D0C0;
  /*p29.TYFO*/ RegQPN TYFO_SFETCH_S0_D1 = REG_D0C0;

  /*p29.TULY*/ RegQPN TULY_SFETCH_S1 = REG_D0C0;
  /*p29.TESE*/ RegQPN TESE_SFETCH_S2 = REG_D0C0;

  /*p29.TOBU*/ RegQP  TOBU_SFETCH_S1_D2 = REG_D0C0;
  /*p29.VONU*/ RegQPN VONU_SFETCH_S1_D4 = REG_D0C0;
  /*p29.SEBA*/ RegQP  SEBA_SFETCH_S1_D5 = REG_D0C0;
};

//-----------------------------------------------------------------------------

}; // namespace Schematics
#pragma once
#include "GateBoyLib/Gates.h"

namespace Schematics {

struct SchematicTop;
struct VramBus;

//-----------------------------------------------------------------------------

struct SpriteFetcher {
  void reset() {}

  void reset_bootrom() {
    WUTY_SPRITE_DONEp.reset(TRI_HZNP);
    WEFY_SPR_READp.reset(TRI_HZNP);
    VAPE_FETCH_OAM_CLK.reset(TRI_HZNP);
    SOHO_SPR_VRAM_RDp.reset(TRI_HZNP);
    TEXY_SPR_READ_VRAMp.reset(TRI_HZNP);
    ABON_SPR_VRM_RDn.reset(TRI_HZNP);
    XUQU_SPRITE_AB.reset(TRI_HZNP);
    RACA_LATCH_SPPIXB.reset(TRI_HZNP);
    TOPU_LATCH_SPPIXA.reset(TRI_HZNP);

    TAKA_SFETCH_RUNNINGp.reset(TRI_D0NP);
    SOBU_SFETCH_REQp.reset(REG_D0C0);
    SUDA_SFETCH_REQp.reset(REG_D0C0);

    TOXE_SFETCH_S0.reset(REG_D0C0);
    TYFO_SFETCH_S0_D1.reset(REG_D0C0);
    TULY_SFETCH_S1.reset(REG_D0C0);
    TESE_SFETCH_S2.reset(REG_D0C0);
    TOBU_SFETCH_S1_D2.reset(REG_D0C0);
    VONU_SFETCH_S1_D4.reset(REG_D0C0);
    SEBA_SFETCH_S1_D5.reset(REG_D0C0);
  }

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

  /*p27.TAKA*/ NandLatch TAKA_SFETCH_RUNNINGp;
  /*p27.SOBU*/ DFF17 SOBU_SFETCH_REQp;
  /*p27.SUDA*/ DFF17 SUDA_SFETCH_REQp;

  /*p29.TOXE*/ DFF17 TOXE_SFETCH_S0;
  /*p29.TYFO*/ DFF17 TYFO_SFETCH_S0_D1;
  /*p29.TULY*/ DFF17 TULY_SFETCH_S1;
  /*p29.TESE*/ DFF17 TESE_SFETCH_S2;
  /*p29.TOBU*/ DFF17 TOBU_SFETCH_S1_D2;
  /*p29.VONU*/ DFF17 VONU_SFETCH_S1_D4;
  /*p29.SEBA*/ DFF17 SEBA_SFETCH_S1_D5;
};

//-----------------------------------------------------------------------------

}; // namespace Schematics
#pragma once
#include "GateBoyLib/Cells.h"

namespace Schematics {

struct SchematicTop;
struct VramBus;

//-----------------------------------------------------------------------------

struct SpriteFetcher {
  void tick(const SchematicTop& gb);
  void tock(wire CLK, SchematicTop& gb);
  void dump(Dumper& d) const;

  /*p27.TAKA*/ Tri _TAKA_SFETCH_RUNNINGp = TRI_D0NP;
  /*p27.SOBU*/ RegQP _SOBU_SFETCH_REQp = REG_D0C0;
  /*p27.SUDA*/ RegQN _SUDA_SFETCH_REQp = REG_D0C0;

  // When TOXE & TYFO & VONU & SEBA, sprites are loaded into the pipe.

  /*p29.TOXE*/ RegQPN _TOXE_SFETCH_S0 = REG_D0C0;
  /*p29.TYFO*/ RegQPN _TYFO_SFETCH_S0_D1 = REG_D0C0;

  /*p29.TULY*/ RegQPN _TULY_SFETCH_S1 = REG_D0C0;
  /*p29.TESE*/ RegQPN _TESE_SFETCH_S2 = REG_D0C0;

  /*p29.TOBU*/ RegQP  _TOBU_SFETCH_S1_D2 = REG_D0C0;
  /*p29.VONU*/ RegQPN _VONU_SFETCH_S1_D4 = REG_D0C0;
  /*p29.SEBA*/ RegQP  _SEBA_SFETCH_S1_D5 = REG_D0C0;

};


//-----------------------------------------------------------------------------

}; // namespace Schematics
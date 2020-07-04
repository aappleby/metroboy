#pragma once
#include "Sch_Common.h"

namespace Schematics {

struct SchematicTop;

//-----------------------------------------------------------------------------

struct SpriteFetcher {
  void tick(SchematicTop& gb);
  SignalHash commit();

private:
  friend struct SchematicTop;

  /*p27.TAKA*/ NandLatch TAKA_SFETCH_RUNNINGp;
  /*p27.SOBU*/ Reg17 SOBU_SPRITE_FETCH_A;
  /*p27.SUDA*/ Reg17 SUDA_SPRITE_FETCH_B;

  // When TOXE & TYFO & VONU & SEBA, sprites are loaded into the pipe.

  /*p29.TOXE*/ Reg17 TOXE_SFETCH_S0;
  /*p29.TYFO*/ Reg17 TYFO_SFETCH_S0_D1;

  /*p29.TULY*/ Reg17 TULY_SFETCH_S1;
  /*p29.TESE*/ Reg17 TESE_SFETCH_S2;

  /*p29.TOBU*/ Reg17 TOBU_SFETCH_S1_D2;
  /*p29.VONU*/ Reg17 VONU_SFETCH_S1_D4;
  /*p29.SEBA*/ Reg17 SEBA_SFETCH_S1_D5;

  /*p33.PEFO*/ Reg8 SPR_PIX_A0;
  /*p33.ROKA*/ Reg8 SPR_PIX_A1;
  /*p33.MYTU*/ Reg8 SPR_PIX_A2;
  /*p33.RAMU*/ Reg8 SPR_PIX_A3;
  /*p33.SELE*/ Reg8 SPR_PIX_A4;
  /*p33.SUTO*/ Reg8 SPR_PIX_A5;
  /*p33.RAMA*/ Reg8 SPR_PIX_A6;
  /*p33.RYDU*/ Reg8 SPR_PIX_A7;

  /*p33.REWO*/ Reg8 SPR_PIX_B0;
  /*p33.PEBA*/ Reg8 SPR_PIX_B1;
  /*p33.MOFO*/ Reg8 SPR_PIX_B2;
  /*p33.PUDU*/ Reg8 SPR_PIX_B3;
  /*p33.SAJA*/ Reg8 SPR_PIX_B4;
  /*p33.SUNY*/ Reg8 SPR_PIX_B5;
  /*p33.SEMO*/ Reg8 SPR_PIX_B6;
  /*p33.SEGA*/ Reg8 SPR_PIX_B7;
};


//-----------------------------------------------------------------------------

}; // namespace Schematics
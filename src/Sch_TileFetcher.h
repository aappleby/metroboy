#pragma once
#include "Cells.h"

namespace Schematics {

struct SchematicTop;

//-----------------------------------------------------------------------------

struct TileFetcher {
  void tick(SchematicTop& top);
  void tock(SchematicTop& top);
  SignalHash commit(SchematicTop& top);

  /*p27.LAXU*/ Reg17 LAXU_BFETCH_S0;
  /*p27.MESU*/ Reg17 MESU_BFETCH_S1;
  /*p27.NYVA*/ Reg17 NYVA_BFETCH_S2;

  /*p27.LOVY*/ Reg17 LOVY_FETCH_DONEp;

  /*p24.NYKA*/ Reg17 NYKA_TILE_FETCH_DONE_Ap;
  /*p24.PORY*/ Reg17 PORY_TILE_FETCH_DONE_Bp;
  /*p27.LYZU*/ Reg17 LYZU_BFETCH_S0_DELAY;

  /*p24.PYGO*/ Reg17 PYGO_FETCH_DONE_Cp;
  /*p24.POKY*/ NorLatch POKY_PORCH_DONEp;            // stops clkpipe until after first tile fetch
  /*p27.LONY*/ NandLatch LONY_BG_READ_VRAM_LATCHp;

  // FF42 - SCY
  /*p23.GAVE*/ Reg9 GAVE_SCY0;
  /*p23.FYMO*/ Reg9 FYMO_SCY1;
  /*p23.FEZU*/ Reg9 FEZU_SCY2;
  /*p23.FUJO*/ Reg9 FUJO_SCY3;
  /*p23.DEDE*/ Reg9 DEDE_SCY4;
  /*p23.FOTY*/ Reg9 FOTY_SCY5;
  /*p23.FOHA*/ Reg9 FOHA_SCY6;
  /*p23.FUNY*/ Reg9 FUNY_SCY7;

  // FF43 - SCX
  /*p23.DATY*/ Reg9 DATY_SCX0;
  /*p23.DUZU*/ Reg9 DUZU_SCX1;
  /*p23.CYXU*/ Reg9 CYXU_SCX2;
  /*p23.GUBO*/ Reg9 GUBO_SCX3;
  /*p23.BEMY*/ Reg9 BEMY_SCX4;
  /*p23.CUZY*/ Reg9 CUZY_SCX5;
  /*p23.CABU*/ Reg9 CABU_SCX6;
  /*p23.BAKE*/ Reg9 BAKE_SCX7;
};

//-----------------------------------------------------------------------------

}; // namespace Schematics
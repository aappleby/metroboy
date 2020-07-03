#pragma once
#include "Sch_Common.h"

namespace Schematics {

struct SchematicTop;

//-----------------------------------------------------------------------------

struct TileFetcher {
  void tick(SchematicTop& gb);
  SignalHash commit();

private:
  friend SchematicTop;

  /*p27.LAXU*/ Reg LAXU_BFETCH_S0;
  /*p27.MESU*/ Reg MESU_BFETCH_S1;
  /*p27.NYVA*/ Reg NYVA_BFETCH_S2;

  /*p27.LOVY*/ Reg LOVY_FETCH_DONEp;

  /*p24.NYKA*/ Reg NYKA_FETCH_DONE_Ap;
  /*p24.PORY*/ Reg PORY_FETCH_DONE_Bp;
  /*p27.LYZU*/ Reg LYZU_BFETCH_S0_DELAY;

  /*p24.PYGO*/ Reg PYGO_FETCH_DONE_Cp;
  /*p24.POKY*/ NorLatch POKY_PORCH_DONEp;            // stops clkpipe until after first tile fetch
  /*p27.LONY*/ NandLatch LONY_BG_READ_VRAM_LATCHp;

  // FF42 - SCY
  /*p23.GAVE*/ Reg GAVE_SCY0;
  /*p23.FYMO*/ Reg FYMO_SCY1;
  /*p23.FEZU*/ Reg FEZU_SCY2;
  /*p23.FUJO*/ Reg FUJO_SCY3;
  /*p23.DEDE*/ Reg DEDE_SCY4;
  /*p23.FOTY*/ Reg FOTY_SCY5;
  /*p23.FOHA*/ Reg FOHA_SCY6;
  /*p23.FUNY*/ Reg FUNY_SCY7;

  // FF43 - SCX
  /*p23.DATY*/ Reg DATY_SCX0;
  /*p23.DUZU*/ Reg DUZU_SCX1;
  /*p23.CYXU*/ Reg CYXU_SCX2;
  /*p23.GUBO*/ Reg GUBO_SCX3;
  /*p23.BEMY*/ Reg BEMY_SCX4;
  /*p23.CUZY*/ Reg CUZY_SCX5;
  /*p23.CABU*/ Reg CABU_SCX6;
  /*p23.BAKE*/ Reg BAKE_SCX7;
};

//-----------------------------------------------------------------------------

}; // namespace Schematics
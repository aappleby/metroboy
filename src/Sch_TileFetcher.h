#pragma once
#include "Cells.h"

namespace Schematics {

struct SchematicTop;
struct VramBus;
struct CpuBus;
struct OamBus;

//-----------------------------------------------------------------------------

struct TileFetcher {
  void tick(const SchematicTop& top);
  void tock(SchematicTop& top, CpuBus& cpu_bus);
  void dump(Dumper& d, const SchematicTop& top) const;

  // FF42 - SCY -> vram bus
  /*p23.GAVE*/ Reg GAVE_SCY0 = REG_D0C0;
  /*p23.FYMO*/ Reg FYMO_SCY1 = REG_D0C0;
  /*p23.FEZU*/ Reg FEZU_SCY2 = REG_D0C0;
  /*p23.FUJO*/ Reg FUJO_SCY3 = REG_D0C0;
  /*p23.DEDE*/ Reg DEDE_SCY4 = REG_D0C0;
  /*p23.FOTY*/ Reg FOTY_SCY5 = REG_D0C0;
  /*p23.FOHA*/ Reg FOHA_SCY6 = REG_D0C0;
  /*p23.FUNY*/ Reg FUNY_SCY7 = REG_D0C0;

  // FF43 - SCX -> ppu, vram bus
  /*p23.DATY*/ Reg DATY_SCX0 = REG_D0C0;
  /*p23.DUZU*/ Reg DUZU_SCX1 = REG_D0C0;
  /*p23.CYXU*/ Reg CYXU_SCX2 = REG_D0C0;
  /*p23.GUBO*/ Reg GUBO_SCX3 = REG_D0C0;
  /*p23.BEMY*/ Reg BEMY_SCX4 = REG_D0C0;
  /*p23.CUZY*/ Reg CUZY_SCX5 = REG_D0C0;
  /*p23.CABU*/ Reg CABU_SCX6 = REG_D0C0;
  /*p23.BAKE*/ Reg BAKE_SCX7 = REG_D0C0;

  /*p27.LAXU*/ RegQPN _LAXU_BFETCH_S0 = REG_D0C0;
  /*p27.MESU*/ RegQPN _MESU_BFETCH_S1 = REG_D0C0;
  /*p27.NYVA*/ RegQPN _NYVA_BFETCH_S2 = REG_D0C0;

  /*p27.LOVY*/ RegQN _LOVY_FETCH_DONEp = REG_D0C0;

  /*p24.NYKA*/ RegQP _NYKA_FETCH_DONE_Ap = REG_D0C0;
  /*p24.PORY*/ RegQP _PORY_FETCH_DONE_Bp = REG_D0C0;
  /*p27.LYZU*/ RegQP _LYZU_BFETCH_S0_DELAY = REG_D0C0;

  /*p24.PYGO*/ RegQP _PYGO_FETCH_DONE_Cp = REG_D0C0;
  /*p24.POKY*/ Tri _POKY_PRELOAD_DONEp = TRI_D0NP;            // stops clkpipe until after first tile fetch
  /*p27.LONY*/ Tri _LONY_BG_READ_VRAM_LATCHp = TRI_D0NP;

};

//-----------------------------------------------------------------------------

}; // namespace Schematics
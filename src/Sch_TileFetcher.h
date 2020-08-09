#pragma once
#include "Cells.h"

namespace Schematics {

struct SchematicTop;
struct VramBus;
struct CpuBus;
struct OamBus;

//-----------------------------------------------------------------------------

struct TileFetcher {
  void tock(SchematicTop& top);
  void dump(Dumper& d, const SchematicTop& top) const;

  /*p27.LAXU*/ RegQPN _LAXU_BFETCH_S0 = REG_D0C0;
  /*p27.MESU*/ RegQPN _MESU_BFETCH_S1 = REG_D0C0;
  /*p27.NYVA*/ RegQPN _NYVA_BFETCH_S2 = REG_D0C0;

  /*p27.LOVY*/ RegQN _LOVY_BG_FETCH_DONEp = REG_D0C0;

  /*p24.NYKA*/ RegQP _NYKA_FETCH_DONE_P11 = REG_D0C0;
  /*p24.PORY*/ RegQP _PORY_FETCH_DONE_P12 = REG_D0C0;
  /*p24.PYGO*/ RegQP _PYGO_FETCH_DONE_P13 = REG_D0C0;
  /*p24.POKY*/ Latch _POKY_PRELOAD_LATCHp = TRI_D0NP; // fires 13 phases into first tile fetch

  /*p27.LYZU*/ RegQP _LYZU_BFETCH_S0_D1 = REG_D0C0;

  /*p27.LONY*/ Latch _LONY_BG_FETCH_RUNNINGp = TRI_D0NP;

};

//-----------------------------------------------------------------------------

}; // namespace Schematics
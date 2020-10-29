#pragma once
#include "GateBoyLib/Gates.h"

namespace Schematics {

struct SchematicTop;
struct VramBus;
struct CpuBus;
struct OamBus;

//-----------------------------------------------------------------------------

struct TileFetcher {
  void reset_cart() {
    POKY_PRELOAD_LATCHp.reset(TRI_D0NP);
    LONY_BG_FETCH_RUNNINGp.reset(TRI_D0NP);

    NYKA_FETCH_DONE_P11.reset(REG_D0C0);
    PORY_FETCH_DONE_P12.reset(REG_D0C1);

    LAXU_BFETCH_S0.reset(REG_D1C1);
    MESU_BFETCH_S1.reset(REG_D0C0);
    NYVA_BFETCH_S2.reset(REG_D1C1);

    LOVY_BG_FETCH_DONEp.reset(REG_D1C1);
    PYGO_FETCH_DONE_P13.reset(REG_D0C0);
    LYZU_BFETCH_S0_D1.reset(REG_D0C0);
  }

  void reset_boot() {
    POKY_PRELOAD_LATCHp.reset(TRI_D0NP);
    LONY_BG_FETCH_RUNNINGp.reset(TRI_D0NP);

    NYKA_FETCH_DONE_P11.reset(REG_D0C0);
    PORY_FETCH_DONE_P12.reset(REG_D0C0);

    LAXU_BFETCH_S0.reset(REG_D0C0);
    MESU_BFETCH_S1.reset(REG_D0C0);
    NYVA_BFETCH_S2.reset(REG_D0C0);

    LOVY_BG_FETCH_DONEp.reset(REG_D0C0);
    PYGO_FETCH_DONE_P13.reset(REG_D0C0);
    LYZU_BFETCH_S0_D1.reset(REG_D0C0);
  }

  /*p24.POKY*/ NorLatch  POKY_PRELOAD_LATCHp;    // -> pix pipe, top. fires 13 phases into first tile fetch
  /*p27.LONY*/ NandLatch LONY_BG_FETCH_RUNNINGp; // -> vram bus

  /*p24.NYKA*/ DFF17 NYKA_FETCH_DONE_P11; // -> top
  /*p24.PORY*/ DFF17 PORY_FETCH_DONE_P12; // -> top

  /*p27.LAXU*/ DFF17 LAXU_BFETCH_S0;
  /*p27.MESU*/ DFF17 MESU_BFETCH_S1;
  /*p27.NYVA*/ DFF17 NYVA_BFETCH_S2;

  /*p27.LOVY*/ DFF17 LOVY_BG_FETCH_DONEp;
  /*p24.PYGO*/ DFF17 PYGO_FETCH_DONE_P13;
  /*p27.LYZU*/ DFF17 LYZU_BFETCH_S0_D1;
};

//-----------------------------------------------------------------------------

}; // namespace Schematics
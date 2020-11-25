#pragma once
#include "GateBoyLib/Gates.h"

//-----------------------------------------------------------------------------

struct TileFetcher {
  void reset_cart() {
    POKY_PRELOAD_LATCHp.reset(0);
    LONY_FETCHINGp.reset(0);

    NYKA_FETCH_DONEp.reset(REG_D0C0);
    PORY_FETCH_DONEp.reset(REG_D0C1);

    LAXU_BFETCH_S0p.reset(REG_D1C1);
    MESU_BFETCH_S1p.reset(REG_D0C0);
    NYVA_BFETCH_S2p.reset(REG_D1C1);

    LOVY_FETCH_DONEp.reset(REG_D1C1);
    PYGO_FETCH_DONEp.reset(REG_D0C0);
    LYZU_BFETCH_S0p_D1.reset(REG_D0C0);
  }

  void reset_boot() {
    POKY_PRELOAD_LATCHp.reset(0);
    LONY_FETCHINGp.reset(0);

    NYKA_FETCH_DONEp.reset(REG_D0C0);
    PORY_FETCH_DONEp.reset(REG_D0C0);

    LAXU_BFETCH_S0p.reset(REG_D0C0);
    MESU_BFETCH_S1p.reset(REG_D0C0);
    NYVA_BFETCH_S2p.reset(REG_D0C0);

    LOVY_FETCH_DONEp.reset(REG_D0C0);
    PYGO_FETCH_DONEp.reset(REG_D0C0);
    LYZU_BFETCH_S0p_D1.reset(REG_D0C0);
  }

  /*p24.POKY*/ NorLatch  POKY_PRELOAD_LATCHp;    // -> pix pipe, top. fires 13 phases into first tile fetch
  /*p27.LONY*/ NandLatch LONY_FETCHINGp; // -> vram bus

  /*p27.LOVY*/ DFF17 LOVY_FETCH_DONEp;
  /*p24.NYKA*/ DFF17 NYKA_FETCH_DONEp; // -> top
  /*p24.PORY*/ DFF17 PORY_FETCH_DONEp; // -> top
  /*p24.PYGO*/ DFF17 PYGO_FETCH_DONEp;

  /*p27.LAXU*/ DFF17 LAXU_BFETCH_S0p;
  /*p27.MESU*/ DFF17 MESU_BFETCH_S1p;
  /*p27.NYVA*/ DFF17 NYVA_BFETCH_S2p;
  /*p27.LYZU*/ DFF17 LYZU_BFETCH_S0p_D1;
};

//-----------------------------------------------------------------------------

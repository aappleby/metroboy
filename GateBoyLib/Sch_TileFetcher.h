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
    LENA_BGW_VRM_RDp.reset(TRI_D0NP);
    MOCE_BFETCH_DONEn.reset(TRI_D0NP);
    NETA_TILE_READp.reset(TRI_D0NP);
    XUHA_FETCH_S2p.reset(TRI_D1NP);

    XUCY_WIN_TILE_READn.reset(TRI_D1NP);
    WUKO_WIN_MAP_READn.reset(TRI_D1NP);
    BAFY_BG_MAP_READn.reset(TRI_D1NP);
    BEJE_BG_TILE_READn.reset(TRI_D1NP);

    METE_LATCH_TILE_DAp.reset(TRI_D0NP);
    LESO_LATCH_TILE_DBn.reset(TRI_D1NP);
    LYRY_BFETCH_DONEp.reset(TRI_D1NP);

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
    LENA_BGW_VRM_RDp.reset(TRI_HZNP);
    MOCE_BFETCH_DONEn.reset(TRI_HZNP);
    NETA_TILE_READp.reset(TRI_HZNP);
    XUHA_FETCH_S2p.reset(TRI_HZNP);

    XUCY_WIN_TILE_READn.reset(TRI_HZNP);
    WUKO_WIN_MAP_READn.reset(TRI_HZNP);
    BAFY_BG_MAP_READn.reset(TRI_HZNP);
    BEJE_BG_TILE_READn.reset(TRI_HZNP);

    METE_LATCH_TILE_DAp.reset(TRI_HZNP);
    LESO_LATCH_TILE_DBn.reset(TRI_HZNP);

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

  /*p27.LENA*/ Sig LENA_BGW_VRM_RDp;  // -> SUTU_MCSn
  /*p27.MOCE*/ Sig MOCE_BFETCH_DONEn; // -> sprite feetcher
  /*p27.NETA*/ Sig NETA_TILE_READp;   // -> vram bus
  /*p27.XUHA*/ Sig XUHA_FETCH_S2p;    // -> vram bus addr mux

  /*p25.XUCY*/ Sig XUCY_WIN_TILE_READn; // -> vram bus
  /*p25.WUKO*/ Sig WUKO_WIN_MAP_READn;// -> vram bus
  /*p26.BAFY*/ Sig BAFY_BG_MAP_READn;// -> vram bus
  /*p26.BEJE*/ Sig BEJE_BG_TILE_READn;// -> vram bus

  /*p32.METE*/ Sig METE_LATCH_TILE_DAp; // -> vram bus
  /*p32.LESO*/ Sig LESO_LATCH_TILE_DBn; // -> vram bus

  /*p27.LYRY*/ Sig LYRY_BFETCH_DONEp;

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
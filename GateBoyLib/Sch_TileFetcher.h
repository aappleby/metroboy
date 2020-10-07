#pragma once
#include "GateBoyLib/Gates.h"

namespace Schematics {

struct SchematicTop;
struct VramBus;
struct CpuBus;
struct OamBus;

//-----------------------------------------------------------------------------

struct TileFetcher {
  void reset() {}
  void reset_bootrom() {}
  void tick(const SchematicTop& top);
  void tock(SchematicTop& top);
  void dump(Dumper& d, const SchematicTop& top) const;

  /* p27.LENA*/ Sig LENA_BGW_VRM_RDp;  // -> SUTU_MCSn
  /* p27.MOCE*/ Sig MOCE_BFETCH_DONEn; // -> sprite feetcher
  /* p27.NETA*/ Sig NETA_TILE_READp;   // -> vram bus
  /*#p27.XUHA*/ Sig XUHA_FETCH_S2p;    // -> vram bus addr mux

  /* p25.XUCY*/ Sig XUCY_WIN_TILE_READn; // -> vram bus
  /*#p25.WUKO*/ Sig WUKO_WIN_MAP_READn;// -> vram bus
  /* p26.BAFY*/ Sig BAFY_BG_MAP_READn;// -> vram bus
  /* p26.BEJE*/ Sig BEJE_BG_TILE_READn;// -> vram bus

  /*#p32.METE*/ Sig METE_LATCH_TILE_DAp; // -> vram bus
  /* p32.LESO*/ Sig LESO_LATCH_TILE_DBn; // -> vram bus

  /*p24.POKY*/ NorLatch  POKY_PRELOAD_LATCHp;    // -> pix pipe, top. fires 13 phases into first tile fetch
  /*p27.LONY*/ NandLatch LONY_BG_FETCH_RUNNINGp; // -> vram bus

  /*p24.NYKA*/ DFF17 NYKA_FETCH_DONE_P11; // -> top
  /*p24.PORY*/ DFF17 PORY_FETCH_DONE_P12; // -> top

private:

  /*p27.LAXU*/ DFF17 LAXU_BFETCH_S0;
  /*p27.MESU*/ DFF17 MESU_BFETCH_S1;
  /*p27.NYVA*/ DFF17 NYVA_BFETCH_S2;

  /*p27.LOVY*/ DFF17 LOVY_BG_FETCH_DONEp;
  /*p24.PYGO*/ DFF17 PYGO_FETCH_DONE_P13;
  /*p27.LYZU*/ DFF17 LYZU_BFETCH_S0_D1;
};

//-----------------------------------------------------------------------------

}; // namespace Schematics
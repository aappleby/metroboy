#pragma once
#include "GateBoyLib/Gates.h"

//-----------------------------------------------------------------------------

struct TileFetcher {
  void reset_cart() {
    POKY_PRELOAD_LATCHp_odd.reset(0);
    LONY_FETCHINGp_xxx.reset(0);

    NYKA_FETCH_DONEp_xxx.reset(REG_D0C0);
    PORY_FETCH_DONEp_xxx.reset(REG_D0C1);

    LAXU_BFETCH_S0p_evn.reset(REG_D1C1);
    MESU_BFETCH_S1p_evn.reset(REG_D0C0);
    NYVA_BFETCH_S2p_evn.reset(REG_D1C1);

    LOVY_FETCH_DONEp_evn.reset(REG_D1C1);
    PYGO_FETCH_DONEp_odd.reset(REG_D0C0);
    LYZU_BFETCH_S0p_D1_odd.reset(REG_D0C0);

    GAVE_SCY0n_h.reset(REG_D1C1);
    FYMO_SCY1n_h.reset(REG_D1C1);
    FEZU_SCY2n_h.reset(REG_D1C1);
    FUJO_SCY3n_h.reset(REG_D1C1);
    DEDE_SCY4n_h.reset(REG_D1C1);
    FOTY_SCY5n_h.reset(REG_D1C1);
    FOHA_SCY6n_h.reset(REG_D1C1);
    FUNY_SCY7n_h.reset(REG_D1C1);

    DATY_SCX0n_h.reset(REG_D1C1);
    DUZU_SCX1n_h.reset(REG_D1C1);
    CYXU_SCX2n_h.reset(REG_D1C1);
    GUBO_SCX3n_h.reset(REG_D1C1);
    BEMY_SCX4n_h.reset(REG_D1C1);
    CUZY_SCX5n_h.reset(REG_D1C1);
    CABU_SCX6n_h.reset(REG_D1C1);
    BAKE_SCX7n_h.reset(REG_D1C1);

    WYKA_WIN_X3_evn.reset(REG_D0C0);
    WODY_WIN_X4_evn.reset(REG_D0C1);
    WOBO_WIN_X5_evn.reset(REG_D0C1);
    WYKO_WIN_X6_evn.reset(REG_D0C1);
    XOLO_WIN_X7_evn.reset(REG_D0C1);

    VYNO_WIN_Y0_evn.reset(REG_D0C1);
    VUJO_WIN_Y1_evn.reset(REG_D0C1);
    VYMU_WIN_Y2_evn.reset(REG_D0C1);
    TUFU_WIN_Y3_evn.reset(REG_D0C1);
    TAXA_WIN_Y4_evn.reset(REG_D0C1);
    TOZO_WIN_Y5_evn.reset(REG_D0C1);
    TATE_WIN_Y6_evn.reset(REG_D0C1);
    TEKE_WIN_Y7_evn.reset(REG_D0C1);

    LEGU_TILE_DA0n_odd.reset(REG_D1C1);
    NUDU_TILE_DA1n_odd.reset(REG_D1C1);
    MUKU_TILE_DA2n_odd.reset(REG_D1C1);
    LUZO_TILE_DA3n_odd.reset(REG_D1C1);
    MEGU_TILE_DA4n_odd.reset(REG_D1C1);
    MYJY_TILE_DA5n_odd.reset(REG_D1C1);
    NASA_TILE_DA6n_odd.reset(REG_D1C1);
    NEFO_TILE_DA7n_odd.reset(REG_D1C1);
    RAWU_TILE_DB0p_odd.reset(REG_D0C1);
    POZO_TILE_DB1p_odd.reset(REG_D0C1);
    PYZO_TILE_DB2p_odd.reset(REG_D0C1);
    POXA_TILE_DB3p_odd.reset(REG_D0C1);
    PULO_TILE_DB4p_odd.reset(REG_D0C1);
    POJU_TILE_DB5p_odd.reset(REG_D0C1);
    POWY_TILE_DB6p_odd.reset(REG_D0C1);
    PYJU_TILE_DB7p_odd.reset(REG_D0C1);
  }

  /*p24.POKY*/ NorLatch  POKY_PRELOAD_LATCHp_odd; // xBxDxFxG
  /*p27.LONY*/ NandLatch LONY_FETCHINGp_xxx;      // Usually changes on even. Changes on odd phase at end of line if we're in a window?

  /*p27.LOVY*/ DFF17 LOVY_FETCH_DONEp_evn;        // AxCxExGx
  /*p24.NYKA*/ DFF17 NYKA_FETCH_DONEp_xxx;        // Usually changes on even. Changes on odd phase at end of line if we're in a window?
  /*p24.PORY*/ DFF17 PORY_FETCH_DONEp_xxx;        // Usually changes on even. Changes on odd phase at end of line if we're in a window?
  /*p24.PYGO*/ DFF17 PYGO_FETCH_DONEp_odd;        // xBxDxFxG

  /*p27.LAXU*/ DFF17 LAXU_BFETCH_S0p_evn;         // AxCxExGx
  /*p27.MESU*/ DFF17 MESU_BFETCH_S1p_evn;         // AxCxExGx
  /*p27.NYVA*/ DFF17 NYVA_BFETCH_S2p_evn;         // AxCxExGx
  /*p27.LYZU*/ DFF17 LYZU_BFETCH_S0p_D1_odd;      // xBxDxFxG

  // FF42 - SCY -> vram bus
  /*p23.GAVE*/ DFF9 GAVE_SCY0n_h;          // xxxxxxxH
  /*p23.FYMO*/ DFF9 FYMO_SCY1n_h;          // xxxxxxxH
  /*p23.FEZU*/ DFF9 FEZU_SCY2n_h;          // xxxxxxxH
  /*p23.FUJO*/ DFF9 FUJO_SCY3n_h;          // xxxxxxxH
  /*p23.DEDE*/ DFF9 DEDE_SCY4n_h;          // xxxxxxxH
  /*p23.FOTY*/ DFF9 FOTY_SCY5n_h;          // xxxxxxxH
  /*p23.FOHA*/ DFF9 FOHA_SCY6n_h;          // xxxxxxxH
  /*p23.FUNY*/ DFF9 FUNY_SCY7n_h;          // xxxxxxxH

  // FF43 - SCX -> ppu, vram bus
  /*p23.DATY*/ DFF9 DATY_SCX0n_h;          // xxxxxxxH
  /*p23.DUZU*/ DFF9 DUZU_SCX1n_h;          // xxxxxxxH
  /*p23.CYXU*/ DFF9 CYXU_SCX2n_h;          // xxxxxxxH
  /*p23.GUBO*/ DFF9 GUBO_SCX3n_h;          // xxxxxxxH
  /*p23.BEMY*/ DFF9 BEMY_SCX4n_h;          // xxxxxxxH
  /*p23.CUZY*/ DFF9 CUZY_SCX5n_h;          // xxxxxxxH
  /*p23.CABU*/ DFF9 CABU_SCX6n_h;          // xxxxxxxH
  /*p23.BAKE*/ DFF9 BAKE_SCX7n_h;          // xxxxxxxH

  // Current window pixel coord
  /*p27.WYKA*/ DFF17 WYKA_WIN_X3_evn;      // AxCxExGx
  /*p27.WODY*/ DFF17 WODY_WIN_X4_evn;      // AxCxExGx
  /*p27.WOBO*/ DFF17 WOBO_WIN_X5_evn;      // AxCxExGx
  /*p27.WYKO*/ DFF17 WYKO_WIN_X6_evn;      // AxCxExGx
  /*p27.XOLO*/ DFF17 XOLO_WIN_X7_evn;      // AxCxExGx

  /*p27.VYNO*/ DFF17 VYNO_WIN_Y0_evn;      // AxCxExGh probably, but not enough data.
  /*p27.VUJO*/ DFF17 VUJO_WIN_Y1_evn;      // AxCxExGh probably, but not enough data.
  /*p27.VYMU*/ DFF17 VYMU_WIN_Y2_evn;      // AxCxExGh probably, but not enough data.
  /*p27.TUFU*/ DFF17 TUFU_WIN_Y3_evn;      // AxCxExGh probably, but not enough data.
  /*p27.TAXA*/ DFF17 TAXA_WIN_Y4_evn;      // AxCxExGh probably, but not enough data.
  /*p27.TOZO*/ DFF17 TOZO_WIN_Y5_evn;      // AxCxExGh probably, but not enough data.
  /*p27.TATE*/ DFF17 TATE_WIN_Y6_evn;      // AxCxExGh probably, but not enough data.
  /*p27.TEKE*/ DFF17 TEKE_WIN_Y7_evn;      // AxCxExGh probably, but not enough data.

  /*p32.LEGU*/ DFF8p LEGU_TILE_DA0n_odd;   // xBxDxFxH
  /*p32.NUDU*/ DFF8p NUDU_TILE_DA1n_odd;   // xBxDxFxH
  /*p32.MUKU*/ DFF8p MUKU_TILE_DA2n_odd;   // xBxDxFxH
  /*p32.LUZO*/ DFF8p LUZO_TILE_DA3n_odd;   // xBxDxFxH
  /*p32.MEGU*/ DFF8p MEGU_TILE_DA4n_odd;   // xBxDxFxH
  /*p32.MYJY*/ DFF8p MYJY_TILE_DA5n_odd;   // xBxDxFxH
  /*p32.NASA*/ DFF8p NASA_TILE_DA6n_odd;   // xBxDxFxH
  /*p32.NEFO*/ DFF8p NEFO_TILE_DA7n_odd;   // xBxDxFxH

  /*p32.RAWU*/ DFF11 RAWU_TILE_DB0p_odd;   // xBxDxFxH
  /*p32.POZO*/ DFF11 POZO_TILE_DB1p_odd;   // xBxDxFxH
  /*p32.PYZO*/ DFF11 PYZO_TILE_DB2p_odd;   // xBxDxFxH
  /*p32.POXA*/ DFF11 POXA_TILE_DB3p_odd;   // xBxDxFxH
  /*p32.PULO*/ DFF11 PULO_TILE_DB4p_odd;   // xBxDxFxH
  /*p32.POJU*/ DFF11 POJU_TILE_DB5p_odd;   // xBxDxFxH
  /*p32.POWY*/ DFF11 POWY_TILE_DB6p_odd;   // xBxDxFxH
  /*p32.PYJU*/ DFF11 PYJU_TILE_DB7p_odd;   // xBxDxFxH
};

//-----------------------------------------------------------------------------

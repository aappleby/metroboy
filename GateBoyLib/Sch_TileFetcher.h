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

    GAVE_SCY0n.reset(REG_D1C1);
    FYMO_SCY1n.reset(REG_D1C1);
    FEZU_SCY2n.reset(REG_D1C1);
    FUJO_SCY3n.reset(REG_D1C1);
    DEDE_SCY4n.reset(REG_D1C1);
    FOTY_SCY5n.reset(REG_D1C1);
    FOHA_SCY6n.reset(REG_D1C1);
    FUNY_SCY7n.reset(REG_D1C1);

    DATY_SCX0n.reset(REG_D1C1);
    DUZU_SCX1n.reset(REG_D1C1);
    CYXU_SCX2n.reset(REG_D1C1);
    GUBO_SCX3n.reset(REG_D1C1);
    BEMY_SCX4n.reset(REG_D1C1);
    CUZY_SCX5n.reset(REG_D1C1);
    CABU_SCX6n.reset(REG_D1C1);
    BAKE_SCX7n.reset(REG_D1C1);

    WYKA_WIN_X3.reset(REG_D0C0);
    WODY_WIN_X4.reset(REG_D0C1);
    WOBO_WIN_X5.reset(REG_D0C1);
    WYKO_WIN_X6.reset(REG_D0C1);
    XOLO_WIN_X7.reset(REG_D0C1);

    VYNO_WIN_Y0.reset(REG_D0C1);
    VUJO_WIN_Y1.reset(REG_D0C1);
    VYMU_WIN_Y2.reset(REG_D0C1);
    TUFU_WIN_Y3.reset(REG_D0C1);
    TAXA_WIN_Y4.reset(REG_D0C1);
    TOZO_WIN_Y5.reset(REG_D0C1);
    TATE_WIN_Y6.reset(REG_D0C1);
    TEKE_WIN_Y7.reset(REG_D0C1);

    XAFO_LCDC_BGMAPn .reset(REG_D1C1);
    WEXU_LCDC_BGTILEn.reset(REG_D0C1);
    WYMO_LCDC_WINENn .reset(REG_D1C1);
    WOKY_LCDC_WINMAPn.reset(REG_D1C1);

    LEGU_TILE_DA0n.reset(REG_D1C1);
    NUDU_TILE_DA1n.reset(REG_D1C1);
    MUKU_TILE_DA2n.reset(REG_D1C1);
    LUZO_TILE_DA3n.reset(REG_D1C1);
    MEGU_TILE_DA4n.reset(REG_D1C1);
    MYJY_TILE_DA5n.reset(REG_D1C1);
    NASA_TILE_DA6n.reset(REG_D1C1);
    NEFO_TILE_DA7n.reset(REG_D1C1);
    RAWU_TILE_DB0p.reset(REG_D0C1);
    POZO_TILE_DB1p.reset(REG_D0C1);
    PYZO_TILE_DB2p.reset(REG_D0C1);
    POXA_TILE_DB3p.reset(REG_D0C1);
    PULO_TILE_DB4p.reset(REG_D0C1);
    POJU_TILE_DB5p.reset(REG_D0C1);
    POWY_TILE_DB6p.reset(REG_D0C1);
    PYJU_TILE_DB7p.reset(REG_D0C1);
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

    GAVE_SCY0n.reset(REG_D0C0);
    FYMO_SCY1n.reset(REG_D0C0);
    FEZU_SCY2n.reset(REG_D0C0);
    FUJO_SCY3n.reset(REG_D0C0);
    DEDE_SCY4n.reset(REG_D0C0);
    FOTY_SCY5n.reset(REG_D0C0);
    FOHA_SCY6n.reset(REG_D0C0);
    FUNY_SCY7n.reset(REG_D0C0);

    DATY_SCX0n.reset(REG_D0C0);
    DUZU_SCX1n.reset(REG_D0C0);
    CYXU_SCX2n.reset(REG_D0C0);
    GUBO_SCX3n.reset(REG_D0C0);
    BEMY_SCX4n.reset(REG_D0C0);
    CUZY_SCX5n.reset(REG_D0C0);
    CABU_SCX6n.reset(REG_D0C0);
    BAKE_SCX7n.reset(REG_D0C0);

    WYKA_WIN_X3.reset(REG_D0C0);
    WODY_WIN_X4.reset(REG_D0C0);
    WOBO_WIN_X5.reset(REG_D0C0);
    WYKO_WIN_X6.reset(REG_D0C0);
    XOLO_WIN_X7.reset(REG_D0C0);

    VYNO_WIN_Y0.reset(REG_D0C0);
    VUJO_WIN_Y1.reset(REG_D0C0);
    VYMU_WIN_Y2.reset(REG_D0C0);
    TUFU_WIN_Y3.reset(REG_D0C0);
    TAXA_WIN_Y4.reset(REG_D0C0);
    TOZO_WIN_Y5.reset(REG_D0C0);
    TATE_WIN_Y6.reset(REG_D0C0);
    TEKE_WIN_Y7.reset(REG_D0C0);

    XAFO_LCDC_BGMAPn.reset(REG_D0C0);
    WEXU_LCDC_BGTILEn.reset(REG_D0C0);
    WYMO_LCDC_WINENn.reset(REG_D0C0);
    WOKY_LCDC_WINMAPn.reset(REG_D0C0);

    LEGU_TILE_DA0n.reset(REG_D0C0);
    NUDU_TILE_DA1n.reset(REG_D0C0);
    MUKU_TILE_DA2n.reset(REG_D0C0);
    LUZO_TILE_DA3n.reset(REG_D0C0);
    MEGU_TILE_DA4n.reset(REG_D0C0);
    MYJY_TILE_DA5n.reset(REG_D0C0);
    NASA_TILE_DA6n.reset(REG_D0C0);
    NEFO_TILE_DA7n.reset(REG_D0C0);
    RAWU_TILE_DB0p.reset(REG_D0C0);
    POZO_TILE_DB1p.reset(REG_D0C0);
    PYZO_TILE_DB2p.reset(REG_D0C0);
    POXA_TILE_DB3p.reset(REG_D0C0);
    PULO_TILE_DB4p.reset(REG_D0C0);
    POJU_TILE_DB5p.reset(REG_D0C0);
    POWY_TILE_DB6p.reset(REG_D0C0);
    PYJU_TILE_DB7p.reset(REG_D0C0);
  }

  //int get_win_x() const     { return pack_u8p(5, &WYKA_WIN_X3); }
  //int get_win_y() const     { return pack_u8p(8, &VYNO_WIN_Y0); }

  /*p24.POKY*/ NorLatch  POKY_PRELOAD_LATCHp; // xBxDxFxG
  /*p27.LONY*/ NandLatch LONY_FETCHINGp;      // ABCDEFGX

  /*p27.LOVY*/ DFF17 LOVY_FETCH_DONEp; // AxCxExGx
  /*p24.NYKA*/ DFF17 NYKA_FETCH_DONEp; // ABCDEFGH
  /*p24.PORY*/ DFF17 PORY_FETCH_DONEp; // ABCDEFGH
  /*p24.PYGO*/ DFF17 PYGO_FETCH_DONEp; // xBxDxFxG

  /*p27.LAXU*/ DFF17 LAXU_BFETCH_S0p;    // AxCxExGx
  /*p27.MESU*/ DFF17 MESU_BFETCH_S1p;    // AxCxExGx
  /*p27.NYVA*/ DFF17 NYVA_BFETCH_S2p;    // AxCxExGx
  /*p27.LYZU*/ DFF17 LYZU_BFETCH_S0p_D1; // xBxDxFxG

  // FF42 - SCY -> vram bus
  /*p23.GAVE*/ DFF9 GAVE_SCY0n; // xxxxxxxH
  /*p23.FYMO*/ DFF9 FYMO_SCY1n; // xxxxxxxH
  /*p23.FEZU*/ DFF9 FEZU_SCY2n; // xxxxxxxH
  /*p23.FUJO*/ DFF9 FUJO_SCY3n; // xxxxxxxH
  /*p23.DEDE*/ DFF9 DEDE_SCY4n; // xxxxxxxH
  /*p23.FOTY*/ DFF9 FOTY_SCY5n; // xxxxxxxH
  /*p23.FOHA*/ DFF9 FOHA_SCY6n; // xxxxxxxH
  /*p23.FUNY*/ DFF9 FUNY_SCY7n; // xxxxxxxH

  // FF43 - SCX -> ppu, vram bus
  /*p23.DATY*/ DFF9 DATY_SCX0n; // xxxxxxxH
  /*p23.DUZU*/ DFF9 DUZU_SCX1n; // xxxxxxxH
  /*p23.CYXU*/ DFF9 CYXU_SCX2n; // xxxxxxxH
  /*p23.GUBO*/ DFF9 GUBO_SCX3n; // xxxxxxxH
  /*p23.BEMY*/ DFF9 BEMY_SCX4n; // xxxxxxxH
  /*p23.CUZY*/ DFF9 CUZY_SCX5n; // xxxxxxxH
  /*p23.CABU*/ DFF9 CABU_SCX6n; // xxxxxxxH
  /*p23.BAKE*/ DFF9 BAKE_SCX7n; // xxxxxxxH

  // Current window pixel coord
  /*p27.WYKA*/ DFF17 WYKA_WIN_X3; // AxCxExGx
  /*p27.WODY*/ DFF17 WODY_WIN_X4; // AxCxExGx
  /*p27.WOBO*/ DFF17 WOBO_WIN_X5; // AxCxExGx
  /*p27.WYKO*/ DFF17 WYKO_WIN_X6; // AxCxExGx
  /*p27.XOLO*/ DFF17 XOLO_WIN_X7; // AxCxExGx

  /*p27.VYNO*/ DFF17 VYNO_WIN_Y0; // AxCxExGh probably, but not enough data.
  /*p27.VUJO*/ DFF17 VUJO_WIN_Y1; // AxCxExGh probably, but not enough data.
  /*p27.VYMU*/ DFF17 VYMU_WIN_Y2; // AxCxExGh probably, but not enough data.
  /*p27.TUFU*/ DFF17 TUFU_WIN_Y3; // AxCxExGh probably, but not enough data.
  /*p27.TAXA*/ DFF17 TAXA_WIN_Y4; // AxCxExGh probably, but not enough data.
  /*p27.TOZO*/ DFF17 TOZO_WIN_Y5; // AxCxExGh probably, but not enough data.
  /*p27.TATE*/ DFF17 TATE_WIN_Y6; // AxCxExGh probably, but not enough data.
  /*p27.TEKE*/ DFF17 TEKE_WIN_Y7; // AxCxExGh probably, but not enough data.

  /*p23.XAFO*/ DFF9 XAFO_LCDC_BGMAPn;  // xxxxxxxH
  /*p23.WEXU*/ DFF9 WEXU_LCDC_BGTILEn; // xxxxxxxH
  /*p23.WYMO*/ DFF9 WYMO_LCDC_WINENn;  // xxxxxxxH
  /*p23.WOKY*/ DFF9 WOKY_LCDC_WINMAPn; // xxxxxxxH

  /*p32.LEGU*/ DFF8p LEGU_TILE_DA0n; // xBxDxFxH
  /*p32.NUDU*/ DFF8p NUDU_TILE_DA1n; // xBxDxFxH
  /*p32.MUKU*/ DFF8p MUKU_TILE_DA2n; // xBxDxFxH
  /*p32.LUZO*/ DFF8p LUZO_TILE_DA3n; // xBxDxFxH
  /*p32.MEGU*/ DFF8p MEGU_TILE_DA4n; // xBxDxFxH
  /*p32.MYJY*/ DFF8p MYJY_TILE_DA5n; // xBxDxFxH
  /*p32.NASA*/ DFF8p NASA_TILE_DA6n; // xBxDxFxH
  /*p32.NEFO*/ DFF8p NEFO_TILE_DA7n; // xBxDxFxH

  /*p32.RAWU*/ DFF11 RAWU_TILE_DB0p; // xBxDxFxH
  /*p32.POZO*/ DFF11 POZO_TILE_DB1p; // xBxDxFxH
  /*p32.PYZO*/ DFF11 PYZO_TILE_DB2p; // xBxDxFxH
  /*p32.POXA*/ DFF11 POXA_TILE_DB3p; // xBxDxFxH
  /*p32.PULO*/ DFF11 PULO_TILE_DB4p; // xBxDxFxH
  /*p32.POJU*/ DFF11 POJU_TILE_DB5p; // xBxDxFxH
  /*p32.POWY*/ DFF11 POWY_TILE_DB6p; // xBxDxFxH
  /*p32.PYJU*/ DFF11 PYJU_TILE_DB7p; // xBxDxFxH
};

//-----------------------------------------------------------------------------

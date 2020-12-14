#pragma once
#include "GateBoyLib/Gates.h"

//-----------------------------------------------------------------------------

struct TileTempA {
  /*p32.LEGU*/ DFF8p LEGU_TILE_DA0n_odd;   // xBxDxFxH
  /*p32.NUDU*/ DFF8p NUDU_TILE_DA1n_odd;   // xBxDxFxH
  /*p32.MUKU*/ DFF8p MUKU_TILE_DA2n_odd;   // xBxDxFxH
  /*p32.LUZO*/ DFF8p LUZO_TILE_DA3n_odd;   // xBxDxFxH
  /*p32.MEGU*/ DFF8p MEGU_TILE_DA4n_odd;   // xBxDxFxH
  /*p32.MYJY*/ DFF8p MYJY_TILE_DA5n_odd;   // xBxDxFxH
  /*p32.NASA*/ DFF8p NASA_TILE_DA6n_odd;   // xBxDxFxH
  /*p32.NEFO*/ DFF8p NEFO_TILE_DA7n_odd;   // xBxDxFxH
};

struct TileTempB {
  /*p32.RAWU*/ DFF11 RAWU_TILE_DB0p_odd;   // xBxDxFxH
  /*p32.POZO*/ DFF11 POZO_TILE_DB1p_odd;   // xBxDxFxH
  /*p32.PYZO*/ DFF11 PYZO_TILE_DB2p_odd;   // xBxDxFxH
  /*p32.POXA*/ DFF11 POXA_TILE_DB3p_odd;   // xBxDxFxH
  /*p32.PULO*/ DFF11 PULO_TILE_DB4p_odd;   // xBxDxFxH
  /*p32.POJU*/ DFF11 POJU_TILE_DB5p_odd;   // xBxDxFxH
  /*p32.POWY*/ DFF11 POWY_TILE_DB6p_odd;   // xBxDxFxH
  /*p32.PYJU*/ DFF11 PYJU_TILE_DB7p_odd;   // xBxDxFxH
};

struct RegSCX {
  // FF43 - SCX -> ppu, vram bus
  /*p23.DATY*/ DFF9 DATY_SCX0n_h;          // xxxxxxxH
  /*p23.DUZU*/ DFF9 DUZU_SCX1n_h;          // xxxxxxxH
  /*p23.CYXU*/ DFF9 CYXU_SCX2n_h;          // xxxxxxxH
  /*p23.GUBO*/ DFF9 GUBO_SCX3n_h;          // xxxxxxxH
  /*p23.BEMY*/ DFF9 BEMY_SCX4n_h;          // xxxxxxxH
  /*p23.CUZY*/ DFF9 CUZY_SCX5n_h;          // xxxxxxxH
  /*p23.CABU*/ DFF9 CABU_SCX6n_h;          // xxxxxxxH
  /*p23.BAKE*/ DFF9 BAKE_SCX7n_h;          // xxxxxxxH
};

struct RegSCY {
  // FF42 - SCY -> vram bus
  /*p23.GAVE*/ DFF9 GAVE_SCY0n_h;          // xxxxxxxH
  /*p23.FYMO*/ DFF9 FYMO_SCY1n_h;          // xxxxxxxH
  /*p23.FEZU*/ DFF9 FEZU_SCY2n_h;          // xxxxxxxH
  /*p23.FUJO*/ DFF9 FUJO_SCY3n_h;          // xxxxxxxH
  /*p23.DEDE*/ DFF9 DEDE_SCY4n_h;          // xxxxxxxH
  /*p23.FOTY*/ DFF9 FOTY_SCY5n_h;          // xxxxxxxH
  /*p23.FOHA*/ DFF9 FOHA_SCY6n_h;          // xxxxxxxH
  /*p23.FUNY*/ DFF9 FUNY_SCY7n_h;          // xxxxxxxH
};

struct WinMapX {
  // Current window map coord
  /*p27.WYKA*/ DFF17 WYKA_WIN_X3_evn;      // AxCxExGx
  /*p27.WODY*/ DFF17 WODY_WIN_X4_evn;      // AxCxExGx
  /*p27.WOBO*/ DFF17 WOBO_WIN_X5_evn;      // AxCxExGx
  /*p27.WYKO*/ DFF17 WYKO_WIN_X6_evn;      // AxCxExGx
  /*p27.XOLO*/ DFF17 XOLO_WIN_X7_evn;      // AxCxExGx
};

struct WinLineY {
  /*p27.VYNO*/ DFF17 VYNO_WIN_Y0_evn;      // AxCxExGh probably, but not enough data.
  /*p27.VUJO*/ DFF17 VUJO_WIN_Y1_evn;      // AxCxExGh probably, but not enough data.
  /*p27.VYMU*/ DFF17 VYMU_WIN_Y2_evn;      // AxCxExGh probably, but not enough data.
  /*p27.TUFU*/ DFF17 TUFU_WIN_Y3_evn;      // AxCxExGh probably, but not enough data.
  /*p27.TAXA*/ DFF17 TAXA_WIN_Y4_evn;      // AxCxExGh probably, but not enough data.
  /*p27.TOZO*/ DFF17 TOZO_WIN_Y5_evn;      // AxCxExGh probably, but not enough data.
  /*p27.TATE*/ DFF17 TATE_WIN_Y6_evn;      // AxCxExGh probably, but not enough data.
  /*p27.TEKE*/ DFF17 TEKE_WIN_Y7_evn;      // AxCxExGh probably, but not enough data.
};

struct TileFetcher {

  void tock2(wire ALET_xBxDxFxH, wire NYXU_BFETCH_RSTn)
  {
    /* p27.LAXU*/ LAXU_BFETCH_S0p_evn.RSTn(NYXU_BFETCH_RSTn);
    /* p27.MESU*/ MESU_BFETCH_S1p_evn.RSTn(NYXU_BFETCH_RSTn);
    /* p27.NYVA*/ NYVA_BFETCH_S2p_evn.RSTn(NYXU_BFETCH_RSTn);

    /* p27.MOCE*/ wire _MOCE_BFETCH_DONEn_mid_any = MOCE_BFETCH_DONEn(NYXU_BFETCH_RSTn);
    /* p27.LEBO*/ wire _LEBO_AxCxExGx = nand2(ALET_xBxDxFxH, _MOCE_BFETCH_DONEn_mid_any);

    /* p27.LAXU*/ LAXU_BFETCH_S0p_evn.dff17(_LEBO_AxCxExGx,               NYXU_BFETCH_RSTn, LAXU_BFETCH_S0p_evn.qn_new());
    /* p27.MESU*/ MESU_BFETCH_S1p_evn.dff17(LAXU_BFETCH_S0p_evn.qn_new(), NYXU_BFETCH_RSTn, MESU_BFETCH_S1p_evn.qn_new());
    /* p27.NYVA*/ NYVA_BFETCH_S2p_evn.dff17(MESU_BFETCH_S1p_evn.qn_new(), NYXU_BFETCH_RSTn, NYVA_BFETCH_S2p_evn.qn_new());
  }

  /* p27.ROMO*/ wire ROMO_PRELOAD_DONEn() const {
    return not1(POKY_PRELOAD_LATCHp_odd.qp_any());
  }

  wire TAVE_PRELOAD_DONE_TRIGp(wire XYMU_RENDERINGp) const {
    /* p27.ROMO*/ wire _ROMO_PRELOAD_DONEn_new_any      = not1(POKY_PRELOAD_LATCHp_odd.qp_any());
    /* p27.SUVU*/ wire _SUVU_PRELOAD_DONE_TRIGn_new_any = nand4(XYMU_RENDERINGp, _ROMO_PRELOAD_DONEn_new_any, NYKA_FETCH_DONEp_xxx.qp_any(), PORY_FETCH_DONEp_xxx.qp_any());
    /* p27.TAVE*/ wire _TAVE_PRELOAD_DONE_TRIGp_new_any = not1(_SUVU_PRELOAD_DONE_TRIGn_new_any);
    return _TAVE_PRELOAD_DONE_TRIGp_new_any;
  }

  /* p27.MOCE*/ wire MOCE_BFETCH_DONEn(wire NYXU_BFETCH_RSTn) const { return nand3(LAXU_BFETCH_S0p_evn.qp_any(), NYVA_BFETCH_S2p_evn.qp_any(), NYXU_BFETCH_RSTn); }
  /* p27.LYRY*/ wire LYRY_BFETCH_DONEp(wire NYXU_BFETCH_RSTn) const { return not1(MOCE_BFETCH_DONEn(NYXU_BFETCH_RSTn)); }

  /* p27.LUSU*/ wire LUSU_FETCHINGn()      const { return not1(LONY_FETCHINGp_xxx.qp_new()); }
  /* p27.MESU*/ wire MESU_BFETCH_S1p()     const { return MESU_BFETCH_S1p_evn.qp_new(); }
  /* p27.NYVA*/ wire NYVA_BFETCH_S2p()     const { return NYVA_BFETCH_S2p_evn.qp_new(); }

  /* p27.LENA*/ wire LENA_BFETCHINGp()     const { return not1(LUSU_FETCHINGn()); }
  /*#p27.NAKO*/ wire NAKO_BFETCH_S1n()     const { return not1(MESU_BFETCH_S1p()); }
  /*#p27.NOFU*/ wire NOFU_BFETCH_S2n()     const { return not1(NYVA_BFETCH_S2p()); }
  /* p27.NOGU*/ wire NOGU_BFETCH_01p()     const { return nand2(NAKO_BFETCH_S1n(), NOFU_BFETCH_S2n()); }
  /* p27.NENY*/ wire NENY_BFETCH_01n()     const { return not1(NOGU_BFETCH_01p()); }
  /* p27.POTU*/ wire POTU_BGW_MAP_READp()  const { return and2(LENA_BFETCHINGp(), NENY_BFETCH_01n()); }
  /* p27.NETA*/ wire NETA_BGW_TILE_READp() const { return and2(LENA_BFETCHINGp(), NOGU_BFETCH_01p()); }

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
};

//-----------------------------------------------------------------------------

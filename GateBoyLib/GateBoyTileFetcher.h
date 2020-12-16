#pragma once
#include "GateBoyLib/Gates.h"

struct GateBoyVramBus;
struct GateBoyClock;

//-----------------------------------------------------------------------------

struct TileTempA {

  void tock(const GateBoyVramBus& vram_bus, wire LOMA_LATCH_TILE_DAn);

  /*p32.LEGU*/ DFF8p LEGU_TILE_DA0n;   // xBxDxFxH
  /*p32.NUDU*/ DFF8p NUDU_TILE_DA1n;   // xBxDxFxH
  /*p32.MUKU*/ DFF8p MUKU_TILE_DA2n;   // xBxDxFxH
  /*p32.LUZO*/ DFF8p LUZO_TILE_DA3n;   // xBxDxFxH
  /*p32.MEGU*/ DFF8p MEGU_TILE_DA4n;   // xBxDxFxH
  /*p32.MYJY*/ DFF8p MYJY_TILE_DA5n;   // xBxDxFxH
  /*p32.NASA*/ DFF8p NASA_TILE_DA6n;   // xBxDxFxH
  /*p32.NEFO*/ DFF8p NEFO_TILE_DA7n;   // xBxDxFxH
};

//-----------------------------------------------------------------------------

struct TileTempB {
  void tock(const GateBoyVramBus& vram_bus, wire _LABU_LATCH_TILE_DBn);

  /*p32.RAWU*/ DFF11 RAWU_TILE_DB0p;   // xBxDxFxH
  /*p32.POZO*/ DFF11 POZO_TILE_DB1p;   // xBxDxFxH
  /*p32.PYZO*/ DFF11 PYZO_TILE_DB2p;   // xBxDxFxH
  /*p32.POXA*/ DFF11 POXA_TILE_DB3p;   // xBxDxFxH
  /*p32.PULO*/ DFF11 PULO_TILE_DB4p;   // xBxDxFxH
  /*p32.POJU*/ DFF11 POJU_TILE_DB5p;   // xBxDxFxH
  /*p32.POWY*/ DFF11 POWY_TILE_DB6p;   // xBxDxFxH
  /*p32.PYJU*/ DFF11 PYJU_TILE_DB7p;   // xBxDxFxH
};

//-----------------------------------------------------------------------------

struct RegSCX {

  void tock(GateBoyResetDebug& rst, GateBoyCpuBus& cpu_bus);

  // FF43 - SCX -> ppu, vram bus
  /*p23.DATY*/ DFF9 DATY_SCX0n;          // xxxxxxxH
  /*p23.DUZU*/ DFF9 DUZU_SCX1n;          // xxxxxxxH
  /*p23.CYXU*/ DFF9 CYXU_SCX2n;          // xxxxxxxH
  /*p23.GUBO*/ DFF9 GUBO_SCX3n;          // xxxxxxxH
  /*p23.BEMY*/ DFF9 BEMY_SCX4n;          // xxxxxxxH
  /*p23.CUZY*/ DFF9 CUZY_SCX5n;          // xxxxxxxH
  /*p23.CABU*/ DFF9 CABU_SCX6n;          // xxxxxxxH
  /*p23.BAKE*/ DFF9 BAKE_SCX7n;          // xxxxxxxH
};

//-----------------------------------------------------------------------------

struct RegSCY {

  void tock(GateBoyResetDebug& rst, GateBoyCpuBus& cpu_bus);

  // FF42 - SCY -> vram bus
  /*p23.GAVE*/ DFF9 GAVE_SCY0n;          // xxxxxxxH
  /*p23.FYMO*/ DFF9 FYMO_SCY1n;          // xxxxxxxH
  /*p23.FEZU*/ DFF9 FEZU_SCY2n;          // xxxxxxxH
  /*p23.FUJO*/ DFF9 FUJO_SCY3n;          // xxxxxxxH
  /*p23.DEDE*/ DFF9 DEDE_SCY4n;          // xxxxxxxH
  /*p23.FOTY*/ DFF9 FOTY_SCY5n;          // xxxxxxxH
  /*p23.FOHA*/ DFF9 FOHA_SCY6n;          // xxxxxxxH
  /*p23.FUNY*/ DFF9 FUNY_SCY7n;          // xxxxxxxH
};

//-----------------------------------------------------------------------------

struct WinMapX {
  void tock(const GateBoyResetDebug& rst, wire TEVO_FETCH_TRIGp, wire PORE_WIN_MODEp, wire WYMO_LCDC_WINENp, wire XAHY_LINE_RSTn);

  // Current window map coord
  /*p27.WYKA*/ DFF17 WYKA_WIN_X3;      // AxCxExGx
  /*p27.WODY*/ DFF17 WODY_WIN_X4;      // AxCxExGx
  /*p27.WOBO*/ DFF17 WOBO_WIN_X5;      // AxCxExGx
  /*p27.WYKO*/ DFF17 WYKO_WIN_X6;      // AxCxExGx
  /*p27.XOLO*/ DFF17 XOLO_WIN_X7;      // AxCxExGx
};

//-----------------------------------------------------------------------------

struct WinLineY {
  void tock(const GateBoyResetDebug& rst, wire PORE_WIN_MODEp, wire PARU_VBLANKp);

  /*p27.VYNO*/ DFF17 VYNO_WIN_Y0;      // AxCxExGh probably, but not enough data.
  /*p27.VUJO*/ DFF17 VUJO_WIN_Y1;      // AxCxExGh probably, but not enough data.
  /*p27.VYMU*/ DFF17 VYMU_WIN_Y2;      // AxCxExGh probably, but not enough data.
  /*p27.TUFU*/ DFF17 TUFU_WIN_Y3;      // AxCxExGh probably, but not enough data.
  /*p27.TAXA*/ DFF17 TAXA_WIN_Y4;      // AxCxExGh probably, but not enough data.
  /*p27.TOZO*/ DFF17 TOZO_WIN_Y5;      // AxCxExGh probably, but not enough data.
  /*p27.TATE*/ DFF17 TATE_WIN_Y6;      // AxCxExGh probably, but not enough data.
  /*p27.TEKE*/ DFF17 TEKE_WIN_Y7;      // AxCxExGh probably, but not enough data.
};

//-----------------------------------------------------------------------------

struct TileFetcher {

  void tock2(GateBoyClock& clk, wire XYMU_RENDERINGp, wire NYXU_BFETCH_RSTn, wire MOCE_BFETCH_DONEn_old);

  /* p27.ROMO*/ wire ROMO_PRELOAD_DONEn() const {
    return not1(POKY_PRELOAD_LATCHp.qp());
  }

  wire TAVE_PRELOAD_DONE_TRIGp() const {
    /* p27.ROMO*/ wire _ROMO_PRELOAD_DONEn      = not1(POKY_PRELOAD_LATCHp.qp());
    /* p27.SUVU*/ wire _SUVU_PRELOAD_DONE_TRIGn = nand4(_XYMU_RENDERINGp.qp(), _ROMO_PRELOAD_DONEn, NYKA_FETCH_DONEp.qp(), PORY_FETCH_DONEp.qp());
    /* p27.TAVE*/ wire _TAVE_PRELOAD_DONE_TRIGp = not1(_SUVU_PRELOAD_DONE_TRIGn);
    return _TAVE_PRELOAD_DONE_TRIGp;
  }

  /* p27.MOCE*/ wire MOCE_BFETCH_DONEn(wire NYXU_BFETCH_RSTn) const { return nand3(_LAXU_BFETCH_S0p.qp(), _NYVA_BFETCH_S2p.qp(), NYXU_BFETCH_RSTn); }
  /* p27.LYRY*/ wire LYRY_BFETCH_DONEp(wire NYXU_BFETCH_RSTn) const { return not1(MOCE_BFETCH_DONEn(NYXU_BFETCH_RSTn)); }

  /* p27.LUSU*/ wire LUSU_FETCHINGn()      const { return not1(LONY_FETCHINGp.qp_new()); }
  /* p27.MESU*/ wire MESU_BFETCH_S1p()     const { return _MESU_BFETCH_S1p.qp_new(); }
  /* p27.NYVA*/ wire NYVA_BFETCH_S2p()     const { return _NYVA_BFETCH_S2p.qp_new(); }

  /* p27.LENA*/ wire LENA_BFETCHINGp()     const { return not1(LUSU_FETCHINGn()); }
  /*#p27.NAKO*/ wire NAKO_BFETCH_S1n()     const { return not1(MESU_BFETCH_S1p()); }
  /*#p27.NOFU*/ wire NOFU_BFETCH_S2n()     const { return not1(NYVA_BFETCH_S2p()); }
  /* p27.NOGU*/ wire NOGU_BFETCH_01p()     const { return nand2(NAKO_BFETCH_S1n(), NOFU_BFETCH_S2n()); }
  /* p27.NENY*/ wire NENY_BFETCH_01n()     const { return not1(NOGU_BFETCH_01p()); }
  /* p27.POTU*/ wire POTU_BGW_MAP_READp()  const { return and2(LENA_BFETCHINGp(), NENY_BFETCH_01n()); }
  /* p27.NETA*/ wire NETA_BGW_TILE_READp() const { return and2(LENA_BFETCHINGp(), NOGU_BFETCH_01p()); }

  /*#p27.XUHA*/ wire XUHA_FETCH_HILOp()    const { return not1(NOFU_BFETCH_S2n()); }

  wire LOMA_LATCH_TILE_DAn() const;
  wire LABU_LATCH_TILE_DBn() const;

  Signal _XYMU_RENDERINGp;

  /*p24.POKY*/ NorLatch  POKY_PRELOAD_LATCHp; // xBxDxFxG
  /*p27.LONY*/ NandLatch LONY_FETCHINGp;      // Usually changes on even. Changes on odd phase at end of line if we're in a window?

  /*p27.LOVY*/ DFF17 LOVY_FETCH_DONEp;        // AxCxExGx
  /*p24.NYKA*/ DFF17 NYKA_FETCH_DONEp;        // Usually changes on even. Changes on odd phase at end of line if we're in a window?
  /*p24.PORY*/ DFF17 PORY_FETCH_DONEp;        // Usually changes on even. Changes on odd phase at end of line if we're in a window?
  /*p24.PYGO*/ DFF17 PYGO_FETCH_DONEp;        // xBxDxFxG

  /*p27.LAXU*/ DFF17 _LAXU_BFETCH_S0p;         // AxCxExGx
  /*p27.MESU*/ DFF17 _MESU_BFETCH_S1p;         // AxCxExGx
  /*p27.NYVA*/ DFF17 _NYVA_BFETCH_S2p;         // AxCxExGx
  /*p27.LYZU*/ DFF17 _LYZU_BFETCH_S0p_D1;      // xBxDxFxG
};

//-----------------------------------------------------------------------------

#pragma once
#include "GateBoyLib/Gates.h"

#include "GateBoyLib/GateBoyRegisters.h"

//-----------------------------------------------------------------------------

struct TileFetcher {
  void tock(GateBoyClock& clk, Bus BUS_VRAM_Dp[8], wire2 XYMU_RENDERINGp, wire2 NYXU_BFETCH_RSTn, wire2 MOCE_BFETCH_DONEn_old);

  wire2 LOMA_LATCH_TILE_DAn() const;
  wire2 LABU_LATCH_TILE_DBn() const;

  /* p27.ROMO*/ wire2 ROMO_PRELOAD_DONEn() const {
    return not1b(POKY_PRELOAD_LATCHp.qp_new());
  }

  wire2 TAVE_PRELOAD_DONE_TRIGp_old() const {
    /* p27.ROMO*/ wire2 _ROMO_PRELOAD_DONEn_old      = not1b(POKY_PRELOAD_LATCHp.qp_old());
    /* p27.SUVU*/ wire2 _SUVU_PRELOAD_DONE_TRIGn_old = nand4b(_XYMU_RENDERINGp.qp_old(), _ROMO_PRELOAD_DONEn_old, NYKA_FETCH_DONEp.qp_old(), PORY_FETCH_DONEp.qp_old());
    /* p27.TAVE*/ wire2 _TAVE_PRELOAD_DONE_TRIGp_old = not1b(_SUVU_PRELOAD_DONE_TRIGn_old);
    return _TAVE_PRELOAD_DONE_TRIGp_old;
  }

  wire2 TAVE_PRELOAD_DONE_TRIGp_new() const {
    /* p27.ROMO*/ wire2 _ROMO_PRELOAD_DONEn_new      = not1b(POKY_PRELOAD_LATCHp.qp_new());
    /* p27.SUVU*/ wire2 _SUVU_PRELOAD_DONE_TRIGn_new = nand4b(_XYMU_RENDERINGp.qp_new(), _ROMO_PRELOAD_DONEn_new, NYKA_FETCH_DONEp.qp_new(), PORY_FETCH_DONEp.qp_new());
    /* p27.TAVE*/ wire2 _TAVE_PRELOAD_DONE_TRIGp_new = not1b(_SUVU_PRELOAD_DONE_TRIGn_new);
    return _TAVE_PRELOAD_DONE_TRIGp_new;
  }

  /* p27.MOCE*/ wire2 MOCE_BFETCH_DONEn_old(wire2 NYXU_BFETCH_RSTn_old) const { return nand3b(_LAXU_BFETCH_S0p.qp_old(), _NYVA_BFETCH_S2p.qp_old(), NYXU_BFETCH_RSTn_old); }
  /* p27.LYRY*/ wire2 LYRY_BFETCH_DONEp_old(wire2 NYXU_BFETCH_RSTn_old) const { return not1b(MOCE_BFETCH_DONEn_old(NYXU_BFETCH_RSTn_old)); }

  /* p27.MOCE*/ wire2 MOCE_BFETCH_DONEn_new(wire2 NYXU_BFETCH_RSTn_new) const { return nand3b(_LAXU_BFETCH_S0p.qp_new(), _NYVA_BFETCH_S2p.qp_new(), NYXU_BFETCH_RSTn_new); }
  /* p27.LYRY*/ wire2 LYRY_BFETCH_DONEp_new(wire2 NYXU_BFETCH_RSTn_new) const { return not1b(MOCE_BFETCH_DONEn_new(NYXU_BFETCH_RSTn_new)); }

  /* p27.MOCE*/ wire2 MOCE_BFETCH_DONEn_any(wire2 NYXU_BFETCH_RSTn_new) const { return nand3b(_LAXU_BFETCH_S0p.qp_any(), _NYVA_BFETCH_S2p.qp_any(), NYXU_BFETCH_RSTn_new); }
  /* p27.LYRY*/ wire2 LYRY_BFETCH_DONEp_any(wire2 NYXU_BFETCH_RSTn_new) const { return not1b(MOCE_BFETCH_DONEn_any(NYXU_BFETCH_RSTn_new)); }

  /* p27.LUSU*/ wire2 LUSU_FETCHINGn()      const { return not1b(LONY_FETCHINGp.qp_new()); }
  /* p27.MESU*/ wire2 MESU_BFETCH_S1p()     const { return _MESU_BFETCH_S1p.qp_new(); }
  /* p27.NYVA*/ wire2 NYVA_BFETCH_S2p()     const { return _NYVA_BFETCH_S2p.qp_new(); }

  /* p27.LENA*/ wire2 LENA_BFETCHINGp()     const { return not1b(LUSU_FETCHINGn()); }
  /*#p27.NAKO*/ wire2 NAKO_BFETCH_S1n()     const { return not1b(MESU_BFETCH_S1p()); }
  /*#p27.NOFU*/ wire2 NOFU_BFETCH_S2n()     const { return not1b(NYVA_BFETCH_S2p()); }
  /* p27.NOGU*/ wire2 NOGU_BFETCH_01p()     const { return nand2b(NAKO_BFETCH_S1n(), NOFU_BFETCH_S2n()); }
  /* p27.NENY*/ wire2 NENY_BFETCH_01n()     const { return not1b(NOGU_BFETCH_01p()); }
  /* p27.POTU*/ wire2 POTU_BGW_MAP_READp()  const { return and2(LENA_BFETCHINGp(), NENY_BFETCH_01n()); }
  /* p27.NETA*/ wire2 NETA_BGW_TILE_READp() const { return and2(LENA_BFETCHINGp(), NOGU_BFETCH_01p()); }

  /*#p27.XUHA*/ wire2 XUHA_FETCH_HILOp()    const { return not1b(NOFU_BFETCH_S2n()); }

  void dump(Dumper& d) {
    d.dump_bitp   ("POKY_PRELOAD_LATCHp : ", POKY_PRELOAD_LATCHp.state);
    d.dump_bitp   ("LONY_FETCHINGp      : ", LONY_FETCHINGp.state);
    d.dump_bitp   ("LOVY_FETCH_DONEp    : ", LOVY_FETCH_DONEp.state);
    d.dump_bitp   ("NYKA_FETCH_DONEp    : ", NYKA_FETCH_DONEp.state);
    d.dump_bitp   ("PORY_FETCH_DONEp    : ", PORY_FETCH_DONEp.state);
    d.dump_bitp   ("PYGO_FETCH_DONEp    : ", PYGO_FETCH_DONEp.state);
    d.dump_bitp   ("LAXU_BFETCH_S0p     : ", _LAXU_BFETCH_S0p.state);
    d.dump_bitp   ("MESU_BFETCH_S1p     : ", _MESU_BFETCH_S1p.state);
    d.dump_bitp   ("NYVA_BFETCH_S2p     : ", _NYVA_BFETCH_S2p.state);
    d.dump_bitp   ("LYZU_BFETCH_S0p_D1  : ", _LYZU_BFETCH_S0p_D1.state);
    d.dump_slice2n("Temp A : ", &tile_temp_a.LEGU_TILE_DA0n, 8);
    d.dump_slice2p("Temp B : ", &tile_temp_b.RAWU_TILE_DB0p, 8);
  }

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

  TileTempA tile_temp_a;
  TileTempB tile_temp_b;
};

//-----------------------------------------------------------------------------

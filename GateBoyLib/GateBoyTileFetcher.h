#pragma once
#include "GateBoyLib/Gates.h"

#include "GateBoyLib/GateBoyRegisters.h"

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

  void dump(Dumper& d) {
    d.dump_bitp   ("POKY_PRELOAD_LATCHp", POKY_PRELOAD_LATCHp.state);
    d.dump_bitp   ("LONY_FETCHINGp     ", LONY_FETCHINGp.state);
    d.dump_bitp   ("LOVY_FETCH_DONEp   ", LOVY_FETCH_DONEp.state);
    d.dump_bitp   ("NYKA_FETCH_DONEp   ", NYKA_FETCH_DONEp.state);
    d.dump_bitp   ("PORY_FETCH_DONEp   ", PORY_FETCH_DONEp.state);
    d.dump_bitp   ("PYGO_FETCH_DONEp   ", PYGO_FETCH_DONEp.state);
    d("\n");
    d.dump_bitp   ("LAXU_BFETCH_S0p    ", _LAXU_BFETCH_S0p.state);
    d.dump_bitp   ("MESU_BFETCH_S1p    ", _MESU_BFETCH_S1p.state);
    d.dump_bitp   ("NYVA_BFETCH_S2p    ", _NYVA_BFETCH_S2p.state);
    d.dump_bitp   ("LYZU_BFETCH_S0p_D1 ", _LYZU_BFETCH_S0p_D1.state);
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

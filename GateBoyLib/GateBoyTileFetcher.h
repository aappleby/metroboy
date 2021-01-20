#pragma once
#include "GateBoyLib/Gates.h"

#include "GateBoyLib/GateBoyRegisters.h"

//-----------------------------------------------------------------------------

struct TileFetcher {

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

  NorLatch _XYMU_RENDERINGn;

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

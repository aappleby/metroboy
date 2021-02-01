#pragma once
#include "GateBoyLib/Gates.h"

#include "GateBoyLib/GateBoyRegisters.h"

//-----------------------------------------------------------------------------

struct TileFetcher {

  /* p27.LYRY*/ Gate LYRY_BFETCH_DONEp;         // old used
  /* p24.POKY*/ NorLatch  POKY_PRELOAD_LATCHp;  // xBxDxFxG
  /* p27.LONY*/ NandLatch LONY_FETCHINGp;       // Usually changes on even. Changes on odd phase at end of line if we're in a window?

  /* p27.LOVY*/ DFF17 LOVY_FETCH_DONEp;         // AxCxExGx
  /* p24.NYKA*/ DFF17 NYKA_FETCH_DONEp;         // Usually changes on even. Changes on odd phase at end of line if we're in a window?
  /* p24.PORY*/ DFF17 PORY_FETCH_DONEp;         // Usually changes on even. Changes on odd phase at end of line if we're in a window?
  /* p24.PYGO*/ DFF17 PYGO_FETCH_DONEp;         // xBxDxFxG

  /* p27.LAXU*/ DFF17 _LAXU_BFETCH_S0p;         // AxCxExGx
  /* p27.MESU*/ DFF17 _MESU_BFETCH_S1p;         // AxCxExGx
  /* p27.NYVA*/ DFF17 _NYVA_BFETCH_S2p;         // AxCxExGx
  /* p27.LYZU*/ DFF17 LYZU_BFETCH_S0p_D1;      // xBxDxFxG
};

//-----------------------------------------------------------------------------

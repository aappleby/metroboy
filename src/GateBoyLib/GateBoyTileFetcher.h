#pragma once
#include "GateBoyLib/Regs.h"

//-----------------------------------------------------------------------------

struct TileFetchCounter {
  void reset();

  /*_p27.LAXU*/ DFF17 LAXU_BFETCH_S0p_odd;         // AxCxExGx
  /*_p27.MESU*/ DFF17 MESU_BFETCH_S1p_odd;         // AxCxExGx
  /*_p27.NYVA*/ DFF17 NYVA_BFETCH_S2p_odd;         // AxCxExGx
};

//-----------------------------------------------------------------------------

struct TileFetchControl {
  void reset();

  /*_p27.LYRY*/ Gate LYRY_BFETCH_DONEp_odd;         // old used
  /*_p24.POKY*/ NorLatch  POKY_PRELOAD_LATCHp_evn;  // xBxDxFxG
  /*_p27.LONY*/ NandLatch LONY_TFETCHINGp;       // Usually changes on even. Changes on odd phase at end of line if we're in a window?

  /*_p27.LOVY*/ DFF17 LOVY_TFETCH_DONEp;         // AxCxExGx
  /*_p24.NYKA*/ DFF17 NYKA_FETCH_DONEp_evn;         // Usually changes on even. Changes on odd phase at end of line if we're in a window?
  /*_p24.PORY*/ DFF17 PORY_FETCH_DONEp_odd;         // Usually changes on even. Changes on odd phase at end of line if we're in a window?
  /*_p24.PYGO*/ DFF17 PYGO_FETCH_DONEp_evn;         // xBxDxFxG

  /*_p27.LYZU*/ DFF17 LYZU_BFETCH_S0p_D1;      // xBxDxFxG
};

//-----------------------------------------------------------------------------

#pragma once
#include "GateBoyLib/Gates.h"

//-----------------------------------------------------------------------------

struct TileFetchCounter {
  void reset_to_poweron() {
    LAXU_BFETCH_S0p_odd.state = 0b00011000;
    MESU_BFETCH_S1p_odd.state = 0b00011000;
    NYVA_BFETCH_S2p_odd.state = 0b00011000;
  }

  void reset_to_bootrom() {
    LAXU_BFETCH_S0p_odd.state = 0b00011011;
    MESU_BFETCH_S1p_odd.state = 0b00011000;
    NYVA_BFETCH_S2p_odd.state = 0b00011011;
  }

  void reset_to_cart() {
    LAXU_BFETCH_S0p_odd.state = 0b00011011;
    MESU_BFETCH_S1p_odd.state = 0b00011000;
    NYVA_BFETCH_S2p_odd.state = 0b00011011;
  }

  /*_p27.LAXU*/ DFF17 LAXU_BFETCH_S0p_odd;         // AxCxExGx
  /*_p27.MESU*/ DFF17 MESU_BFETCH_S1p_odd;         // AxCxExGx
  /*_p27.NYVA*/ DFF17 NYVA_BFETCH_S2p_odd;         // AxCxExGx
};


struct TileFetchControl {
  void reset_to_poweron() {
    LYRY_BFETCH_DONEp_odd.state   = 0b00011000;
    POKY_PRELOAD_LATCHp_evn.state = 0b00011000;
    LONY_FETCHINGp.state      = 0b00011000;
    LOVY_FETCH_DONEp.state    = 0b00011000;
    NYKA_FETCH_DONEp_evn.state    = 0b00011000;
    PORY_FETCH_DONEp_odd.state    = 0b00011000;
    PYGO_FETCH_DONEp_evn.state    = 0b00011000;
    LYZU_BFETCH_S0p_D1.state  = 0b00011000;
  }

  void reset_to_bootrom() {
    LYRY_BFETCH_DONEp_odd.state   = 0b00011001;
    POKY_PRELOAD_LATCHp_evn.state = 0b00011000;
    LONY_FETCHINGp.state      = 0b00011000;
    LOVY_FETCH_DONEp.state    = 0b00011011;
    NYKA_FETCH_DONEp_evn.state    = 0b00011000;
    PORY_FETCH_DONEp_odd.state    = 0b00011010;
    PYGO_FETCH_DONEp_evn.state    = 0b00011000;
    LYZU_BFETCH_S0p_D1.state  = 0b00011000;
  }

  void reset_to_cart() {
    LYRY_BFETCH_DONEp_odd.state   = 0b00011001;
    POKY_PRELOAD_LATCHp_evn.state = 0b00011000;
    LONY_FETCHINGp.state      = 0b00011000;
    LOVY_FETCH_DONEp.state    = 0b00011011;
    NYKA_FETCH_DONEp_evn.state    = 0b00011000;
    PORY_FETCH_DONEp_odd.state    = 0b00011010;
    PYGO_FETCH_DONEp_evn.state    = 0b00011000;
    LYZU_BFETCH_S0p_D1.state  = 0b00011000;
  }

  /*_p27.LYRY*/ Gate LYRY_BFETCH_DONEp_odd;         // old used
  /*_p24.POKY*/ NorLatch  POKY_PRELOAD_LATCHp_evn;  // xBxDxFxG
  /*_p27.LONY*/ NandLatch LONY_FETCHINGp;       // Usually changes on even. Changes on odd phase at end of line if we're in a window?

  /*_p27.LOVY*/ DFF17 LOVY_FETCH_DONEp;         // AxCxExGx
  /*_p24.NYKA*/ DFF17 NYKA_FETCH_DONEp_evn;         // Usually changes on even. Changes on odd phase at end of line if we're in a window?
  /*_p24.PORY*/ DFF17 PORY_FETCH_DONEp_odd;         // Usually changes on even. Changes on odd phase at end of line if we're in a window?
  /*_p24.PYGO*/ DFF17 PYGO_FETCH_DONEp_evn;         // xBxDxFxG

  /*_p27.LYZU*/ DFF17 LYZU_BFETCH_S0p_D1;      // xBxDxFxG
};

//-----------------------------------------------------------------------------

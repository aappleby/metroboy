#include "GateBoyLib/GateBoyTileFetcher.h"

#include "GateBoyLib/GateBoy.h"
#include "GateBoyLib/Gates.h"

//-----------------------------------------------------------------------------

void TileFetchCounter::reset() {
  LAXU_BFETCH_S0p_odd.state = 0b00011011;
  MESU_BFETCH_S1p_odd.state = 0b00011000;
  NYVA_BFETCH_S2p_odd.state = 0b00011011;
}

//-----------------------------------------------------------------------------

void TileFetchControl::reset() {
  LYRY_BFETCH_DONEp_odd.state   = 0b00011001;
  POKY_PRELOAD_LATCHp_evn.state = 0b00011000;
  LONY_TFETCHINGp.state         = 0b00011000;
  LOVY_TFETCH_DONEp.state       = 0b00011001;
  NYKA_FETCH_DONEp_evn.state    = 0b00011010;
  PORY_FETCH_DONEp_odd.state    = 0b00011000;
  PYGO_FETCH_DONEp_evn.state    = 0b00011010;
  LYZU_BFETCH_S0p_D1.state      = 0b00011010;
}

//-----------------------------------------------------------------------------

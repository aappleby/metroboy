#include "GateBoyLib/GateBoySpriteFetcher.h"

#include "GateBoyLib/GateBoy.h"
#include "GateBoyLib/Gates.h"

//-----------------------------------------------------------------------------

void SpriteFetchCounter::reset() {
  TOXE_SFETCH_S0p_evn.state = 0b00011011;
  TULY_SFETCH_S1p_evn.state = 0b00011000;
  TESE_SFETCH_S2p_evn.state = 0b00011011;
}

//-----------------------------------------------------------------------------

void SpriteFetchControl::reset() {
  WUTY_SFETCH_DONE_TRIGp_odd.state = 0b00011000;
  TEXY_SFETCHINGp_evn.state        = 0b00011000;
  TAKA_SFETCH_RUNNINGp_evn.state   = 0b00011000;
  SOBU_SFETCH_REQp_evn.state       = 0b00011010;
  SUDA_SFETCH_REQp_odd.state       = 0b00011000;
  TYFO_SFETCH_S0p_D1_odd.state     = 0b00011001;
  TOBU_SFETCH_S1p_D2_evn.state     = 0b00011010;
  VONU_SFETCH_S1p_D4_evn.state     = 0b00011010;
  SEBA_SFETCH_S1p_D5_odd.state     = 0b00011000;
}

//-----------------------------------------------------------------------------

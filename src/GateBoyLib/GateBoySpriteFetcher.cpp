#include "GateBoyLib/GateBoySpriteFetcher.h"

#include "GateBoyLib/GateBoy.h"
#include "GateBoyLib/Gates.h"

//-----------------------------------------------------------------------------

void SpriteFetchCounter::reset_to_poweron() {
  TOXE_SFETCH_S0p_evn.state = 0b00011000;
  TULY_SFETCH_S1p_evn.state = 0b00011000;
  TESE_SFETCH_S2p_evn.state = 0b00011000;
}

void SpriteFetchCounter::reset_to_bootrom() {
  TOXE_SFETCH_S0p_evn.state = 0b00011000;
  TULY_SFETCH_S1p_evn.state = 0b00011010;
  TESE_SFETCH_S2p_evn.state = 0b00011010;
}

void SpriteFetchCounter::reset_to_cart() {
  TOXE_SFETCH_S0p_evn.state = 0b00011011;
  TULY_SFETCH_S1p_evn.state = 0b00011000;
  TESE_SFETCH_S2p_evn.state = 0b00011011;
}

//-----------------------------------------------------------------------------

void SpriteFetchControl::reset_to_poweron() {
  WUTY_SFETCH_DONE_TRIGp.state = 0b00011000;
  TEXY_SFETCHINGp_evn.state        = 0b00011000;
  TAKA_SFETCH_RUNNINGp_evn.state   = 0b00011000;
  SOBU_SFETCH_REQp_evn.state       = 0b00011000;
  SUDA_SFETCH_REQp_odd.state       = 0b00011000;
  TYFO_SFETCH_S0p_D1_odd.state     = 0b00011000;
  TOBU_SFETCH_S1p_D2_evn.state     = 0b00011000;
  VONU_SFETCH_S1p_D4_evn.state     = 0b00011000;
  SEBA_SFETCH_S1p_D5_odd.state     = 0b00011000;
}

void SpriteFetchControl::reset_to_bootrom() {
  WUTY_SFETCH_DONE_TRIGp.state = 0b00011000;
  TEXY_SFETCHINGp_evn.state        = 0b00011000;
  TAKA_SFETCH_RUNNINGp_evn.state   = 0b00011001;
  SOBU_SFETCH_REQp_evn.state       = 0b00011000;
  SUDA_SFETCH_REQp_odd.state       = 0b00011010;
  TYFO_SFETCH_S0p_D1_odd.state     = 0b00011010;
  TOBU_SFETCH_S1p_D2_evn.state     = 0b00011000;
  VONU_SFETCH_S1p_D4_evn.state     = 0b00011000;
  SEBA_SFETCH_S1p_D5_odd.state     = 0b00011010;
}

void SpriteFetchControl::reset_to_cart() {
  WUTY_SFETCH_DONE_TRIGp.state = 0b00011000;
  TEXY_SFETCHINGp_evn.state        = 0b00011000;
  TAKA_SFETCH_RUNNINGp_evn.state   = 0b00011000;
  SOBU_SFETCH_REQp_evn.state       = 0b00011000;
  SUDA_SFETCH_REQp_odd.state       = 0b00011010;
  TYFO_SFETCH_S0p_D1_odd.state     = 0b00011011;
  TOBU_SFETCH_S1p_D2_evn.state     = 0b00011000;
  VONU_SFETCH_S1p_D4_evn.state     = 0b00011000;
  SEBA_SFETCH_S1p_D5_odd.state     = 0b00011010;
}

//-----------------------------------------------------------------------------

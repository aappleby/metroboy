#include "GateBoyLib/GateBoySpriteScanner.h"

#include "GateBoyLib/Utils.h"

//-----------------------------------------------------------------------------

void SpriteScanner::reset()    {
  FETO_SCAN_DONEp.state       = 0b00011001;
  AVAP_SCAN_DONE_tp_odd.state = 0b00011000;
  BESU_SCAN_DONEn_odd.state   = 0b00011000;
  CENO_SCAN_DONEn_odd.state   = 0b00011000;
  DEZY_INC_COUNTn_odd.state   = 0b00011001;
  BYBA_SCAN_DONEp_odd.state   = 0b00011001;
  DOBA_SCAN_DONEp_evn.state   = 0b00011011;
}

//-----------------------------------------------------------------------------

void ScanCounter::reset() {
  YFEL_SCAN0_odd.state = 0b00011011;
  WEWY_SCAN1_odd.state = 0b00011001;
  GOSO_SCAN2_odd.state = 0b00011001;
  ELYN_SCAN3_odd.state = 0b00011000;
  FAHA_SCAN4_odd.state = 0b00011010;
  FONY_SCAN5_odd.state = 0b00011011;
}

//-----------------------------------------------------------------------------

void SpriteIndex::reset() {
  XADU_SPRITE_IDX0p_odd.state = 0b00011000;
  XEDY_SPRITE_IDX1p_odd.state = 0b00011000;
  ZUZE_SPRITE_IDX2p_odd.state = 0b00011001;
  XOBE_SPRITE_IDX3p_odd.state = 0b00011000;
  YDUF_SPRITE_IDX4p_odd.state = 0b00011001;
  XECU_SPRITE_IDX5p_odd.state = 0b00011000;
}

//-----------------------------------------------------------------------------

void SpriteCounter::reset()    {
  BESE_SPRITE_COUNT0_odd.state = BIT_OLD | BIT_DRIVEN | BIT_CLOCK;
  CUXY_SPRITE_COUNT1_odd.state = BIT_OLD | BIT_DRIVEN | BIT_CLOCK;
  BEGO_SPRITE_COUNT2_odd.state = BIT_OLD | BIT_DRIVEN | BIT_CLOCK;
  DYBE_SPRITE_COUNT3_odd.state = BIT_OLD | BIT_DRIVEN | BIT_CLOCK;
}

//-----------------------------------------------------------------------------

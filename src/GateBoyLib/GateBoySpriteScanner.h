#pragma once
#include "GateBoyLib/Regs.h"
#include "GateBoyLib/Utils.h"

//-----------------------------------------------------------------------------

struct SpriteScanner {
  void reset_to_poweron() { bit_init(*this, BIT_OLD | BIT_DRIVEN, 0x00); }
  void reset_to_bootrom() {
    FETO_SCAN_DONEp.state      = 0b00011000;
    AVAP_SCAN_DONE_tp_odd.state = 0b00011000;
    BESU_SCAN_DONEn_odd.state       = 0b00011000;
    CENO_SCAN_DONEn_odd.state       = 0b00011000;
    DEZY_INC_COUNTn_odd.state      = 0b00011010;
    BYBA_SCAN_DONEp_odd.state    = 0b00011000;
    DOBA_SCAN_DONEp_evn.state    = 0b00011000;
  }
  void reset_to_cart()    {
    FETO_SCAN_DONEp.state      = 0b00011001;
    AVAP_SCAN_DONE_tp_odd.state = 0b00011000;
    BESU_SCAN_DONEn_odd.state       = 0b00011000;
    CENO_SCAN_DONEn_odd.state       = 0b00011010;
    DEZY_INC_COUNTn_odd.state      = 0b00011011;
    BYBA_SCAN_DONEp_odd.state    = 0b00011011;
    DOBA_SCAN_DONEp_evn.state    = 0b00011001;
  }

  /*#p28.FETO*/ Gate FETO_SCAN_DONEp;
  /*#p29.AVAP*/ Gate AVAP_SCAN_DONE_tp_odd;
  /*#p28.BESU*/ NorLatch BESU_SCAN_DONEn_odd;
  /*#p29.CENO*/ DFF17 CENO_SCAN_DONEn_odd;
  /*_p29.DEZY*/ DFF17 DEZY_INC_COUNTn_odd;
  /*#p29.BYBA*/ DFF17 BYBA_SCAN_DONEp_odd;
  /*#p29.DOBA*/ DFF17 DOBA_SCAN_DONEp_evn;
};

//-----------------------------------------------------------------------------
// Scan counter tracks which of the 40 sprites in OAM we're scanning.

struct ScanCounter {
  void reset_to_poweron() {
    YFEL_SCAN0_odd.state = 0b00011000;
    WEWY_SCAN1_odd.state = 0b00011000;
    GOSO_SCAN2_odd.state = 0b00011000;
    ELYN_SCAN3_odd.state = 0b00011000;
    FAHA_SCAN4_odd.state = 0b00011000;
    FONY_SCAN5_odd.state = 0b00011000;
  }

  void reset_to_bootrom() {
    YFEL_SCAN0_odd.state = 0b00011000;
    WEWY_SCAN1_odd.state = 0b00011010;
    GOSO_SCAN2_odd.state = 0b00011010;
    ELYN_SCAN3_odd.state = 0b00011010;
    FAHA_SCAN4_odd.state = 0b00011010;
    FONY_SCAN5_odd.state = 0b00011010;
  }

  void reset_to_cart() {
    YFEL_SCAN0_odd.state = 0b00011011;
    WEWY_SCAN1_odd.state = 0b00011001;
    GOSO_SCAN2_odd.state = 0b00011001;
    ELYN_SCAN3_odd.state = 0b00011000;
    FAHA_SCAN4_odd.state = 0b00011010;
    FONY_SCAN5_odd.state = 0b00011011;
  }

  /*#p28.YFEL*/ DFF17 YFEL_SCAN0_odd;
  /*_p28.WEWY*/ DFF17 WEWY_SCAN1_odd;
  /*_p28.GOSO*/ DFF17 GOSO_SCAN2_odd;
  /*_p28.ELYN*/ DFF17 ELYN_SCAN3_odd;
  /*_p28.FAHA*/ DFF17 FAHA_SCAN4_odd;
  /*_p28.FONY*/ DFF17 FONY_SCAN5_odd;
};

//-----------------------------------------------------------------------------
// Sprite index selects which store to write

struct SpriteIndex {
  void reset_to_poweron() { bit_init(*this, BIT_OLD | BIT_DRIVEN, 0x00); }
  void reset_to_bootrom() { bit_init(*this, BIT_OLD | BIT_DRIVEN, 0x00); }
  void reset_to_cart()    {
    XADU_SPRITE_IDX0p_odd.state = BIT_OLD | BIT_DRIVEN | BIT_CLOCK;
    XEDY_SPRITE_IDX1p_odd.state = BIT_OLD | BIT_DRIVEN | BIT_CLOCK;
    ZUZE_SPRITE_IDX2p_odd.state = BIT_OLD | BIT_DRIVEN | BIT_CLOCK | 1;
    XOBE_SPRITE_IDX3p_odd.state = BIT_OLD | BIT_DRIVEN | BIT_CLOCK;
    YDUF_SPRITE_IDX4p_odd.state = BIT_OLD | BIT_DRIVEN | BIT_CLOCK | 1;
    XECU_SPRITE_IDX5p_odd.state = BIT_OLD | BIT_DRIVEN | BIT_CLOCK;
  }

  /*_p30.XADU*/ DFF13 XADU_SPRITE_IDX0p_odd;
  /*_p30.XEDY*/ DFF13 XEDY_SPRITE_IDX1p_odd;
  /*_p30.ZUZE*/ DFF13 ZUZE_SPRITE_IDX2p_odd;
  /*_p30.XOBE*/ DFF13 XOBE_SPRITE_IDX3p_odd;
  /*_p30.YDUF*/ DFF13 YDUF_SPRITE_IDX4p_odd;
  /*_p30.XECU*/ DFF13 XECU_SPRITE_IDX5p_odd;
};

//-----------------------------------------------------------------------------
// Sprite counter tracks how many sprites have been selected for this line.

struct SpriteCounter {
  void reset_to_poweron() { bit_init(*this, BIT_OLD | BIT_DRIVEN | BIT_CLOCK, 0x00); }
  void reset_to_bootrom() {
    BESE_SPRITE_COUNT0_odd.state = BIT_OLD | BIT_DRIVEN;
    CUXY_SPRITE_COUNT1_odd.state = BIT_OLD | BIT_DRIVEN | BIT_CLOCK;
    BEGO_SPRITE_COUNT2_odd.state = BIT_OLD | BIT_DRIVEN | BIT_CLOCK;
    DYBE_SPRITE_COUNT3_odd.state = BIT_OLD | BIT_DRIVEN | BIT_CLOCK;
  }
  void reset_to_cart()    {
    BESE_SPRITE_COUNT0_odd.state = BIT_OLD | BIT_DRIVEN | BIT_CLOCK;
    CUXY_SPRITE_COUNT1_odd.state = BIT_OLD | BIT_DRIVEN | BIT_CLOCK;
    BEGO_SPRITE_COUNT2_odd.state = BIT_OLD | BIT_DRIVEN | BIT_CLOCK;
    DYBE_SPRITE_COUNT3_odd.state = BIT_OLD | BIT_DRIVEN | BIT_CLOCK;
  }

  /*_p29.BESE*/ DFF17 BESE_SPRITE_COUNT0_odd; // AxxxExxx
  /*_p29.CUXY*/ DFF17 CUXY_SPRITE_COUNT1_odd; // AxxxExxx
  /*_p29.BEGO*/ DFF17 BEGO_SPRITE_COUNT2_odd; // AxxxExxx
  /*_p29.DYBE*/ DFF17 DYBE_SPRITE_COUNT3_odd; // AxxxExxx
};

//-----------------------------------------------------------------------------

#pragma once
#include "GateBoyLib/Gates.h"

//-----------------------------------------------------------------------------

struct SpriteScanner {
  void reset_to_poweron() { bit_init(*this, BIT_OLD | BIT_DRIVEN, 0x00); }
  void reset_to_bootrom() {
    FETO_SCAN_DONEp.state      = 0b00011000;
    AVAP_SCAN_DONE_TRIGp.state = 0b00011000;
    BESU_SCAN_DONEn.state       = 0b00011000;
    CENO_SCAN_DONEn.state       = 0b00011000;
    DEZY_INC_COUNTn.state      = 0b00011010;
    BYBA_SCAN_DONEp.state    = 0b00011000;
    DOBA_SCAN_DONEp.state    = 0b00011000;
  }
  void reset_to_cart()    {
    FETO_SCAN_DONEp.state      = 0b00011001;
    AVAP_SCAN_DONE_TRIGp.state = 0b00011000;
    BESU_SCAN_DONEn.state       = 0b00011000;
    CENO_SCAN_DONEn.state       = 0b00011010;
    DEZY_INC_COUNTn.state      = 0b00011011;
    BYBA_SCAN_DONEp.state    = 0b00011011;
    DOBA_SCAN_DONEp.state    = 0b00011001;
  }

  /*#p28.FETO*/ Gate FETO_SCAN_DONEp;
  /*#p29.AVAP*/ Gate AVAP_SCAN_DONE_TRIGp;
  /*#p28.BESU*/ NorLatch BESU_SCAN_DONEn;
  /*#p29.CENO*/ DFF17 CENO_SCAN_DONEn;
  /*_p29.DEZY*/ DFF17 DEZY_INC_COUNTn;
  /*#p29.BYBA*/ DFF17 BYBA_SCAN_DONEp;
  /*#p29.DOBA*/ DFF17 DOBA_SCAN_DONEp;
};

//-----------------------------------------------------------------------------
// Scan counter tracks which of the 40 sprites in OAM we're scanning.

struct ScanCounter {
  void reset_to_poweron() {
    YFEL_SCAN0.state = 0b00011000;
    WEWY_SCAN1.state = 0b00011000;
    GOSO_SCAN2.state = 0b00011000;
    ELYN_SCAN3.state = 0b00011000;
    FAHA_SCAN4.state = 0b00011000;
    FONY_SCAN5.state = 0b00011000;
  }

  void reset_to_bootrom() {
    YFEL_SCAN0.state = 0b00011000;
    WEWY_SCAN1.state = 0b00011010;
    GOSO_SCAN2.state = 0b00011010;
    ELYN_SCAN3.state = 0b00011010;
    FAHA_SCAN4.state = 0b00011010;
    FONY_SCAN5.state = 0b00011010;
  }

  void reset_to_cart() {
    YFEL_SCAN0.state = 0b00011011;
    WEWY_SCAN1.state = 0b00011001;
    GOSO_SCAN2.state = 0b00011001;
    ELYN_SCAN3.state = 0b00011000;
    FAHA_SCAN4.state = 0b00011010;
    FONY_SCAN5.state = 0b00011011;
  }

  /*#p28.YFEL*/ DFF17 YFEL_SCAN0;
  /*_p28.WEWY*/ DFF17 WEWY_SCAN1;
  /*_p28.GOSO*/ DFF17 GOSO_SCAN2;
  /*_p28.ELYN*/ DFF17 ELYN_SCAN3;
  /*_p28.FAHA*/ DFF17 FAHA_SCAN4;
  /*_p28.FONY*/ DFF17 FONY_SCAN5;
};

//-----------------------------------------------------------------------------
// Sprite index selects which store to write

struct SpriteIndex {
  void reset_to_poweron() { bit_init(*this, BIT_OLD | BIT_DRIVEN, 0x00); }
  void reset_to_bootrom() { bit_init(*this, BIT_OLD | BIT_DRIVEN, 0x00); }
  void reset_to_cart()    {
    XADU_SPRITE_IDX0p.state = BIT_OLD | BIT_DRIVEN | BIT_CLOCK;
    XEDY_SPRITE_IDX1p.state = BIT_OLD | BIT_DRIVEN | BIT_CLOCK;
    ZUZE_SPRITE_IDX2p.state = BIT_OLD | BIT_DRIVEN | BIT_CLOCK | 1;
    XOBE_SPRITE_IDX3p.state = BIT_OLD | BIT_DRIVEN | BIT_CLOCK;
    YDUF_SPRITE_IDX4p.state = BIT_OLD | BIT_DRIVEN | BIT_CLOCK | 1;
    XECU_SPRITE_IDX5p.state = BIT_OLD | BIT_DRIVEN | BIT_CLOCK;
  }

  /*_p30.XADU*/ DFF13 XADU_SPRITE_IDX0p;
  /*_p30.XEDY*/ DFF13 XEDY_SPRITE_IDX1p;
  /*_p30.ZUZE*/ DFF13 ZUZE_SPRITE_IDX2p;
  /*_p30.XOBE*/ DFF13 XOBE_SPRITE_IDX3p;
  /*_p30.YDUF*/ DFF13 YDUF_SPRITE_IDX4p;
  /*_p30.XECU*/ DFF13 XECU_SPRITE_IDX5p;
};

//-----------------------------------------------------------------------------
// Sprite counter tracks how many sprites have been selected for this line.

struct SpriteCounter {
  void reset_to_poweron() { bit_init(*this, BIT_OLD | BIT_DRIVEN | BIT_CLOCK, 0x00); }
  void reset_to_bootrom() {
    BESE_SPRITE_COUNT0.state = BIT_OLD | BIT_DRIVEN;
    CUXY_SPRITE_COUNT1.state = BIT_OLD | BIT_DRIVEN | BIT_CLOCK;
    BEGO_SPRITE_COUNT2.state = BIT_OLD | BIT_DRIVEN | BIT_CLOCK;
    DYBE_SPRITE_COUNT3.state = BIT_OLD | BIT_DRIVEN | BIT_CLOCK;
  }
  void reset_to_cart()    {
    BESE_SPRITE_COUNT0.state = BIT_OLD | BIT_DRIVEN | BIT_CLOCK;
    CUXY_SPRITE_COUNT1.state = BIT_OLD | BIT_DRIVEN | BIT_CLOCK;
    BEGO_SPRITE_COUNT2.state = BIT_OLD | BIT_DRIVEN | BIT_CLOCK;
    DYBE_SPRITE_COUNT3.state = BIT_OLD | BIT_DRIVEN | BIT_CLOCK;
  }

  /*_p29.BESE*/ DFF17 BESE_SPRITE_COUNT0; // AxxxExxx
  /*_p29.CUXY*/ DFF17 CUXY_SPRITE_COUNT1; // AxxxExxx
  /*_p29.BEGO*/ DFF17 BEGO_SPRITE_COUNT2; // AxxxExxx
  /*_p29.DYBE*/ DFF17 DYBE_SPRITE_COUNT3; // AxxxExxx
};

//-----------------------------------------------------------------------------

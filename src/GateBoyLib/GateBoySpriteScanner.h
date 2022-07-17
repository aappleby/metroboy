#pragma once
#include "GateBoyLib/Regs.h"

//-----------------------------------------------------------------------------

struct SpriteScanner {
  void reset();

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
  void reset();

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
  void reset();

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
  void reset();

  /*_p29.BESE*/ DFF17 BESE_SPRITE_COUNT0_odd; // AxxxExxx
  /*_p29.CUXY*/ DFF17 CUXY_SPRITE_COUNT1_odd; // AxxxExxx
  /*_p29.BEGO*/ DFF17 BEGO_SPRITE_COUNT2_odd; // AxxxExxx
  /*_p29.DYBE*/ DFF17 DYBE_SPRITE_COUNT3_odd; // AxxxExxx
};

//-----------------------------------------------------------------------------

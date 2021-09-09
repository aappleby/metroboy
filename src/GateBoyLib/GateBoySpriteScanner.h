#pragma once
#include "GateBoyLib/Gates.h"

//------------------------------------------------------------------------------------------------------------------------

struct SpriteScanner {
  void reset_to_cart() {
    BESU_SCANNINGn.state = 0b00011000;
    CENO_SCANNINGn.state = 0b00011010;
    BYBA_SCAN_DONE_Ap.state = 0b00011011;
    DOBA_SCAN_DONE_Bp.state = 0b00011001;
  }

  /*#p28.ACYL*/ Gate ACYL_SCANNINGp; // new used
  /*#p28.FETO*/ Gate FETO_SCAN_DONEp; // old used
  /*#p29.AVAP*/ Gate AVAP_SCAN_DONE_TRIGp; // new used

  /*#p28.BESU*/ NorLatch BESU_SCANNINGn; // Axxxxxxx
  /*#p29.CENO*/ DFF17 CENO_SCANNINGn;    // xxxxExxx
  /*#p29.BYBA*/ DFF17 BYBA_SCAN_DONE_Ap; // Axxxxxxx
  /*#p29.DOBA*/ DFF17 DOBA_SCAN_DONE_Bp; // ABxxxxxx Cleared on A, set on B
};

//------------------------------------------------------------------------------------------------------------------------

struct ScanCounter {
  void reset_to_cart() {
    YFEL_SCAN0.state = 0b00011011;
    WEWY_SCAN1.state = 0b00011001;
    GOSO_SCAN2.state = 0b00011001;
    ELYN_SCAN3.state = 0b00011000;
    FAHA_SCAN4.state = 0b00011010;
    FONY_SCAN5.state = 0b00011011;
  }

  /*#p28.YFEL*/ DFF17 YFEL_SCAN0;        // AxxxExxx
  /*_p28.WEWY*/ DFF17 WEWY_SCAN1;        // Axxxxxxx
  /*_p28.GOSO*/ DFF17 GOSO_SCAN2;        // Axxxxxxx
  /*_p28.ELYN*/ DFF17 ELYN_SCAN3;        // Axxxxxxx
  /*_p28.FAHA*/ DFF17 FAHA_SCAN4;        // Axxxxxxx
  /*_p28.FONY*/ DFF17 FONY_SCAN5;        // Axxxxxxx
};

//------------------------------------------------------------------------------------------------------------------------
// Sprite index selects which store to write

struct SpriteIndex {
  void reset_to_cart() {
    XADU_SPRITE_IDX0p.state = 0b00011010;
    XEDY_SPRITE_IDX1p.state = 0b00011010;
    ZUZE_SPRITE_IDX2p.state = 0b00011011;
    XOBE_SPRITE_IDX3p.state = 0b00011010;
    YDUF_SPRITE_IDX4p.state = 0b00011011;
    XECU_SPRITE_IDX5p.state = 0b00011010;
  }

  /*_p30.XADU*/ DFF13 XADU_SPRITE_IDX0p; // AxxxExxx
  /*_p30.XEDY*/ DFF13 XEDY_SPRITE_IDX1p; // AxxxExxx
  /*_p30.ZUZE*/ DFF13 ZUZE_SPRITE_IDX2p; // AxxxExxx
  /*_p30.XOBE*/ DFF13 XOBE_SPRITE_IDX3p; // AxxxExxx
  /*_p30.YDUF*/ DFF13 YDUF_SPRITE_IDX4p; // AxxxExxx
  /*_p30.XECU*/ DFF13 XECU_SPRITE_IDX5p; // AxxxExxx
};

//------------------------------------------------------------------------------------------------------------------------

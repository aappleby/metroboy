#pragma once
#include "GateBoyLib/Gates.h"

//-----------------------------------------------------------------------------

struct SpriteScanner {
  void reset_cart() {
    BESU_SCANNINGp.reset(0);
    CENO_SCANNINGp.reset(REG_D0C1);

    YFEL_SCAN0.reset(REG_D1C1);
    WEWY_SCAN1.reset(REG_D1C0);
    GOSO_SCAN2.reset(REG_D1C0);
    ELYN_SCAN3.reset(REG_D0C0);
    FAHA_SCAN4.reset(REG_D0C1);
    FONY_SCAN5.reset(REG_D1C1);

    BYBA_SCAN_DONE_Ap.reset(REG_D1C1);
    DOBA_SCAN_DONE_Bp.reset(REG_D1C0);

    XADU_SPRITE_IDX0p.reset(REG_D0C1);
    XEDY_SPRITE_IDX1p.reset(REG_D0C1);
    ZUZE_SPRITE_IDX2p.reset(REG_D1C1);
    XOBE_SPRITE_IDX3p.reset(REG_D0C1);
    YDUF_SPRITE_IDX4p.reset(REG_D1C1);
    XECU_SPRITE_IDX5p.reset(REG_D0C1);
  }

  void reset_boot() {
    BESU_SCANNINGp.reset(0);
    CENO_SCANNINGp.reset(REG_D0C0);

    YFEL_SCAN0.reset(REG_D0C0);
    WEWY_SCAN1.reset(REG_D0C0);
    GOSO_SCAN2.reset(REG_D0C0);
    ELYN_SCAN3.reset(REG_D0C0);
    FAHA_SCAN4.reset(REG_D0C0);
    FONY_SCAN5.reset(REG_D0C0);

    BYBA_SCAN_DONE_Ap.reset(REG_D0C0);
    DOBA_SCAN_DONE_Bp.reset(REG_D0C0);

    XADU_SPRITE_IDX0p.reset(REG_D0C0);
    XEDY_SPRITE_IDX1p.reset(REG_D0C0);
    ZUZE_SPRITE_IDX2p.reset(REG_D0C0);
    XOBE_SPRITE_IDX3p.reset(REG_D0C0);
    YDUF_SPRITE_IDX4p.reset(REG_D0C0);
    XECU_SPRITE_IDX5p.reset(REG_D0C0);
  }

  //----------------------------------------
  // States

  /*p28.BESU*/ NorLatch BESU_SCANNINGp; // -> oam bus
  /*p29.CENO*/ DFF17 CENO_SCANNINGp;    // -> oam bus
  /*p29.BYBA*/ DFF17 BYBA_SCAN_DONE_Ap;
  /*p29.DOBA*/ DFF17 DOBA_SCAN_DONE_Bp;

  /*p28.YFEL*/ DFF17 YFEL_SCAN0; // -> oam bus address mux
  /*p28.WEWY*/ DFF17 WEWY_SCAN1;
  /*p28.GOSO*/ DFF17 GOSO_SCAN2;
  /*p28.ELYN*/ DFF17 ELYN_SCAN3;
  /*p28.FAHA*/ DFF17 FAHA_SCAN4;
  /*p28.FONY*/ DFF17 FONY_SCAN5;

  /*p30.XADU*/ DFF13 XADU_SPRITE_IDX0p;
  /*p30.XEDY*/ DFF13 XEDY_SPRITE_IDX1p;
  /*p30.ZUZE*/ DFF13 ZUZE_SPRITE_IDX2p;
  /*p30.XOBE*/ DFF13 XOBE_SPRITE_IDX3p;
  /*p30.YDUF*/ DFF13 YDUF_SPRITE_IDX4p;
  /*p30.XECU*/ DFF13 XECU_SPRITE_IDX5p;

  // switching these to DFF8n breajs sprite4_*_b
};

//-----------------------------------------------------------------------------

#pragma once
#include "GateBoyLib/Gates.h"

struct OamTempA;
struct RegLY;

//------------------------------------------------------------------------------------------------------------------------

struct SpriteDeltaY {

  wire GESE_SCAN_MATCH_Yp(wire XYMO_LCDC_SPSIZEn);

  /* p29.DEGE*/ wire DEGE_SPRITE_DELTA0;
  /* p29.DABY*/ wire DABY_SPRITE_DELTA1;
  /* p29.DABU*/ wire DABU_SPRITE_DELTA2;
  /* p29.GYSA*/ wire GYSA_SPRITE_DELTA3;
  /* p29.GACE*/ wire GACE_SPRITE_DELTA4;
  /* p29.GUVU*/ wire GUVU_SPRITE_DELTA5;
  /* p29.GYDA*/ wire GYDA_SPRITE_DELTA6;
  /* p29.GEWY*/ wire GEWY_SPRITE_DELTA7;

  /* p29.GYKY*/ wire GYKY_YDIFF_S3;
  /* p29.WUHU*/ wire WUHU_YDIFF_C7;
};

SpriteDeltaY sprite_delta_y(const OamTempA& oam_temp_a, const RegLY& reg_ly);

//------------------------------------------------------------------------------------------------------------------------

struct ScanCounter {
   /*#p28.FETO*/ wire FETO_SCAN_DONEp() const {
     return and4(YFEL_SCAN0_evn.qp_any(), WEWY_SCAN1_evn.qp_any(), GOSO_SCAN2_evn.qp_any(), FONY_SCAN5_evn.qp_any()); // 32 + 4 + 2 + 1 = 39
   }

   void tock(wire XUPY_ABxxEFxx, wire ANOM_LINE_RSTn);

  /*p28.YFEL*/ DFF17 YFEL_SCAN0_evn;        // AxxxExxx
  /*p28.WEWY*/ DFF17 WEWY_SCAN1_evn;        // Axxxxxxx
  /*p28.GOSO*/ DFF17 GOSO_SCAN2_evn;        // Axxxxxxx
  /*p28.ELYN*/ DFF17 ELYN_SCAN3_evn;        // Axxxxxxx
  /*p28.FAHA*/ DFF17 FAHA_SCAN4_evn;        // Axxxxxxx
  /*p28.FONY*/ DFF17 FONY_SCAN5_evn;        // Axxxxxxx
};

//------------------------------------------------------------------------------------------------------------------------

struct SpriteScanner {

  wire AVAP_SCAN_DONE_TRIGp(wire BALU_LINE_RSTp) const {
    /*#p29.BEBU*/ wire _BEBU_SCAN_DONE_TRIGn = or3(DOBA_SCAN_DONE_Bp_xxx.qp_any(), BALU_LINE_RSTp, BYBA_SCAN_DONE_Ap_evn.qn_any());
    /*#p29.AVAP*/ wire _AVAP_SCAN_DONE_TRIGp = not1(_BEBU_SCAN_DONE_TRIGn);
    return _AVAP_SCAN_DONE_TRIGp;
  }

  /* p29.CEHA*/ wire CEHA_SCANNINGp() const { return not1(CENO_SCANNINGp_evn.qn_new()); }

  /*p28.BESU*/ NorLatch BESU_SCANNINGp_evn; // Axxxxxxx
  /*p29.CENO*/ DFF17 CENO_SCANNINGp_evn;    // xxxxExxx
  /*p29.BYBA*/ DFF17 BYBA_SCAN_DONE_Ap_evn; // Axxxxxxx
  /*p29.DOBA*/ DFF17 DOBA_SCAN_DONE_Bp_xxx; // ABxxxxxx Cleared on A, set on B
};

//------------------------------------------------------------------------------------------------------------------------
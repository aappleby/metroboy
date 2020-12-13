#pragma once
#include "GateBoyLib/Gates.h"

//-----------------------------------------------------------------------------

struct SpriteScanner {

  wire AVAP_SCAN_DONE_TRIGp(wire BALU_LINE_RSTp) const {
    /*#p29.BEBU*/ wire _BEBU_SCAN_DONE_TRIGn = or3(DOBA_SCAN_DONE_Bp_xxx.qp_any(), BALU_LINE_RSTp, BYBA_SCAN_DONE_Ap_evn.qn_any());
    /*#p29.AVAP*/ wire _AVAP_SCAN_DONE_TRIGp = not1(_BEBU_SCAN_DONE_TRIGn);
    return _AVAP_SCAN_DONE_TRIGp;
  }

  /*p28.BESU*/ NorLatch BESU_SCANNINGp_evn; // Axxxxxxx
  /*p29.CENO*/ DFF17 CENO_SCANNINGp_evn;    // xxxxExxx
  /*p29.BYBA*/ DFF17 BYBA_SCAN_DONE_Ap_evn; // Axxxxxxx
  /*p29.DOBA*/ DFF17 DOBA_SCAN_DONE_Bp_xxx; // ABxxxxxx Cleared on A, set on B

  /*p28.YFEL*/ DFF17 YFEL_SCAN0_evn;        // AxxxExxx
  /*p28.WEWY*/ DFF17 WEWY_SCAN1_evn;        // Axxxxxxx
  /*p28.GOSO*/ DFF17 GOSO_SCAN2_evn;        // Axxxxxxx
  /*p28.ELYN*/ DFF17 ELYN_SCAN3_evn;        // Axxxxxxx
  /*p28.FAHA*/ DFF17 FAHA_SCAN4_evn;        // Axxxxxxx
  /*p28.FONY*/ DFF17 FONY_SCAN5_evn;        // Axxxxxxx
};

//-----------------------------------------------------------------------------

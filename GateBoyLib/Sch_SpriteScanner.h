#pragma once
#include "GateBoyLib/Gates.h"

//-----------------------------------------------------------------------------

struct ScanCounter {
   /*#p28.FETO*/ wire FETO_SCAN_DONEp() const {
     return and4(YFEL_SCAN0_evn.qp_any(), WEWY_SCAN1_evn.qp_any(), GOSO_SCAN2_evn.qp_any(), FONY_SCAN5_evn.qp_any()); // 32 + 4 + 2 + 1 = 39
   }

   void tock(wire XUPY_ABxxEFxx, wire ANOM_LINE_RSTn) {
    /* p28.YFEL*/ YFEL_SCAN0_evn.RSTn(ANOM_LINE_RSTn);
    /* p28.WEWY*/ WEWY_SCAN1_evn.RSTn(ANOM_LINE_RSTn);
    /* p28.GOSO*/ GOSO_SCAN2_evn.RSTn(ANOM_LINE_RSTn);
    /* p28.ELYN*/ ELYN_SCAN3_evn.RSTn(ANOM_LINE_RSTn);
    /* p28.FAHA*/ FAHA_SCAN4_evn.RSTn(ANOM_LINE_RSTn);
    /* p28.FONY*/ FONY_SCAN5_evn.RSTn(ANOM_LINE_RSTn);

    /* p28.GAVA*/ wire GAVA_SCAN_CLOCKp = or2(FETO_SCAN_DONEp(), XUPY_ABxxEFxx);
    /* p28.YFEL*/ YFEL_SCAN0_evn.dff17(GAVA_SCAN_CLOCKp,        ANOM_LINE_RSTn, YFEL_SCAN0_evn.qn_any());
    /* p28.WEWY*/ WEWY_SCAN1_evn.dff17(YFEL_SCAN0_evn.qn_any(), ANOM_LINE_RSTn, WEWY_SCAN1_evn.qn_any());
    /* p28.GOSO*/ GOSO_SCAN2_evn.dff17(WEWY_SCAN1_evn.qn_any(), ANOM_LINE_RSTn, GOSO_SCAN2_evn.qn_any());
    /* p28.ELYN*/ ELYN_SCAN3_evn.dff17(GOSO_SCAN2_evn.qn_any(), ANOM_LINE_RSTn, ELYN_SCAN3_evn.qn_any());
    /* p28.FAHA*/ FAHA_SCAN4_evn.dff17(ELYN_SCAN3_evn.qn_any(), ANOM_LINE_RSTn, FAHA_SCAN4_evn.qn_any());
    /* p28.FONY*/ FONY_SCAN5_evn.dff17(FAHA_SCAN4_evn.qn_any(), ANOM_LINE_RSTn, FONY_SCAN5_evn.qn_any());
   }

  /*p28.YFEL*/ DFF17 YFEL_SCAN0_evn;        // AxxxExxx
  /*p28.WEWY*/ DFF17 WEWY_SCAN1_evn;        // Axxxxxxx
  /*p28.GOSO*/ DFF17 GOSO_SCAN2_evn;        // Axxxxxxx
  /*p28.ELYN*/ DFF17 ELYN_SCAN3_evn;        // Axxxxxxx
  /*p28.FAHA*/ DFF17 FAHA_SCAN4_evn;        // Axxxxxxx
  /*p28.FONY*/ DFF17 FONY_SCAN5_evn;        // Axxxxxxx
};

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

//-----------------------------------------------------------------------------

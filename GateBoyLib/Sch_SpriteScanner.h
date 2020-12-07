#pragma once
#include "GateBoyLib/Gates.h"

//-----------------------------------------------------------------------------

struct SpriteScanner {
  void reset_cart() {
    BESU_SCANNINGp_evn.reset(0);
    CENO_SCANNINGp_evn.reset(REG_D0C1);

    YFEL_SCAN0_evn.reset(REG_D1C1);
    WEWY_SCAN1_evn.reset(REG_D1C0);
    GOSO_SCAN2_evn.reset(REG_D1C0);
    ELYN_SCAN3_evn.reset(REG_D0C0);
    FAHA_SCAN4_evn.reset(REG_D0C1);
    FONY_SCAN5_evn.reset(REG_D1C1);

    BYBA_SCAN_DONE_Ap_evn.reset(REG_D1C1);
    DOBA_SCAN_DONE_Bp_xxx.reset(REG_D1C0);
  }

  void reset_boot() {
    BESU_SCANNINGp_evn.reset(0);
    CENO_SCANNINGp_evn.reset(REG_D0C0);

    YFEL_SCAN0_evn.reset(REG_D0C0);
    WEWY_SCAN1_evn.reset(REG_D0C0);
    GOSO_SCAN2_evn.reset(REG_D0C0);
    ELYN_SCAN3_evn.reset(REG_D0C0);
    FAHA_SCAN4_evn.reset(REG_D0C0);
    FONY_SCAN5_evn.reset(REG_D0C0);

    BYBA_SCAN_DONE_Ap_evn.reset(REG_D0C0);
    DOBA_SCAN_DONE_Bp_xxx.reset(REG_D0C0);
  }

  //----------------------------------------
  // States

  /*p28.BESU*/ NorLatch BESU_SCANNINGp_evn; // Axxxxxxx
  /*p29.CENO*/ DFF17 CENO_SCANNINGp_evn;    // xxxxExxx
  /*p29.BYBA*/ DFF17 BYBA_SCAN_DONE_Ap_evn; // Axxxxxxx
  /*p29.DOBA*/ DFF17 DOBA_SCAN_DONE_Bp_xxx; // ABxxxxxx FIXME ab?

  /*p28.YFEL*/ DFF17 YFEL_SCAN0_evn;        // AxxxExxx
  /*p28.WEWY*/ DFF17 WEWY_SCAN1_evn;        // Axxxxxxx
  /*p28.GOSO*/ DFF17 GOSO_SCAN2_evn;        // Axxxxxxx
  /*p28.ELYN*/ DFF17 ELYN_SCAN3_evn;        // Axxxxxxx
  /*p28.FAHA*/ DFF17 FAHA_SCAN4_evn;        // Axxxxxxx
  /*p28.FONY*/ DFF17 FONY_SCAN5_evn;        // Axxxxxxx
};

//-----------------------------------------------------------------------------

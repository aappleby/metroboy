#pragma once
#include "GateBoyLib/Gates.h"

//------------------------------------------------------------------------------------------------------------------------

struct SpriteScanner {
  void reset_to_cart() {
    BESU_SCANNINGp.reset(0);
    CENO_SCANNINGp.reset(1, 0);
    BYBA_SCAN_DONE_Ap.reset(1, 1);
    DOBA_SCAN_DONE_Bp.reset(0, 1);
    YFEL_SCAN0.reset(1, 1);
    WEWY_SCAN1.reset(0, 1);
    GOSO_SCAN2.reset(0, 1);
    ELYN_SCAN3.reset(0, 0);
    FAHA_SCAN4.reset(1, 0);
    FONY_SCAN5.reset(1, 1);
  }

  /*#p28.FETO*/ wire2 FETO_SCAN_DONEp_old() const {
    return and4(YFEL_SCAN0.qp_old(), WEWY_SCAN1.qp_old(), GOSO_SCAN2.qp_old(), FONY_SCAN5.qp_old()); // 32 + 4 + 2 + 1 = 39
  }

  /*#p28.FETO*/ wire2 FETO_SCAN_DONEp_new() const {
    return and4(YFEL_SCAN0.qp_new(), WEWY_SCAN1.qp_new(), GOSO_SCAN2.qp_new(), FONY_SCAN5.qp_new()); // 32 + 4 + 2 + 1 = 39
  }

  /*#p28.FETO*/ wire2 FETO_SCAN_DONEp_any() const {
    return and4(YFEL_SCAN0.qp_any(), WEWY_SCAN1.qp_any(), GOSO_SCAN2.qp_any(), FONY_SCAN5.qp_any()); // 32 + 4 + 2 + 1 = 39
  }

  wire2 AVAP_SCAN_DONE_TRIGp_old(wire2 BALU_LINE_RSTp) const {
    /*#p29.BEBU*/ wire2 _BEBU_SCAN_DONE_TRIGn_old = or3(DOBA_SCAN_DONE_Bp.qp_old(), BALU_LINE_RSTp, BYBA_SCAN_DONE_Ap.qn_old());
    /*#p29.AVAP*/ wire2 _AVAP_SCAN_DONE_TRIGp_old = not1(_BEBU_SCAN_DONE_TRIGn_old);
    return _AVAP_SCAN_DONE_TRIGp_old;
  }

  wire2 AVAP_SCAN_DONE_TRIGp_new(wire2 BALU_LINE_RSTp) const {
    /*#p29.BEBU*/ wire2 _BEBU_SCAN_DONE_TRIGn_new = or3(DOBA_SCAN_DONE_Bp.qp_new(), BALU_LINE_RSTp, BYBA_SCAN_DONE_Ap.qn_new());
    /*#p29.AVAP*/ wire2 _AVAP_SCAN_DONE_TRIGp_new = not1(_BEBU_SCAN_DONE_TRIGn_new);
    return _AVAP_SCAN_DONE_TRIGp_new;
  }

  /* p29.CEHA*/ wire2 CEHA_SCANNINGp() const { return not1(CENO_SCANNINGp.qn_new()); }
  /*#p29.BYJO*/ wire2 BYJO_SCANNINGn() const { return not1(CEHA_SCANNINGp()); }

  void tock(wire2 XUPY_ABxxEFxx, wire2 ANOM_LINE_RSTn);

  void dump(Dumper& d) {
    d.dump_slice2p("SCAN INDEX     : ", &YFEL_SCAN0, 6);
    d.dump_bitp   ("BESU_SCANNINGp : ", BESU_SCANNINGp.state);
    d.dump_bitp   ("CENO_SCANNINGp : ", CENO_SCANNINGp.state);
    d.dump_bitp   ("BYBA_SCAN_DONE_Ap : ", BYBA_SCAN_DONE_Ap.state);
    d.dump_bitp   ("DOBA_SCAN_DONE_Bp : ", DOBA_SCAN_DONE_Bp.state);
  }

  /*p28.BESU*/ NorLatch BESU_SCANNINGp; // Axxxxxxx
  /*p29.CENO*/ DFF17 CENO_SCANNINGp;    // xxxxExxx
  /*p29.BYBA*/ DFF17 BYBA_SCAN_DONE_Ap; // Axxxxxxx
  /*p29.DOBA*/ DFF17 DOBA_SCAN_DONE_Bp; // ABxxxxxx Cleared on A, set on B

  /*p28.YFEL*/ DFF17 YFEL_SCAN0;        // AxxxExxx
  /*p28.WEWY*/ DFF17 WEWY_SCAN1;        // Axxxxxxx
  /*p28.GOSO*/ DFF17 GOSO_SCAN2;        // Axxxxxxx
  /*p28.ELYN*/ DFF17 ELYN_SCAN3;        // Axxxxxxx
  /*p28.FAHA*/ DFF17 FAHA_SCAN4;        // Axxxxxxx
  /*p28.FONY*/ DFF17 FONY_SCAN5;        // Axxxxxxx
};

//------------------------------------------------------------------------------------------------------------------------

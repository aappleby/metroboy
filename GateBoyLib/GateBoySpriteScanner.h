#pragma once
#include "GateBoyLib/Gates.h"

//------------------------------------------------------------------------------------------------------------------------

struct SpriteScanner {
  void reset_to_cart() {
    BESU_SCANNINGp.reset(REG_D0C0);
    CENO_SCANNINGp.reset(REG_D0C1);
    BYBA_SCAN_DONE_Ap.reset(REG_D1C1);
    DOBA_SCAN_DONE_Bp.reset(REG_D1C0);
    YFEL_SCAN0.reset(REG_D1C1);
    WEWY_SCAN1.reset(REG_D1C0);
    GOSO_SCAN2.reset(REG_D1C0);
    ELYN_SCAN3.reset(REG_D0C0);
    FAHA_SCAN4.reset(REG_D0C1);
    FONY_SCAN5.reset(REG_D1C1);
  }

  /*#p28.FETO*/ wire FETO_SCAN_DONEp() const {
    return and4(YFEL_SCAN0.qp_new(), WEWY_SCAN1.qp_new(), GOSO_SCAN2.qp_new(), FONY_SCAN5.qp_new()); // 32 + 4 + 2 + 1 = 39
  }

  wire AVAP_SCAN_DONE_TRIGp(wire BALU_LINE_RSTp) const {
    /*#p29.BEBU*/ wire _BEBU_SCAN_DONE_TRIGn = or3(DOBA_SCAN_DONE_Bp.qp_new(), BALU_LINE_RSTp, BYBA_SCAN_DONE_Ap.qn_new());
    /*#p29.AVAP*/ wire _AVAP_SCAN_DONE_TRIGp = not1(_BEBU_SCAN_DONE_TRIGn);
    return _AVAP_SCAN_DONE_TRIGp;
  }

  /* p29.CEHA*/ wire CEHA_SCANNINGp() const { return not1(CENO_SCANNINGp.qn_new()); }
  /*#p29.BYJO*/ wire BYJO_SCANNINGn() const { return not1(CEHA_SCANNINGp()); }

  void tock(wire XUPY_ABxxEFxx, wire ANOM_LINE_RSTn);

  void dump(Dumper& d) {
    d          ("SCAN INDEX        : %02d\n", pack_u8p(6, &YFEL_SCAN0));
    d.dump_bitp("BESU_SCANNINGp   ", BESU_SCANNINGp.state);
    d.dump_bitp("CENO_SCANNINGp   ", CENO_SCANNINGp.state);
    d.dump_bitp("BYBA_SCAN_DONE_Ap", BYBA_SCAN_DONE_Ap.state);
    d.dump_bitp("DOBA_SCAN_DONE_Bp", DOBA_SCAN_DONE_Bp.state);
    d.dump_bitp("YFEL_SCAN0       ", YFEL_SCAN0.state);
    d.dump_bitp("WEWY_SCAN1       ", WEWY_SCAN1.state);
    d.dump_bitp("GOSO_SCAN2       ", GOSO_SCAN2.state);
    d.dump_bitp("ELYN_SCAN3       ", ELYN_SCAN3.state);
    d.dump_bitp("FAHA_SCAN4       ", FAHA_SCAN4.state);
    d.dump_bitp("FONY_SCAN5       ", FONY_SCAN5.state);
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

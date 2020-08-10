#pragma once
#include "Cells.h"

namespace Schematics {

struct SchematicTop;
struct OamBus;

//-----------------------------------------------------------------------------

struct SpriteScanner {

  void tick(const SchematicTop& top);
  void tock(const SchematicTop& top);
  void dump(Dumper& d, const SchematicTop& top) const;

  /*p29.DYTY*/ wire DYTY_STORE_ENn_xxCDxxGH() const { return not1(_CARE_STORE_ENp_ABxxEFxx); }

  /*p29.AVAP*/ wire AVAP_RENDER_START_TRIGp() const {
    // BEBU := or3(DOBA_Q, BALU, BYBA_QN);
    // AVAP := not(BEBU);
    /*p29.BEBU*/ wire BEBU_SCAN_DONE_TRIGn = or3(DOBA_SCAN_DONE_B.qp(), BALU_LINE_RSTp, BYBA_SCAN_DONE_A.qn());
    /*p29.AVAP*/ wire AVAP_RENDER_START_TRIGp = not1(BEBU_SCAN_DONE_TRIGn);
    return AVAP_RENDER_START_TRIGp;
  }

  /*p29.BUZA*/ wire BUZA_STORE_SPRITE_INDX() const { return and2(CENO_SCANNINGp.qn(), _XYMU_RENDERINGp); }

  /*p28.YFEL*/ wire YFEL_SCAN0() const { return _YFEL_SCAN0.qp(); }
  /*p28.WEWY*/ wire WEWY_SCAN1() const { return _WEWY_SCAN1.qp(); }
  /*p28.GOSO*/ wire GOSO_SCAN2() const { return _GOSO_SCAN2.qp(); }
  /*p28.ELYN*/ wire ELYN_SCAN3() const { return _ELYN_SCAN3.qp(); }
  /*p28.FAHA*/ wire FAHA_SCAN4() const { return _FAHA_SCAN4.qp(); }
  /*p28.FONY*/ wire FONY_SCAN5() const { return _FONY_SCAN5.qp(); }

//private:

  /*p29.AZEM*/ wire AZEM_RENDERINGp() const { return and2(BYJO_SCANNINGn(), _XYMU_RENDERINGp); }
  /*p29.BYJO*/ wire BYJO_SCANNINGn() const { return not1(CEHA_SCANNINGp()); }
  /*p29.CEHA*/ wire CEHA_SCANNINGp() const { return not1(CENO_SCANNINGp.qn()); }

  //----------------------------------------
  // Signals

  Sig _XYMU_RENDERINGp;
  Sig BALU_LINE_RSTp;
  Sig BAGY_LINE_RSTn;

  /*p29.ERUC*/ Sig _ERUC_YDIFF_S0;
  /*p29.ERUC*/ Sig _ERUC_YDIFF_C0;
  /*p29.ENEF*/ Sig _ENEF_YDIFF_S1;
  /*p29.ENEF*/ Sig _ENEF_YDIFF_C1;
  /*p29.FECO*/ Sig _FECO_YDIFF_S2;
  /*p29.FECO*/ Sig _FECO_YDIFF_C2;
  /*p29.GYKY*/ Sig _GYKY_YDIFF_S3;
  /*p29.GYKY*/ Sig _GYKY_YDIFF_C3;
  /*p29.GOPU*/ Sig _GOPU_YDIFF_S4;
  /*p29.GOPU*/ Sig _GOPU_YDIFF_C4;
  /*p29.FUWA*/ Sig _FUWA_YDIFF_S5;
  /*p29.FUWA*/ Sig _FUWA_YDIFF_C5;
  /*p29.GOJU*/ Sig _GOJU_YDIFF_S6;
  /*p29.GOJU*/ Sig _GOJU_YDIFF_C6;
  /*p29.WUHU*/ Sig _WUHU_YDIFF_S7;
  /*p29.WUHU*/ Sig _WUHU_YDIFF_C7;

  /*p29.GACE*/ Sig _GACE_SPRITE_DELTA4;
  /*p29.GUVU*/ Sig _GUVU_SPRITE_DELTA5;
  /*p29.GYDA*/ Sig _GYDA_SPRITE_DELTA6;
  /*p29.GEWY*/ Sig _GEWY_SPRITE_DELTA7;

  /*p29.GOVU*/ Sig _GOVU_SPSIZE_MATCH;
  /*p29.WOTA*/ Sig _WOTA_SCAN_MATCH_Yn;
  /*p29.GESE*/ Sig _GESE_SCAN_MATCH_Y;

  /*p29.CARE*/ Sig _CARE_STORE_ENp_ABxxEFxx; // to sprite store

  //----------------------------------------
  // States

  /*p28.BESU*/ Tri BESU_SCANNINGp = TRI_D0NP;
  /*p29.CENO*/ RegQN  CENO_SCANNINGp = REG_D0C0;

  /*p28.YFEL*/ RegQPN _YFEL_SCAN0 = REG_D0C0;
  /*p28.WEWY*/ RegQPN _WEWY_SCAN1 = REG_D0C0;
  /*p28.GOSO*/ RegQPN _GOSO_SCAN2 = REG_D0C0;
  /*p28.ELYN*/ RegQPN _ELYN_SCAN3 = REG_D0C0;
  /*p28.FAHA*/ RegQPN _FAHA_SCAN4 = REG_D0C0;
  /*p28.FONY*/ RegQPN _FONY_SCAN5 = REG_D0C0;
  /*p29.BYBA*/ RegQPN BYBA_SCAN_DONE_A = REG_D0C0;
  /*p29.DOBA*/ RegQP  DOBA_SCAN_DONE_B = REG_D0C0;
};

//-----------------------------------------------------------------------------

}; // namespace Schematics
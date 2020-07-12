#pragma once
#include "Cells.h"

namespace Schematics {

struct SchematicTop;

//-----------------------------------------------------------------------------

struct SpriteScanner {

  void tick_ymatch(SchematicTop& gb);
  void tock(SchematicTop& gb);
  SignalHash commit();

  wire DEGE_SPRITE_DELTA0() const { return not(_ERUC_YDIFF_S0); }
  wire DABY_SPRITE_DELTA1() const { return not(_ENEF_YDIFF_S1); }
  wire DABU_SPRITE_DELTA2() const { return not(_FECO_YDIFF_S2); }
  wire GYSA_SPRITE_DELTA3() const { return not(_GYKY_YDIFF_S3); }

  /*p29.DYTY*/ wire DYTY_STORE_ENn_xxCDxxGH() const { return not(_CARE_STORE_ENp_ABxxEFxx); }

  /*p29.BYJO*/ wire BYJO_SCANNINGn() const { return not(CEHA_SCANNINGp()); }

  /*p29.CENO*/ wire CENO_SCANNINGp() const { return _CENO_SCANNINGp.q(); }
  /*p28.BESU*/ wire BESU_SCANNINGp() const { return _BESU_SCANNINGp.q(); }

  /*p29.DOBA*/ wire DOBA_SCAN_DONE_B() const { return _DOBA_SCAN_DONE_B.q(); }
  /*p29.BYBA*/ wire BYBA_SCAN_DONE_A() const { return _BYBA_SCAN_DONE_A.q(); }

  /*p28.GUSE*/ wire GUSE_SCAN0n() const { return not(_YFEL_SCAN0.q()); }
  /*p28.GEMA*/ wire GEMA_SCAN1n() const { return not(_WEWY_SCAN1.q()); }
  /*p28.FUTO*/ wire FUTO_SCAN2n() const { return not(_GOSO_SCAN2.q()); }
  /*p28.FAKU*/ wire FAKU_SCAN3n() const { return not(_ELYN_SCAN3.q()); }
  /*p28.GAMA*/ wire GAMA_SCAN4n() const { return not(_FAHA_SCAN4.q()); }
  /*p28.GOBY*/ wire GOBY_SCAN5n() const { return not(_FONY_SCAN5.q()); }

private:

  /*p29.CEHA*/ wire CEHA_SCANNINGp() const { return not(_CENO_SCANNINGp.qn()); }

  /*p29.ERUC*/ Signal _ERUC_YDIFF_S0;
  /*p29.ERUC*/ Signal _ERUC_YDIFF_C0;
  /*p29.ENEF*/ Signal _ENEF_YDIFF_S1;
  /*p29.ENEF*/ Signal _ENEF_YDIFF_C1;
  /*p29.FECO*/ Signal _FECO_YDIFF_S2;
  /*p29.FECO*/ Signal _FECO_YDIFF_C2;
  /*p29.GYKY*/ Signal _GYKY_YDIFF_S3;
  /*p29.GYKY*/ Signal _GYKY_YDIFF_C3;
  /*p29.GOPU*/ Signal _GOPU_YDIFF_S4;
  /*p29.GOPU*/ Signal _GOPU_YDIFF_C4;
  /*p29.FUWA*/ Signal _FUWA_YDIFF_S5;
  /*p29.FUWA*/ Signal _FUWA_YDIFF_C5;
  /*p29.GOJU*/ Signal _GOJU_YDIFF_S6;
  /*p29.GOJU*/ Signal _GOJU_YDIFF_C6;
  /*p29.WUHU*/ Signal _WUHU_YDIFF_S7;
  /*p29.WUHU*/ Signal _WUHU_YDIFF_C7;

  /*p29.CARE*/ Signal _CARE_STORE_ENp_ABxxEFxx; // to sprite store

  /*p28.BESU*/ NorLatch _BESU_SCANNINGp;
  /*p29.CENO*/ Reg17    _CENO_SCANNINGp;

  /*p28.YFEL*/ Reg17 _YFEL_SCAN0;
  /*p28.WEWY*/ Reg17 _WEWY_SCAN1;
  /*p28.GOSO*/ Reg17 _GOSO_SCAN2;
  /*p28.ELYN*/ Reg17 _ELYN_SCAN3;
  /*p28.FAHA*/ Reg17 _FAHA_SCAN4;
  /*p28.FONY*/ Reg17 _FONY_SCAN5;
  /*p29.BYBA*/ Reg17 _BYBA_SCAN_DONE_A;
  /*p29.DOBA*/ Reg17 _DOBA_SCAN_DONE_B;
};

//-----------------------------------------------------------------------------

}; // namespace Schematics
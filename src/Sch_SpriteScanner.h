#pragma once
#include "Cells.h"

namespace Schematics {

struct SchematicTop;

//-----------------------------------------------------------------------------

struct SpriteScanner {
  /*p29.ERUC*/ Signal ERUC_YDIFF_S0;
  /*p29.ERUC*/ Signal ERUC_YDIFF_C0;
  /*p29.ENEF*/ Signal ENEF_YDIFF_S1;
  /*p29.ENEF*/ Signal ENEF_YDIFF_C1;
  /*p29.FECO*/ Signal FECO_YDIFF_S2;
  /*p29.FECO*/ Signal FECO_YDIFF_C2;
  /*p29.GYKY*/ Signal GYKY_YDIFF_S3;
  /*p29.GYKY*/ Signal GYKY_YDIFF_C3;
  /*p29.GOPU*/ Signal GOPU_YDIFF_S4;
  /*p29.GOPU*/ Signal GOPU_YDIFF_C4;
  /*p29.FUWA*/ Signal FUWA_YDIFF_S5;
  /*p29.FUWA*/ Signal FUWA_YDIFF_C5;
  /*p29.GOJU*/ Signal GOJU_YDIFF_S6;
  /*p29.GOJU*/ Signal GOJU_YDIFF_C6;
  /*p29.WUHU*/ Signal WUHU_YDIFF_S7;
  /*p29.WUHU*/ Signal WUHU_YDIFF_C7;

  /*p29.CARE*/ Signal CARE_STORE_ENp_ABxxEFxx; // to sprite store

  void tick_ymatch(SchematicTop& gb);

  void tock(SchematicTop& gb);
  SignalHash commit();

private:
  friend struct SchematicTop;

  /*p28.BESU*/ NorLatch BESU_SCANNINGp;
  /*p29.CENO*/ Reg17    CENO_SCANNINGp;

  /*p28.YFEL*/ Reg17 YFEL_SCAN0;
  /*p28.WEWY*/ Reg17 WEWY_SCAN1;
  /*p28.GOSO*/ Reg17 GOSO_SCAN2;
  /*p28.ELYN*/ Reg17 ELYN_SCAN3;
  /*p28.FAHA*/ Reg17 FAHA_SCAN4;
  /*p28.FONY*/ Reg17 FONY_SCAN5;
  /*p29.BYBA*/ Reg17 BYBA_SCAN_DONE_A;
  /*p29.DOBA*/ Reg17 DOBA_SCAN_DONE_B;
};

//-----------------------------------------------------------------------------

}; // namespace Schematics
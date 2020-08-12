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

  /*p28.YFEL*/ wire YFEL_SCAN0() const { return _YFEL_SCAN0.qp(); }
  /*p28.WEWY*/ wire WEWY_SCAN1() const { return _WEWY_SCAN1.qp(); }
  /*p28.GOSO*/ wire GOSO_SCAN2() const { return _GOSO_SCAN2.qp(); }
  /*p28.ELYN*/ wire ELYN_SCAN3() const { return _ELYN_SCAN3.qp(); }
  /*p28.FAHA*/ wire FAHA_SCAN4() const { return _FAHA_SCAN4.qp(); }
  /*p28.FONY*/ wire FONY_SCAN5() const { return _FONY_SCAN5.qp(); }

//private:

//----------------------------------------
  // Signals

  /*p29.ERUC*/ Sig ERUC_YDIFF_S0;
  /*p29.ERUC*/ Sig ERUC_YDIFF_C0;
  /*p29.ENEF*/ Sig ENEF_YDIFF_S1;
  /*p29.ENEF*/ Sig ENEF_YDIFF_C1;
  /*p29.FECO*/ Sig FECO_YDIFF_S2;
  /*p29.FECO*/ Sig FECO_YDIFF_C2;
  /*p29.GYKY*/ Sig GYKY_YDIFF_S3;
  /*p29.GYKY*/ Sig GYKY_YDIFF_C3;
  /*p29.GOPU*/ Sig GOPU_YDIFF_S4;
  /*p29.GOPU*/ Sig GOPU_YDIFF_C4;
  /*p29.FUWA*/ Sig FUWA_YDIFF_S5;
  /*p29.FUWA*/ Sig FUWA_YDIFF_C5;
  /*p29.GOJU*/ Sig GOJU_YDIFF_S6;
  /*p29.GOJU*/ Sig GOJU_YDIFF_C6;
  /*p29.WUHU*/ Sig WUHU_YDIFF_S7;
  /*p29.WUHU*/ Sig WUHU_YDIFF_C7;

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
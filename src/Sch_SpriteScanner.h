#pragma once
#include "Sch_Common.h"

namespace Schematics {

struct SchematicTop;

//-----------------------------------------------------------------------------

struct SpriteScanner {
  void tick(SchematicTop& gb);
  SignalHash commit();

private:
  friend struct SchematicTop;

  /*p28.BESU*/ NorLatch BESU_SCANNINGp;
  /*p29.CENO*/ Reg      CENO_SCANNINGp;

  /*p29.DEGE*/ Signal DEGE_SPRITE_DELTA0;      // to sprite store
  /*p29.DABY*/ Signal DABY_SPRITE_DELTA1;      // to sprite store
  /*p29.DABU*/ Signal DABU_SPRITE_DELTA2;      // to sprite store
  /*p29.GYSA*/ Signal GYSA_SPRITE_DELTA3;      // to sprite store
  /*p29.CARE*/ Signal CARE_STORE_ENp_ABxxEFxx; // to sprite store

  /*p28.YFEL*/ Reg YFEL_SCAN0;
  /*p28.WEWY*/ Reg WEWY_SCAN1;
  /*p28.GOSO*/ Reg GOSO_SCAN2;
  /*p28.ELYN*/ Reg ELYN_SCAN3;
  /*p28.FAHA*/ Reg FAHA_SCAN4;
  /*p28.FONY*/ Reg FONY_SCAN5;
  /*p29.BYBA*/ Reg SCAN_DONE_TRIG_A;
  /*p29.DOBA*/ Reg SCAN_DONE_TRIG_B;
};

//-----------------------------------------------------------------------------

}; // namespace Schematics
#pragma once
#include "GateBoyLib/Gates.h"

namespace Schematics {

struct SchematicTop;
struct OamBus;

//-----------------------------------------------------------------------------

struct SpriteScanner {

  void tick(const SchematicTop& top);
  void tock(const SchematicTop& top);
  void dump(Dumper& d, const SchematicTop& top) const;

//private:

  //----------------------------------------
  // Signals

  /*p29.DEGE*/ Sig DEGE_SPRITE_DELTA0; // -> sprite store
  /*p29.DABY*/ Sig DABY_SPRITE_DELTA1; // -> sprite store
  /*p29.DABU*/ Sig DABU_SPRITE_DELTA2; // -> sprite store
  /*p29.GYSA*/ Sig GYSA_SPRITE_DELTA3; // -> sprite store
  /*p29.GACE*/ Sig GACE_SPRITE_DELTA4;
  /*p29.GUVU*/ Sig GUVU_SPRITE_DELTA5;
  /*p29.GYDA*/ Sig GYDA_SPRITE_DELTA6;
  /*p29.GEWY*/ Sig GEWY_SPRITE_DELTA7;

  /*p29.CARE*/ Sig CARE_STORE_ENp_ABxxEFxx; // to sprite store

  //----------------------------------------
  // States

  /*p28.BESU*/ Latch  BESU_SCANNINGp = TRI_D0NP; // -> oam bus
  /*p29.CENO*/ Reg  CENO_SCANNINGp = REG_D0C0; // -> oam bus

  /*p28.YFEL*/ Reg YFEL_SCAN0 = REG_D0C0; // -> oam bus address mux
  /*p28.WEWY*/ Reg WEWY_SCAN1 = REG_D0C0;
  /*p28.GOSO*/ Reg GOSO_SCAN2 = REG_D0C0;
  /*p28.ELYN*/ Reg ELYN_SCAN3 = REG_D0C0;
  /*p28.FAHA*/ Reg FAHA_SCAN4 = REG_D0C0;
  /*p28.FONY*/ Reg FONY_SCAN5 = REG_D0C0;

  /*p29.BYBA*/ Reg BYBA_SCAN_DONE_A = REG_D0C0; // -> top.AVAP_RENDER_START_TRIGp
  /*p29.DOBA*/ Reg  DOBA_SCAN_DONE_B = REG_D0C0; // -> top.AVAP_RENDER_START_TRIGp
};

//-----------------------------------------------------------------------------

}; // namespace Schematics
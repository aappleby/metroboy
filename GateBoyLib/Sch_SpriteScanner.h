#pragma once
#include "GateBoyLib/Gates.h"

namespace Schematics {

struct SchematicTop;
struct OamBus;

//-----------------------------------------------------------------------------

struct SpriteScanner {
  void reset() {}

  void reset_bootrom() {
    DEGE_SPRITE_DELTA0.reset(TRI_HZNP);
    DABY_SPRITE_DELTA1.reset(TRI_HZNP);
    DABU_SPRITE_DELTA2.reset(TRI_HZNP);
    GYSA_SPRITE_DELTA3.reset(TRI_HZNP);
    GACE_SPRITE_DELTA4.reset(TRI_HZNP);
    GUVU_SPRITE_DELTA5.reset(TRI_HZNP);
    GYDA_SPRITE_DELTA6.reset(TRI_HZNP);
    GEWY_SPRITE_DELTA7.reset(TRI_HZNP);
    CARE_STORE_ENp_ABxxEFxx.reset(TRI_HZNP);

    BESU_SCANNINGp.reset(TRI_D0NP);
    CENO_SCANNINGp.reset(REG_D0C0);

    YFEL_SCAN0.reset(REG_D0C0);
    WEWY_SCAN1.reset(REG_D0C0);
    GOSO_SCAN2.reset(REG_D0C0);
    ELYN_SCAN3.reset(REG_D0C0);
    FAHA_SCAN4.reset(REG_D0C0);
    FONY_SCAN5.reset(REG_D0C0);

    BYBA_SCAN_DONE_A.reset(REG_D0C0);
    DOBA_SCAN_DONE_B.reset(REG_D0C0);
  }

  void tick(const SchematicTop& top);
  void tock(const SchematicTop& top);
  void dump(Dumper& d, const SchematicTop& top) const;

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

  /*p28.BESU*/ NorLatch BESU_SCANNINGp; // -> oam bus
  /*p29.CENO*/ DFF17 CENO_SCANNINGp;    // -> oam bus

  /*p28.YFEL*/ DFF17 YFEL_SCAN0; // -> oam bus address mux
  /*p28.WEWY*/ DFF17 WEWY_SCAN1;
  /*p28.GOSO*/ DFF17 GOSO_SCAN2;
  /*p28.ELYN*/ DFF17 ELYN_SCAN3;
  /*p28.FAHA*/ DFF17 FAHA_SCAN4;
  /*p28.FONY*/ DFF17 FONY_SCAN5;

  /*p29.BYBA*/ DFF17 BYBA_SCAN_DONE_A; // -> top.AVAP_RENDER_START_TRIGp
  /*p29.DOBA*/ DFF17 DOBA_SCAN_DONE_B; // -> top.AVAP_RENDER_START_TRIGp
};

//-----------------------------------------------------------------------------

}; // namespace Schematics
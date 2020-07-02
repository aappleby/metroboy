#pragma once
#include "Sch_Common.h"

namespace Schematics {

struct SchematicTop;

//-----------------------------------------------------------------------------

struct SpriteScannerSignals {
  /*p28.BESU*/ Signal BESU_SCANNINGp;          // main "oam scanning" signal, to PPU
  /*p29.CENO*/ Signal CENO_SCANNINGp;          // to sprite store, controls driving sprite index onto sprite store ts bus
  /*p29.CEHA*/ Signal CEHA_SCANNINGp;          // to sprite store, enables matcher

  /*p29.AVAP*/ Signal AVAP_SCAN_DONE_TRIGp;    // to ppu, resets tile fetcher, sets XYMU_RENDERING, sets lcd hsync
                                               
                                               
  /*p29.DEGE*/ Signal DEGE_SPRITE_DELTA0;           // to sprite store
  /*p29.DABY*/ Signal DABY_SPRITE_DELTA1;           // to sprite store
  /*p29.DABU*/ Signal DABU_SPRITE_DELTA2;           // to sprite store
  /*p29.GYSA*/ Signal GYSA_SPRITE_DELTA3;           // to sprite store

  /*p29.CARE*/ Signal CARE_STORE_ENp_ABxxEFxx; // to sprite store

  /*p28.GUSE*/ Signal GUSE_SCAN0n;             // to bus mux
  /*p28.GEMA*/ Signal GEMA_SCAN1n;             // to bus mux
  /*p28.FUTO*/ Signal FUTO_SCAN2n;             // to bus mux
  /*p28.FAKU*/ Signal FAKU_SCAN3n;             // to bus mux
  /*p28.GAMA*/ Signal GAMA_SCAN4n;             // to bus mux
  /*p28.GOBY*/ Signal GOBY_SCAN5n;             // to bus mux

};

//-----------------------------------------------------------------------------

struct SpriteScanner {
  SpriteScannerSignals sig(const SchematicTop& gb) const;
  void tick(SchematicTop& gb);
  SignalHash commit();

private:

  /*p28.BESU*/ NorLatch BESU_SCANNINGp;
  /*p29.CENO*/ Reg      CENO_SCANNINGp;

  /*p28.YFEL*/ Reg SCAN0;
  /*p28.WEWY*/ Reg SCAN1;
  /*p28.GOSO*/ Reg SCAN2;
  /*p28.ELYN*/ Reg SCAN3;
  /*p28.FAHA*/ Reg SCAN4;
  /*p28.FONY*/ Reg SCAN5;
  /*p29.BYBA*/ Reg SCAN_DONE_TRIG_A;
  /*p29.DOBA*/ Reg SCAN_DONE_TRIG_B;


};

//-----------------------------------------------------------------------------

}; // namespace Schematics
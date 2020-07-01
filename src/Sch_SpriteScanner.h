#pragma once
#include "Sch_Common.h"

namespace Schematics {

struct TestGB;

//-----------------------------------------------------------------------------

struct SpriteScannerSignals {
  /*p28.BESU*/ Signal BESU_SCANNINGp; 
  /*p29.AVAP*/ Signal AVAP_SCAN_DONE_TRIGp;

  /*p29.CENO*/ Signal CENO_SCANNINGp;
  /*p29.CEHA*/ Signal CEHA_SCANNINGp; 
  /*p29.BYJO*/ Signal BYJO_SCANNINGn;

  /*p29.DEGE*/ Signal SPRITE_DELTA0;
  /*p29.DABY*/ Signal SPRITE_DELTA1;
  /*p29.DABU*/ Signal SPRITE_DELTA2;
  /*p29.GYSA*/ Signal SPRITE_DELTA3;

  /*p29.DYTY*/ Signal DYTY_STORE_ENn_xxCDxxGH;

  /*p28.GUSE*/ Signal GUSE_SCAN0n;
  /*p28.GEMA*/ Signal GEMA_SCAN1n;
  /*p28.FUTO*/ Signal FUTO_SCAN2n;
  /*p28.FAKU*/ Signal FAKU_SCAN3n;
  /*p28.GAMA*/ Signal GAMA_SCAN4n;
  /*p28.GOBY*/ Signal GOBY_SCAN5n;

};

//-----------------------------------------------------------------------------

struct SpriteScanner {
  SpriteScannerSignals sig(const TestGB& gb) const;
  void tick(TestGB& gb);
  bool commit();

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
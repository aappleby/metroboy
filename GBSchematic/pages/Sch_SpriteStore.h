#pragma once
#include "Sch_Common.h"

namespace Schematics {

//-----------------------------------------------------------------------------

struct SpriteIndexLine {
  bool TS_IDX_0;
  bool TS_IDX_1;
  bool TS_IDX_2;
  bool TS_IDX_3;
  bool TS_IDX_4;
  bool TS_IDX_5;

  bool TS_LINE_0;
  bool TS_LINE_1;
  bool TS_LINE_2;
  bool TS_LINE_3;
};

struct SpriteMatcher {

  bool MATCHn;

  Reg SPRITE_GET_SYNCn;

  Reg STORE_X0;
  Reg STORE_X1;
  Reg STORE_X2;
  Reg STORE_X3;
  Reg STORE_X4;
  Reg STORE_X5;
  Reg STORE_X6;
  Reg STORE_X7;

  Reg STORE_IDX0;
  Reg STORE_IDX1;
  Reg STORE_IDX2;
  Reg STORE_IDX3;
  Reg STORE_IDX4;
  Reg STORE_IDX5;

  Reg STORE_LINE0;
  Reg STORE_LINE1;
  Reg STORE_LINE2;
  Reg STORE_LINE3;
};

struct SpriteStore {

  /*p29.FEPO*/ bool STORE_MATCH;

  /*p29.DEZY*/ Reg STORE_EN_SYNC;

  /*p29.BESE*/ Reg SPRITE_COUNT0;
  /*p29.CUXY*/ Reg SPRITE_COUNT1;
  /*p29.BEGO*/ Reg SPRITE_COUNT2;
  /*p29.DYBE*/ Reg SPRITE_COUNT3;

  SpriteMatcher store0;
  SpriteMatcher store1;
  SpriteMatcher store2;
  SpriteMatcher store3;
  SpriteMatcher store4;
  SpriteMatcher store5;
  SpriteMatcher store6;
  SpriteMatcher store7;
  SpriteMatcher store8;
  SpriteMatcher store9;

  SpriteIndexLine sil;
};

//-----------------------------------------------------------------------------

};
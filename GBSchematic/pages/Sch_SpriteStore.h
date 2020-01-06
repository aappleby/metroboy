#pragma once
#include "Sch_Common.h"

namespace Schematics {

struct OamSignals;

//-----------------------------------------------------------------------------

struct SpriteTristate {
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

//-----------------------------------------------------------------------------

struct SpriteMatcherSignals {
  bool MATCHn;
};

struct SpriteMatcherRegisters {

  bool commit();

  SpriteMatcherSignals tick(const LcdSignals& lcd_sig,
                            const SpriteSignals& spr_sig,
                            const VidSignals& vid_sig,
                            const OamSignals& oam_sig,
                            bool MATCH_CHAINn,
                            bool STORE_SEL,
                            SpriteTristate& spr_tri);

  Reg2 SPRITE_GET_SYNCn;

  Reg2 STORE_X0;
  Reg2 STORE_X1;
  Reg2 STORE_X2;
  Reg2 STORE_X3;
  Reg2 STORE_X4;
  Reg2 STORE_X5;
  Reg2 STORE_X6;
  Reg2 STORE_X7;

  Reg2 STORE_IDX0;
  Reg2 STORE_IDX1;
  Reg2 STORE_IDX2;
  Reg2 STORE_IDX3;
  Reg2 STORE_IDX4;
  Reg2 STORE_IDX5;

  Reg2 STORE_LINE0;
  Reg2 STORE_LINE1;
  Reg2 STORE_LINE2;
  Reg2 STORE_LINE3;
};

//-----------------------------------------------------------------------------

struct SpriteStoreSignals {
  /*p29.FEPO*/ bool STORE_MATCH;
};

struct SpriteStoreRegisters {

  bool pass_end();

  SpriteStoreSignals tick(const ClkSignals& clk_sig,
                          const VrstSignals& vid_rst,
                          const LcdSignals& lcd_sig,
                          const SpriteSignals& spr_sig,
                          const VidSignals& vid_sig,
                          const OamSignals& oam_sig);

  SpriteMatcherRegisters store0;
  SpriteMatcherRegisters store1;
  SpriteMatcherRegisters store2;
  SpriteMatcherRegisters store3;
  SpriteMatcherRegisters store4;
  SpriteMatcherRegisters store5;
  SpriteMatcherRegisters store6;
  SpriteMatcherRegisters store7;
  SpriteMatcherRegisters store8;
  SpriteMatcherRegisters store9;

  /*p29.DEZY*/ Reg2 STORE_EN_SYNC;
  /*p29.BESE*/ Reg2 SPRITE_COUNT0;
  /*p29.CUXY*/ Reg2 SPRITE_COUNT1;
  /*p29.BEGO*/ Reg2 SPRITE_COUNT2;
  /*p29.DYBE*/ Reg2 SPRITE_COUNT3;

  SpriteTristate spr_tri;
};

//-----------------------------------------------------------------------------

};
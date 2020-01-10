#pragma once
#include "Sch_Common.h"

namespace Schematics {

struct OamSignals;
struct OamRegisters;

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

#if 0
struct SpriteMatcherRegisters {

  bool commit();

  void tick_store(const LcdSignals& lcd_sig,
             const SpriteSignals& spr_sig,
             const OamRegisters& oam_reg,
             bool STORE_SEL,
             SpriteTristate& spr_tri);

  bool tick_fetch(const LcdSignals& lcd_sig,
             const SpriteSignals& spr_sig,
             const VidRegisters& vid_reg,
             bool MATCH_CHAINn,
             SpriteTristate& spr_tri);

  Reg2 FONO_STORE_CLEARn;

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

struct SpriteStoreRegisters {

  bool pass_end();

  void tick_store(const ClkSignals& clk_sig,
            const RstSignals& rst_sig,
            const LcdSignals& lcd_sig,
            const SpriteSignals& spr_sig,
            const OamRegisters& oam_reg);

  bool tick_fetch(const LcdSignals& lcd_sig,
                  const SpriteSignals& spr_sig,
                  const VidRegisters& vid_reg);

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
#endif


struct SpriteStoreRegisters {

  /*p29.DEZY*/ Reg2 STORE_EN_SYNC;
  /*p29.BESE*/ Reg2 SPRITE_COUNT0;
  /*p29.CUXY*/ Reg2 SPRITE_COUNT1;
  /*p29.BEGO*/ Reg2 SPRITE_COUNT2;
  /*p29.DYBE*/ Reg2 SPRITE_COUNT3;

  /*p29.EBOJ*/ Reg2 SPRITE0_GET_SYNCn;
  /*p30.YGUS*/ Reg2 STORE0_IDX0;
  /*p30.YSOK*/ Reg2 STORE0_IDX1;
  /*p30.YZEP*/ Reg2 STORE0_IDX2;
  /*p30.WYTE*/ Reg2 STORE0_IDX3;
  /*p30.ZONY*/ Reg2 STORE0_IDX4;
  /*p30.YWAK*/ Reg2 STORE0_IDX5;
  /*p30.FYHY*/ Reg2 STORE0_LINE0;
  /*p30.GYHO*/ Reg2 STORE0_LINE1;
  /*p30.BOZU*/ Reg2 STORE0_LINE2;
  /*p30.CUFO*/ Reg2 STORE0_LINE3;
  /*p31.XEPE*/ Reg2 STORE0_X0;
  /*p31.YLAH*/ Reg2 STORE0_X1;
  /*p31.ZOLA*/ Reg2 STORE0_X2;
  /*p31.ZULU*/ Reg2 STORE0_X3;
  /*p31.WELO*/ Reg2 STORE0_X4;
  /*p31.XUNY*/ Reg2 STORE0_X5;
  /*p31.WOTE*/ Reg2 STORE0_X6;
  /*p31.XAKO*/ Reg2 STORE0_X7;

  /*p29.CEDY*/ Reg2 SPRITE1_GET_SYNCn;
  /*p30.CAJY*/ Reg2 STORE1_IDX0;
  /*p30.CUZA*/ Reg2 STORE1_IDX1;
  /*p30.COMA*/ Reg2 STORE1_IDX2;
  /*p30.CUFA*/ Reg2 STORE1_IDX3;
  /*p30.CEBO*/ Reg2 STORE1_IDX4;
  /*p30.CADU*/ Reg2 STORE1_IDX5;
  /*p30.ABUG*/ Reg2 STORE1_LINE0;
  /*p30.AMES*/ Reg2 STORE1_LINE1;
  /*p30.ABOP*/ Reg2 STORE1_LINE2;
  /*p30.AROF*/ Reg2 STORE1_LINE3;
  /*p31.DANY*/ Reg2 STORE1_X0;
  /*p31.DUKO*/ Reg2 STORE1_X1;
  /*p31.DESU*/ Reg2 STORE1_X2;
  /*p31.DAZO*/ Reg2 STORE1_X3;
  /*p31.DAKE*/ Reg2 STORE1_X4;
  /*p31.CESO*/ Reg2 STORE1_X5;
  /*p31.DYFU*/ Reg2 STORE1_X6;
  /*p31.CUSY*/ Reg2 STORE1_X7;
};


//-----------------------------------------------------------------------------

};
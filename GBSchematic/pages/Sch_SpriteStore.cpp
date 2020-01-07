#include "Sch_SpriteStore.h"

#include "Sch_Video.h"
#include "Sch_Sprites.h"
#include "Sch_Clocks.h"
#include "Sch_Pins.h"
#include "Sch_Resets.h"
#include "Sch_LCD.h"
#include "Sch_OAM.h"

#pragma warning(disable:4100)

namespace Schematics {

//-----------------------------------------------------------------------------

bool SpriteMatcherRegisters::commit() {
  bool changed = false;
  changed |= commit_all(SPRITE_GET_SYNCn);
  changed |= commit_all(STORE_X0, STORE_X1, STORE_X2, STORE_X3, STORE_X4, STORE_X5, STORE_X6, STORE_X7);
  changed |= commit_all(STORE_IDX0, STORE_IDX1, STORE_IDX2, STORE_IDX3, STORE_IDX4, STORE_IDX5);
  changed |= commit_all(STORE_LINE0, STORE_LINE1, STORE_LINE2, STORE_LINE3);
  return changed;
}

SpriteMatcherSignals SpriteMatcherRegisters::tick(const LcdSignals& lcd_sig,
                                                  const SpriteSignals& spr_sig,
                                                  const VidSignals& vid_sig,
                                                  const OamSignals& oam_sig,
                                                  bool MATCH_CHAINn,
                                                  bool STORE_SEL,
                                                  SpriteTristate& spr_tri) {

  /*p31.YMAM*/ wire MATCH0 = xor(STORE_X0, vid_sig.X0n);
  /*p31.YTYP*/ wire MATCH1 = xor(STORE_X1, vid_sig.X1n);
  /*p31.YFOP*/ wire MATCH2 = xor(STORE_X2, vid_sig.X2n);
  /*p31.YVAC*/ wire MATCH3 = xor(STORE_X3, vid_sig.X3n);
  /*p31.ZYWU*/ wire MATCH4 = xor(STORE_X4, vid_sig.X4n);
  /*p31.ZUZA*/ wire MATCH5 = xor(STORE_X5, vid_sig.X5n);
  /*p31.ZEJO*/ wire MATCH6 = xor(STORE_X6, vid_sig.X6n);
  /*p31.ZEDA*/ wire MATCH7 = xor(STORE_X7, vid_sig.X7n);
  /*p31.YLEV*/ wire MATCHA = nor(MATCH0, MATCH1, MATCH2, MATCH3);
  /*p31.YTUB*/ wire MATCHB = nor(MATCH4, MATCH5, MATCH6, MATCH7);
  /*p29.YGEM*/ wire MATCHn = nand(spr_sig.MATCH_EN, MATCHA, MATCHB);

  /*p29.CATO*/ wire STORE_CLK = or(spr_sig.STORE_EN, STORE_SEL);
  /*p29.DECU*/ wire STORE_CLKn = not(STORE_CLK);

  /*p29.GUZE*/ wire SPRITE_GETn = nor(MATCHn, MATCH_CHAINn);
  /*p29.FONO*/ SPRITE_GET_SYNCn.set(spr_sig.SPRITE_DONE, lcd_sig.VID_LINE_TRIG_d4p, SPRITE_GETn);

  /*p29.DUBU*/ wire STORE_RST  = or(lcd_sig.VID_LINE_TRIG_d4c, SPRITE_GET_SYNCn);
  /*p29.DOSY*/ wire STORE_RSTn = not(STORE_RST);
    
  /*p29.WEME*/ wire STORE_CLKa = not(STORE_CLKn);
  /*p31.XUVY*/ STORE_X0.set(STORE_CLKa, STORE_RSTn, oam_sig.OAM_A_D0b);
  /*p31.XERE*/ STORE_X1.set(STORE_CLKa, STORE_RSTn, oam_sig.OAM_A_D1b);
  /*p31.XUZO*/ STORE_X2.set(STORE_CLKa, STORE_RSTn, oam_sig.OAM_A_D2b);
  /*p31.XEXA*/ STORE_X3.set(STORE_CLKa, STORE_RSTn, oam_sig.OAM_A_D3b);
  /*p31.YPOD*/ STORE_X4.set(STORE_CLKa, STORE_RSTn, oam_sig.OAM_A_D4b);
  /*p31.YROP*/ STORE_X5.set(STORE_CLKa, STORE_RSTn, oam_sig.OAM_A_D5b);
  /*p31.YNEP*/ STORE_X6.set(STORE_CLKa, STORE_RSTn, oam_sig.OAM_A_D6b);
  /*p31.YZOF*/ STORE_X7.set(STORE_CLKa, STORE_RSTn, oam_sig.OAM_A_D7b);

  /*p29.WUFA*/ wire STORE_CLKb = not(STORE_CLKn);
  /*p30.XUFO*/ STORE_IDX0.set(STORE_CLKb, 1, spr_tri.TS_IDX_0);
  /*p30.XUTE*/ STORE_IDX1.set(STORE_CLKb, 1, spr_tri.TS_IDX_1);
  /*p30.XOTU*/ STORE_IDX2.set(STORE_CLKb, 1, spr_tri.TS_IDX_2);
  /*p30.XYFE*/ STORE_IDX3.set(STORE_CLKb, 1, spr_tri.TS_IDX_3);
  /*p30.YZOR*/ STORE_IDX4.set(STORE_CLKb, 1, spr_tri.TS_IDX_4);
  /*p30.YBER*/ STORE_IDX5.set(STORE_CLKb, 1, spr_tri.TS_IDX_5);

  /*p29.FAKA*/ wire STORE_CLKc = not(STORE_CLKn);
  /*p30.DEWU*/ STORE_LINE0.set(STORE_CLKc, 1, spr_tri.TS_LINE_0);
  /*p30.CANA*/ STORE_LINE1.set(STORE_CLKc, 1, spr_tri.TS_LINE_1);
  /*p30.DYSY*/ STORE_LINE2.set(STORE_CLKc, 1, spr_tri.TS_LINE_2);
  /*p30.FOFO*/ STORE_LINE3.set(STORE_CLKc, 1, spr_tri.TS_LINE_3);

  /*p29.FADO*/ wire SPRITE_GET = not(SPRITE_GETn);
  /*p30.YHAL*/ if (SPRITE_GET) spr_tri.TS_IDX_0  = not(!STORE_IDX0);
  /*p30.YRAD*/ if (SPRITE_GET) spr_tri.TS_IDX_1  = not(!STORE_IDX1);
  /*p30.XYRA*/ if (SPRITE_GET) spr_tri.TS_IDX_2  = not(!STORE_IDX2);
  /*p30.YNEV*/ if (SPRITE_GET) spr_tri.TS_IDX_3  = not(!STORE_IDX3);
  /*p30.ZOJY*/ if (SPRITE_GET) spr_tri.TS_IDX_4  = not(!STORE_IDX4);
  /*p30.ZARO*/ if (SPRITE_GET) spr_tri.TS_IDX_5  = not(!STORE_IDX5);
  /*p30.CAWO*/ if (SPRITE_GET) spr_tri.TS_LINE_0 = not(!STORE_LINE0);
  /*p30.BYME*/ if (SPRITE_GET) spr_tri.TS_LINE_1 = not(!STORE_LINE1);
  /*p30.COHO*/ if (SPRITE_GET) spr_tri.TS_LINE_2 = not(!STORE_LINE2);
  /*p30.GATE*/ if (SPRITE_GET) spr_tri.TS_LINE_3 = not(!STORE_LINE3);

  return {
    .MATCHn = MATCHn,
  };
}

//-----------------------------------------------------------------------------

bool SpriteStoreRegisters::pass_end() {
  bool changed = false;
  changed |= commit_all(store0, store1, store2, store3, store4, store5, store6, store7, store8, store9);
  changed |= commit_all(STORE_EN_SYNC, SPRITE_COUNT0, SPRITE_COUNT1, SPRITE_COUNT2, SPRITE_COUNT3);

  // FIXME should detect changes to sil here too

  return changed;
}

SpriteStoreSignals SpriteStoreRegisters::tick(const ClkSignals& clk_sig,
                                              const RstSignals& rst_sig,
                                              const LcdSignals& lcd_sig,
                                              const SpriteSignals& spr_sig,
                                              const VidSignals& vid_sig,
                                              const OamSignals& oam_sig) {

  /*p29.EDEN*/ wire SPRITE_COUNT0n = not(SPRITE_COUNT0);
  /*p29.CYPY*/ wire SPRITE_COUNT1n = not(SPRITE_COUNT1);
  /*p29.CAPE*/ wire SPRITE_COUNT2n = not(SPRITE_COUNT2);
  /*p29.CAXU*/ wire SPRITE_COUNT3n = not(SPRITE_COUNT3);

  /*p29.FYCU*/ wire SPRITE_COUNT0b = not(SPRITE_COUNT0n);
  /*p29.FONE*/ wire SPRITE_COUNT1b = not(SPRITE_COUNT1n);
  /*p29.EKUD*/ wire SPRITE_COUNT2b = not(SPRITE_COUNT2n);
  /*p29.ELYG*/ wire SPRITE_COUNT3b = not(SPRITE_COUNT3n);

  /*p29.DEZO*/ wire STORE0_SEL = nand(SPRITE_COUNT0n, SPRITE_COUNT1n, SPRITE_COUNT2n, SPRITE_COUNT3n);
  /*p29.CUVA*/ wire STORE1_SEL = nand(SPRITE_COUNT0b, SPRITE_COUNT1n, SPRITE_COUNT2n, SPRITE_COUNT3n);
  /*p29.GEBU*/ wire STORE2_SEL = nand(SPRITE_COUNT0n, SPRITE_COUNT1b, SPRITE_COUNT2n, SPRITE_COUNT3n);
  /*p29.FOCO*/ wire STORE3_SEL = nand(SPRITE_COUNT0b, SPRITE_COUNT1b, SPRITE_COUNT2n, SPRITE_COUNT3n);
  /*p29.CUPE*/ wire STORE4_SEL = nand(SPRITE_COUNT0n, SPRITE_COUNT1n, SPRITE_COUNT2b, SPRITE_COUNT3n);
  /*p29.CUGU*/ wire STORE5_SEL = nand(SPRITE_COUNT0b, SPRITE_COUNT1n, SPRITE_COUNT2b, SPRITE_COUNT3n);
  /*p29.WOMU*/ wire STORE6_SEL = nand(SPRITE_COUNT0n, SPRITE_COUNT1b, SPRITE_COUNT2b, SPRITE_COUNT3n);
  /*p29.GUNA*/ wire STORE7_SEL = nand(SPRITE_COUNT0b, SPRITE_COUNT1b, SPRITE_COUNT2b, SPRITE_COUNT3n);
  /*p29.DEWY*/ wire STORE8_SEL = nand(SPRITE_COUNT0n, SPRITE_COUNT1n, SPRITE_COUNT2n, SPRITE_COUNT3b);
  /*p29.DOGU*/ wire STORE9_SEL = nand(SPRITE_COUNT0b, SPRITE_COUNT1n, SPRITE_COUNT2n, SPRITE_COUNT3b);

  const auto store_sig0 = store0.tick(lcd_sig, spr_sig, vid_sig, oam_sig, false,             STORE0_SEL, spr_tri);
  const auto store_sig1 = store1.tick(lcd_sig, spr_sig, vid_sig, oam_sig, store_sig0.MATCHn, STORE1_SEL, spr_tri);
  const auto store_sig2 = store2.tick(lcd_sig, spr_sig, vid_sig, oam_sig, store_sig1.MATCHn, STORE2_SEL, spr_tri);
  const auto store_sig3 = store3.tick(lcd_sig, spr_sig, vid_sig, oam_sig, store_sig2.MATCHn, STORE3_SEL, spr_tri);
  const auto store_sig4 = store4.tick(lcd_sig, spr_sig, vid_sig, oam_sig, store_sig3.MATCHn, STORE4_SEL, spr_tri);
  const auto store_sig5 = store5.tick(lcd_sig, spr_sig, vid_sig, oam_sig, store_sig4.MATCHn, STORE5_SEL, spr_tri);
  const auto store_sig6 = store6.tick(lcd_sig, spr_sig, vid_sig, oam_sig, store_sig5.MATCHn, STORE6_SEL, spr_tri);
  const auto store_sig7 = store7.tick(lcd_sig, spr_sig, vid_sig, oam_sig, store_sig6.MATCHn, STORE7_SEL, spr_tri);
  const auto store_sig8 = store8.tick(lcd_sig, spr_sig, vid_sig, oam_sig, store_sig7.MATCHn, STORE8_SEL, spr_tri);
  const auto store_sig9 = store9.tick(lcd_sig, spr_sig, vid_sig, oam_sig, store_sig8.MATCHn, STORE9_SEL, spr_tri);

  /*p29.FEFY*/ wire FEFY = nand(store_sig4.MATCHn, store_sig3.MATCHn, store_sig2.MATCHn, store_sig1.MATCHn, store_sig0.MATCHn);
  /*p29.FOVE*/ wire FOVE = nand(store_sig9.MATCHn, store_sig8.MATCHn, store_sig7.MATCHn, store_sig6.MATCHn, store_sig5.MATCHn);
  /*p29.FEPO*/ wire STORE_MATCH = or(FEFY, FOVE);

  /*p29.DEZY*/ STORE_EN_SYNC.set(clk_sig.ZEME_xBxDxFxH, rst_sig.VID_RESETn, spr_sig.STORE_EN);

  /*p29.BAKY*/ wire SPRITES_FULL = and(SPRITE_COUNT1, SPRITE_COUNT3);
  /*p29.CAKE*/ wire SPRITE_COUNT_CLK = or(SPRITES_FULL, STORE_EN_SYNC);
  /*p28.AZYB*/ wire SPRITE_COUNT_RSTn = not(lcd_sig.VID_LINE_TRIG_d4n);
  /*p29.BESE*/ SPRITE_COUNT0.set(SPRITE_COUNT_CLK,  SPRITE_COUNT_RSTn, !SPRITE_COUNT0);
  /*p29.CUXY*/ SPRITE_COUNT1.set(SPRITE_COUNT0,     SPRITE_COUNT_RSTn, !SPRITE_COUNT1);
  /*p29.BEGO*/ SPRITE_COUNT2.set(SPRITE_COUNT1,     SPRITE_COUNT_RSTn, !SPRITE_COUNT2);
  /*p29.DYBE*/ SPRITE_COUNT3.set(SPRITE_COUNT2,     SPRITE_COUNT_RSTn, !SPRITE_COUNT3);

  return {
    .STORE_MATCH = STORE_MATCH,
  };
}













//-----------------------------------------------------------------------------

#if 0
  //----------
  // store 0

  {
    /*p31.ZOGY*/ wire STORE0_MATCH0n = xor(b.spr.STORE0_X0, b.vid.X0n);
    /*p31.ZEBA*/ wire STORE0_MATCH1n = xor(b.spr.STORE0_X1, b.vid.X1n);
    /*p31.ZAHA*/ wire STORE0_MATCH2n = xor(b.spr.STORE0_X2, b.vid.X2n);
    /*p31.ZOKY*/ wire STORE0_MATCH3n = xor(b.spr.STORE0_X3, b.vid.X3n);
    /*p31.WOJU*/ wire STORE0_MATCH4n = xor(b.spr.STORE0_X4, b.vid.X4n);
    /*p31.YFUN*/ wire STORE0_MATCH5n = xor(b.spr.STORE0_X5, b.vid.X5n);
    /*p31.WYZA*/ wire STORE0_MATCH6n = xor(b.spr.STORE0_X6, b.vid.X6n);
    /*p31.YPUK*/ wire STORE0_MATCH7n = xor(b.spr.STORE0_X7, b.vid.X7n);
    /*p31.ZAKO*/ wire STORE0_MATCHA = nor(STORE0_MATCH0n, STORE0_MATCH1n, STORE0_MATCH2n, STORE0_MATCH3n);
    /*p31.XEBA*/ wire STORE0_MATCHB = nor(STORE0_MATCH4n, STORE0_MATCH5n, STORE0_MATCH6n, STORE0_MATCH7n);

    /*p29.YDUG*/ spr.STORE0_MATCHn = nand(b.spr.MATCH_EN, STORE0_MATCHB, STORE0_MATCHA);
    /*p29.WEFU*/ wire STORE0_MATCH = not(b.spr.STORE0_MATCHn);
    /*p29.GEZE*/ spr.STORE0_MATCH_OUT = or (STORE0_MATCH, pins.P10_B);

    /*p29.GUVA*/ wire SPRITE0_GET = nor(b.spr.STORE0_MATCHn,  pins.P10_B);
    /*p29.EBOJ*/ spr.SPRITE0_GET_SYNCn = tock_pos(a.spr.SPRITE_DONE, b.spr.SPRITE_DONE, b.lcd.VID_LINE_TRIG_d4p, b.spr.SPRITE0_GET_SYNCn, SPRITE0_GET);

    /*p29.CEMY*/ wire STORE0_CLK    = or(b.spr.STORE_EN, STORE0_SEL);
    /*p29.DYHU*/ wire STORE0_CLKn   = not(STORE0_CLK);
    /*p29.DYWE*/ wire STORE0_RST    = or(b.lcd.VID_LINE_TRIG_d4c, b.spr.SPRITE0_GET_SYNCn);
    /*p29.DYNA*/ wire STORE0_RSTn   = not(STORE0_RST);

    /*p29.GENY*/ spr.STORE0_CLKa  = not(STORE0_CLKn);
    /*p30.YGUS*/ spr.STORE0_IDX0  = tock_pos(a.spr.STORE0_CLKa, b.spr.STORE0_CLKa, 0,           b.spr.STORE0_IDX0,  b.spr.TS_IDX_0);
    /*p30.YSOK*/ spr.STORE0_IDX1  = tock_pos(a.spr.STORE0_CLKa, b.spr.STORE0_CLKa, 0,           b.spr.STORE0_IDX1,  b.spr.TS_IDX_1);
    /*p30.YZEP*/ spr.STORE0_IDX2  = tock_pos(a.spr.STORE0_CLKa, b.spr.STORE0_CLKa, 0,           b.spr.STORE0_IDX2,  b.spr.TS_IDX_2);
    /*p30.WYTE*/ spr.STORE0_IDX3  = tock_pos(a.spr.STORE0_CLKa, b.spr.STORE0_CLKa, 0,           b.spr.STORE0_IDX3,  b.spr.TS_IDX_3);
    /*p30.ZONY*/ spr.STORE0_IDX4  = tock_pos(a.spr.STORE0_CLKa, b.spr.STORE0_CLKa, 0,           b.spr.STORE0_IDX4,  b.spr.TS_IDX_4);
    /*p30.YWAK*/ spr.STORE0_IDX5  = tock_pos(a.spr.STORE0_CLKa, b.spr.STORE0_CLKa, 0,           b.spr.STORE0_IDX5,  b.spr.TS_IDX_5);
    
    /*p29.ENOB*/ spr.STORE0_CLKb  = not(STORE0_CLKn);
    /*p30.FYHY*/ spr.STORE0_LINE0 = tock_pos(a.spr.STORE0_CLKb, b.spr.STORE0_CLKb, 0,           b.spr.STORE0_LINE0, b.spr.TS_LINE_0);
    /*p30.GYHO*/ spr.STORE0_LINE1 = tock_pos(a.spr.STORE0_CLKb, b.spr.STORE0_CLKb, 0,           b.spr.STORE0_LINE1, b.spr.TS_LINE_1);
    /*p30.BOZU*/ spr.STORE0_LINE2 = tock_pos(a.spr.STORE0_CLKb, b.spr.STORE0_CLKb, 0,           b.spr.STORE0_LINE2, b.spr.TS_LINE_2);
    /*p30.CUFO*/ spr.STORE0_LINE3 = tock_pos(a.spr.STORE0_CLKb, b.spr.STORE0_CLKb, 0,           b.spr.STORE0_LINE3, b.spr.TS_LINE_3);

    /*p29.FUXU*/ spr.STORE0_CLKc  = not(STORE0_CLKn);
    /*p31.XEPE*/ spr.STORE0_X0    = tock_pos(a.spr.STORE0_CLKc, b.spr.STORE0_CLKc, STORE0_RSTn, b.spr.STORE0_X0,    b.spr.OAM_A_D0b);
    /*p31.YLAH*/ spr.STORE0_X1    = tock_pos(a.spr.STORE0_CLKc, b.spr.STORE0_CLKc, STORE0_RSTn, b.spr.STORE0_X1,    b.spr.OAM_A_D1b);
    /*p31.ZOLA*/ spr.STORE0_X2    = tock_pos(a.spr.STORE0_CLKc, b.spr.STORE0_CLKc, STORE0_RSTn, b.spr.STORE0_X2,    b.spr.OAM_A_D2b);
    /*p31.ZULU*/ spr.STORE0_X3    = tock_pos(a.spr.STORE0_CLKc, b.spr.STORE0_CLKc, STORE0_RSTn, b.spr.STORE0_X3,    b.spr.OAM_A_D3b);
    /*p31.WELO*/ spr.STORE0_X4    = tock_pos(a.spr.STORE0_CLKc, b.spr.STORE0_CLKc, STORE0_RSTn, b.spr.STORE0_X4,    b.spr.OAM_A_D4b);
    /*p31.XUNY*/ spr.STORE0_X5    = tock_pos(a.spr.STORE0_CLKc, b.spr.STORE0_CLKc, STORE0_RSTn, b.spr.STORE0_X5,    b.spr.OAM_A_D5b);
    /*p31.WOTE*/ spr.STORE0_X6    = tock_pos(a.spr.STORE0_CLKc, b.spr.STORE0_CLKc, STORE0_RSTn, b.spr.STORE0_X6,    b.spr.OAM_A_D6b);
    /*p31.XAKO*/ spr.STORE0_X7    = tock_pos(a.spr.STORE0_CLKc, b.spr.STORE0_CLKc, STORE0_RSTn, b.spr.STORE0_X7,    b.spr.OAM_A_D7b);

    /*p29.FURO*/ wire SPRITE0_GETn = not(SPRITE0_GET);

    // small tri - input must be inverting
    /*p30.ZETU*/ if (!SPRITE0_GETn) spr.TS_IDX_0  = b.spr.STORE0_IDX0;
    /*p30.ZECE*/ if (!SPRITE0_GETn) spr.TS_IDX_1  = b.spr.STORE0_IDX1;
    /*p30.ZAVE*/ if (!SPRITE0_GETn) spr.TS_IDX_2  = b.spr.STORE0_IDX2;
    /*p30.WOKO*/ if (!SPRITE0_GETn) spr.TS_IDX_3  = b.spr.STORE0_IDX3;
    /*p30.ZUMU*/ if (!SPRITE0_GETn) spr.TS_IDX_4  = b.spr.STORE0_IDX4;
    /*p30.ZEDY*/ if (!SPRITE0_GETn) spr.TS_IDX_5  = b.spr.STORE0_IDX5;
    /*p30.GOFO*/ if (!SPRITE0_GETn) spr.TS_LINE_0 = b.spr.STORE0_LINE0;
    /*p30.WEHE*/ if (!SPRITE0_GETn) spr.TS_LINE_1 = b.spr.STORE0_LINE1;
    /*p30.AJAL*/ if (!SPRITE0_GETn) spr.TS_LINE_2 = b.spr.STORE0_LINE2;
    /*p30.BUKY*/ if (!SPRITE0_GETn) spr.TS_LINE_3 = b.spr.STORE0_LINE3;
  }

  //----------
  // store 1

  {
    /*p31.EDYM*/ wire STORE1_MATCH0 = xor(b.spr.STORE1_X0, b.vid.X0n);
    /*p31.EMYB*/ wire STORE1_MATCH1 = xor(b.spr.STORE1_X1, b.vid.X1n);
    /*p31.EBEF*/ wire STORE1_MATCH2 = xor(b.spr.STORE1_X2, b.vid.X2n);
    /*p31.EWOK*/ wire STORE1_MATCH3 = xor(b.spr.STORE1_X3, b.vid.X3n);
    /*p31.COLA*/ wire STORE1_MATCH4 = xor(b.spr.STORE1_X4, b.vid.X4n);
    /*p31.BOBA*/ wire STORE1_MATCH5 = xor(b.spr.STORE1_X5, b.vid.X5n);
    /*p31.COLU*/ wire STORE1_MATCH6 = xor(b.spr.STORE1_X6, b.vid.X6n);
    /*p31.BAHU*/ wire STORE1_MATCH7 = xor(b.spr.STORE1_X7, b.vid.X7n);
    /*p31.CYVY*/ wire STORE1_MATCHB = nor(STORE1_MATCH4, STORE1_MATCH5, STORE1_MATCH6, STORE1_MATCH7);
    /*p31.EWAM*/ wire STORE1_MATCHA = nor(STORE1_MATCH0, STORE1_MATCH1, STORE1_MATCH2, STORE1_MATCH3);

    /*p29.DYDU*/ spr.STORE1_MATCHn    = nand(b.spr.MATCH_EN, STORE1_MATCHA, STORE1_MATCHB);
    /*p29.GAJA*/ wire STORE1_MATCH = not(b.spr.STORE1_MATCHn);
    /*p29.FUMA*/ spr.STORE1_MATCH_OUT = or(STORE1_MATCH, b.spr.STORE0_MATCH_OUT);

    /*p29.ENUT*/ wire SPRITE1_GETn = nor(b.spr.STORE1_MATCHn, b.spr.STORE0_MATCH_OUT);

    /*p29.BYBY*/ wire STORE1_CLK  = or(b.spr.STORE_EN, STORE1_SEL);
    /*p29.BUCO*/ wire STORE1_CLKn = not(STORE1_CLK);

    /*p29.CEDY*/ spr.SPRITE1_GET_SYNCn = tock_pos(a.spr.SPRITE_DONE, b.spr.SPRITE_DONE, b.lcd.VID_LINE_TRIG_d4p, b.spr.SPRITE1_GET_SYNCn, SPRITE1_GETn);
    /*p29.EFEV*/ wire STORE1_RST = or(b.lcd.VID_LINE_TRIG_d4c, b.spr.SPRITE1_GET_SYNCn);
    /*p29.DOKU*/ wire STORE1_RSTn = not(STORE1_RST);

    /*p29.BYVY*/ spr.STORE1_CLKc = not(STORE1_CLKn);
    /*p30.CAJY*/ spr.STORE1_IDX0  = tock_pos(a.spr.STORE1_CLKc, b.spr.STORE1_CLKc, 0,    b.spr.STORE1_IDX0,  b.spr.TS_IDX_0 );
    /*p30.CUZA*/ spr.STORE1_IDX1  = tock_pos(a.spr.STORE1_CLKc, b.spr.STORE1_CLKc, 0,    b.spr.STORE1_IDX1,  b.spr.TS_IDX_1 );
    /*p30.COMA*/ spr.STORE1_IDX2  = tock_pos(a.spr.STORE1_CLKc, b.spr.STORE1_CLKc, 0,    b.spr.STORE1_IDX2,  b.spr.TS_IDX_2 );
    /*p30.CUFA*/ spr.STORE1_IDX3  = tock_pos(a.spr.STORE1_CLKc, b.spr.STORE1_CLKc, 0,    b.spr.STORE1_IDX3,  b.spr.TS_IDX_3 );
    /*p30.CEBO*/ spr.STORE1_IDX4  = tock_pos(a.spr.STORE1_CLKc, b.spr.STORE1_CLKc, 0,    b.spr.STORE1_IDX4,  b.spr.TS_IDX_4 );
    /*p30.CADU*/ spr.STORE1_IDX5  = tock_pos(a.spr.STORE1_CLKc, b.spr.STORE1_CLKc, 0,    b.spr.STORE1_IDX5,  b.spr.TS_IDX_5 );
                                                                                           
    /*p29.AHOF*/ spr.STORE1_CLKb = not(STORE1_CLKn);
    /*p30.ABUG*/ spr.STORE1_LINE0 = tock_pos(a.spr.STORE1_CLKb, b.spr.STORE1_CLKb, 0,    b.spr.STORE1_LINE0, b.spr.TS_LINE_0);
    /*p30.AMES*/ spr.STORE1_LINE1 = tock_pos(a.spr.STORE1_CLKb, b.spr.STORE1_CLKb, 0,    b.spr.STORE1_LINE1, b.spr.TS_LINE_1);
    /*p30.ABOP*/ spr.STORE1_LINE2 = tock_pos(a.spr.STORE1_CLKb, b.spr.STORE1_CLKb, 0,    b.spr.STORE1_LINE2, b.spr.TS_LINE_2);
    /*p30.AROF*/ spr.STORE1_LINE3 = tock_pos(a.spr.STORE1_CLKb, b.spr.STORE1_CLKb, 0,    b.spr.STORE1_LINE3, b.spr.TS_LINE_3);

    /*p29.ASYS*/ spr.STORE1_CLKa = not(STORE1_CLKn);
    /*p31.DANY*/ spr.STORE1_X0    = tock_pos(a.spr.STORE1_CLKa, b.spr.STORE1_CLKa, STORE1_RSTn, b.spr.STORE1_X0,    b.spr.OAM_A_D0b);
    /*p31.DUKO*/ spr.STORE1_X1    = tock_pos(a.spr.STORE1_CLKa, b.spr.STORE1_CLKa, STORE1_RSTn, b.spr.STORE1_X1,    b.spr.OAM_A_D1b);
    /*p31.DESU*/ spr.STORE1_X2    = tock_pos(a.spr.STORE1_CLKa, b.spr.STORE1_CLKa, STORE1_RSTn, b.spr.STORE1_X2,    b.spr.OAM_A_D2b);
    /*p31.DAZO*/ spr.STORE1_X3    = tock_pos(a.spr.STORE1_CLKa, b.spr.STORE1_CLKa, STORE1_RSTn, b.spr.STORE1_X3,    b.spr.OAM_A_D3b);
    /*p31.DAKE*/ spr.STORE1_X4    = tock_pos(a.spr.STORE1_CLKa, b.spr.STORE1_CLKa, STORE1_RSTn, b.spr.STORE1_X4,    b.spr.OAM_A_D4b);
    /*p31.CESO*/ spr.STORE1_X5    = tock_pos(a.spr.STORE1_CLKa, b.spr.STORE1_CLKa, STORE1_RSTn, b.spr.STORE1_X5,    b.spr.OAM_A_D5b);
    /*p31.DYFU*/ spr.STORE1_X6    = tock_pos(a.spr.STORE1_CLKa, b.spr.STORE1_CLKa, STORE1_RSTn, b.spr.STORE1_X6,    b.spr.OAM_A_D6b);
    /*p31.CUSY*/ spr.STORE1_X7    = tock_pos(a.spr.STORE1_CLKa, b.spr.STORE1_CLKa, STORE1_RSTn, b.spr.STORE1_X7,    b.spr.OAM_A_D7b);

    /*p29.DYDO*/ wire SPRITE1_GET = not(SPRITE1_GETn);
    /*p30.BEMO*/ if (SPRITE1_GET) spr.TS_IDX_0  = not(!b.spr.STORE1_IDX0);
    /*p30.CYBY*/ if (SPRITE1_GET) spr.TS_IDX_1  = not(!b.spr.STORE1_IDX1);
    /*p30.BETY*/ if (SPRITE1_GET) spr.TS_IDX_2  = not(!b.spr.STORE1_IDX2);
    /*p30.CEGY*/ if (SPRITE1_GET) spr.TS_IDX_3  = not(!b.spr.STORE1_IDX3);
    /*p30.CELU*/ if (SPRITE1_GET) spr.TS_IDX_4  = not(!b.spr.STORE1_IDX4);
    /*p30.CUBO*/ if (SPRITE1_GET) spr.TS_IDX_5  = not(!b.spr.STORE1_IDX5);
    /*p30.BEFE*/ if (SPRITE1_GET) spr.TS_LINE_0 = not(!b.spr.STORE1_LINE0);
    /*p30.BYRO*/ if (SPRITE1_GET) spr.TS_LINE_1 = not(!b.spr.STORE1_LINE1);
    /*p30.BACO*/ if (SPRITE1_GET) spr.TS_LINE_2 = not(!b.spr.STORE1_LINE2);
    /*p30.AHUM*/ if (SPRITE1_GET) spr.TS_LINE_3 = not(!b.spr.STORE1_LINE3);
  }

  //----------
  // store 2

  {
    /*p31.FUZU*/ wire STORE2_MATCH0 = xor(b.spr.STORE2_X0, b.vid.X0n);
    /*p31.FESO*/ wire STORE2_MATCH1 = xor(b.spr.STORE2_X1, b.vid.X1n);
    /*p31.FOKY*/ wire STORE2_MATCH2 = xor(b.spr.STORE2_X2, b.vid.X2n);
    /*p31.FYVA*/ wire STORE2_MATCH3 = xor(b.spr.STORE2_X3, b.vid.X3n);
    /*p31.CEKO*/ wire STORE2_MATCH4 = xor(b.spr.STORE2_X4, b.vid.X4n);
    /*p31.DETY*/ wire STORE2_MATCH5 = xor(b.spr.STORE2_X5, b.vid.X5n);
    /*p31.DOZO*/ wire STORE2_MATCH6 = xor(b.spr.STORE2_X6, b.vid.X6n);
    /*p31.CONY*/ wire STORE2_MATCH7 = xor(b.spr.STORE2_X7, b.vid.X7n);
    /*p31.CEHU*/ wire STORE2_MATCHA = nor(STORE2_MATCH4, STORE2_MATCH5, STORE2_MATCH6, STORE2_MATCH7);
    /*p31.EKES*/ wire STORE2_MATCHB = nor(STORE2_MATCH0, STORE2_MATCH1, STORE2_MATCH2, STORE2_MATCH3);

    /*p29.DEGO*/ spr.STORE2_MATCHn = nand(b.spr.MATCH_EN, STORE2_MATCHB, STORE2_MATCHA);
    /*p29.GUPO*/ wire STORE2_MATCH = not(b.spr.STORE2_MATCHn);
    /*p29.GEDE*/ spr.STORE2_MATCH_OUT = or(STORE2_MATCH, b.spr.STORE1_MATCH_OUT);

    /*p29.EMOL*/ wire SPRITE2_GETn = nor(spr.STORE2_MATCHn, b.spr.STORE1_MATCH_OUT);
    /*p29.EGAV*/ spr.SPRITE2_GET_SYNCn = tock_pos(a.spr.SPRITE_DONE, b.spr.SPRITE_DONE, b.lcd.VID_LINE_TRIG_d4p, b.spr.SPRITE2_GET_SYNCn, SPRITE2_GETn);

    /*p29.WYXO*/ wire STORE2_CLK = or(b.spr.STORE_EN, STORE2_SEL); // 0010
    /*p29.GYFO*/ wire STORE2_CLKn = not(STORE2_CLK); // 0010
    /*p29.FOKO*/ wire STORE2_RST  = or(b.lcd.VID_LINE_TRIG_d4c, b.spr.SPRITE2_GET_SYNCn);
    /*p29.GAMY*/ wire STORE2_RSTn = not(STORE2_RST);

    /*p29.BUZY*/ spr.STORE2_CLKb = not(STORE2_CLKn);
    /*p30.BOXA*/ spr.STORE2_IDX0 = tock_pos(a.spr.STORE2_CLKb, b.spr.STORE2_CLKb, 0, b.spr.STORE2_IDX0, b.spr.TS_IDX_0 );
    /*p30.BUNA*/ spr.STORE2_IDX1 = tock_pos(a.spr.STORE2_CLKb, b.spr.STORE2_CLKb, 0, b.spr.STORE2_IDX1, b.spr.TS_IDX_1 );
    /*p30.BULU*/ spr.STORE2_IDX2 = tock_pos(a.spr.STORE2_CLKb, b.spr.STORE2_CLKb, 0, b.spr.STORE2_IDX2, b.spr.TS_IDX_2 );
    /*p30.BECA*/ spr.STORE2_IDX3 = tock_pos(a.spr.STORE2_CLKb, b.spr.STORE2_CLKb, 0, b.spr.STORE2_IDX3, b.spr.TS_IDX_3 );
    /*p30.BYHU*/ spr.STORE2_IDX4 = tock_pos(a.spr.STORE2_CLKb, b.spr.STORE2_CLKb, 0, b.spr.STORE2_IDX4, b.spr.TS_IDX_4 );
    /*p30.BUHE*/ spr.STORE2_IDX5 = tock_pos(a.spr.STORE2_CLKb, b.spr.STORE2_CLKb, 0, b.spr.STORE2_IDX5, b.spr.TS_IDX_5 );

    /*p29.FUKE*/ spr.STORE2_CLKc = not(STORE2_CLKn);
    /*p30.YKUK*/ spr.STORE2_LINE0 = tock_pos(a.spr.STORE2_CLKc, b.spr.STORE2_CLKc, 0, b.spr.STORE2_LINE0, b.spr.TS_LINE_0);
    /*p30.YLOV*/ spr.STORE2_LINE1 = tock_pos(a.spr.STORE2_CLKc, b.spr.STORE2_CLKc, 0, b.spr.STORE2_LINE1, b.spr.TS_LINE_1);
    /*p30.XAZY*/ spr.STORE2_LINE2 = tock_pos(a.spr.STORE2_CLKc, b.spr.STORE2_CLKc, 0, b.spr.STORE2_LINE2, b.spr.TS_LINE_2);
    /*p30.XOSY*/ spr.STORE2_LINE3 = tock_pos(a.spr.STORE2_CLKc, b.spr.STORE2_CLKc, 0, b.spr.STORE2_LINE3, b.spr.TS_LINE_3);

    /*p29.CACU*/ spr.STORE2_CLKa = not(STORE2_CLKn);
    /*p31.FOKA*/ spr.STORE2_X0 = tock_pos(a.spr.STORE2_CLKa, b.spr.STORE2_CLKa, STORE2_RSTn, b.spr.STORE2_X0, b.spr.OAM_A_D0b);
    /*p31.FYTY*/ spr.STORE2_X1 = tock_pos(a.spr.STORE2_CLKa, b.spr.STORE2_CLKa, STORE2_RSTn, b.spr.STORE2_X1, b.spr.OAM_A_D1b);
    /*p31.FUBY*/ spr.STORE2_X2 = tock_pos(a.spr.STORE2_CLKa, b.spr.STORE2_CLKa, STORE2_RSTn, b.spr.STORE2_X2, b.spr.OAM_A_D2b);
    /*p31.GOXU*/ spr.STORE2_X3 = tock_pos(a.spr.STORE2_CLKa, b.spr.STORE2_CLKa, STORE2_RSTn, b.spr.STORE2_X3, b.spr.OAM_A_D3b);
    /*p31.DUHY*/ spr.STORE2_X4 = tock_pos(a.spr.STORE2_CLKa, b.spr.STORE2_CLKa, STORE2_RSTn, b.spr.STORE2_X4, b.spr.OAM_A_D4b);
    /*p31.EJUF*/ spr.STORE2_X5 = tock_pos(a.spr.STORE2_CLKa, b.spr.STORE2_CLKa, STORE2_RSTn, b.spr.STORE2_X5, b.spr.OAM_A_D5b);
    /*p31.ENOR*/ spr.STORE2_X6 = tock_pos(a.spr.STORE2_CLKa, b.spr.STORE2_CLKa, STORE2_RSTn, b.spr.STORE2_X6, b.spr.OAM_A_D6b);
    /*p31.DEPY*/ spr.STORE2_X7 = tock_pos(a.spr.STORE2_CLKa, b.spr.STORE2_CLKa, STORE2_RSTn, b.spr.STORE2_X7, b.spr.OAM_A_D7b);


    /*p29.FAME*/ wire SPRITE2_GET = not(SPRITE2_GETn);
    /*p30.AXEC*/ if (SPRITE2_GET) spr.TS_IDX_0  = not(!b.spr.STORE2_IDX0);
    /*p30.CYRO*/ if (SPRITE2_GET) spr.TS_IDX_1  = not(!b.spr.STORE2_IDX1);
    /*p30.CUVU*/ if (SPRITE2_GET) spr.TS_IDX_2  = not(!b.spr.STORE2_IDX2);
    /*p30.APON*/ if (SPRITE2_GET) spr.TS_IDX_3  = not(!b.spr.STORE2_IDX3);
    /*p30.AFOZ*/ if (SPRITE2_GET) spr.TS_IDX_4  = not(!b.spr.STORE2_IDX4);
    /*p30.CUBE*/ if (SPRITE2_GET) spr.TS_IDX_5  = not(!b.spr.STORE2_IDX5);
    /*p30.ZABY*/ if (SPRITE2_GET) spr.TS_LINE_0 = not(!b.spr.STORE2_LINE0);
    /*p30.ZUKE*/ if (SPRITE2_GET) spr.TS_LINE_1 = not(!b.spr.STORE2_LINE1);
    /*p30.WUXE*/ if (SPRITE2_GET) spr.TS_LINE_2 = not(!b.spr.STORE2_LINE2);
    /*p30.WERE*/ if (SPRITE2_GET) spr.TS_LINE_3 = not(!b.spr.STORE2_LINE3);
  }

  // store 3
  {
    /*p31.YHOK*/ wire STORE3_MATCH0 = xor(b.spr.XOLY, b.vid.X0n);
    /*p31.YCAH*/ wire STORE3_MATCH1 = xor(b.spr.XYBA, b.vid.X1n);
    /*p31.YDAJ*/ wire STORE3_MATCH2 = xor(b.spr.XABE, b.vid.X2n);
    /*p31.YVUZ*/ wire STORE3_MATCH3 = xor(b.spr.XEKA, b.vid.X3n);
    /*p31.YVAP*/ wire STORE3_MATCH4 = xor(b.spr.XOMY, b.vid.X4n);
    /*p31.XENY*/ wire STORE3_MATCH5 = xor(b.spr.WUHA, b.vid.X5n);
    /*p31.XAVU*/ wire STORE3_MATCH6 = xor(b.spr.WYNA, b.vid.X6n);
    /*p31.XEVA*/ wire STORE3_MATCH7 = xor(b.spr.WECO, b.vid.X7n);
    /*p31.ZURE*/ wire STORE3_MATCHA = nor(STORE3_MATCH0, STORE3_MATCH1, STORE3_MATCH2, STORE3_MATCH3);
    /*p31.YWOS*/ wire STORE3_MATCHB = nor(STORE3_MATCH4, STORE3_MATCH5, STORE3_MATCH6, STORE3_MATCH7);

    /*p29.YLOZ*/ spr.STORE3_MATCHn = nand(b.spr.MATCH_EN, STORE3_MATCHA, STORE3_MATCHB);
    /*p29.WEBO*/ wire STORE3_MATCH = not(b.spr.STORE3_MATCHn);
    /*p29.WUTO*/ spr.STORE3_MATCH_OUT = or(STORE3_MATCH, b.spr.STORE2_MATCH_OUT);

    /*p29.GYFY*/ wire SPRITE3_GETn = nor(b.spr.STORE3_MATCHn, b.spr.STORE2_MATCH_OUT);
    /*p29.GOTA*/ spr.SPRITE3_GET_SYNCn = tock_pos(a.spr.SPRITE_DONE, b.spr.SPRITE_DONE, b.lcd.VID_LINE_TRIG_d4p, b.spr.SPRITE3_GET_SYNCn, SPRITE3_GETn);

    /*p29.GAKE*/ wire STORE3_RST = or(b.lcd.VID_LINE_TRIG_d4c, b.spr.SPRITE3_GET_SYNCn);
    /*p29.WUPA*/ wire STORE3_RSTn = not(STORE3_RST);

    /*p29.GUVE*/ wire STORE3_CLK = or(b.spr.STORE_EN, STORE3_SEL); // 0011
    /*p29.GUSA*/ wire STORE3_CLKn = not(STORE3_CLK);
    
    /*p29.FEKA*/ spr.FEKA = not(STORE3_CLKn);
    /*p30.DAFU*/ spr.DAFU = tock_pos(a.spr.FEKA, b.spr.FEKA, 0,          b.spr.DAFU, b.spr.TS_IDX_0 );
    /*p30.DEBA*/ spr.DEBA = tock_pos(a.spr.FEKA, b.spr.FEKA, 0,          b.spr.DEBA, b.spr.TS_IDX_1 );
    /*p30.DUHA*/ spr.DUHA = tock_pos(a.spr.FEKA, b.spr.FEKA, 0,          b.spr.DUHA, b.spr.TS_IDX_2 );
    /*p30.DUNY*/ spr.DUNY = tock_pos(a.spr.FEKA, b.spr.FEKA, 0,          b.spr.DUNY, b.spr.TS_IDX_3 );
    /*p30.DESE*/ spr.DESE = tock_pos(a.spr.FEKA, b.spr.FEKA, 0,          b.spr.DESE, b.spr.TS_IDX_4 );
    /*p30.DEVY*/ spr.DEVY = tock_pos(a.spr.FEKA, b.spr.FEKA, 0,          b.spr.DEVY, b.spr.TS_IDX_5 );

    /*p29.XYHA*/ spr.XYHA = not(STORE3_CLKn);
    /*p30.ZURY*/ spr.ZURY = tock_pos(a.spr.XYHA, b.spr.XYHA, 0,          b.spr.ZURY, b.spr.TS_LINE_0);
    /*p30.ZURO*/ spr.ZURO = tock_pos(a.spr.XYHA, b.spr.XYHA, 0,          b.spr.ZURO, b.spr.TS_LINE_1);
    /*p30.ZENE*/ spr.ZENE = tock_pos(a.spr.XYHA, b.spr.XYHA, 0,          b.spr.ZENE, b.spr.TS_LINE_2);
    /*p30.ZYLU*/ spr.ZYLU = tock_pos(a.spr.XYHA, b.spr.XYHA, 0,          b.spr.ZYLU, b.spr.TS_LINE_3);

    /*p29.YFAG*/ spr.YFAG = not(STORE3_CLKn);
    /*p31.XOLY*/ spr.XOLY = tock_pos(a.spr.YFAG, b.spr.YFAG, STORE3_RSTn, b.spr.XOLY, b.spr.OAM_A_D0b);
    /*p31.XYBA*/ spr.XYBA = tock_pos(a.spr.YFAG, b.spr.YFAG, STORE3_RSTn, b.spr.XYBA, b.spr.OAM_A_D1b);
    /*p31.XABE*/ spr.XABE = tock_pos(a.spr.YFAG, b.spr.YFAG, STORE3_RSTn, b.spr.XABE, b.spr.OAM_A_D2b);
    /*p31.XEKA*/ spr.XEKA = tock_pos(a.spr.YFAG, b.spr.YFAG, STORE3_RSTn, b.spr.XEKA, b.spr.OAM_A_D3b);
    /*p31.XOMY*/ spr.XOMY = tock_pos(a.spr.YFAG, b.spr.YFAG, STORE3_RSTn, b.spr.XOMY, b.spr.OAM_A_D4b);
    /*p31.WUHA*/ spr.WUHA = tock_pos(a.spr.YFAG, b.spr.YFAG, STORE3_RSTn, b.spr.WUHA, b.spr.OAM_A_D5b);
    /*p31.WYNA*/ spr.WYNA = tock_pos(a.spr.YFAG, b.spr.YFAG, STORE3_RSTn, b.spr.WYNA, b.spr.OAM_A_D6b);
    /*p31.WECO*/ spr.WECO = tock_pos(a.spr.YFAG, b.spr.YFAG, STORE3_RSTn, b.spr.WECO, b.spr.OAM_A_D7b);

    /*p29.GYMA*/ wire SPRITE3_GET = not(SPRITE3_GETn);
    /*p30.DEZU*/ if (SPRITE3_GET) spr.TS_IDX_0  = not(!b.spr.DAFU);
    /*p30.EFUD*/ if (SPRITE3_GET) spr.TS_IDX_1  = not(!b.spr.DEBA);
    /*p30.DONY*/ if (SPRITE3_GET) spr.TS_IDX_2  = not(!b.spr.DUHA);
    /*p30.DOWA*/ if (SPRITE3_GET) spr.TS_IDX_3  = not(!b.spr.DUNY);
    /*p30.DYGO*/ if (SPRITE3_GET) spr.TS_IDX_4  = not(!b.spr.DESE);
    /*p30.ENAP*/ if (SPRITE3_GET) spr.TS_IDX_5  = not(!b.spr.DEVY);
    /*p30.ZYPO*/ if (SPRITE3_GET) spr.TS_LINE_0 = not(!b.spr.ZURY);
    /*p30.ZEXE*/ if (SPRITE3_GET) spr.TS_LINE_1 = not(!b.spr.ZURO);
    /*p30.YJEM*/ if (SPRITE3_GET) spr.TS_LINE_2 = not(!b.spr.ZENE);
    /*p30.YWAV*/ if (SPRITE3_GET) spr.TS_LINE_3 = not(!b.spr.ZYLU);
  }

  // store 4
  {
    /*p31.ZYKU*/ wire ZYKU = xor(b.spr.YBED, b.vid.X4n);
    /*p31.ZYPU*/ wire ZYPU = xor(b.spr.ZALA, b.vid.X5n);
    /*p31.XAHA*/ wire XAHA = xor(b.spr.WYDE, b.vid.X6n);
    /*p31.ZEFE*/ wire ZEFE = xor(b.spr.XEPA, b.vid.X7n);
    /*p31.XEJU*/ wire XEJU = xor(b.spr.WEDU, b.vid.X0n);
    /*p31.ZATE*/ wire ZATE = xor(b.spr.YGAJ, b.vid.X1n);
    /*p31.ZAKU*/ wire ZAKU = xor(b.spr.ZYJO, b.vid.X2n);
    /*p31.YBOX*/ wire YBOX = xor(b.spr.XURY, b.vid.X3n);
    /*p31.YKOK*/ wire YKOK = nor(ZYKU, ZYPU, XAHA, ZEFE);
    /*p31.YNAZ*/ wire YNAZ = nor(XEJU, ZATE, ZAKU, YBOX);
    /*p29.XAGE*/ spr.STORE4_MATCHn = nand(b.spr.MATCH_EN, YNAZ, YKOK);
    /*p29.WUNA*/ wire STORE4_MATCH = not(b.spr.STORE4_MATCHn);

    /*p29.XUDY*/ spr.SPRITE4_GET_SYNCn = tock_pos(a.spr.SPRITE_DONE, b.spr.SPRITE_DONE, b.lcd.VID_LINE_TRIG_d4p, b.spr.SPRITE4_GET_SYNCn, b.spr.GONO);
    /*p29.WOHU*/ spr.WOHU = or(b.lcd.VID_LINE_TRIG_d4c, b.spr.SPRITE4_GET_SYNCn);
    /*p29.XYLA*/ spr.STORE4_MATCH_OUT = or(STORE4_MATCH, b.spr.STORE3_MATCH_OUT);

    /*p29.CECU*/ wire STORE4_CLK = or(b.spr.STORE_EN, STORE4_SEL); // 0100
    /*p29.DUKE*/ spr.DUKE = not(STORE4_CLK);
    
    // i think these are swapped
    /*p29.WUNU*/ spr.WUNU = not(b.spr.WOHU);
    /*p29.WOFO*/ spr.WOFO = not(b.spr.DUKE);
    /*p31.WEDU*/ spr.WEDU = tock_pos(a.spr.WUNU, b.spr.WUNU, b.spr.WOFO, b.spr.WEDU, b.spr.OAM_A_D0b);
    /*p31.YGAJ*/ spr.YGAJ = tock_pos(a.spr.WUNU, b.spr.WUNU, b.spr.WOFO, b.spr.YGAJ, b.spr.OAM_A_D1b);
    /*p31.ZYJO*/ spr.ZYJO = tock_pos(a.spr.WUNU, b.spr.WUNU, b.spr.WOFO, b.spr.ZYJO, b.spr.OAM_A_D2b);
    /*p31.XURY*/ spr.XURY = tock_pos(a.spr.WUNU, b.spr.WUNU, b.spr.WOFO, b.spr.XURY, b.spr.OAM_A_D3b);
    /*p31.YBED*/ spr.YBED = tock_pos(a.spr.WUNU, b.spr.WUNU, b.spr.WOFO, b.spr.YBED, b.spr.OAM_A_D4b);
    /*p31.ZALA*/ spr.ZALA = tock_pos(a.spr.WUNU, b.spr.WUNU, b.spr.WOFO, b.spr.ZALA, b.spr.OAM_A_D5b);
    /*p31.WYDE*/ spr.WYDE = tock_pos(a.spr.WUNU, b.spr.WUNU, b.spr.WOFO, b.spr.WYDE, b.spr.OAM_A_D6b);
    /*p31.XEPA*/ spr.XEPA = tock_pos(a.spr.WUNU, b.spr.WUNU, b.spr.WOFO, b.spr.XEPA, b.spr.OAM_A_D7b);

    /*p29.WYLU*/ spr.WYLU = not(b.spr.DUKE);
    /*p30.XYNU*/ spr.XYNU = tock_pos(a.spr.WYLU, b.spr.WYLU, 0, b.spr.XYNU, b.spr.TS_IDX_0 );
    /*p30.XEGE*/ spr.XEGE = tock_pos(a.spr.WYLU, b.spr.WYLU, 0, b.spr.XEGE, b.spr.TS_IDX_1 );
    /*p30.XABO*/ spr.XABO = tock_pos(a.spr.WYLU, b.spr.WYLU, 0, b.spr.XABO, b.spr.TS_IDX_2 );
    /*p30.WANU*/ spr.WANU = tock_pos(a.spr.WYLU, b.spr.WYLU, 0, b.spr.WANU, b.spr.TS_IDX_3 );
    /*p30.XEFE*/ spr.XEFE = tock_pos(a.spr.WYLU, b.spr.WYLU, 0, b.spr.XEFE, b.spr.TS_IDX_4 );
    /*p30.XAVE*/ spr.XAVE = tock_pos(a.spr.WYLU, b.spr.WYLU, 0, b.spr.XAVE, b.spr.TS_IDX_5 );

    /*p29.EWOT*/ spr.EWOT = not(b.spr.DUKE);
    /*p30.CUMU*/ spr.CUMU = tock_pos(a.spr.EWOT, b.spr.EWOT, 0, b.spr.CUMU, b.spr.TS_LINE_0);
    /*p30.CAPO*/ spr.CAPO = tock_pos(a.spr.EWOT, b.spr.EWOT, 0, b.spr.CAPO, b.spr.TS_LINE_1);
    /*p30.CONO*/ spr.CONO = tock_pos(a.spr.EWOT, b.spr.EWOT, 0, b.spr.CONO, b.spr.TS_LINE_2);
    /*p30.CAJU*/ spr.CAJU = tock_pos(a.spr.EWOT, b.spr.EWOT, 0, b.spr.CAJU, b.spr.TS_LINE_3);

    /*p29.GONO*/ spr.GONO = nor(b.spr.STORE4_MATCHn, b.spr.STORE3_MATCH_OUT);

    /*p29.GOWO*/ spr.GOWO = not(b.spr.GONO);
    /*p30.WAJA*/ if (b.spr.GOWO) spr.TS_IDX_0  = not(!b.spr.XYNU);
    /*p30.WOXY*/ if (b.spr.GOWO) spr.TS_IDX_1  = not(!b.spr.XEGE);
    /*p30.XYRE*/ if (b.spr.GOWO) spr.TS_IDX_2  = not(!b.spr.XABO);
    /*p30.WERU*/ if (b.spr.GOWO) spr.TS_IDX_3  = not(!b.spr.WANU);
    /*p30.WEPY*/ if (b.spr.GOWO) spr.TS_IDX_4  = not(!b.spr.XEFE);
    /*p30.WUXU*/ if (b.spr.GOWO) spr.TS_IDX_5  = not(!b.spr.XAVE);
    /*p30.BYDO*/ if (b.spr.GOWO) spr.TS_LINE_0 = not(!b.spr.CUMU);
    /*p30.BUCE*/ if (b.spr.GOWO) spr.TS_LINE_1 = not(!b.spr.CAPO);
    /*p30.BOVE*/ if (b.spr.GOWO) spr.TS_LINE_2 = not(!b.spr.CONO);
    /*p30.BEVY*/ if (b.spr.GOWO) spr.TS_LINE_3 = not(!b.spr.CAJU);
  }

  {
    // store 5
    /*p31.BAZY*/ wire BAZY = xor(b.spr.CYWE, b.vid.X4n);
    /*p31.CYLE*/ wire CYLE = xor(b.spr.DYBY, b.vid.X5n);
    /*p31.CEVA*/ wire CEVA = xor(b.spr.DURY, b.vid.X6n);
    /*p31.BUMY*/ wire BUMY = xor(b.spr.CUVY, b.vid.X7n);
    /*p31.GUZO*/ wire GUZO = xor(b.spr.FUSA, b.vid.X0n);
    /*p31.GOLA*/ wire GOLA = xor(b.spr.FAXA, b.vid.X1n);
    /*p31.GEVE*/ wire GEVE = xor(b.spr.FOZY, b.vid.X2n);
    /*p31.GUDE*/ wire GUDE = xor(b.spr.FESY, b.vid.X3n);
    /*p31.COGY*/ wire COGY = nor(BAZY, CYLE, CEVA, BUMY);
    /*p31.FYMA*/ wire FYMA = nor(GUZO, GOLA, GEVE, GUDE);
    /*p29.EGOM*/ spr.STORE5_MATCHn = nand(b.spr.MATCH_EN, FYMA, COGY);

    /*p29.GEGA*/ spr.GEGA = nor(b.spr.STORE5_MATCHn, b.spr.STORE4_MATCH_OUT);
    /*p29.WAFY*/ spr.SPRITE5_GET_SYNCn = tock_pos(a.spr.SPRITE_DONE, b.spr.SPRITE_DONE, b.lcd.VID_LINE_TRIG_d4p, b.spr.SPRITE5_GET_SYNCn, b.spr.GEGA);
    /*p29.FEVE*/ spr.FEVE = or(b.lcd.VID_LINE_TRIG_d4c, b.spr.SPRITE5_GET_SYNCn);
    /*p29.EJAD*/ spr.EJAD = not(b.spr.FEVE);
    /*p29.GABA*/ wire STORE5_MATCH = not(b.spr.STORE5_MATCHn);
    
    /*p29.WEJA*/ spr.STORE5_MATCH_OUT = or(STORE5_MATCH, b.spr.STORE4_MATCH_OUT);

    /*p29.CADO*/ wire STORE5_CLK = or(b.spr.STORE_EN, STORE5_SEL); // 0101
    /*p29.BEDE*/ spr.BEDE = not(STORE5_CLK);

    /*p29.CYLA*/ spr.CYLA = not(b.spr.BEDE);
    /*p31.FUSA*/ spr.FUSA = tock_pos(a.spr.CYLA, b.spr.CYLA, b.spr.EJAD, b.spr.FUSA, b.spr.OAM_A_D0b);
    /*p31.FAXA*/ spr.FAXA = tock_pos(a.spr.CYLA, b.spr.CYLA, b.spr.EJAD, b.spr.FAXA, b.spr.OAM_A_D1b);
    /*p31.FOZY*/ spr.FOZY = tock_pos(a.spr.CYLA, b.spr.CYLA, b.spr.EJAD, b.spr.FOZY, b.spr.OAM_A_D2b);
    /*p31.FESY*/ spr.FESY = tock_pos(a.spr.CYLA, b.spr.CYLA, b.spr.EJAD, b.spr.FESY, b.spr.OAM_A_D3b);
    /*p31.CYWE*/ spr.CYWE = tock_pos(a.spr.CYLA, b.spr.CYLA, b.spr.EJAD, b.spr.CYWE, b.spr.OAM_A_D4b);
    /*p31.DYBY*/ spr.DYBY = tock_pos(a.spr.CYLA, b.spr.CYLA, b.spr.EJAD, b.spr.DYBY, b.spr.OAM_A_D5b);
    /*p31.DURY*/ spr.DURY = tock_pos(a.spr.CYLA, b.spr.CYLA, b.spr.EJAD, b.spr.DURY, b.spr.OAM_A_D6b);
    /*p31.CUVY*/ spr.CUVY = tock_pos(a.spr.CYLA, b.spr.CYLA, b.spr.EJAD, b.spr.CUVY, b.spr.OAM_A_D7b);

    /*p29.DYMO*/ spr.DYMO = not(b.spr.BEDE);
    /*p30.EKAP*/ spr.EKAP = tock_pos(a.spr.DYMO, b.spr.DYMO, 0, b.spr.EKAP, b.spr.TS_IDX_0 );
    /*p30.ETAV*/ spr.ETAV = tock_pos(a.spr.DYMO, b.spr.DYMO, 0, b.spr.ETAV, b.spr.TS_IDX_1 );
    /*p30.EBEX*/ spr.EBEX = tock_pos(a.spr.DYMO, b.spr.DYMO, 0, b.spr.EBEX, b.spr.TS_IDX_2 );
    /*p30.GORU*/ spr.GORU = tock_pos(a.spr.DYMO, b.spr.DYMO, 0, b.spr.GORU, b.spr.TS_IDX_3 );
    /*p30.ETYM*/ spr.ETYM = tock_pos(a.spr.DYMO, b.spr.DYMO, 0, b.spr.ETYM, b.spr.TS_IDX_4 );
    /*p30.EKOP*/ spr.EKOP = tock_pos(a.spr.DYMO, b.spr.DYMO, 0, b.spr.EKOP, b.spr.TS_IDX_5 );

    /*p29.BUCY*/ spr.BUCY = not(b.spr.BEDE);
    /*p30.ANED*/ spr.ANED = tock_pos(a.spr.BUCY, b.spr.BUCY, 0, b.spr.ANED, b.spr.TS_LINE_0);
    /*p30.ACEP*/ spr.ACEP = tock_pos(a.spr.BUCY, b.spr.BUCY, 0, b.spr.ACEP, b.spr.TS_LINE_1);
    /*p30.ABUX*/ spr.ABUX = tock_pos(a.spr.BUCY, b.spr.BUCY, 0, b.spr.ABUX, b.spr.TS_LINE_2);
    /*p30.ABEG*/ spr.ABEG = tock_pos(a.spr.BUCY, b.spr.BUCY, 0, b.spr.ABEG, b.spr.TS_LINE_3);

    /*p29.GYGY*/ spr.GYGY = not(b.spr.GEGA);
    /*p30.DALO*/ if (b.spr.GYGY) spr.TS_IDX_0  = not(!b.spr.EKAP);
    /*p30.DALY*/ if (b.spr.GYGY) spr.TS_IDX_1  = not(!b.spr.ETAV);
    /*p30.DUZA*/ if (b.spr.GYGY) spr.TS_IDX_2  = not(!b.spr.EBEX);
    /*p30.WAGA*/ if (b.spr.GYGY) spr.TS_IDX_3  = not(!b.spr.GORU);
    /*p30.DYNY*/ if (b.spr.GYGY) spr.TS_IDX_4  = not(!b.spr.ETYM);
    /*p30.DOBO*/ if (b.spr.GYGY) spr.TS_IDX_5  = not(!b.spr.EKOP);
    /*p30.AWAT*/ if (b.spr.GYGY) spr.TS_LINE_0 = not(!b.spr.ANED);
    /*p30.BACE*/ if (b.spr.GYGY) spr.TS_LINE_1 = not(!b.spr.ACEP);
    /*p30.BODU*/ if (b.spr.GYGY) spr.TS_LINE_2 = not(!b.spr.ABUX);
    /*p30.BUJA*/ if (b.spr.GYGY) spr.TS_LINE_3 = not(!b.spr.ABEG);

  }

  {
    // store 6
    /*p31.ZARE*/ wire ZARE = xor(b.spr.ZOLY, b.vid.X4n);
    /*p31.ZEMU*/ wire ZEMU = xor(b.spr.ZOGO, b.vid.X5n);
    /*p31.ZYGO*/ wire ZYGO = xor(b.spr.ZECU, b.vid.X6n);
    /*p31.ZUZY*/ wire ZUZY = xor(b.spr.ZESA, b.vid.X7n);
    /*p31.XOSU*/ wire XOSU = xor(b.spr.YCOL, b.vid.X0n);
    /*p31.ZUVU*/ wire ZUVU = xor(b.spr.YRAC, b.vid.X1n);
    /*p31.XUCO*/ wire XUCO = xor(b.spr.YMEM, b.vid.X2n);
    /*p31.ZULO*/ wire ZULO = xor(b.spr.YVAG, b.vid.X3n);
    /*p31.YWAP*/ wire YWAP = nor(ZARE, ZEMU, ZYGO, ZUZY);
    /*p31.YDOT*/ wire YDOT = nor(XOSU, ZUVU, XUCO, ZULO);
    /*p29.YBEZ*/ spr.STORE6_MATCHn = nand(b.spr.MATCH_EN, YDOT, YWAP);

    /*p29.XOJA*/ spr.XOJA = nor(b.spr.STORE6_MATCHn, b.spr.STORE5_MATCH_OUT);
    /*p29.WOMY*/ spr.SPRITE6_GET_SYNCn = tock_pos(a.spr.SPRITE_DONE, b.spr.SPRITE_DONE, b.lcd.VID_LINE_TRIG_d4p, b.spr.SPRITE6_GET_SYNCn, b.spr.XOJA);
    /*p29.WACY*/ spr.WACY = or(b.lcd.VID_LINE_TRIG_d4c, b.spr.SPRITE6_GET_SYNCn);
    /*p29.XAHO*/ spr.XAHO = not(b.spr.WACY);
    /*p29.WASE*/ wire STORE6_MATCH = not(b.spr.STORE6_MATCHn);
    /*p29.WYLA*/ spr.STORE6_MATCH_OUT = or(STORE6_MATCH, b.spr.STORE5_MATCH_OUT);

    /*p29.XUJO*/ wire STORE6_CLK = or(b.spr.STORE_EN, STORE6_SEL); // 0110
    /*p29.WEKA*/ spr.WEKA = not(STORE6_CLK);

    /*p29.ZAPE*/ spr.ZAPE = not(b.spr.WEKA);
    /*p31.YCOL*/ spr.YCOL = tock_pos(a.spr.ZAPE, b.spr.ZAPE, b.spr.XAHO, b.spr.YCOL, b.spr.OAM_A_D0b);
    /*p31.YRAC*/ spr.YRAC = tock_pos(a.spr.ZAPE, b.spr.ZAPE, b.spr.XAHO, b.spr.YRAC, b.spr.OAM_A_D1b);
    /*p31.YMEM*/ spr.YMEM = tock_pos(a.spr.ZAPE, b.spr.ZAPE, b.spr.XAHO, b.spr.YMEM, b.spr.OAM_A_D2b);
    /*p31.YVAG*/ spr.YVAG = tock_pos(a.spr.ZAPE, b.spr.ZAPE, b.spr.XAHO, b.spr.YVAG, b.spr.OAM_A_D3b);
    /*p31.ZOLY*/ spr.ZOLY = tock_pos(a.spr.ZAPE, b.spr.ZAPE, b.spr.XAHO, b.spr.ZOLY, b.spr.OAM_A_D4b);
    /*p31.ZOGO*/ spr.ZOGO = tock_pos(a.spr.ZAPE, b.spr.ZAPE, b.spr.XAHO, b.spr.ZOGO, b.spr.OAM_A_D5b);
    /*p31.ZECU*/ spr.ZECU = tock_pos(a.spr.ZAPE, b.spr.ZAPE, b.spr.XAHO, b.spr.ZECU, b.spr.OAM_A_D6b);
    /*p31.ZESA*/ spr.ZESA = tock_pos(a.spr.ZAPE, b.spr.ZAPE, b.spr.XAHO, b.spr.ZESA, b.spr.OAM_A_D7b);

    /*p29.WUSE*/ spr.WUSE = not(b.spr.WEKA);
    /*p30.GECU*/ spr.GECU = tock_pos(a.spr.WUSE, b.spr.WUSE, 0, b.spr.GECU, b.spr.TS_IDX_0 );
    /*p30.FOXY*/ spr.FOXY = tock_pos(a.spr.WUSE, b.spr.WUSE, 0, b.spr.FOXY, b.spr.TS_IDX_1 );
    /*p30.GOHU*/ spr.GOHU = tock_pos(a.spr.WUSE, b.spr.WUSE, 0, b.spr.GOHU, b.spr.TS_IDX_2 );
    /*p30.FOGO*/ spr.FOGO = tock_pos(a.spr.WUSE, b.spr.WUSE, 0, b.spr.FOGO, b.spr.TS_IDX_3 );
    /*p30.GACY*/ spr.GACY = tock_pos(a.spr.WUSE, b.spr.WUSE, 0, b.spr.GACY, b.spr.TS_IDX_4 );
    /*p30.GABO*/ spr.GABO = tock_pos(a.spr.WUSE, b.spr.WUSE, 0, b.spr.GABO, b.spr.TS_IDX_5 );
    
    /*p29.ZURU*/ spr.ZURU = not(b.spr.WEKA);
    /*p30.ZUBE*/ spr.ZUBE = tock_pos(a.spr.ZURU, b.spr.ZURU, 0, b.spr.ZUBE, b.spr.TS_LINE_0);
    /*p30.ZUMY*/ spr.ZUMY = tock_pos(a.spr.ZURU, b.spr.ZURU, 0, b.spr.ZUMY, b.spr.TS_LINE_1);
    /*p30.ZEXO*/ spr.ZEXO = tock_pos(a.spr.ZURU, b.spr.ZURU, 0, b.spr.ZEXO, b.spr.TS_LINE_2);
    /*p30.ZAFU*/ spr.ZAFU = tock_pos(a.spr.ZURU, b.spr.ZURU, 0, b.spr.ZAFU, b.spr.TS_LINE_3);

    /*p29.XYME*/ spr.XYME = not(b.spr.XOJA);
    /*p30.WOCY*/ if (b.spr.XYME) spr.TS_IDX_0  = not(!b.spr.GECU);
    /*p30.ELYC*/ if (b.spr.XYME) spr.TS_IDX_1  = not(!b.spr.FOXY);
    /*p30.WABO*/ if (b.spr.XYME) spr.TS_IDX_2  = not(!b.spr.GOHU);
    /*p30.EZOC*/ if (b.spr.XYME) spr.TS_IDX_3  = not(!b.spr.FOGO);
    /*p30.WYWY*/ if (b.spr.XYME) spr.TS_IDX_4  = not(!b.spr.GACY);
    /*p30.WATO*/ if (b.spr.XYME) spr.TS_IDX_5  = not(!b.spr.GABO);
    /*p30.ZUDO*/ if (b.spr.XYME) spr.TS_LINE_0 = not(!b.spr.ZUBE);
    /*p30.YBUK*/ if (b.spr.XYME) spr.TS_LINE_1 = not(!b.spr.ZUMY);
    /*p30.ZYTO*/ if (b.spr.XYME) spr.TS_LINE_2 = not(!b.spr.ZEXO);
    /*p30.YKOZ*/ if (b.spr.XYME) spr.TS_LINE_3 = not(!b.spr.ZAFU);
  }

  {
    // store 7
    /*p31.DUSE*/ wire MATCH0 = xor(b.spr.STORE7_X0, b.vid.X0n);
    /*p31.DAGU*/ wire MATCH1 = xor(b.spr.STORE7_X1, b.vid.X1n);
    /*p31.DYZE*/ wire MATCH2 = xor(b.spr.STORE7_X2, b.vid.X2n);
    /*p31.DESO*/ wire MATCH3 = xor(b.spr.STORE7_X3, b.vid.X3n);
    /*p31.EJOT*/ wire MATCH4 = xor(b.spr.STORE7_X4, b.vid.X4n);
    /*p31.ESAJ*/ wire MATCH5 = xor(b.spr.STORE7_X5, b.vid.X5n);
    /*p31.DUCU*/ wire MATCH6 = xor(b.spr.STORE7_X6, b.vid.X6n);
    /*p31.EWUD*/ wire MATCH7 = xor(b.spr.STORE7_X7, b.vid.X7n);
    /*p31.CYCO*/ wire MATCHA = nor(MATCH0, MATCH1, MATCH2, MATCH3);
    /*p31.DAJE*/ wire MATCHB = nor(MATCH4, MATCH5, MATCH6, MATCH7);
    /*p29.DYKA*/ spr.STORE7_MATCHn = nand(b.spr.MATCH_EN, MATCHA, MATCHB);
    /*p29.GYTE*/ wire STORE7_MATCH = not(b.spr.STORE7_MATCHn);
    /*p29.FAVO*/ spr.STORE7_MATCH_OUT = or(STORE7_MATCH, b.spr.STORE6_MATCH_OUT);

    /*p29.GUTU*/ wire STORE7_RD = nor(b.spr.STORE7_MATCHn, b.spr.STORE6_MATCH_OUT);
    /*p29.WAPO*/ spr.SPRITE7_GET_SYNCn = tock_pos(a.spr.SPRITE_DONE, b.spr.SPRITE_DONE, b.lcd.VID_LINE_TRIG_d4p, b.spr.SPRITE7_GET_SYNCn, STORE7_RD);
    /*p29.GUKY*/ wire STORE7_RST = or(b.lcd.VID_LINE_TRIG_d4c, b.spr.SPRITE7_GET_SYNCn);
    /*p29.GAFY*/ wire STORE7_RSTn = not(STORE7_RST);

    /*p29.GAPE*/ wire STORE7_CLK = or(b.spr.STORE_EN, STORE7_SEL); // 0111
    /*p29.GYVO*/ wire STORE7_CLKn = not(STORE7_CLK);

    /*p29.GECY*/ spr.GECY = not(STORE7_CLKn);
    /*p31.ERAZ*/ spr.STORE7_X0 = tock_pos(a.spr.GECY, b.spr.GECY, STORE7_RSTn, b.spr.STORE7_X0, b.spr.OAM_A_D0b);
    /*p31.EPUM*/ spr.STORE7_X1 = tock_pos(a.spr.GECY, b.spr.GECY, STORE7_RSTn, b.spr.STORE7_X1, b.spr.OAM_A_D1b);
    /*p31.EROL*/ spr.STORE7_X2 = tock_pos(a.spr.GECY, b.spr.GECY, STORE7_RSTn, b.spr.STORE7_X2, b.spr.OAM_A_D2b);
    /*p31.EHYN*/ spr.STORE7_X3 = tock_pos(a.spr.GECY, b.spr.GECY, STORE7_RSTn, b.spr.STORE7_X3, b.spr.OAM_A_D3b);
    /*p31.FAZU*/ spr.STORE7_X4 = tock_pos(a.spr.GECY, b.spr.GECY, STORE7_RSTn, b.spr.STORE7_X4, b.spr.OAM_A_D4b);
    /*p31.FAXE*/ spr.STORE7_X5 = tock_pos(a.spr.GECY, b.spr.GECY, STORE7_RSTn, b.spr.STORE7_X5, b.spr.OAM_A_D5b);
    /*p31.EXUK*/ spr.STORE7_X6 = tock_pos(a.spr.GECY, b.spr.GECY, STORE7_RSTn, b.spr.STORE7_X6, b.spr.OAM_A_D6b);
    /*p31.FEDE*/ spr.STORE7_X7 = tock_pos(a.spr.GECY, b.spr.GECY, STORE7_RSTn, b.spr.STORE7_X7, b.spr.OAM_A_D7b);

    /*p29.FEFO*/ spr.FEFO = not(STORE7_CLKn);
    /*p30.FUZO*/ spr.FUZO = tock_pos(a.spr.FEFO, b.spr.FEFO, 0, b.spr.FUZO, b.spr.TS_IDX_0 );
    /*p30.GESY*/ spr.GESY = tock_pos(a.spr.FEFO, b.spr.FEFO, 0, b.spr.GESY, b.spr.TS_IDX_1 );
    /*p30.FYSU*/ spr.FYSU = tock_pos(a.spr.FEFO, b.spr.FEFO, 0, b.spr.FYSU, b.spr.TS_IDX_2 );
    /*p30.FEFA*/ spr.FEFA = tock_pos(a.spr.FEFO, b.spr.FEFO, 0, b.spr.FEFA, b.spr.TS_IDX_3 );
    /*p30.GYNO*/ spr.GYNO = tock_pos(a.spr.FEFO, b.spr.FEFO, 0, b.spr.GYNO, b.spr.TS_IDX_4 );
    /*p30.GULE*/ spr.GULE = tock_pos(a.spr.FEFO, b.spr.FEFO, 0, b.spr.GULE, b.spr.TS_IDX_5 );

    /*p29.WABE*/ spr.WABE = not(STORE7_CLKn);
    /*p30.XYGO*/ spr.XYGO = tock_pos(a.spr.WABE, b.spr.WABE, 0, b.spr.XYGO, b.spr.TS_LINE_0);
    /*p30.XYNA*/ spr.XYNA = tock_pos(a.spr.WABE, b.spr.WABE, 0, b.spr.XYNA, b.spr.TS_LINE_1);
    /*p30.XAKU*/ spr.XAKU = tock_pos(a.spr.WABE, b.spr.WABE, 0, b.spr.XAKU, b.spr.TS_LINE_2);
    /*p30.YGUM*/ spr.YGUM = tock_pos(a.spr.WABE, b.spr.WABE, 0, b.spr.YGUM, b.spr.TS_LINE_3);

    /*p29.GUGY*/ wire STORE7_RDn = not(STORE7_RD);
    /*p30.EVYT*/ if (!STORE7_RDn) spr.TS_IDX_0  = not(!b.spr.FUZO);
    /*p30.WABA*/ if (!STORE7_RDn) spr.TS_IDX_1  = not(!b.spr.GESY);
    /*p30.ETAD*/ if (!STORE7_RDn) spr.TS_IDX_2  = not(!b.spr.FYSU);
    /*p30.ELEP*/ if (!STORE7_RDn) spr.TS_IDX_3  = not(!b.spr.FEFA);
    /*p30.WYGO*/ if (!STORE7_RDn) spr.TS_IDX_4  = not(!b.spr.GYNO);
    /*p30.WAKO*/ if (!STORE7_RDn) spr.TS_IDX_5  = not(!b.spr.GULE);
    /*p30.WANA*/ if (!STORE7_RDn) spr.TS_LINE_0 = not(!b.spr.XYGO);
    /*p30.WAXE*/ if (!STORE7_RDn) spr.TS_LINE_1 = not(!b.spr.XYNA);
    /*p30.WABU*/ if (!STORE7_RDn) spr.TS_LINE_2 = not(!b.spr.XAKU);
    /*p30.YPOZ*/ if (!STORE7_RDn) spr.TS_LINE_3 = not(!b.spr.YGUM);

  }


  {
    // store 8
    /*p31.DUZE*/ wire DUZE = xor(b.spr.EZUF, b.vid.X4n);
    /*p31.DAGA*/ wire DAGA = xor(b.spr.ENAD, b.vid.X5n);
    /*p31.DAWU*/ wire DAWU = xor(b.spr.EBOW, b.vid.X6n);
    /*p31.EJAW*/ wire EJAW = xor(b.spr.FYCA, b.vid.X7n);
    /*p31.GOHO*/ wire GOHO = xor(b.spr.GAVY, b.vid.X0n);
    /*p31.GASU*/ wire GASU = xor(b.spr.GYPU, b.vid.X1n);
    /*p31.GABU*/ wire GABU = xor(b.spr.GADY, b.vid.X2n);
    /*p31.GAFE*/ wire GAFE = xor(b.spr.GAZA, b.vid.X3n);
    /*p31.DAMA*/ wire DAMA = nor(DUZE, DAGA, DAWU, EJAW);
    /*p31.FEHA*/ wire FEHA = nor(GOHO, GASU, GABU, GAFE);
    /*p29.EFYL*/ spr.STORE8_MATCHn = nand(b.spr.MATCH_EN, FEHA, DAMA);
    /*p29.GEKE*/ wire STORE8_MATCH = not(b.spr.STORE8_MATCHn);
    /*p29.GYGA*/ spr.STORE8_MATCH_OUT = or(STORE8_MATCH, b.spr.STORE7_MATCH_OUT);
    /*p29.FOXA*/ spr.FOXA = nor(b.spr.STORE8_MATCHn, b.spr.STORE7_MATCH_OUT);

    /*p29.EXUQ*/ spr.SPRITE8_GET_SYNCn = tock_pos(a.spr.SPRITE_DONE, b.spr.SPRITE_DONE, b.lcd.VID_LINE_TRIG_d4p, b.spr.SPRITE8_GET_SYNCn, b.spr.FOXA);
    /*p29.GORO*/ spr.GORO = or(b.lcd.VID_LINE_TRIG_d4c, b.spr.SPRITE8_GET_SYNCn);
    /*p29.WUZO*/ spr.WUZO = not(b.spr.GORO);

    /*p29.CAHO*/ wire STORE8_CLK = or(b.spr.STORE_EN, STORE8_SEL); // 1000
    /*p29.BUKA*/ spr.BUKA = not(STORE8_CLK);
    /*p29.CEXU*/ spr.CEXU = not(b.spr.BUKA);
    /*p29.AKOL*/ spr.AKOL = not(b.spr.BUKA);
    /*p29.BYMY*/ spr.BYMY = not(b.spr.BUKA);
    /*p31.GAVY*/ spr.GAVY = tock_pos(a.spr.CEXU, b.spr.CEXU, b.spr.WUZO, b.spr.GAVY, b.spr.OAM_A_D0b);
    /*p31.GYPU*/ spr.GYPU = tock_pos(a.spr.CEXU, b.spr.CEXU, b.spr.WUZO, b.spr.GYPU, b.spr.OAM_A_D1b);
    /*p31.GADY*/ spr.GADY = tock_pos(a.spr.CEXU, b.spr.CEXU, b.spr.WUZO, b.spr.GADY, b.spr.OAM_A_D2b);
    /*p31.GAZA*/ spr.GAZA = tock_pos(a.spr.CEXU, b.spr.CEXU, b.spr.WUZO, b.spr.GAZA, b.spr.OAM_A_D3b);
    /*p31.EZUF*/ spr.EZUF = tock_pos(a.spr.CEXU, b.spr.CEXU, b.spr.WUZO, b.spr.EZUF, b.spr.OAM_A_D4b);
    /*p31.ENAD*/ spr.ENAD = tock_pos(a.spr.CEXU, b.spr.CEXU, b.spr.WUZO, b.spr.ENAD, b.spr.OAM_A_D5b);
    /*p31.EBOW*/ spr.EBOW = tock_pos(a.spr.CEXU, b.spr.CEXU, b.spr.WUZO, b.spr.EBOW, b.spr.OAM_A_D6b);
    /*p31.FYCA*/ spr.FYCA = tock_pos(a.spr.CEXU, b.spr.CEXU, b.spr.WUZO, b.spr.FYCA, b.spr.OAM_A_D7b);


    /*p30.AXUV*/ spr.AXUV = tock_pos(a.spr.AKOL, b.spr.AKOL, 0, b.spr.AXUV, b.spr.TS_IDX_0 );
    /*p30.BADA*/ spr.BADA = tock_pos(a.spr.AKOL, b.spr.AKOL, 0, b.spr.BADA, b.spr.TS_IDX_1 );
    /*p30.APEV*/ spr.APEV = tock_pos(a.spr.AKOL, b.spr.AKOL, 0, b.spr.APEV, b.spr.TS_IDX_2 );
    /*p30.BADO*/ spr.BADO = tock_pos(a.spr.AKOL, b.spr.AKOL, 0, b.spr.BADO, b.spr.TS_IDX_3 );
    /*p30.BEXY*/ spr.BEXY = tock_pos(a.spr.AKOL, b.spr.AKOL, 0, b.spr.BEXY, b.spr.TS_IDX_4 );
    /*p30.BYHE*/ spr.BYHE = tock_pos(a.spr.AKOL, b.spr.AKOL, 0, b.spr.BYHE, b.spr.TS_IDX_5 );
    /*p30.AFYM*/ spr.AFYM = tock_pos(a.spr.BYMY, b.spr.BYMY, 0, b.spr.AFYM, b.spr.TS_LINE_0);
    /*p30.AZAP*/ spr.AZAP = tock_pos(a.spr.BYMY, b.spr.BYMY, 0, b.spr.AZAP, b.spr.TS_LINE_1);
    /*p30.AFUT*/ spr.AFUT = tock_pos(a.spr.BYMY, b.spr.BYMY, 0, b.spr.AFUT, b.spr.TS_LINE_2);
    /*p30.AFYX*/ spr.AFYX = tock_pos(a.spr.BYMY, b.spr.BYMY, 0, b.spr.AFYX, b.spr.TS_LINE_3);

    /*p29.DENY*/ spr.DENY = not(b.spr.FOXA);
    /*p30.ADYB*/ if (b.spr.DENY) spr.TS_IDX_0  = b.spr.AXUV;
    /*p30.APOB*/ if (b.spr.DENY) spr.TS_IDX_1  = b.spr.BADA;
    /*p30.APYV*/ if (b.spr.DENY) spr.TS_IDX_2  = b.spr.APEV;
    /*p30.AFEN*/ if (b.spr.DENY) spr.TS_IDX_3  = b.spr.BADO;
    /*p30.AKYH*/ if (b.spr.DENY) spr.TS_IDX_4  = b.spr.BEXY;
    /*p30.APOC*/ if (b.spr.DENY) spr.TS_IDX_5  = b.spr.BYHE;
    /*p30.BUJY*/ if (b.spr.DENY) spr.TS_LINE_0 = b.spr.AFYM;
    /*p30.BOSO*/ if (b.spr.DENY) spr.TS_LINE_1 = b.spr.AZAP;
    /*p30.AHAC*/ if (b.spr.DENY) spr.TS_LINE_2 = b.spr.AFUT;
    /*p30.BAZU*/ if (b.spr.DENY) spr.TS_LINE_3 = b.spr.AFYX;
  }

  {
    // store 9
    /*p31.YMAM*/ wire MATCH0 = xor(b.spr.STORE9_X0, b.vid.X0n);
    /*p31.YTYP*/ wire MATCH1 = xor(b.spr.STORE9_X1, b.vid.X1n);
    /*p31.YFOP*/ wire MATCH2 = xor(b.spr.STORE9_X2, b.vid.X2n);
    /*p31.YVAC*/ wire MATCH3 = xor(b.spr.STORE9_X3, b.vid.X3n);
    /*p31.ZYWU*/ wire MATCH4 = xor(b.spr.STORE9_X4, b.vid.X4n);
    /*p31.ZUZA*/ wire MATCH5 = xor(b.spr.STORE9_X5, b.vid.X5n);
    /*p31.ZEJO*/ wire MATCH6 = xor(b.spr.STORE9_X6, b.vid.X6n);
    /*p31.ZEDA*/ wire MATCH7 = xor(b.spr.STORE9_X7, b.vid.X7n);
    /*p31.YLEV*/ wire MATCHA = nor(MATCH0, MATCH1, MATCH2, MATCH3);
    /*p31.YTUB*/ wire MATCHB = nor(MATCH4, MATCH5, MATCH6, MATCH7);
    /*p29.YGEM*/ spr.STORE9_MATCHn = nand(b.spr.MATCH_EN, MATCHA, MATCHB);

    /*p29.CATO*/ wire STORE9_CLK = or(b.spr.STORE_EN, STORE9_SEL); // 1001
    /*p29.DECU*/ wire STORE9_CLKn = not(STORE9_CLK);

    /*p29.GUZE*/ wire SPRITE9_GETn = nor(b.spr.STORE9_MATCHn, b.spr.STORE8_MATCH_OUT);
    /*p29.FONO*/ spr.SPRITE9_GET_SYNCn = tock_pos(a.spr.SPRITE_DONE, b.spr.SPRITE_DONE, b.lcd.VID_LINE_TRIG_d4p, b.spr.SPRITE9_GET_SYNCn, SPRITE9_GETn);

    /*p29.DUBU*/ wire STORE9_RST  = or(b.lcd.VID_LINE_TRIG_d4c, b.spr.SPRITE9_GET_SYNCn);
    /*p29.DOSY*/ wire STORE9_RSTn = not(STORE9_RST);
    
    /*p29.WEME*/ spr.STORE9_CLKa = not(STORE9_CLKn);
    /*p31.XUVY*/ spr.STORE9_X0    = tock_pos(a.spr.STORE9_CLKa, b.spr.STORE9_CLKa, STORE9_RSTn, b.spr.STORE9_X0,    b.spr.OAM_A_D0b);
    /*p31.XERE*/ spr.STORE9_X1    = tock_pos(a.spr.STORE9_CLKa, b.spr.STORE9_CLKa, STORE9_RSTn, b.spr.STORE9_X1,    b.spr.OAM_A_D1b);
    /*p31.XUZO*/ spr.STORE9_X2    = tock_pos(a.spr.STORE9_CLKa, b.spr.STORE9_CLKa, STORE9_RSTn, b.spr.STORE9_X2,    b.spr.OAM_A_D2b);
    /*p31.XEXA*/ spr.STORE9_X3    = tock_pos(a.spr.STORE9_CLKa, b.spr.STORE9_CLKa, STORE9_RSTn, b.spr.STORE9_X3,    b.spr.OAM_A_D3b);
    /*p31.YPOD*/ spr.STORE9_X4    = tock_pos(a.spr.STORE9_CLKa, b.spr.STORE9_CLKa, STORE9_RSTn, b.spr.STORE9_X4,    b.spr.OAM_A_D4b);
    /*p31.YROP*/ spr.STORE9_X5    = tock_pos(a.spr.STORE9_CLKa, b.spr.STORE9_CLKa, STORE9_RSTn, b.spr.STORE9_X5,    b.spr.OAM_A_D5b);
    /*p31.YNEP*/ spr.STORE9_X6    = tock_pos(a.spr.STORE9_CLKa, b.spr.STORE9_CLKa, STORE9_RSTn, b.spr.STORE9_X6,    b.spr.OAM_A_D6b);
    /*p31.YZOF*/ spr.STORE9_X7    = tock_pos(a.spr.STORE9_CLKa, b.spr.STORE9_CLKa, STORE9_RSTn, b.spr.STORE9_X7,    b.spr.OAM_A_D7b);

    /*p29.WUFA*/ spr.STORE9_CLKb = not(STORE9_CLKn);
    /*p30.XUFO*/ spr.STORE9_IDX0  = tock_pos(a.spr.STORE9_CLKb, b.spr.STORE9_CLKb, 0,           b.spr.STORE9_IDX0,  b.spr.TS_IDX_0 );
    /*p30.XUTE*/ spr.STORE9_IDX1  = tock_pos(a.spr.STORE9_CLKb, b.spr.STORE9_CLKb, 0,           b.spr.STORE9_IDX1,  b.spr.TS_IDX_1 );
    /*p30.XOTU*/ spr.STORE9_IDX2  = tock_pos(a.spr.STORE9_CLKb, b.spr.STORE9_CLKb, 0,           b.spr.STORE9_IDX2,  b.spr.TS_IDX_2 );
    /*p30.XYFE*/ spr.STORE9_IDX3  = tock_pos(a.spr.STORE9_CLKb, b.spr.STORE9_CLKb, 0,           b.spr.STORE9_IDX3,  b.spr.TS_IDX_3 );
    /*p30.YZOR*/ spr.STORE9_IDX4  = tock_pos(a.spr.STORE9_CLKb, b.spr.STORE9_CLKb, 0,           b.spr.STORE9_IDX4,  b.spr.TS_IDX_4 );
    /*p30.YBER*/ spr.STORE9_IDX5  = tock_pos(a.spr.STORE9_CLKb, b.spr.STORE9_CLKb, 0,           b.spr.STORE9_IDX5,  b.spr.TS_IDX_5 );

    /*p29.FAKA*/ spr.STORE9_CLKc = not(STORE9_CLKn);
    /*p30.DEWU*/ spr.STORE9_LINE0 = tock_pos(a.spr.STORE9_CLKc, b.spr.STORE9_CLKc, 0,           b.spr.STORE9_LINE0, b.spr.TS_LINE_0);
    /*p30.CANA*/ spr.STORE9_LINE1 = tock_pos(a.spr.STORE9_CLKc, b.spr.STORE9_CLKc, 0,           b.spr.STORE9_LINE1, b.spr.TS_LINE_1);
    /*p30.DYSY*/ spr.STORE9_LINE2 = tock_pos(a.spr.STORE9_CLKc, b.spr.STORE9_CLKc, 0,           b.spr.STORE9_LINE2, b.spr.TS_LINE_2);
    /*p30.FOFO*/ spr.STORE9_LINE3 = tock_pos(a.spr.STORE9_CLKc, b.spr.STORE9_CLKc, 0,           b.spr.STORE9_LINE3, b.spr.TS_LINE_3);

    /*p29.FADO*/ wire SPRITE9_GET = not(SPRITE9_GETn);
    /*p30.YHAL*/ if (SPRITE9_GET) spr.TS_IDX_0  = not(!b.spr.STORE9_IDX0);
    /*p30.YRAD*/ if (SPRITE9_GET) spr.TS_IDX_1  = not(!b.spr.STORE9_IDX1);
    /*p30.XYRA*/ if (SPRITE9_GET) spr.TS_IDX_2  = not(!b.spr.STORE9_IDX2);
    /*p30.YNEV*/ if (SPRITE9_GET) spr.TS_IDX_3  = not(!b.spr.STORE9_IDX3);
    /*p30.ZOJY*/ if (SPRITE9_GET) spr.TS_IDX_4  = not(!b.spr.STORE9_IDX4);
    /*p30.ZARO*/ if (SPRITE9_GET) spr.TS_IDX_5  = not(!b.spr.STORE9_IDX5);
    /*p30.CAWO*/ if (SPRITE9_GET) spr.TS_LINE_0 = not(!b.spr.STORE9_LINE0);
    /*p30.BYME*/ if (SPRITE9_GET) spr.TS_LINE_1 = not(!b.spr.STORE9_LINE1);
    /*p30.COHO*/ if (SPRITE9_GET) spr.TS_LINE_2 = not(!b.spr.STORE9_LINE2);
    /*p30.GATE*/ if (SPRITE9_GET) spr.TS_LINE_3 = not(!b.spr.STORE9_LINE3);

  }
#endif

//-----------------------------------------------------------------------------
};
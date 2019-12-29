#include "Sch_SpriteStore.h"

#include "Sch_Video.h"
#include "Sch_Sprites.h"
#include "Sch_Clocks.h"
#include "Sch_Pins.h"
#include "Sch_Resets.h"
#include "Sch_LCD.h"
#include "Sch_OAM.h"

namespace Schematics {

//-----------------------------------------------------------------------------

void SpriteMatcher_tick(const Sprites& spr,
                        const Video& vid,

                        bool MATCH_CHAINn,

                        const SpriteMatcher& sst,
                        SpriteMatcher& next,
                        SpriteIndexLine& out) {
  /*p31.YMAM*/ wire MATCH0 = xor(sst.STORE_X0, vid.X0n);
  /*p31.YTYP*/ wire MATCH1 = xor(sst.STORE_X1, vid.X1n);
  /*p31.YFOP*/ wire MATCH2 = xor(sst.STORE_X2, vid.X2n);
  /*p31.YVAC*/ wire MATCH3 = xor(sst.STORE_X3, vid.X3n);
  /*p31.ZYWU*/ wire MATCH4 = xor(sst.STORE_X4, vid.X4n);
  /*p31.ZUZA*/ wire MATCH5 = xor(sst.STORE_X5, vid.X5n);
  /*p31.ZEJO*/ wire MATCH6 = xor(sst.STORE_X6, vid.X6n);
  /*p31.ZEDA*/ wire MATCH7 = xor(sst.STORE_X7, vid.X7n);
  /*p31.YLEV*/ wire MATCHA = nor(MATCH0, MATCH1, MATCH2, MATCH3);
  /*p31.YTUB*/ wire MATCHB = nor(MATCH4, MATCH5, MATCH6, MATCH7);
  /*p29.YGEM*/ next.MATCHn = nand(spr.MATCH_EN, MATCHA, MATCHB);

  /*p29.GUZE*/ wire SPRITE_GETn = nor(sst.MATCHn, MATCH_CHAINn);
  /*p29.FADO*/ wire SPRITE_GET = not(SPRITE_GETn);
  /*p30.YHAL*/ if (SPRITE_GET) out.TS_IDX_0  = not(!sst.STORE_IDX0);
  /*p30.YRAD*/ if (SPRITE_GET) out.TS_IDX_1  = not(!sst.STORE_IDX1);
  /*p30.XYRA*/ if (SPRITE_GET) out.TS_IDX_2  = not(!sst.STORE_IDX2);
  /*p30.YNEV*/ if (SPRITE_GET) out.TS_IDX_3  = not(!sst.STORE_IDX3);
  /*p30.ZOJY*/ if (SPRITE_GET) out.TS_IDX_4  = not(!sst.STORE_IDX4);
  /*p30.ZARO*/ if (SPRITE_GET) out.TS_IDX_5  = not(!sst.STORE_IDX5);
  /*p30.CAWO*/ if (SPRITE_GET) out.TS_LINE_0 = not(!sst.STORE_LINE0);
  /*p30.BYME*/ if (SPRITE_GET) out.TS_LINE_1 = not(!sst.STORE_LINE1);
  /*p30.COHO*/ if (SPRITE_GET) out.TS_LINE_2 = not(!sst.STORE_LINE2);
  /*p30.GATE*/ if (SPRITE_GET) out.TS_LINE_3 = not(!sst.STORE_LINE3);
}

//----------

void SpriteMatcher_tock(const LCD& lcd,
                        const Sprites& spr,
                        const SpriteIndexLine& sil,
                        const OAM& oam,

                        bool STORE_SEL,
                        bool MATCH_CHAINn,

                        const SpriteMatcher& sst,
                        SpriteMatcher& next) {

  /*p29.CATO*/ wire STORE_CLK = or(spr.STORE_EN, STORE_SEL);
  /*p29.DECU*/ wire STORE_CLKn = not(STORE_CLK);

  /*p29.GUZE*/ wire SPRITE_GETn = nor(sst.MATCHn, MATCH_CHAINn);
  /*p29.FONO*/ next.SPRITE_GET_SYNCn.tock(spr.SPRITE_DONE, lcd.VID_LINE_TRIG_d4p, SPRITE_GETn);

  /*p29.DUBU*/ wire STORE_RST  = or(lcd.VID_LINE_TRIG_d4c, sst.SPRITE_GET_SYNCn);
  /*p29.DOSY*/ wire STORE_RSTn = not(STORE_RST);
    
  /*p29.WEME*/ wire STORE_CLKa = not(STORE_CLKn);
  /*p31.XUVY*/ next.STORE_X0.tock(STORE_CLKa, STORE_RSTn, oam.OAM_A_D0b);
  /*p31.XERE*/ next.STORE_X1.tock(STORE_CLKa, STORE_RSTn, oam.OAM_A_D1b);
  /*p31.XUZO*/ next.STORE_X2.tock(STORE_CLKa, STORE_RSTn, oam.OAM_A_D2b);
  /*p31.XEXA*/ next.STORE_X3.tock(STORE_CLKa, STORE_RSTn, oam.OAM_A_D3b);
  /*p31.YPOD*/ next.STORE_X4.tock(STORE_CLKa, STORE_RSTn, oam.OAM_A_D4b);
  /*p31.YROP*/ next.STORE_X5.tock(STORE_CLKa, STORE_RSTn, oam.OAM_A_D5b);
  /*p31.YNEP*/ next.STORE_X6.tock(STORE_CLKa, STORE_RSTn, oam.OAM_A_D6b);
  /*p31.YZOF*/ next.STORE_X7.tock(STORE_CLKa, STORE_RSTn, oam.OAM_A_D7b);

  /*p29.WUFA*/ wire STORE_CLKb = not(STORE_CLKn);
  /*p30.XUFO*/ next.STORE_IDX0.tock(STORE_CLKb, 1, sil.TS_IDX_0);
  /*p30.XUTE*/ next.STORE_IDX1.tock(STORE_CLKb, 1, sil.TS_IDX_1);
  /*p30.XOTU*/ next.STORE_IDX2.tock(STORE_CLKb, 1, sil.TS_IDX_2);
  /*p30.XYFE*/ next.STORE_IDX3.tock(STORE_CLKb, 1, sil.TS_IDX_3);
  /*p30.YZOR*/ next.STORE_IDX4.tock(STORE_CLKb, 1, sil.TS_IDX_4);
  /*p30.YBER*/ next.STORE_IDX5.tock(STORE_CLKb, 1, sil.TS_IDX_5);

  /*p29.FAKA*/ wire STORE_CLKc = not(STORE_CLKn);
  /*p30.DEWU*/ next.STORE_LINE0.tock(STORE_CLKc, 1, sil.TS_LINE_0);
  /*p30.CANA*/ next.STORE_LINE1.tock(STORE_CLKc, 1, sil.TS_LINE_1);
  /*p30.DYSY*/ next.STORE_LINE2.tock(STORE_CLKc, 1, sil.TS_LINE_2);
  /*p30.FOFO*/ next.STORE_LINE3.tock(STORE_CLKc, 1, sil.TS_LINE_3);

}

//-----------------------------------------------------------------------------

void SpriteStore_tick(const SpriteStore& sst,
                      const Sprites& spr,
                      const Video& vid,

                      SpriteStore& next) {

  /*p29.FEFY*/ wire FEFY = nand(sst.store4.MATCHn, sst.store3.MATCHn, sst.store2.MATCHn, sst.store1.MATCHn, sst.store0.MATCHn);
  /*p29.FOVE*/ wire FOVE = nand(sst.store9.MATCHn, sst.store8.MATCHn, sst.store7.MATCHn, sst.store6.MATCHn, sst.store5.MATCHn);
  /*p29.FEPO*/ next.STORE_MATCH = or(FEFY, FOVE);

  SpriteMatcher_tick(spr, vid, false,             sst.store0, next.store0, next.sil);
  SpriteMatcher_tick(spr, vid, sst.store0.MATCHn, sst.store1, next.store1, next.sil);
  SpriteMatcher_tick(spr, vid, sst.store1.MATCHn, sst.store2, next.store2, next.sil);
  SpriteMatcher_tick(spr, vid, sst.store2.MATCHn, sst.store3, next.store3, next.sil);
  SpriteMatcher_tick(spr, vid, sst.store3.MATCHn, sst.store4, next.store4, next.sil);
  SpriteMatcher_tick(spr, vid, sst.store4.MATCHn, sst.store5, next.store5, next.sil);
  SpriteMatcher_tick(spr, vid, sst.store5.MATCHn, sst.store6, next.store6, next.sil);
  SpriteMatcher_tick(spr, vid, sst.store6.MATCHn, sst.store7, next.store7, next.sil);
  SpriteMatcher_tick(spr, vid, sst.store7.MATCHn, sst.store8, next.store8, next.sil);
  SpriteMatcher_tick(spr, vid, sst.store8.MATCHn, sst.store9, next.store9, next.sil);
}

//----------

void SpriteStore_tock(const SpriteStore& sst,
                      const ClockSignals1& clk,
                      const Sprites& spr,
                      const ResetSignals1& rst_sig,
                      const LCD& lcd,
                      const OAM& oam,

                      SpriteStore& next) {

  /*p29.DEZY*/ next.STORE_EN_SYNC.tock(clk.ZEME_AxCxExGx, rst_sig.VID_RESETn, spr.STORE_EN);

  /*p29.BAKY*/ wire SPRITES_FULL = and(sst.SPRITE_COUNT1, sst.SPRITE_COUNT3);
  /*p29.CAKE*/ wire SPRITE_COUNT_CLK = or(SPRITES_FULL, sst.STORE_EN_SYNC);
  /*p28.AZYB*/ wire SPRITE_COUNT_RSTn = not(lcd.VID_LINE_TRIG_d4n);
  /*p29.BESE*/ next.SPRITE_COUNT0.tock(SPRITE_COUNT_CLK,  SPRITE_COUNT_RSTn, !sst.SPRITE_COUNT0);
  /*p29.CUXY*/ next.SPRITE_COUNT1.tock(sst.SPRITE_COUNT0, SPRITE_COUNT_RSTn, !sst.SPRITE_COUNT1);
  /*p29.BEGO*/ next.SPRITE_COUNT2.tock(sst.SPRITE_COUNT1, SPRITE_COUNT_RSTn, !sst.SPRITE_COUNT2);
  /*p29.DYBE*/ next.SPRITE_COUNT3.tock(sst.SPRITE_COUNT2, SPRITE_COUNT_RSTn, !sst.SPRITE_COUNT3);

  /*p29.EDEN*/ wire SPRITE_COUNT0n = not(sst.SPRITE_COUNT0);
  /*p29.CYPY*/ wire SPRITE_COUNT1n = not(sst.SPRITE_COUNT1);
  /*p29.CAPE*/ wire SPRITE_COUNT2n = not(sst.SPRITE_COUNT2);
  /*p29.CAXU*/ wire SPRITE_COUNT3n = not(sst.SPRITE_COUNT3);

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

  SpriteMatcher_tock(lcd, spr, sst.sil, oam, STORE0_SEL, false,             sst.store0, next.store0);
  SpriteMatcher_tock(lcd, spr, sst.sil, oam, STORE1_SEL, sst.store0.MATCHn, sst.store1, next.store1);
  SpriteMatcher_tock(lcd, spr, sst.sil, oam, STORE2_SEL, sst.store1.MATCHn, sst.store2, next.store2);
  SpriteMatcher_tock(lcd, spr, sst.sil, oam, STORE3_SEL, sst.store2.MATCHn, sst.store3, next.store3);
  SpriteMatcher_tock(lcd, spr, sst.sil, oam, STORE4_SEL, sst.store3.MATCHn, sst.store4, next.store4);
  SpriteMatcher_tock(lcd, spr, sst.sil, oam, STORE5_SEL, sst.store4.MATCHn, sst.store5, next.store5);
  SpriteMatcher_tock(lcd, spr, sst.sil, oam, STORE6_SEL, sst.store5.MATCHn, sst.store6, next.store6);
  SpriteMatcher_tock(lcd, spr, sst.sil, oam, STORE7_SEL, sst.store6.MATCHn, sst.store7, next.store7);
  SpriteMatcher_tock(lcd, spr, sst.sil, oam, STORE8_SEL, sst.store7.MATCHn, sst.store8, next.store8);
  SpriteMatcher_tock(lcd, spr, sst.sil, oam, STORE9_SEL, sst.store8.MATCHn, sst.store9, next.store9);
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

    /*p29.YDUG*/ next.spr.STORE0_MATCHn = nand(b.spr.MATCH_EN, STORE0_MATCHB, STORE0_MATCHA);
    /*p29.WEFU*/ wire STORE0_MATCH = not(b.spr.STORE0_MATCHn);
    /*p29.GEZE*/ next.spr.STORE0_MATCH_OUT = or (STORE0_MATCH, pins.P10_B);

    /*p29.GUVA*/ wire SPRITE0_GET = nor(b.spr.STORE0_MATCHn,  pins.P10_B);
    /*p29.EBOJ*/ next.spr.SPRITE0_GET_SYNCn = tock_pos(a.spr.SPRITE_DONE, b.spr.SPRITE_DONE, b.lcd.VID_LINE_TRIG_d4p, b.spr.SPRITE0_GET_SYNCn, SPRITE0_GET);

    /*p29.CEMY*/ wire STORE0_CLK    = or(b.spr.STORE_EN, STORE0_SEL);
    /*p29.DYHU*/ wire STORE0_CLKn   = not(STORE0_CLK);
    /*p29.DYWE*/ wire STORE0_RST    = or(b.lcd.VID_LINE_TRIG_d4c, b.spr.SPRITE0_GET_SYNCn);
    /*p29.DYNA*/ wire STORE0_RSTn   = not(STORE0_RST);

    /*p29.GENY*/ next.spr.STORE0_CLKa  = not(STORE0_CLKn);
    /*p30.YGUS*/ next.spr.STORE0_IDX0  = tock_pos(a.spr.STORE0_CLKa, b.spr.STORE0_CLKa, 0,           b.spr.STORE0_IDX0,  b.spr.TS_IDX_0);
    /*p30.YSOK*/ next.spr.STORE0_IDX1  = tock_pos(a.spr.STORE0_CLKa, b.spr.STORE0_CLKa, 0,           b.spr.STORE0_IDX1,  b.spr.TS_IDX_1);
    /*p30.YZEP*/ next.spr.STORE0_IDX2  = tock_pos(a.spr.STORE0_CLKa, b.spr.STORE0_CLKa, 0,           b.spr.STORE0_IDX2,  b.spr.TS_IDX_2);
    /*p30.WYTE*/ next.spr.STORE0_IDX3  = tock_pos(a.spr.STORE0_CLKa, b.spr.STORE0_CLKa, 0,           b.spr.STORE0_IDX3,  b.spr.TS_IDX_3);
    /*p30.ZONY*/ next.spr.STORE0_IDX4  = tock_pos(a.spr.STORE0_CLKa, b.spr.STORE0_CLKa, 0,           b.spr.STORE0_IDX4,  b.spr.TS_IDX_4);
    /*p30.YWAK*/ next.spr.STORE0_IDX5  = tock_pos(a.spr.STORE0_CLKa, b.spr.STORE0_CLKa, 0,           b.spr.STORE0_IDX5,  b.spr.TS_IDX_5);
    
    /*p29.ENOB*/ next.spr.STORE0_CLKb  = not(STORE0_CLKn);
    /*p30.FYHY*/ next.spr.STORE0_LINE0 = tock_pos(a.spr.STORE0_CLKb, b.spr.STORE0_CLKb, 0,           b.spr.STORE0_LINE0, b.spr.TS_LINE_0);
    /*p30.GYHO*/ next.spr.STORE0_LINE1 = tock_pos(a.spr.STORE0_CLKb, b.spr.STORE0_CLKb, 0,           b.spr.STORE0_LINE1, b.spr.TS_LINE_1);
    /*p30.BOZU*/ next.spr.STORE0_LINE2 = tock_pos(a.spr.STORE0_CLKb, b.spr.STORE0_CLKb, 0,           b.spr.STORE0_LINE2, b.spr.TS_LINE_2);
    /*p30.CUFO*/ next.spr.STORE0_LINE3 = tock_pos(a.spr.STORE0_CLKb, b.spr.STORE0_CLKb, 0,           b.spr.STORE0_LINE3, b.spr.TS_LINE_3);

    /*p29.FUXU*/ next.spr.STORE0_CLKc  = not(STORE0_CLKn);
    /*p31.XEPE*/ next.spr.STORE0_X0    = tock_pos(a.spr.STORE0_CLKc, b.spr.STORE0_CLKc, STORE0_RSTn, b.spr.STORE0_X0,    b.spr.OAM_A_D0b);
    /*p31.YLAH*/ next.spr.STORE0_X1    = tock_pos(a.spr.STORE0_CLKc, b.spr.STORE0_CLKc, STORE0_RSTn, b.spr.STORE0_X1,    b.spr.OAM_A_D1b);
    /*p31.ZOLA*/ next.spr.STORE0_X2    = tock_pos(a.spr.STORE0_CLKc, b.spr.STORE0_CLKc, STORE0_RSTn, b.spr.STORE0_X2,    b.spr.OAM_A_D2b);
    /*p31.ZULU*/ next.spr.STORE0_X3    = tock_pos(a.spr.STORE0_CLKc, b.spr.STORE0_CLKc, STORE0_RSTn, b.spr.STORE0_X3,    b.spr.OAM_A_D3b);
    /*p31.WELO*/ next.spr.STORE0_X4    = tock_pos(a.spr.STORE0_CLKc, b.spr.STORE0_CLKc, STORE0_RSTn, b.spr.STORE0_X4,    b.spr.OAM_A_D4b);
    /*p31.XUNY*/ next.spr.STORE0_X5    = tock_pos(a.spr.STORE0_CLKc, b.spr.STORE0_CLKc, STORE0_RSTn, b.spr.STORE0_X5,    b.spr.OAM_A_D5b);
    /*p31.WOTE*/ next.spr.STORE0_X6    = tock_pos(a.spr.STORE0_CLKc, b.spr.STORE0_CLKc, STORE0_RSTn, b.spr.STORE0_X6,    b.spr.OAM_A_D6b);
    /*p31.XAKO*/ next.spr.STORE0_X7    = tock_pos(a.spr.STORE0_CLKc, b.spr.STORE0_CLKc, STORE0_RSTn, b.spr.STORE0_X7,    b.spr.OAM_A_D7b);

    /*p29.FURO*/ wire SPRITE0_GETn = not(SPRITE0_GET);

    // small tri - input must be inverting
    /*p30.ZETU*/ if (!SPRITE0_GETn) next.spr.TS_IDX_0  = b.spr.STORE0_IDX0;
    /*p30.ZECE*/ if (!SPRITE0_GETn) next.spr.TS_IDX_1  = b.spr.STORE0_IDX1;
    /*p30.ZAVE*/ if (!SPRITE0_GETn) next.spr.TS_IDX_2  = b.spr.STORE0_IDX2;
    /*p30.WOKO*/ if (!SPRITE0_GETn) next.spr.TS_IDX_3  = b.spr.STORE0_IDX3;
    /*p30.ZUMU*/ if (!SPRITE0_GETn) next.spr.TS_IDX_4  = b.spr.STORE0_IDX4;
    /*p30.ZEDY*/ if (!SPRITE0_GETn) next.spr.TS_IDX_5  = b.spr.STORE0_IDX5;
    /*p30.GOFO*/ if (!SPRITE0_GETn) next.spr.TS_LINE_0 = b.spr.STORE0_LINE0;
    /*p30.WEHE*/ if (!SPRITE0_GETn) next.spr.TS_LINE_1 = b.spr.STORE0_LINE1;
    /*p30.AJAL*/ if (!SPRITE0_GETn) next.spr.TS_LINE_2 = b.spr.STORE0_LINE2;
    /*p30.BUKY*/ if (!SPRITE0_GETn) next.spr.TS_LINE_3 = b.spr.STORE0_LINE3;
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

    /*p29.DYDU*/ next.spr.STORE1_MATCHn    = nand(b.spr.MATCH_EN, STORE1_MATCHA, STORE1_MATCHB);
    /*p29.GAJA*/ wire STORE1_MATCH = not(b.spr.STORE1_MATCHn);
    /*p29.FUMA*/ next.spr.STORE1_MATCH_OUT = or(STORE1_MATCH, b.spr.STORE0_MATCH_OUT);

    /*p29.ENUT*/ wire SPRITE1_GETn = nor(b.spr.STORE1_MATCHn, b.spr.STORE0_MATCH_OUT);

    /*p29.BYBY*/ wire STORE1_CLK  = or(b.spr.STORE_EN, STORE1_SEL);
    /*p29.BUCO*/ wire STORE1_CLKn = not(STORE1_CLK);

    /*p29.CEDY*/ next.spr.SPRITE1_GET_SYNCn = tock_pos(a.spr.SPRITE_DONE, b.spr.SPRITE_DONE, b.lcd.VID_LINE_TRIG_d4p, b.spr.SPRITE1_GET_SYNCn, SPRITE1_GETn);
    /*p29.EFEV*/ wire STORE1_RST = or(b.lcd.VID_LINE_TRIG_d4c, b.spr.SPRITE1_GET_SYNCn);
    /*p29.DOKU*/ wire STORE1_RSTn = not(STORE1_RST);

    /*p29.BYVY*/ next.spr.STORE1_CLKc = not(STORE1_CLKn);
    /*p30.CAJY*/ next.spr.STORE1_IDX0  = tock_pos(a.spr.STORE1_CLKc, b.spr.STORE1_CLKc, 0,    b.spr.STORE1_IDX0,  b.spr.TS_IDX_0 );
    /*p30.CUZA*/ next.spr.STORE1_IDX1  = tock_pos(a.spr.STORE1_CLKc, b.spr.STORE1_CLKc, 0,    b.spr.STORE1_IDX1,  b.spr.TS_IDX_1 );
    /*p30.COMA*/ next.spr.STORE1_IDX2  = tock_pos(a.spr.STORE1_CLKc, b.spr.STORE1_CLKc, 0,    b.spr.STORE1_IDX2,  b.spr.TS_IDX_2 );
    /*p30.CUFA*/ next.spr.STORE1_IDX3  = tock_pos(a.spr.STORE1_CLKc, b.spr.STORE1_CLKc, 0,    b.spr.STORE1_IDX3,  b.spr.TS_IDX_3 );
    /*p30.CEBO*/ next.spr.STORE1_IDX4  = tock_pos(a.spr.STORE1_CLKc, b.spr.STORE1_CLKc, 0,    b.spr.STORE1_IDX4,  b.spr.TS_IDX_4 );
    /*p30.CADU*/ next.spr.STORE1_IDX5  = tock_pos(a.spr.STORE1_CLKc, b.spr.STORE1_CLKc, 0,    b.spr.STORE1_IDX5,  b.spr.TS_IDX_5 );
                                                                                           
    /*p29.AHOF*/ next.spr.STORE1_CLKb = not(STORE1_CLKn);
    /*p30.ABUG*/ next.spr.STORE1_LINE0 = tock_pos(a.spr.STORE1_CLKb, b.spr.STORE1_CLKb, 0,    b.spr.STORE1_LINE0, b.spr.TS_LINE_0);
    /*p30.AMES*/ next.spr.STORE1_LINE1 = tock_pos(a.spr.STORE1_CLKb, b.spr.STORE1_CLKb, 0,    b.spr.STORE1_LINE1, b.spr.TS_LINE_1);
    /*p30.ABOP*/ next.spr.STORE1_LINE2 = tock_pos(a.spr.STORE1_CLKb, b.spr.STORE1_CLKb, 0,    b.spr.STORE1_LINE2, b.spr.TS_LINE_2);
    /*p30.AROF*/ next.spr.STORE1_LINE3 = tock_pos(a.spr.STORE1_CLKb, b.spr.STORE1_CLKb, 0,    b.spr.STORE1_LINE3, b.spr.TS_LINE_3);

    /*p29.ASYS*/ next.spr.STORE1_CLKa = not(STORE1_CLKn);
    /*p31.DANY*/ next.spr.STORE1_X0    = tock_pos(a.spr.STORE1_CLKa, b.spr.STORE1_CLKa, STORE1_RSTn, b.spr.STORE1_X0,    b.spr.OAM_A_D0b);
    /*p31.DUKO*/ next.spr.STORE1_X1    = tock_pos(a.spr.STORE1_CLKa, b.spr.STORE1_CLKa, STORE1_RSTn, b.spr.STORE1_X1,    b.spr.OAM_A_D1b);
    /*p31.DESU*/ next.spr.STORE1_X2    = tock_pos(a.spr.STORE1_CLKa, b.spr.STORE1_CLKa, STORE1_RSTn, b.spr.STORE1_X2,    b.spr.OAM_A_D2b);
    /*p31.DAZO*/ next.spr.STORE1_X3    = tock_pos(a.spr.STORE1_CLKa, b.spr.STORE1_CLKa, STORE1_RSTn, b.spr.STORE1_X3,    b.spr.OAM_A_D3b);
    /*p31.DAKE*/ next.spr.STORE1_X4    = tock_pos(a.spr.STORE1_CLKa, b.spr.STORE1_CLKa, STORE1_RSTn, b.spr.STORE1_X4,    b.spr.OAM_A_D4b);
    /*p31.CESO*/ next.spr.STORE1_X5    = tock_pos(a.spr.STORE1_CLKa, b.spr.STORE1_CLKa, STORE1_RSTn, b.spr.STORE1_X5,    b.spr.OAM_A_D5b);
    /*p31.DYFU*/ next.spr.STORE1_X6    = tock_pos(a.spr.STORE1_CLKa, b.spr.STORE1_CLKa, STORE1_RSTn, b.spr.STORE1_X6,    b.spr.OAM_A_D6b);
    /*p31.CUSY*/ next.spr.STORE1_X7    = tock_pos(a.spr.STORE1_CLKa, b.spr.STORE1_CLKa, STORE1_RSTn, b.spr.STORE1_X7,    b.spr.OAM_A_D7b);

    /*p29.DYDO*/ wire SPRITE1_GET = not(SPRITE1_GETn);
    /*p30.BEMO*/ if (SPRITE1_GET) next.spr.TS_IDX_0  = not(!b.spr.STORE1_IDX0);
    /*p30.CYBY*/ if (SPRITE1_GET) next.spr.TS_IDX_1  = not(!b.spr.STORE1_IDX1);
    /*p30.BETY*/ if (SPRITE1_GET) next.spr.TS_IDX_2  = not(!b.spr.STORE1_IDX2);
    /*p30.CEGY*/ if (SPRITE1_GET) next.spr.TS_IDX_3  = not(!b.spr.STORE1_IDX3);
    /*p30.CELU*/ if (SPRITE1_GET) next.spr.TS_IDX_4  = not(!b.spr.STORE1_IDX4);
    /*p30.CUBO*/ if (SPRITE1_GET) next.spr.TS_IDX_5  = not(!b.spr.STORE1_IDX5);
    /*p30.BEFE*/ if (SPRITE1_GET) next.spr.TS_LINE_0 = not(!b.spr.STORE1_LINE0);
    /*p30.BYRO*/ if (SPRITE1_GET) next.spr.TS_LINE_1 = not(!b.spr.STORE1_LINE1);
    /*p30.BACO*/ if (SPRITE1_GET) next.spr.TS_LINE_2 = not(!b.spr.STORE1_LINE2);
    /*p30.AHUM*/ if (SPRITE1_GET) next.spr.TS_LINE_3 = not(!b.spr.STORE1_LINE3);
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

    /*p29.DEGO*/ next.spr.STORE2_MATCHn = nand(b.spr.MATCH_EN, STORE2_MATCHB, STORE2_MATCHA);
    /*p29.GUPO*/ wire STORE2_MATCH = not(b.spr.STORE2_MATCHn);
    /*p29.GEDE*/ next.spr.STORE2_MATCH_OUT = or(STORE2_MATCH, b.spr.STORE1_MATCH_OUT);

    /*p29.EMOL*/ wire SPRITE2_GETn = nor(next.spr.STORE2_MATCHn, b.spr.STORE1_MATCH_OUT);
    /*p29.EGAV*/ next.spr.SPRITE2_GET_SYNCn = tock_pos(a.spr.SPRITE_DONE, b.spr.SPRITE_DONE, b.lcd.VID_LINE_TRIG_d4p, b.spr.SPRITE2_GET_SYNCn, SPRITE2_GETn);

    /*p29.WYXO*/ wire STORE2_CLK = or(b.spr.STORE_EN, STORE2_SEL); // 0010
    /*p29.GYFO*/ wire STORE2_CLKn = not(STORE2_CLK); // 0010
    /*p29.FOKO*/ wire STORE2_RST  = or(b.lcd.VID_LINE_TRIG_d4c, b.spr.SPRITE2_GET_SYNCn);
    /*p29.GAMY*/ wire STORE2_RSTn = not(STORE2_RST);

    /*p29.BUZY*/ next.spr.STORE2_CLKb = not(STORE2_CLKn);
    /*p30.BOXA*/ next.spr.STORE2_IDX0 = tock_pos(a.spr.STORE2_CLKb, b.spr.STORE2_CLKb, 0, b.spr.STORE2_IDX0, b.spr.TS_IDX_0 );
    /*p30.BUNA*/ next.spr.STORE2_IDX1 = tock_pos(a.spr.STORE2_CLKb, b.spr.STORE2_CLKb, 0, b.spr.STORE2_IDX1, b.spr.TS_IDX_1 );
    /*p30.BULU*/ next.spr.STORE2_IDX2 = tock_pos(a.spr.STORE2_CLKb, b.spr.STORE2_CLKb, 0, b.spr.STORE2_IDX2, b.spr.TS_IDX_2 );
    /*p30.BECA*/ next.spr.STORE2_IDX3 = tock_pos(a.spr.STORE2_CLKb, b.spr.STORE2_CLKb, 0, b.spr.STORE2_IDX3, b.spr.TS_IDX_3 );
    /*p30.BYHU*/ next.spr.STORE2_IDX4 = tock_pos(a.spr.STORE2_CLKb, b.spr.STORE2_CLKb, 0, b.spr.STORE2_IDX4, b.spr.TS_IDX_4 );
    /*p30.BUHE*/ next.spr.STORE2_IDX5 = tock_pos(a.spr.STORE2_CLKb, b.spr.STORE2_CLKb, 0, b.spr.STORE2_IDX5, b.spr.TS_IDX_5 );

    /*p29.FUKE*/ next.spr.STORE2_CLKc = not(STORE2_CLKn);
    /*p30.YKUK*/ next.spr.STORE2_LINE0 = tock_pos(a.spr.STORE2_CLKc, b.spr.STORE2_CLKc, 0, b.spr.STORE2_LINE0, b.spr.TS_LINE_0);
    /*p30.YLOV*/ next.spr.STORE2_LINE1 = tock_pos(a.spr.STORE2_CLKc, b.spr.STORE2_CLKc, 0, b.spr.STORE2_LINE1, b.spr.TS_LINE_1);
    /*p30.XAZY*/ next.spr.STORE2_LINE2 = tock_pos(a.spr.STORE2_CLKc, b.spr.STORE2_CLKc, 0, b.spr.STORE2_LINE2, b.spr.TS_LINE_2);
    /*p30.XOSY*/ next.spr.STORE2_LINE3 = tock_pos(a.spr.STORE2_CLKc, b.spr.STORE2_CLKc, 0, b.spr.STORE2_LINE3, b.spr.TS_LINE_3);

    /*p29.CACU*/ next.spr.STORE2_CLKa = not(STORE2_CLKn);
    /*p31.FOKA*/ next.spr.STORE2_X0 = tock_pos(a.spr.STORE2_CLKa, b.spr.STORE2_CLKa, STORE2_RSTn, b.spr.STORE2_X0, b.spr.OAM_A_D0b);
    /*p31.FYTY*/ next.spr.STORE2_X1 = tock_pos(a.spr.STORE2_CLKa, b.spr.STORE2_CLKa, STORE2_RSTn, b.spr.STORE2_X1, b.spr.OAM_A_D1b);
    /*p31.FUBY*/ next.spr.STORE2_X2 = tock_pos(a.spr.STORE2_CLKa, b.spr.STORE2_CLKa, STORE2_RSTn, b.spr.STORE2_X2, b.spr.OAM_A_D2b);
    /*p31.GOXU*/ next.spr.STORE2_X3 = tock_pos(a.spr.STORE2_CLKa, b.spr.STORE2_CLKa, STORE2_RSTn, b.spr.STORE2_X3, b.spr.OAM_A_D3b);
    /*p31.DUHY*/ next.spr.STORE2_X4 = tock_pos(a.spr.STORE2_CLKa, b.spr.STORE2_CLKa, STORE2_RSTn, b.spr.STORE2_X4, b.spr.OAM_A_D4b);
    /*p31.EJUF*/ next.spr.STORE2_X5 = tock_pos(a.spr.STORE2_CLKa, b.spr.STORE2_CLKa, STORE2_RSTn, b.spr.STORE2_X5, b.spr.OAM_A_D5b);
    /*p31.ENOR*/ next.spr.STORE2_X6 = tock_pos(a.spr.STORE2_CLKa, b.spr.STORE2_CLKa, STORE2_RSTn, b.spr.STORE2_X6, b.spr.OAM_A_D6b);
    /*p31.DEPY*/ next.spr.STORE2_X7 = tock_pos(a.spr.STORE2_CLKa, b.spr.STORE2_CLKa, STORE2_RSTn, b.spr.STORE2_X7, b.spr.OAM_A_D7b);


    /*p29.FAME*/ wire SPRITE2_GET = not(SPRITE2_GETn);
    /*p30.AXEC*/ if (SPRITE2_GET) next.spr.TS_IDX_0  = not(!b.spr.STORE2_IDX0);
    /*p30.CYRO*/ if (SPRITE2_GET) next.spr.TS_IDX_1  = not(!b.spr.STORE2_IDX1);
    /*p30.CUVU*/ if (SPRITE2_GET) next.spr.TS_IDX_2  = not(!b.spr.STORE2_IDX2);
    /*p30.APON*/ if (SPRITE2_GET) next.spr.TS_IDX_3  = not(!b.spr.STORE2_IDX3);
    /*p30.AFOZ*/ if (SPRITE2_GET) next.spr.TS_IDX_4  = not(!b.spr.STORE2_IDX4);
    /*p30.CUBE*/ if (SPRITE2_GET) next.spr.TS_IDX_5  = not(!b.spr.STORE2_IDX5);
    /*p30.ZABY*/ if (SPRITE2_GET) next.spr.TS_LINE_0 = not(!b.spr.STORE2_LINE0);
    /*p30.ZUKE*/ if (SPRITE2_GET) next.spr.TS_LINE_1 = not(!b.spr.STORE2_LINE1);
    /*p30.WUXE*/ if (SPRITE2_GET) next.spr.TS_LINE_2 = not(!b.spr.STORE2_LINE2);
    /*p30.WERE*/ if (SPRITE2_GET) next.spr.TS_LINE_3 = not(!b.spr.STORE2_LINE3);
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

    /*p29.YLOZ*/ next.spr.STORE3_MATCHn = nand(b.spr.MATCH_EN, STORE3_MATCHA, STORE3_MATCHB);
    /*p29.WEBO*/ wire STORE3_MATCH = not(b.spr.STORE3_MATCHn);
    /*p29.WUTO*/ next.spr.STORE3_MATCH_OUT = or(STORE3_MATCH, b.spr.STORE2_MATCH_OUT);

    /*p29.GYFY*/ wire SPRITE3_GETn = nor(b.spr.STORE3_MATCHn, b.spr.STORE2_MATCH_OUT);
    /*p29.GOTA*/ next.spr.SPRITE3_GET_SYNCn = tock_pos(a.spr.SPRITE_DONE, b.spr.SPRITE_DONE, b.lcd.VID_LINE_TRIG_d4p, b.spr.SPRITE3_GET_SYNCn, SPRITE3_GETn);

    /*p29.GAKE*/ wire STORE3_RST = or(b.lcd.VID_LINE_TRIG_d4c, b.spr.SPRITE3_GET_SYNCn);
    /*p29.WUPA*/ wire STORE3_RSTn = not(STORE3_RST);

    /*p29.GUVE*/ wire STORE3_CLK = or(b.spr.STORE_EN, STORE3_SEL); // 0011
    /*p29.GUSA*/ wire STORE3_CLKn = not(STORE3_CLK);
    
    /*p29.FEKA*/ next.spr.FEKA = not(STORE3_CLKn);
    /*p30.DAFU*/ next.spr.DAFU = tock_pos(a.spr.FEKA, b.spr.FEKA, 0,          b.spr.DAFU, b.spr.TS_IDX_0 );
    /*p30.DEBA*/ next.spr.DEBA = tock_pos(a.spr.FEKA, b.spr.FEKA, 0,          b.spr.DEBA, b.spr.TS_IDX_1 );
    /*p30.DUHA*/ next.spr.DUHA = tock_pos(a.spr.FEKA, b.spr.FEKA, 0,          b.spr.DUHA, b.spr.TS_IDX_2 );
    /*p30.DUNY*/ next.spr.DUNY = tock_pos(a.spr.FEKA, b.spr.FEKA, 0,          b.spr.DUNY, b.spr.TS_IDX_3 );
    /*p30.DESE*/ next.spr.DESE = tock_pos(a.spr.FEKA, b.spr.FEKA, 0,          b.spr.DESE, b.spr.TS_IDX_4 );
    /*p30.DEVY*/ next.spr.DEVY = tock_pos(a.spr.FEKA, b.spr.FEKA, 0,          b.spr.DEVY, b.spr.TS_IDX_5 );

    /*p29.XYHA*/ next.spr.XYHA = not(STORE3_CLKn);
    /*p30.ZURY*/ next.spr.ZURY = tock_pos(a.spr.XYHA, b.spr.XYHA, 0,          b.spr.ZURY, b.spr.TS_LINE_0);
    /*p30.ZURO*/ next.spr.ZURO = tock_pos(a.spr.XYHA, b.spr.XYHA, 0,          b.spr.ZURO, b.spr.TS_LINE_1);
    /*p30.ZENE*/ next.spr.ZENE = tock_pos(a.spr.XYHA, b.spr.XYHA, 0,          b.spr.ZENE, b.spr.TS_LINE_2);
    /*p30.ZYLU*/ next.spr.ZYLU = tock_pos(a.spr.XYHA, b.spr.XYHA, 0,          b.spr.ZYLU, b.spr.TS_LINE_3);

    /*p29.YFAG*/ next.spr.YFAG = not(STORE3_CLKn);
    /*p31.XOLY*/ next.spr.XOLY = tock_pos(a.spr.YFAG, b.spr.YFAG, STORE3_RSTn, b.spr.XOLY, b.spr.OAM_A_D0b);
    /*p31.XYBA*/ next.spr.XYBA = tock_pos(a.spr.YFAG, b.spr.YFAG, STORE3_RSTn, b.spr.XYBA, b.spr.OAM_A_D1b);
    /*p31.XABE*/ next.spr.XABE = tock_pos(a.spr.YFAG, b.spr.YFAG, STORE3_RSTn, b.spr.XABE, b.spr.OAM_A_D2b);
    /*p31.XEKA*/ next.spr.XEKA = tock_pos(a.spr.YFAG, b.spr.YFAG, STORE3_RSTn, b.spr.XEKA, b.spr.OAM_A_D3b);
    /*p31.XOMY*/ next.spr.XOMY = tock_pos(a.spr.YFAG, b.spr.YFAG, STORE3_RSTn, b.spr.XOMY, b.spr.OAM_A_D4b);
    /*p31.WUHA*/ next.spr.WUHA = tock_pos(a.spr.YFAG, b.spr.YFAG, STORE3_RSTn, b.spr.WUHA, b.spr.OAM_A_D5b);
    /*p31.WYNA*/ next.spr.WYNA = tock_pos(a.spr.YFAG, b.spr.YFAG, STORE3_RSTn, b.spr.WYNA, b.spr.OAM_A_D6b);
    /*p31.WECO*/ next.spr.WECO = tock_pos(a.spr.YFAG, b.spr.YFAG, STORE3_RSTn, b.spr.WECO, b.spr.OAM_A_D7b);

    /*p29.GYMA*/ wire SPRITE3_GET = not(SPRITE3_GETn);
    /*p30.DEZU*/ if (SPRITE3_GET) next.spr.TS_IDX_0  = not(!b.spr.DAFU);
    /*p30.EFUD*/ if (SPRITE3_GET) next.spr.TS_IDX_1  = not(!b.spr.DEBA);
    /*p30.DONY*/ if (SPRITE3_GET) next.spr.TS_IDX_2  = not(!b.spr.DUHA);
    /*p30.DOWA*/ if (SPRITE3_GET) next.spr.TS_IDX_3  = not(!b.spr.DUNY);
    /*p30.DYGO*/ if (SPRITE3_GET) next.spr.TS_IDX_4  = not(!b.spr.DESE);
    /*p30.ENAP*/ if (SPRITE3_GET) next.spr.TS_IDX_5  = not(!b.spr.DEVY);
    /*p30.ZYPO*/ if (SPRITE3_GET) next.spr.TS_LINE_0 = not(!b.spr.ZURY);
    /*p30.ZEXE*/ if (SPRITE3_GET) next.spr.TS_LINE_1 = not(!b.spr.ZURO);
    /*p30.YJEM*/ if (SPRITE3_GET) next.spr.TS_LINE_2 = not(!b.spr.ZENE);
    /*p30.YWAV*/ if (SPRITE3_GET) next.spr.TS_LINE_3 = not(!b.spr.ZYLU);
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
    /*p29.XAGE*/ next.spr.STORE4_MATCHn = nand(b.spr.MATCH_EN, YNAZ, YKOK);
    /*p29.WUNA*/ wire STORE4_MATCH = not(b.spr.STORE4_MATCHn);

    /*p29.XUDY*/ next.spr.SPRITE4_GET_SYNCn = tock_pos(a.spr.SPRITE_DONE, b.spr.SPRITE_DONE, b.lcd.VID_LINE_TRIG_d4p, b.spr.SPRITE4_GET_SYNCn, b.spr.GONO);
    /*p29.WOHU*/ next.spr.WOHU = or(b.lcd.VID_LINE_TRIG_d4c, b.spr.SPRITE4_GET_SYNCn);
    /*p29.XYLA*/ next.spr.STORE4_MATCH_OUT = or(STORE4_MATCH, b.spr.STORE3_MATCH_OUT);

    /*p29.CECU*/ wire STORE4_CLK = or(b.spr.STORE_EN, STORE4_SEL); // 0100
    /*p29.DUKE*/ next.spr.DUKE = not(STORE4_CLK);
    
    // i think these are swapped
    /*p29.WUNU*/ next.spr.WUNU = not(b.spr.WOHU);
    /*p29.WOFO*/ next.spr.WOFO = not(b.spr.DUKE);
    /*p31.WEDU*/ next.spr.WEDU = tock_pos(a.spr.WUNU, b.spr.WUNU, b.spr.WOFO, b.spr.WEDU, b.spr.OAM_A_D0b);
    /*p31.YGAJ*/ next.spr.YGAJ = tock_pos(a.spr.WUNU, b.spr.WUNU, b.spr.WOFO, b.spr.YGAJ, b.spr.OAM_A_D1b);
    /*p31.ZYJO*/ next.spr.ZYJO = tock_pos(a.spr.WUNU, b.spr.WUNU, b.spr.WOFO, b.spr.ZYJO, b.spr.OAM_A_D2b);
    /*p31.XURY*/ next.spr.XURY = tock_pos(a.spr.WUNU, b.spr.WUNU, b.spr.WOFO, b.spr.XURY, b.spr.OAM_A_D3b);
    /*p31.YBED*/ next.spr.YBED = tock_pos(a.spr.WUNU, b.spr.WUNU, b.spr.WOFO, b.spr.YBED, b.spr.OAM_A_D4b);
    /*p31.ZALA*/ next.spr.ZALA = tock_pos(a.spr.WUNU, b.spr.WUNU, b.spr.WOFO, b.spr.ZALA, b.spr.OAM_A_D5b);
    /*p31.WYDE*/ next.spr.WYDE = tock_pos(a.spr.WUNU, b.spr.WUNU, b.spr.WOFO, b.spr.WYDE, b.spr.OAM_A_D6b);
    /*p31.XEPA*/ next.spr.XEPA = tock_pos(a.spr.WUNU, b.spr.WUNU, b.spr.WOFO, b.spr.XEPA, b.spr.OAM_A_D7b);

    /*p29.WYLU*/ next.spr.WYLU = not(b.spr.DUKE);
    /*p30.XYNU*/ next.spr.XYNU = tock_pos(a.spr.WYLU, b.spr.WYLU, 0, b.spr.XYNU, b.spr.TS_IDX_0 );
    /*p30.XEGE*/ next.spr.XEGE = tock_pos(a.spr.WYLU, b.spr.WYLU, 0, b.spr.XEGE, b.spr.TS_IDX_1 );
    /*p30.XABO*/ next.spr.XABO = tock_pos(a.spr.WYLU, b.spr.WYLU, 0, b.spr.XABO, b.spr.TS_IDX_2 );
    /*p30.WANU*/ next.spr.WANU = tock_pos(a.spr.WYLU, b.spr.WYLU, 0, b.spr.WANU, b.spr.TS_IDX_3 );
    /*p30.XEFE*/ next.spr.XEFE = tock_pos(a.spr.WYLU, b.spr.WYLU, 0, b.spr.XEFE, b.spr.TS_IDX_4 );
    /*p30.XAVE*/ next.spr.XAVE = tock_pos(a.spr.WYLU, b.spr.WYLU, 0, b.spr.XAVE, b.spr.TS_IDX_5 );

    /*p29.EWOT*/ next.spr.EWOT = not(b.spr.DUKE);
    /*p30.CUMU*/ next.spr.CUMU = tock_pos(a.spr.EWOT, b.spr.EWOT, 0, b.spr.CUMU, b.spr.TS_LINE_0);
    /*p30.CAPO*/ next.spr.CAPO = tock_pos(a.spr.EWOT, b.spr.EWOT, 0, b.spr.CAPO, b.spr.TS_LINE_1);
    /*p30.CONO*/ next.spr.CONO = tock_pos(a.spr.EWOT, b.spr.EWOT, 0, b.spr.CONO, b.spr.TS_LINE_2);
    /*p30.CAJU*/ next.spr.CAJU = tock_pos(a.spr.EWOT, b.spr.EWOT, 0, b.spr.CAJU, b.spr.TS_LINE_3);

    /*p29.GONO*/ next.spr.GONO = nor(b.spr.STORE4_MATCHn, b.spr.STORE3_MATCH_OUT);

    /*p29.GOWO*/ next.spr.GOWO = not(b.spr.GONO);
    /*p30.WAJA*/ if (b.spr.GOWO) next.spr.TS_IDX_0  = not(!b.spr.XYNU);
    /*p30.WOXY*/ if (b.spr.GOWO) next.spr.TS_IDX_1  = not(!b.spr.XEGE);
    /*p30.XYRE*/ if (b.spr.GOWO) next.spr.TS_IDX_2  = not(!b.spr.XABO);
    /*p30.WERU*/ if (b.spr.GOWO) next.spr.TS_IDX_3  = not(!b.spr.WANU);
    /*p30.WEPY*/ if (b.spr.GOWO) next.spr.TS_IDX_4  = not(!b.spr.XEFE);
    /*p30.WUXU*/ if (b.spr.GOWO) next.spr.TS_IDX_5  = not(!b.spr.XAVE);
    /*p30.BYDO*/ if (b.spr.GOWO) next.spr.TS_LINE_0 = not(!b.spr.CUMU);
    /*p30.BUCE*/ if (b.spr.GOWO) next.spr.TS_LINE_1 = not(!b.spr.CAPO);
    /*p30.BOVE*/ if (b.spr.GOWO) next.spr.TS_LINE_2 = not(!b.spr.CONO);
    /*p30.BEVY*/ if (b.spr.GOWO) next.spr.TS_LINE_3 = not(!b.spr.CAJU);
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
    /*p29.EGOM*/ next.spr.STORE5_MATCHn = nand(b.spr.MATCH_EN, FYMA, COGY);

    /*p29.GEGA*/ next.spr.GEGA = nor(b.spr.STORE5_MATCHn, b.spr.STORE4_MATCH_OUT);
    /*p29.WAFY*/ next.spr.SPRITE5_GET_SYNCn = tock_pos(a.spr.SPRITE_DONE, b.spr.SPRITE_DONE, b.lcd.VID_LINE_TRIG_d4p, b.spr.SPRITE5_GET_SYNCn, b.spr.GEGA);
    /*p29.FEVE*/ next.spr.FEVE = or(b.lcd.VID_LINE_TRIG_d4c, b.spr.SPRITE5_GET_SYNCn);
    /*p29.EJAD*/ next.spr.EJAD = not(b.spr.FEVE);
    /*p29.GABA*/ wire STORE5_MATCH = not(b.spr.STORE5_MATCHn);
    
    /*p29.WEJA*/ next.spr.STORE5_MATCH_OUT = or(STORE5_MATCH, b.spr.STORE4_MATCH_OUT);

    /*p29.CADO*/ wire STORE5_CLK = or(b.spr.STORE_EN, STORE5_SEL); // 0101
    /*p29.BEDE*/ next.spr.BEDE = not(STORE5_CLK);

    /*p29.CYLA*/ next.spr.CYLA = not(b.spr.BEDE);
    /*p31.FUSA*/ next.spr.FUSA = tock_pos(a.spr.CYLA, b.spr.CYLA, b.spr.EJAD, b.spr.FUSA, b.spr.OAM_A_D0b);
    /*p31.FAXA*/ next.spr.FAXA = tock_pos(a.spr.CYLA, b.spr.CYLA, b.spr.EJAD, b.spr.FAXA, b.spr.OAM_A_D1b);
    /*p31.FOZY*/ next.spr.FOZY = tock_pos(a.spr.CYLA, b.spr.CYLA, b.spr.EJAD, b.spr.FOZY, b.spr.OAM_A_D2b);
    /*p31.FESY*/ next.spr.FESY = tock_pos(a.spr.CYLA, b.spr.CYLA, b.spr.EJAD, b.spr.FESY, b.spr.OAM_A_D3b);
    /*p31.CYWE*/ next.spr.CYWE = tock_pos(a.spr.CYLA, b.spr.CYLA, b.spr.EJAD, b.spr.CYWE, b.spr.OAM_A_D4b);
    /*p31.DYBY*/ next.spr.DYBY = tock_pos(a.spr.CYLA, b.spr.CYLA, b.spr.EJAD, b.spr.DYBY, b.spr.OAM_A_D5b);
    /*p31.DURY*/ next.spr.DURY = tock_pos(a.spr.CYLA, b.spr.CYLA, b.spr.EJAD, b.spr.DURY, b.spr.OAM_A_D6b);
    /*p31.CUVY*/ next.spr.CUVY = tock_pos(a.spr.CYLA, b.spr.CYLA, b.spr.EJAD, b.spr.CUVY, b.spr.OAM_A_D7b);

    /*p29.DYMO*/ next.spr.DYMO = not(b.spr.BEDE);
    /*p30.EKAP*/ next.spr.EKAP = tock_pos(a.spr.DYMO, b.spr.DYMO, 0, b.spr.EKAP, b.spr.TS_IDX_0 );
    /*p30.ETAV*/ next.spr.ETAV = tock_pos(a.spr.DYMO, b.spr.DYMO, 0, b.spr.ETAV, b.spr.TS_IDX_1 );
    /*p30.EBEX*/ next.spr.EBEX = tock_pos(a.spr.DYMO, b.spr.DYMO, 0, b.spr.EBEX, b.spr.TS_IDX_2 );
    /*p30.GORU*/ next.spr.GORU = tock_pos(a.spr.DYMO, b.spr.DYMO, 0, b.spr.GORU, b.spr.TS_IDX_3 );
    /*p30.ETYM*/ next.spr.ETYM = tock_pos(a.spr.DYMO, b.spr.DYMO, 0, b.spr.ETYM, b.spr.TS_IDX_4 );
    /*p30.EKOP*/ next.spr.EKOP = tock_pos(a.spr.DYMO, b.spr.DYMO, 0, b.spr.EKOP, b.spr.TS_IDX_5 );

    /*p29.BUCY*/ next.spr.BUCY = not(b.spr.BEDE);
    /*p30.ANED*/ next.spr.ANED = tock_pos(a.spr.BUCY, b.spr.BUCY, 0, b.spr.ANED, b.spr.TS_LINE_0);
    /*p30.ACEP*/ next.spr.ACEP = tock_pos(a.spr.BUCY, b.spr.BUCY, 0, b.spr.ACEP, b.spr.TS_LINE_1);
    /*p30.ABUX*/ next.spr.ABUX = tock_pos(a.spr.BUCY, b.spr.BUCY, 0, b.spr.ABUX, b.spr.TS_LINE_2);
    /*p30.ABEG*/ next.spr.ABEG = tock_pos(a.spr.BUCY, b.spr.BUCY, 0, b.spr.ABEG, b.spr.TS_LINE_3);

    /*p29.GYGY*/ next.spr.GYGY = not(b.spr.GEGA);
    /*p30.DALO*/ if (b.spr.GYGY) next.spr.TS_IDX_0  = not(!b.spr.EKAP);
    /*p30.DALY*/ if (b.spr.GYGY) next.spr.TS_IDX_1  = not(!b.spr.ETAV);
    /*p30.DUZA*/ if (b.spr.GYGY) next.spr.TS_IDX_2  = not(!b.spr.EBEX);
    /*p30.WAGA*/ if (b.spr.GYGY) next.spr.TS_IDX_3  = not(!b.spr.GORU);
    /*p30.DYNY*/ if (b.spr.GYGY) next.spr.TS_IDX_4  = not(!b.spr.ETYM);
    /*p30.DOBO*/ if (b.spr.GYGY) next.spr.TS_IDX_5  = not(!b.spr.EKOP);
    /*p30.AWAT*/ if (b.spr.GYGY) next.spr.TS_LINE_0 = not(!b.spr.ANED);
    /*p30.BACE*/ if (b.spr.GYGY) next.spr.TS_LINE_1 = not(!b.spr.ACEP);
    /*p30.BODU*/ if (b.spr.GYGY) next.spr.TS_LINE_2 = not(!b.spr.ABUX);
    /*p30.BUJA*/ if (b.spr.GYGY) next.spr.TS_LINE_3 = not(!b.spr.ABEG);

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
    /*p29.YBEZ*/ next.spr.STORE6_MATCHn = nand(b.spr.MATCH_EN, YDOT, YWAP);

    /*p29.XOJA*/ next.spr.XOJA = nor(b.spr.STORE6_MATCHn, b.spr.STORE5_MATCH_OUT);
    /*p29.WOMY*/ next.spr.SPRITE6_GET_SYNCn = tock_pos(a.spr.SPRITE_DONE, b.spr.SPRITE_DONE, b.lcd.VID_LINE_TRIG_d4p, b.spr.SPRITE6_GET_SYNCn, b.spr.XOJA);
    /*p29.WACY*/ next.spr.WACY = or(b.lcd.VID_LINE_TRIG_d4c, b.spr.SPRITE6_GET_SYNCn);
    /*p29.XAHO*/ next.spr.XAHO = not(b.spr.WACY);
    /*p29.WASE*/ wire STORE6_MATCH = not(b.spr.STORE6_MATCHn);
    /*p29.WYLA*/ next.spr.STORE6_MATCH_OUT = or(STORE6_MATCH, b.spr.STORE5_MATCH_OUT);

    /*p29.XUJO*/ wire STORE6_CLK = or(b.spr.STORE_EN, STORE6_SEL); // 0110
    /*p29.WEKA*/ next.spr.WEKA = not(STORE6_CLK);

    /*p29.ZAPE*/ next.spr.ZAPE = not(b.spr.WEKA);
    /*p31.YCOL*/ next.spr.YCOL = tock_pos(a.spr.ZAPE, b.spr.ZAPE, b.spr.XAHO, b.spr.YCOL, b.spr.OAM_A_D0b);
    /*p31.YRAC*/ next.spr.YRAC = tock_pos(a.spr.ZAPE, b.spr.ZAPE, b.spr.XAHO, b.spr.YRAC, b.spr.OAM_A_D1b);
    /*p31.YMEM*/ next.spr.YMEM = tock_pos(a.spr.ZAPE, b.spr.ZAPE, b.spr.XAHO, b.spr.YMEM, b.spr.OAM_A_D2b);
    /*p31.YVAG*/ next.spr.YVAG = tock_pos(a.spr.ZAPE, b.spr.ZAPE, b.spr.XAHO, b.spr.YVAG, b.spr.OAM_A_D3b);
    /*p31.ZOLY*/ next.spr.ZOLY = tock_pos(a.spr.ZAPE, b.spr.ZAPE, b.spr.XAHO, b.spr.ZOLY, b.spr.OAM_A_D4b);
    /*p31.ZOGO*/ next.spr.ZOGO = tock_pos(a.spr.ZAPE, b.spr.ZAPE, b.spr.XAHO, b.spr.ZOGO, b.spr.OAM_A_D5b);
    /*p31.ZECU*/ next.spr.ZECU = tock_pos(a.spr.ZAPE, b.spr.ZAPE, b.spr.XAHO, b.spr.ZECU, b.spr.OAM_A_D6b);
    /*p31.ZESA*/ next.spr.ZESA = tock_pos(a.spr.ZAPE, b.spr.ZAPE, b.spr.XAHO, b.spr.ZESA, b.spr.OAM_A_D7b);

    /*p29.WUSE*/ next.spr.WUSE = not(b.spr.WEKA);
    /*p30.GECU*/ next.spr.GECU = tock_pos(a.spr.WUSE, b.spr.WUSE, 0, b.spr.GECU, b.spr.TS_IDX_0 );
    /*p30.FOXY*/ next.spr.FOXY = tock_pos(a.spr.WUSE, b.spr.WUSE, 0, b.spr.FOXY, b.spr.TS_IDX_1 );
    /*p30.GOHU*/ next.spr.GOHU = tock_pos(a.spr.WUSE, b.spr.WUSE, 0, b.spr.GOHU, b.spr.TS_IDX_2 );
    /*p30.FOGO*/ next.spr.FOGO = tock_pos(a.spr.WUSE, b.spr.WUSE, 0, b.spr.FOGO, b.spr.TS_IDX_3 );
    /*p30.GACY*/ next.spr.GACY = tock_pos(a.spr.WUSE, b.spr.WUSE, 0, b.spr.GACY, b.spr.TS_IDX_4 );
    /*p30.GABO*/ next.spr.GABO = tock_pos(a.spr.WUSE, b.spr.WUSE, 0, b.spr.GABO, b.spr.TS_IDX_5 );
    
    /*p29.ZURU*/ next.spr.ZURU = not(b.spr.WEKA);
    /*p30.ZUBE*/ next.spr.ZUBE = tock_pos(a.spr.ZURU, b.spr.ZURU, 0, b.spr.ZUBE, b.spr.TS_LINE_0);
    /*p30.ZUMY*/ next.spr.ZUMY = tock_pos(a.spr.ZURU, b.spr.ZURU, 0, b.spr.ZUMY, b.spr.TS_LINE_1);
    /*p30.ZEXO*/ next.spr.ZEXO = tock_pos(a.spr.ZURU, b.spr.ZURU, 0, b.spr.ZEXO, b.spr.TS_LINE_2);
    /*p30.ZAFU*/ next.spr.ZAFU = tock_pos(a.spr.ZURU, b.spr.ZURU, 0, b.spr.ZAFU, b.spr.TS_LINE_3);

    /*p29.XYME*/ next.spr.XYME = not(b.spr.XOJA);
    /*p30.WOCY*/ if (b.spr.XYME) next.spr.TS_IDX_0  = not(!b.spr.GECU);
    /*p30.ELYC*/ if (b.spr.XYME) next.spr.TS_IDX_1  = not(!b.spr.FOXY);
    /*p30.WABO*/ if (b.spr.XYME) next.spr.TS_IDX_2  = not(!b.spr.GOHU);
    /*p30.EZOC*/ if (b.spr.XYME) next.spr.TS_IDX_3  = not(!b.spr.FOGO);
    /*p30.WYWY*/ if (b.spr.XYME) next.spr.TS_IDX_4  = not(!b.spr.GACY);
    /*p30.WATO*/ if (b.spr.XYME) next.spr.TS_IDX_5  = not(!b.spr.GABO);
    /*p30.ZUDO*/ if (b.spr.XYME) next.spr.TS_LINE_0 = not(!b.spr.ZUBE);
    /*p30.YBUK*/ if (b.spr.XYME) next.spr.TS_LINE_1 = not(!b.spr.ZUMY);
    /*p30.ZYTO*/ if (b.spr.XYME) next.spr.TS_LINE_2 = not(!b.spr.ZEXO);
    /*p30.YKOZ*/ if (b.spr.XYME) next.spr.TS_LINE_3 = not(!b.spr.ZAFU);
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
    /*p29.DYKA*/ next.spr.STORE7_MATCHn = nand(b.spr.MATCH_EN, MATCHA, MATCHB);
    /*p29.GYTE*/ wire STORE7_MATCH = not(b.spr.STORE7_MATCHn);
    /*p29.FAVO*/ next.spr.STORE7_MATCH_OUT = or(STORE7_MATCH, b.spr.STORE6_MATCH_OUT);

    /*p29.GUTU*/ wire STORE7_RD = nor(b.spr.STORE7_MATCHn, b.spr.STORE6_MATCH_OUT);
    /*p29.WAPO*/ next.spr.SPRITE7_GET_SYNCn = tock_pos(a.spr.SPRITE_DONE, b.spr.SPRITE_DONE, b.lcd.VID_LINE_TRIG_d4p, b.spr.SPRITE7_GET_SYNCn, STORE7_RD);
    /*p29.GUKY*/ wire STORE7_RST = or(b.lcd.VID_LINE_TRIG_d4c, b.spr.SPRITE7_GET_SYNCn);
    /*p29.GAFY*/ wire STORE7_RSTn = not(STORE7_RST);

    /*p29.GAPE*/ wire STORE7_CLK = or(b.spr.STORE_EN, STORE7_SEL); // 0111
    /*p29.GYVO*/ wire STORE7_CLKn = not(STORE7_CLK);

    /*p29.GECY*/ next.spr.GECY = not(STORE7_CLKn);
    /*p31.ERAZ*/ next.spr.STORE7_X0 = tock_pos(a.spr.GECY, b.spr.GECY, STORE7_RSTn, b.spr.STORE7_X0, b.spr.OAM_A_D0b);
    /*p31.EPUM*/ next.spr.STORE7_X1 = tock_pos(a.spr.GECY, b.spr.GECY, STORE7_RSTn, b.spr.STORE7_X1, b.spr.OAM_A_D1b);
    /*p31.EROL*/ next.spr.STORE7_X2 = tock_pos(a.spr.GECY, b.spr.GECY, STORE7_RSTn, b.spr.STORE7_X2, b.spr.OAM_A_D2b);
    /*p31.EHYN*/ next.spr.STORE7_X3 = tock_pos(a.spr.GECY, b.spr.GECY, STORE7_RSTn, b.spr.STORE7_X3, b.spr.OAM_A_D3b);
    /*p31.FAZU*/ next.spr.STORE7_X4 = tock_pos(a.spr.GECY, b.spr.GECY, STORE7_RSTn, b.spr.STORE7_X4, b.spr.OAM_A_D4b);
    /*p31.FAXE*/ next.spr.STORE7_X5 = tock_pos(a.spr.GECY, b.spr.GECY, STORE7_RSTn, b.spr.STORE7_X5, b.spr.OAM_A_D5b);
    /*p31.EXUK*/ next.spr.STORE7_X6 = tock_pos(a.spr.GECY, b.spr.GECY, STORE7_RSTn, b.spr.STORE7_X6, b.spr.OAM_A_D6b);
    /*p31.FEDE*/ next.spr.STORE7_X7 = tock_pos(a.spr.GECY, b.spr.GECY, STORE7_RSTn, b.spr.STORE7_X7, b.spr.OAM_A_D7b);

    /*p29.FEFO*/ next.spr.FEFO = not(STORE7_CLKn);
    /*p30.FUZO*/ next.spr.FUZO = tock_pos(a.spr.FEFO, b.spr.FEFO, 0, b.spr.FUZO, b.spr.TS_IDX_0 );
    /*p30.GESY*/ next.spr.GESY = tock_pos(a.spr.FEFO, b.spr.FEFO, 0, b.spr.GESY, b.spr.TS_IDX_1 );
    /*p30.FYSU*/ next.spr.FYSU = tock_pos(a.spr.FEFO, b.spr.FEFO, 0, b.spr.FYSU, b.spr.TS_IDX_2 );
    /*p30.FEFA*/ next.spr.FEFA = tock_pos(a.spr.FEFO, b.spr.FEFO, 0, b.spr.FEFA, b.spr.TS_IDX_3 );
    /*p30.GYNO*/ next.spr.GYNO = tock_pos(a.spr.FEFO, b.spr.FEFO, 0, b.spr.GYNO, b.spr.TS_IDX_4 );
    /*p30.GULE*/ next.spr.GULE = tock_pos(a.spr.FEFO, b.spr.FEFO, 0, b.spr.GULE, b.spr.TS_IDX_5 );

    /*p29.WABE*/ next.spr.WABE = not(STORE7_CLKn);
    /*p30.XYGO*/ next.spr.XYGO = tock_pos(a.spr.WABE, b.spr.WABE, 0, b.spr.XYGO, b.spr.TS_LINE_0);
    /*p30.XYNA*/ next.spr.XYNA = tock_pos(a.spr.WABE, b.spr.WABE, 0, b.spr.XYNA, b.spr.TS_LINE_1);
    /*p30.XAKU*/ next.spr.XAKU = tock_pos(a.spr.WABE, b.spr.WABE, 0, b.spr.XAKU, b.spr.TS_LINE_2);
    /*p30.YGUM*/ next.spr.YGUM = tock_pos(a.spr.WABE, b.spr.WABE, 0, b.spr.YGUM, b.spr.TS_LINE_3);

    /*p29.GUGY*/ wire STORE7_RDn = not(STORE7_RD);
    /*p30.EVYT*/ if (!STORE7_RDn) next.spr.TS_IDX_0  = not(!b.spr.FUZO);
    /*p30.WABA*/ if (!STORE7_RDn) next.spr.TS_IDX_1  = not(!b.spr.GESY);
    /*p30.ETAD*/ if (!STORE7_RDn) next.spr.TS_IDX_2  = not(!b.spr.FYSU);
    /*p30.ELEP*/ if (!STORE7_RDn) next.spr.TS_IDX_3  = not(!b.spr.FEFA);
    /*p30.WYGO*/ if (!STORE7_RDn) next.spr.TS_IDX_4  = not(!b.spr.GYNO);
    /*p30.WAKO*/ if (!STORE7_RDn) next.spr.TS_IDX_5  = not(!b.spr.GULE);
    /*p30.WANA*/ if (!STORE7_RDn) next.spr.TS_LINE_0 = not(!b.spr.XYGO);
    /*p30.WAXE*/ if (!STORE7_RDn) next.spr.TS_LINE_1 = not(!b.spr.XYNA);
    /*p30.WABU*/ if (!STORE7_RDn) next.spr.TS_LINE_2 = not(!b.spr.XAKU);
    /*p30.YPOZ*/ if (!STORE7_RDn) next.spr.TS_LINE_3 = not(!b.spr.YGUM);

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
    /*p29.EFYL*/ next.spr.STORE8_MATCHn = nand(b.spr.MATCH_EN, FEHA, DAMA);
    /*p29.GEKE*/ wire STORE8_MATCH = not(b.spr.STORE8_MATCHn);
    /*p29.GYGA*/ next.spr.STORE8_MATCH_OUT = or(STORE8_MATCH, b.spr.STORE7_MATCH_OUT);
    /*p29.FOXA*/ next.spr.FOXA = nor(b.spr.STORE8_MATCHn, b.spr.STORE7_MATCH_OUT);

    /*p29.EXUQ*/ next.spr.SPRITE8_GET_SYNCn = tock_pos(a.spr.SPRITE_DONE, b.spr.SPRITE_DONE, b.lcd.VID_LINE_TRIG_d4p, b.spr.SPRITE8_GET_SYNCn, b.spr.FOXA);
    /*p29.GORO*/ next.spr.GORO = or(b.lcd.VID_LINE_TRIG_d4c, b.spr.SPRITE8_GET_SYNCn);
    /*p29.WUZO*/ next.spr.WUZO = not(b.spr.GORO);

    /*p29.CAHO*/ wire STORE8_CLK = or(b.spr.STORE_EN, STORE8_SEL); // 1000
    /*p29.BUKA*/ next.spr.BUKA = not(STORE8_CLK);
    /*p29.CEXU*/ next.spr.CEXU = not(b.spr.BUKA);
    /*p29.AKOL*/ next.spr.AKOL = not(b.spr.BUKA);
    /*p29.BYMY*/ next.spr.BYMY = not(b.spr.BUKA);
    /*p31.GAVY*/ next.spr.GAVY = tock_pos(a.spr.CEXU, b.spr.CEXU, b.spr.WUZO, b.spr.GAVY, b.spr.OAM_A_D0b);
    /*p31.GYPU*/ next.spr.GYPU = tock_pos(a.spr.CEXU, b.spr.CEXU, b.spr.WUZO, b.spr.GYPU, b.spr.OAM_A_D1b);
    /*p31.GADY*/ next.spr.GADY = tock_pos(a.spr.CEXU, b.spr.CEXU, b.spr.WUZO, b.spr.GADY, b.spr.OAM_A_D2b);
    /*p31.GAZA*/ next.spr.GAZA = tock_pos(a.spr.CEXU, b.spr.CEXU, b.spr.WUZO, b.spr.GAZA, b.spr.OAM_A_D3b);
    /*p31.EZUF*/ next.spr.EZUF = tock_pos(a.spr.CEXU, b.spr.CEXU, b.spr.WUZO, b.spr.EZUF, b.spr.OAM_A_D4b);
    /*p31.ENAD*/ next.spr.ENAD = tock_pos(a.spr.CEXU, b.spr.CEXU, b.spr.WUZO, b.spr.ENAD, b.spr.OAM_A_D5b);
    /*p31.EBOW*/ next.spr.EBOW = tock_pos(a.spr.CEXU, b.spr.CEXU, b.spr.WUZO, b.spr.EBOW, b.spr.OAM_A_D6b);
    /*p31.FYCA*/ next.spr.FYCA = tock_pos(a.spr.CEXU, b.spr.CEXU, b.spr.WUZO, b.spr.FYCA, b.spr.OAM_A_D7b);


    /*p30.AXUV*/ next.spr.AXUV = tock_pos(a.spr.AKOL, b.spr.AKOL, 0, b.spr.AXUV, b.spr.TS_IDX_0 );
    /*p30.BADA*/ next.spr.BADA = tock_pos(a.spr.AKOL, b.spr.AKOL, 0, b.spr.BADA, b.spr.TS_IDX_1 );
    /*p30.APEV*/ next.spr.APEV = tock_pos(a.spr.AKOL, b.spr.AKOL, 0, b.spr.APEV, b.spr.TS_IDX_2 );
    /*p30.BADO*/ next.spr.BADO = tock_pos(a.spr.AKOL, b.spr.AKOL, 0, b.spr.BADO, b.spr.TS_IDX_3 );
    /*p30.BEXY*/ next.spr.BEXY = tock_pos(a.spr.AKOL, b.spr.AKOL, 0, b.spr.BEXY, b.spr.TS_IDX_4 );
    /*p30.BYHE*/ next.spr.BYHE = tock_pos(a.spr.AKOL, b.spr.AKOL, 0, b.spr.BYHE, b.spr.TS_IDX_5 );
    /*p30.AFYM*/ next.spr.AFYM = tock_pos(a.spr.BYMY, b.spr.BYMY, 0, b.spr.AFYM, b.spr.TS_LINE_0);
    /*p30.AZAP*/ next.spr.AZAP = tock_pos(a.spr.BYMY, b.spr.BYMY, 0, b.spr.AZAP, b.spr.TS_LINE_1);
    /*p30.AFUT*/ next.spr.AFUT = tock_pos(a.spr.BYMY, b.spr.BYMY, 0, b.spr.AFUT, b.spr.TS_LINE_2);
    /*p30.AFYX*/ next.spr.AFYX = tock_pos(a.spr.BYMY, b.spr.BYMY, 0, b.spr.AFYX, b.spr.TS_LINE_3);

    /*p29.DENY*/ next.spr.DENY = not(b.spr.FOXA);
    /*p30.ADYB*/ if (b.spr.DENY) next.spr.TS_IDX_0  = b.spr.AXUV;
    /*p30.APOB*/ if (b.spr.DENY) next.spr.TS_IDX_1  = b.spr.BADA;
    /*p30.APYV*/ if (b.spr.DENY) next.spr.TS_IDX_2  = b.spr.APEV;
    /*p30.AFEN*/ if (b.spr.DENY) next.spr.TS_IDX_3  = b.spr.BADO;
    /*p30.AKYH*/ if (b.spr.DENY) next.spr.TS_IDX_4  = b.spr.BEXY;
    /*p30.APOC*/ if (b.spr.DENY) next.spr.TS_IDX_5  = b.spr.BYHE;
    /*p30.BUJY*/ if (b.spr.DENY) next.spr.TS_LINE_0 = b.spr.AFYM;
    /*p30.BOSO*/ if (b.spr.DENY) next.spr.TS_LINE_1 = b.spr.AZAP;
    /*p30.AHAC*/ if (b.spr.DENY) next.spr.TS_LINE_2 = b.spr.AFUT;
    /*p30.BAZU*/ if (b.spr.DENY) next.spr.TS_LINE_3 = b.spr.AFYX;
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
    /*p29.YGEM*/ next.spr.STORE9_MATCHn = nand(b.spr.MATCH_EN, MATCHA, MATCHB);

    /*p29.CATO*/ wire STORE9_CLK = or(b.spr.STORE_EN, STORE9_SEL); // 1001
    /*p29.DECU*/ wire STORE9_CLKn = not(STORE9_CLK);

    /*p29.GUZE*/ wire SPRITE9_GETn = nor(b.spr.STORE9_MATCHn, b.spr.STORE8_MATCH_OUT);
    /*p29.FONO*/ next.spr.SPRITE9_GET_SYNCn = tock_pos(a.spr.SPRITE_DONE, b.spr.SPRITE_DONE, b.lcd.VID_LINE_TRIG_d4p, b.spr.SPRITE9_GET_SYNCn, SPRITE9_GETn);

    /*p29.DUBU*/ wire STORE9_RST  = or(b.lcd.VID_LINE_TRIG_d4c, b.spr.SPRITE9_GET_SYNCn);
    /*p29.DOSY*/ wire STORE9_RSTn = not(STORE9_RST);
    
    /*p29.WEME*/ next.spr.STORE9_CLKa = not(STORE9_CLKn);
    /*p31.XUVY*/ next.spr.STORE9_X0    = tock_pos(a.spr.STORE9_CLKa, b.spr.STORE9_CLKa, STORE9_RSTn, b.spr.STORE9_X0,    b.spr.OAM_A_D0b);
    /*p31.XERE*/ next.spr.STORE9_X1    = tock_pos(a.spr.STORE9_CLKa, b.spr.STORE9_CLKa, STORE9_RSTn, b.spr.STORE9_X1,    b.spr.OAM_A_D1b);
    /*p31.XUZO*/ next.spr.STORE9_X2    = tock_pos(a.spr.STORE9_CLKa, b.spr.STORE9_CLKa, STORE9_RSTn, b.spr.STORE9_X2,    b.spr.OAM_A_D2b);
    /*p31.XEXA*/ next.spr.STORE9_X3    = tock_pos(a.spr.STORE9_CLKa, b.spr.STORE9_CLKa, STORE9_RSTn, b.spr.STORE9_X3,    b.spr.OAM_A_D3b);
    /*p31.YPOD*/ next.spr.STORE9_X4    = tock_pos(a.spr.STORE9_CLKa, b.spr.STORE9_CLKa, STORE9_RSTn, b.spr.STORE9_X4,    b.spr.OAM_A_D4b);
    /*p31.YROP*/ next.spr.STORE9_X5    = tock_pos(a.spr.STORE9_CLKa, b.spr.STORE9_CLKa, STORE9_RSTn, b.spr.STORE9_X5,    b.spr.OAM_A_D5b);
    /*p31.YNEP*/ next.spr.STORE9_X6    = tock_pos(a.spr.STORE9_CLKa, b.spr.STORE9_CLKa, STORE9_RSTn, b.spr.STORE9_X6,    b.spr.OAM_A_D6b);
    /*p31.YZOF*/ next.spr.STORE9_X7    = tock_pos(a.spr.STORE9_CLKa, b.spr.STORE9_CLKa, STORE9_RSTn, b.spr.STORE9_X7,    b.spr.OAM_A_D7b);

    /*p29.WUFA*/ next.spr.STORE9_CLKb = not(STORE9_CLKn);
    /*p30.XUFO*/ next.spr.STORE9_IDX0  = tock_pos(a.spr.STORE9_CLKb, b.spr.STORE9_CLKb, 0,           b.spr.STORE9_IDX0,  b.spr.TS_IDX_0 );
    /*p30.XUTE*/ next.spr.STORE9_IDX1  = tock_pos(a.spr.STORE9_CLKb, b.spr.STORE9_CLKb, 0,           b.spr.STORE9_IDX1,  b.spr.TS_IDX_1 );
    /*p30.XOTU*/ next.spr.STORE9_IDX2  = tock_pos(a.spr.STORE9_CLKb, b.spr.STORE9_CLKb, 0,           b.spr.STORE9_IDX2,  b.spr.TS_IDX_2 );
    /*p30.XYFE*/ next.spr.STORE9_IDX3  = tock_pos(a.spr.STORE9_CLKb, b.spr.STORE9_CLKb, 0,           b.spr.STORE9_IDX3,  b.spr.TS_IDX_3 );
    /*p30.YZOR*/ next.spr.STORE9_IDX4  = tock_pos(a.spr.STORE9_CLKb, b.spr.STORE9_CLKb, 0,           b.spr.STORE9_IDX4,  b.spr.TS_IDX_4 );
    /*p30.YBER*/ next.spr.STORE9_IDX5  = tock_pos(a.spr.STORE9_CLKb, b.spr.STORE9_CLKb, 0,           b.spr.STORE9_IDX5,  b.spr.TS_IDX_5 );

    /*p29.FAKA*/ next.spr.STORE9_CLKc = not(STORE9_CLKn);
    /*p30.DEWU*/ next.spr.STORE9_LINE0 = tock_pos(a.spr.STORE9_CLKc, b.spr.STORE9_CLKc, 0,           b.spr.STORE9_LINE0, b.spr.TS_LINE_0);
    /*p30.CANA*/ next.spr.STORE9_LINE1 = tock_pos(a.spr.STORE9_CLKc, b.spr.STORE9_CLKc, 0,           b.spr.STORE9_LINE1, b.spr.TS_LINE_1);
    /*p30.DYSY*/ next.spr.STORE9_LINE2 = tock_pos(a.spr.STORE9_CLKc, b.spr.STORE9_CLKc, 0,           b.spr.STORE9_LINE2, b.spr.TS_LINE_2);
    /*p30.FOFO*/ next.spr.STORE9_LINE3 = tock_pos(a.spr.STORE9_CLKc, b.spr.STORE9_CLKc, 0,           b.spr.STORE9_LINE3, b.spr.TS_LINE_3);

    /*p29.FADO*/ wire SPRITE9_GET = not(SPRITE9_GETn);
    /*p30.YHAL*/ if (SPRITE9_GET) next.spr.TS_IDX_0  = not(!b.spr.STORE9_IDX0);
    /*p30.YRAD*/ if (SPRITE9_GET) next.spr.TS_IDX_1  = not(!b.spr.STORE9_IDX1);
    /*p30.XYRA*/ if (SPRITE9_GET) next.spr.TS_IDX_2  = not(!b.spr.STORE9_IDX2);
    /*p30.YNEV*/ if (SPRITE9_GET) next.spr.TS_IDX_3  = not(!b.spr.STORE9_IDX3);
    /*p30.ZOJY*/ if (SPRITE9_GET) next.spr.TS_IDX_4  = not(!b.spr.STORE9_IDX4);
    /*p30.ZARO*/ if (SPRITE9_GET) next.spr.TS_IDX_5  = not(!b.spr.STORE9_IDX5);
    /*p30.CAWO*/ if (SPRITE9_GET) next.spr.TS_LINE_0 = not(!b.spr.STORE9_LINE0);
    /*p30.BYME*/ if (SPRITE9_GET) next.spr.TS_LINE_1 = not(!b.spr.STORE9_LINE1);
    /*p30.COHO*/ if (SPRITE9_GET) next.spr.TS_LINE_2 = not(!b.spr.STORE9_LINE2);
    /*p30.GATE*/ if (SPRITE9_GET) next.spr.TS_LINE_3 = not(!b.spr.STORE9_LINE3);

  }
#endif

//-----------------------------------------------------------------------------
};
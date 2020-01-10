#include "Sch_SpriteStore.h"

#include "Sch_Video.h"
#include "Sch_Sprites.h"
#include "Sch_Clocks.h"
#include "Sch_Pins.h"
#include "Sch_Resets.h"
#include "Sch_LCD.h"
#include "Sch_OAM.h"


namespace Schematics {

#if 0

//-----------------------------------------------------------------------------

bool SpriteMatcherRegisters::commit() {
  bool changed = false;
  changed |= commit_all(FONO_STORE_CLEARn);
  changed |= commit_all(STORE_X0, STORE_X1, STORE_X2, STORE_X3, STORE_X4, STORE_X5, STORE_X6, STORE_X7);
  changed |= commit_all(STORE_IDX0, STORE_IDX1, STORE_IDX2, STORE_IDX3, STORE_IDX4, STORE_IDX5);
  changed |= commit_all(STORE_LINE0, STORE_LINE1, STORE_LINE2, STORE_LINE3);
  return changed;
}

//-----------------------------------------------------------------------------

void SpriteMatcherRegisters::tick_store(const LcdSignals& lcd_sig,
                                   const SpriteSignals& spr_sig,
                                   const OamRegisters& oam_reg,
                                   bool STORE_SEL,
                                   SpriteTristate& spr_tri) {

  /*p31.ZAGO*/ wire SPRITE_X0 = not(!oam_reg.REG_OAM_A0);
  /*p31.ZOCY*/ wire SPRITE_X1 = not(!oam_reg.REG_OAM_A1);
  /*p31.YPUR*/ wire SPRITE_X2 = not(!oam_reg.REG_OAM_A2);
  /*p31.YVOK*/ wire SPRITE_X3 = not(!oam_reg.REG_OAM_A3);
  /*p31.COSE*/ wire SPRITE_X4 = not(!oam_reg.REG_OAM_A4);
  /*p31.AROP*/ wire SPRITE_X5 = not(!oam_reg.REG_OAM_A5);
  /*p31.XATU*/ wire SPRITE_X6 = not(!oam_reg.REG_OAM_A6);
  /*p31.BADY*/ wire SPRITE_X7 = not(!oam_reg.REG_OAM_A7);

  /*p29.CATO*/ wire STORE_CLK = or(spr_sig.DYTY_STORE_EN, STORE_SEL);
  /*p29.DECU*/ wire STORE_CLKn = not(STORE_CLK);
  /*p29.DUBU*/ wire STORE_RST  = or(lcd_sig.DYBA_VID_LINE_TRIG_d4, FONO_STORE_CLEARn);
  /*p29.DOSY*/ wire STORE_RSTn = not(STORE_RST);

  /*p29.WEME*/ wire STORE_CLKa = not(STORE_CLKn);
  /*p31.XUVY*/ STORE_X0.set(STORE_CLKa, STORE_RSTn, SPRITE_X0);
  /*p31.XERE*/ STORE_X1.set(STORE_CLKa, STORE_RSTn, SPRITE_X1);
  /*p31.XUZO*/ STORE_X2.set(STORE_CLKa, STORE_RSTn, SPRITE_X2);
  /*p31.XEXA*/ STORE_X3.set(STORE_CLKa, STORE_RSTn, SPRITE_X3);
  /*p31.YPOD*/ STORE_X4.set(STORE_CLKa, STORE_RSTn, SPRITE_X4);
  /*p31.YROP*/ STORE_X5.set(STORE_CLKa, STORE_RSTn, SPRITE_X5);
  /*p31.YNEP*/ STORE_X6.set(STORE_CLKa, STORE_RSTn, SPRITE_X6);
  /*p31.YZOF*/ STORE_X7.set(STORE_CLKa, STORE_RSTn, SPRITE_X7);

  /*p29.WUFA*/ wire STORE_CLKb = not(STORE_CLKn);
  /*p30.XUFO*/ STORE_IDX0.set(STORE_CLKb, spr_tri.TS_IDX_0);
  /*p30.XUTE*/ STORE_IDX1.set(STORE_CLKb, spr_tri.TS_IDX_1);
  /*p30.XOTU*/ STORE_IDX2.set(STORE_CLKb, spr_tri.TS_IDX_2);
  /*p30.XYFE*/ STORE_IDX3.set(STORE_CLKb, spr_tri.TS_IDX_3);
  /*p30.YZOR*/ STORE_IDX4.set(STORE_CLKb, spr_tri.TS_IDX_4);
  /*p30.YBER*/ STORE_IDX5.set(STORE_CLKb, spr_tri.TS_IDX_5);

  /*p29.FAKA*/ wire STORE_CLKc = not(STORE_CLKn);
  /*p30.DEWU*/ STORE_LINE0.set(STORE_CLKc, spr_tri.TS_LINE_0);
  /*p30.CANA*/ STORE_LINE1.set(STORE_CLKc, spr_tri.TS_LINE_1);
  /*p30.DYSY*/ STORE_LINE2.set(STORE_CLKc, spr_tri.TS_LINE_2);
  /*p30.FOFO*/ STORE_LINE3.set(STORE_CLKc, spr_tri.TS_LINE_3);
}

//-----------------------------------------------------------------------------

bool SpriteMatcherRegisters::tick_fetch(const LcdSignals& lcd_sig,
                                   const SpriteSignals& spr_sig,
                                   const VidRegisters& vid_reg,
                                   bool MATCH_CHAINn,
                                   SpriteTristate& spr_tri) {

  /*p21.ACAM*/ wire X0n = not(vid_reg.X0);
  /*p21.AZUB*/ wire X1n = not(vid_reg.X1);
  /*p21.AMEL*/ wire X2n = not(vid_reg.X2);
  /*p21.AHAL*/ wire X3n = not(vid_reg.X3);
  /*p21.APUX*/ wire X4n = not(vid_reg.X4);
  /*p21.ABEF*/ wire X5n = not(vid_reg.X5);
  /*p21.ADAZ*/ wire X6n = not(vid_reg.X6);
  /*p21.ASAH*/ wire X7n = not(vid_reg.X7);

  /*p31.YMAM*/ wire MATCH0 = xor(STORE_X0, X0n);
  /*p31.YTYP*/ wire MATCH1 = xor(STORE_X1, X1n);
  /*p31.YFOP*/ wire MATCH2 = xor(STORE_X2, X2n);
  /*p31.YVAC*/ wire MATCH3 = xor(STORE_X3, X3n);
  /*p31.ZYWU*/ wire MATCH4 = xor(STORE_X4, X4n);
  /*p31.ZUZA*/ wire MATCH5 = xor(STORE_X5, X5n);
  /*p31.ZEJO*/ wire MATCH6 = xor(STORE_X6, X6n);
  /*p31.ZEDA*/ wire MATCH7 = xor(STORE_X7, X7n);
  /*p31.YLEV*/ wire MATCHA = nor(MATCH0, MATCH1, MATCH2, MATCH3);
  /*p31.YTUB*/ wire MATCHB = nor(MATCH4, MATCH5, MATCH6, MATCH7);
  /*p29.YGEM*/ wire MATCHn = nand(spr_sig.AROR_MATCH_EN, MATCHA, MATCHB);

  /*p29.GUZE*/ wire GUZE_SPRITE_GETn = nor(MATCHn, MATCH_CHAINn);
  /*p29.FONO*/ FONO_STORE_CLEARn.set(spr_sig.WUTY_SPRITE_DONE, lcd_sig.BYVA_VID_LINE_TRIG_d4n, GUZE_SPRITE_GETn);
  /*p29.FADO*/ wire FADO_SPRITE_GET = not(GUZE_SPRITE_GETn);
  /*p30.YHAL*/ if (FADO_SPRITE_GET) spr_tri.TS_IDX_0  = not(!STORE_IDX0);
  /*p30.YRAD*/ if (FADO_SPRITE_GET) spr_tri.TS_IDX_1  = not(!STORE_IDX1);
  /*p30.XYRA*/ if (FADO_SPRITE_GET) spr_tri.TS_IDX_2  = not(!STORE_IDX2);
  /*p30.YNEV*/ if (FADO_SPRITE_GET) spr_tri.TS_IDX_3  = not(!STORE_IDX3);
  /*p30.ZOJY*/ if (FADO_SPRITE_GET) spr_tri.TS_IDX_4  = not(!STORE_IDX4);
  /*p30.ZARO*/ if (FADO_SPRITE_GET) spr_tri.TS_IDX_5  = not(!STORE_IDX5);
  /*p30.CAWO*/ if (FADO_SPRITE_GET) spr_tri.TS_LINE_0 = not(!STORE_LINE0);
  /*p30.BYME*/ if (FADO_SPRITE_GET) spr_tri.TS_LINE_1 = not(!STORE_LINE1);
  /*p30.COHO*/ if (FADO_SPRITE_GET) spr_tri.TS_LINE_2 = not(!STORE_LINE2);
  /*p30.GATE*/ if (FADO_SPRITE_GET) spr_tri.TS_LINE_3 = not(!STORE_LINE3);

  return MATCHn;
}

//-----------------------------------------------------------------------------

bool SpriteStoreRegisters::pass_end() {
  bool changed = false;
  changed |= commit_all(store0, store1, store2, store3, store4, store5, store6, store7, store8, store9);
  changed |= commit_all(STORE_EN_SYNC, SPRITE_COUNT0, SPRITE_COUNT1, SPRITE_COUNT2, SPRITE_COUNT3);

  // FIXME should detect changes to sil here too

  return changed;
}

//-----------------------------------------------------------------------------

void SpriteStoreRegisters::tick_store(const ClkSignals& clk_sig,
                                      const RstSignals& rst_sig,
                                      const LcdSignals& lcd_sig,
                                      const SpriteSignals& spr_sig,
                                      const OamRegisters& oam_reg) {

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

  store0.tick_store(lcd_sig, spr_sig, oam_reg, STORE0_SEL, spr_tri);
  store1.tick_store(lcd_sig, spr_sig, oam_reg, STORE1_SEL, spr_tri);
  store2.tick_store(lcd_sig, spr_sig, oam_reg, STORE2_SEL, spr_tri);
  store3.tick_store(lcd_sig, spr_sig, oam_reg, STORE3_SEL, spr_tri);
  store4.tick_store(lcd_sig, spr_sig, oam_reg, STORE4_SEL, spr_tri);
  store5.tick_store(lcd_sig, spr_sig, oam_reg, STORE5_SEL, spr_tri);
  store6.tick_store(lcd_sig, spr_sig, oam_reg, STORE6_SEL, spr_tri);
  store7.tick_store(lcd_sig, spr_sig, oam_reg, STORE7_SEL, spr_tri);
  store8.tick_store(lcd_sig, spr_sig, oam_reg, STORE8_SEL, spr_tri);
  store9.tick_store(lcd_sig, spr_sig, oam_reg, STORE9_SEL, spr_tri);

  /*p29.DEZY*/ STORE_EN_SYNC.set(clk_sig.ZEME_xBxDxFxH, rst_sig.VID_RESETn, spr_sig.DYTY_STORE_EN);

  /*p29.BAKY*/ wire SPRITES_FULL = and(SPRITE_COUNT1, SPRITE_COUNT3);
  /*p29.CAKE*/ wire SPRITE_COUNT_CLK = or(SPRITES_FULL, STORE_EN_SYNC);
  /*p28.AZYB*/ wire SPRITE_COUNT_RSTn = not(lcd_sig.BYHA_VID_LINE_TRIG_d4n);
  /*p29.BESE*/ SPRITE_COUNT0.set(SPRITE_COUNT_CLK,  SPRITE_COUNT_RSTn, !SPRITE_COUNT0);
  /*p29.CUXY*/ SPRITE_COUNT1.set(SPRITE_COUNT0,     SPRITE_COUNT_RSTn, !SPRITE_COUNT1);
  /*p29.BEGO*/ SPRITE_COUNT2.set(SPRITE_COUNT1,     SPRITE_COUNT_RSTn, !SPRITE_COUNT2);
  /*p29.DYBE*/ SPRITE_COUNT3.set(SPRITE_COUNT2,     SPRITE_COUNT_RSTn, !SPRITE_COUNT3);
}

//-----------------------------------------------------------------------------

bool SpriteStoreRegisters::tick_fetch(const LcdSignals& lcd_sig,
                                      const SpriteSignals& spr_sig,
                                      const VidRegisters& vid_reg) {

  wire match0n = store0.tick_fetch(lcd_sig, spr_sig, vid_reg, false,   spr_tri);
  wire match1n = store1.tick_fetch(lcd_sig, spr_sig, vid_reg, match0n, spr_tri);
  wire match2n = store2.tick_fetch(lcd_sig, spr_sig, vid_reg, match1n, spr_tri);
  wire match3n = store3.tick_fetch(lcd_sig, spr_sig, vid_reg, match2n, spr_tri);
  wire match4n = store4.tick_fetch(lcd_sig, spr_sig, vid_reg, match3n, spr_tri);
  wire match5n = store5.tick_fetch(lcd_sig, spr_sig, vid_reg, match4n, spr_tri);
  wire match6n = store6.tick_fetch(lcd_sig, spr_sig, vid_reg, match5n, spr_tri);
  wire match7n = store7.tick_fetch(lcd_sig, spr_sig, vid_reg, match6n, spr_tri);
  wire match8n = store8.tick_fetch(lcd_sig, spr_sig, vid_reg, match7n, spr_tri);
  wire match9n = store9.tick_fetch(lcd_sig, spr_sig, vid_reg, match8n, spr_tri);

  /*p29.FEFY*/ wire FEFY = nand(match4n, match3n, match2n, match1n, match0n);
  /*p29.FOVE*/ wire FOVE = nand(match9n, match8n, match7n, match6n, match5n);
  /*p29.FEPO*/ wire FEPO_STORE_MATCH = or(FEFY, FOVE);

  return FEPO_STORE_MATCH;
}

#endif

//-----------------------------------------------------------------------------

#if 0
  //----------
  // store 0

  {
    /*p31.ZOGY*/ wire STORE0_MATCH0n = xor(STORE0_X0, X0n);
    /*p31.ZEBA*/ wire STORE0_MATCH1n = xor(STORE0_X1, X1n);
    /*p31.ZAHA*/ wire STORE0_MATCH2n = xor(STORE0_X2, X2n);
    /*p31.ZOKY*/ wire STORE0_MATCH3n = xor(STORE0_X3, X3n);
    /*p31.WOJU*/ wire STORE0_MATCH4n = xor(STORE0_X4, X4n);
    /*p31.YFUN*/ wire STORE0_MATCH5n = xor(STORE0_X5, X5n);
    /*p31.WYZA*/ wire STORE0_MATCH6n = xor(STORE0_X6, X6n);
    /*p31.YPUK*/ wire STORE0_MATCH7n = xor(STORE0_X7, X7n);
    /*p31.ZAKO*/ wire STORE0_MATCHA = nor(STORE0_MATCH0n, STORE0_MATCH1n, STORE0_MATCH2n, STORE0_MATCH3n);
    /*p31.XEBA*/ wire STORE0_MATCHB = nor(STORE0_MATCH4n, STORE0_MATCH5n, STORE0_MATCH6n, STORE0_MATCH7n);

    /*p29.YDUG*/ sst_reg.STORE0_MATCHn = nand(MATCH_EN, STORE0_MATCHB, STORE0_MATCHA);
    /*p29.WEFU*/ wire STORE0_MATCH = not(STORE0_MATCHn);
    /*p29.GEZE*/ sst_reg.STORE0_MATCH_OUT = or (STORE0_MATCH, pins.P10_B);

    /*p29.GUVA*/ wire SPRITE0_GET = nor(STORE0_MATCHn,  pins.P10_B);
    /*p29.EBOJ*/ sst_reg.SPRITE0_GET_SYNCn.set(SPRITE_DONE, SPRITE_DONE, BYVA_VID_LINE_TRIG_d4n, SPRITE0_GET_SYNCn, SPRITE0_GET);

    /*p29.CEMY*/ wire STORE0_CLK    = or(STORE_EN, STORE0_SEL);
    /*p29.DYHU*/ wire STORE0_CLKn   = not(STORE0_CLK);
    /*p29.DYWE*/ wire STORE0_RST    = or(DYBA_VID_LINE_TRIG_d4, SPRITE0_GET_SYNCn);
    /*p29.DYNA*/ wire STORE0_RSTn   = not(STORE0_RST);

    /*p29.GENY*/ sst_reg.STORE0_CLKa  = not(STORE0_CLKn);
    /*p30.YGUS*/ sst_reg.STORE0_IDX0 .set(STORE0_CLKa, STORE0_CLKa, 0,           STORE0_IDX0,  TS_IDX_0);
    /*p30.YSOK*/ sst_reg.STORE0_IDX1 .set(STORE0_CLKa, STORE0_CLKa, 0,           STORE0_IDX1,  TS_IDX_1);
    /*p30.YZEP*/ sst_reg.STORE0_IDX2 .set(STORE0_CLKa, STORE0_CLKa, 0,           STORE0_IDX2,  TS_IDX_2);
    /*p30.WYTE*/ sst_reg.STORE0_IDX3 .set(STORE0_CLKa, STORE0_CLKa, 0,           STORE0_IDX3,  TS_IDX_3);
    /*p30.ZONY*/ sst_reg.STORE0_IDX4 .set(STORE0_CLKa, STORE0_CLKa, 0,           STORE0_IDX4,  TS_IDX_4);
    /*p30.YWAK*/ sst_reg.STORE0_IDX5 .set(STORE0_CLKa, STORE0_CLKa, 0,           STORE0_IDX5,  TS_IDX_5);
    
    /*p29.ENOB*/ sst_reg.STORE0_CLKb  = not(STORE0_CLKn);
    /*p30.FYHY*/ sst_reg.STORE0_LINE0.set(STORE0_CLKb, STORE0_CLKb, 0,           STORE0_LINE0, TS_LINE_0);
    /*p30.GYHO*/ sst_reg.STORE0_LINE1.set(STORE0_CLKb, STORE0_CLKb, 0,           STORE0_LINE1, TS_LINE_1);
    /*p30.BOZU*/ sst_reg.STORE0_LINE2.set(STORE0_CLKb, STORE0_CLKb, 0,           STORE0_LINE2, TS_LINE_2);
    /*p30.CUFO*/ sst_reg.STORE0_LINE3.set(STORE0_CLKb, STORE0_CLKb, 0,           STORE0_LINE3, TS_LINE_3);

    /*p29.FUXU*/ sst_reg.STORE0_CLKc  = not(STORE0_CLKn);
    /*p31.XEPE*/ sst_reg.STORE0_X0   .set(STORE0_CLKc, STORE0_CLKc, STORE0_RSTn, STORE0_X0,    OAM_A_D0b);
    /*p31.YLAH*/ sst_reg.STORE0_X1   .set(STORE0_CLKc, STORE0_CLKc, STORE0_RSTn, STORE0_X1,    OAM_A_D1b);
    /*p31.ZOLA*/ sst_reg.STORE0_X2   .set(STORE0_CLKc, STORE0_CLKc, STORE0_RSTn, STORE0_X2,    OAM_A_D2b);
    /*p31.ZULU*/ sst_reg.STORE0_X3   .set(STORE0_CLKc, STORE0_CLKc, STORE0_RSTn, STORE0_X3,    OAM_A_D3b);
    /*p31.WELO*/ sst_reg.STORE0_X4   .set(STORE0_CLKc, STORE0_CLKc, STORE0_RSTn, STORE0_X4,    OAM_A_D4b);
    /*p31.XUNY*/ sst_reg.STORE0_X5   .set(STORE0_CLKc, STORE0_CLKc, STORE0_RSTn, STORE0_X5,    OAM_A_D5b);
    /*p31.WOTE*/ sst_reg.STORE0_X6   .set(STORE0_CLKc, STORE0_CLKc, STORE0_RSTn, STORE0_X6,    OAM_A_D6b);
    /*p31.XAKO*/ sst_reg.STORE0_X7   .set(STORE0_CLKc, STORE0_CLKc, STORE0_RSTn, STORE0_X7,    OAM_A_D7b);

    /*p29.FURO*/ wire SPRITE0_GETn = not(SPRITE0_GET);

    // small tri - input must be inverting
    /*p30.ZETU*/ if (!SPRITE0_GETn) sil_tri.TS_IDX_0  = STORE0_IDX0;
    /*p30.ZECE*/ if (!SPRITE0_GETn) sil_tri.TS_IDX_1  = STORE0_IDX1;
    /*p30.ZAVE*/ if (!SPRITE0_GETn) sil_tri.TS_IDX_2  = STORE0_IDX2;
    /*p30.WOKO*/ if (!SPRITE0_GETn) sil_tri.TS_IDX_3  = STORE0_IDX3;
    /*p30.ZUMU*/ if (!SPRITE0_GETn) sil_tri.TS_IDX_4  = STORE0_IDX4;
    /*p30.ZEDY*/ if (!SPRITE0_GETn) sil_tri.TS_IDX_5  = STORE0_IDX5;
    /*p30.GOFO*/ if (!SPRITE0_GETn) sil_tri.TS_LINE_0 = STORE0_LINE0;
    /*p30.WEHE*/ if (!SPRITE0_GETn) sil_tri.TS_LINE_1 = STORE0_LINE1;
    /*p30.AJAL*/ if (!SPRITE0_GETn) sil_tri.TS_LINE_2 = STORE0_LINE2;
    /*p30.BUKY*/ if (!SPRITE0_GETn) sil_tri.TS_LINE_3 = STORE0_LINE3;
  }

  //----------
  // store 1

  {
    /*p31.EDYM*/ wire STORE1_MATCH0 = xor(STORE1_X0, X0n);
    /*p31.EMYB*/ wire STORE1_MATCH1 = xor(STORE1_X1, X1n);
    /*p31.EBEF*/ wire STORE1_MATCH2 = xor(STORE1_X2, X2n);
    /*p31.EWOK*/ wire STORE1_MATCH3 = xor(STORE1_X3, X3n);
    /*p31.COLA*/ wire STORE1_MATCH4 = xor(STORE1_X4, X4n);
    /*p31.BOBA*/ wire STORE1_MATCH5 = xor(STORE1_X5, X5n);
    /*p31.COLU*/ wire STORE1_MATCH6 = xor(STORE1_X6, X6n);
    /*p31.BAHU*/ wire STORE1_MATCH7 = xor(STORE1_X7, X7n);
    /*p31.CYVY*/ wire STORE1_MATCHB = nor(STORE1_MATCH4, STORE1_MATCH5, STORE1_MATCH6, STORE1_MATCH7);
    /*p31.EWAM*/ wire STORE1_MATCHA = nor(STORE1_MATCH0, STORE1_MATCH1, STORE1_MATCH2, STORE1_MATCH3);

    /*p29.DYDU*/ sst_reg.STORE1_MATCHn    = nand(MATCH_EN, STORE1_MATCHA, STORE1_MATCHB);
    /*p29.GAJA*/ wire STORE1_MATCH = not(STORE1_MATCHn);
    /*p29.FUMA*/ sst_reg.STORE1_MATCH_OUT = or(STORE1_MATCH, STORE0_MATCH_OUT);

    /*p29.ENUT*/ wire SPRITE1_GETn = nor(STORE1_MATCHn, STORE0_MATCH_OUT);

    /*p29.BYBY*/ wire STORE1_CLK  = or(STORE_EN, STORE1_SEL);
    /*p29.BUCO*/ wire STORE1_CLKn = not(STORE1_CLK);

    /*p29.CEDY*/ sst_reg.SPRITE1_GET_SYNCn.set(SPRITE_DONE, SPRITE_DONE, BYVA_VID_LINE_TRIG_d4n, SPRITE1_GET_SYNCn, SPRITE1_GETn);
    /*p29.EFEV*/ wire STORE1_RST = or(DYBA_VID_LINE_TRIG_d4, SPRITE1_GET_SYNCn);
    /*p29.DOKU*/ wire STORE1_RSTn = not(STORE1_RST);

    /*p29.BYVY*/ sst_reg.STORE1_CLKc = not(STORE1_CLKn);
    /*p30.CAJY*/ sst_reg.STORE1_IDX0 .set(STORE1_CLKc, STORE1_CLKc, 0,    STORE1_IDX0,  TS_IDX_0 );
    /*p30.CUZA*/ sst_reg.STORE1_IDX1 .set(STORE1_CLKc, STORE1_CLKc, 0,    STORE1_IDX1,  TS_IDX_1 );
    /*p30.COMA*/ sst_reg.STORE1_IDX2 .set(STORE1_CLKc, STORE1_CLKc, 0,    STORE1_IDX2,  TS_IDX_2 );
    /*p30.CUFA*/ sst_reg.STORE1_IDX3 .set(STORE1_CLKc, STORE1_CLKc, 0,    STORE1_IDX3,  TS_IDX_3 );
    /*p30.CEBO*/ sst_reg.STORE1_IDX4 .set(STORE1_CLKc, STORE1_CLKc, 0,    STORE1_IDX4,  TS_IDX_4 );
    /*p30.CADU*/ sst_reg.STORE1_IDX5 .set(STORE1_CLKc, STORE1_CLKc, 0,    STORE1_IDX5,  TS_IDX_5 );
                                                                                           
    /*p29.AHOF*/ sst_reg.STORE1_CLKb = not(STORE1_CLKn);
    /*p30.ABUG*/ sst_reg.STORE1_LINE0.set(STORE1_CLKb, STORE1_CLKb, 0,    STORE1_LINE0, TS_LINE_0);
    /*p30.AMES*/ sst_reg.STORE1_LINE1.set(STORE1_CLKb, STORE1_CLKb, 0,    STORE1_LINE1, TS_LINE_1);
    /*p30.ABOP*/ sst_reg.STORE1_LINE2.set(STORE1_CLKb, STORE1_CLKb, 0,    STORE1_LINE2, TS_LINE_2);
    /*p30.AROF*/ sst_reg.STORE1_LINE3.set(STORE1_CLKb, STORE1_CLKb, 0,    STORE1_LINE3, TS_LINE_3);

    /*p29.ASYS*/ sst_reg.STORE1_CLKa = not(STORE1_CLKn);
    /*p31.DANY*/ sst_reg.STORE1_X0   .set(STORE1_CLKa, STORE1_CLKa, STORE1_RSTn, STORE1_X0,    OAM_A_D0b);
    /*p31.DUKO*/ sst_reg.STORE1_X1   .set(STORE1_CLKa, STORE1_CLKa, STORE1_RSTn, STORE1_X1,    OAM_A_D1b);
    /*p31.DESU*/ sst_reg.STORE1_X2   .set(STORE1_CLKa, STORE1_CLKa, STORE1_RSTn, STORE1_X2,    OAM_A_D2b);
    /*p31.DAZO*/ sst_reg.STORE1_X3   .set(STORE1_CLKa, STORE1_CLKa, STORE1_RSTn, STORE1_X3,    OAM_A_D3b);
    /*p31.DAKE*/ sst_reg.STORE1_X4   .set(STORE1_CLKa, STORE1_CLKa, STORE1_RSTn, STORE1_X4,    OAM_A_D4b);
    /*p31.CESO*/ sst_reg.STORE1_X5   .set(STORE1_CLKa, STORE1_CLKa, STORE1_RSTn, STORE1_X5,    OAM_A_D5b);
    /*p31.DYFU*/ sst_reg.STORE1_X6   .set(STORE1_CLKa, STORE1_CLKa, STORE1_RSTn, STORE1_X6,    OAM_A_D6b);
    /*p31.CUSY*/ sst_reg.STORE1_X7   .set(STORE1_CLKa, STORE1_CLKa, STORE1_RSTn, STORE1_X7,    OAM_A_D7b);

    /*p29.DYDO*/ wire SPRITE1_GET = not(SPRITE1_GETn);
    /*p30.BEMO*/ if (SPRITE1_GET) sil_tri.TS_IDX_0  = not(!STORE1_IDX0);
    /*p30.CYBY*/ if (SPRITE1_GET) sil_tri.TS_IDX_1  = not(!STORE1_IDX1);
    /*p30.BETY*/ if (SPRITE1_GET) sil_tri.TS_IDX_2  = not(!STORE1_IDX2);
    /*p30.CEGY*/ if (SPRITE1_GET) sil_tri.TS_IDX_3  = not(!STORE1_IDX3);
    /*p30.CELU*/ if (SPRITE1_GET) sil_tri.TS_IDX_4  = not(!STORE1_IDX4);
    /*p30.CUBO*/ if (SPRITE1_GET) sil_tri.TS_IDX_5  = not(!STORE1_IDX5);
    /*p30.BEFE*/ if (SPRITE1_GET) sil_tri.TS_LINE_0 = not(!STORE1_LINE0);
    /*p30.BYRO*/ if (SPRITE1_GET) sil_tri.TS_LINE_1 = not(!STORE1_LINE1);
    /*p30.BACO*/ if (SPRITE1_GET) sil_tri.TS_LINE_2 = not(!STORE1_LINE2);
    /*p30.AHUM*/ if (SPRITE1_GET) sil_tri.TS_LINE_3 = not(!STORE1_LINE3);
  }

  //----------
  // store 2

  {
    /*p31.FUZU*/ wire STORE2_MATCH0 = xor(STORE2_X0, X0n);
    /*p31.FESO*/ wire STORE2_MATCH1 = xor(STORE2_X1, X1n);
    /*p31.FOKY*/ wire STORE2_MATCH2 = xor(STORE2_X2, X2n);
    /*p31.FYVA*/ wire STORE2_MATCH3 = xor(STORE2_X3, X3n);
    /*p31.CEKO*/ wire STORE2_MATCH4 = xor(STORE2_X4, X4n);
    /*p31.DETY*/ wire STORE2_MATCH5 = xor(STORE2_X5, X5n);
    /*p31.DOZO*/ wire STORE2_MATCH6 = xor(STORE2_X6, X6n);
    /*p31.CONY*/ wire STORE2_MATCH7 = xor(STORE2_X7, X7n);
    /*p31.CEHU*/ wire STORE2_MATCHA = nor(STORE2_MATCH4, STORE2_MATCH5, STORE2_MATCH6, STORE2_MATCH7);
    /*p31.EKES*/ wire STORE2_MATCHB = nor(STORE2_MATCH0, STORE2_MATCH1, STORE2_MATCH2, STORE2_MATCH3);

    /*p29.DEGO*/ sst_reg.STORE2_MATCHn = nand(MATCH_EN, STORE2_MATCHB, STORE2_MATCHA);
    /*p29.GUPO*/ wire STORE2_MATCH = not(STORE2_MATCHn);
    /*p29.GEDE*/ sst_reg.STORE2_MATCH_OUT = or(STORE2_MATCH, STORE1_MATCH_OUT);

    /*p29.EMOL*/ wire SPRITE2_GETn = nor(sst_reg.STORE2_MATCHn, STORE1_MATCH_OUT);
    /*p29.EGAV*/ sst_reg.SPRITE2_GET_SYNCn.set(SPRITE_DONE, SPRITE_DONE, BYVA_VID_LINE_TRIG_d4n, SPRITE2_GET_SYNCn, SPRITE2_GETn);

    /*p29.WYXO*/ wire STORE2_CLK = or(STORE_EN, STORE2_SEL); // 0010
    /*p29.GYFO*/ wire STORE2_CLKn = not(STORE2_CLK); // 0010
    /*p29.FOKO*/ wire STORE2_RST  = or(DYBA_VID_LINE_TRIG_d4, SPRITE2_GET_SYNCn);
    /*p29.GAMY*/ wire STORE2_RSTn = not(STORE2_RST);

    /*p29.BUZY*/ sst_reg.STORE2_CLKb = not(STORE2_CLKn);
    /*p30.BOXA*/ sst_reg.STORE2_IDX0.set(STORE2_CLKb, STORE2_CLKb, 0, STORE2_IDX0, TS_IDX_0 );
    /*p30.BUNA*/ sst_reg.STORE2_IDX1.set(STORE2_CLKb, STORE2_CLKb, 0, STORE2_IDX1, TS_IDX_1 );
    /*p30.BULU*/ sst_reg.STORE2_IDX2.set(STORE2_CLKb, STORE2_CLKb, 0, STORE2_IDX2, TS_IDX_2 );
    /*p30.BECA*/ sst_reg.STORE2_IDX3.set(STORE2_CLKb, STORE2_CLKb, 0, STORE2_IDX3, TS_IDX_3 );
    /*p30.BYHU*/ sst_reg.STORE2_IDX4.set(STORE2_CLKb, STORE2_CLKb, 0, STORE2_IDX4, TS_IDX_4 );
    /*p30.BUHE*/ sst_reg.STORE2_IDX5.set(STORE2_CLKb, STORE2_CLKb, 0, STORE2_IDX5, TS_IDX_5 );

    /*p29.FUKE*/ sst_reg.STORE2_CLKc = not(STORE2_CLKn);
    /*p30.YKUK*/ sst_reg.STORE2_LINE0.set(STORE2_CLKc, STORE2_CLKc, 0, STORE2_LINE0, TS_LINE_0);
    /*p30.YLOV*/ sst_reg.STORE2_LINE1.set(STORE2_CLKc, STORE2_CLKc, 0, STORE2_LINE1, TS_LINE_1);
    /*p30.XAZY*/ sst_reg.STORE2_LINE2.set(STORE2_CLKc, STORE2_CLKc, 0, STORE2_LINE2, TS_LINE_2);
    /*p30.XOSY*/ sst_reg.STORE2_LINE3.set(STORE2_CLKc, STORE2_CLKc, 0, STORE2_LINE3, TS_LINE_3);

    /*p29.CACU*/ sst_reg.STORE2_CLKa = not(STORE2_CLKn);
    /*p31.FOKA*/ sst_reg.STORE2_X0.set(STORE2_CLKa, STORE2_CLKa, STORE2_RSTn, STORE2_X0, OAM_A_D0b);
    /*p31.FYTY*/ sst_reg.STORE2_X1.set(STORE2_CLKa, STORE2_CLKa, STORE2_RSTn, STORE2_X1, OAM_A_D1b);
    /*p31.FUBY*/ sst_reg.STORE2_X2.set(STORE2_CLKa, STORE2_CLKa, STORE2_RSTn, STORE2_X2, OAM_A_D2b);
    /*p31.GOXU*/ sst_reg.STORE2_X3.set(STORE2_CLKa, STORE2_CLKa, STORE2_RSTn, STORE2_X3, OAM_A_D3b);
    /*p31.DUHY*/ sst_reg.STORE2_X4.set(STORE2_CLKa, STORE2_CLKa, STORE2_RSTn, STORE2_X4, OAM_A_D4b);
    /*p31.EJUF*/ sst_reg.STORE2_X5.set(STORE2_CLKa, STORE2_CLKa, STORE2_RSTn, STORE2_X5, OAM_A_D5b);
    /*p31.ENOR*/ sst_reg.STORE2_X6.set(STORE2_CLKa, STORE2_CLKa, STORE2_RSTn, STORE2_X6, OAM_A_D6b);
    /*p31.DEPY*/ sst_reg.STORE2_X7.set(STORE2_CLKa, STORE2_CLKa, STORE2_RSTn, STORE2_X7, OAM_A_D7b);


    /*p29.FAME*/ wire SPRITE2_GET = not(SPRITE2_GETn);
    /*p30.AXEC*/ if (SPRITE2_GET) sil_tri.TS_IDX_0  = not(!STORE2_IDX0);
    /*p30.CYRO*/ if (SPRITE2_GET) sil_tri.TS_IDX_1  = not(!STORE2_IDX1);
    /*p30.CUVU*/ if (SPRITE2_GET) sil_tri.TS_IDX_2  = not(!STORE2_IDX2);
    /*p30.APON*/ if (SPRITE2_GET) sil_tri.TS_IDX_3  = not(!STORE2_IDX3);
    /*p30.AFOZ*/ if (SPRITE2_GET) sil_tri.TS_IDX_4  = not(!STORE2_IDX4);
    /*p30.CUBE*/ if (SPRITE2_GET) sil_tri.TS_IDX_5  = not(!STORE2_IDX5);
    /*p30.ZABY*/ if (SPRITE2_GET) sil_tri.TS_LINE_0 = not(!STORE2_LINE0);
    /*p30.ZUKE*/ if (SPRITE2_GET) sil_tri.TS_LINE_1 = not(!STORE2_LINE1);
    /*p30.WUXE*/ if (SPRITE2_GET) sil_tri.TS_LINE_2 = not(!STORE2_LINE2);
    /*p30.WERE*/ if (SPRITE2_GET) sil_tri.TS_LINE_3 = not(!STORE2_LINE3);
  }

  // store 3
  {
    /*p31.YHOK*/ wire STORE3_MATCH0 = xor(XOLY, X0n);
    /*p31.YCAH*/ wire STORE3_MATCH1 = xor(XYBA, X1n);
    /*p31.YDAJ*/ wire STORE3_MATCH2 = xor(XABE, X2n);
    /*p31.YVUZ*/ wire STORE3_MATCH3 = xor(XEKA, X3n);
    /*p31.YVAP*/ wire STORE3_MATCH4 = xor(XOMY, X4n);
    /*p31.XENY*/ wire STORE3_MATCH5 = xor(WUHA, X5n);
    /*p31.XAVU*/ wire STORE3_MATCH6 = xor(WYNA, X6n);
    /*p31.XEVA*/ wire STORE3_MATCH7 = xor(WECO, X7n);
    /*p31.ZURE*/ wire STORE3_MATCHA = nor(STORE3_MATCH0, STORE3_MATCH1, STORE3_MATCH2, STORE3_MATCH3);
    /*p31.YWOS*/ wire STORE3_MATCHB = nor(STORE3_MATCH4, STORE3_MATCH5, STORE3_MATCH6, STORE3_MATCH7);

    /*p29.YLOZ*/ wire STORE3_MATCHn = nand(MATCH_EN, STORE3_MATCHA, STORE3_MATCHB);
    /*p29.WEBO*/ wire STORE3_MATCH = not(STORE3_MATCHn);
    /*p29.WUTO*/ wire STORE3_MATCH_OUT = or(STORE3_MATCH, STORE2_MATCH_OUT);

    /*p29.GYFY*/ wire SPRITE3_GETn = nor(STORE3_MATCHn, STORE2_MATCH_OUT);
    /*p29.GOTA*/ sst_reg.SPRITE3_GET_SYNCn.set(SPRITE_DONE, SPRITE_DONE, BYVA_VID_LINE_TRIG_d4n, SPRITE3_GET_SYNCn, SPRITE3_GETn);

    /*p29.GAKE*/ wire STORE3_RST = or(DYBA_VID_LINE_TRIG_d4, SPRITE3_GET_SYNCn);
    /*p29.WUPA*/ wire STORE3_RSTn = not(STORE3_RST);

    /*p29.GUVE*/ wire STORE3_CLK = or(STORE_EN, STORE3_SEL); // 0011
    /*p29.GUSA*/ wire STORE3_CLKn = not(STORE3_CLK);
    
    /*p29.FEKA*/ sst_reg.FEKA = not(STORE3_CLKn);
    /*p30.DAFU*/ sst_reg.DAFU.set(FEKA, FEKA, 0,          DAFU, TS_IDX_0 );
    /*p30.DEBA*/ sst_reg.DEBA.set(FEKA, FEKA, 0,          DEBA, TS_IDX_1 );
    /*p30.DUHA*/ sst_reg.DUHA.set(FEKA, FEKA, 0,          DUHA, TS_IDX_2 );
    /*p30.DUNY*/ sst_reg.DUNY.set(FEKA, FEKA, 0,          DUNY, TS_IDX_3 );
    /*p30.DESE*/ sst_reg.DESE.set(FEKA, FEKA, 0,          DESE, TS_IDX_4 );
    /*p30.DEVY*/ sst_reg.DEVY.set(FEKA, FEKA, 0,          DEVY, TS_IDX_5 );

    /*p29.XYHA*/ sst_reg.XYHA = not(STORE3_CLKn);
    /*p30.ZURY*/ sst_reg.ZURY.set(XYHA, XYHA, 0,          ZURY, TS_LINE_0);
    /*p30.ZURO*/ sst_reg.ZURO.set(XYHA, XYHA, 0,          ZURO, TS_LINE_1);
    /*p30.ZENE*/ sst_reg.ZENE.set(XYHA, XYHA, 0,          ZENE, TS_LINE_2);
    /*p30.ZYLU*/ sst_reg.ZYLU.set(XYHA, XYHA, 0,          ZYLU, TS_LINE_3);

    /*p29.YFAG*/ sst_reg.YFAG = not(STORE3_CLKn);
    /*p31.XOLY*/ sst_reg.XOLY.set(YFAG, YFAG, STORE3_RSTn, XOLY, OAM_A_D0b);
    /*p31.XYBA*/ sst_reg.XYBA.set(YFAG, YFAG, STORE3_RSTn, XYBA, OAM_A_D1b);
    /*p31.XABE*/ sst_reg.XABE.set(YFAG, YFAG, STORE3_RSTn, XABE, OAM_A_D2b);
    /*p31.XEKA*/ sst_reg.XEKA.set(YFAG, YFAG, STORE3_RSTn, XEKA, OAM_A_D3b);
    /*p31.XOMY*/ sst_reg.XOMY.set(YFAG, YFAG, STORE3_RSTn, XOMY, OAM_A_D4b);
    /*p31.WUHA*/ sst_reg.WUHA.set(YFAG, YFAG, STORE3_RSTn, WUHA, OAM_A_D5b);
    /*p31.WYNA*/ sst_reg.WYNA.set(YFAG, YFAG, STORE3_RSTn, WYNA, OAM_A_D6b);
    /*p31.WECO*/ sst_reg.WECO.set(YFAG, YFAG, STORE3_RSTn, WECO, OAM_A_D7b);

    /*p29.GYMA*/ wire SPRITE3_GET = not(SPRITE3_GETn);
    /*p30.DEZU*/ if (SPRITE3_GET) sil_tri.TS_IDX_0  = not(!DAFU);
    /*p30.EFUD*/ if (SPRITE3_GET) sil_tri.TS_IDX_1  = not(!DEBA);
    /*p30.DONY*/ if (SPRITE3_GET) sil_tri.TS_IDX_2  = not(!DUHA);
    /*p30.DOWA*/ if (SPRITE3_GET) sil_tri.TS_IDX_3  = not(!DUNY);
    /*p30.DYGO*/ if (SPRITE3_GET) sil_tri.TS_IDX_4  = not(!DESE);
    /*p30.ENAP*/ if (SPRITE3_GET) sil_tri.TS_IDX_5  = not(!DEVY);
    /*p30.ZYPO*/ if (SPRITE3_GET) sil_tri.TS_LINE_0 = not(!ZURY);
    /*p30.ZEXE*/ if (SPRITE3_GET) sil_tri.TS_LINE_1 = not(!ZURO);
    /*p30.YJEM*/ if (SPRITE3_GET) sil_tri.TS_LINE_2 = not(!ZENE);
    /*p30.YWAV*/ if (SPRITE3_GET) sil_tri.TS_LINE_3 = not(!ZYLU);
  }

  // store 4
  {
    /*p31.ZYKU*/ wire ZYKU = xor(YBED, X4n);
    /*p31.ZYPU*/ wire ZYPU = xor(ZALA, X5n);
    /*p31.XAHA*/ wire XAHA = xor(WYDE, X6n);
    /*p31.ZEFE*/ wire ZEFE = xor(XEPA, X7n);
    /*p31.XEJU*/ wire XEJU = xor(WEDU, X0n);
    /*p31.ZATE*/ wire ZATE = xor(YGAJ, X1n);
    /*p31.ZAKU*/ wire ZAKU = xor(ZYJO, X2n);
    /*p31.YBOX*/ wire YBOX = xor(XURY, X3n);
    /*p31.YKOK*/ wire YKOK = nor(ZYKU, ZYPU, XAHA, ZEFE);
    /*p31.YNAZ*/ wire YNAZ = nor(XEJU, ZATE, ZAKU, YBOX);
    /*p29.XAGE*/ sst_reg.STORE4_MATCHn = nand(MATCH_EN, YNAZ, YKOK);
    /*p29.WUNA*/ wire STORE4_MATCH = not(STORE4_MATCHn);

    /*p29.XUDY*/ sst_reg.SPRITE4_GET_SYNCn.set(SPRITE_DONE, SPRITE_DONE, BYVA_VID_LINE_TRIG_d4n, SPRITE4_GET_SYNCn, GONO);
    /*p29.WOHU*/ sst_reg.WOHU = or(DYBA_VID_LINE_TRIG_d4, SPRITE4_GET_SYNCn);
    /*p29.XYLA*/ sst_reg.STORE4_MATCH_OUT = or(STORE4_MATCH, STORE3_MATCH_OUT);

    /*p29.CECU*/ wire STORE4_CLK = or(STORE_EN, STORE4_SEL); // 0100
    /*p29.DUKE*/ sst_reg.DUKE = not(STORE4_CLK);
    
    // i think these are swapped
    /*p29.WUNU*/ sst_reg.WUNU = not(WOHU);
    /*p29.WOFO*/ sst_reg.WOFO = not(DUKE);
    /*p31.WEDU*/ sst_reg.WEDU.set(WUNU, WUNU, WOFO, WEDU, OAM_A_D0b);
    /*p31.YGAJ*/ sst_reg.YGAJ.set(WUNU, WUNU, WOFO, YGAJ, OAM_A_D1b);
    /*p31.ZYJO*/ sst_reg.ZYJO.set(WUNU, WUNU, WOFO, ZYJO, OAM_A_D2b);
    /*p31.XURY*/ sst_reg.XURY.set(WUNU, WUNU, WOFO, XURY, OAM_A_D3b);
    /*p31.YBED*/ sst_reg.YBED.set(WUNU, WUNU, WOFO, YBED, OAM_A_D4b);
    /*p31.ZALA*/ sst_reg.ZALA.set(WUNU, WUNU, WOFO, ZALA, OAM_A_D5b);
    /*p31.WYDE*/ sst_reg.WYDE.set(WUNU, WUNU, WOFO, WYDE, OAM_A_D6b);
    /*p31.XEPA*/ sst_reg.XEPA.set(WUNU, WUNU, WOFO, XEPA, OAM_A_D7b);

    /*p29.WYLU*/ sst_reg.WYLU = not(DUKE);
    /*p30.XYNU*/ sst_reg.XYNU.set(WYLU, WYLU, 0, XYNU, TS_IDX_0 );
    /*p30.XEGE*/ sst_reg.XEGE.set(WYLU, WYLU, 0, XEGE, TS_IDX_1 );
    /*p30.XABO*/ sst_reg.XABO.set(WYLU, WYLU, 0, XABO, TS_IDX_2 );
    /*p30.WANU*/ sst_reg.WANU.set(WYLU, WYLU, 0, WANU, TS_IDX_3 );
    /*p30.XEFE*/ sst_reg.XEFE.set(WYLU, WYLU, 0, XEFE, TS_IDX_4 );
    /*p30.XAVE*/ sst_reg.XAVE.set(WYLU, WYLU, 0, XAVE, TS_IDX_5 );

    /*p29.EWOT*/ sst_reg.EWOT = not(DUKE);
    /*p30.CUMU*/ sst_reg.CUMU.set(EWOT, EWOT, 0, CUMU, TS_LINE_0);
    /*p30.CAPO*/ sst_reg.CAPO.set(EWOT, EWOT, 0, CAPO, TS_LINE_1);
    /*p30.CONO*/ sst_reg.CONO.set(EWOT, EWOT, 0, CONO, TS_LINE_2);
    /*p30.CAJU*/ sst_reg.CAJU.set(EWOT, EWOT, 0, CAJU, TS_LINE_3);

    /*p29.GONO*/ sst_reg.GONO = nor(STORE4_MATCHn, STORE3_MATCH_OUT);

    /*p29.GOWO*/ sst_reg.GOWO = not(GONO);
    /*p30.WAJA*/ if (GOWO) sil_tri.TS_IDX_0  = not(!XYNU);
    /*p30.WOXY*/ if (GOWO) sil_tri.TS_IDX_1  = not(!XEGE);
    /*p30.XYRE*/ if (GOWO) sil_tri.TS_IDX_2  = not(!XABO);
    /*p30.WERU*/ if (GOWO) sil_tri.TS_IDX_3  = not(!WANU);
    /*p30.WEPY*/ if (GOWO) sil_tri.TS_IDX_4  = not(!XEFE);
    /*p30.WUXU*/ if (GOWO) sil_tri.TS_IDX_5  = not(!XAVE);
    /*p30.BYDO*/ if (GOWO) sil_tri.TS_LINE_0 = not(!CUMU);
    /*p30.BUCE*/ if (GOWO) sil_tri.TS_LINE_1 = not(!CAPO);
    /*p30.BOVE*/ if (GOWO) sil_tri.TS_LINE_2 = not(!CONO);
    /*p30.BEVY*/ if (GOWO) sil_tri.TS_LINE_3 = not(!CAJU);
  }

  {
  // store 5
  /*p31.BAZY*/ wire BAZY = xor(CYWE, X4n);
  /*p31.CYLE*/ wire CYLE = xor(DYBY, X5n);
  /*p31.CEVA*/ wire CEVA = xor(DURY, X6n);
  /*p31.BUMY*/ wire BUMY = xor(CUVY, X7n);
  /*p31.GUZO*/ wire GUZO = xor(FUSA, X0n);
  /*p31.GOLA*/ wire GOLA = xor(FAXA, X1n);
  /*p31.GEVE*/ wire GEVE = xor(FOZY, X2n);
  /*p31.GUDE*/ wire GUDE = xor(FESY, X3n);
  /*p31.COGY*/ wire COGY = nor(BAZY, CYLE, CEVA, BUMY);
  /*p31.FYMA*/ wire FYMA = nor(GUZO, GOLA, GEVE, GUDE);
  /*p29.EGOM*/ sst_reg.STORE5_MATCHn = nand(MATCH_EN, FYMA, COGY);

  /*p29.GEGA*/ sst_reg.GEGA = nor(STORE5_MATCHn, STORE4_MATCH_OUT);
  /*p29.WAFY*/ sst_reg.SPRITE5_GET_SYNCn.set(SPRITE_DONE, SPRITE_DONE, BYVA_VID_LINE_TRIG_d4n, SPRITE5_GET_SYNCn, GEGA);
  /*p29.FEVE*/ sst_reg.FEVE = or(DYBA_VID_LINE_TRIG_d4, SPRITE5_GET_SYNCn);
  /*p29.EJAD*/ sst_reg.EJAD = not(FEVE);
  /*p29.GABA*/ wire STORE5_MATCH = not(STORE5_MATCHn);
    
  /*p29.WEJA*/ sst_reg.STORE5_MATCH_OUT = or(STORE5_MATCH, STORE4_MATCH_OUT);

  /*p29.CADO*/ wire STORE5_CLK = or(STORE_EN, STORE5_SEL); // 0101
  /*p29.BEDE*/ sst_reg.BEDE = not(STORE5_CLK);

  /*p29.CYLA*/ sst_reg.CYLA = not(BEDE);
  /*p31.FUSA*/ sst_reg.FUSA.set(CYLA, CYLA, EJAD, FUSA, OAM_A_D0b);
  /*p31.FAXA*/ sst_reg.FAXA.set(CYLA, CYLA, EJAD, FAXA, OAM_A_D1b);
  /*p31.FOZY*/ sst_reg.FOZY.set(CYLA, CYLA, EJAD, FOZY, OAM_A_D2b);
  /*p31.FESY*/ sst_reg.FESY.set(CYLA, CYLA, EJAD, FESY, OAM_A_D3b);
  /*p31.CYWE*/ sst_reg.CYWE.set(CYLA, CYLA, EJAD, CYWE, OAM_A_D4b);
  /*p31.DYBY*/ sst_reg.DYBY.set(CYLA, CYLA, EJAD, DYBY, OAM_A_D5b);
  /*p31.DURY*/ sst_reg.DURY.set(CYLA, CYLA, EJAD, DURY, OAM_A_D6b);
  /*p31.CUVY*/ sst_reg.CUVY.set(CYLA, CYLA, EJAD, CUVY, OAM_A_D7b);

  /*p29.DYMO*/ sst_reg.DYMO = not(BEDE);
  /*p30.EKAP*/ sst_reg.EKAP.set(DYMO, DYMO, 0, EKAP, TS_IDX_0 );
  /*p30.ETAV*/ sst_reg.ETAV.set(DYMO, DYMO, 0, ETAV, TS_IDX_1 );
  /*p30.EBEX*/ sst_reg.EBEX.set(DYMO, DYMO, 0, EBEX, TS_IDX_2 );
  /*p30.GORU*/ sst_reg.GORU.set(DYMO, DYMO, 0, GORU, TS_IDX_3 );
  /*p30.ETYM*/ sst_reg.ETYM.set(DYMO, DYMO, 0, ETYM, TS_IDX_4 );
  /*p30.EKOP*/ sst_reg.EKOP.set(DYMO, DYMO, 0, EKOP, TS_IDX_5 );

  /*p29.BUCY*/ sst_reg.BUCY = not(BEDE);
  /*p30.ANED*/ sst_reg.ANED.set(BUCY, BUCY, 0, ANED, TS_LINE_0);
  /*p30.ACEP*/ sst_reg.ACEP.set(BUCY, BUCY, 0, ACEP, TS_LINE_1);
  /*p30.ABUX*/ sst_reg.ABUX.set(BUCY, BUCY, 0, ABUX, TS_LINE_2);
  /*p30.ABEG*/ sst_reg.ABEG.set(BUCY, BUCY, 0, ABEG, TS_LINE_3);

  /*p29.GYGY*/ sst_reg.GYGY = not(GEGA);
  /*p30.DALO*/ if (GYGY) sil_tri.TS_IDX_0  = not(!EKAP);
  /*p30.DALY*/ if (GYGY) sil_tri.TS_IDX_1  = not(!ETAV);
  /*p30.DUZA*/ if (GYGY) sil_tri.TS_IDX_2  = not(!EBEX);
  /*p30.WAGA*/ if (GYGY) sil_tri.TS_IDX_3  = not(!GORU);
  /*p30.DYNY*/ if (GYGY) sil_tri.TS_IDX_4  = not(!ETYM);
  /*p30.DOBO*/ if (GYGY) sil_tri.TS_IDX_5  = not(!EKOP);
  /*p30.AWAT*/ if (GYGY) sil_tri.TS_LINE_0 = not(!ANED);
  /*p30.BACE*/ if (GYGY) sil_tri.TS_LINE_1 = not(!ACEP);
  /*p30.BODU*/ if (GYGY) sil_tri.TS_LINE_2 = not(!ABUX);
  /*p30.BUJA*/ if (GYGY) sil_tri.TS_LINE_3 = not(!ABEG);

  }

  {
  // sprite store 6
  /*p31.ZARE*/ wire ZARE = xor(ZOLY, X4n);
  /*p31.ZEMU*/ wire ZEMU = xor(ZOGO, X5n);
  /*p31.ZYGO*/ wire ZYGO = xor(ZECU, X6n);
  /*p31.ZUZY*/ wire ZUZY = xor(ZESA, X7n);
  /*p31.XOSU*/ wire XOSU = xor(YCOL, X0n);
  /*p31.ZUVU*/ wire ZUVU = xor(YRAC, X1n);
  /*p31.XUCO*/ wire XUCO = xor(YMEM, X2n);
  /*p31.ZULO*/ wire ZULO = xor(YVAG, X3n);
  /*p31.YWAP*/ wire YWAP = nor(ZARE, ZEMU, ZYGO, ZUZY);
  /*p31.YDOT*/ wire YDOT = nor(XOSU, ZUVU, XUCO, ZULO);
  /*p29.YBEZ*/ sst_reg.STORE6_MATCHn = nand(MATCH_EN, YDOT, YWAP);

  /*p29.XOJA*/ sst_reg.XOJA = nor(STORE6_MATCHn, STORE5_MATCH_OUT);
  /*p29.WOMY*/ sst_reg.SPRITE6_GET_SYNCn.set(SPRITE_DONE, SPRITE_DONE, BYVA_VID_LINE_TRIG_d4n, SPRITE6_GET_SYNCn, XOJA);
  /*p29.WACY*/ sst_reg.WACY = or(DYBA_VID_LINE_TRIG_d4, SPRITE6_GET_SYNCn);
  /*p29.XAHO*/ sst_reg.XAHO = not(WACY);
  /*p29.WASE*/ wire STORE6_MATCH = not(STORE6_MATCHn);
  /*p29.WYLA*/ sst_reg.STORE6_MATCH_OUT = or(STORE6_MATCH, STORE5_MATCH_OUT);

  /*p29.XUJO*/ wire STORE6_CLK = or(STORE_EN, STORE6_SEL); // 0110
  /*p29.WEKA*/ sst_reg.WEKA = not(STORE6_CLK);

  /*p29.ZAPE*/ sst_reg.ZAPE = not(WEKA);
  /*p31.YCOL*/ sst_reg.YCOL.set(ZAPE, ZAPE, XAHO, YCOL, OAM_A_D0b);
  /*p31.YRAC*/ sst_reg.YRAC.set(ZAPE, ZAPE, XAHO, YRAC, OAM_A_D1b);
  /*p31.YMEM*/ sst_reg.YMEM.set(ZAPE, ZAPE, XAHO, YMEM, OAM_A_D2b);
  /*p31.YVAG*/ sst_reg.YVAG.set(ZAPE, ZAPE, XAHO, YVAG, OAM_A_D3b);
  /*p31.ZOLY*/ sst_reg.ZOLY.set(ZAPE, ZAPE, XAHO, ZOLY, OAM_A_D4b);
  /*p31.ZOGO*/ sst_reg.ZOGO.set(ZAPE, ZAPE, XAHO, ZOGO, OAM_A_D5b);
  /*p31.ZECU*/ sst_reg.ZECU.set(ZAPE, ZAPE, XAHO, ZECU, OAM_A_D6b);
  /*p31.ZESA*/ sst_reg.ZESA.set(ZAPE, ZAPE, XAHO, ZESA, OAM_A_D7b);

  /*p29.WUSE*/ sst_reg.WUSE = not(WEKA);
  /*p30.GECU*/ sst_reg.GECU.set(WUSE, WUSE, 0, GECU, TS_IDX_0 );
  /*p30.FOXY*/ sst_reg.FOXY.set(WUSE, WUSE, 0, FOXY, TS_IDX_1 );
  /*p30.GOHU*/ sst_reg.GOHU.set(WUSE, WUSE, 0, GOHU, TS_IDX_2 );
  /*p30.FOGO*/ sst_reg.FOGO.set(WUSE, WUSE, 0, FOGO, TS_IDX_3 );
  /*p30.GACY*/ sst_reg.GACY.set(WUSE, WUSE, 0, GACY, TS_IDX_4 );
  /*p30.GABO*/ sst_reg.GABO.set(WUSE, WUSE, 0, GABO, TS_IDX_5 );
    
  /*p29.ZURU*/ sst_reg.ZURU = not(WEKA);
  /*p30.ZUBE*/ sst_reg.ZUBE.set(ZURU, ZURU, 0, ZUBE, TS_LINE_0);
  /*p30.ZUMY*/ sst_reg.ZUMY.set(ZURU, ZURU, 0, ZUMY, TS_LINE_1);
  /*p30.ZEXO*/ sst_reg.ZEXO.set(ZURU, ZURU, 0, ZEXO, TS_LINE_2);
  /*p30.ZAFU*/ sst_reg.ZAFU.set(ZURU, ZURU, 0, ZAFU, TS_LINE_3);

  /*p29.XYME*/ sst_reg.XYME = not(XOJA);
  /*p30.WOCY*/ if (XYME) sil_tri.TS_IDX_0  = not(!GECU);
  /*p30.ELYC*/ if (XYME) sil_tri.TS_IDX_1  = not(!FOXY);
  /*p30.WABO*/ if (XYME) sil_tri.TS_IDX_2  = not(!GOHU);
  /*p30.EZOC*/ if (XYME) sil_tri.TS_IDX_3  = not(!FOGO);
  /*p30.WYWY*/ if (XYME) sil_tri.TS_IDX_4  = not(!GACY);
  /*p30.WATO*/ if (XYME) sil_tri.TS_IDX_5  = not(!GABO);
  /*p30.ZUDO*/ if (XYME) sil_tri.TS_LINE_0 = not(!ZUBE);
  /*p30.YBUK*/ if (XYME) sil_tri.TS_LINE_1 = not(!ZUMY);
  /*p30.ZYTO*/ if (XYME) sil_tri.TS_LINE_2 = not(!ZEXO);
  /*p30.YKOZ*/ if (XYME) sil_tri.TS_LINE_3 = not(!ZAFU);
  }

  {
  // sprite store 7
  /*p31.DUSE*/ wire MATCH0 = xor(STORE7_X0, X0n);
  /*p31.DAGU*/ wire MATCH1 = xor(STORE7_X1, X1n);
  /*p31.DYZE*/ wire MATCH2 = xor(STORE7_X2, X2n);
  /*p31.DESO*/ wire MATCH3 = xor(STORE7_X3, X3n);
  /*p31.EJOT*/ wire MATCH4 = xor(STORE7_X4, X4n);
  /*p31.ESAJ*/ wire MATCH5 = xor(STORE7_X5, X5n);
  /*p31.DUCU*/ wire MATCH6 = xor(STORE7_X6, X6n);
  /*p31.EWUD*/ wire MATCH7 = xor(STORE7_X7, X7n);
  /*p31.CYCO*/ wire MATCHA = nor(MATCH0, MATCH1, MATCH2, MATCH3);
  /*p31.DAJE*/ wire MATCHB = nor(MATCH4, MATCH5, MATCH6, MATCH7);
  /*p29.DYKA*/ sst_reg.STORE7_MATCHn = nand(MATCH_EN, MATCHA, MATCHB);
  /*p29.GYTE*/ wire STORE7_MATCH = not(STORE7_MATCHn);
  /*p29.FAVO*/ sst_reg.STORE7_MATCH_OUT = or(STORE7_MATCH, STORE6_MATCH_OUT);

  /*p29.GUTU*/ wire STORE7_RD = nor(STORE7_MATCHn, STORE6_MATCH_OUT);
  /*p29.WAPO*/ sst_reg.SPRITE7_GET_SYNCn.set(SPRITE_DONE, SPRITE_DONE, BYVA_VID_LINE_TRIG_d4n, SPRITE7_GET_SYNCn, STORE7_RD);
  /*p29.GUKY*/ wire STORE7_RST = or(DYBA_VID_LINE_TRIG_d4, SPRITE7_GET_SYNCn);
  /*p29.GAFY*/ wire STORE7_RSTn = not(STORE7_RST);

  /*p29.GAPE*/ wire STORE7_CLK = or(STORE_EN, STORE7_SEL); // 0111
  /*p29.GYVO*/ wire STORE7_CLKn = not(STORE7_CLK);

  /*p29.GECY*/ sst_reg.GECY = not(STORE7_CLKn);
  /*p31.ERAZ*/ sst_reg.STORE7_X0.set(GECY, GECY, STORE7_RSTn, STORE7_X0, OAM_A_D0b);
  /*p31.EPUM*/ sst_reg.STORE7_X1.set(GECY, GECY, STORE7_RSTn, STORE7_X1, OAM_A_D1b);
  /*p31.EROL*/ sst_reg.STORE7_X2.set(GECY, GECY, STORE7_RSTn, STORE7_X2, OAM_A_D2b);
  /*p31.EHYN*/ sst_reg.STORE7_X3.set(GECY, GECY, STORE7_RSTn, STORE7_X3, OAM_A_D3b);
  /*p31.FAZU*/ sst_reg.STORE7_X4.set(GECY, GECY, STORE7_RSTn, STORE7_X4, OAM_A_D4b);
  /*p31.FAXE*/ sst_reg.STORE7_X5.set(GECY, GECY, STORE7_RSTn, STORE7_X5, OAM_A_D5b);
  /*p31.EXUK*/ sst_reg.STORE7_X6.set(GECY, GECY, STORE7_RSTn, STORE7_X6, OAM_A_D6b);
  /*p31.FEDE*/ sst_reg.STORE7_X7.set(GECY, GECY, STORE7_RSTn, STORE7_X7, OAM_A_D7b);

  /*p29.FEFO*/ sst_reg.FEFO = not(STORE7_CLKn);
  /*p30.FUZO*/ sst_reg.FUZO.set(FEFO, FEFO, 0, FUZO, TS_IDX_0 );
  /*p30.GESY*/ sst_reg.GESY.set(FEFO, FEFO, 0, GESY, TS_IDX_1 );
  /*p30.FYSU*/ sst_reg.FYSU.set(FEFO, FEFO, 0, FYSU, TS_IDX_2 );
  /*p30.FEFA*/ sst_reg.FEFA.set(FEFO, FEFO, 0, FEFA, TS_IDX_3 );
  /*p30.GYNO*/ sst_reg.GYNO.set(FEFO, FEFO, 0, GYNO, TS_IDX_4 );
  /*p30.GULE*/ sst_reg.GULE.set(FEFO, FEFO, 0, GULE, TS_IDX_5 );

  /*p29.WABE*/ sst_reg.WABE = not(STORE7_CLKn);
  /*p30.XYGO*/ sst_reg.XYGO.set(WABE, WABE, 0, XYGO, TS_LINE_0);
  /*p30.XYNA*/ sst_reg.XYNA.set(WABE, WABE, 0, XYNA, TS_LINE_1);
  /*p30.XAKU*/ sst_reg.XAKU.set(WABE, WABE, 0, XAKU, TS_LINE_2);
  /*p30.YGUM*/ sst_reg.YGUM.set(WABE, WABE, 0, YGUM, TS_LINE_3);

  /*p29.GUGY*/ wire STORE7_RDn = not(STORE7_RD);
  /*p30.EVYT*/ if (!STORE7_RDn) sil_tri.TS_IDX_0  = not(!FUZO);
  /*p30.WABA*/ if (!STORE7_RDn) sil_tri.TS_IDX_1  = not(!GESY);
  /*p30.ETAD*/ if (!STORE7_RDn) sil_tri.TS_IDX_2  = not(!FYSU);
  /*p30.ELEP*/ if (!STORE7_RDn) sil_tri.TS_IDX_3  = not(!FEFA);
  /*p30.WYGO*/ if (!STORE7_RDn) sil_tri.TS_IDX_4  = not(!GYNO);
  /*p30.WAKO*/ if (!STORE7_RDn) sil_tri.TS_IDX_5  = not(!GULE);
  /*p30.WANA*/ if (!STORE7_RDn) sil_tri.TS_LINE_0 = not(!XYGO);
  /*p30.WAXE*/ if (!STORE7_RDn) sil_tri.TS_LINE_1 = not(!XYNA);
  /*p30.WABU*/ if (!STORE7_RDn) sil_tri.TS_LINE_2 = not(!XAKU);
  /*p30.YPOZ*/ if (!STORE7_RDn) sil_tri.TS_LINE_3 = not(!YGUM);

  }


  {
  // sprite store 8
  /*p31.DUZE*/ wire DUZE = xor(EZUF, X4n);
  /*p31.DAGA*/ wire DAGA = xor(ENAD, X5n);
  /*p31.DAWU*/ wire DAWU = xor(EBOW, X6n);
  /*p31.EJAW*/ wire EJAW = xor(FYCA, X7n);
  /*p31.GOHO*/ wire GOHO = xor(GAVY, X0n);
  /*p31.GASU*/ wire GASU = xor(GYPU, X1n);
  /*p31.GABU*/ wire GABU = xor(GADY, X2n);
  /*p31.GAFE*/ wire GAFE = xor(GAZA, X3n);
  /*p31.DAMA*/ wire DAMA = nor(DUZE, DAGA, DAWU, EJAW);
  /*p31.FEHA*/ wire FEHA = nor(GOHO, GASU, GABU, GAFE);
  /*p29.EFYL*/ sst_reg.STORE8_MATCHn = nand(MATCH_EN, FEHA, DAMA);
  /*p29.GEKE*/ wire STORE8_MATCH = not(STORE8_MATCHn);
  /*p29.GYGA*/ sst_reg.STORE8_MATCH_OUT = or(STORE8_MATCH, STORE7_MATCH_OUT);
  /*p29.FOXA*/ sst_reg.FOXA = nor(STORE8_MATCHn, STORE7_MATCH_OUT);

  /*p29.EXUQ*/ sst_reg.SPRITE8_GET_SYNCn.set(SPRITE_DONE, SPRITE_DONE, BYVA_VID_LINE_TRIG_d4n, SPRITE8_GET_SYNCn, FOXA);
  /*p29.GORO*/ sst_reg.GORO = or(DYBA_VID_LINE_TRIG_d4, SPRITE8_GET_SYNCn);
  /*p29.WUZO*/ sst_reg.WUZO = not(GORO);

  /*p29.CAHO*/ wire STORE8_CLK = or(STORE_EN, STORE8_SEL); // 1000
  /*p29.BUKA*/ sst_reg.BUKA = not(STORE8_CLK);
  /*p29.CEXU*/ sst_reg.CEXU = not(BUKA);
  /*p29.AKOL*/ sst_reg.AKOL = not(BUKA);
  /*p29.BYMY*/ sst_reg.BYMY = not(BUKA);
  /*p31.GAVY*/ sst_reg.GAVY.set(CEXU, CEXU, WUZO, GAVY, OAM_A_D0b);
  /*p31.GYPU*/ sst_reg.GYPU.set(CEXU, CEXU, WUZO, GYPU, OAM_A_D1b);
  /*p31.GADY*/ sst_reg.GADY.set(CEXU, CEXU, WUZO, GADY, OAM_A_D2b);
  /*p31.GAZA*/ sst_reg.GAZA.set(CEXU, CEXU, WUZO, GAZA, OAM_A_D3b);
  /*p31.EZUF*/ sst_reg.EZUF.set(CEXU, CEXU, WUZO, EZUF, OAM_A_D4b);
  /*p31.ENAD*/ sst_reg.ENAD.set(CEXU, CEXU, WUZO, ENAD, OAM_A_D5b);
  /*p31.EBOW*/ sst_reg.EBOW.set(CEXU, CEXU, WUZO, EBOW, OAM_A_D6b);
  /*p31.FYCA*/ sst_reg.FYCA.set(CEXU, CEXU, WUZO, FYCA, OAM_A_D7b);


  /*p30.AXUV*/ sst_reg.AXUV.set(AKOL, AKOL, 0, AXUV, TS_IDX_0 );
  /*p30.BADA*/ sst_reg.BADA.set(AKOL, AKOL, 0, BADA, TS_IDX_1 );
  /*p30.APEV*/ sst_reg.APEV.set(AKOL, AKOL, 0, APEV, TS_IDX_2 );
  /*p30.BADO*/ sst_reg.BADO.set(AKOL, AKOL, 0, BADO, TS_IDX_3 );
  /*p30.BEXY*/ sst_reg.BEXY.set(AKOL, AKOL, 0, BEXY, TS_IDX_4 );
  /*p30.BYHE*/ sst_reg.BYHE.set(AKOL, AKOL, 0, BYHE, TS_IDX_5 );
  /*p30.AFYM*/ sst_reg.AFYM.set(BYMY, BYMY, 0, AFYM, TS_LINE_0);
  /*p30.AZAP*/ sst_reg.AZAP.set(BYMY, BYMY, 0, AZAP, TS_LINE_1);
  /*p30.AFUT*/ sst_reg.AFUT.set(BYMY, BYMY, 0, AFUT, TS_LINE_2);
  /*p30.AFYX*/ sst_reg.AFYX.set(BYMY, BYMY, 0, AFYX, TS_LINE_3);

  /*p29.DENY*/ sst_reg.DENY = not(FOXA);
  /*p30.ADYB*/ if (DENY) sil_tri.TS_IDX_0  = AXUV;
  /*p30.APOB*/ if (DENY) sil_tri.TS_IDX_1  = BADA;
  /*p30.APYV*/ if (DENY) sil_tri.TS_IDX_2  = APEV;
  /*p30.AFEN*/ if (DENY) sil_tri.TS_IDX_3  = BADO;
  /*p30.AKYH*/ if (DENY) sil_tri.TS_IDX_4  = BEXY;
  /*p30.APOC*/ if (DENY) sil_tri.TS_IDX_5  = BYHE;
  /*p30.BUJY*/ if (DENY) sil_tri.TS_LINE_0 = AFYM;
  /*p30.BOSO*/ if (DENY) sil_tri.TS_LINE_1 = AZAP;
  /*p30.AHAC*/ if (DENY) sil_tri.TS_LINE_2 = AFUT;
  /*p30.BAZU*/ if (DENY) sil_tri.TS_LINE_3 = AFYX;
  }

  {
  // sprite store 9
  /*p31.YMAM*/ wire MATCH0 = xor(STORE9_X0, X0n);
  /*p31.YTYP*/ wire MATCH1 = xor(STORE9_X1, X1n);
  /*p31.YFOP*/ wire MATCH2 = xor(STORE9_X2, X2n);
  /*p31.YVAC*/ wire MATCH3 = xor(STORE9_X3, X3n);
  /*p31.ZYWU*/ wire MATCH4 = xor(STORE9_X4, X4n);
  /*p31.ZUZA*/ wire MATCH5 = xor(STORE9_X5, X5n);
  /*p31.ZEJO*/ wire MATCH6 = xor(STORE9_X6, X6n);
  /*p31.ZEDA*/ wire MATCH7 = xor(STORE9_X7, X7n);
  /*p31.YLEV*/ wire MATCHA = nor(MATCH0, MATCH1, MATCH2, MATCH3);
  /*p31.YTUB*/ wire MATCHB = nor(MATCH4, MATCH5, MATCH6, MATCH7);
  /*p29.YGEM*/ sst_reg.STORE9_MATCHn = nand(MATCH_EN, MATCHA, MATCHB);

  /*p29.CATO*/ wire STORE9_CLK = or(STORE_EN, STORE9_SEL); // 1001
  /*p29.DECU*/ wire STORE9_CLKn = not(STORE9_CLK);

  /*p29.GUZE*/ wire SPRITE9_GETn = nor(STORE9_MATCHn, STORE8_MATCH_OUT);
  /*p29.FONO*/ sst_reg.SPRITE9_GET_SYNCn.set(SPRITE_DONE, SPRITE_DONE, BYVA_VID_LINE_TRIG_d4n, SPRITE9_GET_SYNCn, SPRITE9_GETn);

  /*p29.DUBU*/ wire STORE9_RST  = or(DYBA_VID_LINE_TRIG_d4, SPRITE9_GET_SYNCn);
  /*p29.DOSY*/ wire STORE9_RSTn = not(STORE9_RST);
    
  /*p29.WEME*/ sst_reg.STORE9_CLKa = not(STORE9_CLKn);
  /*p31.XUVY*/ sst_reg.STORE9_X0   .set(STORE9_CLKa, STORE9_CLKa, STORE9_RSTn, STORE9_X0,    OAM_A_D0b);
  /*p31.XERE*/ sst_reg.STORE9_X1   .set(STORE9_CLKa, STORE9_CLKa, STORE9_RSTn, STORE9_X1,    OAM_A_D1b);
  /*p31.XUZO*/ sst_reg.STORE9_X2   .set(STORE9_CLKa, STORE9_CLKa, STORE9_RSTn, STORE9_X2,    OAM_A_D2b);
  /*p31.XEXA*/ sst_reg.STORE9_X3   .set(STORE9_CLKa, STORE9_CLKa, STORE9_RSTn, STORE9_X3,    OAM_A_D3b);
  /*p31.YPOD*/ sst_reg.STORE9_X4   .set(STORE9_CLKa, STORE9_CLKa, STORE9_RSTn, STORE9_X4,    OAM_A_D4b);
  /*p31.YROP*/ sst_reg.STORE9_X5   .set(STORE9_CLKa, STORE9_CLKa, STORE9_RSTn, STORE9_X5,    OAM_A_D5b);
  /*p31.YNEP*/ sst_reg.STORE9_X6   .set(STORE9_CLKa, STORE9_CLKa, STORE9_RSTn, STORE9_X6,    OAM_A_D6b);
  /*p31.YZOF*/ sst_reg.STORE9_X7   .set(STORE9_CLKa, STORE9_CLKa, STORE9_RSTn, STORE9_X7,    OAM_A_D7b);

  /*p29.WUFA*/ sst_reg.STORE9_CLKb = not(STORE9_CLKn);
  /*p30.XUFO*/ sst_reg.STORE9_IDX0 .set(STORE9_CLKb, STORE9_CLKb, 0,           STORE9_IDX0,  TS_IDX_0 );
  /*p30.XUTE*/ sst_reg.STORE9_IDX1 .set(STORE9_CLKb, STORE9_CLKb, 0,           STORE9_IDX1,  TS_IDX_1 );
  /*p30.XOTU*/ sst_reg.STORE9_IDX2 .set(STORE9_CLKb, STORE9_CLKb, 0,           STORE9_IDX2,  TS_IDX_2 );
  /*p30.XYFE*/ sst_reg.STORE9_IDX3 .set(STORE9_CLKb, STORE9_CLKb, 0,           STORE9_IDX3,  TS_IDX_3 );
  /*p30.YZOR*/ sst_reg.STORE9_IDX4 .set(STORE9_CLKb, STORE9_CLKb, 0,           STORE9_IDX4,  TS_IDX_4 );
  /*p30.YBER*/ sst_reg.STORE9_IDX5 .set(STORE9_CLKb, STORE9_CLKb, 0,           STORE9_IDX5,  TS_IDX_5 );

  /*p29.FAKA*/ sst_reg.STORE9_CLKc = not(STORE9_CLKn);
  /*p30.DEWU*/ sst_reg.STORE9_LINE0.set(STORE9_CLKc, STORE9_CLKc, 0,           STORE9_LINE0, TS_LINE_0);
  /*p30.CANA*/ sst_reg.STORE9_LINE1.set(STORE9_CLKc, STORE9_CLKc, 0,           STORE9_LINE1, TS_LINE_1);
  /*p30.DYSY*/ sst_reg.STORE9_LINE2.set(STORE9_CLKc, STORE9_CLKc, 0,           STORE9_LINE2, TS_LINE_2);
  /*p30.FOFO*/ sst_reg.STORE9_LINE3.set(STORE9_CLKc, STORE9_CLKc, 0,           STORE9_LINE3, TS_LINE_3);

  /*p29.FADO*/ wire SPRITE9_GET = not(SPRITE9_GETn);
  /*p30.YHAL*/ if (SPRITE9_GET) sil_tri.TS_IDX_0  = not(!STORE9_IDX0);
  /*p30.YRAD*/ if (SPRITE9_GET) sil_tri.TS_IDX_1  = not(!STORE9_IDX1);
  /*p30.XYRA*/ if (SPRITE9_GET) sil_tri.TS_IDX_2  = not(!STORE9_IDX2);
  /*p30.YNEV*/ if (SPRITE9_GET) sil_tri.TS_IDX_3  = not(!STORE9_IDX3);
  /*p30.ZOJY*/ if (SPRITE9_GET) sil_tri.TS_IDX_4  = not(!STORE9_IDX4);
  /*p30.ZARO*/ if (SPRITE9_GET) sil_tri.TS_IDX_5  = not(!STORE9_IDX5);
  /*p30.CAWO*/ if (SPRITE9_GET) sil_tri.TS_LINE_0 = not(!STORE9_LINE0);
  /*p30.BYME*/ if (SPRITE9_GET) sil_tri.TS_LINE_1 = not(!STORE9_LINE1);
  /*p30.COHO*/ if (SPRITE9_GET) sil_tri.TS_LINE_2 = not(!STORE9_LINE2);
  /*p30.GATE*/ if (SPRITE9_GET) sil_tri.TS_LINE_3 = not(!STORE9_LINE3);

  }
#endif

//-----------------------------------------------------------------------------
};
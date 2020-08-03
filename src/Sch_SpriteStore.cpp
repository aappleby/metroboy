#include "Sch_SpriteStore.h"

#include "Sch_Top.h"

using namespace Schematics;

void SpriteStore::dump(Dumper& d) const {
  d("----------SpriteStore---------\n");
  d("DEZY_STORE_ENn %c\n", DEZY_STORE_ENn.c());
  //d("FEPO_STORE_MATCHp %d\n", wire(FEPO_STORE_MATCHp));

  d("SPRITE INDEX %02d\n", 
    pack(
      XADU_SPRITE_INDX0.q(),
      XEDY_SPRITE_INDX1.q(),
      ZUZE_SPRITE_INDX2.q(),
      XOBE_SPRITE_INDX3.q(),
      YDUF_SPRITE_INDX4.q(),
      XECU_SPRITE_INDX5.q(),
      0,
      0
    )
  );

  d("STORE MATCH %c%c%c%c%c%c%c%c%c%c\n",
    STORE0_MATCHn.c(), STORE1_MATCHn.c(), STORE2_MATCHn.c(), STORE3_MATCHn.c(), STORE4_MATCHn.c(),
    STORE5_MATCHn.c(), STORE6_MATCHn.c(), STORE7_MATCHn.c(), STORE8_MATCHn.c(), STORE9_MATCHn.c());

  d("SPRITE COUNT %02d\n", pack(SPRITE_COUNT3.q(), SPRITE_COUNT2.q(), SPRITE_COUNT1.q(), SPRITE_COUNT0.q()));
  d("\n");
}


//------------------------------------------------------------------------------

void SpriteStore::tick(const SchematicTop& top) {
  {
    /*p21.ACAM*/ wire _ACAM_X0n = not(top.pix_pipe.XEHO_X0.q());
    /*p21.AZUB*/ wire _AZUB_X1n = not(top.pix_pipe.SAVY_X1.q());
    /*p21.AMEL*/ wire _AMEL_X2n = not(top.pix_pipe.XODU_X2.q());
    /*p21.AHAL*/ wire _AHAL_X3n = not(top.pix_pipe.XYDO_X3.q());
    /*p21.APUX*/ wire _APUX_X4n = not(top.pix_pipe.TUHU_X4.q());
    /*p21.ABEF*/ wire _ABEF_X5n = not(top.pix_pipe.TUKY_X5.q());
    /*p21.ADAZ*/ wire _ADAZ_X6n = not(top.pix_pipe.TAKO_X6.q());
    /*p21.ASAH*/ wire _ASAH_X7n = not(top.pix_pipe.SYBE_X7.q());

    /*p31.ZOGY*/ wire STORE0_MATCH0n = xor (XEPE_STORE0_X0.q(), _ACAM_X0n);
    /*p31.ZEBA*/ wire STORE0_MATCH1n = xor (YLAH_STORE0_X1.q(), _AZUB_X1n);
    /*p31.ZAHA*/ wire STORE0_MATCH2n = xor (ZOLA_STORE0_X2.q(), _AMEL_X2n);
    /*p31.ZOKY*/ wire STORE0_MATCH3n = xor (ZULU_STORE0_X3.q(), _AHAL_X3n);
    /*p31.WOJU*/ wire STORE0_MATCH4n = xor (WELO_STORE0_X4.q(), _APUX_X4n);
    /*p31.YFUN*/ wire STORE0_MATCH5n = xor (XUNY_STORE0_X5.q(), _ABEF_X5n);
    /*p31.WYZA*/ wire STORE0_MATCH6n = xor (WOTE_STORE0_X6.q(), _ADAZ_X6n);
    /*p31.YPUK*/ wire STORE0_MATCH7n = xor (XAKO_STORE0_X7.q(), _ASAH_X7n);

    /*p31.EDYM*/ wire STORE1_MATCH0n = xor (STORE1_X0.q(), _ACAM_X0n);
    /*p31.EMYB*/ wire STORE1_MATCH1n = xor (STORE1_X1.q(), _AZUB_X1n);
    /*p31.EBEF*/ wire STORE1_MATCH2n = xor (STORE1_X2.q(), _AMEL_X2n);
    /*p31.EWOK*/ wire STORE1_MATCH3n = xor (STORE1_X3.q(), _AHAL_X3n);
    /*p31.COLA*/ wire STORE1_MATCH4n = xor (STORE1_X4.q(), _APUX_X4n);
    /*p31.BOBA*/ wire STORE1_MATCH5n = xor (STORE1_X5.q(), _ABEF_X5n);
    /*p31.COLU*/ wire STORE1_MATCH6n = xor (STORE1_X6.q(), _ADAZ_X6n);
    /*p31.BAHU*/ wire STORE1_MATCH7n = xor (STORE1_X7.q(), _ASAH_X7n);

    /*p31.FUZU*/ wire STORE2_MATCH0n = xor (STORE2_X0.q(), _ACAM_X0n);
    /*p31.FESO*/ wire STORE2_MATCH1n = xor (STORE2_X1.q(), _AZUB_X1n);
    /*p31.FOKY*/ wire STORE2_MATCH2n = xor (STORE2_X2.q(), _AMEL_X2n);
    /*p31.FYVA*/ wire STORE2_MATCH3n = xor (STORE2_X3.q(), _AHAL_X3n);
    /*p31.CEKO*/ wire STORE2_MATCH4n = xor (STORE2_X4.q(), _APUX_X4n);
    /*p31.DETY*/ wire STORE2_MATCH5n = xor (STORE2_X5.q(), _ABEF_X5n);
    /*p31.DOZO*/ wire STORE2_MATCH6n = xor (STORE2_X6.q(), _ADAZ_X6n);
    /*p31.CONY*/ wire STORE2_MATCH7n = xor (STORE2_X7.q(), _ASAH_X7n);

    /*p31.YHOK*/ wire STORE3_MATCH0n = xor (STORE3_X0.q(), _ACAM_X0n);
    /*p31.YCAH*/ wire STORE3_MATCH1n = xor (STORE3_X1.q(), _AZUB_X1n);
    /*p31.YDAJ*/ wire STORE3_MATCH2n = xor (STORE3_X2.q(), _AMEL_X2n);
    /*p31.YVUZ*/ wire STORE3_MATCH3n = xor (STORE3_X3.q(), _AHAL_X3n);
    /*p31.YVAP*/ wire STORE3_MATCH4n = xor (STORE3_X4.q(), _APUX_X4n);
    /*p31.XENY*/ wire STORE3_MATCH5n = xor (STORE3_X5.q(), _ABEF_X5n);
    /*p31.XAVU*/ wire STORE3_MATCH6n = xor (STORE3_X6.q(), _ADAZ_X6n);
    /*p31.XEVA*/ wire STORE3_MATCH7n = xor (STORE3_X7.q(), _ASAH_X7n);

    /*p31.XEJU*/ wire STORE4_MATCH0n = xor (STORE4_X0.q(), _ACAM_X0n);
    /*p31.ZATE*/ wire STORE4_MATCH1n = xor (STORE4_X1.q(), _AZUB_X1n);
    /*p31.ZAKU*/ wire STORE4_MATCH2n = xor (STORE4_X2.q(), _AMEL_X2n);
    /*p31.YBOX*/ wire STORE4_MATCH3n = xor (STORE4_X3.q(), _AHAL_X3n);
    /*p31.ZYKU*/ wire STORE4_MATCH4n = xor (STORE4_X4.q(), _APUX_X4n);
    /*p31.ZYPU*/ wire STORE4_MATCH5n = xor (STORE4_X5.q(), _ABEF_X5n);
    /*p31.XAHA*/ wire STORE4_MATCH6n = xor (STORE4_X6.q(), _ADAZ_X6n);
    /*p31.ZEFE*/ wire STORE4_MATCH7n = xor (STORE4_X7.q(), _ASAH_X7n);

    /*p31.GUZO*/ wire STORE5_MATCH0n = xor (STORE5_X0.q(), _ACAM_X0n);
    /*p31.GOLA*/ wire STORE5_MATCH1n = xor (STORE5_X1.q(), _AZUB_X1n);
    /*p31.GEVE*/ wire STORE5_MATCH2n = xor (STORE5_X2.q(), _AMEL_X2n);
    /*p31.GUDE*/ wire STORE5_MATCH3n = xor (STORE5_X3.q(), _AHAL_X3n);
    /*p31.BAZY*/ wire STORE5_MATCH4n = xor (STORE5_X4.q(), _APUX_X4n);
    /*p31.CYLE*/ wire STORE5_MATCH5n = xor (STORE5_X5.q(), _ABEF_X5n);
    /*p31.CEVA*/ wire STORE5_MATCH6n = xor (STORE5_X6.q(), _ADAZ_X6n);
    /*p31.BUMY*/ wire STORE5_MATCH7n = xor (STORE5_X7.q(), _ASAH_X7n);

    /*p31.XOSU*/ wire STORE6_MATCH0n = xor (STORE6_X0.q(), _ACAM_X0n);
    /*p31.ZUVU*/ wire STORE6_MATCH1n = xor (STORE6_X1.q(), _AZUB_X1n);
    /*p31.XUCO*/ wire STORE6_MATCH2n = xor (STORE6_X2.q(), _AMEL_X2n);
    /*p31.ZULO*/ wire STORE6_MATCH3n = xor (STORE6_X3.q(), _AHAL_X3n);
    /*p31.ZARE*/ wire STORE6_MATCH4n = xor (STORE6_X4.q(), _APUX_X4n);
    /*p31.ZEMU*/ wire STORE6_MATCH5n = xor (STORE6_X5.q(), _ABEF_X5n);
    /*p31.ZYGO*/ wire STORE6_MATCH6n = xor (STORE6_X6.q(), _ADAZ_X6n);
    /*p31.ZUZY*/ wire STORE6_MATCH7n = xor (STORE6_X7.q(), _ASAH_X7n);

    /*p31.DUSE*/ wire STORE7_MATCH0n = xor (STORE7_X0.q(), _ACAM_X0n);
    /*p31.DAGU*/ wire STORE7_MATCH1n = xor (STORE7_X1.q(), _AZUB_X1n);
    /*p31.DYZE*/ wire STORE7_MATCH2n = xor (STORE7_X2.q(), _AMEL_X2n);
    /*p31.DESO*/ wire STORE7_MATCH3n = xor (STORE7_X3.q(), _AHAL_X3n);
    /*p31.EJOT*/ wire STORE7_MATCH4n = xor (STORE7_X4.q(), _APUX_X4n);
    /*p31.ESAJ*/ wire STORE7_MATCH5n = xor (STORE7_X5.q(), _ABEF_X5n);
    /*p31.DUCU*/ wire STORE7_MATCH6n = xor (STORE7_X6.q(), _ADAZ_X6n);
    /*p31.EWUD*/ wire STORE7_MATCH7n = xor (STORE7_X7.q(), _ASAH_X7n);

    /*p31.DUZE*/ wire STORE8_MATCH0n = xor (STORE8_X0.q(), _APUX_X4n);
    /*p31.DAGA*/ wire STORE8_MATCH1n = xor (STORE8_X1.q(), _ABEF_X5n);
    /*p31.DAWU*/ wire STORE8_MATCH2n = xor (STORE8_X2.q(), _ADAZ_X6n);
    /*p31.EJAW*/ wire STORE8_MATCH3n = xor (STORE8_X3.q(), _ASAH_X7n);
    /*p31.GOHO*/ wire STORE8_MATCH4n = xor (STORE8_X4.q(), _ACAM_X0n);
    /*p31.GASU*/ wire STORE8_MATCH5n = xor (STORE8_X5.q(), _AZUB_X1n);
    /*p31.GABU*/ wire STORE8_MATCH6n = xor (STORE8_X6.q(), _AMEL_X2n);
    /*p31.GAFE*/ wire STORE8_MATCH7n = xor (STORE8_X7.q(), _AHAL_X3n);

    /*p31.YMAM*/ wire STORE9_MATCH0n = xor (STORE9_X0.q(), _ACAM_X0n);
    /*p31.YTYP*/ wire STORE9_MATCH1n = xor (STORE9_X1.q(), _AZUB_X1n);
    /*p31.YFOP*/ wire STORE9_MATCH2n = xor (STORE9_X2.q(), _AMEL_X2n);
    /*p31.YVAC*/ wire STORE9_MATCH3n = xor (STORE9_X3.q(), _AHAL_X3n);
    /*p31.ZYWU*/ wire STORE9_MATCH4n = xor (STORE9_X4.q(), _APUX_X4n);
    /*p31.ZUZA*/ wire STORE9_MATCH5n = xor (STORE9_X5.q(), _ABEF_X5n);
    /*p31.ZEJO*/ wire STORE9_MATCH6n = xor (STORE9_X6.q(), _ADAZ_X6n);
    /*p31.ZEDA*/ wire STORE9_MATCH7n = xor (STORE9_X7.q(), _ASAH_X7n);

    /*p31.ZAKO*/ wire STORE0_MATCHAp = nor(STORE0_MATCH0n, STORE0_MATCH1n, STORE0_MATCH2n, STORE0_MATCH3n);
    /*p31.XEBA*/ wire STORE0_MATCHBp = nor(STORE0_MATCH4n, STORE0_MATCH5n, STORE0_MATCH6n, STORE0_MATCH7n);
    /*p31.CYVY*/ wire STORE1_MATCHB = nor(STORE1_MATCH4n, STORE1_MATCH5n, STORE1_MATCH6n, STORE1_MATCH7n);
    /*p31.EWAM*/ wire STORE1_MATCHA = nor(STORE1_MATCH0n, STORE1_MATCH1n, STORE1_MATCH2n, STORE1_MATCH3n);
    /*p31.CEHU*/ wire STORE2_MATCHA = nor(STORE2_MATCH4n, STORE2_MATCH5n, STORE2_MATCH6n, STORE2_MATCH7n);
    /*p31.EKES*/ wire STORE2_MATCHB = nor(STORE2_MATCH0n, STORE2_MATCH1n, STORE2_MATCH2n, STORE2_MATCH3n);
    /*p31.ZURE*/ wire STORE3_MATCHA = nor(STORE3_MATCH0n, STORE3_MATCH1n, STORE3_MATCH2n, STORE3_MATCH3n);
    /*p31.YWOS*/ wire STORE3_MATCHB = nor(STORE3_MATCH4n, STORE3_MATCH5n, STORE3_MATCH6n, STORE3_MATCH7n);
    /*p31.YKOK*/ wire STORE4_MATCHA = nor(STORE4_MATCH0n, STORE4_MATCH1n, STORE4_MATCH2n, STORE4_MATCH3n);
    /*p31.YNAZ*/ wire STORE4_MATCHB = nor(STORE4_MATCH4n, STORE4_MATCH5n, STORE4_MATCH6n, STORE4_MATCH7n);
    /*p31.COGY*/ wire STORE5_MATCHA = nor(STORE5_MATCH4n, STORE5_MATCH5n, STORE5_MATCH6n, STORE5_MATCH7n);
    /*p31.FYMA*/ wire STORE5_MATCHB = nor(STORE5_MATCH0n, STORE5_MATCH1n, STORE5_MATCH2n, STORE5_MATCH3n);
    /*p31.YWAP*/ wire STORE6_MATCHA = nor(STORE6_MATCH4n, STORE6_MATCH5n, STORE6_MATCH6n, STORE6_MATCH7n);
    /*p31.YDOT*/ wire STORE6_MATCHB = nor(STORE6_MATCH0n, STORE6_MATCH1n, STORE6_MATCH2n, STORE6_MATCH3n);
    /*p31.CYCO*/ wire STORE7_MATCHA = nor(STORE7_MATCH0n, STORE7_MATCH1n, STORE7_MATCH2n, STORE7_MATCH3n);
    /*p31.DAJE*/ wire STORE7_MATCHB = nor(STORE7_MATCH4n, STORE7_MATCH5n, STORE7_MATCH6n, STORE7_MATCH7n);
    /*p31.DAMA*/ wire STORE8_MATCHA = nor(STORE8_MATCH0n, STORE8_MATCH1n, STORE8_MATCH2n, STORE8_MATCH3n);
    /*p31.FEHA*/ wire STORE8_MATCHB = nor(STORE8_MATCH4n, STORE8_MATCH5n, STORE8_MATCH6n, STORE8_MATCH7n);
    /*p31.YLEV*/ wire STORE9_MATCHA = nor(STORE9_MATCH0n, STORE9_MATCH1n, STORE9_MATCH2n, STORE9_MATCH3n);
    /*p31.YTUB*/ wire STORE9_MATCHB = nor(STORE9_MATCH4n, STORE9_MATCH5n, STORE9_MATCH6n, STORE9_MATCH7n);

    /*p29.YDUG*/ STORE0_MATCHn = nand(top.sprite_scanner.AROR_MATCH_ENp(), STORE0_MATCHBp, STORE0_MATCHAp);
    /*p29.DYDU*/ STORE1_MATCHn = nand(top.sprite_scanner.AROR_MATCH_ENp(), STORE1_MATCHA, STORE1_MATCHB);
    /*p29.DEGO*/ STORE2_MATCHn = nand(top.sprite_scanner.AROR_MATCH_ENp(), STORE2_MATCHB, STORE2_MATCHA);
    /*p29.YLOZ*/ STORE3_MATCHn = nand(top.sprite_scanner.AROR_MATCH_ENp(), STORE3_MATCHA, STORE3_MATCHB);
    /*p29.XAGE*/ STORE4_MATCHn = nand(top.sprite_scanner.AROR_MATCH_ENp(), STORE4_MATCHB, STORE4_MATCHA);
    /*p29.EGOM*/ STORE5_MATCHn = nand(top.sprite_scanner.AROR_MATCH_ENp(), STORE5_MATCHB, STORE5_MATCHA);
    /*p29.YBEZ*/ STORE6_MATCHn = nand(top.sprite_scanner.AROR_MATCH_ENp(), STORE6_MATCHB, STORE6_MATCHA);
    /*p29.DYKA*/ STORE7_MATCHn = nand(top.sprite_scanner.AROR_MATCH_ENp(), STORE7_MATCHA, STORE7_MATCHB);
    /*p29.EFYL*/ STORE8_MATCHn = nand(top.sprite_scanner.AROR_MATCH_ENp(), STORE8_MATCHB, STORE8_MATCHA);
    /*p29.YGEM*/ STORE9_MATCHn = nand(top.sprite_scanner.AROR_MATCH_ENp(), STORE9_MATCHA, STORE9_MATCHB);

    /*p29.FEFY*/ wire _FEFY = nand(STORE4_MATCHn, STORE3_MATCHn, STORE2_MATCHn, STORE1_MATCHn, STORE0_MATCHn);
    /*p29.FOVE*/ wire _FOVE = nand(STORE9_MATCHn, STORE8_MATCHn, STORE7_MATCHn, STORE6_MATCHn, STORE5_MATCHn);
    /*p29.FEPO*/ FEPO_STORE_MATCHp = or(_FEFY, _FOVE);
  }
}

  //------------------------------------------------------------------------------

void SpriteStore::tock(const SchematicTop& top) {
  wire WEFE_VCC = 1;

  //----------------------------------------
  // Sprite scan Y matcher

  {
    // Sprite store counter.
    // The sprite count clock stops ticking once we have 10 sprites.

    /*p29.BAKY*/ wire _BAKY_SPRITES_FULL = and (SPRITE_COUNT1.q(), SPRITE_COUNT3.q());
    /*p29.CAKE*/ wire _CAKE_CLKp = or (_BAKY_SPRITES_FULL, DEZY_STORE_ENn.q());
    
    /*p28.AZYB*/ wire _AZYB_RST = not(top.lcd_reg.BYHA_VID_LINE_END_TRIGn());
    /*p29.BESE*/ SPRITE_COUNT0 = dff17(_CAKE_CLKp,         _AZYB_RST, SPRITE_COUNT0.qn());
    /*p29.CUXY*/ SPRITE_COUNT1 = dff17(SPRITE_COUNT0.q(), _AZYB_RST, SPRITE_COUNT1.qn());
    /*p29.BEGO*/ SPRITE_COUNT2 = dff17(SPRITE_COUNT1.q(), _AZYB_RST, SPRITE_COUNT2.qn());
    /*p29.DYBE*/ SPRITE_COUNT3 = dff17(SPRITE_COUNT2.q(), _AZYB_RST, SPRITE_COUNT3.qn());
  }

  //----------------------------------------

  {
    // FEPO_STORE_MATCHp here is weird, I guess it's just an easy signal to use to mux the bus?

    /*p30.WENU*/ SPR_TRI_LINE_0 = tribuf_6n(FEPO_STORE_MATCHp, top.sprite_scanner.DEGE_SPRITE_DELTA0());
    /*p30.CUCU*/ SPR_TRI_LINE_1 = tribuf_6n(FEPO_STORE_MATCHp, top.sprite_scanner.DABY_SPRITE_DELTA1());
    /*p30.CUCA*/ SPR_TRI_LINE_2 = tribuf_6n(FEPO_STORE_MATCHp, top.sprite_scanner.DABU_SPRITE_DELTA2());
    /*p30.CEGA*/ SPR_TRI_LINE_3 = tribuf_6n(FEPO_STORE_MATCHp, top.sprite_scanner.GYSA_SPRITE_DELTA3());

    /*p29.DEZY*/ DEZY_STORE_ENn = dff17(top.clk_reg.ZEME_AxCxExGx(), top.clk_reg.XAPO_VID_RSTn(), top.sprite_scanner.DYTY_STORE_ENn_xxCDxxGH());
  }

  //----------------------------------------
  // Sprite store input driver.

  {
    /*p30.CYKE*/ wire _CYKE_AxxDExxH = not(top.clk_reg.XUPY_ABxxEFxx());
    /*p30.WUDA*/ wire _WUDA_xBCxxFGx = not(_CYKE_AxxDExxH);

    /*p30.XADU*/ XADU_SPRITE_INDX0 = dff13(_WUDA_xBCxxFGx, _CYKE_AxxDExxH, WEFE_VCC, top.oam_bus.YFOT_OAM_A2p());
    /*p30.XEDY*/ XEDY_SPRITE_INDX1 = dff13(_WUDA_xBCxxFGx, _CYKE_AxxDExxH, WEFE_VCC, top.oam_bus.YFOC_OAM_A3p());
    /*p30.ZUZE*/ ZUZE_SPRITE_INDX2 = dff13(_WUDA_xBCxxFGx, _CYKE_AxxDExxH, WEFE_VCC, top.oam_bus.YVOM_OAM_A4p());
    /*p30.XOBE*/ XOBE_SPRITE_INDX3 = dff13(_WUDA_xBCxxFGx, _CYKE_AxxDExxH, WEFE_VCC, top.oam_bus.YMEV_OAM_A5p());
    /*p30.YDUF*/ YDUF_SPRITE_INDX4 = dff13(_WUDA_xBCxxFGx, _CYKE_AxxDExxH, WEFE_VCC, top.oam_bus.XEMU_OAM_A6p());
    /*p30.XECU*/ XECU_SPRITE_INDX5 = dff13(_WUDA_xBCxxFGx, _CYKE_AxxDExxH, WEFE_VCC, top.oam_bus.YZET_OAM_A7p());

    /*p30.WUZY*/ SPR_TRI_INDX_0 = tribuf_6n(top.sprite_scanner.BUZA_STORE_SPRITE_INDX(), XADU_SPRITE_INDX0.q());
    /*p30.WYSE*/ SPR_TRI_INDX_1 = tribuf_6n(top.sprite_scanner.BUZA_STORE_SPRITE_INDX(), XEDY_SPRITE_INDX1.q());
    /*p30.ZYSU*/ SPR_TRI_INDX_2 = tribuf_6n(top.sprite_scanner.BUZA_STORE_SPRITE_INDX(), ZUZE_SPRITE_INDX2.q());
    /*p30.WYDA*/ SPR_TRI_INDX_3 = tribuf_6n(top.sprite_scanner.BUZA_STORE_SPRITE_INDX(), XOBE_SPRITE_INDX3.q());
    /*p30.WUCO*/ SPR_TRI_INDX_4 = tribuf_6n(top.sprite_scanner.BUZA_STORE_SPRITE_INDX(), YDUF_SPRITE_INDX4.q());
    /*p30.WEZA*/ SPR_TRI_INDX_5 = tribuf_6n(top.sprite_scanner.BUZA_STORE_SPRITE_INDX(), XECU_SPRITE_INDX5.q());
  }

  //----------------------------------------
  // Sprite store getter

  {
    wire GND = 0;

    /*p29.WEFU*/ wire STORE0_MATCHp = not(STORE0_MATCHn);
    /*p29.GAJA*/ wire STORE1_MATCH = not(STORE1_MATCHn);
    /*p29.GUPO*/ wire STORE2_MATCH = not(STORE2_MATCHn);
    /*p29.WEBO*/ wire STORE3_MATCH = not(STORE3_MATCHn);
    /*p29.WUNA*/ wire STORE4_MATCH = not(STORE4_MATCHn);
    /*p29.GABA*/ wire STORE5_MATCH = not(STORE5_MATCHn);
    /*p29.WASE*/ wire STORE6_MATCH = not(STORE6_MATCHn);
    /*p29.GYTE*/ wire STORE7_MATCH = not(STORE7_MATCHn);
    /*p29.GEKE*/ wire STORE8_MATCH = not(STORE8_MATCHn);

    // Priority encoder so we fetch the first sprite that matches

    /*p29.GEZE*/ wire STORE0_MATCH_OUT = or(STORE0_MATCHp, GND);
    /*p29.FUMA*/ wire STORE1_MATCH_OUT = or(STORE1_MATCH, STORE0_MATCH_OUT);
    /*p29.GEDE*/ wire STORE2_MATCH_OUT = or(STORE2_MATCH, STORE1_MATCH_OUT);
    /*p29.WUTO*/ wire STORE3_MATCH_OUT = or(STORE3_MATCH, STORE2_MATCH_OUT);
    /*p29.XYLA*/ wire STORE4_MATCH_OUT = or(STORE4_MATCH, STORE3_MATCH_OUT);
    /*p29.WEJA*/ wire STORE5_MATCH_OUT = or(STORE5_MATCH, STORE4_MATCH_OUT);
    /*p29.WYLA*/ wire STORE6_MATCH_OUT = or(STORE6_MATCH, STORE5_MATCH_OUT);
    /*p29.FAVO*/ wire STORE7_MATCH_OUT = or(STORE7_MATCH, STORE6_MATCH_OUT);
    /*p29.GYGA*/ wire STORE8_MATCH_OUT = or(STORE8_MATCH, STORE7_MATCH_OUT);

    /*p29.GUVA*/ wire _GUVA_SPRITE0_GETp = nor(STORE0_MATCHn, GND);
    /*p29.ENUT*/ wire _ENUT_SPRITE1_GETp = nor(STORE1_MATCHn, STORE0_MATCH_OUT);
    /*p29.EMOL*/ wire _EMOL_SPRITE2_GETp = nor(STORE2_MATCHn, STORE1_MATCH_OUT);
    /*p29.GYFY*/ wire _GYFY_SPRITE3_GETp = nor(STORE3_MATCHn, STORE2_MATCH_OUT);
    /*p29.GONO*/ wire _GONO_SPRITE4_GETp = nor(STORE4_MATCHn, STORE3_MATCH_OUT);
    /*p29.GEGA*/ wire _GEGA_SPRITE5_GETp = nor(STORE5_MATCHn, STORE4_MATCH_OUT);
    /*p29.XOJA*/ wire _XOJA_SPRITE6_GETp = nor(STORE6_MATCHn, STORE5_MATCH_OUT);
    /*p29.GUTU*/ wire _GUTU_SPRITE7_GETp = nor(STORE7_MATCHn, STORE6_MATCH_OUT);
    /*p29.FOXA*/ wire _FOXA_SPRITE8_GETp = nor(STORE8_MATCHn, STORE7_MATCH_OUT);
    /*p29.GUZE*/ wire _GUZE_SPRITE9_GETp = nor(STORE9_MATCHn, STORE8_MATCH_OUT);

    // Delayed reset signal for the selected store

    /*p28.ABAK*/ wire _ABAK_VID_LINE_TRIGp = or (top.lcd_reg.ATEJ_VID_LINE_TRIGp(), top.clk_reg.AMYG_VID_RSTp());
    /*p28.BYVA*/ wire BYVA_VID_LINE_TRIGn = not(_ABAK_VID_LINE_TRIGp);

    /*p29.EBOJ*/ EBOJ_STORE0_RSTp = dff17(top.sprite_fetcher.WUTY_SPRITE_DONEp(), BYVA_VID_LINE_TRIGn, _GUVA_SPRITE0_GETp);
    /*p29.CEDY*/ CEDY_STORE1_RSTp = dff17(top.sprite_fetcher.WUTY_SPRITE_DONEp(), BYVA_VID_LINE_TRIGn, _ENUT_SPRITE1_GETp);
    /*p29.EGAV*/ EGAV_STORE2_RSTp = dff17(top.sprite_fetcher.WUTY_SPRITE_DONEp(), BYVA_VID_LINE_TRIGn, _EMOL_SPRITE2_GETp);
    /*p29.GOTA*/ GOTA_STORE3_RSTp = dff17(top.sprite_fetcher.WUTY_SPRITE_DONEp(), BYVA_VID_LINE_TRIGn, _GYFY_SPRITE3_GETp);
    /*p29.XUDY*/ XUDY_STORE4_RSTp = dff17(top.sprite_fetcher.WUTY_SPRITE_DONEp(), BYVA_VID_LINE_TRIGn, _GONO_SPRITE4_GETp);
    /*p29.WAFY*/ WAFY_STORE5_RSTp = dff17(top.sprite_fetcher.WUTY_SPRITE_DONEp(), BYVA_VID_LINE_TRIGn, _GEGA_SPRITE5_GETp);
    /*p29.WOMY*/ WOMY_STORE6_RSTp = dff17(top.sprite_fetcher.WUTY_SPRITE_DONEp(), BYVA_VID_LINE_TRIGn, _XOJA_SPRITE6_GETp);
    /*p29.WAPO*/ WAPO_STORE7_RSTp = dff17(top.sprite_fetcher.WUTY_SPRITE_DONEp(), BYVA_VID_LINE_TRIGn, _GUTU_SPRITE7_GETp);
    /*p29.EXUQ*/ EXUQ_STORE8_RSTp = dff17(top.sprite_fetcher.WUTY_SPRITE_DONEp(), BYVA_VID_LINE_TRIGn, _FOXA_SPRITE8_GETp);
    /*p29.FONO*/ FONO_STORE9_RSTp = dff17(top.sprite_fetcher.WUTY_SPRITE_DONEp(), BYVA_VID_LINE_TRIGn, _GUZE_SPRITE9_GETp);

    /*p29.FURO*/ wire FURO_SPRITE0_GETn = not(_GUVA_SPRITE0_GETp);
    /*p29.DYDO*/ wire DYDO_SPRITE1_GETn = not(_ENUT_SPRITE1_GETp);
    /*p29.FAME*/ wire FAME_SPRITE2_GETn = not(_EMOL_SPRITE2_GETp);
    /*p29.GYMA*/ wire GYMA_SPRITE3_GETn = not(_GYFY_SPRITE3_GETp);
    /*p29.GOWO*/ wire GOWO_SPRITE4_GETn = not(_GONO_SPRITE4_GETp);
    /*p29.GYGY*/ wire GYGY_SPRITE5_GETn = not(_GEGA_SPRITE5_GETp);
    /*p29.XYME*/ wire XYME_SPRITE6_GETn = not(_XOJA_SPRITE6_GETp);
    /*p29.GUGY*/ wire GUGY_SPRITE7_GETn = not(_GUTU_SPRITE7_GETp);
    /*p29.DENY*/ wire DENY_SPRITE8_GETn = not(_FOXA_SPRITE8_GETp);
    /*p29.FADO*/ wire FADO_SPRITE9_GETn = not(_GUZE_SPRITE9_GETp);

    /*p30.ZEDY*/ SPR_TRI_INDX_0 = tribuf_6n(FURO_SPRITE0_GETn, YWAK_STORE0_INDX0.q());
    /*p30.ZUMU*/ SPR_TRI_INDX_1 = tribuf_6n(FURO_SPRITE0_GETn, ZONY_STORE0_INDX1.q());
    /*p30.WOKO*/ SPR_TRI_INDX_2 = tribuf_6n(FURO_SPRITE0_GETn, WYTE_STORE0_INDX2.q());
    /*p30.ZAVE*/ SPR_TRI_INDX_3 = tribuf_6n(FURO_SPRITE0_GETn, YZEP_STORE0_INDX3.q());
    /*p30.ZECE*/ SPR_TRI_INDX_4 = tribuf_6n(FURO_SPRITE0_GETn, YSOK_STORE0_INDX4.q());
    /*p30.ZETU*/ SPR_TRI_INDX_5 = tribuf_6n(FURO_SPRITE0_GETn, YGUS_STORE0_INDX5.q());
    /*p30.GOFO*/ SPR_TRI_LINE_0 = tribuf_6n(FURO_SPRITE0_GETn, FYHY_STORE0_LINE0.q());
    /*p30.WEHE*/ SPR_TRI_LINE_1 = tribuf_6n(FURO_SPRITE0_GETn, GYHO_STORE0_LINE1.q());
    /*p30.AJAL*/ SPR_TRI_LINE_2 = tribuf_6n(FURO_SPRITE0_GETn, BOZU_STORE0_LINE2.q());
    /*p30.BUKY*/ SPR_TRI_LINE_3 = tribuf_6n(FURO_SPRITE0_GETn, CUFO_STORE0_LINE3.q());

    /*p30.CUBO*/ SPR_TRI_INDX_0 = tribuf_6n(DYDO_SPRITE1_GETn, STORE1_INDX5.q());
    /*p30.CELU*/ SPR_TRI_INDX_1 = tribuf_6n(DYDO_SPRITE1_GETn, STORE1_INDX4.q());
    /*p30.CEGY*/ SPR_TRI_INDX_2 = tribuf_6n(DYDO_SPRITE1_GETn, STORE1_INDX3.q());
    /*p30.BETY*/ SPR_TRI_INDX_3 = tribuf_6n(DYDO_SPRITE1_GETn, STORE1_INDX2.q());
    /*p30.CYBY*/ SPR_TRI_INDX_4 = tribuf_6n(DYDO_SPRITE1_GETn, STORE1_INDX1.q());
    /*p30.BEMO*/ SPR_TRI_INDX_5 = tribuf_6n(DYDO_SPRITE1_GETn, STORE1_INDX0.q());
    /*p30.BEFE*/ SPR_TRI_LINE_0 = tribuf_6n(DYDO_SPRITE1_GETn, STORE1_LINE0.q());
    /*p30.BYRO*/ SPR_TRI_LINE_1 = tribuf_6n(DYDO_SPRITE1_GETn, STORE1_LINE1.q());
    /*p30.BACO*/ SPR_TRI_LINE_2 = tribuf_6n(DYDO_SPRITE1_GETn, STORE1_LINE2.q());
    /*p30.AHUM*/ SPR_TRI_LINE_3 = tribuf_6n(DYDO_SPRITE1_GETn, STORE1_LINE3.q());

    /*p30.AXEC*/ SPR_TRI_INDX_5 = tribuf_6n(FAME_SPRITE2_GETn, STORE2_INDX0.q());
    /*p30.CYRO*/ SPR_TRI_INDX_4 = tribuf_6n(FAME_SPRITE2_GETn, STORE2_INDX1.q());
    /*p30.CUVU*/ SPR_TRI_INDX_3 = tribuf_6n(FAME_SPRITE2_GETn, STORE2_INDX2.q());
    /*p30.APON*/ SPR_TRI_INDX_2 = tribuf_6n(FAME_SPRITE2_GETn, STORE2_INDX3.q());
    /*p30.AFOZ*/ SPR_TRI_INDX_1 = tribuf_6n(FAME_SPRITE2_GETn, STORE2_INDX4.q());
    /*p30.CUBE*/ SPR_TRI_INDX_0 = tribuf_6n(FAME_SPRITE2_GETn, STORE2_INDX5.q());
    /*p30.ZABY*/ SPR_TRI_LINE_0 = tribuf_6n(FAME_SPRITE2_GETn, STORE2_LINE0.q());
    /*p30.ZUKE*/ SPR_TRI_LINE_1 = tribuf_6n(FAME_SPRITE2_GETn, STORE2_LINE1.q());
    /*p30.WUXE*/ SPR_TRI_LINE_2 = tribuf_6n(FAME_SPRITE2_GETn, STORE2_LINE2.q());
    /*p30.WERE*/ SPR_TRI_LINE_3 = tribuf_6n(FAME_SPRITE2_GETn, STORE2_LINE3.q());

    /*p30.DEZU*/ SPR_TRI_INDX_5 = tribuf_6n(GYMA_SPRITE3_GETn, STORE3_INDX0.q());
    /*p30.EFUD*/ SPR_TRI_INDX_4 = tribuf_6n(GYMA_SPRITE3_GETn, STORE3_INDX1.q());
    /*p30.DONY*/ SPR_TRI_INDX_3 = tribuf_6n(GYMA_SPRITE3_GETn, STORE3_INDX2.q());
    /*p30.DOWA*/ SPR_TRI_INDX_2 = tribuf_6n(GYMA_SPRITE3_GETn, STORE3_INDX3.q());
    /*p30.DYGO*/ SPR_TRI_INDX_1 = tribuf_6n(GYMA_SPRITE3_GETn, STORE3_INDX4.q());
    /*p30.ENAP*/ SPR_TRI_INDX_0 = tribuf_6n(GYMA_SPRITE3_GETn, STORE3_INDX5.q());
    /*p30.ZYPO*/ SPR_TRI_LINE_0 = tribuf_6n(GYMA_SPRITE3_GETn, STORE3_LINE0.q());
    /*p30.ZEXE*/ SPR_TRI_LINE_1 = tribuf_6n(GYMA_SPRITE3_GETn, STORE3_LINE1.q());
    /*p30.YJEM*/ SPR_TRI_LINE_2 = tribuf_6n(GYMA_SPRITE3_GETn, STORE3_LINE2.q());
    /*p30.YWAV*/ SPR_TRI_LINE_3 = tribuf_6n(GYMA_SPRITE3_GETn, STORE3_LINE4.q());

    /*p30.WAJA*/ SPR_TRI_INDX_5 = tribuf_6n(GOWO_SPRITE4_GETn, XYNU.q());
    /*p30.WOXY*/ SPR_TRI_INDX_4 = tribuf_6n(GOWO_SPRITE4_GETn, XEGE.q());
    /*p30.XYRE*/ SPR_TRI_INDX_3 = tribuf_6n(GOWO_SPRITE4_GETn, XABO.q());
    /*p30.WERU*/ SPR_TRI_INDX_2 = tribuf_6n(GOWO_SPRITE4_GETn, WANU.q());
    /*p30.WEPY*/ SPR_TRI_INDX_1 = tribuf_6n(GOWO_SPRITE4_GETn, XEFE.q());
    /*p30.WUXU*/ SPR_TRI_INDX_0 = tribuf_6n(GOWO_SPRITE4_GETn, XAVE.q());
    /*p30.BYDO*/ SPR_TRI_LINE_0 = tribuf_6n(GOWO_SPRITE4_GETn, CUMU.q());
    /*p30.BUCE*/ SPR_TRI_LINE_1 = tribuf_6n(GOWO_SPRITE4_GETn, CAPO.q());
    /*p30.BOVE*/ SPR_TRI_LINE_2 = tribuf_6n(GOWO_SPRITE4_GETn, CONO.q());
    /*p30.BEVY*/ SPR_TRI_LINE_3 = tribuf_6n(GOWO_SPRITE4_GETn, CAJU.q());

    /*p30.DALO*/ SPR_TRI_INDX_5 = tribuf_6n(GYGY_SPRITE5_GETn, EKAP.q());
    /*p30.DALY*/ SPR_TRI_INDX_4 = tribuf_6n(GYGY_SPRITE5_GETn, ETAV.q());
    /*p30.DUZA*/ SPR_TRI_INDX_3 = tribuf_6n(GYGY_SPRITE5_GETn, EBEX.q());
    /*p30.WAGA*/ SPR_TRI_INDX_2 = tribuf_6n(GYGY_SPRITE5_GETn, GORU.q());
    /*p30.DYNY*/ SPR_TRI_INDX_1 = tribuf_6n(GYGY_SPRITE5_GETn, ETYM.q());
    /*p30.DOBO*/ SPR_TRI_INDX_0 = tribuf_6n(GYGY_SPRITE5_GETn, EKOP.q());
    /*p30.AWAT*/ SPR_TRI_LINE_0 = tribuf_6n(GYGY_SPRITE5_GETn, ANED.q());
    /*p30.BACE*/ SPR_TRI_LINE_1 = tribuf_6n(GYGY_SPRITE5_GETn, ACEP.q());
    /*p30.BODU*/ SPR_TRI_LINE_2 = tribuf_6n(GYGY_SPRITE5_GETn, ABUX.q());
    /*p30.BUJA*/ SPR_TRI_LINE_3 = tribuf_6n(GYGY_SPRITE5_GETn, ABEG.q());

    /*p30.WOCY*/ SPR_TRI_INDX_5 = tribuf_6n(XYME_SPRITE6_GETn, GECU.q());
    /*p30.ELYC*/ SPR_TRI_INDX_4 = tribuf_6n(XYME_SPRITE6_GETn, FOXY.q());
    /*p30.WABO*/ SPR_TRI_INDX_3 = tribuf_6n(XYME_SPRITE6_GETn, GOHU.q());
    /*p30.EZOC*/ SPR_TRI_INDX_2 = tribuf_6n(XYME_SPRITE6_GETn, FOGO.q());
    /*p30.WYWY*/ SPR_TRI_INDX_1 = tribuf_6n(XYME_SPRITE6_GETn, GACY.q());
    /*p30.WATO*/ SPR_TRI_INDX_0 = tribuf_6n(XYME_SPRITE6_GETn, GABO.q());
    /*p30.ZUDO*/ SPR_TRI_LINE_0 = tribuf_6n(XYME_SPRITE6_GETn, ZUBE.q());
    /*p30.YBUK*/ SPR_TRI_LINE_1 = tribuf_6n(XYME_SPRITE6_GETn, ZUMY.q());
    /*p30.ZYTO*/ SPR_TRI_LINE_2 = tribuf_6n(XYME_SPRITE6_GETn, ZEXO.q());
    /*p30.YKOZ*/ SPR_TRI_LINE_3 = tribuf_6n(XYME_SPRITE6_GETn, ZAFU.q());

    /*p30.EVYT*/ SPR_TRI_INDX_5 = tribuf_6n(GUGY_SPRITE7_GETn, FUZO.q());
    /*p30.WABA*/ SPR_TRI_INDX_4 = tribuf_6n(GUGY_SPRITE7_GETn, GESY.q());
    /*p30.ETAD*/ SPR_TRI_INDX_3 = tribuf_6n(GUGY_SPRITE7_GETn, FYSU.q());
    /*p30.ELEP*/ SPR_TRI_INDX_2 = tribuf_6n(GUGY_SPRITE7_GETn, FEFA.q());
    /*p30.WYGO*/ SPR_TRI_INDX_1 = tribuf_6n(GUGY_SPRITE7_GETn, GYNO.q());
    /*p30.WAKO*/ SPR_TRI_INDX_0 = tribuf_6n(GUGY_SPRITE7_GETn, GULE.q());
    /*p30.WANA*/ SPR_TRI_LINE_0 = tribuf_6n(GUGY_SPRITE7_GETn, XYGO.q());
    /*p30.WAXE*/ SPR_TRI_LINE_1 = tribuf_6n(GUGY_SPRITE7_GETn, XYNA.q());
    /*p30.WABU*/ SPR_TRI_LINE_2 = tribuf_6n(GUGY_SPRITE7_GETn, XAKU.q());
    /*p30.YPOZ*/ SPR_TRI_LINE_3 = tribuf_6n(GUGY_SPRITE7_GETn, YGUM.q());

    /*p30.ADYB*/ SPR_TRI_INDX_5 = tribuf_6n(DENY_SPRITE8_GETn, STORE8_INDX5.q());
    /*p30.APOB*/ SPR_TRI_INDX_4 = tribuf_6n(DENY_SPRITE8_GETn, STORE8_INDX4.q());
    /*p30.APYV*/ SPR_TRI_INDX_3 = tribuf_6n(DENY_SPRITE8_GETn, STORE8_INDX3.q());
    /*p30.AFEN*/ SPR_TRI_INDX_2 = tribuf_6n(DENY_SPRITE8_GETn, STORE8_INDX2.q());
    /*p30.AKYH*/ SPR_TRI_INDX_1 = tribuf_6n(DENY_SPRITE8_GETn, STORE8_INDX1.q());
    /*p30.APOC*/ SPR_TRI_INDX_0 = tribuf_6n(DENY_SPRITE8_GETn, STORE8_INDX0.q());
    /*p30.BUJY*/ SPR_TRI_LINE_0 = tribuf_6n(DENY_SPRITE8_GETn, STORE8_LINE0.q());
    /*p30.BOSO*/ SPR_TRI_LINE_1 = tribuf_6n(DENY_SPRITE8_GETn, STORE8_LINE1.q());
    /*p30.AHAC*/ SPR_TRI_LINE_2 = tribuf_6n(DENY_SPRITE8_GETn, STORE8_LINE2.q());
    /*p30.BAZU*/ SPR_TRI_LINE_3 = tribuf_6n(DENY_SPRITE8_GETn, STORE8_LINE3.q());

    /*p30.YHAL*/ SPR_TRI_INDX_5 = tribuf_6n(FADO_SPRITE9_GETn, STORE9_INDX0.q());
    /*p30.YRAD*/ SPR_TRI_INDX_4 = tribuf_6n(FADO_SPRITE9_GETn, STORE9_INDX1.q());
    /*p30.XYRA*/ SPR_TRI_INDX_3 = tribuf_6n(FADO_SPRITE9_GETn, STORE9_INDX2.q());
    /*p30.YNEV*/ SPR_TRI_INDX_2 = tribuf_6n(FADO_SPRITE9_GETn, STORE9_INDX3.q());
    /*p30.ZOJY*/ SPR_TRI_INDX_1 = tribuf_6n(FADO_SPRITE9_GETn, STORE9_INDX4.q());
    /*p30.ZARO*/ SPR_TRI_INDX_0 = tribuf_6n(FADO_SPRITE9_GETn, STORE9_INDX5.q());
    /*p30.CAWO*/ SPR_TRI_LINE_0 = tribuf_6n(FADO_SPRITE9_GETn, STORE9_LINE0.q());
    /*p30.BYME*/ SPR_TRI_LINE_1 = tribuf_6n(FADO_SPRITE9_GETn, STORE9_LINE1.q());
    /*p30.COHO*/ SPR_TRI_LINE_2 = tribuf_6n(FADO_SPRITE9_GETn, STORE9_LINE2.q());
    /*p30.GATE*/ SPR_TRI_LINE_3 = tribuf_6n(FADO_SPRITE9_GETn, STORE9_LINE3.q());
  }

  //----------------------------------------
  // Sprite store setter

  {
    /*p28.ABAK*/ wire _ABAK_VID_LINE_TRIGp = or (top.lcd_reg.ATEJ_VID_LINE_TRIGp(), top.clk_reg.AMYG_VID_RSTp());
    /*p28.BYVA*/ wire _BYVA_VID_LINE_TRIGn = not(_ABAK_VID_LINE_TRIGp);
    /*p29.DYBA*/ wire _DYBA_VID_LINE_TRIGp = not(_BYVA_VID_LINE_TRIGn);

    /*p29.DYWE*/ wire DYWE_STORE0_RSTp = or(_DYBA_VID_LINE_TRIGp, EBOJ_STORE0_RSTp.q());
    /*p29.EFEV*/ wire EFEV_STORE1_RSTp = or(_DYBA_VID_LINE_TRIGp, CEDY_STORE1_RSTp.q());
    /*p29.FOKO*/ wire FOKO_STORE2_RSTp = or(_DYBA_VID_LINE_TRIGp, EGAV_STORE2_RSTp.q());
    /*p29.GAKE*/ wire GAKE_STORE3_RSTp = or(_DYBA_VID_LINE_TRIGp, GOTA_STORE3_RSTp.q());
    /*p29.WOHU*/ wire WOHU_STORE4_RSTp = or(_DYBA_VID_LINE_TRIGp, XUDY_STORE4_RSTp.q());
    /*p29.FEVE*/ wire FEVE_STORE5_RSTp = or(_DYBA_VID_LINE_TRIGp, WAFY_STORE5_RSTp.q());
    /*p29.WACY*/ wire WACY_STORE6_RSTp = or(_DYBA_VID_LINE_TRIGp, WOMY_STORE6_RSTp.q());
    /*p29.GUKY*/ wire GUKY_STORE7_RSTp = or(_DYBA_VID_LINE_TRIGp, WAPO_STORE7_RSTp.q());
    /*p29.GORO*/ wire GORO_STORE8_RSTp = or(_DYBA_VID_LINE_TRIGp, EXUQ_STORE8_RSTp.q());
    /*p29.DUBU*/ wire DUBU_STORE9_RSTp = or(_DYBA_VID_LINE_TRIGp, FONO_STORE9_RSTp.q());

    /*p29.DYNA*/ wire DYNA_STORE0_RSTn = not(DYWE_STORE0_RSTp);
    /*p29.DOKU*/ wire DOKU_STORE1_RSTn = not(EFEV_STORE1_RSTp);
    /*p29.GAMY*/ wire GAMY_STORE2_RSTn = not(FOKO_STORE2_RSTp);
    /*p29.WUPA*/ wire WUPA_STORE3_RSTn = not(GAKE_STORE3_RSTp);
    /*p29.WUNU*/ wire WUNU_STORE4_RSTn = not(WOHU_STORE4_RSTp);
    /*p29.EJAD*/ wire EJAD_STORE5_RSTn = not(FEVE_STORE5_RSTp);
    /*p29.XAHO*/ wire XAHO_STORE6_RSTn = not(WACY_STORE6_RSTp);
    /*p29.GAFY*/ wire GAFY_STORE7_RSTn = not(GUKY_STORE7_RSTp);
    /*p29.WUZO*/ wire WUZO_STORE8_RSTn = not(GORO_STORE8_RSTp);
    /*p29.DOSY*/ wire DOSY_STORE9_RSTn = not(DUBU_STORE9_RSTp);

    /*p29.EDEN*/ wire EDEN_SPRITE_COUNT0n = not(SPRITE_COUNT0.q());
    /*p29.CYPY*/ wire CYPY_SPRITE_COUNT1n = not(SPRITE_COUNT1.q());
    /*p29.CAPE*/ wire CAPE_SPRITE_COUNT2n = not(SPRITE_COUNT2.q());
    /*p29.CAXU*/ wire CAXU_SPRITE_COUNT3n = not(SPRITE_COUNT3.q());

    /*p29.FYCU*/ wire FYCU_SPRITE_COUNT0p = not(EDEN_SPRITE_COUNT0n);
    /*p29.FONE*/ wire FONE_SPRITE_COUNT1p = not(CYPY_SPRITE_COUNT1n);
    /*p29.EKUD*/ wire EKUD_SPRITE_COUNT2p = not(CAPE_SPRITE_COUNT2n);
    /*p29.ELYG*/ wire ELYG_SPRITE_COUNT3p = not(CAXU_SPRITE_COUNT3n);

    /*p29.DEZO*/ wire DEZO_STORE0_SELn = nand(EDEN_SPRITE_COUNT0n, CYPY_SPRITE_COUNT1n, CAPE_SPRITE_COUNT2n, CAXU_SPRITE_COUNT3n);
    /*p29.CUVA*/ wire CUVA_STORE1_SELn = nand(FYCU_SPRITE_COUNT0p, CYPY_SPRITE_COUNT1n, CAPE_SPRITE_COUNT2n, CAXU_SPRITE_COUNT3n);
    /*p29.GEBU*/ wire GEBU_STORE2_SELn = nand(EDEN_SPRITE_COUNT0n, FONE_SPRITE_COUNT1p, CAPE_SPRITE_COUNT2n, CAXU_SPRITE_COUNT3n);
    /*p29.FOCO*/ wire FOCO_STORE3_SELn = nand(FYCU_SPRITE_COUNT0p, FONE_SPRITE_COUNT1p, CAPE_SPRITE_COUNT2n, CAXU_SPRITE_COUNT3n);
    /*p29.CUPE*/ wire CUPE_STORE4_SELn = nand(EDEN_SPRITE_COUNT0n, CYPY_SPRITE_COUNT1n, EKUD_SPRITE_COUNT2p, CAXU_SPRITE_COUNT3n);
    /*p29.CUGU*/ wire CUGU_STORE5_SELn = nand(FYCU_SPRITE_COUNT0p, CYPY_SPRITE_COUNT1n, EKUD_SPRITE_COUNT2p, CAXU_SPRITE_COUNT3n);
    /*p29.WOMU*/ wire WOMU_STORE6_SELn = nand(EDEN_SPRITE_COUNT0n, FONE_SPRITE_COUNT1p, EKUD_SPRITE_COUNT2p, CAXU_SPRITE_COUNT3n);
    /*p29.GUNA*/ wire GUNA_STORE7_SELn = nand(FYCU_SPRITE_COUNT0p, FONE_SPRITE_COUNT1p, EKUD_SPRITE_COUNT2p, CAXU_SPRITE_COUNT3n);
    /*p29.DEWY*/ wire DEWY_STORE8_SELn = nand(EDEN_SPRITE_COUNT0n, CYPY_SPRITE_COUNT1n, CAPE_SPRITE_COUNT2n, ELYG_SPRITE_COUNT3p);
    /*p29.DOGU*/ wire DOGU_STORE9_SELn = nand(FYCU_SPRITE_COUNT0p, CYPY_SPRITE_COUNT1n, CAPE_SPRITE_COUNT2n, ELYG_SPRITE_COUNT3p);

    // Sprite stores latch their input when their SELn signal goes _high_
    /*p29.CEMY*/ wire CEMY_STORE0_CLKp = or(top.sprite_scanner.DYTY_STORE_ENn_xxCDxxGH(), DEZO_STORE0_SELn);
    /*p29.BYBY*/ wire BYBY_STORE1_CLKp = or(top.sprite_scanner.DYTY_STORE_ENn_xxCDxxGH(), CUVA_STORE1_SELn);
    /*p29.WYXO*/ wire WYXO_STORE2_CLKp = or(top.sprite_scanner.DYTY_STORE_ENn_xxCDxxGH(), GEBU_STORE2_SELn);
    /*p29.GUVE*/ wire GUVE_STORE3_CLKp = or(top.sprite_scanner.DYTY_STORE_ENn_xxCDxxGH(), FOCO_STORE3_SELn);
    /*p29.CECU*/ wire CECU_STORE4_CLKp = or(top.sprite_scanner.DYTY_STORE_ENn_xxCDxxGH(), CUPE_STORE4_SELn);
    /*p29.CADO*/ wire CADO_STORE5_CLKp = or(top.sprite_scanner.DYTY_STORE_ENn_xxCDxxGH(), CUGU_STORE5_SELn);
    /*p29.XUJO*/ wire XUJO_STORE6_CLKp = or(top.sprite_scanner.DYTY_STORE_ENn_xxCDxxGH(), WOMU_STORE6_SELn);
    /*p29.GAPE*/ wire GAPE_STORE7_CLKp = or(top.sprite_scanner.DYTY_STORE_ENn_xxCDxxGH(), GUNA_STORE7_SELn);
    /*p29.CAHO*/ wire CAHO_STORE8_CLKp = or(top.sprite_scanner.DYTY_STORE_ENn_xxCDxxGH(), DEWY_STORE8_SELn);
    /*p29.CATO*/ wire CATO_STORE9_CLKp = or(top.sprite_scanner.DYTY_STORE_ENn_xxCDxxGH(), DOGU_STORE9_SELn);

    /*p29.DYHU*/ wire DYHU_STORE0_CLKn = not(CEMY_STORE0_CLKp);
    /*p29.BUCO*/ wire BUCO_STORE1_CLKn = not(BYBY_STORE1_CLKp);
    /*p29.GYFO*/ wire GYFO_STORE2_CLKn = not(WYXO_STORE2_CLKp);
    /*p29.GUSA*/ wire GUSA_STORE3_CLKn = not(GUVE_STORE3_CLKp);
    /*p29.DUKE*/ wire DUKE_STORE4_CLKn = not(CECU_STORE4_CLKp);
    /*p29.BEDE*/ wire BEDE_STORE5_CLKn = not(CADO_STORE5_CLKp);
    /*p29.WEKA*/ wire WEKA_STORE6_CLKn = not(XUJO_STORE6_CLKp);
    /*p29.GYVO*/ wire GYVO_STORE7_CLKn = not(GAPE_STORE7_CLKp);
    /*p29.BUKA*/ wire BUKA_STORE8_CLKn = not(CAHO_STORE8_CLKp);
    /*p29.DECU*/ wire DECU_STORE9_CLKn = not(CATO_STORE9_CLKp);

    // 10 sprite stores

    /*p31.ZAGO*/ wire ZAGO_OAM_DA0 = not(top.oam_bus.YLOR_OAM_DA0.qn());
    /*p31.ZOCY*/ wire ZOCY_OAM_DA1 = not(top.oam_bus.ZYTY_OAM_DA1.qn());
    /*p31.YPUR*/ wire YPUR_OAM_DA2 = not(top.oam_bus.ZYVE_OAM_DA2.qn());
    /*p31.YVOK*/ wire YVOK_OAM_DA3 = not(top.oam_bus.ZEZY_OAM_DA3.qn());
    /*p31.COSE*/ wire COSE_OAM_DA4 = not(top.oam_bus.GOMO_OAM_DA4.qn());
    /*p31.AROP*/ wire AROP_OAM_DA5 = not(top.oam_bus.BAXO_OAM_DA5.qn());
    /*p31.XATU*/ wire XATU_OAM_DA6 = not(top.oam_bus.YZOS_OAM_DA6.qn());
    /*p31.BADY*/ wire BADY_OAM_DA7 = not(top.oam_bus.DEPO_OAM_DA7.qn());

    /*p29.GENY*/ wire GENY_STORE0_CLKp = not(DYHU_STORE0_CLKn);
    /*p29.ENOB*/ wire ENOB_STORE0_CLKp = not(DYHU_STORE0_CLKn);
    /*p29.FUXU*/ wire FUXU_STORE0_CLKp = not(DYHU_STORE0_CLKn);
    /*p30.YWAK*/ YWAK_STORE0_INDX0 = dff8(GENY_STORE0_CLKp, !GENY_STORE0_CLKp, SPR_TRI_INDX_0);
    /*p30.ZONY*/ ZONY_STORE0_INDX1 = dff8(GENY_STORE0_CLKp, !GENY_STORE0_CLKp, SPR_TRI_INDX_1);
    /*p30.WYTE*/ WYTE_STORE0_INDX2 = dff8(GENY_STORE0_CLKp, !GENY_STORE0_CLKp, SPR_TRI_INDX_2);
    /*p30.YZEP*/ YZEP_STORE0_INDX3 = dff8(GENY_STORE0_CLKp, !GENY_STORE0_CLKp, SPR_TRI_INDX_3);
    /*p30.YSOK*/ YSOK_STORE0_INDX4 = dff8(GENY_STORE0_CLKp, !GENY_STORE0_CLKp, SPR_TRI_INDX_4);
    /*p30.YGUS*/ YGUS_STORE0_INDX5 = dff8(GENY_STORE0_CLKp, !GENY_STORE0_CLKp, SPR_TRI_INDX_5);
    /*p30.FYHY*/ FYHY_STORE0_LINE0 = dff8(ENOB_STORE0_CLKp, !ENOB_STORE0_CLKp, SPR_TRI_LINE_0);
    /*p30.GYHO*/ GYHO_STORE0_LINE1 = dff8(ENOB_STORE0_CLKp, !ENOB_STORE0_CLKp, SPR_TRI_LINE_1);
    /*p30.BOZU*/ BOZU_STORE0_LINE2 = dff8(ENOB_STORE0_CLKp, !ENOB_STORE0_CLKp, SPR_TRI_LINE_2);
    /*p30.CUFO*/ CUFO_STORE0_LINE3 = dff8(ENOB_STORE0_CLKp, !ENOB_STORE0_CLKp, SPR_TRI_LINE_3);
    /*p31.XEPE*/ XEPE_STORE0_X0    = dff9(FUXU_STORE0_CLKp, !FUXU_STORE0_CLKp, DYNA_STORE0_RSTn, ZAGO_OAM_DA0);
    /*p31.YLAH*/ YLAH_STORE0_X1    = dff9(FUXU_STORE0_CLKp, !FUXU_STORE0_CLKp, DYNA_STORE0_RSTn, ZOCY_OAM_DA1);
    /*p31.ZOLA*/ ZOLA_STORE0_X2    = dff9(FUXU_STORE0_CLKp, !FUXU_STORE0_CLKp, DYNA_STORE0_RSTn, YPUR_OAM_DA2);
    /*p31.ZULU*/ ZULU_STORE0_X3    = dff9(FUXU_STORE0_CLKp, !FUXU_STORE0_CLKp, DYNA_STORE0_RSTn, YVOK_OAM_DA3);
    /*p31.WELO*/ WELO_STORE0_X4    = dff9(FUXU_STORE0_CLKp, !FUXU_STORE0_CLKp, DYNA_STORE0_RSTn, COSE_OAM_DA4);
    /*p31.XUNY*/ XUNY_STORE0_X5    = dff9(FUXU_STORE0_CLKp, !FUXU_STORE0_CLKp, DYNA_STORE0_RSTn, AROP_OAM_DA5);
    /*p31.WOTE*/ WOTE_STORE0_X6    = dff9(FUXU_STORE0_CLKp, !FUXU_STORE0_CLKp, DYNA_STORE0_RSTn, XATU_OAM_DA6);
    /*p31.XAKO*/ XAKO_STORE0_X7    = dff9(FUXU_STORE0_CLKp, !FUXU_STORE0_CLKp, DYNA_STORE0_RSTn, BADY_OAM_DA7);

    /*p29.BYVY*/ wire BYVU_STORE1_CLKp = not(BUCO_STORE1_CLKn);
    /*p29.AHOF*/ wire AHOF_STORE1_CLKp = not(BUCO_STORE1_CLKn);
    /*p29.ASYS*/ wire ASYS_STORE1_CLKp = not(BUCO_STORE1_CLKn);
    /*p30.CAJY*/ STORE1_INDX0 = dff8(BYVU_STORE1_CLKp, !BYVU_STORE1_CLKp, SPR_TRI_INDX_5);
    /*p30.CUZA*/ STORE1_INDX1 = dff8(BYVU_STORE1_CLKp, !BYVU_STORE1_CLKp, SPR_TRI_INDX_4);
    /*p30.COMA*/ STORE1_INDX2 = dff8(BYVU_STORE1_CLKp, !BYVU_STORE1_CLKp, SPR_TRI_INDX_3);
    /*p30.CUFA*/ STORE1_INDX3 = dff8(BYVU_STORE1_CLKp, !BYVU_STORE1_CLKp, SPR_TRI_INDX_2);
    /*p30.CEBO*/ STORE1_INDX4 = dff8(BYVU_STORE1_CLKp, !BYVU_STORE1_CLKp, SPR_TRI_INDX_1);
    /*p30.CADU*/ STORE1_INDX5 = dff8(BYVU_STORE1_CLKp, !BYVU_STORE1_CLKp, SPR_TRI_INDX_0);
    /*p30.ABUG*/ STORE1_LINE0 = dff8(AHOF_STORE1_CLKp, !AHOF_STORE1_CLKp, SPR_TRI_LINE_0);
    /*p30.AMES*/ STORE1_LINE1 = dff8(AHOF_STORE1_CLKp, !AHOF_STORE1_CLKp, SPR_TRI_LINE_1);
    /*p30.ABOP*/ STORE1_LINE2 = dff8(AHOF_STORE1_CLKp, !AHOF_STORE1_CLKp, SPR_TRI_LINE_2);
    /*p30.AROF*/ STORE1_LINE3 = dff8(AHOF_STORE1_CLKp, !AHOF_STORE1_CLKp, SPR_TRI_LINE_3);
    /*p31.DANY*/ STORE1_X0    = dff9(ASYS_STORE1_CLKp, !ASYS_STORE1_CLKp, DOKU_STORE1_RSTn, ZAGO_OAM_DA0);
    /*p31.DUKO*/ STORE1_X1    = dff9(ASYS_STORE1_CLKp, !ASYS_STORE1_CLKp, DOKU_STORE1_RSTn, ZOCY_OAM_DA1);
    /*p31.DESU*/ STORE1_X2    = dff9(ASYS_STORE1_CLKp, !ASYS_STORE1_CLKp, DOKU_STORE1_RSTn, YPUR_OAM_DA2);
    /*p31.DAZO*/ STORE1_X3    = dff9(ASYS_STORE1_CLKp, !ASYS_STORE1_CLKp, DOKU_STORE1_RSTn, YVOK_OAM_DA3);
    /*p31.DAKE*/ STORE1_X4    = dff9(ASYS_STORE1_CLKp, !ASYS_STORE1_CLKp, DOKU_STORE1_RSTn, COSE_OAM_DA4);
    /*p31.CESO*/ STORE1_X5    = dff9(ASYS_STORE1_CLKp, !ASYS_STORE1_CLKp, DOKU_STORE1_RSTn, AROP_OAM_DA5);
    /*p31.DYFU*/ STORE1_X6    = dff9(ASYS_STORE1_CLKp, !ASYS_STORE1_CLKp, DOKU_STORE1_RSTn, XATU_OAM_DA6);
    /*p31.CUSY*/ STORE1_X7    = dff9(ASYS_STORE1_CLKp, !ASYS_STORE1_CLKp, DOKU_STORE1_RSTn, BADY_OAM_DA7);

    /*p29.BUZY*/ wire BUZY_STORE2_CLKp = not(GYFO_STORE2_CLKn);
    /*p29.FUKE*/ wire FUKE_STORE2_CLKp = not(GYFO_STORE2_CLKn);
    /*p29.CACU*/ wire CACU_STORE2_CLKp = not(GYFO_STORE2_CLKn);
    /*p30.BOXA*/ STORE2_INDX0 = dff8(BUZY_STORE2_CLKp, !BUZY_STORE2_CLKp, SPR_TRI_INDX_5);
    /*p30.BUNA*/ STORE2_INDX1 = dff8(BUZY_STORE2_CLKp, !BUZY_STORE2_CLKp, SPR_TRI_INDX_4);
    /*p30.BULU*/ STORE2_INDX2 = dff8(BUZY_STORE2_CLKp, !BUZY_STORE2_CLKp, SPR_TRI_INDX_3);
    /*p30.BECA*/ STORE2_INDX3 = dff8(BUZY_STORE2_CLKp, !BUZY_STORE2_CLKp, SPR_TRI_INDX_2);
    /*p30.BYHU*/ STORE2_INDX4 = dff8(BUZY_STORE2_CLKp, !BUZY_STORE2_CLKp, SPR_TRI_INDX_1);
    /*p30.BUHE*/ STORE2_INDX5 = dff8(BUZY_STORE2_CLKp, !BUZY_STORE2_CLKp, SPR_TRI_INDX_0);
    /*p30.YKUK*/ STORE2_LINE0 = dff8(FUKE_STORE2_CLKp, !FUKE_STORE2_CLKp, SPR_TRI_LINE_0);
    /*p30.YLOV*/ STORE2_LINE1 = dff8(FUKE_STORE2_CLKp, !FUKE_STORE2_CLKp, SPR_TRI_LINE_1);
    /*p30.XAZY*/ STORE2_LINE2 = dff8(FUKE_STORE2_CLKp, !FUKE_STORE2_CLKp, SPR_TRI_LINE_2);
    /*p30.XOSY*/ STORE2_LINE3 = dff8(FUKE_STORE2_CLKp, !FUKE_STORE2_CLKp, SPR_TRI_LINE_3);
    /*p31.FOKA*/ STORE2_X0    = dff9(CACU_STORE2_CLKp, !CACU_STORE2_CLKp, GAMY_STORE2_RSTn, ZAGO_OAM_DA0);
    /*p31.FYTY*/ STORE2_X1    = dff9(CACU_STORE2_CLKp, !CACU_STORE2_CLKp, GAMY_STORE2_RSTn, ZOCY_OAM_DA1);
    /*p31.FUBY*/ STORE2_X2    = dff9(CACU_STORE2_CLKp, !CACU_STORE2_CLKp, GAMY_STORE2_RSTn, YPUR_OAM_DA2);
    /*p31.GOXU*/ STORE2_X3    = dff9(CACU_STORE2_CLKp, !CACU_STORE2_CLKp, GAMY_STORE2_RSTn, YVOK_OAM_DA3);
    /*p31.DUHY*/ STORE2_X4    = dff9(CACU_STORE2_CLKp, !CACU_STORE2_CLKp, GAMY_STORE2_RSTn, COSE_OAM_DA4);
    /*p31.EJUF*/ STORE2_X5    = dff9(CACU_STORE2_CLKp, !CACU_STORE2_CLKp, GAMY_STORE2_RSTn, AROP_OAM_DA5);
    /*p31.ENOR*/ STORE2_X6    = dff9(CACU_STORE2_CLKp, !CACU_STORE2_CLKp, GAMY_STORE2_RSTn, XATU_OAM_DA6);
    /*p31.DEPY*/ STORE2_X7    = dff9(CACU_STORE2_CLKp, !CACU_STORE2_CLKp, GAMY_STORE2_RSTn, BADY_OAM_DA7);

    /*p29.FEKA*/ wire FEKA = not(GUSA_STORE3_CLKn);
    /*p29.XYHA*/ wire XYHA = not(GUSA_STORE3_CLKn);
    /*p29.YFAG*/ wire YFAG = not(GUSA_STORE3_CLKn);
    /*p30.DAFU*/ STORE3_INDX0 = dff8(FEKA, !FEKA, SPR_TRI_INDX_5);
    /*p30.DEBA*/ STORE3_INDX1 = dff8(FEKA, !FEKA, SPR_TRI_INDX_4);
    /*p30.DUHA*/ STORE3_INDX2 = dff8(FEKA, !FEKA, SPR_TRI_INDX_3);
    /*p30.DUNY*/ STORE3_INDX3 = dff8(FEKA, !FEKA, SPR_TRI_INDX_2);
    /*p30.DESE*/ STORE3_INDX4 = dff8(FEKA, !FEKA, SPR_TRI_INDX_1);
    /*p30.DEVY*/ STORE3_INDX5 = dff8(FEKA, !FEKA, SPR_TRI_INDX_0);
    /*p30.ZURY*/ STORE3_LINE0 = dff8(XYHA, !XYHA, SPR_TRI_LINE_0);
    /*p30.ZURO*/ STORE3_LINE1 = dff8(XYHA, !XYHA, SPR_TRI_LINE_1);
    /*p30.ZENE*/ STORE3_LINE2 = dff8(XYHA, !XYHA, SPR_TRI_LINE_2);
    /*p30.ZYLU*/ STORE3_LINE4 = dff8(XYHA, !XYHA, SPR_TRI_LINE_3);
    /*p31.XOLY*/ STORE3_X0    = dff9(YFAG, !YFAG, WUPA_STORE3_RSTn, ZAGO_OAM_DA0);
    /*p31.XYBA*/ STORE3_X1    = dff9(YFAG, !YFAG, WUPA_STORE3_RSTn, ZOCY_OAM_DA1);
    /*p31.XABE*/ STORE3_X2    = dff9(YFAG, !YFAG, WUPA_STORE3_RSTn, YPUR_OAM_DA2);
    /*p31.XEKA*/ STORE3_X3    = dff9(YFAG, !YFAG, WUPA_STORE3_RSTn, YVOK_OAM_DA3);
    /*p31.XOMY*/ STORE3_X4    = dff9(YFAG, !YFAG, WUPA_STORE3_RSTn, COSE_OAM_DA4);
    /*p31.WUHA*/ STORE3_X5    = dff9(YFAG, !YFAG, WUPA_STORE3_RSTn, AROP_OAM_DA5);
    /*p31.WYNA*/ STORE3_X6    = dff9(YFAG, !YFAG, WUPA_STORE3_RSTn, XATU_OAM_DA6);
    /*p31.WECO*/ STORE3_X7    = dff9(YFAG, !YFAG, WUPA_STORE3_RSTn, BADY_OAM_DA7);

    /*p29.WOFO*/ wire WOFO = not(DUKE_STORE4_CLKn);
    /*p29.WYLU*/ wire WYLU = not(DUKE_STORE4_CLKn);
    /*p29.EWOT*/ wire EWOT = not(DUKE_STORE4_CLKn);
    /*p30.XYNU*/ XYNU      = dff8(WYLU, !WYLU, SPR_TRI_INDX_5);
    /*p30.XEGE*/ XEGE      = dff8(WYLU, !WYLU, SPR_TRI_INDX_4);
    /*p30.XABO*/ XABO      = dff8(WYLU, !WYLU, SPR_TRI_INDX_3);
    /*p30.WANU*/ WANU      = dff8(WYLU, !WYLU, SPR_TRI_INDX_2);
    /*p30.XEFE*/ XEFE      = dff8(WYLU, !WYLU, SPR_TRI_INDX_1);
    /*p30.XAVE*/ XAVE      = dff8(WYLU, !WYLU, SPR_TRI_INDX_0);
    /*p30.CUMU*/ CUMU      = dff8(EWOT, !EWOT, SPR_TRI_LINE_0);
    /*p30.CAPO*/ CAPO      = dff8(EWOT, !EWOT, SPR_TRI_LINE_1);
    /*p30.CONO*/ CONO      = dff8(EWOT, !EWOT, SPR_TRI_LINE_2);
    /*p30.CAJU*/ CAJU      = dff8(EWOT, !EWOT, SPR_TRI_LINE_3);
    /*p31.WEDU*/ STORE4_X0 = dff9(WOFO, !WOFO, WUNU_STORE4_RSTn, ZAGO_OAM_DA0);
    /*p31.YGAJ*/ STORE4_X1 = dff9(WOFO, !WOFO, WUNU_STORE4_RSTn, ZOCY_OAM_DA1);
    /*p31.ZYJO*/ STORE4_X2 = dff9(WOFO, !WOFO, WUNU_STORE4_RSTn, YPUR_OAM_DA2);
    /*p31.XURY*/ STORE4_X3 = dff9(WOFO, !WOFO, WUNU_STORE4_RSTn, YVOK_OAM_DA3);
    /*p31.YBED*/ STORE4_X4 = dff9(WOFO, !WOFO, WUNU_STORE4_RSTn, COSE_OAM_DA4);
    /*p31.ZALA*/ STORE4_X5 = dff9(WOFO, !WOFO, WUNU_STORE4_RSTn, AROP_OAM_DA5);
    /*p31.WYDE*/ STORE4_X6 = dff9(WOFO, !WOFO, WUNU_STORE4_RSTn, XATU_OAM_DA6);
    /*p31.XEPA*/ STORE4_X7 = dff9(WOFO, !WOFO, WUNU_STORE4_RSTn, BADY_OAM_DA7);

    /*p29.CYLA*/ wire CYLA = not(BEDE_STORE5_CLKn);
    /*p29.DYMO*/ wire DYMO = not(BEDE_STORE5_CLKn);
    /*p29.BUCY*/ wire BUCY = not(BEDE_STORE5_CLKn);
    /*p30.EKAP*/ EKAP      = dff8(DYMO, !DYMO, SPR_TRI_INDX_5);
    /*p30.ETAV*/ ETAV      = dff8(DYMO, !DYMO, SPR_TRI_INDX_4);
    /*p30.EBEX*/ EBEX      = dff8(DYMO, !DYMO, SPR_TRI_INDX_3);
    /*p30.GORU*/ GORU      = dff8(DYMO, !DYMO, SPR_TRI_INDX_2);
    /*p30.ETYM*/ ETYM      = dff8(DYMO, !DYMO, SPR_TRI_INDX_1);
    /*p30.EKOP*/ EKOP      = dff8(DYMO, !DYMO, SPR_TRI_INDX_0);
    /*p30.ANED*/ ANED      = dff8(BUCY, !BUCY, SPR_TRI_LINE_0);
    /*p30.ACEP*/ ACEP      = dff8(BUCY, !BUCY, SPR_TRI_LINE_1);
    /*p30.ABUX*/ ABUX      = dff8(BUCY, !BUCY, SPR_TRI_LINE_2);
    /*p30.ABEG*/ ABEG      = dff8(BUCY, !BUCY, SPR_TRI_LINE_3);
    /*p31.FUSA*/ STORE5_X0 = dff9(CYLA, !CYLA, EJAD_STORE5_RSTn, ZAGO_OAM_DA0);
    /*p31.FAXA*/ STORE5_X1 = dff9(CYLA, !CYLA, EJAD_STORE5_RSTn, ZOCY_OAM_DA1);
    /*p31.FOZY*/ STORE5_X2 = dff9(CYLA, !CYLA, EJAD_STORE5_RSTn, YPUR_OAM_DA2);
    /*p31.FESY*/ STORE5_X3 = dff9(CYLA, !CYLA, EJAD_STORE5_RSTn, YVOK_OAM_DA3);
    /*p31.CYWE*/ STORE5_X4 = dff9(CYLA, !CYLA, EJAD_STORE5_RSTn, COSE_OAM_DA4);
    /*p31.DYBY*/ STORE5_X5 = dff9(CYLA, !CYLA, EJAD_STORE5_RSTn, AROP_OAM_DA5);
    /*p31.DURY*/ STORE5_X6 = dff9(CYLA, !CYLA, EJAD_STORE5_RSTn, XATU_OAM_DA6);
    /*p31.CUVY*/ STORE5_X7 = dff9(CYLA, !CYLA, EJAD_STORE5_RSTn, BADY_OAM_DA7);

    /*p29.ZAPE*/ wire ZAPE = not(WEKA_STORE6_CLKn);
    /*p29.WUSE*/ wire WUSE = not(WEKA_STORE6_CLKn);
    /*p29.ZURU*/ wire ZURU = not(WEKA_STORE6_CLKn);
    /*p30.GECU*/ GECU      = dff8(WUSE, !WUSE, SPR_TRI_INDX_5);
    /*p30.FOXY*/ FOXY      = dff8(WUSE, !WUSE, SPR_TRI_INDX_4);
    /*p30.GOHU*/ GOHU      = dff8(WUSE, !WUSE, SPR_TRI_INDX_3);
    /*p30.FOGO*/ FOGO      = dff8(WUSE, !WUSE, SPR_TRI_INDX_2);
    /*p30.GACY*/ GACY      = dff8(WUSE, !WUSE, SPR_TRI_INDX_1);
    /*p30.GABO*/ GABO      = dff8(WUSE, !WUSE, SPR_TRI_INDX_0);
    /*p30.ZUBE*/ ZUBE      = dff8(ZURU, !ZURU, SPR_TRI_LINE_0);
    /*p30.ZUMY*/ ZUMY      = dff8(ZURU, !ZURU, SPR_TRI_LINE_1);
    /*p30.ZEXO*/ ZEXO      = dff8(ZURU, !ZURU, SPR_TRI_LINE_2);
    /*p30.ZAFU*/ ZAFU      = dff8(ZURU, !ZURU, SPR_TRI_LINE_3);
    /*p31.YCOL*/ STORE6_X0 = dff9(ZAPE, !ZAPE, XAHO_STORE6_RSTn, ZAGO_OAM_DA0);
    /*p31.YRAC*/ STORE6_X1 = dff9(ZAPE, !ZAPE, XAHO_STORE6_RSTn, ZOCY_OAM_DA1);
    /*p31.YMEM*/ STORE6_X2 = dff9(ZAPE, !ZAPE, XAHO_STORE6_RSTn, YPUR_OAM_DA2);
    /*p31.YVAG*/ STORE6_X3 = dff9(ZAPE, !ZAPE, XAHO_STORE6_RSTn, YVOK_OAM_DA3);
    /*p31.ZOLY*/ STORE6_X4 = dff9(ZAPE, !ZAPE, XAHO_STORE6_RSTn, COSE_OAM_DA4);
    /*p31.ZOGO*/ STORE6_X5 = dff9(ZAPE, !ZAPE, XAHO_STORE6_RSTn, AROP_OAM_DA5);
    /*p31.ZECU*/ STORE6_X6 = dff9(ZAPE, !ZAPE, XAHO_STORE6_RSTn, XATU_OAM_DA6);
    /*p31.ZESA*/ STORE6_X7 = dff9(ZAPE, !ZAPE, XAHO_STORE6_RSTn, BADY_OAM_DA7);

    /*p29.GECY*/ wire GECY = not(GYVO_STORE7_CLKn);
    /*p29.FEFO*/ wire FEFO = not(GYVO_STORE7_CLKn);
    /*p29.WABE*/ wire WABE = not(GYVO_STORE7_CLKn);
    /*p30.FUZO*/ FUZO      = dff8(FEFO, !FEFO, SPR_TRI_INDX_5);
    /*p30.GESY*/ GESY      = dff8(FEFO, !FEFO, SPR_TRI_INDX_4);
    /*p30.FYSU*/ FYSU      = dff8(FEFO, !FEFO, SPR_TRI_INDX_3);
    /*p30.FEFA*/ FEFA      = dff8(FEFO, !FEFO, SPR_TRI_INDX_2);
    /*p30.GYNO*/ GYNO      = dff8(FEFO, !FEFO, SPR_TRI_INDX_1);
    /*p30.GULE*/ GULE      = dff8(FEFO, !FEFO, SPR_TRI_INDX_0);
    /*p30.XYGO*/ XYGO      = dff8(WABE, !WABE, SPR_TRI_LINE_0);
    /*p30.XYNA*/ XYNA      = dff8(WABE, !WABE, SPR_TRI_LINE_1);
    /*p30.XAKU*/ XAKU      = dff8(WABE, !WABE, SPR_TRI_LINE_2);
    /*p30.YGUM*/ YGUM      = dff8(WABE, !WABE, SPR_TRI_LINE_3);
    /*p31.ERAZ*/ STORE7_X0 = dff9(GECY, !GECY, GAFY_STORE7_RSTn, ZAGO_OAM_DA0);
    /*p31.EPUM*/ STORE7_X1 = dff9(GECY, !GECY, GAFY_STORE7_RSTn, ZOCY_OAM_DA1);
    /*p31.EROL*/ STORE7_X2 = dff9(GECY, !GECY, GAFY_STORE7_RSTn, YPUR_OAM_DA2);
    /*p31.EHYN*/ STORE7_X3 = dff9(GECY, !GECY, GAFY_STORE7_RSTn, YVOK_OAM_DA3);
    /*p31.FAZU*/ STORE7_X4 = dff9(GECY, !GECY, GAFY_STORE7_RSTn, COSE_OAM_DA4);
    /*p31.FAXE*/ STORE7_X5 = dff9(GECY, !GECY, GAFY_STORE7_RSTn, AROP_OAM_DA5);
    /*p31.EXUK*/ STORE7_X6 = dff9(GECY, !GECY, GAFY_STORE7_RSTn, XATU_OAM_DA6);
    /*p31.FEDE*/ STORE7_X7 = dff9(GECY, !GECY, GAFY_STORE7_RSTn, BADY_OAM_DA7);

    /*p29.CEXU*/ wire CEXU = not(BUKA_STORE8_CLKn);
    /*p29.AKOL*/ wire AKOL = not(BUKA_STORE8_CLKn);
    /*p29.BYMY*/ wire BYMY = not(BUKA_STORE8_CLKn);
    /*p30.AXUV*/ STORE8_INDX5 = dff8(AKOL, !AKOL, SPR_TRI_INDX_5);
    /*p30.BADA*/ STORE8_INDX4 = dff8(AKOL, !AKOL, SPR_TRI_INDX_4);
    /*p30.APEV*/ STORE8_INDX3 = dff8(AKOL, !AKOL, SPR_TRI_INDX_3);
    /*p30.BADO*/ STORE8_INDX2 = dff8(AKOL, !AKOL, SPR_TRI_INDX_2);
    /*p30.BEXY*/ STORE8_INDX1 = dff8(AKOL, !AKOL, SPR_TRI_INDX_1);
    /*p30.BYHE*/ STORE8_INDX0 = dff8(AKOL, !AKOL, SPR_TRI_INDX_0);
    /*p30.AFYM*/ STORE8_LINE0 = dff8(BYMY, !BYMY, SPR_TRI_LINE_0);
    /*p30.AZAP*/ STORE8_LINE1 = dff8(BYMY, !BYMY, SPR_TRI_LINE_1);
    /*p30.AFUT*/ STORE8_LINE2 = dff8(BYMY, !BYMY, SPR_TRI_LINE_2);
    /*p30.AFYX*/ STORE8_LINE3 = dff8(BYMY, !BYMY, SPR_TRI_LINE_3);
    /*p31.GAVY*/ STORE8_X4    = dff9(CEXU, !CEXU, WUZO_STORE8_RSTn, ZAGO_OAM_DA0);
    /*p31.GYPU*/ STORE8_X5    = dff9(CEXU, !CEXU, WUZO_STORE8_RSTn, ZOCY_OAM_DA1);
    /*p31.GADY*/ STORE8_X6    = dff9(CEXU, !CEXU, WUZO_STORE8_RSTn, YPUR_OAM_DA2);
    /*p31.GAZA*/ STORE8_X7    = dff9(CEXU, !CEXU, WUZO_STORE8_RSTn, YVOK_OAM_DA3);
    /*p31.EZUF*/ STORE8_X0    = dff9(CEXU, !CEXU, WUZO_STORE8_RSTn, COSE_OAM_DA4);
    /*p31.ENAD*/ STORE8_X1    = dff9(CEXU, !CEXU, WUZO_STORE8_RSTn, AROP_OAM_DA5);
    /*p31.EBOW*/ STORE8_X2    = dff9(CEXU, !CEXU, WUZO_STORE8_RSTn, XATU_OAM_DA6);
    /*p31.FYCA*/ STORE8_X3    = dff9(CEXU, !CEXU, WUZO_STORE8_RSTn, BADY_OAM_DA7);

    /*p29.WEME*/ wire STORE9_CLKa = not(DECU_STORE9_CLKn);
    /*p29.WUFA*/ wire STORE9_CLKb = not(DECU_STORE9_CLKn);
    /*p29.FAKA*/ wire STORE9_CLKc = not(DECU_STORE9_CLKn);
    /*p30.XUFO*/ STORE9_INDX0 = dff8(STORE9_CLKb, !STORE9_CLKb, SPR_TRI_INDX_5);
    /*p30.XUTE*/ STORE9_INDX1 = dff8(STORE9_CLKb, !STORE9_CLKb, SPR_TRI_INDX_4);
    /*p30.XOTU*/ STORE9_INDX2 = dff8(STORE9_CLKb, !STORE9_CLKb, SPR_TRI_INDX_3);
    /*p30.XYFE*/ STORE9_INDX3 = dff8(STORE9_CLKb, !STORE9_CLKb, SPR_TRI_INDX_2);
    /*p30.YZOR*/ STORE9_INDX4 = dff8(STORE9_CLKb, !STORE9_CLKb, SPR_TRI_INDX_1);
    /*p30.YBER*/ STORE9_INDX5 = dff8(STORE9_CLKb, !STORE9_CLKb, SPR_TRI_INDX_0);
    /*p30.DEWU*/ STORE9_LINE0 = dff8(STORE9_CLKc, !STORE9_CLKc, SPR_TRI_LINE_0);
    /*p30.CANA*/ STORE9_LINE1 = dff8(STORE9_CLKc, !STORE9_CLKc, SPR_TRI_LINE_1);
    /*p30.DYSY*/ STORE9_LINE2 = dff8(STORE9_CLKc, !STORE9_CLKc, SPR_TRI_LINE_2);
    /*p30.FOFO*/ STORE9_LINE3 = dff8(STORE9_CLKc, !STORE9_CLKc, SPR_TRI_LINE_3);
    /*p31.XUVY*/ STORE9_X0    = dff9(STORE9_CLKa, !STORE9_CLKa, DOSY_STORE9_RSTn, ZAGO_OAM_DA0);
    /*p31.XERE*/ STORE9_X1    = dff9(STORE9_CLKa, !STORE9_CLKa, DOSY_STORE9_RSTn, ZOCY_OAM_DA1);
    /*p31.XUZO*/ STORE9_X2    = dff9(STORE9_CLKa, !STORE9_CLKa, DOSY_STORE9_RSTn, YPUR_OAM_DA2);
    /*p31.XEXA*/ STORE9_X3    = dff9(STORE9_CLKa, !STORE9_CLKa, DOSY_STORE9_RSTn, YVOK_OAM_DA3);
    /*p31.YPOD*/ STORE9_X4    = dff9(STORE9_CLKa, !STORE9_CLKa, DOSY_STORE9_RSTn, COSE_OAM_DA4);
    /*p31.YROP*/ STORE9_X5    = dff9(STORE9_CLKa, !STORE9_CLKa, DOSY_STORE9_RSTn, AROP_OAM_DA5);
    /*p31.YNEP*/ STORE9_X6    = dff9(STORE9_CLKa, !STORE9_CLKa, DOSY_STORE9_RSTn, XATU_OAM_DA6);
    /*p31.YZOF*/ STORE9_X7    = dff9(STORE9_CLKa, !STORE9_CLKa, DOSY_STORE9_RSTn, BADY_OAM_DA7);
  }
}

//------------------------------------------------------------------------------

#pragma once
#include "Cells.h"

namespace Schematics {

struct SchematicTop;

//-----------------------------------------------------------------------------

struct SpriteStore {
  void tick(const SchematicTop& top);
  void tock(const SchematicTop& top);

  /*p29.FEPO*/ Sig2 FEPO_STORE_MATCHp;
  /*p21.XENA*/ wire XENA_STORE_MATCHn() const { return not(FEPO_STORE_MATCHp); }

  Pin2 SPR_TRI_INDX_0 = Pin2::HIZ_PU;
  Pin2 SPR_TRI_INDX_1 = Pin2::HIZ_PU;
  Pin2 SPR_TRI_INDX_2 = Pin2::HIZ_PU;
  Pin2 SPR_TRI_INDX_3 = Pin2::HIZ_PU;
  Pin2 SPR_TRI_INDX_4 = Pin2::HIZ_PU;
  Pin2 SPR_TRI_INDX_5 = Pin2::HIZ_PU;

  Pin2 SPR_TRI_LINE_0 = Pin2::HIZ_PU;
  Pin2 SPR_TRI_LINE_1 = Pin2::HIZ_PU;
  Pin2 SPR_TRI_LINE_2 = Pin2::HIZ_PU;
  Pin2 SPR_TRI_LINE_3 = Pin2::HIZ_PU;

  void dump(Dumper& d) {
    d("----------SpriteStore---------\n");
    d("DEZY_STORE_ENn %c\n", DEZY_STORE_ENn.c());

    d("SPRITE INDEX %02d\n", 
      pack(0, 0, XECU_SPRITE_INDX5.q(), YDUF_SPRITE_INDX4.q(),
           XOBE_SPRITE_INDX3.q(), ZUZE_SPRITE_INDX2.q(), XEDY_SPRITE_INDX1.q(), XADU_SPRITE_INDX0.q()));

    d("STORE MATCH %c%c%c%c%c%c%c%c%c%c\n",
      STORE0_MATCHn.c(), STORE1_MATCHn.c(), STORE2_MATCHn.c(), STORE3_MATCHn.c(), STORE4_MATCHn.c(),
      STORE5_MATCHn.c(), STORE6_MATCHn.c(), STORE7_MATCHn.c(), STORE8_MATCHn.c(), STORE9_MATCHn.c());

    d("SPRITE COUNT %02d\n", pack(SPRITE_COUNT3.q(), SPRITE_COUNT2.q(), SPRITE_COUNT1.q(), SPRITE_COUNT0.q()));
    d("\n");
  }

private:
  friend struct SchematicTop;

  /*p30.XADU*/ Reg2 XADU_SPRITE_INDX0 = Reg2::D0C0;
  /*p30.XEDY*/ Reg2 XEDY_SPRITE_INDX1 = Reg2::D0C0;
  /*p30.ZUZE*/ Reg2 ZUZE_SPRITE_INDX2 = Reg2::D0C0;
  /*p30.XOBE*/ Reg2 XOBE_SPRITE_INDX3 = Reg2::D0C0;
  /*p30.YDUF*/ Reg2 YDUF_SPRITE_INDX4 = Reg2::D0C0;
  /*p30.XECU*/ Reg2 XECU_SPRITE_INDX5 = Reg2::D0C0;

  /*p29.YDUG*/ Sig2 STORE0_MATCHn;
  /*p29.DYDU*/ Sig2 STORE1_MATCHn;
  /*p29.DEGO*/ Sig2 STORE2_MATCHn;
  /*p29.YLOZ*/ Sig2 STORE3_MATCHn;
  /*p29.XAGE*/ Sig2 STORE4_MATCHn;
  /*p29.EGOM*/ Sig2 STORE5_MATCHn;
  /*p29.YBEZ*/ Sig2 STORE6_MATCHn;
  /*p29.DYKA*/ Sig2 STORE7_MATCHn;
  /*p29.EFYL*/ Sig2 STORE8_MATCHn;
  /*p29.YGEM*/ Sig2 STORE9_MATCHn;

  /*p29.DEZY*/ Reg2 DEZY_STORE_ENn = Reg2::D0C0;
  /*p29.BESE*/ Reg2 SPRITE_COUNT0 = Reg2::D0C0;
  /*p29.CUXY*/ Reg2 SPRITE_COUNT1 = Reg2::D0C0;
  /*p29.BEGO*/ Reg2 SPRITE_COUNT2 = Reg2::D0C0;
  /*p29.DYBE*/ Reg2 SPRITE_COUNT3 = Reg2::D0C0;

  //----------

  /*p29.EBOJ*/ Reg2 EBOJ_STORE0_RSTp = Reg2::D0C0;
  /*p30.YGUS*/ Reg2 YGUS_STORE0_INDX5 = Reg2::D0C0;
  /*p30.YSOK*/ Reg2 YSOK_STORE0_INDX4 = Reg2::D0C0;
  /*p30.YZEP*/ Reg2 YZEP_STORE0_INDX3 = Reg2::D0C0;
  /*p30.WYTE*/ Reg2 WYTE_STORE0_INDX2 = Reg2::D0C0;
  /*p30.ZONY*/ Reg2 ZONY_STORE0_INDX1 = Reg2::D0C0;
  /*p30.YWAK*/ Reg2 YWAK_STORE0_INDX0 = Reg2::D0C0;
  /*p30.FYHY*/ Reg2 FYHY_STORE0_LINE0 = Reg2::D0C0;
  /*p30.GYHO*/ Reg2 GYHO_STORE0_LINE1 = Reg2::D0C0;
  /*p30.BOZU*/ Reg2 BOZU_STORE0_LINE2 = Reg2::D0C0;
  /*p30.CUFO*/ Reg2 CUFO_STORE0_LINE3 = Reg2::D0C0;
  /*p31.XEPE*/ Reg2 XEPE_STORE0_X0 = Reg2::D0C0;
  /*p31.YLAH*/ Reg2 YLAH_STORE0_X1 = Reg2::D0C0;
  /*p31.ZOLA*/ Reg2 ZOLA_STORE0_X2 = Reg2::D0C0;
  /*p31.ZULU*/ Reg2 ZULU_STORE0_X3 = Reg2::D0C0;
  /*p31.WELO*/ Reg2 WELO_STORE0_X4 = Reg2::D0C0;
  /*p31.XUNY*/ Reg2 XUNY_STORE0_X5 = Reg2::D0C0;
  /*p31.WOTE*/ Reg2 WOTE_STORE0_X6 = Reg2::D0C0;
  /*p31.XAKO*/ Reg2 XAKO_STORE0_X7 = Reg2::D0C0;

  //----------

  /*p29.CEDY*/ Reg2 CEDY_STORE1_RSTp = Reg2::D0C0;
  /*p30.CAJY*/ Reg2 STORE1_INDX0 = Reg2::D0C0;
  /*p30.CUZA*/ Reg2 STORE1_INDX1 = Reg2::D0C0;
  /*p30.COMA*/ Reg2 STORE1_INDX2 = Reg2::D0C0;
  /*p30.CUFA*/ Reg2 STORE1_INDX3 = Reg2::D0C0;
  /*p30.CEBO*/ Reg2 STORE1_INDX4 = Reg2::D0C0;
  /*p30.CADU*/ Reg2 STORE1_INDX5 = Reg2::D0C0;
  /*p30.ABUG*/ Reg2 STORE1_LINE0 = Reg2::D0C0;
  /*p30.AMES*/ Reg2 STORE1_LINE1 = Reg2::D0C0;
  /*p30.ABOP*/ Reg2 STORE1_LINE2 = Reg2::D0C0;
  /*p30.AROF*/ Reg2 STORE1_LINE3 = Reg2::D0C0;
  /*p31.DANY*/ Reg2 STORE1_X0 = Reg2::D0C0;
  /*p31.DUKO*/ Reg2 STORE1_X1 = Reg2::D0C0;
  /*p31.DESU*/ Reg2 STORE1_X2 = Reg2::D0C0;
  /*p31.DAZO*/ Reg2 STORE1_X3 = Reg2::D0C0;
  /*p31.DAKE*/ Reg2 STORE1_X4 = Reg2::D0C0;
  /*p31.CESO*/ Reg2 STORE1_X5 = Reg2::D0C0;
  /*p31.DYFU*/ Reg2 STORE1_X6 = Reg2::D0C0;
  /*p31.CUSY*/ Reg2 STORE1_X7 = Reg2::D0C0;

  /*p29.EGAV*/ Reg2 EGAV_STORE2_RSTp = Reg2::D0C0;
  /*p30.BOXA*/ Reg2 STORE2_INDX0 = Reg2::D0C0;
  /*p30.BUNA*/ Reg2 STORE2_INDX1 = Reg2::D0C0;
  /*p30.BULU*/ Reg2 STORE2_INDX2 = Reg2::D0C0;
  /*p30.BECA*/ Reg2 STORE2_INDX3 = Reg2::D0C0;
  /*p30.BYHU*/ Reg2 STORE2_INDX4 = Reg2::D0C0;
  /*p30.BUHE*/ Reg2 STORE2_INDX5 = Reg2::D0C0;
  /*p30.YKUK*/ Reg2 STORE2_LINE0 = Reg2::D0C0;
  /*p30.YLOV*/ Reg2 STORE2_LINE1 = Reg2::D0C0;
  /*p30.XAZY*/ Reg2 STORE2_LINE2 = Reg2::D0C0;
  /*p30.XOSY*/ Reg2 STORE2_LINE3 = Reg2::D0C0;
  /*p31.FOKA*/ Reg2 STORE2_X0 = Reg2::D0C0;
  /*p31.FYTY*/ Reg2 STORE2_X1 = Reg2::D0C0;
  /*p31.FUBY*/ Reg2 STORE2_X2 = Reg2::D0C0;
  /*p31.GOXU*/ Reg2 STORE2_X3 = Reg2::D0C0;
  /*p31.DUHY*/ Reg2 STORE2_X4 = Reg2::D0C0;
  /*p31.EJUF*/ Reg2 STORE2_X5 = Reg2::D0C0;
  /*p31.ENOR*/ Reg2 STORE2_X6 = Reg2::D0C0;
  /*p31.DEPY*/ Reg2 STORE2_X7 = Reg2::D0C0;

  /*p29.GOTA*/ Reg2 GOTA_STORE3_RSTp = Reg2::D0C0;
  /*p30.DAFU*/ Reg2 STORE3_INDX0 = Reg2::D0C0;
  /*p30.DEBA*/ Reg2 STORE3_INDX1 = Reg2::D0C0;
  /*p30.DUHA*/ Reg2 STORE3_INDX2 = Reg2::D0C0;
  /*p30.DUNY*/ Reg2 STORE3_INDX3 = Reg2::D0C0;
  /*p30.DESE*/ Reg2 STORE3_INDX4 = Reg2::D0C0;
  /*p30.DEVY*/ Reg2 STORE3_INDX5 = Reg2::D0C0;
  /*p30.ZURY*/ Reg2 STORE3_LINE0 = Reg2::D0C0;
  /*p30.ZURO*/ Reg2 STORE3_LINE1 = Reg2::D0C0;
  /*p30.ZENE*/ Reg2 STORE3_LINE2 = Reg2::D0C0;
  /*p30.ZYLU*/ Reg2 STORE3_LINE4 = Reg2::D0C0;
  /*p31.XOLY*/ Reg2 STORE3_X0 = Reg2::D0C0;
  /*p31.XYBA*/ Reg2 STORE3_X1 = Reg2::D0C0;
  /*p31.XABE*/ Reg2 STORE3_X2 = Reg2::D0C0;
  /*p31.XEKA*/ Reg2 STORE3_X3 = Reg2::D0C0;
  /*p31.XOMY*/ Reg2 STORE3_X4 = Reg2::D0C0;
  /*p31.WUHA*/ Reg2 STORE3_X5 = Reg2::D0C0;
  /*p31.WYNA*/ Reg2 STORE3_X6 = Reg2::D0C0;
  /*p31.WECO*/ Reg2 STORE3_X7 = Reg2::D0C0;

  /*p29.XUDY*/ Reg2 XUDY_STORE4_RSTp = Reg2::D0C0;
  /*p30.XYNU*/ Reg2 XYNU = Reg2::D0C0;
  /*p30.XEGE*/ Reg2 XEGE = Reg2::D0C0;
  /*p30.XABO*/ Reg2 XABO = Reg2::D0C0;
  /*p30.WANU*/ Reg2 WANU = Reg2::D0C0;
  /*p30.XEFE*/ Reg2 XEFE = Reg2::D0C0;
  /*p30.XAVE*/ Reg2 XAVE = Reg2::D0C0;
  /*p30.CUMU*/ Reg2 CUMU = Reg2::D0C0;
  /*p30.CAPO*/ Reg2 CAPO = Reg2::D0C0;
  /*p30.CONO*/ Reg2 CONO = Reg2::D0C0;
  /*p30.CAJU*/ Reg2 CAJU = Reg2::D0C0;
  /*p31.WEDU*/ Reg2 STORE4_X0 = Reg2::D0C0;
  /*p31.YGAJ*/ Reg2 STORE4_X1 = Reg2::D0C0;
  /*p31.ZYJO*/ Reg2 STORE4_X2 = Reg2::D0C0;
  /*p31.XURY*/ Reg2 STORE4_X3 = Reg2::D0C0;
  /*p31.YBED*/ Reg2 STORE4_X4 = Reg2::D0C0;
  /*p31.ZALA*/ Reg2 STORE4_X5 = Reg2::D0C0;
  /*p31.WYDE*/ Reg2 STORE4_X6 = Reg2::D0C0;
  /*p31.XEPA*/ Reg2 STORE4_X7 = Reg2::D0C0;

  // store 5
  /*p29.WAFY*/ Reg2 WAFY_STORE5_RSTp = Reg2::D0C0;
  /*p30.EKAP*/ Reg2 EKAP = Reg2::D0C0;
  /*p30.ETAV*/ Reg2 ETAV = Reg2::D0C0;
  /*p30.EBEX*/ Reg2 EBEX = Reg2::D0C0;
  /*p30.GORU*/ Reg2 GORU = Reg2::D0C0;
  /*p30.ETYM*/ Reg2 ETYM = Reg2::D0C0;
  /*p30.EKOP*/ Reg2 EKOP = Reg2::D0C0;
  /*p30.ANED*/ Reg2 ANED = Reg2::D0C0;
  /*p30.ACEP*/ Reg2 ACEP = Reg2::D0C0;
  /*p30.ABUX*/ Reg2 ABUX = Reg2::D0C0;
  /*p30.ABEG*/ Reg2 ABEG = Reg2::D0C0;
  /*p31.FUSA*/ Reg2 STORE5_X0 = Reg2::D0C0;
  /*p31.FAXA*/ Reg2 STORE5_X1 = Reg2::D0C0;
  /*p31.FOZY*/ Reg2 STORE5_X2 = Reg2::D0C0;
  /*p31.FESY*/ Reg2 STORE5_X3 = Reg2::D0C0;
  /*p31.CYWE*/ Reg2 STORE5_X4 = Reg2::D0C0;
  /*p31.DYBY*/ Reg2 STORE5_X5 = Reg2::D0C0;
  /*p31.DURY*/ Reg2 STORE5_X6 = Reg2::D0C0;
  /*p31.CUVY*/ Reg2 STORE5_X7 = Reg2::D0C0;

  // sprite store 6
  /*p29.WOMY*/ Reg2 WOMY_STORE6_RSTp = Reg2::D0C0;
  /*p30.GECU*/ Reg2 GECU = Reg2::D0C0;
  /*p30.FOXY*/ Reg2 FOXY = Reg2::D0C0;
  /*p30.GOHU*/ Reg2 GOHU = Reg2::D0C0;
  /*p30.FOGO*/ Reg2 FOGO = Reg2::D0C0;
  /*p30.GACY*/ Reg2 GACY = Reg2::D0C0;
  /*p30.GABO*/ Reg2 GABO = Reg2::D0C0;
  /*p30.ZUBE*/ Reg2 ZUBE = Reg2::D0C0;
  /*p30.ZUMY*/ Reg2 ZUMY = Reg2::D0C0;
  /*p30.ZEXO*/ Reg2 ZEXO = Reg2::D0C0;
  /*p30.ZAFU*/ Reg2 ZAFU = Reg2::D0C0;
  /*p31.YCOL*/ Reg2 STORE6_X0 = Reg2::D0C0;
  /*p31.YRAC*/ Reg2 STORE6_X1 = Reg2::D0C0;
  /*p31.YMEM*/ Reg2 STORE6_X2 = Reg2::D0C0;
  /*p31.YVAG*/ Reg2 STORE6_X3 = Reg2::D0C0;
  /*p31.ZOLY*/ Reg2 STORE6_X4 = Reg2::D0C0;
  /*p31.ZOGO*/ Reg2 STORE6_X5 = Reg2::D0C0;
  /*p31.ZECU*/ Reg2 STORE6_X6 = Reg2::D0C0;
  /*p31.ZESA*/ Reg2 STORE6_X7 = Reg2::D0C0;

  // sprite store 7
  /*p29.WAPO*/ Reg2 WAPO_STORE7_RSTp = Reg2::D0C0;
  /*p30.FUZO*/ Reg2 FUZO = Reg2::D0C0;
  /*p30.GESY*/ Reg2 GESY = Reg2::D0C0;
  /*p30.FYSU*/ Reg2 FYSU = Reg2::D0C0;
  /*p30.FEFA*/ Reg2 FEFA = Reg2::D0C0;
  /*p30.GYNO*/ Reg2 GYNO = Reg2::D0C0;
  /*p30.GULE*/ Reg2 GULE = Reg2::D0C0;
  /*p30.XYGO*/ Reg2 XYGO = Reg2::D0C0;
  /*p30.XYNA*/ Reg2 XYNA = Reg2::D0C0;
  /*p30.XAKU*/ Reg2 XAKU = Reg2::D0C0;
  /*p30.YGUM*/ Reg2 YGUM = Reg2::D0C0;
  /*p31.ERAZ*/ Reg2 STORE7_X0 = Reg2::D0C0;
  /*p31.EPUM*/ Reg2 STORE7_X1 = Reg2::D0C0;
  /*p31.EROL*/ Reg2 STORE7_X2 = Reg2::D0C0;
  /*p31.EHYN*/ Reg2 STORE7_X3 = Reg2::D0C0;
  /*p31.FAZU*/ Reg2 STORE7_X4 = Reg2::D0C0;
  /*p31.FAXE*/ Reg2 STORE7_X5 = Reg2::D0C0;
  /*p31.EXUK*/ Reg2 STORE7_X6 = Reg2::D0C0;
  /*p31.FEDE*/ Reg2 STORE7_X7 = Reg2::D0C0;

  // sprite store 8
  /*p29.EXUQ*/ Reg2 EXUQ_STORE8_RSTp = Reg2::D0C0;
  /*p30.AXUV*/ Reg2 STORE8_INDX5 = Reg2::D0C0;
  /*p30.BADA*/ Reg2 STORE8_INDX4 = Reg2::D0C0;
  /*p30.APEV*/ Reg2 STORE8_INDX3 = Reg2::D0C0;
  /*p30.BADO*/ Reg2 STORE8_INDX2 = Reg2::D0C0;
  /*p30.BEXY*/ Reg2 STORE8_INDX1 = Reg2::D0C0;
  /*p30.BYHE*/ Reg2 STORE8_INDX0 = Reg2::D0C0;
  /*p30.AFYM*/ Reg2 STORE8_LINE0 = Reg2::D0C0;
  /*p30.AZAP*/ Reg2 STORE8_LINE1 = Reg2::D0C0;
  /*p30.AFUT*/ Reg2 STORE8_LINE2 = Reg2::D0C0;
  /*p30.AFYX*/ Reg2 STORE8_LINE3 = Reg2::D0C0;
  /*p31.GAVY*/ Reg2 STORE8_X4 = Reg2::D0C0;
  /*p31.GYPU*/ Reg2 STORE8_X5 = Reg2::D0C0;
  /*p31.GADY*/ Reg2 STORE8_X6 = Reg2::D0C0;
  /*p31.GAZA*/ Reg2 STORE8_X7 = Reg2::D0C0;
  /*p31.EZUF*/ Reg2 STORE8_X0 = Reg2::D0C0;
  /*p31.ENAD*/ Reg2 STORE8_X1 = Reg2::D0C0;
  /*p31.EBOW*/ Reg2 STORE8_X2 = Reg2::D0C0;
  /*p31.FYCA*/ Reg2 STORE8_X3 = Reg2::D0C0;

  // sprite store 9
  /*p29.FONO*/ Reg2 FONO_STORE9_RSTp = Reg2::D0C0;
  /*p30.XUFO*/ Reg2 STORE9_INDX0 = Reg2::D0C0;
  /*p30.XUTE*/ Reg2 STORE9_INDX1 = Reg2::D0C0;
  /*p30.XOTU*/ Reg2 STORE9_INDX2 = Reg2::D0C0;
  /*p30.XYFE*/ Reg2 STORE9_INDX3 = Reg2::D0C0;
  /*p30.YZOR*/ Reg2 STORE9_INDX4 = Reg2::D0C0;
  /*p30.YBER*/ Reg2 STORE9_INDX5 = Reg2::D0C0;
  /*p30.DEWU*/ Reg2 STORE9_LINE0 = Reg2::D0C0;
  /*p30.CANA*/ Reg2 STORE9_LINE1 = Reg2::D0C0;
  /*p30.DYSY*/ Reg2 STORE9_LINE2 = Reg2::D0C0;
  /*p30.FOFO*/ Reg2 STORE9_LINE3 = Reg2::D0C0;
  /*p31.XUVY*/ Reg2 STORE9_X0 = Reg2::D0C0;
  /*p31.XERE*/ Reg2 STORE9_X1 = Reg2::D0C0;
  /*p31.XUZO*/ Reg2 STORE9_X2 = Reg2::D0C0;
  /*p31.XEXA*/ Reg2 STORE9_X3 = Reg2::D0C0;
  /*p31.YPOD*/ Reg2 STORE9_X4 = Reg2::D0C0;
  /*p31.YROP*/ Reg2 STORE9_X5 = Reg2::D0C0;
  /*p31.YNEP*/ Reg2 STORE9_X6 = Reg2::D0C0;
  /*p31.YZOF*/ Reg2 STORE9_X7 = Reg2::D0C0;
};

//-----------------------------------------------------------------------------

}; // namespace Schematics
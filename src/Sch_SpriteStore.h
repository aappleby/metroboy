#pragma once
#include "Cells.h"

namespace Schematics {

struct SchematicTop;

//-----------------------------------------------------------------------------

struct SpriteStoreRegisters {
  void tick_match(SchematicTop& gb);
  void tock(SchematicTop& gb);
  SignalHash commit(SchematicTop& top);

  /*p29.FEPO*/ Signal FEPO_STORE_MATCHp;

private:
  friend struct SchematicTop;

  /*p29.YDUG*/ Signal STORE0_MATCHn;
  /*p29.DYDU*/ Signal STORE1_MATCHn;
  /*p29.DEGO*/ Signal STORE2_MATCHn;
  /*p29.YLOZ*/ Signal STORE3_MATCHn;
  /*p29.XAGE*/ Signal STORE4_MATCHn;
  /*p29.EGOM*/ Signal STORE5_MATCHn;
  /*p29.YBEZ*/ Signal STORE6_MATCHn;
  /*p29.DYKA*/ Signal STORE7_MATCHn;
  /*p29.EFYL*/ Signal STORE8_MATCHn;
  /*p29.YGEM*/ Signal STORE9_MATCHn;

  /*p29.DEZY*/ Reg17 DEZY_STORE_ENn_SYNC;
  /*p29.BESE*/ Reg17 SPRITE_COUNT0;
  /*p29.CUXY*/ Reg17 SPRITE_COUNT1;
  /*p29.BEGO*/ Reg17 SPRITE_COUNT2;
  /*p29.DYBE*/ Reg17 SPRITE_COUNT3;

  //----------

  /*p29.EBOJ*/ Reg17 EBOJ_STORE0_RSTp;
  /*p30.YGUS*/ Reg8 YGUS_STORE0_INDX5;
  /*p30.YSOK*/ Reg8 YSOK_STORE0_INDX4;
  /*p30.YZEP*/ Reg8 YZEP_STORE0_INDX3;
  /*p30.WYTE*/ Reg8 WYTE_STORE0_INDX2;
  /*p30.ZONY*/ Reg8 ZONY_STORE0_INDX1;
  /*p30.YWAK*/ Reg8 YWAK_STORE0_INDX0;
  /*p30.FYHY*/ Reg8 FYHY_STORE0_LINE0;
  /*p30.GYHO*/ Reg8 GYHO_STORE0_LINE1;
  /*p30.BOZU*/ Reg8 BOZU_STORE0_LINE2;
  /*p30.CUFO*/ Reg8 CUFO_STORE0_LINE3;
  /*p31.XEPE*/ Reg9 XEPE_STORE0_X0;
  /*p31.YLAH*/ Reg9 YLAH_STORE0_X1;
  /*p31.ZOLA*/ Reg9 ZOLA_STORE0_X2;
  /*p31.ZULU*/ Reg9 ZULU_STORE0_X3;
  /*p31.WELO*/ Reg9 WELO_STORE0_X4;
  /*p31.XUNY*/ Reg9 XUNY_STORE0_X5;
  /*p31.WOTE*/ Reg9 WOTE_STORE0_X6;
  /*p31.XAKO*/ Reg9 XAKO_STORE0_X7;

  //----------

  /*p29.CEDY*/ Reg17 CEDY_STORE1_RSTp;
  /*p30.CAJY*/ Reg8 STORE1_INDX0;
  /*p30.CUZA*/ Reg8 STORE1_INDX1;
  /*p30.COMA*/ Reg8 STORE1_INDX2;
  /*p30.CUFA*/ Reg8 STORE1_INDX3;
  /*p30.CEBO*/ Reg8 STORE1_INDX4;
  /*p30.CADU*/ Reg8 STORE1_INDX5;
  /*p30.ABUG*/ Reg8 STORE1_LINE0;
  /*p30.AMES*/ Reg8 STORE1_LINE1;
  /*p30.ABOP*/ Reg8 STORE1_LINE2;
  /*p30.AROF*/ Reg8 STORE1_LINE3;
  /*p31.DANY*/ Reg9 STORE1_X0;
  /*p31.DUKO*/ Reg9 STORE1_X1;
  /*p31.DESU*/ Reg9 STORE1_X2;
  /*p31.DAZO*/ Reg9 STORE1_X3;
  /*p31.DAKE*/ Reg9 STORE1_X4;
  /*p31.CESO*/ Reg9 STORE1_X5;
  /*p31.DYFU*/ Reg9 STORE1_X6;
  /*p31.CUSY*/ Reg9 STORE1_X7;

  /*p29.EGAV*/ Reg17 EGAV_STORE2_RSTp;
  /*p30.BOXA*/ Reg8 STORE2_INDX0;
  /*p30.BUNA*/ Reg8 STORE2_INDX1;
  /*p30.BULU*/ Reg8 STORE2_INDX2;
  /*p30.BECA*/ Reg8 STORE2_INDX3;
  /*p30.BYHU*/ Reg8 STORE2_INDX4;
  /*p30.BUHE*/ Reg8 STORE2_INDX5;
  /*p30.YKUK*/ Reg8 STORE2_LINE0;
  /*p30.YLOV*/ Reg8 STORE2_LINE1;
  /*p30.XAZY*/ Reg8 STORE2_LINE2;
  /*p30.XOSY*/ Reg8 STORE2_LINE3;
  /*p31.FOKA*/ Reg9 STORE2_X0;
  /*p31.FYTY*/ Reg9 STORE2_X1;
  /*p31.FUBY*/ Reg9 STORE2_X2;
  /*p31.GOXU*/ Reg9 STORE2_X3;
  /*p31.DUHY*/ Reg9 STORE2_X4;
  /*p31.EJUF*/ Reg9 STORE2_X5;
  /*p31.ENOR*/ Reg9 STORE2_X6;
  /*p31.DEPY*/ Reg9 STORE2_X7;

  /*p29.GOTA*/ Reg17 GOTA_STORE3_RSTp;
  /*p30.DAFU*/ Reg8 STORE3_INDX0;
  /*p30.DEBA*/ Reg8 STORE3_INDX1;
  /*p30.DUHA*/ Reg8 STORE3_INDX2;
  /*p30.DUNY*/ Reg8 STORE3_INDX3;
  /*p30.DESE*/ Reg8 STORE3_INDX4;
  /*p30.DEVY*/ Reg8 STORE3_INDX5;
  /*p30.ZURY*/ Reg8 STORE3_LINE0;
  /*p30.ZURO*/ Reg8 STORE3_LINE1;
  /*p30.ZENE*/ Reg8 STORE3_LINE2;
  /*p30.ZYLU*/ Reg8 STORE3_LINE4;
  /*p31.XOLY*/ Reg9 STORE3_X0;
  /*p31.XYBA*/ Reg9 STORE3_X1;
  /*p31.XABE*/ Reg9 STORE3_X2;
  /*p31.XEKA*/ Reg9 STORE3_X3;
  /*p31.XOMY*/ Reg9 STORE3_X4;
  /*p31.WUHA*/ Reg9 STORE3_X5;
  /*p31.WYNA*/ Reg9 STORE3_X6;
  /*p31.WECO*/ Reg9 STORE3_X7;

  /*p29.XUDY*/ Reg17 XUDY_STORE4_RSTp;
  /*p30.XYNU*/ Reg8 XYNU;
  /*p30.XEGE*/ Reg8 XEGE;
  /*p30.XABO*/ Reg8 XABO;
  /*p30.WANU*/ Reg8 WANU;
  /*p30.XEFE*/ Reg8 XEFE;
  /*p30.XAVE*/ Reg8 XAVE;
  /*p30.CUMU*/ Reg8 CUMU;
  /*p30.CAPO*/ Reg8 CAPO;
  /*p30.CONO*/ Reg8 CONO;
  /*p30.CAJU*/ Reg8 CAJU;
  /*p31.WEDU*/ Reg9 STORE4_X0;
  /*p31.YGAJ*/ Reg9 STORE4_X1;
  /*p31.ZYJO*/ Reg9 STORE4_X2;
  /*p31.XURY*/ Reg9 STORE4_X3;
  /*p31.YBED*/ Reg9 STORE4_X4;
  /*p31.ZALA*/ Reg9 STORE4_X5;
  /*p31.WYDE*/ Reg9 STORE4_X6;
  /*p31.XEPA*/ Reg9 STORE4_X7;

  // store 5
  /*p29.WAFY*/ Reg17 WAFY_STORE5_RSTp;
  /*p30.EKAP*/ Reg8 EKAP;
  /*p30.ETAV*/ Reg8 ETAV;
  /*p30.EBEX*/ Reg8 EBEX;
  /*p30.GORU*/ Reg8 GORU;
  /*p30.ETYM*/ Reg8 ETYM;
  /*p30.EKOP*/ Reg8 EKOP;
  /*p30.ANED*/ Reg8 ANED;
  /*p30.ACEP*/ Reg8 ACEP;
  /*p30.ABUX*/ Reg8 ABUX;
  /*p30.ABEG*/ Reg8 ABEG;
  /*p31.FUSA*/ Reg9 STORE5_X0;
  /*p31.FAXA*/ Reg9 STORE5_X1;
  /*p31.FOZY*/ Reg9 STORE5_X2;
  /*p31.FESY*/ Reg9 STORE5_X3;
  /*p31.CYWE*/ Reg9 STORE5_X4;
  /*p31.DYBY*/ Reg9 STORE5_X5;
  /*p31.DURY*/ Reg9 STORE5_X6;
  /*p31.CUVY*/ Reg9 STORE5_X7;

  // sprite store 6
  /*p29.WOMY*/ Reg17 WOMY_STORE6_RSTp;
  /*p30.GECU*/ Reg8 GECU;
  /*p30.FOXY*/ Reg8 FOXY;
  /*p30.GOHU*/ Reg8 GOHU;
  /*p30.FOGO*/ Reg8 FOGO;
  /*p30.GACY*/ Reg8 GACY;
  /*p30.GABO*/ Reg8 GABO;
  /*p30.ZUBE*/ Reg8 ZUBE;
  /*p30.ZUMY*/ Reg8 ZUMY;
  /*p30.ZEXO*/ Reg8 ZEXO;
  /*p30.ZAFU*/ Reg8 ZAFU;
  /*p31.YCOL*/ Reg9 STORE6_X0;
  /*p31.YRAC*/ Reg9 STORE6_X1;
  /*p31.YMEM*/ Reg9 STORE6_X2;
  /*p31.YVAG*/ Reg9 STORE6_X3;
  /*p31.ZOLY*/ Reg9 STORE6_X4;
  /*p31.ZOGO*/ Reg9 STORE6_X5;
  /*p31.ZECU*/ Reg9 STORE6_X6;
  /*p31.ZESA*/ Reg9 STORE6_X7;

  // sprite store 7
  /*p29.WAPO*/ Reg17 WAPO_STORE7_RSTp;
  /*p30.FUZO*/ Reg8 FUZO;
  /*p30.GESY*/ Reg8 GESY;
  /*p30.FYSU*/ Reg8 FYSU;
  /*p30.FEFA*/ Reg8 FEFA;
  /*p30.GYNO*/ Reg8 GYNO;
  /*p30.GULE*/ Reg8 GULE;
  /*p30.XYGO*/ Reg8 XYGO;
  /*p30.XYNA*/ Reg8 XYNA;
  /*p30.XAKU*/ Reg8 XAKU;
  /*p30.YGUM*/ Reg8 YGUM;
  /*p31.ERAZ*/ Reg9 STORE7_X0;
  /*p31.EPUM*/ Reg9 STORE7_X1;
  /*p31.EROL*/ Reg9 STORE7_X2;
  /*p31.EHYN*/ Reg9 STORE7_X3;
  /*p31.FAZU*/ Reg9 STORE7_X4;
  /*p31.FAXE*/ Reg9 STORE7_X5;
  /*p31.EXUK*/ Reg9 STORE7_X6;
  /*p31.FEDE*/ Reg9 STORE7_X7;

  // sprite store 8
  /*p29.EXUQ*/ Reg17 EXUQ_STORE8_RSTp;
  /*p30.AXUV*/ Reg8 STORE8_INDX5;
  /*p30.BADA*/ Reg8 STORE8_INDX4;
  /*p30.APEV*/ Reg8 STORE8_INDX3;
  /*p30.BADO*/ Reg8 STORE8_INDX2;
  /*p30.BEXY*/ Reg8 STORE8_INDX1;
  /*p30.BYHE*/ Reg8 STORE8_INDX0;
  /*p30.AFYM*/ Reg8 STORE8_LINE0;
  /*p30.AZAP*/ Reg8 STORE8_LINE1;
  /*p30.AFUT*/ Reg8 STORE8_LINE2;
  /*p30.AFYX*/ Reg8 STORE8_LINE3;
  /*p31.GAVY*/ Reg9 STORE8_X4;
  /*p31.GYPU*/ Reg9 STORE8_X5;
  /*p31.GADY*/ Reg9 STORE8_X6;
  /*p31.GAZA*/ Reg9 STORE8_X7;
  /*p31.EZUF*/ Reg9 STORE8_X0;
  /*p31.ENAD*/ Reg9 STORE8_X1;
  /*p31.EBOW*/ Reg9 STORE8_X2;
  /*p31.FYCA*/ Reg9 STORE8_X3;

  // sprite store 9
  /*p29.FONO*/ Reg17 FONO_STORE9_RSTp;
  /*p30.XUFO*/ Reg8 STORE9_INDX0;
  /*p30.XUTE*/ Reg8 STORE9_INDX1;
  /*p30.XOTU*/ Reg8 STORE9_INDX2;
  /*p30.XYFE*/ Reg8 STORE9_INDX3;
  /*p30.YZOR*/ Reg8 STORE9_INDX4;
  /*p30.YBER*/ Reg8 STORE9_INDX5;
  /*p30.DEWU*/ Reg8 STORE9_LINE0;
  /*p30.CANA*/ Reg8 STORE9_LINE1;
  /*p30.DYSY*/ Reg8 STORE9_LINE2;
  /*p30.FOFO*/ Reg8 STORE9_LINE3;
  /*p31.XUVY*/ Reg9 STORE9_X0;
  /*p31.XERE*/ Reg9 STORE9_X1;
  /*p31.XUZO*/ Reg9 STORE9_X2;
  /*p31.XEXA*/ Reg9 STORE9_X3;
  /*p31.YPOD*/ Reg9 STORE9_X4;
  /*p31.YROP*/ Reg9 STORE9_X5;
  /*p31.YNEP*/ Reg9 STORE9_X6;
  /*p31.YZOF*/ Reg9 STORE9_X7;
};

//-----------------------------------------------------------------------------

}; // namespace Schematics
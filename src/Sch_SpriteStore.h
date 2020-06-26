#pragma once
#include "Sch_Common.h"

namespace Schematics {

struct TestGB;

//-----------------------------------------------------------------------------

struct SpriteStoreSignals {
  /*p29.FEPO*/ wire FEPO_STORE_MATCHp;

  /*p29.YDUG*/ wire STORE0_MATCHn;
  /*p29.DYDU*/ wire STORE1_MATCHn;
  /*p29.DEGO*/ wire STORE2_MATCHn;
  /*p29.YLOZ*/ wire STORE3_MATCHn;
  /*p29.XAGE*/ wire STORE4_MATCHn;
  /*p29.EGOM*/ wire STORE5_MATCHn;
  /*p29.YBEZ*/ wire STORE6_MATCHn;
  /*p29.DYKA*/ wire STORE7_MATCHn;
  /*p29.EFYL*/ wire STORE8_MATCHn;
  /*p29.YGEM*/ wire STORE9_MATCHn;

  /*p29.AVAP*/ wire AVAP_SCAN_DONE_d0_TRIGp;
};

//-----------------------------------------------------------------------------

struct SpriteStoreRegisters {

  SpriteStoreSignals sig(const TestGB& gb) const;
  void tick(const TestGB& gb);
  bool commit();

  void dump_regs(TextPainter& text_painter) {
    text_painter.dprintf("----- SPR_STORE -----\n");
    text_painter.dprintf("SPRITE_COUNT %d\n", pack(SPRITE_COUNT0.q(), SPRITE_COUNT1.q(), SPRITE_COUNT2.q(), SPRITE_COUNT3.q()));

    /*
    text_painter.dprintf("STORE0 %d %d %d %d\n",
                 SPRITE0_GET_SYNC.a.val,
                 pack(STORE0_X0,    STORE0_X1,    STORE0_X2,    STORE0_X3,   STORE0_X4,   STORE0_X5, STORE0_X6, STORE0_X7),
                 pack(STORE0_IDX0,  STORE0_IDX1,  STORE0_IDX2,  STORE0_IDX3, STORE0_IDX4, STORE0_IDX5),
                 pack(STORE0_LINE0, STORE0_LINE1, STORE0_LINE2, STORE0_LINE3));

    text_painter.dprintf("STORE1 %d %d %d %d\n",
                 SPRITE1_GET_SYNCn.a.val,
                 pack(STORE1_X0,    STORE1_X1,    STORE1_X2,    STORE1_X3,   STORE1_X4,   STORE1_X5, STORE1_X6, STORE1_X7),
                 pack(STORE1_IDX0,  STORE1_IDX1,  STORE1_IDX2,  STORE1_IDX3, STORE1_IDX4, STORE1_IDX5),
                 pack(STORE1_LINE0, STORE1_LINE1, STORE1_LINE2, STORE1_LINE3));

    text_painter.dprintf("STORE2 %d %d %d %d\n",
                 SPRITE2_GET_SYNCn.a.val,
                 pack(STORE2_X0,    STORE2_X1,    STORE2_X2,    STORE2_X3,   STORE2_X4,   STORE2_X5, STORE2_X6, STORE2_X7),
                 pack(STORE2_IDX0,  STORE2_IDX1,  STORE2_IDX2,  STORE2_IDX3, STORE2_IDX4, STORE2_IDX5),
                 pack(STORE2_LINE0, STORE2_LINE1, STORE2_LINE2, STORE2_LINE3));
    text_painter.dprintf("...\n");
    */
    text_painter.newline();
  }

  /*p28.BESU*/ NorLatch BESU_SCANNINGp;
  /*p29.CENO*/ Reg   CENO_SCANNINGp;

  /*p28.YFEL*/ Reg SCAN0;
  /*p28.WEWY*/ Reg SCAN1;
  /*p28.GOSO*/ Reg SCAN2;
  /*p28.ELYN*/ Reg SCAN3;
  /*p28.FAHA*/ Reg SCAN4;
  /*p28.FONY*/ Reg SCAN5;
  /*p29.BYBA*/ Reg SCAN_DONE_d4;
  /*p29.DOBA*/ Reg SCAN_DONE_d5;

  /*p30.XADU*/ Reg XADU_SPRITE_IDX0;
  /*p30.XEDY*/ Reg XEDY_SPRITE_IDX1;
  /*p30.ZUZE*/ Reg ZUZE_SPRITE_IDX2;
  /*p30.XOBE*/ Reg XOBE_SPRITE_IDX3;
  /*p30.YDUF*/ Reg YDUF_SPRITE_IDX4;
  /*p30.XECU*/ Reg XECU_SPRITE_IDX5;

  /*p30.WUZY*/ Tribuf WUZY_TS_IDX_0;
  /*p30.WYSE*/ Tribuf WYSE_TS_IDX_1;
  /*p30.ZYSU*/ Tribuf ZYSU_TS_IDX_2;
  /*p30.WYDA*/ Tribuf WYDA_TS_IDX_3;
  /*p30.WUCO*/ Tribuf WUCO_TS_IDX_4;
  /*p30.WEZA*/ Tribuf WEZA_TS_IDX_5;

  /*p30.WENU*/ Tribuf WENU_TS_LINE_0;
  /*p30.CUCU*/ Tribuf CUCU_TS_LINE_1;
  /*p30.CUCA*/ Tribuf CUCA_TS_LINE_2;
  /*p30.CEGA*/ Tribuf CEGA_TS_LINE_3;

private:

  /*p29.DEZY*/ Reg DEZY_CLKp;
  /*p29.BESE*/ Reg SPRITE_COUNT0;
  /*p29.CUXY*/ Reg SPRITE_COUNT1;
  /*p29.BEGO*/ Reg SPRITE_COUNT2;
  /*p29.DYBE*/ Reg SPRITE_COUNT3;

  /*p29.EBOJ*/ Reg EBOJ_STORE0_RSTp;
  /*p30.YGUS*/ Reg YGUS_STORE0_IDX5;
  /*p30.YSOK*/ Reg YSOK_STORE0_IDX4;
  /*p30.YZEP*/ Reg YZEP_STORE0_IDX3;
  /*p30.WYTE*/ Reg WYTE_STORE0_IDX2;
  /*p30.ZONY*/ Reg ZONY_STORE0_IDX1;
  /*p30.YWAK*/ Reg YWAK_STORE0_IDX0;
  /*p30.FYHY*/ Reg STORE0_LINE0;
  /*p30.GYHO*/ Reg STORE0_LINE1;
  /*p30.BOZU*/ Reg STORE0_LINE2;
  /*p30.CUFO*/ Reg STORE0_LINE3;
  /*p31.XEPE*/ Reg STORE0_X0;
  /*p31.YLAH*/ Reg STORE0_X1;
  /*p31.ZOLA*/ Reg STORE0_X2;
  /*p31.ZULU*/ Reg STORE0_X3;
  /*p31.WELO*/ Reg STORE0_X4;
  /*p31.XUNY*/ Reg STORE0_X5;
  /*p31.WOTE*/ Reg STORE0_X6;
  /*p31.XAKO*/ Reg STORE0_X7;

  /*p29.CEDY*/ Reg CEDY_STORE1_RSTp;
  /*p30.CAJY*/ Reg STORE1_IDX0;
  /*p30.CUZA*/ Reg STORE1_IDX1;
  /*p30.COMA*/ Reg STORE1_IDX2;
  /*p30.CUFA*/ Reg STORE1_IDX3;
  /*p30.CEBO*/ Reg STORE1_IDX4;
  /*p30.CADU*/ Reg STORE1_IDX5;
  /*p30.ABUG*/ Reg STORE1_LINE0;
  /*p30.AMES*/ Reg STORE1_LINE1;
  /*p30.ABOP*/ Reg STORE1_LINE2;
  /*p30.AROF*/ Reg STORE1_LINE3;
  /*p31.DANY*/ Reg STORE1_X0;
  /*p31.DUKO*/ Reg STORE1_X1;
  /*p31.DESU*/ Reg STORE1_X2;
  /*p31.DAZO*/ Reg STORE1_X3;
  /*p31.DAKE*/ Reg STORE1_X4;
  /*p31.CESO*/ Reg STORE1_X5;
  /*p31.DYFU*/ Reg STORE1_X6;
  /*p31.CUSY*/ Reg STORE1_X7;

  /*p29.EGAV*/ Reg EGAV_STORE2_RSTp;
  /*p30.BOXA*/ Reg STORE2_IDX0;
  /*p30.BUNA*/ Reg STORE2_IDX1;
  /*p30.BULU*/ Reg STORE2_IDX2;
  /*p30.BECA*/ Reg STORE2_IDX3;
  /*p30.BYHU*/ Reg STORE2_IDX4;
  /*p30.BUHE*/ Reg STORE2_IDX5;
  /*p30.YKUK*/ Reg STORE2_LINE0;
  /*p30.YLOV*/ Reg STORE2_LINE1;
  /*p30.XAZY*/ Reg STORE2_LINE2;
  /*p30.XOSY*/ Reg STORE2_LINE3;
  /*p31.FOKA*/ Reg STORE2_X0;
  /*p31.FYTY*/ Reg STORE2_X1;
  /*p31.FUBY*/ Reg STORE2_X2;
  /*p31.GOXU*/ Reg STORE2_X3;
  /*p31.DUHY*/ Reg STORE2_X4;
  /*p31.EJUF*/ Reg STORE2_X5;
  /*p31.ENOR*/ Reg STORE2_X6;
  /*p31.DEPY*/ Reg STORE2_X7;

  /*p29.GOTA*/ Reg GOTA_STORE3_RSTp;
  /*p30.DAFU*/ Reg STORE3_IDX0;
  /*p30.DEBA*/ Reg STORE3_IDX1;
  /*p30.DUHA*/ Reg STORE3_IDX2;
  /*p30.DUNY*/ Reg STORE3_IDX3;
  /*p30.DESE*/ Reg STORE3_IDX4;
  /*p30.DEVY*/ Reg STORE3_IDX5;
  /*p30.ZURY*/ Reg STORE3_LINE0;
  /*p30.ZURO*/ Reg STORE3_LINE1;
  /*p30.ZENE*/ Reg STORE3_LINE2;
  /*p30.ZYLU*/ Reg STORE3_LINE4;
  /*p31.XOLY*/ Reg STORE3_X0;
  /*p31.XYBA*/ Reg STORE3_X1;
  /*p31.XABE*/ Reg STORE3_X2;
  /*p31.XEKA*/ Reg STORE3_X3;
  /*p31.XOMY*/ Reg STORE3_X4;
  /*p31.WUHA*/ Reg STORE3_X5;
  /*p31.WYNA*/ Reg STORE3_X6;
  /*p31.WECO*/ Reg STORE3_X7;

  /*p29.XUDY*/ Reg XUDY_STORE4_RSTp;
  /*p31.WEDU*/ Reg STORE4_X0;
  /*p31.YGAJ*/ Reg STORE4_X1;
  /*p31.ZYJO*/ Reg STORE4_X2;
  /*p31.XURY*/ Reg STORE4_X3;
  /*p31.YBED*/ Reg STORE4_X4;
  /*p31.ZALA*/ Reg STORE4_X5;
  /*p31.WYDE*/ Reg STORE4_X6;
  /*p31.XEPA*/ Reg STORE4_X7;
  /*p30.XYNU*/ Reg XYNU;
  /*p30.XEGE*/ Reg XEGE;
  /*p30.XABO*/ Reg XABO;
  /*p30.WANU*/ Reg WANU;
  /*p30.XEFE*/ Reg XEFE;
  /*p30.XAVE*/ Reg XAVE;
  /*p30.CUMU*/ Reg CUMU;
  /*p30.CAPO*/ Reg CAPO;
  /*p30.CONO*/ Reg CONO;
  /*p30.CAJU*/ Reg CAJU;

  // store 5
  /*p29.WAFY*/ Reg WAFY_STORE5_RSTp;
  /*p31.FUSA*/ Reg STORE5_X0;
  /*p31.FAXA*/ Reg STORE5_X1;
  /*p31.FOZY*/ Reg STORE5_X2;
  /*p31.FESY*/ Reg STORE5_X3;
  /*p31.CYWE*/ Reg STORE5_X4;
  /*p31.DYBY*/ Reg STORE5_X5;
  /*p31.DURY*/ Reg STORE5_X6;
  /*p31.CUVY*/ Reg STORE5_X7;
  /*p30.EKAP*/ Reg EKAP;
  /*p30.ETAV*/ Reg ETAV;
  /*p30.EBEX*/ Reg EBEX;
  /*p30.GORU*/ Reg GORU;
  /*p30.ETYM*/ Reg ETYM;
  /*p30.EKOP*/ Reg EKOP;
  /*p30.ANED*/ Reg ANED;
  /*p30.ACEP*/ Reg ACEP;
  /*p30.ABUX*/ Reg ABUX;
  /*p30.ABEG*/ Reg ABEG;

  // sprite store 6
  /*p29.WOMY*/ Reg WOMY_STORE6_RSTp;
  /*p31.YCOL*/ Reg STORE6_X0;
  /*p31.YRAC*/ Reg STORE6_X1;
  /*p31.YMEM*/ Reg STORE6_X2;
  /*p31.YVAG*/ Reg STORE6_X3;
  /*p31.ZOLY*/ Reg STORE6_X4;
  /*p31.ZOGO*/ Reg STORE6_X5;
  /*p31.ZECU*/ Reg STORE6_X6;
  /*p31.ZESA*/ Reg STORE6_X7;
  /*p30.GECU*/ Reg GECU;
  /*p30.FOXY*/ Reg FOXY;
  /*p30.GOHU*/ Reg GOHU;
  /*p30.FOGO*/ Reg FOGO;
  /*p30.GACY*/ Reg GACY;
  /*p30.GABO*/ Reg GABO;
  /*p30.ZUBE*/ Reg ZUBE;
  /*p30.ZUMY*/ Reg ZUMY;
  /*p30.ZEXO*/ Reg ZEXO;
  /*p30.ZAFU*/ Reg ZAFU;

  // sprite store 7
  /*p29.WAPO*/ Reg WAPO_STORE7_RSTp;
  /*p31.ERAZ*/ Reg STORE7_X0;
  /*p31.EPUM*/ Reg STORE7_X1;
  /*p31.EROL*/ Reg STORE7_X2;
  /*p31.EHYN*/ Reg STORE7_X3;
  /*p31.FAZU*/ Reg STORE7_X4;
  /*p31.FAXE*/ Reg STORE7_X5;
  /*p31.EXUK*/ Reg STORE7_X6;
  /*p31.FEDE*/ Reg STORE7_X7;
  /*p30.FUZO*/ Reg FUZO;
  /*p30.GESY*/ Reg GESY;
  /*p30.FYSU*/ Reg FYSU;
  /*p30.FEFA*/ Reg FEFA;
  /*p30.GYNO*/ Reg GYNO;
  /*p30.GULE*/ Reg GULE;
  /*p30.XYGO*/ Reg XYGO;
  /*p30.XYNA*/ Reg XYNA;
  /*p30.XAKU*/ Reg XAKU;
  /*p30.YGUM*/ Reg YGUM;

  // sprite store 8
  /*p29.EXUQ*/ Reg EXUQ_STORE8_RSTp;
  /*p31.GAVY*/ Reg STORE8_X4;
  /*p31.GYPU*/ Reg STORE8_X5;
  /*p31.GADY*/ Reg STORE8_X6;
  /*p31.GAZA*/ Reg STORE8_X7;
  /*p31.EZUF*/ Reg STORE8_X0;
  /*p31.ENAD*/ Reg STORE8_X1;
  /*p31.EBOW*/ Reg STORE8_X2;
  /*p31.FYCA*/ Reg STORE8_X3;
  /*p30.AXUV*/ Reg STORE8_IDX5;
  /*p30.BADA*/ Reg STORE8_IDX4;
  /*p30.APEV*/ Reg STORE8_IDX3;
  /*p30.BADO*/ Reg STORE8_IDX2;
  /*p30.BEXY*/ Reg STORE8_IDX1;
  /*p30.BYHE*/ Reg STORE8_IDX0;
  /*p30.AFYM*/ Reg STORE8_LINE0;
  /*p30.AZAP*/ Reg STORE8_LINE1;
  /*p30.AFUT*/ Reg STORE8_LINE2;
  /*p30.AFYX*/ Reg STORE8_LINE3;

  // sprite store 9
  /*p29.FONO*/ Reg FONO_STORE9_RSTp;
  /*p31.XUVY*/ Reg STORE9_X0;
  /*p31.XERE*/ Reg STORE9_X1;
  /*p31.XUZO*/ Reg STORE9_X2;
  /*p31.XEXA*/ Reg STORE9_X3;
  /*p31.YPOD*/ Reg STORE9_X4;
  /*p31.YROP*/ Reg STORE9_X5;
  /*p31.YNEP*/ Reg STORE9_X6;
  /*p31.YZOF*/ Reg STORE9_X7;
  /*p30.XUFO*/ Reg STORE9_IDX0;
  /*p30.XUTE*/ Reg STORE9_IDX1;
  /*p30.XOTU*/ Reg STORE9_IDX2;
  /*p30.XYFE*/ Reg STORE9_IDX3;
  /*p30.YZOR*/ Reg STORE9_IDX4;
  /*p30.YBER*/ Reg STORE9_IDX5;
  /*p30.DEWU*/ Reg STORE9_LINE0;
  /*p30.CANA*/ Reg STORE9_LINE1;
  /*p30.DYSY*/ Reg STORE9_LINE2;
  /*p30.FOFO*/ Reg STORE9_LINE3;
};

//-----------------------------------------------------------------------------

}; // namespace Schematics
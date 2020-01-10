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

  /*p29.EGAV*/ Reg2 SPRITE2_GET_SYNCn;
  /*p30.BOXA*/ Reg2 STORE2_IDX0;
  /*p30.BUNA*/ Reg2 STORE2_IDX1;
  /*p30.BULU*/ Reg2 STORE2_IDX2;
  /*p30.BECA*/ Reg2 STORE2_IDX3;
  /*p30.BYHU*/ Reg2 STORE2_IDX4;
  /*p30.BUHE*/ Reg2 STORE2_IDX5;
  /*p30.YKUK*/ Reg2 STORE2_LINE0;
  /*p30.YLOV*/ Reg2 STORE2_LINE1;
  /*p30.XAZY*/ Reg2 STORE2_LINE2;
  /*p30.XOSY*/ Reg2 STORE2_LINE3;
  /*p31.FOKA*/ Reg2 STORE2_X0;
  /*p31.FYTY*/ Reg2 STORE2_X1;
  /*p31.FUBY*/ Reg2 STORE2_X2;
  /*p31.GOXU*/ Reg2 STORE2_X3;
  /*p31.DUHY*/ Reg2 STORE2_X4;
  /*p31.EJUF*/ Reg2 STORE2_X5;
  /*p31.ENOR*/ Reg2 STORE2_X6;
  /*p31.DEPY*/ Reg2 STORE2_X7;

  /*p29.GOTA*/ Reg2 SPRITE3_GET_SYNCn;
  /*p30.DAFU*/ Reg2 DAFU;
  /*p30.DEBA*/ Reg2 DEBA;
  /*p30.DUHA*/ Reg2 DUHA;
  /*p30.DUNY*/ Reg2 DUNY;
  /*p30.DESE*/ Reg2 DESE;
  /*p30.DEVY*/ Reg2 DEVY;
  /*p30.ZURY*/ Reg2 ZURY;
  /*p30.ZURO*/ Reg2 ZURO;
  /*p30.ZENE*/ Reg2 ZENE;
  /*p30.ZYLU*/ Reg2 ZYLU;
  /*p31.XOLY*/ Reg2 XOLY;
  /*p31.XYBA*/ Reg2 XYBA;
  /*p31.XABE*/ Reg2 XABE;
  /*p31.XEKA*/ Reg2 XEKA;
  /*p31.XOMY*/ Reg2 XOMY;
  /*p31.WUHA*/ Reg2 WUHA;
  /*p31.WYNA*/ Reg2 WYNA;
  /*p31.WECO*/ Reg2 WECO;

  /*p29.XUDY*/ Reg2 SPRITE4_GET_SYNCn;
  /*p31.WEDU*/ Reg2 WEDU;
  /*p31.YGAJ*/ Reg2 YGAJ;
  /*p31.ZYJO*/ Reg2 ZYJO;
  /*p31.XURY*/ Reg2 XURY;
  /*p31.YBED*/ Reg2 YBED;
  /*p31.ZALA*/ Reg2 ZALA;
  /*p31.WYDE*/ Reg2 WYDE;
  /*p31.XEPA*/ Reg2 XEPA;
  /*p30.XYNU*/ Reg2 XYNU;
  /*p30.XEGE*/ Reg2 XEGE;
  /*p30.XABO*/ Reg2 XABO;
  /*p30.WANU*/ Reg2 WANU;
  /*p30.XEFE*/ Reg2 XEFE;
  /*p30.XAVE*/ Reg2 XAVE;
  /*p30.CUMU*/ Reg2 CUMU;
  /*p30.CAPO*/ Reg2 CAPO;
  /*p30.CONO*/ Reg2 CONO;
  /*p30.CAJU*/ Reg2 CAJU;

  // store 5
  /*p29.WAFY*/ Reg2 SPRITE5_GET_SYNCn;
  /*p31.FUSA*/ Reg2 FUSA;
  /*p31.FAXA*/ Reg2 FAXA;
  /*p31.FOZY*/ Reg2 FOZY;
  /*p31.FESY*/ Reg2 FESY;
  /*p31.CYWE*/ Reg2 CYWE;
  /*p31.DYBY*/ Reg2 DYBY;
  /*p31.DURY*/ Reg2 DURY;
  /*p31.CUVY*/ Reg2 CUVY;
  /*p30.EKAP*/ Reg2 EKAP;
  /*p30.ETAV*/ Reg2 ETAV;
  /*p30.EBEX*/ Reg2 EBEX;
  /*p30.GORU*/ Reg2 GORU;
  /*p30.ETYM*/ Reg2 ETYM;
  /*p30.EKOP*/ Reg2 EKOP;
  /*p30.ANED*/ Reg2 ANED;
  /*p30.ACEP*/ Reg2 ACEP;
  /*p30.ABUX*/ Reg2 ABUX;
  /*p30.ABEG*/ Reg2 ABEG;

  // sprite store 6
  /*p29.WOMY*/ Reg2 SPRITE6_GET_SYNCn;
  /*p31.YCOL*/ Reg2 YCOL;
  /*p31.YRAC*/ Reg2 YRAC;
  /*p31.YMEM*/ Reg2 YMEM;
  /*p31.YVAG*/ Reg2 YVAG;
  /*p31.ZOLY*/ Reg2 ZOLY;
  /*p31.ZOGO*/ Reg2 ZOGO;
  /*p31.ZECU*/ Reg2 ZECU;
  /*p31.ZESA*/ Reg2 ZESA;
  /*p30.GECU*/ Reg2 GECU;
  /*p30.FOXY*/ Reg2 FOXY;
  /*p30.GOHU*/ Reg2 GOHU;
  /*p30.FOGO*/ Reg2 FOGO;
  /*p30.GACY*/ Reg2 GACY;
  /*p30.GABO*/ Reg2 GABO;
  /*p30.ZUBE*/ Reg2 ZUBE;
  /*p30.ZUMY*/ Reg2 ZUMY;
  /*p30.ZEXO*/ Reg2 ZEXO;
  /*p30.ZAFU*/ Reg2 ZAFU;

  // sprite store 7
  /*p29.WAPO*/ Reg2 SPRITE7_GET_SYNCn;
  /*p31.ERAZ*/ Reg2 STORE7_X0;
  /*p31.EPUM*/ Reg2 STORE7_X1;
  /*p31.EROL*/ Reg2 STORE7_X2;
  /*p31.EHYN*/ Reg2 STORE7_X3;
  /*p31.FAZU*/ Reg2 STORE7_X4;
  /*p31.FAXE*/ Reg2 STORE7_X5;
  /*p31.EXUK*/ Reg2 STORE7_X6;
  /*p31.FEDE*/ Reg2 STORE7_X7;
  /*p30.FUZO*/ Reg2 FUZO;
  /*p30.GESY*/ Reg2 GESY;
  /*p30.FYSU*/ Reg2 FYSU;
  /*p30.FEFA*/ Reg2 FEFA;
  /*p30.GYNO*/ Reg2 GYNO;
  /*p30.GULE*/ Reg2 GULE;
  /*p30.XYGO*/ Reg2 XYGO;
  /*p30.XYNA*/ Reg2 XYNA;
  /*p30.XAKU*/ Reg2 XAKU;
  /*p30.YGUM*/ Reg2 YGUM;

  // sprite store 8
  /*p29.EXUQ*/ Reg2 SPRITE8_GET_SYNCn;
  /*p31.GAVY*/ Reg2 GAVY;
  /*p31.GYPU*/ Reg2 GYPU;
  /*p31.GADY*/ Reg2 GADY;
  /*p31.GAZA*/ Reg2 GAZA;
  /*p31.EZUF*/ Reg2 EZUF;
  /*p31.ENAD*/ Reg2 ENAD;
  /*p31.EBOW*/ Reg2 EBOW;
  /*p31.FYCA*/ Reg2 FYCA;
  /*p30.AXUV*/ Reg2 AXUV;
  /*p30.BADA*/ Reg2 BADA;
  /*p30.APEV*/ Reg2 APEV;
  /*p30.BADO*/ Reg2 BADO;
  /*p30.BEXY*/ Reg2 BEXY;
  /*p30.BYHE*/ Reg2 BYHE;
  /*p30.AFYM*/ Reg2 AFYM;
  /*p30.AZAP*/ Reg2 AZAP;
  /*p30.AFUT*/ Reg2 AFUT;
  /*p30.AFYX*/ Reg2 AFYX;

  // sprite store 9
  /*p29.FONO*/ Reg2 SPRITE9_GET_SYNCn;
  /*p31.XUVY*/ Reg2 STORE9_X0;
  /*p31.XERE*/ Reg2 STORE9_X1;
  /*p31.XUZO*/ Reg2 STORE9_X2;
  /*p31.XEXA*/ Reg2 STORE9_X3;
  /*p31.YPOD*/ Reg2 STORE9_X4;
  /*p31.YROP*/ Reg2 STORE9_X5;
  /*p31.YNEP*/ Reg2 STORE9_X6;
  /*p31.YZOF*/ Reg2 STORE9_X7;
  /*p30.XUFO*/ Reg2 STORE9_IDX0;
  /*p30.XUTE*/ Reg2 STORE9_IDX1;
  /*p30.XOTU*/ Reg2 STORE9_IDX2;
  /*p30.XYFE*/ Reg2 STORE9_IDX3;
  /*p30.YZOR*/ Reg2 STORE9_IDX4;
  /*p30.YBER*/ Reg2 STORE9_IDX5;
  /*p30.DEWU*/ Reg2 STORE9_LINE0;
  /*p30.CANA*/ Reg2 STORE9_LINE1;
  /*p30.DYSY*/ Reg2 STORE9_LINE2;
  /*p30.FOFO*/ Reg2 STORE9_LINE3;
};


//-----------------------------------------------------------------------------

};
#pragma once

namespace Schematics {

//-----------------------------------------------------------------------------

struct Sprites {

  /*p28.BODE*/ bool OAM_LATCH;






  /*p28.ACYL*/ bool OAM_ADDR_PARSEn;

  /*p28.GEKA*/ bool OAM_A0;
  /*p28.ZYFO*/ bool OAM_A1;
  /*p28.YFOT*/ bool OAM_A2;
  /*p28.YFOC*/ bool OAM_A3;
  /*p28.YVOM*/ bool OAM_A4;
  /*p28.YMEV*/ bool OAM_A5;
  /*p28.XEMU*/ bool OAM_A6;
  /*p28.YZET*/ bool OAM_A7;


  /*p28.ATEJ*/ bool NEW_LINE; // drives sprite count reset, must be new line

  /*p28.BYVA*/ bool NEW_LINE3;
  /*p29.DYBA*/ bool NEW_LINE4;



  /*p28.WEFE*/ bool P10_Bn;

  /*p28.AWOH*/ bool CLK_2Mn;

  /*p28.GAVA*/ bool SCAN_CLK;


  /*p28.ZONE*/ bool OAM_A_CS;
  /*p28.ZOFE*/ bool OAM_B_CS;








  /*p01.TAVA*/ bool CLK_AxCxExGx10; // sprites

  /*p29.CATU*/ bool IN_LINE_SYNCa;
  /*p28.ANEL*/ bool IN_LINE_SYNCb;

  /*p28.YFEL*/ bool SCAN0;
  /*p28.WEWY*/ bool SCAN1;
  /*p28.GOSO*/ bool SCAN2;
  /*p28.ELYN*/ bool SCAN3;
  /*p28.FAHA*/ bool SCAN4;
  /*p28.FONY*/ bool SCAN5;





  /*p28.BOFE*/ bool FROM_CPU5n;






  /*p28.XUCA*/ bool XUCA; // unused
  /*p28.XECY*/ bool XECY; // unused
  /*p28.XUVA*/ bool XUVA; // unused


  /*p29.TACU*/ bool SEQ_5_TRIG;

  /*p29.AVAP*/ bool SCAN_DONE_TRIG;
  /*p29.FEPO*/ bool STORE_MATCH;
  /*p29.WUTY*/ bool WUTY_CLK;
  /*p29.XEFY*/ bool XEFY_CLK;
  /*p29.PUCO*/ bool SPRITE_PIX_LATCH_B;
  /*p29.XADO*/ bool SPRITE_PIX_LATCH_A;

  /*p29.TYFO*/ bool SEQ_5_SYNCn;
  /*p29.XOCE*/ bool CLK_XOCE;

  /*p29.DENY*/ bool DENY;
  /*p29.XYME*/ bool XYME;
  /*p29.GOWO*/ bool GOWO;
  /*p29.GUGY*/ bool GUGY;

  /*p29.GYGY*/ bool GYGY;


  /*p29.BUZA*/ bool STORE_SPRITE_IDX;

  // sprite matcher clock lines
  /*p29.YFAG*/ bool YFAG;
  /*p29.GECY*/ bool GECY;

  /*p29.XAHO*/ bool XAHO;
  /*p29.WOFO*/ bool WOFO;
  /*p29.CEXU*/ bool CEXU;

  /*p29.WEME*/ bool STORE9_CLKa;
  /*p29.WUFA*/ bool STORE9_CLKb;
  /*p29.FAKA*/ bool STORE9_CLKc;

  /*p29.CYLA*/ bool CYLA;



  /*p29.ASYS*/ bool STORE1_CLKa;
  /*p29.AHOF*/ bool STORE1_CLKb;
  /*p29.BYVY*/ bool STORE1_CLKc;

  // sprite matcher reset lines
  
  /*p29.GAFY*/ bool GAFY;
  /*p29.ZAPE*/ bool ZAPE;
  /*p29.WUNU*/ bool WUNU;
  /*p29.WUZO*/ bool WUZO;

  /*p29.EJAD*/ bool EJAD;

  // sprite store clock lines
  /*p29.AKOL*/ bool AKOL;
  /*p29.BYMY*/ bool BYMY;
  /*p29.WUSE*/ bool WUSE;
  /*p29.ZURU*/ bool ZURU;
  /*p29.WYLU*/ bool WYLU;
  /*p29.EWOT*/ bool EWOT;
  /*p29.FEFO*/ bool FEFO;
  /*p29.WABE*/ bool WABE;
  /*p29.DYMO*/ bool DYMO;
  /*p29.BUCY*/ bool BUCY;
  /*p29.FEKA*/ bool FEKA;
  /*p29.XYHA*/ bool XYHA;

  /*p29.YDYV*/ bool OAM_B_LATCH0;
  /*p29.YCEB*/ bool OAM_B_LATCH1;
  /*p29.ZUCA*/ bool OAM_B_LATCH2;
  /*p29.WONE*/ bool OAM_B_LATCH3;
  /*p29.ZAXE*/ bool OAM_B_LATCH4;
  /*p29.XAFU*/ bool OAM_B_LATCH5;
  /*p29.YSES*/ bool OAM_B_LATCH6;
  /*p29.ZECA*/ bool OAM_B_LATCH7;


  /*p29.XUSO*/ bool OAM_B_D0;
  /*p29.XEGU*/ bool OAM_B_D1;
  /*p29.YJEX*/ bool OAM_B_D2;
  /*p29.XYJU*/ bool OAM_B_D3;
  /*p29.YBOG*/ bool OAM_B_D4;
  /*p29.WYSO*/ bool OAM_B_D5;
  /*p29.XOTE*/ bool OAM_B_D6;
  /*p29.YZAB*/ bool OAM_B_D7;


  /*p29.YWOK*/ bool OAM_B_IN_CLK;










  /*p29.WOSU*/ bool CLK_WOSU;

  /*p29.CENO*/ bool CENO;
  /*p29.CEHA*/ bool CEHA;

  /*p29.DYTY*/ bool STORE_EN;

  /*p29.BYBA*/ bool SCAN_DONE_SYNC1;
  /*p29.DOBA*/ bool SCAN_DONE_SYNC2;


  /*p29.TOBU*/ bool TULY_SYNC1;
  /*p29.VONU*/ bool SPRITE_ABn;

  /*p29.TEPA*/ bool RENDERINGn;

  /*p29.TOMA*/ bool TOMA_CLK;


  /*p29.SEBA*/ bool SEBA;

  /*p29.TOXE*/ bool SEQ0;
  /*p29.TULY*/ bool SEQ1;
  /*p29.TESE*/ bool SEQ2;

  /*p29.BYJO*/ bool BYJO;
  /*p29.AZEM*/ bool AZEM;
  /*p29.AROR*/ bool MATCH_EN;
  /*p29.XAGE*/ bool STORE4_MATCHn;
  /*p29.YLOZ*/ bool STORE3_MATCHn;

  /*p29.DYDU*/ bool STORE1_MATCHn;
  /*p29.YGEM*/ bool STORE9_MATCHn;
  /*p29.EFYL*/ bool STORE8_MATCHn;
  /*p29.DYKA*/ bool STORE7_MATCHn;
  /*p29.YBEZ*/ bool STORE6_MATCHn;
  /*p29.EGOM*/ bool STORE5_MATCHn;


  /*p29.FUMA*/ bool STORE1_MATCH_OUT;
  /*p29.GEDE*/ bool STORE2_MATCH_OUT;

  /*p29.DEGO*/ bool STORE2_MATCHn;

  /*p29.WUTO*/ bool STORE3_MATCH_OUT;

  /*p29.WUNA*/ bool WUNA;
  /*p29.XYLA*/ bool XYLA;
  /*p29.GONO*/ bool GONO;
  /*p29.GABA*/ bool GABA;
  /*p29.WEJA*/ bool WEJA;
  /*p29.GEGA*/ bool GEGA;
  /*p29.WASE*/ bool WASE;
  /*p29.WYLA*/ bool WYLA;
  /*p29.XOJA*/ bool XOJA;
  /*p29.GYTE*/ bool GYTE;
  /*p29.FAVO*/ bool FAVO;
  /*p29.GUTU*/ bool GUTU;
  /*p29.GEKE*/ bool GEKE;
  /*p29.GYGA*/ bool GYGA;
  /*p29.FOXA*/ bool FOXA;


  
  /*p29.FONO*/ bool SPRITE9_GET_SYNCn;
  /*p29.EXUQ*/ bool SPRITE8_GET_SYNCn;
  /*p29.WAPO*/ bool SPRITE7_GET_SYNCn;
  /*p29.WOMY*/ bool SPRITE6_GET_SYNCn;
  /*p29.WAFY*/ bool SPRITE5_GET_SYNCn;
  /*p29.XUDY*/ bool SPRITE4_GET_SYNCn;
  /*p29.GOTA*/ bool SPRITE3_GET_SYNCn;
  /*p29.EGAV*/ bool SPRITE2_GET_SYNCn;
  /*p29.CEDY*/ bool SPRITE1_GET_SYNCn;


  /*p29.GORO*/ bool GORO;
  /*p29.GUKY*/ bool GUKY;
  /*p29.WACY*/ bool WACY;
  /*p29.FEVE*/ bool FEVE;
  /*p29.WOHU*/ bool WOHU;




  
  

  /*p29.FAMU*/ bool FAMU;


  /*p29.CAKE*/ bool SPRITE_COUNT_CLK;

  /*p29.EDEN*/ bool SPRITE_COUNT0n;
  /*p29.FYCU*/ bool SPRITE_COUNT0b;
  /*p29.CYPY*/ bool SPRITE_COUNT1n;
  /*p29.FONE*/ bool SPRITE_COUNT1b;
  /*p29.CAPE*/ bool SPRITE_COUNT2n;
  /*p29.EKUD*/ bool SPRITE_COUNT2b;
  /*p29.CAXU*/ bool SPRITE_COUNT3n;
  /*p29.ELYG*/ bool SPRITE_COUNT3b;



  /*p29.CACU*/ bool STORE2_CLKa;
  /*p29.BUZY*/ bool STORE2_CLKb;
  /*p29.FUKE*/ bool STORE2_CLKc;


  /*p29.CUPE*/ bool STORE4_SEL;
  /*p29.CECU*/ bool STORE4_CLK;

  /*p29.CUGU*/ bool STORE5_SEL;
  /*p29.CADO*/ bool STORE5_CLK;

  /*p29.WOMU*/ bool STORE6_SEL;
  /*p29.XUJO*/ bool STORE6_CLK;

  /*p29.GUNA*/ bool STORE7_SEL;
  /*p29.GAPE*/ bool STORE7_CLK;

  /*p29.DEWY*/ bool STORE8_SEL;
  /*p29.CAHO*/ bool STORE8_CLK;





  /*p29.WEKA*/ bool WEKA;
  /*p29.GYVO*/ bool GYVO;
  /*p29.BUKA*/ bool BUKA;

  /*p29.BEDE*/ bool BEDE;
  /*p29.DUKE*/ bool DUKE;
  /*p29.DEZY*/ bool STORE_EN_SYNC;

  /*p29.BESE*/ bool SPRITE_COUNT0;
  /*p29.CUXY*/ bool SPRITE_COUNT1;
  /*p29.BEGO*/ bool SPRITE_COUNT2;
  /*p29.DYBE*/ bool SPRITE_COUNT3;

  /*p30.WEZA*/ bool TS_IDX_0;
  /*p30.WUCO*/ bool TS_IDX_1;
  /*p30.WYDA*/ bool TS_IDX_2;
  /*p30.ZYSU*/ bool TS_IDX_3;
  /*p30.WYSE*/ bool TS_IDX_4;
  /*p30.WUZY*/ bool TS_IDX_5;

  /*p30.WENU*/ bool TS_LINE_0;
  /*p30.CUCU*/ bool TS_LINE_1;
  /*p30.CUCA*/ bool TS_LINE_2;
  /*p30.CEGA*/ bool TS_LINE_3;

  /*p30.CYKE*/ bool CYKE;
  /*p30.WUDA*/ bool WUDA;

  /*p30.XECU*/ bool IDX_5;
  /*p30.YDUF*/ bool IDX_4;
  /*p30.XOBE*/ bool IDX_3;
  /*p30.ZUZE*/ bool IDX_2;
  /*p30.XEDY*/ bool IDX_1;
  /*p30.XADU*/ bool IDX_0;

  /*p30.AXUV*/ bool AXUV;
  /*p30.BADA*/ bool BADA;
  /*p30.APEV*/ bool APEV;
  /*p30.BADO*/ bool BADO;
  /*p30.BEXY*/ bool BEXY;
  /*p30.BYHE*/ bool BYHE;
  /*p30.AFYM*/ bool AFYM;
  /*p30.AZAP*/ bool AZAP;
  /*p30.AFUT*/ bool AFUT;
  /*p30.AFYX*/ bool AFYX;

  /*p30.GECU*/ bool GECU;
  /*p30.FOXY*/ bool FOXY;
  /*p30.GOHU*/ bool GOHU;
  /*p30.FOGO*/ bool FOGO;
  /*p30.GACY*/ bool GACY;
  /*p30.GABO*/ bool GABO;
  /*p30.ZUBE*/ bool ZUBE;
  /*p30.ZUMY*/ bool ZUMY;
  /*p30.ZEXO*/ bool ZEXO;
  /*p30.ZAFU*/ bool ZAFU;

  /*p30.XYNU*/ bool XYNU;
  /*p30.XEGE*/ bool XEGE;
  /*p30.XABO*/ bool XABO;
  /*p30.WANU*/ bool WANU;
  /*p30.XEFE*/ bool XEFE;
  /*p30.XAVE*/ bool XAVE;
  /*p30.CUMU*/ bool CUMU;
  /*p30.CAPO*/ bool CAPO;
  /*p30.CONO*/ bool CONO;
  /*p30.CAJU*/ bool CAJU;

  /*p30.FUZO*/ bool FUZO;
  /*p30.GESY*/ bool GESY;
  /*p30.FYSU*/ bool FYSU;
  /*p30.FEFA*/ bool FEFA;
  /*p30.GYNO*/ bool GYNO;
  /*p30.GULE*/ bool GULE;
  /*p30.XYGO*/ bool XYGO;
  /*p30.XYNA*/ bool XYNA;
  /*p30.XAKU*/ bool XAKU;
  /*p30.YGUM*/ bool YGUM;

  /*p30.CAJY*/ bool STORE1_IDX0;
  /*p30.CUZA*/ bool STORE1_IDX1;
  /*p30.COMA*/ bool STORE1_IDX2;
  /*p30.CUFA*/ bool STORE1_IDX3;
  /*p30.CEBO*/ bool STORE1_IDX4;
  /*p30.CADU*/ bool STORE1_IDX5;

  /*p30.ABUG*/ bool STORE1_LINE0;
  /*p30.AMES*/ bool STORE1_LINE1;
  /*p30.ABOP*/ bool STORE1_LINE2;
  /*p30.AROF*/ bool STORE1_LINE3;

  /*p30.EKAP*/ bool EKAP;
  /*p30.ETAV*/ bool ETAV;
  /*p30.EBEX*/ bool EBEX;
  /*p30.GORU*/ bool GORU;
  /*p30.ETYM*/ bool ETYM;
  /*p30.EKOP*/ bool EKOP;
  /*p30.ANED*/ bool ANED;
  /*p30.ACEP*/ bool ACEP;
  /*p30.ABUX*/ bool ABUX;
  /*p30.ABEG*/ bool ABEG;

  /*p30.DAFU*/ bool DAFU;
  /*p30.DEBA*/ bool DEBA;
  /*p30.DUHA*/ bool DUHA;
  /*p30.DUNY*/ bool DUNY;
  /*p30.DESE*/ bool DESE;
  /*p30.DEVY*/ bool DEVY;
  /*p30.ZURY*/ bool ZURY;
  /*p30.ZURO*/ bool ZURO;
  /*p30.ZENE*/ bool ZENE;
  /*p30.ZYLU*/ bool ZYLU;

  /*p30.BOXA*/ bool STORE2_IDX0;
  /*p30.BUNA*/ bool STORE2_IDX1;
  /*p30.BULU*/ bool STORE2_IDX2;
  /*p30.BECA*/ bool STORE2_IDX3;
  /*p30.BYHU*/ bool STORE2_IDX4;
  /*p30.BUHE*/ bool STORE2_IDX5;
  /*p30.YKUK*/ bool STORE2_LINE0;
  /*p30.YLOV*/ bool STORE2_LINE1;
  /*p30.XAZY*/ bool STORE2_LINE2;
  /*p30.XOSY*/ bool STORE2_LINE3;

  /*p30.XUFO*/ bool STORE9_IDX0;
  /*p30.XUTE*/ bool STORE9_IDX1;
  /*p30.XOTU*/ bool STORE9_IDX2;
  /*p30.XYFE*/ bool STORE9_IDX3;
  /*p30.YZOR*/ bool STORE9_IDX4;
  /*p30.YBER*/ bool STORE9_IDX5;
  /*p30.DEWU*/ bool STORE9_LINE0;
  /*p30.CANA*/ bool STORE9_LINE1;
  /*p30.DYSY*/ bool STORE9_LINE2;
  /*p30.FOFO*/ bool STORE9_LINE3;

  //----------
  // Sprite store 0

  /*p29.GENY*/ bool STORE0_CLKa;
  /*p29.ENOB*/ bool STORE0_CLKb;
  /*p29.FUXU*/ bool STORE0_CLKc;

  /*p30.YGUS*/ bool STORE0_IDX0;
  /*p30.YSOK*/ bool STORE0_IDX1;
  /*p30.YZEP*/ bool STORE0_IDX2;
  /*p30.WYTE*/ bool STORE0_IDX3;
  /*p30.ZONY*/ bool STORE0_IDX4;
  /*p30.YWAK*/ bool STORE0_IDX5;
  /*p30.FYHY*/ bool STORE0_LINE0;
  /*p30.GYHO*/ bool STORE0_LINE1;
  /*p30.BOZU*/ bool STORE0_LINE2;
  /*p30.CUFO*/ bool STORE0_LINE3;
  /*p31.XEPE*/ bool STORE0_X0;
  /*p31.YLAH*/ bool STORE0_X1;
  /*p31.ZOLA*/ bool STORE0_X2;
  /*p31.ZULU*/ bool STORE0_X3;
  /*p31.WELO*/ bool STORE0_X4;
  /*p31.XUNY*/ bool STORE0_X5;
  /*p31.WOTE*/ bool STORE0_X6;
  /*p31.XAKO*/ bool STORE0_X7;

  /*p29.YDUG*/ bool STORE0_MATCHn;
  /*p29.GEZE*/ bool STORE0_MATCH_OUT;
  /*p29.EBOJ*/ bool SPRITE0_GET_SYNCn;

  //----------

  /*p31.XYKY*/ bool OAM_A_LATCH0;
  /*p31.YRUM*/ bool OAM_A_LATCH1;
  /*p31.YSEX*/ bool OAM_A_LATCH2;
  /*p31.YVEL*/ bool OAM_A_LATCH3;
  /*p31.WYNO*/ bool OAM_A_LATCH4;
  /*p31.CYRA*/ bool OAM_A_LATCH5;
  /*p31.ZUVE*/ bool OAM_A_LATCH6;
  /*p31.ECED*/ bool OAM_A_LATCH7;

  /*p31.XEGA*/ bool OAM_A_IN_CLK;

  /*p31.YLOR*/ bool OAM_A_D0;
  /*p31.ZYTY*/ bool OAM_A_D1;
  /*p31.ZYVE*/ bool OAM_A_D2;
  /*p31.ZEZY*/ bool OAM_A_D3;
  /*p31.GOMO*/ bool OAM_A_D4;
  /*p31.BAXO*/ bool OAM_A_D5;
  /*p31.YZOS*/ bool OAM_A_D6;
  /*p31.DEPO*/ bool OAM_A_D7;

  /*p31.ZAGO*/ bool OAM_A_D0b;
  /*p31.ZOCY*/ bool OAM_A_D1b;
  /*p31.YPUR*/ bool OAM_A_D2b;
  /*p31.YVOK*/ bool OAM_A_D3b;
  /*p31.COSE*/ bool OAM_A_D4b;
  /*p31.AROP*/ bool OAM_A_D5b;
  /*p31.XATU*/ bool OAM_A_D6b;
  /*p31.BADY*/ bool OAM_A_D7b;

  /*p31.XOMY*/ bool XOMY;
  /*p31.WUHA*/ bool WUHA;
  /*p31.WYNA*/ bool WYNA;
  /*p31.WECO*/ bool WECO;
  /*p31.XOLY*/ bool XOLY;
  /*p31.XYBA*/ bool XYBA;
  /*p31.XABE*/ bool XABE;
  /*p31.XEKA*/ bool XEKA;

  /*p31.FAZU*/ bool FAZU;
  /*p31.FAXE*/ bool FAXE;
  /*p31.EXUK*/ bool EXUK;
  /*p31.FEDE*/ bool FEDE;
  /*p31.ERAZ*/ bool ERAZ;
  /*p31.EPUM*/ bool EPUM;
  /*p31.EROL*/ bool EROL;
  /*p31.EHYN*/ bool EHYN;

  /*p31.DANY*/ bool STORE1_X0;
  /*p31.DUKO*/ bool STORE1_X1;
  /*p31.DESU*/ bool STORE1_X2;
  /*p31.DAZO*/ bool STORE1_X3;
  /*p31.DAKE*/ bool STORE1_X4;
  /*p31.CESO*/ bool STORE1_X5;
  /*p31.DYFU*/ bool STORE1_X6;
  /*p31.CUSY*/ bool STORE1_X7;

  /*p31.ZOLY*/ bool ZOLY;
  /*p31.ZOGO*/ bool ZOGO;
  /*p31.ZECU*/ bool ZECU;
  /*p31.ZESA*/ bool ZESA;
  /*p31.YCOL*/ bool YCOL;
  /*p31.YRAC*/ bool YRAC;
  /*p31.YMEM*/ bool YMEM;
  /*p31.YVAG*/ bool YVAG;

  /*p31.YBED*/ bool YBED;
  /*p31.ZALA*/ bool ZALA;
  /*p31.WYDE*/ bool WYDE;
  /*p31.XEPA*/ bool XEPA;
  /*p31.WEDU*/ bool WEDU;
  /*p31.YGAJ*/ bool YGAJ;
  /*p31.ZYJO*/ bool ZYJO;
  /*p31.XURY*/ bool XURY;

  /*p31.EZUF*/ bool EZUF;
  /*p31.ENAD*/ bool ENAD;
  /*p31.EBOW*/ bool EBOW;
  /*p31.FYCA*/ bool FYCA;
  /*p31.GAVY*/ bool GAVY;
  /*p31.GYPU*/ bool GYPU;
  /*p31.GADY*/ bool GADY;
  /*p31.GAZA*/ bool GAZA;

  /*p31.XUVY*/ bool STORE9_X0;
  /*p31.XERE*/ bool STORE9_X1;
  /*p31.XUZO*/ bool STORE9_X2;
  /*p31.XEXA*/ bool STORE9_X3;
  /*p31.YPOD*/ bool STORE9_X4;
  /*p31.YROP*/ bool STORE9_X5;
  /*p31.YNEP*/ bool STORE9_X6;
  /*p31.YZOF*/ bool STORE9_X7;

  /*p31.CYWE*/ bool CYWE;
  /*p31.DYBY*/ bool DYBY;
  /*p31.DURY*/ bool DURY;
  /*p31.CUVY*/ bool CUVY;
  /*p31.FUSA*/ bool FUSA;
  /*p31.FAXA*/ bool FAXA;
  /*p31.FOZY*/ bool FOZY;
  /*p31.FESY*/ bool FESY;

  /*p31.FOKA*/ bool STORE2_X0;
  /*p31.FYTY*/ bool STORE2_X1;
  /*p31.FUBY*/ bool STORE2_X2;
  /*p31.GOXU*/ bool STORE2_X3;
  /*p31.DUHY*/ bool STORE2_X4;
  /*p31.EJUF*/ bool STORE2_X5;
  /*p31.ENOR*/ bool STORE2_X6;
  /*p31.DEPY*/ bool STORE2_X7;
};

//-----------------------------------------------------------------------------

};
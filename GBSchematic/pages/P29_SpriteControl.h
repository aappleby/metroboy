#pragma once

struct P29_SpriteControl {

  /*p29.XONO*/ bool FLIP_X;
  /*p29.GESE*/ bool SPR_MATCH;



  /*p29.TACU*/ bool TACU;
  /*p29.XYSO*/ bool XYSO;
  /*p29.TEXY*/ bool TEXY;
  /*p29.AVAP*/ bool AVAP;
  /*p29.FEPO*/ bool SPRITE_MATCH_X;
  /*p29.WUTY*/ bool WUTY;
  /*p29.XEFY*/ bool XEFY;
  /*p29.PUCO*/ bool PUCO;
  /*p29.XADO*/ bool XADO;
  /*p29.TUVO*/ bool TUVO;
  /*p29.TYFO*/ bool TYFO;
  /*p29.XOCE*/ bool CLK_XOCE;
  /*p29.CATU*/ bool IN_FRAME;
  /*p29.DENY*/ bool DENY;
  /*p29.XYME*/ bool XYME;
  /*p29.GOWO*/ bool GOWO;
  /*p29.GUGY*/ bool GUGY;
  /*p29.DYDO*/ bool DYDO;
  /*p29.GYGY*/ bool GYGY;
  /*p29.GYMA*/ bool GYMA;
  /*p29.FAME*/ bool FAME;
  /*p29.FADO*/ bool FADO;
  /*p29.FURO*/ bool SPRITE0_SELo;
  /*p29.BUZA*/ bool BUZA;
  
  /*p29.DEGE*/ bool SPRITE_DELTAY0;
  /*p29.DABY*/ bool SPRITE_DELTAY1;
  /*p29.DABU*/ bool SPRITE_DELTAY2;
  /*p29.GYSA*/ bool SPRITE_DELTAY3;
  /*p29.GACE*/ bool SPRITE_DELTAY4;
  /*p29.GUVU*/ bool SPRITE_DELTAY5;
  /*p29.GYDA*/ bool SPRITE_DELTAY6;
  /*p29.GEWY*/ bool SPRITE_DELTAY7;

  // sprite matcher clock lines
  /*p29.FUXU*/ bool MATCHER0_CLKb;
  /*p29.YFAG*/ bool YFAG;
  /*p29.GECY*/ bool GECY;
  /*p29.DOKU*/ bool DOKU;
  /*p29.XAHO*/ bool XAHO;
  /*p29.WOFO*/ bool WOFO;
  /*p29.CEXU*/ bool CEXU;
  /*p29.WEME*/ bool WEME;
  /*p29.CYLA*/ bool CYLA;
  /*p29.GAMY*/ bool GAMY;

  // sprite matcher reset lines
  /*p29.DYNA*/ bool MATCHER0_RST;
  /*p29.WUPA*/ bool WUPA;
  /*p29.GAFY*/ bool GAFY;
  /*p29.ASYS*/ bool ASYS;
  /*p29.ZAPE*/ bool ZAPE;
  /*p29.WUNU*/ bool WUNU;
  /*p29.WUZO*/ bool WUZO;
  /*p29.DOSY*/ bool DOSY;
  /*p29.EJAD*/ bool EJAD;
  /*p29.CACU*/ bool CACU;

  // sprite store clock lines
  /*p29.AKOL*/ bool AKOL;
  /*p29.BYMY*/ bool BYMY;
  /*p29.WUSE*/ bool WUSE;
  /*p29.ZURU*/ bool ZURU;
  /*p29.WYLU*/ bool WYLU;
  /*p29.EWOT*/ bool EWOT;
  /*p29.FEFO*/ bool FEFO;
  /*p29.WABE*/ bool WABE;
  /*p29.BYVY*/ bool BYVY;
  /*p29.AHOF*/ bool AHOF;
  /*p29.DYMO*/ bool DYMO;
  /*p29.BUCY*/ bool BUCY;
  /*p29.FEKA*/ bool FEKA;
  /*p29.XYHA*/ bool XYHA;
  /*p29.BUZY*/ bool BUZY;
  /*p29.FUKE*/ bool FUKE;
  /*p29.WUFA*/ bool WUFA;
  /*p29.FAKA*/ bool FAKA;
  /*p29.GENY*/ bool MATCHER0_CLKA;
  /*p29.ENOB*/ bool MATCHER0_CLKB;

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


  /*p29.YWOK*/ bool YWOK;



  /*p29.ABON*/ bool ABON;

  /*p29.EBOS*/ bool V0n;
  /*p29.DASA*/ bool V1n;
  /*p29.FUKY*/ bool V2n;
  /*p29.FUVE*/ bool V3n;
  /*p29.FEPU*/ bool V4n;
  /*p29.FOFA*/ bool V5n;
  /*p29.FEMO*/ bool V6n;
  /*p29.GUSU*/ bool V7n;

  /*p29.ERUC*/ bool ERUC_S;
  /*p29.ENEF*/ bool ENEF_S;
  /*p29.FECO*/ bool FECO_S;
  /*p29.GYKY*/ bool GYKY_S;
  /*p29.GOPU*/ bool GOPU_S;
  /*p29.FUWA*/ bool FUWA_S;
  /*p29.GOJU*/ bool GOJU_S;
  /*p29.WUHU*/ bool WUHU_S;
  /*p29.ERUC*/ bool ERUC_C;
  /*p29.ENEF*/ bool ENEF_C;
  /*p29.FECO*/ bool FECO_C;
  /*p29.GYKY*/ bool GYKY_C;
  /*p29.GOPU*/ bool GOPU_C;
  /*p29.FUWA*/ bool FUWA_C;
  /*p29.GOJU*/ bool GOJU_C;
  /*p29.WUHU*/ bool WUHU_C;

  /*p29.GOVU*/ bool GOVU;
  /*p29.WOTA*/ bool SPR_MATCHn;

  /*p29.ALES*/ bool LINE_144n;
  /*p29.ABOV*/ bool IN_FRAMEb;
  /*p29.WOSU*/ bool CLK_WOSU;
  /*p29.WOJO*/ bool WOJO;
  /*p29.CENO*/ bool CENO;
  /*p29.CEHA*/ bool CEHA;
  /*p29.CARE*/ bool CARE;
  /*p29.DYTY*/ bool DYTY;
  /*p29.BALU*/ bool SCAN_RSTa;
  /*p29.BAGY*/ bool SCAN_RSTo;
  /*p29.BYBA*/ bool BYBA;
  /*p29.DOBA*/ bool DOBA;
  /*p29.BEBU*/ bool BEBU;

  /*p29.TOBU*/ bool TOBU;
  /*p29.VONU*/ bool VONU;

  /*p29.TEPA*/ bool TEPA;
  /*p29.TYNO*/ bool TYNO;
  /*p29.VUSA*/ bool VUSA;
  /*p29.SAKY*/ bool SAKY;
  /*p29.TYSO*/ bool TYSO;
  /*p29.TAME*/ bool TAME;
  /*p29.TOMA*/ bool TOMA;
  /*p29.TYTU*/ bool TYTU;
  /*p29.SYCU*/ bool SYCU;
  /*p29.TOPU*/ bool TOPU;
  /*p29.RACA*/ bool RACA;
  /*p29.VYWA*/ bool VYWA;
  /*p29.WENY*/ bool WENY;
  /*p29.PEBY*/ bool PEBY;
  /*p29.NYBE*/ bool NYBE;
  /*p29.SEBA*/ bool SEBA;
  /*p29.TOXE*/ bool TOXE;
  /*p29.TULY*/ bool TULY;
  /*p29.TESE*/ bool TESE;

  /*p29.BYJO*/ bool BYJO;
  /*p29.AZEM*/ bool AZEM;
  /*p29.AROR*/ bool AROR;
  /*p29.XAGE*/ bool XAGE;
  /*p29.YLOZ*/ bool YLOZ;
  /*p29.DEGO*/ bool DEGO;
  /*p29.DYDU*/ bool DYDU;
  /*p29.YDUG*/ bool SPRITE0_MATCH_Xn;
  /*p29.YGEM*/ bool YGEM;
  /*p29.EFYL*/ bool EFYL;
  /*p29.DYKA*/ bool DYKA;
  /*p29.YBEZ*/ bool YBEZ;
  /*p29.EGOM*/ bool EGOM;
  /*p29.FEFY*/ bool FEFY;
  /*p29.FOVE*/ bool FOVE;
  /*p29.WEFU*/ bool SPRITE0_MATCH_X;
  /*p29.GEZE*/ bool GEZE;
  /*p29.GUVA*/ bool SPRITE0_SELn;
  /*p29.GAJA*/ bool GAJA;
  /*p29.FUMA*/ bool FUMA;
  /*p29.ENUT*/ bool ENUT;
  /*p29.GUPO*/ bool GUPO;
  /*p29.GEDE*/ bool GEDE;
  /*p29.EMOL*/ bool EMOL;
  /*p29.WEBO*/ bool WEBO;
  /*p29.WUTO*/ bool WUTO;
  /*p29.GYFY*/ bool GYFY;
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
  /*p29.GUZE*/ bool GUZE;
  /*p29.FONO*/ bool FONO;
  /*p29.EXUQ*/ bool EXUQ;
  /*p29.WAPO*/ bool WAPO;
  /*p29.WOMY*/ bool WOMY;
  /*p29.WAFY*/ bool WAFY;
  /*p29.XUDY*/ bool XUDY;
  /*p29.GOTA*/ bool GOTA;
  /*p29.EGAV*/ bool EGAV;
  /*p29.CEDY*/ bool CEDY;
  /*p29.EBOJ*/ bool SPRITE0_SEL_SYNCn;
  /*p29.DYBA*/ bool DYBA;
  /*p29.DUBU*/ bool DUBU;
  /*p29.GORO*/ bool GORO;
  /*p29.GUKY*/ bool GUKY;
  /*p29.WACY*/ bool WACY;
  /*p29.FEVE*/ bool FEVE;
  /*p29.WOHU*/ bool WOHU;
  /*p29.GAKE*/ bool GAKE;
  /*p29.FOKO*/ bool FOKO;
  /*p29.EFEV*/ bool EFEV;
  /*p29.DYWE*/ bool MATCHER0_RSTn;

  /*p29.WUKY*/ bool FLIP_Y;
  /*p29.WAGO*/ bool WAGO;
  /*p29.CYVU*/ bool CYVU;
  /*p29.BORE*/ bool BORE;
  /*p29.BUVY*/ bool BUVY;

  /*p29.XUQU*/ bool XUQU;

  /*p29.FUFO*/ bool SPRITE_SIZEn;
  /*p29.GEJY*/ bool GEJY;
  /*p29.FAMU*/ bool FAMU;

  /*p29.BAKY*/ bool BAKY;
  /*p29.CAKE*/ bool CAKE;
  /*p29.EDEN*/ bool EDEN;
  /*p29.FYCU*/ bool FYCU;
  /*p29.CYPY*/ bool CYPY;
  /*p29.FONE*/ bool FONE;
  /*p29.CAPE*/ bool CAPE;
  /*p29.EKUD*/ bool EKUD;
  /*p29.CAXU*/ bool CAXU;
  /*p29.ELYG*/ bool ELYG;
  /*p29.GEBU*/ bool GEBU;
  /*p29.WOMU*/ bool WOMU;
  /*p29.GUNA*/ bool GUNA;
  /*p29.FOCO*/ bool FOCO;
  /*p29.DEWY*/ bool DEWY;
  /*p29.DEZO*/ bool DEZO;
  /*p29.DOGU*/ bool DOGU;
  /*p29.CUGU*/ bool CUGU;
  /*p29.CUPE*/ bool CUPE;
  /*p29.CUVA*/ bool CUVA;
  /*p29.WYXO*/ bool WYXO;
  /*p29.XUJO*/ bool XUJO;
  /*p29.GAPE*/ bool GAPE;
  /*p29.GUVE*/ bool GUVE;
  /*p29.CAHO*/ bool CAHO;
  /*p29.CEMY*/ bool MATCHER0_CLK;
  /*p29.CATO*/ bool CATO;
  /*p29.CADO*/ bool CADO;
  /*p29.CECU*/ bool CECU;
  /*p29.BYBY*/ bool BYBY;
  /*p29.GYFO*/ bool GYFO;
  /*p29.WEKA*/ bool WEKA;
  /*p29.GYVO*/ bool GYVO;
  /*p29.GUSA*/ bool GUSA;
  /*p29.BUKA*/ bool BUKA;
  /*p29.DYHU*/ bool MATCHER0_CLKn;
  /*p29.DECU*/ bool DECU;
  /*p29.BEDE*/ bool BEDE;
  /*p29.DUKE*/ bool DUKE;
  /*p29.BUCO*/ bool BUCO;
  /*p29.DEZY*/ bool DEZY;
  /*p29.BESE*/ bool BESE;
  /*p29.CUXY*/ bool CUXY;
  /*p29.BEGO*/ bool BEGO;
  /*p29.DYBE*/ bool DYBE;
};




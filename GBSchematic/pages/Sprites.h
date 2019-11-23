#pragma once

struct Sprites {

  /*p28.BOGE*/ bool DMA_RUNNING_SYNCb;
  /*p28.FODO*/ bool DMA_A00n;
  /*p28.FESA*/ bool DMA_A01n;
  /*p28.FAGO*/ bool DMA_A02n;
  /*p28.FYKY*/ bool DMA_A03n;
  /*p28.ELUG*/ bool DMA_A04n;
  /*p28.EDOL*/ bool DMA_A05n;
  /*p28.FYDU*/ bool DMA_A06n;
  /*p28.FETU*/ bool DMA_A07n;

  /*p28.BODE*/ bool OAM_LATCH;

  /*p28.ASYT*/ bool OAM_LATCHn;
  /*p28.YVAL*/ bool OAM_LATCHo;
  /*p28.YRYV*/ bool OAM_LATCHb;

  /*p28.BETE*/ bool OAM_ADDR_RENDER;
  /*p28.AJON*/ bool OAM_ADDR_RENDERn;
  /*p28.APAR*/ bool OAM_ADDR_PARSE;
  /*p28.ACYL*/ bool OAM_ADDR_PARSEn;
  /*p28.ASAM*/ bool OAM_ADDR_CPU;

  /*p28.ADAH*/ bool ADDR_OAMn;
  /*p28.GEKA*/ bool OAM_A0;
  /*p28.WAFO*/ bool OAM_A0n;
  /*p28.ZYFO*/ bool OAM_A1;
  /*p28.YFOT*/ bool OAM_A2;
  /*p28.YFOC*/ bool OAM_A3;
  /*p28.YVOM*/ bool OAM_A4;
  /*p28.YMEV*/ bool OAM_A5;
  /*p28.XEMU*/ bool OAM_A6;
  /*p28.YZET*/ bool OAM_A7;

  /*p28.BYHA*/ bool NEW_LINEn;
  /*p28.ATEJ*/ bool NEW_LINE; // drives sprite count reset, must be new line
  /*p28.ABAK*/ bool NEW_LINE2;
  /*p28.BYVA*/ bool NEW_LINE3;
  /*p29.DYBA*/ bool NEW_LINE4;



  /*p28.WEFE*/ bool P10_Bn;

  /*p28.AWOH*/ bool CLK_2Mn;
  /*p28.ABAF*/ bool IN_LINEn;
  /*p28.GAVA*/ bool SCAN_CLK;

  /*p28.GARO*/ bool A0n;
  /*p28.WACU*/ bool A1n;
  /*p28.GOSE*/ bool A2n;
  /*p28.WAPE*/ bool A3n;
  /*p28.FEVU*/ bool A4n;
  /*p28.GERA*/ bool A5n;
  /*p28.WAXA*/ bool A6n;
  /*p28.FOBY*/ bool A7n;

  /*p28.FYKE*/ bool IDX_0n;
  /*p28.FUGU*/ bool IDX_1n;
  /*p28.FACO*/ bool IDX_2n;
  /*p28.FABY*/ bool IDX_3n;
  /*p28.GYKA*/ bool IDX_4n;
  /*p28.GYBU*/ bool IDX_5n;

  /*p28.WUWE*/ bool SCANZn;
  /*p28.GUSE*/ bool SCAN0n;
  /*p28.GEMA*/ bool SCAN1n;
  /*p28.FUTO*/ bool SCAN2n;
  /*p28.FAKU*/ bool SCAN3n;
  /*p28.GAMA*/ bool SCAN4n;
  /*p28.GOBY*/ bool SCAN5n;


  /*p28.XUTO*/ bool CPU_OAM_WR;

  /*p28.WEWU*/ bool OAM_A_CPU_RD;
  /*p28.WUKU*/ bool OAM_A_CPU_RDn;
  /*p28.ZONE*/ bool OAM_A_CS;
  /*p28.YLYC*/ bool OAM_A_CSn;

  /*p28.WUME*/ bool OAM_B_CPU_RD;
  /*p28.GUKO*/ bool OAM_B_CPU_RDn;
  /*p28.ZOFE*/ bool OAM_B_CS;
  /*p28.YNYC*/ bool OAM_B_CSn;

  /*p28.ANOM*/ bool SCAN_RSTn;
  /*p28.FETO*/ bool SCAN_DONE;
  /*p28.AZYB*/ bool SPRITE_COUNT_RSTn;

  /*p28.APAG*/ bool D_TO_OAMDn;
  /*p28.AZUL*/ bool D_TO_OAMD;
  /*p28.AZAR*/ bool VRAM_TO_OAMc;

  /*p28.MYNU*/ bool CPU_READ_MYSTERYn;
  /*p28.BESU*/ bool BESU;
  /*p28.AMAB*/ bool AMAB;
  /*p28.LEKO*/ bool CPU_READ_MYSTERY;

  /*p28.ANEL*/ bool IN_LINE_SYNC;

  /*p28.YFEL*/ bool SCAN0;
  /*p28.WEWY*/ bool SCAN1;
  /*p28.GOSO*/ bool SCAN2;
  /*p28.ELYN*/ bool SCAN3;
  /*p28.FAHA*/ bool SCAN4;
  /*p28.FONY*/ bool SCAN5;

  /*p28.ASEN*/ bool ASEN;
  /*p28.AJUJ*/ bool AJUJ;
  /*p28.XYNY*/ bool CPU_OAM_WR_CLK1;
  /*p28.WUJE*/ bool CPU_OAM_WR_CLK2;
  /*p28.XUPA*/ bool CPU_OAM_WR_CLK3;

  /*p28.WEFY*/ bool WEFY;
  /*p28.BOFE*/ bool FROM_CPU5n;
  /*p28.AJEP*/ bool AJEP;
  /*p28.XUJA*/ bool XUJA;
  /*p28.BOTA*/ bool BOTA;

  /*p28.WYDU*/ bool WYDU;
  /*p28.GECA*/ bool GECA;

  /*p28.GEFY*/ bool GEFY;

  /*p28.XUCA*/ bool XUCA; // unused
  /*p28.XECY*/ bool XECY; // unused
  /*p28.XUVA*/ bool XUVA; // unused


  /*p29.XONO*/ bool FLIP_X;



  /*p29.TACU*/ bool TACU;
  /*p29.XYSO*/ bool XYSO;
  /*p29.TEXY*/ bool TEXY;
  /*p29.AVAP*/ bool SCAN_DONE_TRIG;
  /*p29.FEPO*/ bool OAM_SCAN;
  /*p29.WUTY*/ bool WUTY;
  /*p29.XEFY*/ bool XEFY;
  /*p29.PUCO*/ bool SPRITE_PIX_LATCH_B;
  /*p29.XADO*/ bool SPRITE_PIX_LATCH_A;
  /*p29.TUVO*/ bool TUVO;
  /*p29.TYFO*/ bool TYFO;
  /*p29.XOCE*/ bool CLK_XOCE;
  /*p29.CATU*/ bool IN_LINE;
  /*p29.DENY*/ bool DENY;
  /*p29.XYME*/ bool XYME;
  /*p29.GOWO*/ bool GOWO;
  /*p29.GUGY*/ bool GUGY;
  /*p29.DYDO*/ bool DYDO;
  /*p29.GYGY*/ bool GYGY;
  /*p29.GYMA*/ bool GYMA;
  /*p29.FAME*/ bool FAME;
  /*p29.FADO*/ bool FADO;
  /*p29.BUZA*/ bool STORE_SPRITE_IDX;

  /*p29.DEGE*/ bool SPRITE_DELTA0;
  /*p29.DABY*/ bool SPRITE_DELTA1;
  /*p29.DABU*/ bool SPRITE_DELTA2;
  /*p29.GYSA*/ bool SPRITE_DELTA3;
  /*p29.GACE*/ bool SPRITE_DELTA4;
  /*p29.GUVU*/ bool SPRITE_DELTA5;
  /*p29.GYDA*/ bool SPRITE_DELTA6;
  /*p29.GEWY*/ bool SPRITE_DELTA7;

  // sprite matcher clock lines
  /*p29.YFAG*/ bool YFAG;
  /*p29.GECY*/ bool GECY;
  /*p29.DOKU*/ bool DOKU;
  /*p29.XAHO*/ bool XAHO;
  /*p29.WOFO*/ bool WOFO;
  /*p29.CEXU*/ bool CEXU;
  /*p29.WEME*/ bool WEME;
  /*p29.CYLA*/ bool CYLA;
  /*p29.GAMY*/ bool GAMY;


  /*p29.ASYS*/ bool STORE1_CLKa;
  /*p29.AHOF*/ bool STORE1_CLKb;
  /*p29.BYVY*/ bool STORE1_CLKc;

  // sprite matcher reset lines
  /*p29.WUPA*/ bool WUPA;
  /*p29.GAFY*/ bool GAFY;
  /*p29.ZAPE*/ bool ZAPE;
  /*p29.WUNU*/ bool WUNU;
  /*p29.WUZO*/ bool WUZO;
  /*p29.DOSY*/ bool DOSY;
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
  /*p29.WUFA*/ bool WUFA;
  /*p29.FAKA*/ bool FAKA;

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



  /*p29.ABON*/ bool ABON;

  /*p29.EBOS*/ bool V0n;
  /*p29.DASA*/ bool V1n;
  /*p29.FUKY*/ bool V2n;
  /*p29.FUVE*/ bool V3n;
  /*p29.FEPU*/ bool V4n;
  /*p29.FOFA*/ bool V5n;
  /*p29.FEMO*/ bool V6n;
  /*p29.GUSU*/ bool V7n;

  /*p29.ERUC*/ bool YDIFF0;
  /*p29.ENEF*/ bool YDIFF1;
  /*p29.FECO*/ bool YDIFF2;
  /*p29.GYKY*/ bool YDIFF3;
  /*p29.GOPU*/ bool YDIFF4;
  /*p29.FUWA*/ bool YDIFF5;
  /*p29.GOJU*/ bool YDIFF6;
  /*p29.WUHU*/ bool YDIFF7;

  /*p29.ERUC*/ bool YDIFF_C0;
  /*p29.ENEF*/ bool YDIFF_C1;
  /*p29.FECO*/ bool YDIFF_C2;
  /*p29.GYKY*/ bool YDIFF_C3;
  /*p29.GOPU*/ bool YDIFF_C4;
  /*p29.FUWA*/ bool YDIFF_C5;
  /*p29.GOJU*/ bool YDIFF_C6;
  /*p29.WUHU*/ bool YDIFF_C7;

  /*p29.GOVU*/ bool GOVU;

  /*p29.WOTA*/ bool SPR_MATCH_Yn;
  /*p29.GESE*/ bool SPR_MATCH_Y;


  /*p29.ALES*/ bool LINE_144n;
  /*p29.ABOV*/ bool IN_LINEb;
  /*p29.WOSU*/ bool CLK_WOSU;
  /*p29.WOJO*/ bool WOJO;
  /*p29.CENO*/ bool CENO;
  /*p29.CEHA*/ bool CEHA;
  /*p29.CARE*/ bool STORE_ENn;
  /*p29.DYTY*/ bool STORE_EN;
  /*p29.BALU*/ bool SCAN_RSTa;
  /*p29.BAGY*/ bool SCAN_RSTo;
  /*p29.BYBA*/ bool SCAN_DONE_SYNC1;
  /*p29.DOBA*/ bool SCAN_DONE_SYNC2;
  /*p29.BEBU*/ bool SCAN_DONE_TRIGn;

  /*p29.TOBU*/ bool TULY_SYNC1;
  /*p29.VONU*/ bool SPRITE_ABn;

  /*p29.TEPA*/ bool RENDERINGn;
  /*p29.TYNO*/ bool TYNO;
  /*p29.VUSA*/ bool VUSA;
  /*p29.SAKY*/ bool SAKY;
  /*p29.TYSO*/ bool TYSO;
  /*p29.TAME*/ bool TAME;
  /*p29.TOMA*/ bool TOMA_CLK;
  /*p29.TYTU*/ bool TYTU;
  /*p29.SYCU*/ bool SYCU;
  /*p29.TOPU*/ bool SPRITE_PIX_LATCH_Ao;
  /*p29.RACA*/ bool SPRITE_PIX_LATCH_Bo;
  /*p29.VYWA*/ bool SPRITE_PIX_LATCH_Ab;
  /*p29.WENY*/ bool SPRITE_PIX_LATCH_An;
  /*p29.PEBY*/ bool SPRITE_PIX_LATCH_Bb;
  /*p29.NYBE*/ bool SPRITE_PIX_LATCH_Bn;
  /*p29.SEBA*/ bool SEBA;
  /*p29.TOXE*/ bool TOXE0;
  /*p29.TULY*/ bool TULY1;
  /*p29.TESE*/ bool TESE2;

  /*p29.BYJO*/ bool BYJO;
  /*p29.AZEM*/ bool AZEM;
  /*p29.AROR*/ bool MATCH_EN;
  /*p29.XAGE*/ bool XAGE;
  /*p29.YLOZ*/ bool YLOZ;
  /*p29.DEGO*/ bool DEGO;
  /*p29.DYDU*/ bool STORE1_MATCHn;
  /*p29.YGEM*/ bool YGEM;
  /*p29.EFYL*/ bool EFYL;
  /*p29.DYKA*/ bool DYKA;
  /*p29.YBEZ*/ bool YBEZ;
  /*p29.EGOM*/ bool EGOM;
  /*p29.FEFY*/ bool FEFY;
  /*p29.FOVE*/ bool FOVE;

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
  /*p29.DUBU*/ bool DUBU;
  /*p29.GORO*/ bool GORO;
  /*p29.GUKY*/ bool GUKY;
  /*p29.WACY*/ bool WACY;
  /*p29.FEVE*/ bool FEVE;
  /*p29.WOHU*/ bool WOHU;
  /*p29.GAKE*/ bool GAKE;
  /*p29.FOKO*/ bool FOKO;
  /*p29.EFEV*/ bool EFEV;

  /*p29.WUKY*/ bool FLIP_Y;
  /*p29.WAGO*/ bool WAGO;

  /*p29.CYVU*/ bool SPRITE_Y0;
  /*p29.BORE*/ bool SPRITE_Y1;
  /*p29.BUVY*/ bool SPRITE_Y2;

  /*p29.XUQU*/ bool SPRITE_AB;

  /*p29.FUFO*/ bool LCDC_SPSIZEn;
  /*p29.GEJY*/ bool GEJY;
  /*p29.FAMU*/ bool FAMU;

  /*p29.BAKY*/ bool SPRITES_FULL;
  /*p29.CAKE*/ bool SPRITE_COUNT_CLK;

  /*p29.EDEN*/ bool SPRITE_COUNT0n;
  /*p29.FYCU*/ bool SPRITE_COUNT0b;
  /*p29.CYPY*/ bool SPRITE_COUNT1n;
  /*p29.FONE*/ bool SPRITE_COUNT1b;
  /*p29.CAPE*/ bool SPRITE_COUNT2n;
  /*p29.EKUD*/ bool SPRITE_COUNT2b;
  /*p29.CAXU*/ bool SPRITE_COUNT3n;
  /*p29.ELYG*/ bool SPRITE_COUNT3b;

  /*p29.CUVA*/ bool STORE1_SEL;
  /*p29.BYBY*/ bool STORE1_CLK;
  /*p29.BUCO*/ bool STORE1_CLKn;

  /*p29.GEBU*/ bool STORE2_SEL;
  /*p29.WYXO*/ bool STORE2_CLK;
  /*p29.GYFO*/ bool GYFO;
  /*p29.CACU*/ bool CACU;
  /*p29.BUZY*/ bool BUZY;
  /*p29.FUKE*/ bool FUKE;

  /*p29.FOCO*/ bool STORE3_SEL;
  /*p29.GUVE*/ bool STORE3_CLK;

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

  /*p29.DOGU*/ bool STORE9_SEL;
  /*p29.CATO*/ bool STORE9_CLK;





  /*p29.WEKA*/ bool WEKA;
  /*p29.GYVO*/ bool GYVO;
  /*p29.GUSA*/ bool GUSA;
  /*p29.BUKA*/ bool BUKA;
  /*p29.DECU*/ bool DECU;
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

  /*p30.BOXA*/ bool BOXA;
  /*p30.BUNA*/ bool BUNA;
  /*p30.BULU*/ bool BULU;
  /*p30.BECA*/ bool BECA;
  /*p30.BYHU*/ bool BYHU;
  /*p30.BUHE*/ bool BUHE;
  /*p30.YKUK*/ bool YKUK;
  /*p30.YLOV*/ bool YLOV;
  /*p30.XAZY*/ bool XAZY;
  /*p30.XOSY*/ bool XOSY;

  /*p30.XUFO*/ bool XUFO;
  /*p30.XUTE*/ bool XUTE;
  /*p30.XOTU*/ bool XOTU;
  /*p30.XYFE*/ bool XYFE;
  /*p30.YZOR*/ bool YZOR;
  /*p30.YBER*/ bool YBER;
  /*p30.DEWU*/ bool DEWU;
  /*p30.CANA*/ bool CANA;
  /*p30.DYSY*/ bool DYSY;
  /*p30.FOFO*/ bool FOFO;

  //----------
  // Sprite store 0

  /*p29.DEZO*/ bool STORE0_SEL;
  /*p29.CEMY*/ bool STORE0_CLK;
  /*p29.DYHU*/ bool STORE0_CLKn;
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

  /*p31.ZOGY*/ bool STORE0_MATCH0;
  /*p31.ZEBA*/ bool STORE0_MATCH1;
  /*p31.ZAHA*/ bool STORE0_MATCH2;
  /*p31.ZOKY*/ bool STORE0_MATCH3;
  /*p31.WOJU*/ bool STORE0_MATCH4;
  /*p31.YFUN*/ bool STORE0_MATCH5;
  /*p31.WYZA*/ bool STORE0_MATCH6;
  /*p31.YPUK*/ bool STORE0_MATCH7;

  /*p29.YDUG*/ bool STORE0_MATCHn;
  /*p29.WEFU*/ bool SPRITE0_MATCH_OUT;
  /*p29.GEZE*/ bool SPRITE0_MATCH_OUTn;
  /*p29.FURO*/ bool SPRITE0_GET;
  /*p29.GUVA*/ bool SPRITE0_GETn;
  /*p29.EBOJ*/ bool SPRITE0_GET_SYNCn;

  /*p29.DYWE*/ bool STORE0_RST;
  /*p29.DYNA*/ bool STORE0_RSTn;

  /*p31.ZAKO*/ bool STORE0_MATCHA;
  /*p31.XEBA*/ bool STORE0_MATCHB;

  //----------

  /*p31.YWOS*/ bool YWOS;
  /*p31.DAJE*/ bool DAJE;
  /*p31.CYVY*/ bool STORE1_MATCHB;
  /*p31.YWAP*/ bool YWAP;
  /*p31.YKOK*/ bool YKOK;
  /*p31.DAMA*/ bool DAMA;
  /*p31.YTUB*/ bool YTUB;
  /*p31.COGY*/ bool COGY;
  /*p31.CEHU*/ bool CEHU;
  /*p31.ZURE*/ bool ZURE;
  /*p31.CYCO*/ bool CYCO;
  /*p31.EWAM*/ bool STORE1_MATCHA;
  /*p31.YDOT*/ bool YDOT;
  /*p31.YNAZ*/ bool YNAZ;
  /*p31.FEHA*/ bool FEHA;
  /*p31.YLEV*/ bool YLEV;
  /*p31.FYMA*/ bool FYMA;
  /*p31.EKES*/ bool EKES;

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

  /*p31.YVAP*/ bool YVAP;
  /*p31.XENY*/ bool XENY;
  /*p31.XAVU*/ bool XAVU;
  /*p31.XEVA*/ bool XEVA;
  /*p31.YHOK*/ bool YHOK;
  /*p31.YCAH*/ bool YCAH;
  /*p31.YDAJ*/ bool YDAJ;
  /*p31.YVUZ*/ bool YVUZ;

  /*p31.FAZU*/ bool FAZU;
  /*p31.FAXE*/ bool FAXE;
  /*p31.EXUK*/ bool EXUK;
  /*p31.FEDE*/ bool FEDE;
  /*p31.ERAZ*/ bool ERAZ;
  /*p31.EPUM*/ bool EPUM;
  /*p31.EROL*/ bool EROL;
  /*p31.EHYN*/ bool EHYN;

  /*p31.EJOT*/ bool EJOT;
  /*p31.ESAJ*/ bool ESAJ;
  /*p31.DUCU*/ bool DUCU;
  /*p31.EWUD*/ bool EWUD;
  /*p31.DUSE*/ bool DUSE;
  /*p31.DAGU*/ bool DAGU;
  /*p31.DYZE*/ bool DYZE;
  /*p31.DESO*/ bool DESO;

  /*p31.DANY*/ bool STORE1_X0;
  /*p31.DUKO*/ bool STORE1_X1;
  /*p31.DESU*/ bool STORE1_X2;
  /*p31.DAZO*/ bool STORE1_X3;
  /*p31.DAKE*/ bool STORE1_X4;
  /*p31.CESO*/ bool STORE1_X5;
  /*p31.DYFU*/ bool STORE1_X6;
  /*p31.CUSY*/ bool STORE1_X7;

  /*p31.EDYM*/ bool STORE1_MATCH0;
  /*p31.EMYB*/ bool STORE1_MATCH1;
  /*p31.EBEF*/ bool STORE1_MATCH2;
  /*p31.EWOK*/ bool STORE1_MATCH3;
  /*p31.COLA*/ bool STORE1_MATCH4;
  /*p31.BOBA*/ bool STORE1_MATCH5;
  /*p31.COLU*/ bool STORE1_MATCH6;
  /*p31.BAHU*/ bool STORE1_MATCH7;

  /*p31.ZOLY*/ bool ZOLY;
  /*p31.ZOGO*/ bool ZOGO;
  /*p31.ZECU*/ bool ZECU;
  /*p31.ZESA*/ bool ZESA;
  /*p31.YCOL*/ bool YCOL;
  /*p31.YRAC*/ bool YRAC;
  /*p31.YMEM*/ bool YMEM;
  /*p31.YVAG*/ bool YVAG;

  /*p31.ZARE*/ bool ZARE;
  /*p31.ZEMU*/ bool ZEMU;
  /*p31.ZYGO*/ bool ZYGO;
  /*p31.ZUZY*/ bool ZUZY;
  /*p31.XOSU*/ bool XOSU;
  /*p31.ZUVU*/ bool ZUVU;
  /*p31.XUCO*/ bool XUCO;
  /*p31.ZULO*/ bool ZULO;

  /*p31.YBED*/ bool YBED;
  /*p31.ZALA*/ bool ZALA;
  /*p31.WYDE*/ bool WYDE;
  /*p31.XEPA*/ bool XEPA;
  /*p31.WEDU*/ bool WEDU;
  /*p31.YGAJ*/ bool YGAJ;
  /*p31.ZYJO*/ bool ZYJO;
  /*p31.XURY*/ bool XURY;

  /*p31.ZYKU*/ bool ZYKU;
  /*p31.ZYPU*/ bool ZYPU;
  /*p31.XAHA*/ bool XAHA;
  /*p31.ZEFE*/ bool ZEFE;
  /*p31.XEJU*/ bool XEJU;
  /*p31.ZATE*/ bool ZATE;
  /*p31.ZAKU*/ bool ZAKU;
  /*p31.YBOX*/ bool YBOX;

  /*p31.EZUF*/ bool EZUF;
  /*p31.ENAD*/ bool ENAD;
  /*p31.EBOW*/ bool EBOW;
  /*p31.FYCA*/ bool FYCA;
  /*p31.GAVY*/ bool GAVY;
  /*p31.GYPU*/ bool GYPU;
  /*p31.GADY*/ bool GADY;
  /*p31.GAZA*/ bool GAZA;

  /*p31.DUZE*/ bool DUZE;
  /*p31.DAGA*/ bool DAGA;
  /*p31.DAWU*/ bool DAWU;
  /*p31.EJAW*/ bool EJAW;
  /*p31.GOHO*/ bool GOHO;
  /*p31.GASU*/ bool GASU;
  /*p31.GABU*/ bool GABU;
  /*p31.GAFE*/ bool GAFE;

  /*p31.YPOD*/ bool YPOD;
  /*p31.YROP*/ bool YROP;
  /*p31.YNEP*/ bool YNEP;
  /*p31.YZOF*/ bool YZOF;
  /*p31.XUVY*/ bool XUVY;
  /*p31.XERE*/ bool XERE;
  /*p31.XUZO*/ bool XUZO;
  /*p31.XEXA*/ bool XEXA;

  /*p31.ZYWU*/ bool ZYWU;
  /*p31.ZUZA*/ bool ZUZA;
  /*p31.ZEJO*/ bool ZEJO;
  /*p31.ZEDA*/ bool ZEDA;
  /*p31.YMAM*/ bool YMAM;
  /*p31.YTYP*/ bool YTYP;
  /*p31.YFOP*/ bool YFOP;
  /*p31.YVAC*/ bool YVAC;

  /*p31.CYWE*/ bool CYWE;
  /*p31.DYBY*/ bool DYBY;
  /*p31.DURY*/ bool DURY;
  /*p31.CUVY*/ bool CUVY;
  /*p31.FUSA*/ bool FUSA;
  /*p31.FAXA*/ bool FAXA;
  /*p31.FOZY*/ bool FOZY;
  /*p31.FESY*/ bool FESY;

  /*p31.BAZY*/ bool BAZY;
  /*p31.CYLE*/ bool CYLE;
  /*p31.CEVA*/ bool CEVA;
  /*p31.BUMY*/ bool BUMY;
  /*p31.GUZO*/ bool GUZO;
  /*p31.GOLA*/ bool GOLA;
  /*p31.GEVE*/ bool GEVE;
  /*p31.GUDE*/ bool GUDE;

  /*p31.DUHY*/ bool DUHY;
  /*p31.EJUF*/ bool EJUF;
  /*p31.ENOR*/ bool ENOR;
  /*p31.DEPY*/ bool DEPY;
  /*p31.FOKA*/ bool FOKA;
  /*p31.FYTY*/ bool FYTY;
  /*p31.FUBY*/ bool FUBY;
  /*p31.GOXU*/ bool GOXU;

  /*p31.CEKO*/ bool CEKO;
  /*p31.DETY*/ bool DETY;
  /*p31.DOZO*/ bool DOZO;
  /*p31.CONY*/ bool CONY;
  /*p31.FUZU*/ bool FUZU;
  /*p31.FESO*/ bool FESO;
  /*p31.FOKY*/ bool FOKY;
  /*p31.FYVA*/ bool FYVA;
};

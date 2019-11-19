#pragma once

struct Gameboy;

struct P31_SpriteXMatchers {
public:

  static void tick(const Gameboy& a, const Gameboy& b, Gameboy& c);

  /*p31.XEBA*/ bool SP_MATCH_0A;
  /*p31.ZAKO*/ bool SP_MATCH_0B;

  /*p31.YWOS*/ union { bool YWOS; bool SP_MATCH_1A; };
  /*p31.DAJE*/ union { bool DAJE; bool SP_MATCH_2A; };
  /*p31.CYVY*/ union { bool CYVY; bool SP_MATCH_3A; };
  /*p31.YWAP*/ union { bool YWAP; bool SP_MATCH_4A; };
  /*p31.YKOK*/ union { bool YKOK; bool SP_MATCH_5A; };
  /*p31.DAMA*/ union { bool DAMA; bool SP_MATCH_6A; };
  /*p31.YTUB*/ union { bool YTUB; bool SP_MATCH_7A; };
  /*p31.COGY*/ union { bool COGY; bool SP_MATCH_8A; };
  /*p31.CEHU*/ union { bool CEHU; bool SP_MATCH_9A; };
  /*p31.ZURE*/ union { bool ZURE; bool SP_MATCH_1B; };
  /*p31.CYCO*/ union { bool CYCO; bool SP_MATCH_2B; };
  /*p31.EWAM*/ union { bool EWAM; bool SP_MATCH_3B; };
  /*p31.YDOT*/ union { bool YDOT; bool SP_MATCH_4B; };
  /*p31.YNAZ*/ union { bool YNAZ; bool SP_MATCH_5B; };
  /*p31.FEHA*/ union { bool FEHA; bool SP_MATCH_6B; };
  /*p31.YLEV*/ union { bool YLEV; bool SP_MATCH_7B; };
  /*p31.FYMA*/ union { bool FYMA; bool SP_MATCH_8B; };
  /*p31.EKES*/ union { bool EKES; bool SP_MATCH_9B; };

  /*p31.XYKY*/ bool OAM_A_LATCH0;
  /*p31.YRUM*/ bool OAM_A_LATCH1;
  /*p31.YSEX*/ bool OAM_A_LATCH2;
  /*p31.YVEL*/ bool OAM_A_LATCH3;
  /*p31.WYNO*/ bool OAM_A_LATCH4;
  /*p31.CYRA*/ bool OAM_A_LATCH5;
  /*p31.ZUVE*/ bool OAM_A_LATCH6;
  /*p31.ECED*/ bool OAM_A_LATCH7;

  /*p31.XEGA*/ bool XEGA;

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

  /*p31.XEPE*/ bool M0X0;
  /*p31.YLAH*/ bool M0X1;
  /*p31.ZOLA*/ bool M0X2;
  /*p31.ZULU*/ bool M0X3;
  /*p31.WELO*/ bool M0X4;
  /*p31.XUNY*/ bool M0X5;
  /*p31.WOTE*/ bool M0X6;
  /*p31.XAKO*/ bool M0X7;

  /*p31.ZOGY*/ bool MATCHER0_MATCH0;
  /*p31.ZEBA*/ bool MATCHER0_MATCH1;
  /*p31.ZAHA*/ bool MATCHER0_MATCH2;
  /*p31.ZOKY*/ bool MATCHER0_MATCH3;
  /*p31.WOJU*/ bool MATCHER0_MATCH4;
  /*p31.YFUN*/ bool MATCHER0_MATCH5;
  /*p31.WYZA*/ bool MATCHER0_MATCH6;
  /*p31.YPUK*/ bool MATCHER0_MATCH7;

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

  /*p31.DAKE*/ bool DAKE;
  /*p31.CESO*/ bool CESO;
  /*p31.DYFU*/ bool DYFU;
  /*p31.CUSY*/ bool CUSY;
  /*p31.DANY*/ bool DANY;
  /*p31.DUKO*/ bool DUKO;
  /*p31.DESU*/ bool DESU;
  /*p31.DAZO*/ bool DAZO;

  /*p31.COLA*/ bool COLA;
  /*p31.BOBA*/ bool BOBA;
  /*p31.COLU*/ bool COLU;
  /*p31.BAHU*/ bool BAHU;
  /*p31.EDYM*/ bool EDYM;
  /*p31.EMYB*/ bool EMYB;
  /*p31.EBEF*/ bool EBEF;
  /*p31.EWOK*/ bool EWOK;

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

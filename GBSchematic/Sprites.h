#pragma once
#include "Schematics.h"

struct Sprites {
public:

  bool SPR_MATCH;
  bool FEPO;
  bool ABEZ;
  bool CEHA;
  bool XYSO;
  bool XOCE;
  bool DYTY;
  bool BUZA;
  bool AVAP;
  bool DEGE;
  bool DABY;
  bool DABU;
  bool GYSA;
  bool TACU;
  bool TEXY;
  bool TUVO;
  bool WUTY; // used as a clock
  bool XUPY;
  bool LAPE;
  bool XONO;
  bool XADO;
  bool PUCO;
  bool XEFY;

  //----------
  // outputs

  // sprite hit flags
  bool FADO;
  bool DENY;
  bool GUGY;
  bool XYME;
  bool GYGY;
  bool GOWO;
  bool GYMA;
  bool FAME;
  bool DYDO;
  bool FURO;

  // sprite hit flags 2
  bool DOSY;
  bool WUZO;
  bool GAFY;
  bool XAHO;
  bool EJAD;
  bool WUNU;
  bool WUPA;
  bool GAMY;
  bool DOKU;
  bool DYNA;

  // sprite hit flags or something
  bool CACU; bool BUZY; bool FUKE;
  bool ZAPE; bool WUSE; bool ZURU;
  bool FEFO; bool GECY; bool WABE;
  bool FEKA; bool XYHA; bool YFAG;
  bool CEXU; bool AKOL; bool BYMY;
  bool FUXU; bool ENOB; bool GENY;
  bool WEME; bool WUFA; bool FAKA;
  bool CYLA; bool DYMO; bool BUCY;
  bool WOFO; bool WYLU; bool EWOT;
  bool ASYS; bool AHOF; bool BYVY;

  //----------
  // outputs

  bool AMAB;
  bool ATEJ;
  bool WEFE;
  bool ACYL;
  bool ANOM;
  bool AZYB;
  bool BESU;
  bool BYVA;
  bool WUME;
  bool WEWU;
  bool FETO;
  bool LEKO;

  //----------
  // outputs

  bool XEBA, ZAKO;
  bool YWOS, ZURE;
  bool DAJE, CYCO;
  bool CYVY, EWAM;
  bool YWAP, YDOT;
  bool YKOK, YNAZ;
  bool DAMA, FEHA;
  bool YTUB, YLEV;
  bool COGY, FYMA;
  bool CEHU, EKES;

  void tick();

  void tick_matcher();
  void tick_control();

  // note, these use !Q as their output signal
  reg GOMO, DEPO;

  reg WUVU;

private:

  // sprite scan counter
  reg YFEL;
  reg WEWY;
  reg GOSO;
  reg ELYN;
  reg FAHA;
  reg FONY;

  reg ANEL;
  reg XECY;
  reg XUVA;

  //----------
  // registers

  // oam_a_d latch
  reg WYNO, CYRA, ZUVE, ECED, XYKY, YRUM, YSEX, YVEL;

  reg BAXO, YZOS; // note, these use !Q as their output signal
  reg YLOR, ZYTY, ZYVE, ZEZY;

  // match registers
  reg WELO, XUNY, WOTE, XAKO, XEPE, YLAH, ZOLA, ZULU;
  reg XOMY, WUHA, WYNA, WECO, XOLY, XYBA, XABE, XEKA;
  reg FAZU, FAXE, EXUK, FEDE, ERAZ, EPUM, EROL, EHYN;
  reg DAKE, CESO, DYFU, CUSY, DANY, DUKO, DESU, DAZO;
  reg ZOLY, ZOGO, ZECU, ZESA, YCOL, YRAC, YMEM, YVAG;
  reg YBED, ZALA, WYDE, XEPA, WEDU, YGAJ, ZYJO, XURY;
  reg EZUF, ENAD, EBOW, FYCA, GAVY, GYPU, GADY, GAZA;
  reg YPOD, YROP, YNEP, YZOF, XUVY, XERE, XUZO, XEXA;
  reg CYWE, DYBY, DURY, CUVY, FUSA, FAXA, FOZY, FESY;
  reg DUHY, EJUF, ENOR, DEPY, FOKA, FYTY, FUBY, GOXU;

  //----------
  // registers

  // sprite hit regs
  reg FONO;
  reg EXUQ;
  reg WAPO;
  reg WOMY;
  reg WAFY;
  reg XUDY;
  reg GOTA;
  reg EGAV;
  reg CEDY;
  reg EBOJ;

  // sprite y comparator reg?


  reg WOSU;
  reg CENO;
  reg CATU;
  reg BYBA;
  reg DOBA;


  reg TYFO;
  reg SEBA;
  reg TOXE;
  reg TULY;
  reg TESE;

  // Sprite Y Comparator

  reg YCEB; // oam_b latch
  reg ZUCA;
  reg WONE;
  reg ZAXE;
  reg XAFU;
  reg YSES;
  reg ZECA;
  reg YDYV;

  reg XEGU; // oam_b temp reg?
  reg YJEX;
  reg XYJU;
  reg YBOG;
  reg WYSO;
  reg XOTE;
  reg YZAB;
  reg XUSO;

  reg TOBU;
  reg VONU;

  reg DEZY;
  reg BESE; // "Count from 0 to A"
  reg CUXY;
  reg BEGO;
  reg DYBE;
};

extern Sprites spr;

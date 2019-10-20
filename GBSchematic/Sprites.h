#pragma once
#include "Schematics.h"

struct Resets;
struct OAM;
struct AddressDecoder;
struct Window;
struct Vram;

struct Sprites {
public:

  void tick(OAM& oam, const Resets& rst, const AddressDecoder& dec, const Vram& vram);
  void tick_control(const OAM& oam, const Resets& rst, const Window& win, Vram& vram);

  void tick_matcher(const OAM& oam);
  void tick_spritestore();
  void tick_paletteshifter();
  void tick_pixelshifter(const Vram& vram);


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

  bool ATEJ; // this is a clock that controls whether the x position increments or not

  bool AMAB;
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

  // note, these use !Q as their output signal
  reg GOMO, DEPO;

  reg WUVU;

  //----------
  // outputs

  bool WEZA;
  bool WUCO;
  bool WYDA;
  bool ZYSU;
  bool WYSE;
  bool WUZY;

  bool CUCA;
  bool CEGA;
  bool CUCU;
  bool WENU;

private:

  bool COTA;

  // these are probably named wrong and are actually the oam address lines?
  bool OAM_A_A7;
  bool OAM_A_A6;
  bool OAM_A_A5;
  bool OAM_A_A4;
  bool OAM_A_A3;
  bool OAM_A_A2;

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

  reg XECU;
  reg YDUF;
  reg XOBE;
  reg ZUZE;
  reg XEDY;
  reg XADU;

  // Sprite store
  reg AXUV, BADA, APEV, BADO, BEXY, BYHE, AFYM, AZAP, AFUT, AFYX;
  reg GECU, FOXY, GOHU, FOGO, GACY, GABO, ZUBE, ZUMY, ZEXO, ZAFU;
  reg XYNU, XEGE, XABO, WANU, XEFE, XAVE, CUMU, CAPO, CONO, CAJU;
  reg FUZO, GESY, FYSU, FEFA, GYNO, GULE, XYGO, XYNA, XAKU, YGUM;
  reg CAJY, CUZA, COMA, CUFA, CEBO, CADU, ABUG, AMES, ABOP, AROF;
  reg EKAP, ETAV, EBEX, GORU, ETYM, EKOP, ANED, ACEP, ABUX, ABEG;
  reg DAFU, DEBA, DUHA, DUNY, DESE, DEVY, ZURY, ZURO, ZENE, ZYLU;
  reg BOXA, BUNA, BULU, BECA, BYHU, BUHE, YKUK, YLOV, XAZY, XOSY;
  reg XUFO, XUTE, XOTU, XYFE, YZOR, YBER, DEWU, CANA, DYSY, FOFO;
  reg YGUS, YSOK, YZEP, WYTE, ZONY, YWAK, FYHY, GYHO, BOZU, CUFO;

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

  //----------
  // registers

  reg RUGO;
  reg SATA;
  reg ROSA;
  reg SOMY;
  reg PALU;
  reg NUKE;
  reg MODA;
  reg LYME;


  reg PUDU;
  reg RAMU;
  reg SELE;
  reg SAJA;
  reg MYTU;
  reg MOFO;
  reg SUNY;
  reg SUTO;
  reg PEFO;
  reg REWO;
  reg RYDU;
  reg SEGA;
  reg PEBA;
  reg ROKA;
  reg RAMA;
  reg SEMO;

  reg NURO;
  reg MASO;
  reg LEFE;
  reg LESU;
  reg WYHO;
  reg WORA;
  reg VAFO;
  reg WUFY;

  reg NYLU;
  reg PEFU;
  reg NATY;
  reg PYJO;
  reg VARE;
  reg WEBA;
  reg VANU;
  reg VUPY;
};

extern Sprites spr;

#pragma once

struct P29_SpriteControl {
  bool TACU;
  bool XYSO;
  bool TEXY;
  bool AVAP;
  bool WUVU;
  bool FEPO;
  bool WUTY;
  bool XEFY;
  bool PUCO;
  bool XADO;
  union { bool XONO; bool FLIP_X; };

  bool TUVO;
  bool TYFO;
  bool XOCE;
  bool XUPY;
  bool CATU;
  bool ABEZ;

  bool DENY;
  bool XYME;
  bool GOWO;
  bool GUGY;
  bool DYDO;
  bool GYGY;
  bool GYMA;
  bool FAME;
  bool FADO;
  bool FURO;
  bool BUZA;
  bool DABY;
  bool DABU;
  bool DEGE;
  bool GYSA;

  // sprite matcher clock lines
  bool FUXU,YFAG,GECY,DOKU,XAHO,WOFO,CEXU,WEME,CYLA,GAMY;

  // sprite matcher reset lines
  bool DYNA,WUPA,GAFY,ASYS,ZAPE,WUNU,WUZO,DOSY,EJAD,CACU;

  // sprite store clock lines
  bool AKOL,BYMY,WUSE,ZURU,WYLU,EWOT,FEFO,WABE,BYVY,AHOF;
  bool DYMO,BUCY,FEKA,XYHA,BUZY,FUKE,WUFA,FAKA,GENY,ENOB;
};



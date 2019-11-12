#pragma once

struct P29_SpriteControl {
public:
  static void tick(const Gameboy& a, const Gameboy& b, Gameboy& c);

  union { bool TACU; };
  union { bool XYSO; };
  union { bool TEXY; };
  union { bool AVAP; };
  union { bool WUVU; };
  union { bool FEPO; };
  union { bool WUTY; };
  union { bool XEFY; };
  union { bool PUCO; };
  union { bool XADO; };
  union { bool XONO; bool FLIP_X; };

  union { bool TUVO; };
  union { bool TYFO; };
  union { bool XOCE; };
  union { bool XUPY; };
  union { bool CATU; };
  union { bool ABEZ; };

  union { bool DENY; };
  union { bool XYME; };
  union { bool GOWO; };
  union { bool GUGY; };
  union { bool DYDO; };
  union { bool GYGY; };
  union { bool GYMA; };
  union { bool FAME; };
  union { bool FADO; };
  union { bool FURO; };
  union { bool BUZA; };
  union { bool DABY; };
  union { bool DABU; };
  union { bool DEGE; };
  union { bool GYSA; };
  union { bool GESE; bool SPR_MATCH; };

  // sprite matcher clock lines
  bool FUXU,YFAG,GECY,DOKU,XAHO,WOFO,CEXU,WEME,CYLA,GAMY;

  // sprite matcher reset lines
  bool DYNA,WUPA,GAFY,ASYS,ZAPE,WUNU,WUZO,DOSY,EJAD,CACU;

  // sprite store clock lines
  bool AKOL,BYMY,WUSE,ZURU,WYLU,EWOT,FEFO,WABE,BYVY,AHOF;
  bool DYMO,BUCY,FEKA,XYHA,BUZY,FUKE,WUFA,FAKA,GENY,ENOB;

private:

  bool YCEB,ZUCA,WONE,ZAXE,XAFU,YSES,ZECA,YDYV;
  bool XELE,YPON,XUVO,ZYSA,YWEG,XABU,YTUX,YFAP;
  bool YWOK;
  bool XEGU,YJEX,XYJU,YBOG,WYSO,XOTE,YZAB,XUSO;
  bool ABON;
  bool FUGY,GAVO,WYGA,WUNE,GOTU,GEGU,XEHE;
  bool EBOS,DASA,FUKY,FUVE,FEPU,FOFA,FEMO,GUSU;

  bool ERUC_S,ENEF_S,FECO_S,GYKY_S,GOPU_S,FUWA_S,GOJU_S,WUHU_S;
  bool ERUC_C,ENEF_C,FECO_C,GYKY_C,GOPU_C,FUWA_C,GOJU_C,WUHU_C;

  bool GACE,GUVU,GYDA,GEWY;
  bool GOVU,WOTA;

  bool XYVA,XOTA,XYFY,ALES,ABOV,WOSU,WOJO;
  bool CENO,CEHA,CARE,DYTY;
  bool BALU,BAGY,BYBA,DOBA,BEBU;

  bool TOBU,VONU;
  bool LAPE,TEPA;
  bool TYNO,VUSA;
  bool SAKY,TYSO;
  bool TAME,TOMA;
  bool TYTU,SYCU,TOPU,RACA,VYWA,WENY,PEBY,NYBE;
  bool SEBA,TOXE,TULY,TESE;

  bool BYJO,AZEM,AROR;
  bool XAGE,YLOZ,DEGO,DYDU,YDUG;
  bool YGEM,EFYL,DYKA,YBEZ,EGOM;
  bool FEFY,FOVE;
  bool WEFU,GEZE,GUVA;
  bool GAJA,FUMA,ENUT;
  bool GUPO,GEDE,EMOL;
  bool WEBO,WUTO,GYFY;
  bool WUNA,XYLA,GONO;
  bool GABA,WEJA,GEGA;
  bool WASE,WYLA,XOJA;
  bool GYTE,FAVO,GUTU;
  bool GEKE,GYGA,FOXA;
  bool GUZE;
  bool FONO,EXUQ,WAPO,WOMY,WAFY,XUDY,GOTA,EGAV,CEDY,EBOJ;
  bool DYBA;
  bool DUBU,GORO,GUKY,WACY,FEVE,WOHU,GAKE,FOKO,EFEV,DYWE;

  bool WUKY,WAGO,CYVU,BORE,BUVY,XUQU;
  bool BAXE,ARAS,AGAG,ABEM,DYSO;
  bool FUFO,GEJY,FAMU;

  bool BAKY,CAKE,EDEN,FYCU,CYPY,FONE,CAPE,EKUD,CAXU,ELYG;
  bool GEBU,WOMU,GUNA,FOCO,DEWY,DEZO,DOGU,CUGU,CUPE,CUVA;
  bool WYXO,XUJO,GAPE,GUVE,CAHO,CEMY,CATO,CADO,CECU,BYBY;
  bool GYFO,WEKA,GYVO,GUSA,BUKA,DYHU,DECU,BEDE,DUKE,BUCO;
  bool DEZY,BESE,CUXY,BEGO,DYBE;
};




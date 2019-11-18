#pragma once

struct Gameboy;

struct P31_SpriteXMatchers {
public:

  static void tick(const Gameboy& a, const Gameboy& b, Gameboy& c);

  union { bool GOMO; };
  union { bool DEPO; };

  union { bool XEBA; bool SP_MATCH_0A; };
  union { bool YWOS; bool SP_MATCH_1A; };
  union { bool DAJE; bool SP_MATCH_2A; };
  union { bool CYVY; bool SP_MATCH_3A; };
  union { bool YWAP; bool SP_MATCH_4A; };
  union { bool YKOK; bool SP_MATCH_5A; };
  union { bool DAMA; bool SP_MATCH_6A; };
  union { bool YTUB; bool SP_MATCH_7A; };
  union { bool COGY; bool SP_MATCH_8A; };
  union { bool CEHU; bool SP_MATCH_9A; };

  union { bool ZAKO; bool SP_MATCH_0B; };
  union { bool ZURE; bool SP_MATCH_1B; };
  union { bool CYCO; bool SP_MATCH_2B; };
  union { bool EWAM; bool SP_MATCH_3B; };
  union { bool YDOT; bool SP_MATCH_4B; };
  union { bool YNAZ; bool SP_MATCH_5B; };
  union { bool FEHA; bool SP_MATCH_6B; };
  union { bool YLEV; bool SP_MATCH_7B; };
  union { bool FYMA; bool SP_MATCH_8B; };
  union { bool EKES; bool SP_MATCH_9B; };

  bool BAXO;
  bool FLIP_Yn;

  bool WYNO,CYRA,ZUVE,ECED,XYKY,YRUM,YSEX,YVEL;
  bool XEGA;
  bool YLOR,ZYTY,ZYVE,ZEZY;
  bool COSE,AROP,XATU,BADY,ZAGO,ZOCY,YPUR,YVOK;

  bool WELO,XUNY,WOTE,XAKO,XEPE,YLAH,ZOLA,ZULU;
  bool WOJU,YFUN,WYZA,YPUK,ZOGY,ZEBA,ZAHA,ZOKY;
  bool XOMY,WUHA,WYNA,WECO,XOLY,XYBA,XABE,XEKA;
  bool YVAP,XENY,XAVU,XEVA,YHOK,YCAH,YDAJ,YVUZ;
  bool FAZU,FAXE,EXUK,FEDE,ERAZ,EPUM,EROL,EHYN;
  bool EJOT,ESAJ,DUCU,EWUD,DUSE,DAGU,DYZE,DESO;
  bool DAKE,CESO,DYFU,CUSY,DANY,DUKO,DESU,DAZO;
  bool COLA,BOBA,COLU,BAHU,EDYM,EMYB,EBEF,EWOK;
  bool ZOLY,ZOGO,ZECU,ZESA,YCOL,YRAC,YMEM,YVAG;
  bool ZARE,ZEMU,ZYGO,ZUZY,XOSU,ZUVU,XUCO,ZULO;
  bool YBED,ZALA,WYDE,XEPA,WEDU,YGAJ,ZYJO,XURY;
  bool ZYKU,ZYPU,XAHA,ZEFE,XEJU,ZATE,ZAKU,YBOX;
  bool EZUF,ENAD,EBOW,FYCA,GAVY,GYPU,GADY,GAZA;
  bool DUZE,DAGA,DAWU,EJAW,GOHO,GASU,GABU,GAFE;
  bool YPOD,YROP,YNEP,YZOF,XUVY,XERE,XUZO,XEXA;
  bool ZYWU,ZUZA,ZEJO,ZEDA,YMAM,YTYP,YFOP,YVAC;
  bool CYWE,DYBY,DURY,CUVY,FUSA,FAXA,FOZY,FESY;
  bool BAZY,CYLE,CEVA,BUMY,GUZO,GOLA,GEVE,GUDE;
  bool DUHY,EJUF,ENOR,DEPY,FOKA,FYTY,FUBY,GOXU;
  bool CEKO,DETY,DOZO,CONY,FUZU,FESO,FOKY,FYVA;
};

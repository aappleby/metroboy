// This file should contain the schematics as directly translated to C, no modifications or simplifications

#include "Schematics.h"

//----------
// inputs

extern bool FF40_D1;
extern bool FF40_D2;
extern bool P10_B;

extern bool CLK1;
extern bool CLK3;

extern bool CEHA;
extern bool XYMU;
extern bool BYVA;
extern bool WUTY;
extern bool WUME;
extern bool COTA;
extern bool TEXY;

extern bool TAVA;
extern bool TULY;
extern bool XYMU;

extern bool YZOS;
extern bool WENU;
extern bool CUCU;
extern bool CEGA;

extern bool XEBA;
extern bool ZAKO;
extern bool YWOS;
extern bool ZURE;
extern bool DAJE;
extern bool CYCO;
extern bool CYVY;
extern bool EWAM;
extern bool YWAP;
extern bool YDOT;
extern bool YKOK;
extern bool YNAZ;
extern bool DAMA;
extern bool FEHA;
extern bool YTUB;
extern bool YLEV;
extern bool COGY;
extern bool FYMA;
extern bool CEHU;
extern bool EKES;

//----------
// outputs

bool SPR_MATCH;
bool FEPO;

bool DEGE;
bool DABY;
bool DABU;
bool GYSA;

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

//----------
// mystery signals

//----------
// registers

// sprite hit regs
bool FONO;
bool EXUQ;
bool WAPO;
bool WOMY;
bool WAFY;
bool XUDY;
bool GOTA;
bool EGAV;
bool CEDY;
bool EBOJ;
bool WUTY_CLK;

// oam_b latch
bool YCEB;
bool ZUCA;
bool WONE;
bool ZAXE;
bool XAFU;
bool YSES;
bool ZECA;
bool YDYV;

// sprite y comparator reg?
bool XEGU;
bool YJEX;
bool XYJU;
bool YBOG;
bool WYSO;
bool XOTE;
bool YZAB;
bool XUSO;
bool YWOK_CLK;

bool TOBU;
bool VONU;
bool TAVA_CLK;

bool WUVU;
bool XOTA_CLK;

bool WOSU;
bool XYFY_CLK;

bool CENO;
bool CATU;
bool BYBA;
bool XUPY_CLK;

//-----------------------------------------------------------------------------

void tick_spritecontrol() {
  bool BYJO = not(CEHA);
  bool AZEM = and(BYJO, XYMU);
  bool AROR = and(AZEM, FF40_D1);

  bool XAGE = nand(AROR, YNAZ, YKOK);
  bool YLOZ = nand(AROR, ZURE, YWOS);
  bool DEGO = nand(AROR, EKES, CEHU);
  bool DYDU = nand(AROR, EWAM, CYVY);
  bool YDUG = nand(AROR, ZAKO, XEBA);

  bool YGEM = nand(AROR, YLEV, YTUB);
  bool EFYL = nand(AROR, FEHA, DAMA);
  bool DYKA = nand(AROR, CYCO, DAJE);
  bool YBEZ = nand(AROR, YDOT, YWAP);
  bool EGOM = nand(AROR, FYMA, COGY);

  bool FEFY = nand(XAGE, YLOZ, DEGO, DYDU, YDUG);
  bool FOVE = nand(YGEM, EFYL, DYKA, YBEZ, EGOM);

  FEPO = or(FEFY, FOVE);

  bool WEFU = not(YDUG);
  bool GEZE = or(WEFU, P10_B);
  bool GUVA = nor(YDUG, P10_B);

  bool GAJA = not(DYDU);
  bool FUMA = or(GAJA, GEZE);
  bool ENUT = nor(DYDU, GEZE);

  bool GUPO = not(DEGO);
  bool GEDE = or(GUPO, FUMA);
  bool EMOL = nor(DEGO, FUMA);

  bool WEBO = not(YLOZ);
  bool WUTO = or(WEBO, GEDE);
  bool GYFY = nor(YLOZ, GEDE);

  bool WUNA = not(XAGE);
  bool XYLA = or(WUNA, WUTO);
  bool GONO = nor(XAGE, WUTO);


  bool GABA = not(EGOM);
  bool WEJA = or(GABA, XYLA);
  bool GEGA = nor(EGOM, XYLA);

  bool WASE = not(YBEZ);
  bool WYLA = or(WASE, WEJA);
  bool XOJA = nor(YBEZ, WEJA);

  bool GYTE = not(DYKA);
  bool FAVO = or(GYTE, WYLA);
  bool GUTU = nor(DYKA, WYLA);

  bool GEKE = not(EFYL);
  bool GYGA = or(GEKE, FAVO);
  bool FOXA = nor(EFYL, FAVO);

  bool GUZE = nor(YGEM, GYGA);

  FADO = not(GUZE);
  DENY = not(FOXA);
  GUGY = not(GUTU);
  XYME = not(XOJA);
  GYGY = not(GEGA);
  GOWO = not(GONO);
  GYMA = not(GYFY);
  FAME = not(EMOL);
  DYDO = not(ENUT);
  FURO = not(GUVA);

  bool DYBA = not(BYVA);

  bool DUBU = or(DYBA, FONO);
  bool GORO = or(DYBA, EXUQ);
  bool GUKY = or(DYBA, WAPO);
  bool WACY = or(DYBA, WOMY);
  bool FEVE = or(DYBA, WAFY);
  bool WOHU = or(DYBA, XUDY);
  bool GAKE = or(DYBA, GOTA);
  bool FOKO = or(DYBA, EGAV);
  bool EFEV = or(DYBA, CEDY);
  bool DYWE = or(DYBA, EBOJ);

  DOSY = not(DUBU);
  WUZO = not(GORO);
  GAFY = not(GUKY);
  XAHO = not(WACY);
  EJAD = not(FEVE);
  WUNU = not(WOHU);
  WUPA = not(GAKE);
  GAMY = not(FOKO);
  DOKU = not(EFEV);
  DYNA = not(DYWE);

  // sprite y comparator

  // active low latch?
  if (CLK3) {
    YCEB = OAM_B_D1;
    ZUCA = OAM_B_D2;
    WONE = OAM_B_D3;
    ZAXE = OAM_B_D4;
    XAFU = OAM_B_D5;
    YSES = OAM_B_D6;
    ZECA = OAM_B_D7;
    YDYV = OAM_B_D0;
  }

  bool XELE = YCEB;
  bool YPON = ZUCA;
  bool XUVO = WONE;
  bool ZYSA = ZAXE;
  bool YWEG = XAFU;
  bool XABU = YSES;
  bool YTUX = ZECA;
  bool YFAP = YDYV;

  if (WUME) {
    D1 = XELE;
    D2 = YPON;
    D3 = XUVO;
    D4 = ZYSA;
    D5 = YWEG;
    D6 = XABU;
    D7 = YTUX;
    D0 = YFAP;
  }

  bool YWOK = not(COTA);
  bool ABON = not(TEXY);

  // only 7 of these?
  bool FUGY = not(!XEGU);
  bool GAVO = not(!YJEX);
  bool WYGA = not(!XYJU);
  bool WUNE = not(!YBOG);
  bool GOTU = not(!WYSO);
  bool GEGU = not(!XOTE);
  bool XEHE = not(!YZAB);

  if (ABON) {
    MA5 = FUGY;
    MA6 = GAVO;
    MA7 = WYGA;
    MA8 = WUNE;
    MA9 = GOTU;
    MA10 = GEGU;
    MA11 = XEHE;
  }

  bool WUKY = not(YZOS);
  bool WAGO = xor(WUKY, WENU);
  bool CYVU = xor(WUKY, CUCU);
  bool BORE = xor(WUKY, WENU);
  bool BUVY = xor(WUKY, CEGA);

  bool XUQU = not(!VONU);
  bool BAXE = not(CYVU);
  bool ARAS = not(BORE);
  bool AGAG = not(BUVY);
  bool ABEM = not(XUQU);
  bool DYSO = not(P10_B);

  if (ABON) {
    MA1 = BAXE;
    MA2 = ARAS;
    MA3 = AGAG;
    MA0 = ABEM;
    MA12 = DYSO;
  }

  bool EBOS = not(V0);
  bool DASA = not(V1);
  bool FUKY = not(V2);
  bool FUVE = not(V3);
  bool FEPU = not(V4);
  bool FOFA = not(V5);
  bool FEMO = not(V6);
  bool GUSU = not(V7);

  // FIXME totally wrong, probably a counter...
  bool ERUC = unk1(/*CIN=*/P10_B, /*COUT=*/0, EBOS, !XUSO);
  bool ENEF = unk1(/*CIN=*/0,     /*COUT=*/0, DASA, !XEGU);
  bool FECO = unk1(/*CIN=*/0,     /*COUT=*/0, FUKY, !YJEX);
  bool GYKY = unk1(/*CIN=*/0,     /*COUT=*/0, FUVE, !XYJU);
  bool GOPU = unk1(/*CIN=*/0,     /*COUT=*/0, FEPU, !YBOG);
  bool FUWA = unk1(/*CIN=*/0,     /*COUT=*/0, FOFA, !WYSO);
  bool GOJU = unk1(/*CIN=*/0,     /*COUT=*/0, FEMO, !XOTE);
  bool WUHU = unk1(/*CIN=*/0,     /*COUT=*/0, GUSU, !YZAB);

  DEGE = not(ERUC);
  DABY = not(ENEF);
  DABU = not(FECO);
  GYSA = not(GYKY);
  bool GACE = not(GOPU);
  bool GUVU = not(FUWA);
  bool GYDA = not(GOJU);
  bool GEWY = not(WUHU);
  
  bool GOVU = or(GYKY, FF40_D2);
  bool FUFO = not(FF40_D2);
  bool GEJY = amux2(!XUSO, FUFO, FF40_D2, WAGO);
  bool FAMU = not(GEJY);

  if (ABON) {
    MA4 = FAMU;
  }

  // FIXME
  bool WOTA = nand(GACE, GUVU, GYDA, GEWY, /*WUHU_COUT*/0, GOVU);
  bool GESE = not(WOTA);
  SPR_MATCH = GESE;

  bool XYVA = not(CLK1);
  bool XOTA = not(XYVA);

  //----------
  // registers

  bool WUTY_CLK_ = WUTY;
  if (WUTY_CLK && !WUTY_CLK_) {
    FONO = GUZE;
    EXUQ = DENY;
    WAPO = GUGY;
    WOMY = XYME;
    WAFY = GYGY;
    XUDY = GOWO;
    GOTA = GYMA;
    EGAV = FAME;
    CEDY = DYDO;
    EBOJ = FURO;
  }
  if (!BYVA) {
    FONO = 0;
    EXUQ = 0;
    WAPO = 0;
    WOMY = 0;
    WAFY = 0;
    XUDY = 0;
    GOTA = 0;
    EGAV = 0;
    CEDY = 0;
    EBOJ = 0;
  }
  WUTY_CLK = WUTY_CLK_;

  bool YWOK_CLK_ = YWOK;
  if (YWOK_CLK && !YWOK_CLK_) {
    XEGU = YCEB;
    YJEX = ZUCA;
    XYJU = WONE;
    YBOG = ZAXE;
    WYSO = XAFU;
    XOTE = YSES;
    YZAB = ZECA;
    XUSO = YDYV;
  }
  YWOK_CLK = YWOK_CLK_;

  bool TOBU_;
  bool VONU_;
  bool TAVA_CLK_ = TAVA;
  if (TAVA_CLK && !TAVA_CLK_) {
    TOBU_ = TULY;
    VONU_ = !TOBU;
  }
  if (!XYMU) {
    TOBU = 0;
    VONU = 0;
  }
  TOBU = TOBU_;
  VONU = VONU_;
  TAVA_CLK = TAVA_CLK_;
}